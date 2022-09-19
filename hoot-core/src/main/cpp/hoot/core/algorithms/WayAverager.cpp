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

#include "WayAverager.h"

// GEOS
#include <geos/geom/CoordinateSequence.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/LineString.h>
#include <geos/geom/Point.h>
#include <geos/operation/distance/DistanceOp.h>

// Hoot
#include <hoot/core/algorithms/DirectionFinder.h>
#include <hoot/core/criterion/OneWayCriterion.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/geometry/ElementToGeometryConverter.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/ops/RemoveWayByEid.h>
#include <hoot/core/schema/TagMergerFactory.h>

using namespace geos::geom;
using namespace geos::operation::distance;
using namespace std;

namespace hoot
{

WayAverager::WayAverager(const OsmMapPtr& map, const WayPtr& w1, const WayPtr& w2)
  : _map(map),
  _meanMovement1(0.0),
  _meanMovement2(0.0),
  _sumMovement1(0.0),
  _sumMovement2(0.0),
  _maxMovement1(0.0),
  _maxMovement2(0.0),
  _moveCount1(0),
  _moveCount2(0)
{
  if (w1->getStatus() == Status::Unknown2 && w2->getStatus() == Status::Unknown1)
  {
    _w2 = w1;
    _w1 = w2;
  }
  else
  {
    _w1 = w1;
    _w2 = w2;
  }
}

WayPtr WayAverager::replaceWaysWithAveragedWay()
{
  _sumMovement1 = 0.0;
  _sumMovement2 = 0.0;
  _moveCount1 = 0;
  _moveCount2 = 0;
  _maxMovement1 = 0.0;
  _maxMovement2 = 0.0;

  LOG_VART(DirectionFinder::isSimilarDirection(_map, _w1, _w2));
  if (DirectionFinder::isSimilarDirection(_map, _w1, _w2) == false)
  {
    if (OneWayCriterion().isSatisfied(_w1) == true)
      _w2->reverseOrder();
    else
      _w1->reverseOrder();
  }

  std::shared_ptr<const LineString> ls1 =
    ElementToGeometryConverter(_map).convertToLineString(_w1);
  std::shared_ptr<const LineString> ls2 =
    ElementToGeometryConverter(_map).convertToLineString(_w2);
  if (!ls1 || !ls2)
    return WayPtr();

  // All of the fancy stats here are compliments of Mike Porter.

  // calculate standard deviation in meters
  Meters sd1 = _w1->getCircularError() / 2.0;
  Meters sd2 = _w2->getCircularError() / 2.0;
  // calculate variance;
  double v1 = sd1 * sd1;
  double v2 = sd2 * sd2;
  // calculate weights for averaging.
  double weight1 = 1 - v1 / (v1 + v2);
  double weight2 = 1 - v2 / (v1 + v2);

  Meters newAcc = 2.0 * sqrt(weight1 * weight1 * v1 + weight2 * weight2 * v2);
  LOG_VART(newAcc);

  WayPtr result = std::make_shared<Way>(_w1->getStatus(), _w1->getId(), newAcc);
  result->setPid(Way::getPid(_w1, _w2));

  ConstNodePtr node1a = _map->getNode(_w1->getNodeIds()[0]);
  ConstNodePtr node2a = _map->getNode(_w2->getNodeIds()[0]);
  if (node1a && node2a)
    result->addNode(_merge(node1a, weight1, node2a, weight2));
  OsmMapWriterFactory::writeDebugMap(_map, className(), "after-first-add-node");

  // We're getting the vectors after the above merge because the merge will change node ids.
  const std::vector<long>& ns1 = _w1->getNodeIds();
  const std::vector<long>& ns2 = _w2->getNodeIds();
  LOG_VART(ns1.size());
  LOG_VART(ns2.size());

  size_t i1 = 1;
  size_t i2 = 1;

  // while there is more than one point available in each line
  int index = 1;
  while (i1 < ns1.size() - 1 || i2 < ns2.size() - 1)
  {
    // if we're all out of cs1 points
    if (i1 == ns1.size() - 1)
    {
      result->addNode(_moveToLine(ns2[i2++], weight2, ls1.get(), weight1, 2));
      OsmMapWriterFactory::writeDebugMap(_map, className(), "after-move-to-line-1");
    }
    // if we're all out of cs2 points
    else if (i2 == ns2.size() - 1)
    {
      result->addNode(_moveToLine(ns1[i1++], weight1, ls2.get(), weight2, 1));
      OsmMapWriterFactory::writeDebugMap(_map, className(), "after-move-to-line-2");
    }
    else
    {
      const Coordinate& last = _map->getNode(result->getLastNodeId())->toCoordinate();

      const Coordinate& nc1 = _moveToLineAsCoordinate(ns1[i1], weight1, ls2.get(), weight2);
      const Coordinate& nc2 = _moveToLineAsCoordinate(ns2[i2], weight2, ls1.get(), weight1);

      if (nc1.distance(last) < nc2.distance(last))
      {
        result->addNode(_moveToLine(ns1[i1++], weight1, ls2.get(), weight2, 1));
        OsmMapWriterFactory::writeDebugMap(_map, className(), "after-move-to-line-3");
      }
      else
      {
        result->addNode(_moveToLine(ns2[i2++], weight2, ls1.get(), weight1, 2));
        OsmMapWriterFactory::writeDebugMap(_map, className(), "after-move-to-line-4");
      }
      index++;
    }

    OsmMapWriterFactory::writeDebugMap(_map, className(), "after-add-node-loop-" + QString::number(index));
  }
  LOG_VART(result->getNodeCount());
  LOG_VART(i1);
  LOG_VART(i2);

  // merge the last two nodes and move to the average location
  ConstNodePtr node1b = _map->getNode(ns1[i1]);
  ConstNodePtr node2b = _map->getNode(ns2[i2]);
  if (node1b && node2b)
    result->addNode(_merge(node1b, weight1, node2b, weight2));
  LOG_VART(result->getNodeCount());
  OsmMapWriterFactory::writeDebugMap(_map, className(), "after-last-add-node");

  // use the default tag merging mechanism
  result->setTags(TagMergerFactory::mergeTags(_w1->getTags(), _w2->getTags(), ElementType::Way));

  RemoveWayByEid::removeWay(_map, _w1->getId());
  RemoveWayByEid::removeWay(_map, _w2->getId());

  _map->addWay(result);

  _meanMovement1 = _sumMovement1 / (double)_moveCount1;
  _meanMovement2 = _sumMovement2 / (double)_moveCount2;

  return result;
}

WayPtr WayAverager::replaceWaysWithAveragedWay(const OsmMapPtr& map, const WayPtr& w1, const WayPtr& w2)
{
  return WayAverager(map, w1, w2).replaceWaysWithAveragedWay();
}

long WayAverager::_merge(const ConstNodePtr& node1, double weight1, const ConstNodePtr& node2, double weight2)
{
  Meters d = node1->toCoordinate().distance(node2->toCoordinate());
  LOG_VART(d);

  _sumMovement1 += d * weight2;
  _moveCount1++;
  _maxMovement2 = max(_maxMovement2, d * weight2);

  _sumMovement2 += d * weight1;
  _moveCount2++;
  _maxMovement2 = max(_maxMovement2, d * weight1);

  NodePtr node =
    std::make_shared<Node>(
      Status::Conflated, _map->createNextNodeId(),
      node1->getX() * weight1 + node2->getX() * weight2,
      node1->getY() * weight1 + node2->getY() * weight2,
      std::min(node1->getCircularError(), node2->getCircularError()));

  _map->addNode(node);
  OsmMapWriterFactory::writeDebugMap(_map, className(), "after-add-node");
  // replace all instances of node1/node2 with node.
  _map->replaceNode(node1->getId(), node->getId());
  OsmMapWriterFactory::writeDebugMap(_map, className(), "after-node-replacement-1");
  _map->replaceNode(node2->getId(), node->getId());
  OsmMapWriterFactory::writeDebugMap(_map, className(), "after-node-replacement-2");

  return node->getId();
}

long WayAverager::_moveToLine(long ni, double nWeight, const LineString* ls, double lWeight, int w1OrW2)
{
  NodePtr n = _map->getNode(ni);
  Coordinate c = _moveToLineAsCoordinate(ni, nWeight, ls, lWeight);

  Meters d = c.distance(n->toCoordinate());
  if (w1OrW2 == 1)
  {
    _sumMovement1 += d;
    _moveCount1++;
    _maxMovement1 = max(_maxMovement1, d);
  }
  else
  {
    _sumMovement2 += d;
    _moveCount2++;
    _maxMovement2 = max(_maxMovement2, d);
  }

  n->setX(c.x);
  n->setY(c.y);

  return ni;
}

Coordinate WayAverager::_moveToLineAsCoordinate(long ni, double nWeight, const LineString* ls,
                                                double lWeight) const
{
  NodePtr n = _map->getNode(ni);
  std::shared_ptr<Point> point(
    GeometryFactory::getDefaultInstance()->createPoint(n->toCoordinate()));

  // find the two closest points
  std::shared_ptr<CoordinateSequence> cs(
    DistanceOp::nearestPoints(point.get(), const_cast<LineString*>(ls)));

  Coordinate result = Coordinate(cs->getAt(0).x * nWeight + cs->getAt(1).x * lWeight,
                                 cs->getAt(0).y * nWeight + cs->getAt(1).y * lWeight);

  return result;
}

}
