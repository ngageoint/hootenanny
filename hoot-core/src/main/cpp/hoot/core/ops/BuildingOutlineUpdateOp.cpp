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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "BuildingOutlineUpdateOp.h"

// geos
#include <geos/geom/Geometry.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/Polygon.h>
#include <geos/util/TopologyException.h>
#include <geos/opBuffer.h>

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/conflate/NodeToWayMap.h>
#include <hoot/core/elements/ConstElementVisitor.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/ops/RemoveNodeOp.h>
#include <hoot/core/util/ElementConverter.h>
#include <hoot/core/util/GeometryConverter.h>
#include <hoot/core/util/GeometryUtils.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/util/Log.h>

using namespace geos::geom;
using namespace std;

namespace hoot
{

unsigned int BuildingOutlineUpdateOp::logWarnCount = 0;

HOOT_FACTORY_REGISTER(OsmMapOperation, BuildingOutlineUpdateOp)

class NodeIdVisitor : public ConstElementVisitor
{
public:
  set<long>& allNodes;

  NodeIdVisitor(set<long>& nodes) : allNodes(nodes) {}

  virtual void visit(const ConstElementPtr& e)
  {
    if (e->getElementType() == ElementType::Node)
    {
      allNodes.insert(e->getId());
    }
  }

  virtual QString getDescription() const { return ""; }
};

class NodeReplaceVisitor : public ConstElementVisitor
{
public:
  NodeReplaceVisitor(OsmMap& map, const std::map<long, long>& fromTo) : _fromTo(fromTo), _map(map)
  {}

  virtual void visit(const ConstElementPtr& e)
  {
    if (e->getElementType() == ElementType::Way)
    {
      const WayPtr& w = _map.getWay(e->getId());
      std::vector<long> oldNodes = w->getNodeIds();
      std::vector<long> newNodes = w->getNodeIds();

      for (size_t i = 0; i < newNodes.size(); i++)
      {
        map<long, long>::const_iterator it = _fromTo.find(newNodes[i]);
        if (it != _fromTo.end())
        {
          newNodes[i] = it->second;
        }
      }

      w->setNodes(newNodes);

      // remove any unused nodes.
      const NodeToWayMap& n2w = *_map.getIndex().getNodeToWayMap();
      for (size_t i = 0; i < oldNodes.size(); i++)
      {
        if (n2w.getWaysByNode(oldNodes[i]).size() == 0 && _map.containsNode(oldNodes[i]))
        {
          RemoveNodeOp::removeNode(_map.shared_from_this(), oldNodes[i]);
        }
      }
    }
  }

