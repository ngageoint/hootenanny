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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "BuildingOutlineUpdateOp.h"

// geos
#include <geos/geom/Geometry.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/Polygon.h>
#include <geos/util/TopologyException.h>
#include <geos/opBuffer.h>

// hoot
#include <hoot/core/Factory.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/conflate/NodeToWayMap.h>
#include <hoot/core/elements/ElementVisitor.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/ops/RemoveNodeOp.h>
#include <hoot/core/util/ElementConverter.h>
#include <hoot/core/util/GeometryConverter.h>
#include <hoot/core/util/GeometryUtils.h>
#include <hoot/core/MapProjector.h>
#include <hoot/core/conflate/ReviewMarker.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, BuildingOutlineUpdateOp)

class NodeIdVisitor : public ElementVisitor
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
};

class NodeReplaceVisitor : public ElementVisitor
{
public:
  NodeReplaceVisitor(OsmMap& map, const std::map<long, long>& fromTo) : _fromTo(fromTo), _map(map)
  {}

  virtual void visit(const ConstElementPtr& e)
  {
    if (e->getElementType() == ElementType::Way)
    {
      const shared_ptr<Way>& w = _map.getWay(e->getId());
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

private:
  const map<long, long>& _fromTo;
  OsmMap& _map;
};

BuildingOutlineUpdateOp::BuildingOutlineUpdateOp()
{
}

void BuildingOutlineUpdateOp::apply(shared_ptr<OsmMap>& map)
{
  _map = map;

  // go through all the relations
  const RelationMap& relations = map->getRelationMap();
  for (RelationMap::const_iterator it = relations.begin(); it != relations.end(); it++)
  {
    const shared_ptr<Relation>& r = it->second;
    // add the relation to a building group if appropriate
    if (OsmSchema::getInstance().isBuilding(r->getTags(), r->getElementType()))
    {
      _createOutline(r);
    }
  }
}

void BuildingOutlineUpdateOp::_createOutline(const shared_ptr<Relation>& building)
{
  shared_ptr<Geometry> outline(GeometryFactory::getDefaultInstance()->createEmptyGeometry());

  const vector<RelationData::Entry> entries = building->getMembers();
  for (size_t i = 0; i < entries.size(); i++)
  {
    if (entries[i].role == "outline")
    {
      building->removeElement(entries[i].role, entries[i].getElementId());
    }
    else if (entries[i].role == "part")
    {
      if (entries[i].getElementId().getType() == ElementType::Way)
      {
        shared_ptr<Way> way = _map->getWay(entries[i].getElementId().getId());

        if (way->getNodeCount() >= 4)
        {
          shared_ptr<Geometry> g = ElementConverter(_map).convertToGeometry(way);
          try
          {
            outline.reset(outline->Union(g.get()));
          }
          catch (geos::util::TopologyException& e)
          {
            LOG_VARD(building->toString());
            LOG_DEBUG("Attempting to clean way geometry after union error: " << e.what());
            LOG_VARD(way->toString());
            Geometry* cleanedGeom = GeometryUtils::validateGeometry(g.get());
            try
            {
              outline.reset(outline->Union(cleanedGeom));
            }
            catch (geos::util::TopologyException& e)
            {
              //couldn't clean, so mark parent relation for review (eventually we'll come up with
              //cleaning that works here)
              ElementPtr elem = boost::dynamic_pointer_cast<Element>(building);
              ReviewMarker().mark(
                _map,
                elem,
                "Element with uncleanable topology.  Error occurred during union operation.",
                ReviewMarker::getBadGeometryType());
              LOG_WARN(
                "Element with uncleanable topology.  Error occurred during union operation: " +
                QString(e.what()))
            }
          }
        }
      }
      else if (entries[i].getElementId().getType() == ElementType::Relation)
      {
        shared_ptr<Relation> relation = _map->getRelation(entries[i].getElementId().getId());
        if (relation->isMultiPolygon())
        {
          shared_ptr<Geometry> g = ElementConverter(_map).convertToGeometry(relation);
          outline.reset(outline->Union(g.get()));
        }
        else
        {
          LOG_WARN("Found a building with a non-multipolygon relation 'part'. " <<
            relation->toString());
          LOG_WARN("Building: " << building->toString());
        }
      }
    }
  }

  if (outline->isEmpty() == false)
  {
    const shared_ptr<Element> outlineElement = GeometryConverter(_map).convertGeometryToElement(
      outline.get(), building->getStatus(), building->getCircularError());
    _mergeNodes(outlineElement, building);
    outlineElement->setTags(building->getTags());
    // we don't need the relation "type" tag.
    outlineElement->getTags().remove("type");
    building->addElement("outline", outlineElement);
  }
}

void BuildingOutlineUpdateOp::_mergeNodes(const shared_ptr<Element>& changed,
  const shared_ptr<Relation>& reference)
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
  for (set<long>::const_iterator ci = changedNodes.begin(); ci != changedNodes.end(); ci++)
  {
    double bestDistance = epsilon;
    // should never be used uninitialized
    long bestMatch = -9999;
    const shared_ptr<Node>& cn = _map->getNode(*ci);
    for (set<long>::const_iterator ri = referenceNodes.begin(); ri != referenceNodes.end(); ri++)
    {
      const shared_ptr<Node>& rn = _map->getNode(*ri);
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
