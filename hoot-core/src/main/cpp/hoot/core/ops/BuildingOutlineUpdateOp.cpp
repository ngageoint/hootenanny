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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
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
#include <hoot/core/ops/RemoveNodeOp.h>
#include <hoot/core/elements/ElementConverter.h>
#include <hoot/core/util/GeometryConverter.h>
#include <hoot/core/util/GeometryUtils.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/criterion/BuildingCriterion.h>
#include <hoot/core/ops/RecursiveElementRemover.h>

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

BuildingOutlineUpdateOp::BuildingOutlineUpdateOp() :
_removeBuildingRelations(false)
{
}

void BuildingOutlineUpdateOp::setConfiguration(const Settings& conf)
{
  _removeBuildingRelations =
    ConfigOptions(conf).getBuildingOutlineUpdateOpRemoveBuildingRelations();
}

void BuildingOutlineUpdateOp::apply(boost::shared_ptr<OsmMap> &map)
{
  _map = map;
  _buildingRelationIds.clear();

  // go through all the relations
  const RelationMap& relations = map->getRelations();
  for (RelationMap::const_iterator it = relations.begin(); it != relations.end(); ++it)
  {
    const RelationPtr& r = it->second;
    // add the relation to a building group if appropriate
    if (BuildingCriterion().isSatisfied(r))
    {
      _createOutline(r);
      _buildingRelationIds.insert(r->getElementId());
    }
  }

  if (_removeBuildingRelations)
  {
    _deleteBuildingRelations();
  }
}

void BuildingOutlineUpdateOp::_deleteBuildingRelations()
{
  for (std::set<ElementId>::const_iterator it = _buildingRelationIds.begin(); it !=
       _buildingRelationIds.end(); ++it)
  {
    RecursiveElementRemover(*it).apply(_map);
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
      LOG_TRACE("Removing outline role from: " << entries[i] << "...");
      building->removeElement(entries[i].role, entries[i].getElementId());
    }
    else if (entries[i].role == MetadataTags::RolePart())
    {
      if (entries[i].getElementId().getType() == ElementType::Way)
      {
        WayPtr way = _map->getWay(entries[i].getElementId().getId());
        if (way->getNodeCount() >= 4)   // TODO: put this in a config?
        {
          LOG_TRACE("Unioning building part: " << way << "...");

          // TODO: Consolidate this with other duplicated code in this method as part of #3024.
          boost::shared_ptr<Geometry> g = ElementConverter(_map).convertToGeometry(way);
          try
          {
            outline.reset(outline->Union(g.get()));
          }
          catch (const geos::util::TopologyException& e)
          {
            LOG_TRACE("Attempting to clean way geometry after union error: " << e.what());
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
      }
      else if (entries[i].getElementId().getType() == ElementType::Relation)
      {
        RelationPtr relation = _map->getRelation(entries[i].getElementId().getId());
        if (relation->isMultiPolygon())
        {
          LOG_TRACE("Unioning multipoly relation: " << relation << "...");

          // TODO: Consolidate this with other duplicated code in this method as part of #3024.
          boost::shared_ptr<Geometry> g = ElementConverter(_map).convertToGeometry(relation);
          try
          {
            outline.reset(outline->Union(g.get()));
          }
          catch (const geos::util::TopologyException& e)
          {
            LOG_TRACE("Attempting to clean way geometry after union error: " << e.what());
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

  if (!outline->isEmpty())
  {
    LOG_TRACE("Adding building outline element...");

    const boost::shared_ptr<Element> outlineElement =
      GeometryConverter(_map).convertGeometryToElement(
        outline.get(), building->getStatus(), building->getCircularError());
    _mergeNodes(outlineElement, building);
    outlineElement->setTags(building->getTags());;
    // We don't need the relation "type" tag.
    outlineElement->getTags().remove("type");
    LOG_VART(outlineElement);
    if (!_removeBuildingRelations)
    {
      building->addElement(MetadataTags::RoleOutline(), outlineElement);
    }
  }

  LOG_VART(building);
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