  virtual QString getDescription() const { return ""; }

private:
  const map<long, long>& _fromTo;
  OsmMap& _map;
};

BuildingOutlineUpdateOp::BuildingOutlineUpdateOp()
{
}

void BuildingOutlineUpdateOp::apply(boost::shared_ptr<OsmMap> &map)
{
  _map = map;

  // go through all the relations
  const RelationMap& relations = map->getRelations();
  for (RelationMap::const_iterator it = relations.begin(); it != relations.end(); ++it)
  {
    const RelationPtr& r = it->second;
    // add the relation to a building group if appropriate
    if (OsmSchema::getInstance().isBuilding(r->getTags(), r->getElementType()))
    {
      _createOutline(r);
    }
  }
}

void BuildingOutlineUpdateOp::_unionOutline(const RelationPtr& building,
                                            boost::shared_ptr<Geometry> outline,
                                            ElementPtr buildingMember)
{
  boost::shared_ptr<Geometry> g = ElementConverter(_map).convertToGeometry(buildingMember);
  try
  {
    outline.reset(outline->Union(g.get()));
  }
  catch (const geos::util::TopologyException& e)
  {
    LOG_TRACE("Attempting to clean way geometry after union error: " << e.what());
    LOG_VART(buildingMember->toString());
    Geometry* cleanedGeom = GeometryUtils::validateGeometry(g.get());
    try
    {
      outline.reset(outline->Union(cleanedGeom));
    }
    catch (const geos::util::TopologyException& e)
    {
      //couldn't clean, so mark parent relation for review (eventually we'll come up with
      //cleaning that works here)
      const QString errMsg =
        QString("Element with uncleanable topology.  Error occurred during union ") +
        QString("operation of element: ") + buildingMember->getElementId().toString();
      //TODO: if _unionOutline gets activated, make a ReviewMaker mem var to use for this
      ReviewMarker().mark(_map, building, errMsg + ".", ReviewMarker::getBadGeometryType());
      if (logWarnCount < Log::getWarnMessageLimit())
      {
        LOG_WARN(errMsg + ": " + QString(e.what()));
      }
      else if (logWarnCount == Log::getWarnMessageLimit())
      {
        LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
      }
      logWarnCount++;
    }
  }
}

void BuildingOutlineUpdateOp::_createOutline(const RelationPtr& building)
{
  LOG_VART(building->toString());

  boost::shared_ptr<Geometry> outline(GeometryFactory::getDefaultInstance()->createEmptyGeometry());

  const vector<RelationData::Entry> entries = building->getMembers();
  for (size_t i = 0; i < entries.size(); i++)
  {
    if (entries[i].role == MetadataTags::RoleOutline())
    {
      LOG_TRACE("Removing outline from building: " << entries[i].getElementId() << "...");
      building->removeElement(entries[i].role, entries[i].getElementId());
    }
    else if (entries[i].role == MetadataTags::RolePart())
    {
      LOG_TRACE("Processing building part: " << entries[i].getElementId() << "...");
      if (entries[i].getElementId().getType() == ElementType::Way)
      {
        {
          WayPtr way = _map->getWay(entries[i].getElementId().getId());
          if (way->getNodeCount() >= 4)
          {
            boost::shared_ptr<Geometry> g = ElementConverter(_map).convertToGeometry(way);
            try
            {
              outline.reset(outline->Union(g.get()));
            }
            catch (const geos::util::TopologyException& e)
            {
              LOG_TRACE("Attempting to clean way geometry after union error: " << e.what());
              LOG_VART(way->toString());
              Geometry* cleanedGeom = GeometryUtils::validateGeometry(g.get());
              try
              {
                outline.reset(outline->Union(cleanedGeom));
              }
              catch (const geos::util::TopologyException& e)
              {
                //couldn't clean, so mark parent relation for review (eventually we'll come up with
                //cleaning that works here)
                const QString errMsg =
                  "Marking parent element for review for element with uncleanable topology: " +
                  way->getElementId().toString();
                _reviewMarker.mark(_map, building, errMsg + ".", ReviewMarker::getBadGeometryType());
                if (logWarnCount < Log::getWarnMessageLimit())
                {
                  LOG_WARN(errMsg + ": " + QString(e.what()));
                }
                else if (logWarnCount == Log::getWarnMessageLimit())
                {
                  LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
                }
                logWarnCount++;
              }
            }
          }

          {
            //TODO: There are some strange scoping/casting issues going on here, where if I try
            //to consolidate the unioning code for ways and relations into this method, the
            //resulting output differs significantly.  I believe the casting going on in
            //GeometryConverter is related to the cause.  If that's solved, then the duplicated
            //code can go away.

            //_unionOutline(building, outline, way);
          }
        }
      }
      else if (entries[i].getElementId().getType() == ElementType::Relation)
      {
        RelationPtr relation = _map->getRelation(entries[i].getElementId().getId());
        if (relation->isMultiPolygon())
        {
          {
            boost::shared_ptr<Geometry> g = ElementConverter(_map).convertToGeometry(relation);
            try
            {
              outline.reset(outline->Union(g.get()));
            }
            catch (const geos::util::TopologyException& e)
            {
              LOG_TRACE("Attempting to clean way geometry after union error: " << e.what());
              LOG_VART(relation->toString());
              Geometry* cleanedGeom = GeometryUtils::validateGeometry(g.get());
              try
              {
                outline.reset(outline->Union(cleanedGeom));
              }
              catch (const geos::util::TopologyException& e)
              {
                //couldn't clean, so mark parent relation for review (eventually we'll come up with
                //cleaning that works here)
                const QString errMsg =
                  "Marking parent element for review for element with uncleanable topology: " +
                  relation->getElementId().toString();
                _reviewMarker.mark(_map, building, errMsg + ".", ReviewMarker::getBadGeometryType());
                if (logWarnCount < Log::getWarnMessageLimit())
                {
                  LOG_WARN(errMsg + ": " + QString(e.what()));
                }
                else if (logWarnCount == Log::getWarnMessageLimit())
                {
                  LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
                }
                logWarnCount++;
              }
            }
          }

          {
            //see comment above
            //_unionOutline(building, outline, relation);
          }
        }
        else
        {
          if (logWarnCount < Log::getWarnMessageLimit())
          {
            LOG_WARN(
              "Found a building with a non-multipolygon relation 'part'. " <<
              relation->toString() << "Building: " << building->toString());
          }
          else if (logWarnCount == Log::getWarnMessageLimit())
          {
            LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
          }
          logWarnCount++;
        }
      }
    }
  }

  if (outline->isEmpty() == false)
  {
    LOG_TRACE("Processing outline...");
    const boost::shared_ptr<Element> outlineElement =
      GeometryConverter(_map).convertGeometryToElement(
        outline.get(), building->getStatus(), building->getCircularError());
    _mergeNodes(outlineElement, building);
    outlineElement->setTags(building->getTags());
    // we don't need the relation "type" tag.
    outlineElement->getTags().remove("type");
    building->addElement(MetadataTags::RoleOutline(), outlineElement);
  }
}

void BuildingOutlineUpdateOp::_mergeNodes(const boost::shared_ptr<Element>& changed,
  const RelationPtr& reference)
{
  set<long> changedNodes;
  set<long> referenceNodes;

  NodeIdVisitor changedVisitor(changedNodes);
  changed->visitRo(*_map, changedVisitor);
  NodeIdVisitor referenceVisitor(referenceNodes);
  reference->visitRo(*_map, referenceVisitor);

  // map the changed nodes to reference nodes.
  map<long, long> nodeIdMap;

  double epsilon = 1e-9;
  for (set<long>::const_iterator ci = changedNodes.begin(); ci != changedNodes.end(); ++ci)
  {
    double bestDistance = epsilon;
    // should never be used uninitialized
    long bestMatch = -9999;
    const NodePtr& cn = _map->getNode(*ci);
    for (set<long>::const_iterator ri = referenceNodes.begin(); ri != referenceNodes.end(); ++ri)
    {
      const NodePtr& rn = _map->getNode(*ri);
      double distance = cn->toCoordinate().distance(rn->toCoordinate());
      if (distance < bestDistance)
      {
        bestDistance = distance;
        bestMatch = *ri;
      }
    }
    if (bestDistance < epsilon)
    {
      nodeIdMap[*ci] = bestMatch;
    }
  }

  // replace the nodes in changed with the new nodes we found.
  NodeReplaceVisitor nrv(*_map, nodeIdMap);
  changed->visitRw(*_map, nrv);
}

}
