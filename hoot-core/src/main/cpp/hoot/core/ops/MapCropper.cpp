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
 * This will properly maintain the copyright information. DigitalGlobe
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "MapCropper.h"

// GEOS
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/MultiLineString.h>
#include <geos/geom/Point.h>
#include <geos/geom/Polygon.h>
#include <geos/io/WKBReader.h>
#include <geos/io/WKBWriter.h>
#include <geos/util/GEOSException.h>

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/elements/NodeToWayMap.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/ops/RemoveWayByEid.h>
#include <hoot/core/ops/RemoveNodeByEid.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/elements/ElementConverter.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/algorithms/FindNodesInWayFactory.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/GeometryUtils.h>
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/util/Validate.h>
#include <hoot/core/ops/RemoveEmptyRelationsOp.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/elements/OsmUtils.h>
#include <hoot/core/ops/SuperfluousWayRemover.h>
#include <hoot/core/ops/SuperfluousNodeRemover.h>
#include <hoot/core/visitors/RemoveMissingElementsVisitor.h>

// Standard
#include <limits>

// TGS
#include <tgs/StreamUtils.h>

using namespace geos::geom;
using namespace std;
using namespace Tgs;

namespace hoot
{

int MapCropper::logWarnCount = 0;

HOOT_FACTORY_REGISTER(OsmMapOperation, MapCropper)

MapCropper::MapCropper() :
_invert(false),
_keepEntireFeaturesCrossingBounds(false),
_keepOnlyFeaturesInsideBounds(false),
_removeSuperfluousFeatures(true),
_statusUpdateInterval(1000),
_numWaysInBounds(0),
_numWaysOutOfBounds(0),
_numWaysCrossingThreshold(0),
_numCrossingWaysKept(0),
_numCrossingWaysRemoved(0),
_numNodesRemoved(0)
{
}

MapCropper::MapCropper(const Envelope& envelope) :
_envelope(envelope),
_envelopeG(GeometryFactory::getDefaultInstance()->toGeometry(&_envelope)),
_invert(false),
_keepEntireFeaturesCrossingBounds(false),
_keepOnlyFeaturesInsideBounds(false),
_removeSuperfluousFeatures(true),
_statusUpdateInterval(1000),
_numWaysInBounds(0),
_numWaysOutOfBounds(0),
_numWaysCrossingThreshold(0),
_numCrossingWaysKept(0),
_numCrossingWaysRemoved(0),
_numNodesRemoved(0)
{
}

MapCropper::MapCropper(const std::shared_ptr<const Geometry>& g) :
_envelopeG(g),
_invert(false),
_keepEntireFeaturesCrossingBounds(false),
_keepOnlyFeaturesInsideBounds(false),
_removeSuperfluousFeatures(true),
_statusUpdateInterval(1000),
_numWaysInBounds(0),
_numWaysOutOfBounds(0),
_numWaysCrossingThreshold(0),
_numCrossingWaysKept(0),
_numCrossingWaysRemoved(0),
_numNodesRemoved(0)
{
}

void MapCropper::setBounds(const geos::geom::Envelope& bounds)
{
  _nodeBounds = bounds;
  _envelope = bounds,
  _envelopeG.reset(GeometryFactory::getDefaultInstance()->toGeometry(&_envelope));
}

void MapCropper::setBounds(const std::shared_ptr<const geos::geom::Geometry>& g)
{
  _envelopeG = g;
}

void MapCropper::setInvert(bool invert)
{
  _invert = invert;
  // Haven't seen evidence that these options make sense when we're doing inverted cropping so
  // let's leave them turned off if inverting is selected.
  if (!_invert)
  {
    _keepOnlyFeaturesInsideBounds = false;
    _keepEntireFeaturesCrossingBounds = false;
  }
}

void MapCropper::setKeepEntireFeaturesCrossingBounds(bool keep)
{
  // this option is ignored when set to invert
  if (_invert)
  {
    _keepEntireFeaturesCrossingBounds = false;
  }
  // this option is incomptible with the option to keep only features inside the bounds
  else if (keep && _keepOnlyFeaturesInsideBounds)
  {
    throw IllegalArgumentException(
      QString("Incompatible crop options: _keepOnlyFeaturesInsideBounds and ") +
      QString("_keepEntireFeaturesCrossingBounds cannot both be enabled."));
  }
  else
  {
    _keepEntireFeaturesCrossingBounds = keep;
  }
}

void MapCropper::setKeepOnlyFeaturesInsideBounds(bool keep)
{
  // this option is ignored when set to invert
  if (_invert)
  {
    _keepOnlyFeaturesInsideBounds = false;
  }
  // this option is incomptible with the option to keep features crossing the bounds
  else if (keep && _keepEntireFeaturesCrossingBounds)
  {
    throw IllegalArgumentException(
      QString("Incompatible crop options: _keepOnlyFeaturesInsideBounds and ") +
      QString("_keepEntireFeaturesCrossingBounds cannot both be enabled."));
  }
  else
  {
    _keepOnlyFeaturesInsideBounds = keep;
  }
}

void MapCropper::setConfiguration(const Settings& conf)
{
  ConfigOptions confOpts = ConfigOptions(conf);
  const QString boundsStr = confOpts.getCropBounds();
  if (!boundsStr.isEmpty())
  {
    _envelope = GeometryUtils::envelopeFromConfigString(boundsStr);
    LOG_VARD(_envelope);
    _envelopeG.reset(GeometryFactory::getDefaultInstance()->toGeometry(&_envelope));
  }
  // invert must be set before the two options following it
  setInvert(confOpts.getCropInvert());
  setKeepEntireFeaturesCrossingBounds(confOpts.getCropKeepEntireFeaturesCrossingBounds());
  setKeepOnlyFeaturesInsideBounds(confOpts.getCropKeepOnlyFeaturesInsideBounds());

  _statusUpdateInterval = confOpts.getTaskStatusUpdateInterval();
}

void MapCropper::apply(OsmMapPtr& map)
{
  if (MapProjector::isGeographic(map) == false && _nodeBounds.isNull() == false)
  {
    throw HootException("If the node bounds is set the projection must be geographic.");
  }

  LOG_DEBUG("Cropping ways...");
  LOG_VARD(map->size());
  LOG_VART(OsmUtils::allElementIdsPositive(map));

  _numProcessed = 0;
  _numAffected = 0;
  _numWaysInBounds = 0;
  _numWaysOutOfBounds = 0;
  _numWaysCrossingThreshold = 0;
  _numCrossingWaysKept = 0;
  _numCrossingWaysRemoved = 0;
  _numNodesRemoved = 0;
  _explicitlyIncludedWayIds.clear();

  LOG_VARD(_invert);
  LOG_VARD(_keepEntireFeaturesCrossingBounds);
  LOG_VARD(_keepOnlyFeaturesInsideBounds);
  LOG_VARD(_envelope);
  if (_envelopeG)
  {
    LOG_VARD(_envelopeG->toString());
  }
  LOG_VARD(_inclusionCrit.get());

  // go through all the ways
  long wayCtr = 0;
  const WayMap ways = map->getWays();
  for (WayMap::const_iterator it = ways.begin(); it != ways.end(); ++it)
  {
    const std::shared_ptr<Way>& w = it->second;
    LOG_TRACE("Checking " << w->getElementId() << " for cropping...");
    LOG_VART(w->getNodeIds());
    LOG_VART(w);

    std::shared_ptr<LineString> ls = ElementConverter(map).convertToLineString(w);
    if (!ls.get())
    {
      if (logWarnCount < Log::getWarnMessageLimit())
      {
        LOG_WARN("Couldn't convert " << w->getElementId() << " to line string. Keeping way...");
        LOG_VARW(w);
      }
      else if (logWarnCount == Log::getWarnMessageLimit())
      {
        LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
      }
      logWarnCount++;

      _numProcessed++;
      wayCtr++;
      continue;
    }
    const Envelope& wayEnv = *(ls->getEnvelopeInternal());
    LOG_VART(wayEnv);

    if (_inclusionCrit)
    {
      LOG_VART(_inclusionCrit->isSatisfied(w));
    }
    if (_inclusionCrit && _inclusionCrit->isSatisfied(w))
    {
      // keep the way
      LOG_TRACE("Keeping explicitly included way: " << w->getElementId() << "...");
      _explicitlyIncludedWayIds.insert(w->getId());
      _numWaysInBounds++;
    }
    // Have found that if we have _envelopeG, using it for inside/outside way comparison is more
    // accurate than using the envelope. Its possible that we could eventually use that method
    // exclusively and get rid of storing _envelope.
    else if ((_envelopeG && _isWhollyOutside(*ls)) || _isWhollyOutside(wayEnv))
    {
      // remove the way
      LOG_TRACE("Dropping wholly outside way: " << w->getElementId() << "...");
      RemoveWayByEid::removeWayFully(map, w->getId());
      _numWaysOutOfBounds++;
      _numAffected++;
    }
    else if ((_envelopeG && _isWhollyInside(*ls)) || _isWhollyInside(wayEnv))
    {
      // keep the way
      LOG_TRACE("Keeping wholly inside way: " << w->getElementId() << "...");
      _numWaysInBounds++;
    }
    else if (_keepOnlyFeaturesInsideBounds)
    {
      // Way isn't wholly inside and the configuration requires it to be, so remove the way.
      LOG_TRACE(
        "Dropping due to _keepOnlyFeaturesInsideBounds=true: " << w->getElementId() << "...");
      RemoveWayByEid::removeWayFully(map, w->getId());
      _numWaysOutOfBounds++;
      _numAffected++;
    }
    else if (!_keepEntireFeaturesCrossingBounds)
    {
      // Way crosses the boundary and we're not configured to keep ways that cross the bounds, so
      // do an expensive operation to decide how much to keep, if any.
      LOG_TRACE(
        "Cropping due to _keepEntireFeaturesCrossingBounds=false: " << w->getElementId() << "...");
      _cropWay(map, w->getId());
      _numWaysCrossingThreshold++;
    }
    else
    {
      // keep the way
      LOG_TRACE("Keeping way: " << w->getElementId() << "...");
      _numWaysInBounds++;
    }

    wayCtr++;
    _numProcessed++;
    if (wayCtr % _statusUpdateInterval == 0)
    {
      PROGRESS_INFO(
        "Cropped " << StringUtils::formatLargeNumber(wayCtr) << " / " <<
        StringUtils::formatLargeNumber(ways.size()) << " ways.");
    }
  }

  std::shared_ptr<NodeToWayMap> n2w = map->getIndex().getNodeToWayMap();

  LOG_DEBUG("Removing nodes...");

  // go through all the nodes
  long nodeCtr = 0;
  long nodesRemoved = 0;
  const NodeMap nodes = map->getNodes();
  for (NodeMap::const_iterator it = nodes.begin(); it != nodes.end(); ++it)
  {
    NodePtr node = it->second;
    LOG_TRACE("Checking " << node->getElementId() << " for cropping...");
    LOG_VART(node);

    bool nodeInside = false;

    LOG_VART(_explicitlyIncludedWayIds.size());
    if (_explicitlyIncludedWayIds.size() > 0)
    {
      LOG_VART(OsmUtils::nodeContainedByAnyWay(node->getId(), _explicitlyIncludedWayIds, map));
    }
    if (_inclusionCrit && _explicitlyIncludedWayIds.size() > 0 &&
        OsmUtils::nodeContainedByAnyWay(node->getId(), _explicitlyIncludedWayIds, map))
    {
      LOG_TRACE(
        "Skipping delete for: " << node->getElementId() <<
        " belonging to explicitly included way(s)...");
      nodeInside = true;
    }
    else
    {
      const Coordinate& c = it->second->toCoordinate();
      LOG_VART(c.toString());
      if (_envelope.isNull() == false)
      {
        if (_invert == false)
        {
          nodeInside = _envelope.covers(c);
          LOG_TRACE(
            "Node inside check: non-inverted crop and the envelope covers the element=" <<
            nodeInside);
        }
        else
        {
          nodeInside = !_envelope.covers(c);
          LOG_TRACE(
            "Node inside check: inverted crop and the envelope covers the element=" << !nodeInside);
        }
      }
      else
      {
        std::shared_ptr<Point> p(GeometryFactory::getDefaultInstance()->createPoint(c));
        if (_invert == false)
        {
          nodeInside = _envelopeG->intersects(p.get());
          LOG_TRACE(
            "Node inside check: non-inverted crop and the envelope intersects the element=" <<
            nodeInside);
        }
        else
        {
          nodeInside = !_envelopeG->intersects(p.get());
          LOG_TRACE(
            "Node inside check: inverted crop and the envelope intersects the element=" <<
            !nodeInside);
        }
      }
      LOG_VART(nodeInside);

      // if the node is outside
      if (!nodeInside)
      {
        // if the node is within the limiting bounds.
        // TODO: This may have been left over from support for four pass conflation using Hadoop.
        // Could we just use _envelope or _envelopeG here instead?
        LOG_VART(_nodeBounds.isNull());
        if (!_nodeBounds.isNull())
        {
          LOG_VART(_nodeBounds.contains(c));
        }
        if (_nodeBounds.isNull() == true || _nodeBounds.contains(c))
        {
          // if the node is not part of a way
          if (n2w->find(it->first) == n2w->end())
          {
            // remove the node
            LOG_TRACE(
              "Removing node with coords: " << it->second->getX() << " : " << it->second->getY());
            RemoveNodeByEid::removeNodeNoCheck(map, it->second->getId());
            nodesRemoved++;
            _numAffected++;
          }
        }
      }
    }

    nodeCtr++;
    _numProcessed++;
    if (nodeCtr % _statusUpdateInterval == 0)
    {
      PROGRESS_INFO(
        "Cropped " << StringUtils::formatLargeNumber(nodeCtr) << " / " <<
        StringUtils::formatLargeNumber(nodes.size()) << " nodes.");
    }
  }

  // Remove dangling features here now, which used to be done in CropCmd only. Haven't seen any
  // bad side effects yet.
  long numSuperfluousWaysRemoved = 0;
  long numSuperfluousNodesRemoved = 0;
  if (_removeSuperfluousFeatures)
  {
    numSuperfluousWaysRemoved = SuperfluousWayRemover::removeWays(map);
    numSuperfluousNodesRemoved = SuperfluousNodeRemover::removeNodes(map);
  }

  // This will handle removing refs in relation members we've cropped out.
  RemoveMissingElementsVisitor missingElementsRemover;
  LOG_INFO("\t" << missingElementsRemover.getInitStatusMessage());
  map->visitRw(missingElementsRemover);
  LOG_DEBUG("\t" << missingElementsRemover.getCompletedStatusMessage());

  // This will remove any relations that were already empty or became empty after the previous step.
  RemoveEmptyRelationsOp emptyRelationRemover;
  LOG_INFO("\t" << emptyRelationRemover.getInitStatusMessage());
  emptyRelationRemover.apply(map);
  LOG_DEBUG("\t" << emptyRelationRemover.getCompletedStatusMessage());

  LOG_VARD(_numAffected);
  LOG_VARD(map->size());
  LOG_VART(OsmUtils::allElementIdsPositive(map));
  LOG_VARD(StringUtils::formatLargeNumber(_numWaysInBounds));
  LOG_VARD(StringUtils::formatLargeNumber(_numWaysOutOfBounds));
  LOG_VARD(StringUtils::formatLargeNumber(_numWaysCrossingThreshold));
  LOG_VARD(StringUtils::formatLargeNumber(_numCrossingWaysKept));
  LOG_VARD(StringUtils::formatLargeNumber(_numCrossingWaysRemoved));
  LOG_VARD(StringUtils::formatLargeNumber(_numNodesRemoved));
  LOG_VARD(numSuperfluousWaysRemoved);
  LOG_VARD(numSuperfluousNodesRemoved);
}

void MapCropper::_cropWay(const OsmMapPtr& map, long wid)
{
  LOG_TRACE("Cropping way crossing bounds: " << wid << "...");

  std::shared_ptr<Way> way = map->getWay(wid);
  std::shared_ptr<Geometry> fg = ElementConverter(map).convertToGeometry(way);

  // perform the intersection with the geometry
  std::shared_ptr<Geometry> g;
  try
  {
    if (_invert)
      g.reset(fg->difference(_envelopeG.get()));
    else
      g.reset(fg->intersection(_envelopeG.get()));
  }
  catch (const geos::util::GEOSException&)
  {
    // try cleaning up the geometry and try again.
    fg.reset(GeometryUtils::validateGeometry(fg.get()));
    if (_invert)
      g.reset(fg->difference(_envelopeG.get()));
    else
      g.reset(fg->intersection(_envelopeG.get()));
  }

  std::shared_ptr<FindNodesInWayFactory> nodeFactory(new FindNodesInWayFactory(way));
  GeometryConverter gc(map);
  gc.setNodeFactory(nodeFactory);
  std::shared_ptr<Element> e =
    gc.convertGeometryToElement(g.get(), way->getStatus(), way->getCircularError());
  LOG_VART(e.get());

  if (e == 0)
  {
    LOG_TRACE("Removing way during crop check: " << way->getElementId() << "...");
    RemoveWayByEid::removeWayFully(map, way->getId());
    _numCrossingWaysRemoved++;
    _numAffected++;
  }
  else
  {
    LOG_TRACE(
      "Replacing way during crop check: " << way->getElementId() << " with element: " <<
      e->getElementId() << "...");

    e->setTags(way->getTags());

    // retain the parent ID
    if (e->getElementType() == ElementType::Way)
    {
      WayPtr newWay = std::dynamic_pointer_cast<Way>(e);
      newWay->setPid(way->getId());
    }
    else if (e->getElementType() == ElementType::Relation)
    {
      RelationPtr newRelation = std::dynamic_pointer_cast<Relation>(e);
      const vector<RelationData::Entry>& members = newRelation->getMembers();
      for (size_t i = 0; i < members.size(); i++)
      {
        RelationData::Entry element = members[i];
        if (element.getElementId().getType() == ElementType::Way)
        {
          WayPtr memberWay = map->getWay(element.getElementId().getId());
          memberWay->setPid(way->getId());
        }
      }
    }

    map->replace(way, e);

    _numCrossingWaysKept++;
  }
}

bool MapCropper::_isWhollyInside(const Envelope& e)
{
  bool result = false;
  LOG_VART(_envelope.isNull());
  if (_envelope.isNull() == false)
  {
    if (_invert)
    {
      result = !_envelope.intersects(e);
      LOG_TRACE(
        "Wholly inside check: inverted crop and the envelope intersects with the element=" <<
        !result);
    }
    else
    {
      result = _envelope.covers(e);
      LOG_TRACE(
        "Wholly inside check: non-inverted crop and the envelope covers the element=" << result);
    }
  }
  else
  {
    if (_invert)
    {
      result = !_envelopeG->getEnvelopeInternal()->intersects(e);
      LOG_TRACE(
        "Wholly inside way check: inverted crop and the envelope intersects with the element=" <<
        !result);
    }
    else
    {
      // If it isn't inverted, we need to do an expensive check.
      result = _envelopeG->getEnvelopeInternal()->covers(e);
      LOG_TRACE(
        "Wholly inside way check: non-inverted crop and the envelope covers the element=" << result);
    }
  }
  LOG_TRACE("Wholly inside way check result: " << result);
  return result;
}

bool MapCropper::_isWhollyInside(const Geometry& e)
{
  bool result = false;
  LOG_VART(_envelope.isNull());

  if (_invert)
  {
    result = !_envelopeG->intersects(&e);
    LOG_TRACE(
      "Wholly inside way check: inverted crop and the envelope intersects with the element=" <<
      !result);
  }
  else
  {
    // If it isn't inverted, we need to do an expensive check.
    result = _envelopeG->covers(&e);
    LOG_TRACE(
      "Wholly inside way check: non-inverted crop and the envelope covers the element=" << result);
  }
  LOG_TRACE("Wholly inside way check result: " << result);
  return result;
}

bool MapCropper::_isWhollyOutside(const Envelope& e)
{
  bool result = false;
  LOG_VART(_envelope.isNull());
  if (_envelope.isNull() == false)
  {
    if (_invert)
    {
      result = _envelope.covers(e);
      LOG_TRACE(
        "Wholly outside check: inverted crop and the envelope covers the element=" << result);
    }
    else
    {
      result = !_envelope.intersects(e);
      LOG_TRACE(
        "Wholly outside way check: non-inverted crop and the envelope intersects with the element=" <<
        !result);
    }
  }
  else
  {
    if (_invert == false)
    {
      result = !_envelopeG->getEnvelopeInternal()->intersects(e);
      //result = !_envelopeG->intersects(GeometryFactory::getDefaultInstance()->toGeometry(&e));
      LOG_TRACE(
        "Wholly outside way check: non-inverted crop and the envelope intersects with the element=" <<
        !result);
    }
    else
    {
      result = _envelopeG->getEnvelopeInternal()->covers(e);
      //result = _envelopeG->covers(GeometryFactory::getDefaultInstance()->toGeometry(&e));
      LOG_TRACE(
        "Wholly outside way check: inverted crop and the envelope covers the element=" << result);
    }
  }
  LOG_TRACE("Wholly outside way check result: " << result);
  return result;
}

bool MapCropper::_isWhollyOutside(const Geometry& e)
{
  bool result = false;
  if (_invert == false)
  {
    result = !_envelopeG->intersects(&e);
    LOG_TRACE(
      "Wholly outside way check: non-inverted crop and the envelope intersects with the element=" <<
      !result);
  }
  else
  {
    result = _envelopeG->covers(&e);
    LOG_TRACE(
      "Wholly outside way check: inverted crop and the envelope covers the element=" << result);
  }
  LOG_TRACE("Wholly outside way check result: " << result);
  return result;
}

void MapCropper::readObject(QDataStream& is)
{
  is >> _invert;
  uint egSize;
  is >> egSize;
  _envelopeG.reset();
  // if this is a polygon for cropping
  if (egSize > 0)
  {
    string s;
    s.resize(egSize, '*');
    char* data = (char*)s.data();
    is.readRawData(data, egSize);
    stringstream ss(s, ios_base::in);
    _envelopeG.reset(geos::io::WKBReader().read(ss));
    _envelope.setToNull();
  }
  // if this is a simple envelope
  else
  {
    double minx, miny, maxx, maxy;
    is >> minx >> miny >> maxx >> maxy;
    _envelope = Envelope(minx, maxx, miny, maxy);
    _envelopeG.reset(GeometryFactory::getDefaultInstance()->toGeometry(&_envelope));
  }
}

void MapCropper::writeObject(QDataStream& os) const
{
  os << _invert;
  // if this is a polygon for cropping
  if (_envelope.isNull())
  {
    stringstream ss;
    // use WKB to avoid small differences in the geometries before/after serialization.
    geos::io::WKBWriter().write(*_envelopeG, ss);
    const string& s = ss.str();
    os << (uint)s.size();
    os.writeRawData(s.data(), s.size());
  }
  // if this is a simple envelope
  else
  {
    os << 0;
    os << _envelope.getMinX();
    os << _envelope.getMinY();
    os << _envelope.getMaxX();
    os << _envelope.getMaxY();
  }
}

}
