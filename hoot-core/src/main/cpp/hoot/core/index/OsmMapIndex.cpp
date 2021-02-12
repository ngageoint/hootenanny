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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020 Maxar (http://www.maxar.com/)
 */

#include "OsmMapIndex.h"

// GEOS
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/LineString.h>
#include <geos/geom/Point.h>

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/elements/NodeToWayMap.h>
#include <hoot/core/index/KnnWayIterator.h>
#include <hoot/core/geometry/ElementToGeometryConverter.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/Validate.h>
#include <hoot/core/util/StringUtils.h>

// Qt
#include <QTime>

// Standard
#include <limits>
#include <algorithm>

// TGS
#include <tgs/RStarTree/MemoryPageStore.h>
#include <tgs/RStarTree/RStarTreePrinter.h>
#include <tgs/RStarTree/IntersectionIterator.h>

using namespace geos::geom;
using namespace std;
using namespace Tgs;

namespace hoot
{

OsmMapIndex::OsmMapIndex(const OsmMap& map) : _map(map)
{
  _indexSlush = 0.0;
}

void OsmMapIndex::addWay(ConstWayPtr w)
{ 
  if (_nodeToWayMap != 0)
  {
    LOG_TRACE("Adding way to index: " << w->getElementId());
    _nodeToWayMap->addWay(w);
    VALIDATE(_nodeToWayMap->validate(_map));
  }

  // rather than manipulate the R-Tree (expensive) we'll just keep track of what changed.
  _pendingWayInsert.insert(w->getId());
  _pendingWayRemoval.erase(w->getId());
}

void OsmMapIndex::addRelation(const ConstRelationPtr& r)
{
  if (_elementToRelationMap != 0)
  {
    LOG_TRACE("Adding relation to index: " << r->getElementId());
    _elementToRelationMap->addRelation(_map, r);
    VALIDATE(validate());
  }
}

void OsmMapIndex::_buildNodeTree() const
{
  QTime t;
  t.start();

  LOG_INFO("Building node R-Tree index");
  // 10 children - 368 - see #3054
  std::shared_ptr<MemoryPageStore> mps(new MemoryPageStore(728));
  _nodeTree.reset(new HilbertRTree(mps, 2));

  vector<Box> boxes;
  vector<int> ids;

  const NodeMap& nodes = _map.getNodes();

  _treeIdToNid.resize(0);
  _treeIdToNid.reserve(nodes.size());
  boxes.reserve(nodes.size());
  ids.reserve(nodes.size());

  Box b(2);
  int count = 0;
  for (NodeMap::const_iterator it = nodes.begin(); it != nodes.end(); ++it, ++count)
  {
    ConstNodePtr n = it->second;

    b.setBounds(0, n->getX(), n->getX());
    b.setBounds(1, n->getY(), n->getY());

    boxes.push_back(b);
    ids.push_back(_createTreeNid(n->getId()));

    if (count % 1000 == 0)
    {
      PROGRESS_DEBUG("  Node R-Tree count: " << count << " / " << nodes.size() << "       ");
    }
  }

  _pendingNodeInsert.clear();
  _pendingNodeRemoval.clear();

  LOG_INFO("\tBulk inserting Node R-Tree...");

  // If this takes a very long time, you can uncomment out the cout statements in HilbertRTree to
  // get a better idea of how longs its going to take.

  _nodeTree->bulkInsert(boxes, ids);

  LOG_INFO(
    "\tNode R-Tree index built. Time elapsed: " << StringUtils::millisecondsToDhms(t.elapsed()));
}

void OsmMapIndex::_buildWayTree() const
{
  QTime t;
  t.start();

  LOG_INFO("Building way R-Tree index...");
  // 10 children - 368 - see #3054
  std::shared_ptr<MemoryPageStore> mps(new MemoryPageStore(728));
  _wayTree.reset(new HilbertRTree(mps, 2));

  vector<Box> boxes;
  vector<int> ids;

  const WayMap& ways = _map.getWays();

  _treeIdToWid.resize(0);
  _treeIdToWid.reserve(ways.size());
  boxes.reserve(ways.size());
  ids.reserve(ways.size());

  Box b(2);
  int count = 0;
  for (WayMap::const_iterator it = ways.begin(); it != ways.end(); ++it, ++count)
  {
    ConstWayPtr w = it->second;

    std::shared_ptr<LineString> ls =
      ElementToGeometryConverter(_map.shared_from_this()).convertToLineString(w);
    if (ls)
    {
      const Envelope* e = ls->getEnvelopeInternal();

      Meters a = w->getCircularError();
      b.setBounds(0, e->getMinX() - a, e->getMaxX() + a);
      b.setBounds(1, e->getMinY() - a, e->getMaxY() + a);

      boxes.push_back(b);
      ids.push_back(_createTreeWid(w->getId()));
    }

    if (count % 1000 == 0)
    {
      PROGRESS_DEBUG("Way R-Tree Index: " << count << " / " << ways.size() << "       ");
    }
  }

  _pendingWayInsert.clear();
  _pendingWayRemoval.clear();

  LOG_INFO("Bulk inserting Way R-Tree...");

  _wayTree->bulkInsert(boxes, ids);

  LOG_INFO("Way R-Tree index built in: " << StringUtils::millisecondsToDhms(t.elapsed()));
}

int OsmMapIndex::_createTreeNid(long nid) const
{
  int result = _treeIdToNid.size();
  _treeIdToNid.push_back(nid);
  return result;
}

int OsmMapIndex::_createTreeWid(long wid) const
{
  int result = _treeIdToWid.size();
  _treeIdToWid.push_back(wid);
  return result;
}

set<ElementId> OsmMapIndex::findWayRelations(const Envelope& e) const
{
  set<ElementId> result;

  const ElementToRelationMap& e2r = *getElementToRelationMap();

  vector<long> wids = findWays(e);
  for (size_t i = 0; i < wids.size(); i++)
  {
    result.insert(ElementId(ElementType::Way, wids[i]));
    const set<long>& relations = e2r.getRelationByElement(ElementId::way(wids[i]));

    for (set<long>::const_iterator it = relations.begin(); it != relations.end(); ++it)
    {
      result.insert(ElementId(ElementType::Relation, *it));
    }
  }

  return result;
}

vector<long> OsmMapIndex::findNodes(const Coordinate& from, Meters maxDistance) const
{
  vector<long> result;
  Box b(2);
  b.setBounds(0, from.x - maxDistance, from.x + maxDistance);
  b.setBounds(1, from.y - maxDistance, from.y + maxDistance);
  KnnIterator it(getNodeTree().get(), from.x, from.y);

  while (it.hasNext() && it.getDistance() <= maxDistance)
  {
    // sometimes the index has more nodes than actually exist.
    if (_map.containsNode(_mapToNodeId(it.getId())))
    {
      result.push_back(_mapToNodeId(it.getId()));
    }
  }

  return result;
}

vector<long> OsmMapIndex::findNodes(const Envelope& e) const
{
  vector<long> result;

  vector<double> min(2), max(2);
  min[0] = e.getMinX();
  min[1] = e.getMinY();
  max[0] = e.getMaxX();
  max[1] = e.getMaxY();
  IntersectionIterator it(getNodeTree().get(), min, max);

  while (it.next())
  {
    long nid = _mapToNodeId(it.getId());
    if (_map.containsNode(nid))
    {
      result.push_back(nid);
    }
  }

  return result;
}

vector<long> OsmMapIndex::findWayNeighbors(const ConstWayPtr &way, Meters buffer,
                                           bool addError) const
{
  vector<long> result;

  KnnWayIterator it(_map, way, getWayTree().get(), _treeIdToWid, addError);

  while (it.hasNext() && it.getDistance() <= buffer)
  {
    long wid = _mapToWayId(it.getId());
    // if this way isn't pending removal.
    if (_map.containsWay(wid))
    {
      result.push_back(wid);
    }
  }

  LOG_TRACE("findWayNeighbors result size: " << result.size());
  LOG_TRACE("distance count: " << it.getDistanceCount());

  return result;
}

vector<long> OsmMapIndex::findWayNeighborsBruteForce(ConstWayPtr way, Meters buffer) const
{
  vector<long> result;

  // grab the geometry for the way that we're comparing all others against.
  std::shared_ptr<LineString> ls1 =
    ElementToGeometryConverter(_map.shared_from_this()).convertToLineString(way);

  // go through all other ways
  for (WayMap::const_iterator it = _map.getWays().begin();
    it != _map.getWays().end(); ++it)
  {
    long nId = it->first;
    ConstWayPtr n = it->second;
    if (n != 0 && nId != way->getId())
    {
      std::shared_ptr<LineString> ls2 =
        ElementToGeometryConverter(_map.shared_from_this()).convertToLineString(n);
      Meters d = ls1->distance(ls2.get());

      if (d < buffer)
      {
        result.push_back(nId);
      }
    }
  }

  return result;
}

long OsmMapIndex::findNearestWay(Coordinate c) const
{
  long result = 0;
  double bestDistance = std::numeric_limits<double>::max();

  // grab the geometry for the way that we're comparing all others against.
  Point* p = GeometryFactory::getDefaultInstance()->createPoint(c);

  // go through all other ways
  for (WayMap::const_iterator it = _map.getWays().begin();
    it != _map.getWays().end(); ++it)
  {
    long nId = it->first;
    ConstWayPtr n = it->second;
    if (n != 0 && n->getNodeCount() > 1)
    {
      std::shared_ptr<LineString> ls2 =
        ElementToGeometryConverter(_map.shared_from_this()).convertToLineString(n);
      Meters d = p->distance(ls2.get());

      if (d < bestDistance)
      {
        result = nId;
        bestDistance = d;
      }
    }
  }

  delete p;

  return result;
}

std::vector<long> OsmMapIndex::findWayNeighbors(Coordinate& from, Meters buffer) const
{
  vector<long> result;

  // grab the geometry for the way that we're comparing all others against.
  Point* p = GeometryFactory::getDefaultInstance()->createPoint(from);

  // go through all other ways
  for (WayMap::const_iterator it = _map.getWays().begin();
    it != _map.getWays().end(); ++it)
  {
    long nId = it->first;
    ConstWayPtr n = it->second;
    if (n != 0 && n->getNodeCount() > 1)
    {
      std::shared_ptr<LineString> ls2 =
        ElementToGeometryConverter(_map.shared_from_this()).convertToLineString(n);
      Meters d = p->distance(ls2.get());

      if (d < buffer)
      {
        result.push_back(nId);
      }
    }
  }

  delete p;

  return result;
}

vector<long> OsmMapIndex::findWays(const Envelope& e) const
{
  vector<long> result;

  vector<double> min(2), max(2);
  min[0] = e.getMinX();
  min[1] = e.getMinY();
  max[0] = e.getMaxX();
  max[1] = e.getMaxY();
  IntersectionIterator it(getWayTree().get(), min, max);

  while (it.next())
  {
    const long wid = _mapToWayId(it.getId());
    // if this way isn't pending removal
    if (_map.containsWay(wid))
    {
      result.push_back(wid);
    }
  }

  return result;
}

const std::shared_ptr<ElementToRelationMap>& OsmMapIndex::getElementToRelationMap() const
{
  if (_elementToRelationMap == 0)
  {
    _elementToRelationMap.reset(new ElementToRelationMap());

    const RelationMap& relations = _map.getRelations();
    for (RelationMap::const_iterator it = relations.begin(); it != relations.end(); ++it)
    {
      _elementToRelationMap->addRelation(_map, it->second);
    }
    VALIDATE(validate());
  }
  return _elementToRelationMap;
}

std::shared_ptr<NodeToWayMap> OsmMapIndex::getNodeToWayMap() const
{
  if (_nodeToWayMap == 0)
  {
    LOG_TRACE("Initializing node to way map with map of size: " << _map.size() << "...");
    _nodeToWayMap.reset(new NodeToWayMap(_map));
  }
  return _nodeToWayMap;
}

std::shared_ptr<const HilbertRTree> OsmMapIndex::getNodeTree() const
{
  if (_nodeTree == 0)
  {
    _buildNodeTree();
  }

  OsmMapIndex* t = const_cast<OsmMapIndex*>(this);
  for (set<long>::const_iterator it = _pendingNodeInsert.begin(); it != _pendingNodeInsert.end();
       ++it)
  {
    if (_map.containsNode(*it))
    {
      t->_insertNode(*it);
    }
  }
  _pendingNodeInsert.clear();

  return _nodeTree;
}

set<ElementId> OsmMapIndex::getParents(ElementId eid) const
{
  set<ElementId> result;

  if (eid.getType() == ElementType::Node)
  {
    const set<long>& ways = getNodeToWayMap()->getWaysByNode(eid.getId());
    for (set<long>::const_iterator it = ways.begin(); it != ways.end(); ++it)
    {
      result.insert(ElementId::way(*it));
    }
  }

  const set<long>& ancestors = getElementToRelationMap()->getRelationByElement(eid);
  LOG_VART(ancestors.size());
  LOG_VART(_map.size());
  for (set<long>::const_iterator it = ancestors.begin(); it != ancestors.end(); ++it)
  {
    LOG_VART(*it);
    if (!_map.containsRelation(*it))
    {
      LOG_INFO("Child element: " << eid);
      LOG_INFO("Missing relation: " << *it);
      LOG_INFO("Child element: " << _map.getElement(eid)->toString());
      continue;
    }
    // the map should contain all the relations returned by the index.
    assert(_map.containsRelation(*it));
    const ConstRelationPtr& r = _map.getRelation(*it);
    LOG_VART(r.get())
    if (r && r->contains(eid))
    {
      result.insert(r->getElementId());
    }
  }

  LOG_VART(result);
  return result;
}

std::shared_ptr<const HilbertRTree> OsmMapIndex::getWayTree() const
{
  if (_wayTree == 0)
  {
    _buildWayTree();
  }

  OsmMapIndex* t = const_cast<OsmMapIndex*>(this);
  for (set<long>::const_iterator it = _pendingWayInsert.begin(); it != _pendingWayInsert.end();
       ++it)
  {
    if (_map.containsWay(*it))
    {
      t->_insertWay(*it);
    }
  }
  _pendingWayInsert.clear();

  return _wayTree;
}

void OsmMapIndex::_insertNode(long nid)
{
  ConstNodePtr n = _map.getNode(nid);

  Box b(2);

  b.setBounds(0, n->getX(), n->getX());
  b.setBounds(1, n->getY(), n->getY());

  _nodeTree->insert(b, _createTreeNid(nid));
}

void OsmMapIndex::_insertWay(long wid)
{
  ConstWayPtr w = _map.getWay(wid);

  Box b(2);

  std::shared_ptr<LineString> ls =
    ElementToGeometryConverter(_map.shared_from_this()).convertToLineString(w);
  const Envelope* e = ls->getEnvelopeInternal();

  b.setBounds(0, e->getMinX() - _indexSlush, e->getMaxX() + _indexSlush);
  b.setBounds(1, e->getMinY() - _indexSlush, e->getMaxY() + _indexSlush);

  _wayTree->insert(b, _createTreeWid(wid));
}

void OsmMapIndex::preGeometryChange(Element* e)
{
  if (_nodeToWayMap != 0)
  {
    VALIDATE(_nodeToWayMap->validate(_map));
  }
  // if the element to relation map is being maintained and this is not a node. Node movement
  // doesn't impact any relation memberships.
  if (_elementToRelationMap != 0 && e->getElementType() != ElementType::Node)
  {
    assert(_pendingRelationChange.size() == 0);
    VALIDATE(_elementToRelationMap->validate(_map));
    // find out which relations will be impacted by the changes.
    _pendingRelationChange = _elementToRelationMap->getRelationByElement(e);

    // if the relation itself is being changed, then add it to the pending list.
    if (e->getElementType() == ElementType::Relation)
    {
      _pendingRelationChange.insert(e->getId());
    }

    // remove all the relations that are being impacted.
    for (set<long>::iterator it = _pendingRelationChange.begin();
      it != _pendingRelationChange.end(); ++it)
    {
      _elementToRelationMap->removeRelation(_map, _map.getRelation(*it));
    }
  }
  if (e->getElementType() == ElementType::Way)
  {
    removeWay(_map.getWay(e->getId()));
  }
}

void OsmMapIndex::postGeometryChange(Element* e)
{
  if (e->getElementType() == ElementType::Way)
  {
    addWay(_map.getWay(e->getId()));
  }
  if (_nodeToWayMap != 0)
  {
    VALIDATE(_nodeToWayMap->validate(_map));
  }
  if (_elementToRelationMap != 0)
  {
    // add all the relations that were impacted back into the index.
    for (set<long>::iterator it = _pendingRelationChange.begin();
         it != _pendingRelationChange.end(); ++it)
    {
      _elementToRelationMap->addRelation(_map, _map.getRelation(*it));
    }
    _pendingRelationChange.clear();
    VALIDATE(_elementToRelationMap->validate(_map));
  }
}

void OsmMapIndex::removeNode(ConstNodePtr n)
{
  if (_nodeTree && n)
  {
    _pendingNodeRemoval.insert(n->getId());
    _pendingNodeInsert.erase(n->getId());

    if (_pendingNodeRemoval.size() > std::max((size_t)100, (size_t)_map.getNodes().size() / 8) &&
        _nodeTree != 0)
    {
      LOG_DEBUG("pending removal size: " << _pendingNodeRemoval.size());
      _nodeTree.reset();
    }
  }
}

void OsmMapIndex::removeRelation(const ConstRelationPtr& r)
{
  if (_elementToRelationMap != 0)
  {
    _elementToRelationMap->removeRelation(_map, r);
  }
}

void OsmMapIndex::removeWay(ConstWayPtr w)
{
  _pendingWayRemoval.insert(w->getId());
  _pendingWayInsert.erase(w->getId());

  if (_nodeToWayMap != 0)
  {
    VALIDATE(_nodeToWayMap->validate(_map));
    _nodeToWayMap->removeWay(w);
  }

  if (_wayTree != 0 &&
      _pendingWayRemoval.size() > std::max((size_t)100, (size_t)_map.getWays().size() / 8))
  {
    LOG_DEBUG("pending removal size: " << _pendingWayRemoval.size());
    _wayTree.reset();
  }
}

void OsmMapIndex::reset()
{
  _pendingWayInsert.clear();
  _pendingWayRemoval.clear();
  _pendingNodeInsert.clear();
  _pendingNodeRemoval.clear();
  _nodeToWayMap.reset();
  _wayTree.reset();
  _nodeTree.reset();
}

bool OsmMapIndex::validate() const
{
  bool result = true;
  if (_nodeToWayMap != 0)
  {
    result &= _nodeToWayMap->validate(_map);
  }
  if (_elementToRelationMap != 0)
  {
    result &= _elementToRelationMap->validate(_map);
  }
  if (result == false)
  {
    LOG_WARN("OsmMapIndex is invalid.");
  }
  return result;
}

}
