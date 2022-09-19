/*
 * This file is part of Hootenanny.
 *
 * Hootenanny is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * --------------------------------------------------------------------
 *
 * The following copyright notices are generated automatically. If you
 * have a new notice to add, please use the format:
 * " * @copyright Copyright ..."
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */

#include "AlphaShapeGenerator.h"

// Hoot
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/geometry/GeometryMerger.h>
#include <hoot/core/geometry/GeometryToElementConverter.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/StringUtils.h>

// GEOS
#include <geos/geom/GeometryFactory.h>

// Standard
#include <thread>

using namespace geos::geom;
using namespace std;

namespace hoot
{

AlphaShapeGenerator::AlphaShapeGenerator(const double alpha, const double buffer)
  : _alpha(alpha),
    _buffer(buffer),
    _manuallyCoverSmallPointClusters(true),
    _maxThreads(ConfigOptions().getCookieCutterAlphaShapeMaxThreads()),
    _working(true)
{
  LOG_VART(_alpha);
  LOG_VART(_buffer);
}

OsmMapPtr AlphaShapeGenerator::generateMap(OsmMapPtr inputMap)
{
  LOG_DEBUG("Generating map...");
  OsmMapWriterFactory::writeDebugMap(inputMap, className(), "alpha-shape-input-map");

  std::shared_ptr<Geometry> cutterShape = generateGeometry(inputMap);
  //  Would rather this be thrown than a warning logged, as the warning may go unoticed by
  //  clients who are expecting the alpha shape to be generated
  if (cutterShape->getArea() == 0.0)
    throw HootException("Alpha Shape area is zero. Try increasing the buffer size and/or alpha.");

  OsmMapWriterFactory::writeDebugMap(cutterShape, inputMap->getProjection(), className(), "cutter-shape-map");

  OsmMapPtr result = std::make_shared<OsmMap>(inputMap->getProjection());
  result->appendSource(inputMap->getSource());
  GeometryToElementConverter(result)
    .convertGeometryToElement(cutterShape.get(), Status::Invalid, -1);

  const RelationMap& rm = result->getRelations();
  for (auto it = rm.begin(); it != rm.end(); ++it)
  {
    Relation* r = result->getRelation(it->first).get();
    r->setTag(MetadataTags::Area(), "yes");
  }

  LOG_VART(MapProjector::toWkt(result->getProjection()));
  OsmMapWriterFactory::writeDebugMap(result, className(), "alpha-shape-result-map");

  return result;
}

std::shared_ptr<Geometry> AlphaShapeGenerator::generateGeometry(OsmMapPtr inputMap)
{
  MapProjector::projectToPlanar(inputMap);
  LOG_VART(MapProjector::toWkt(inputMap->getProjection()));

  // put all the nodes into a vector of points
  std::vector<std::pair<double, double>> points;
  points.reserve(inputMap->getNodeCount());
  const NodeMap& nodes = inputMap->getNodes();
  for (auto it = nodes.begin(); it != nodes.end(); ++it)
  {
    pair<double, double> p;
    p.first = (it->second)->getX();
    p.second = (it->second)->getY();
    points.push_back(p);
  }
  LOG_VART(points.size());

  // create a complex geometry representing the alpha shape
  AlphaShape alphaShape(_alpha);
  alphaShape.insert(points);
  try
  {
    _geometry = alphaShape.toGeometry();
  }
  catch (const IllegalArgumentException& /*e*/)
  {
    LOG_DEBUG(
      "Failed to generate alpha shape geometry with alpha value of: " <<
      StringUtils::formatLargeNumber(_alpha) << ".");
  }
  if (!_geometry)
    _geometry = GeometryFactory::getDefaultInstance()->createEmptyGeometry();

  _geometry = _geometry->buffer(_buffer);

  // See _coverStragglers description. This is an add-on behavior that is separate from the original
  // Alpha Shape algorithm.
  if (_manuallyCoverSmallPointClusters)
    _coverStragglers(inputMap);

  return _geometry;
}

void AlphaShapeGenerator::_coverStragglers(const ConstOsmMapPtr& map)
{
  LOG_DEBUG("Covering stragglers...");

  // Pretty simple...go through and find any point that wasn't covered by the Alpha Shape and draw
  // a buffer around it. This can get very slow for fairly large linear datasets. May need a new alg
  // here, if possible, for better performance.
  const NodeMap& nodes = map->getNodes();
  //  Push all nodes onto the work queue
  for (auto it = nodes.begin(); it != nodes.end(); ++it)
    _nodes.push(it->second);
  //  Minimize the threads needed to either _maxThreads or the node count
  _maxThreads = std::min(_maxThreads, static_cast<int>(_nodes.size()));
  //  Start all of the threads
  std::vector<std::thread> threads;
  for (int i = 0; i < _maxThreads; ++i)
    threads.push_back(thread(&AlphaShapeGenerator::_coverStragglersWorker, this));
  //  Set working to false after starting all threads so that they end once the queue is empty
  _working = false;
  //  Wait on all threads to end
  for (int i = 0; i < _maxThreads; ++i)
    threads[i].join();

  if (!_stragglers.empty())
  {
    LOG_DEBUG("Adding " << StringUtils::formatLargeNumber(_stragglers.size())
              << " point stragglers to alpha shape.");
    //  Merge the stragglers geometries
    GeometryPtr merged = GeometryMerger().mergeGeometries(_stragglers, *_geometry->getEnvelopeInternal());
    //  Join the original geometry with the stragglers geometry
    _geometry = _geometry->Union(merged.get());
  }
  LOG_VART(_geometry->getArea());
}

void AlphaShapeGenerator::_coverStragglersWorker()
{
  //  Make a thread local copy of the geometry because Geometry::contains() is not thread-safe
  GeometryPtr copy(_geometry->clone());
  //  Set the initial stack size to int max to enter the loop once and check actual stack size
  int stack_size = std::numeric_limits<int>::max();
  //  Keep looping while there is work on the stack
  while (_working || stack_size > 0)
  {
    //  Get the top node off of the stack
    NodePtr node;
    {
      //  Lock the mutex
      std::lock_guard<std::mutex> nodes_lock(_nodesMutex);
      stack_size = static_cast<int>(_nodes.size());
      if (stack_size > 0)
      {
        node = _nodes.top();
        _nodes.pop();
        --stack_size;
      }
    }
    //  Where the node is valid check ::contains()
    if (node)
    {
      GeometryPtr point(
        GeometryFactory::getDefaultInstance()->createPoint(
          geos::geom::Coordinate(node->getX(), node->getY())));
      //  Geometry::contains() is a time consuming function call (and not thread safe
      //  so all threads need their own `copy`)
      if (!copy->contains(point.get()))
      {
        LOG_TRACE("Point " << point->toString() << " not covered by alpha shape. Buffering and adding it...");
        point = point->buffer(_buffer);
        std::lock_guard<std::mutex> stragglers_lock(_stragglersMutex);
        _stragglers.push_back(point);
      }
    }
    else  //  Sleep if the node wasn't valid
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
}

}
