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

#include "OsmMap.h"

// Boost
using namespace boost;

// GEOS
#include <geos/geom/LineString.h>

// Hoot
#include <hoot/core/ConstOsmMapConsumer.h>
#include <hoot/core/OsmMapListener.h>
#include <hoot/core/conflate/NodeToWayMap.h>
#include <hoot/core/elements/ElementVisitor.h>
#include <hoot/core/elements/Node.h>
#include <hoot/core/filters/NodeFilter.h>
#include <hoot/core/filters/WayFilter.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/util/GeometryUtils.h>
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/SignalCatcher.h>
#include <hoot/core/util/Validate.h>
using namespace hoot::elements;

// Qt
#include <QDebug>

namespace hoot {

shared_ptr<OGRSpatialReference> OsmMap::_wgs84;

OsmMap::OsmMap()
{
  setIdGenerator(IdGenerator::getInstance());
  _index.reset(new OsmMapIndex(*this));
  _srs = getWgs84();
}

OsmMap::OsmMap(shared_ptr<const OsmMap> map)
{
  _copy(map);
}

OsmMap::OsmMap(shared_ptr<OsmMap> map)
{
  _copy(map);
}

OsmMap::OsmMap(shared_ptr<OGRSpatialReference> srs)
{
  setIdGenerator(IdGenerator::getInstance());
  _index.reset(new OsmMapIndex(*this));
  _srs = srs;
}

OsmMap::OsmMap(shared_ptr<const OsmMap> map, shared_ptr<OGRSpatialReference> srs)
{
  _copy(map);
  _srs = srs;
}

OsmMap::~OsmMap()
{
}

void OsmMap::append(ConstOsmMapPtr appendFromMap)
{
  if (this == appendFromMap.get())
  {
    throw HootException("Can't append to the same map.");
  }
  if (!getProjection()->IsSame(appendFromMap->getProjection().get()))
  {
    char* wkt1 = 0;
    getProjection()->exportToPrettyWkt(&wkt1);
    QString proj1 = QString::fromAscii(wkt1);
    char* wkt2 = 0;
    appendFromMap->getProjection()->exportToPrettyWkt(&wkt2);
    QString proj2 = QString::fromAscii(wkt2);
    throw HootException(
      "Incompatible maps.  Map being appended to has projection:\n" + proj1 +
      "\nMap being appended from has projection:\n" + proj2);
  }
  _srs = appendFromMap->getProjection();

  const RelationMap& allRelations = appendFromMap->getRelationMap();
  for (RelationMap::const_iterator it = allRelations.begin(); it != allRelations.end(); ++it)
  {
    RelationPtr relation = it->second;
    if (containsElement(ElementId(relation->getElementId())))
    {
      throw HootException("Map already contains this relation: " + relation->toString());
    }
    shared_ptr<Relation> r = shared_ptr<Relation>(new Relation(*relation));
    addRelation(r);
  }

  WayMap::const_iterator it = appendFromMap->_ways.begin();
  while (it != appendFromMap->_ways.end())
  {
    WayPtr way = it->second;
    if (containsElement(ElementId(way->getElementId())))
    {
      throw HootException("Map already contains this way: " + way->toString());
    }
    shared_ptr<Way> w = shared_ptr<Way>(new Way(*way));
    addWay(w);
    ++it;
  }

  NodeMap::const_iterator itn = appendFromMap->_nodes.begin();
  while (itn != appendFromMap->_nodes.end())
  {
    NodePtr node = itn->second;
    if (containsElement(ElementId(node->getElementId())))
    {
      throw HootException("Map already contains this node: " + node->toString());
    }
    NodePtr n = NodePtr(new Node(*node));
    addNode(n);
    ++itn;
  }

  for (size_t i = 0; i < appendFromMap->getListeners().size(); i++)
  {
    shared_ptr<OsmMapListener> l = appendFromMap->getListeners()[i];
    _listeners.push_back(l->clone());
  }
}

void OsmMap::addElement(const shared_ptr<Element>& e)
{
  switch(e->getElementType().getEnum())
  {
  case ElementType::Node:
    addNode(dynamic_pointer_cast<Node>(e));
    break;
  case ElementType::Way:
    addWay(dynamic_pointer_cast<Way>(e));
    break;
  case ElementType::Relation:
    addRelation(dynamic_pointer_cast<Relation>(e));
    break;
  default:
    throw HootException(QString("Unexpected element type: %1").arg(e->getElementType().toString()));
  }
}

void OsmMap::addNode(const boost::shared_ptr<Node>& n)
{
  _idGen->ensureNodeBounds(n->getId());
  _nodes[n->getId()] = n;
  n->registerListener(_index.get());
  _index->addNode(n);
  //_nodeCounter = std::min(n->getId() - 1, _nodeCounter);
}

void OsmMap::addRelation(const shared_ptr<Relation>& r)
{
  VALIDATE(validate());
  _idGen->ensureRelationBounds(r->getId());
  _relations[r->getId()] = r;
  r->registerListener(_index.get());
  _index->addRelation(r);
  VALIDATE(validate());
}

void OsmMap::addWay(const shared_ptr<Way>& w)
{
  _idGen->ensureWayBounds(w->getId());
  _ways[w->getId()] = w;
  w->registerListener(_index.get());
  _index->addWay(w);
  //_wayCounter = std::min(w->getId() - 1, _wayCounter);

  // this is a bit too strict, especially when dealing with MapReduce
//# ifdef DEBUG
//    for (int i = 0; i < w->getNodeCount(); i++)
//    {
//      assert(_nodes.contains(w->getNodeId(i)));
//    }
//# endif
}

OGREnvelope OsmMap::calculateBounds() const
{
  OGREnvelope result;

  bool first = true;
  for (NodeMap::const_iterator it = _nodes.begin(); it != _nodes.end(); ++it)
  {
    const shared_ptr<const Node>& n = it->second;
    if (first)
    {
      result.MinX = result.MaxX = n->getX();
      result.MinY = result.MaxY = n->getY();
      first = false;
    }
    else
    {
      result.MinX = min(n->getX(), result.MinX);
      result.MinY = min(n->getY(), result.MinY);
      result.MaxX = max(n->getX(), result.MaxX);
      result.MaxY = max(n->getY(), result.MaxY);
    }
  }

  return result;
}

geos::geom::Envelope OsmMap::calculateEnvelope() const
{
  auto_ptr<geos::geom::Envelope> e(GeometryUtils::toEnvelope(calculateBounds()));
  return *e;
}

double OsmMap::calculateMaxCircularError() const
{
  double acc = 0.0;
  for (WayMap::const_iterator it = _ways.begin(); it != _ways.end(); ++it)
  {
    const shared_ptr<const Way>& w = it->second;
    acc = max(acc, w->getCircularError());
  }

  for (NodeMap::const_iterator it = _nodes.begin(); it != _nodes.end(); ++it)
  {
    const shared_ptr<const Node>& n = it->second;
    acc = max(acc, n->getCircularError());
  }

  return acc;
}

void OsmMap::clear()
{
  _srs = getWgs84();

  _nodes.clear();
  _ways.clear();
  _relations.clear();

  _index->reset();
  _listeners.clear();
}

bool OsmMap::containsElement(const ElementId& eid) const
{
  return containsElement(eid.getType(), eid.getId());
}

bool OsmMap::containsElement(ElementType type, long id) const
{
  switch (type.getEnum())
  {
  case ElementType::Node:
    return containsNode(id);
  case ElementType::Way:
    return containsWay(id);
  case ElementType::Relation:
    return containsRelation(id);
  default:
    throw HootException(QString("Unexpected element type: %1").arg(type.toString()));
  }
}

bool OsmMap::containsElement(const shared_ptr<const Element>& e) const
{
  return containsElement(e->getElementType(), e->getId());
}

void OsmMap::_copy(boost::shared_ptr<const OsmMap> from)
{
  _idGen = from->_idGen;
  _idGenSp = from->_idGenSp;
  _index.reset(new OsmMapIndex(*this));
  _srs = from->getProjection();

  int i = 0;
  const RelationMap& allRelations = from->getRelationMap();
  for (RelationMap::const_iterator it = allRelations.begin(); it != allRelations.end(); ++it)
  {
    shared_ptr<Relation> r = shared_ptr<Relation>(new Relation(*(it->second)));
    r->registerListener(_index.get());
    _relations[it->first] = r;
    // no need to add it to the index b/c the index is created in a lazy fashion.
    i++;
  }

  WayMap::const_iterator it = from->_ways.begin();
  while (it != from->_ways.end())
  {
    shared_ptr<Way> w = shared_ptr<Way>(new Way(*(it->second)));
    w->registerListener(_index.get());
    _ways[it->first] = w;
    // no need to add it to the index b/c the index is created in a lazy fashion.
    ++it;
  }

  NodeMap::const_iterator itn = from->_nodes.begin();
  while (itn != from->_nodes.end())
  {
    _nodes[itn->first] = shared_ptr<Node>(new Node(*itn->second));
    // no need to add it to the index b/c the index is created in a lazy fashion.
    ++itn;
  }

  for (size_t i = 0; i < from->getListeners().size(); i++)
  {
    shared_ptr<OsmMapListener> l = from->getListeners()[i];
    _listeners.push_back(l->clone());
  }
}

shared_ptr<OsmMap> OsmMap::copyWays(const vector<long>& wIds) const
{
  shared_ptr<OsmMap> result(new OsmMap());
  result->_srs = _srs;

  for (size_t i = 0; i < wIds.size(); i++)
  {
    shared_ptr<const Way> oldWay = getWay(wIds[i]);
    shared_ptr<Way> w = shared_ptr<Way>(new Way(*oldWay));
    w->registerListener(_index.get());
    result->_ways[wIds[i]] = w;

    for (size_t j = 0; j < oldWay->getNodeCount(); j++)
    {
      shared_ptr<const Node> oldNode = getNode(oldWay->getNodeId(j));
      result->_nodes[oldNode->getId()] = shared_ptr<Node>(new Node(*oldNode));
    }
  }

  return result;
}

std::vector<long> OsmMap::filterNodes(const NodeFilter& filter) const
{
  std::vector<long> result;

  for (NodeMap::const_iterator it = _nodes.begin(); it != _nodes.end(); ++it)
  {
    const shared_ptr<const Node>& n = it->second;
    if (filter.isFiltered(n) == false)
    {
      result.push_back(n->getId());
    }
  }

  return result;
}

std::vector<long> OsmMap::filterNodes(const NodeFilter& filter, const Coordinate& from,
                                      Meters maxDistance) const
{
  vector<long> result;
  vector<long> close = getIndex().findNodes(from, maxDistance);

  for (size_t i = 0; i < close.size(); i++)
  {
    const shared_ptr<const Node>& n = getNode(close[i]);
    if (filter.isFiltered(n) == false)
    {
      result.push_back(close[i]);
    }
  }

  return result;
}

vector<long> OsmMap::filterWays(const WayFilter& filter, shared_ptr<const Way> from,
                        Meters maxDistance, bool addError) const
{
  vector<long> close = getIndex().findWayNeighbors(from, maxDistance, addError);
  vector<long> result;

  for (size_t i = 0; i < close.size(); i++)
  {
    const shared_ptr<const Way>& w = getWay(close[i]);
    if (filter.isFiltered(w) == false)
    {
      result.push_back(w->getId());
    }
  }

  return result;
}


std::vector<long> OsmMap::filterWays(const WayFilter& filter) const
{
  std::vector<long> result;

  for (WayMap::const_iterator it = _ways.begin(); it != _ways.end(); ++it)
  {
    const shared_ptr<const Way>& w = it->second;
    if (filter.isFiltered(w) == false)
    {
      result.push_back(w->getId());
    }
  }

  return result;
}

set<ElementId> OsmMap::findElements(const Envelope& e) const
{
  set<ElementId> result;

  const ElementToRelationMap& e2r = *getIndex().getElementToRelationMap();

  vector<long> wids = getIndex().findWays(e);
  for (size_t i = 0; i < wids.size(); i++)
  {
    result.insert(ElementId(ElementType::Way, wids[i]));
    const set<long>& relations = e2r.getRelationByElement(ElementId::way(wids[i]));

    for (set<long>::const_iterator it = relations.begin(); it != relations.end(); ++it)
    {
      result.insert(ElementId(ElementType::Relation, *it));
    }
  }

  vector<long> nodes = getIndex().findNodes(e);

  for (size_t i = 0; i < nodes.size(); i++)
  {
    result.insert(ElementId(ElementType::Node, nodes[i]));
    const set<long>& relations = e2r.getRelationByElement(ElementId::node(nodes[i]));

    for (set<long>::const_iterator it = relations.begin(); it != relations.end(); ++it)
    {
      result.insert(ElementId(ElementType::Relation, *it));
    }
  }

  return result;
}

std::vector<long> OsmMap::findWayByNode(long nodeId) const
{
  std::vector<long> result;

  for (WayMap::const_iterator it = _ways.begin();
    it != _ways.end(); ++it)
  {
    Way* w = it->second.get();
    if (w->hasNode(nodeId))
    {
      result.push_back(w->getId());
    }
  }

  return result;
}

std::vector<long> OsmMap::findWays(QString key, QString value) const
{
  std::vector<long> result;

  for (WayMap::const_iterator it = _ways.begin();
    it != _ways.end(); ++it)
  {
    Way* w = it->second.get();
    if (w->getTags().contains(key) && w->getTags()[key] == value)
    {
      result.push_back(w->getId());
    }
  }

  return result;
}

std::vector<long> OsmMap::findNodes(QString key, QString value) const
{
  std::vector<long> result;
  for (NodeMap::const_iterator it = _nodes.begin(); it != _nodes.end(); ++it)
  {
    NodePtr node = it->second;
    if (node->getTags().contains(key) && node->getTags()[key] == value)
    {
      result.push_back(node->getId());
    }
  }
  return result;
}

ConstElementPtr OsmMap::getElement(const ElementId& eid) const
{
  return getElement(eid.getType(), eid.getId());
}

ElementPtr OsmMap::getElement(const ElementId& eid)
{
  return getElement(eid.getType(), eid.getId());
}

ConstElementPtr OsmMap::getElement(ElementType type, long id) const
{
  switch (type.getEnum())
  {
  case ElementType::Node:
    return getNode(id);
  case ElementType::Way:
    return getWay(id);
  case ElementType::Relation:
    return getRelation(id);
  default:
    throw HootException(QString("Unexpected element type: %1").arg(type.toString()));
  }
}

ElementPtr OsmMap::getElement(ElementType type, long id)
{
  switch (type.getEnum())
  {
  case ElementType::Node:
    return getNode(id);
  case ElementType::Way:
    return getWay(id);
  case ElementType::Relation:
    return getRelation(id);
  default:
    throw HootException(QString("Unexpected element type: %1").arg(type.toString()));
  }
}

size_t OsmMap::getElementCount() const
{
  return getNodeMap().size() + getWays().size() + getRelationMap().size();
}

set<ElementId> OsmMap::getParents(ElementId eid) const
{
  return getIndex().getParents(eid);
}

boost::shared_ptr<OGRSpatialReference> OsmMap::getWgs84()
{
  if (_wgs84 == 0)
  {
    _wgs84.reset(new OGRSpatialReference());
    if (_wgs84->SetWellKnownGeogCS("WGS84") != OGRERR_NONE)
    {
      throw HootException("Error creating EPSG:4326 projection.");
    }
  }
  return _wgs84;
}

bool OsmMap::_listContainsNode(const QList<ElementPtr> l) const
{
  for (int i = 0; i < l.size(); ++i)
  {
    if (l[i]->getElementType() == ElementType::Node)
    {
      return true;
    }
  }

  return false;
}

void OsmMap::removeElement(ElementId eid)
{
  switch (eid.getType().getEnum())
  {
  case ElementType::Node:
    removeNodeFully(eid.getId());
    break;
  case ElementType::Way:
    removeWayFully(eid.getId());
    break;
  case ElementType::Relation:
    removeRelation(eid.getId());
    break;
  default:
    throw HootException(QString("Unexpected element type: %1").arg(eid.toString()));
  }
}

void OsmMap::removeElementNoCheck(ElementType type, long id)
{
  switch (type.getEnum())
  {
  case ElementType::Node:
    removeNodeNoCheck(id);
    break;
  case ElementType::Way:
    removeWay(id);
    break;
  case ElementType::Relation:
    removeRelation(id);
    break;
  default:
    throw HootException(QString("Unexpected element type: %1").arg(type.toString()));
  }
}

void OsmMap::removeNode(long nid)
{
  const shared_ptr<NodeToWayMap>& n2w = getIndex().getNodeToWayMap();
  const set<long>& ways = n2w->getWaysByNode(nid);
  if (ways.size() > 0)
  {
    throw HootException("Removing a node, but it is still part of one or more ways.");
  }
  removeNodeNoCheck(nid);
}

void OsmMap::removeNodeFully(long nId)
{
  // copy the set because we may modify it later.
  set<long> rid = getIndex().getElementToRelationMap()->
      getRelationByElement(ElementId::way(nId));

  for (set<long>::const_iterator it = rid.begin(); it != rid.end(); ++it)
  {
    getRelation(*it)->removeElement(ElementId::node(nId));
  }

  const shared_ptr<NodeToWayMap>& n2w = getIndex().getNodeToWayMap();
  const set<long> ways = n2w->getWaysByNode(nId);

  for (set<long>::const_iterator it = ways.begin(); it != ways.end(); ++it)
  {
    getWay(*it)->removeNode(nId);
  }

  removeNodeNoCheck(nId);

  VALIDATE(validate());
}

void OsmMap::removeNodeNoCheck(long nId)
{
  _index->removeNode(getNode(nId));
  _nodes.erase(nId);
}

void OsmMap::removeRelation(long rId)
{
  if (_relations.find(rId) != _relations.end())
  {
    // determine if this relation is a part of any other relations

    // make a copy of the rids in case the index gets changed.
    const set<long> rids = getIndex().getElementToRelationMap()->
      getRelationByElement(ElementId::relation(rId));

    // remove this relation from all other parent relations.
    for (set<long>::const_iterator it = rids.begin(); it != rids.end(); ++it)
    {
      getRelation(*it)->removeElement(ElementId::relation(rId));
    }

    _index->removeRelation(getRelation(rId));
    _relations.erase(rId);
    VALIDATE(validate());
  }
}

void OsmMap::removeWay(const shared_ptr<const Way>& w)
{
  removeWay(w->getId());
}

void OsmMap::removeWay(long wId)
{
  if (_ways.find(wId) != _ways.end())
  {
    _index->removeWay(getWay(wId));
    _ways.erase(wId);
  }
}

void OsmMap::removeWayFully(long wId)
{
  // copy the set because we may modify it later.
  set<long> rid = getIndex().getElementToRelationMap()->
      getRelationByElement(ElementId::way(wId));

  for (set<long>::const_iterator it = rid.begin(); it != rid.end(); ++it)
  {
    getRelation(*it)->removeElement(ElementId::way(wId));
  }
  removeWay(wId);
  VALIDATE(validate());
}

void OsmMap::removeWays(const WayFilter& filter)
{
  const WayMap ways = getWays();
  for (WayMap::const_iterator it = ways.begin(); it != ways.end(); ++it)
  {
    const shared_ptr<const Way>& w = it->second;
    if (filter.isFiltered(w) == true)
    {
      removeWay(w->getId());
    }
  }
}

void OsmMap::replace(const shared_ptr<const Element>& from, const shared_ptr<Element>& to)
{
  const shared_ptr<NodeToWayMap>& n2w = getIndex().getNodeToWayMap();

  // do some error checking before we add the new element.
  if (from->getElementType() == ElementType::Node && to->getElementType() != ElementType::Node)
  {
    if (n2w->getWaysByNode(from->getId()).size() != 0)
    {
      throw HootException("Trying to replace a node with a non-node when the node is part of a "
        "way.");
    }
  }

  if (from->getElementType() == ElementType::Node && to->getElementType() == ElementType::Node)
  {
    replaceNode(from->getId(), to->getId());
  }
  else
  {
    if (!containsElement(to))
    {
      addElement(to);
    }

    // create a copy of the set b/c we may modify it with replace commands.
    const set<long> rids = getIndex().getElementToRelationMap()->getRelationByElement(from.get());
    for (set<long>::const_iterator it = rids.begin(); it != rids.end(); it++)
    {
      const shared_ptr<Relation>& r = getRelation(*it);
      r->replaceElement(from, to);
    }

    removeElementNoCheck(from->getElementType(), from->getId());
  }
}

void OsmMap::replace(const shared_ptr<const Element>& from, const QList<ElementPtr>& to)
{
  const shared_ptr<NodeToWayMap>& n2w = getIndex().getNodeToWayMap();

  // do some error checking before we add the new element.
  if (from->getElementType() == ElementType::Node &&
    (_listContainsNode(to) == false || to.size() > 1))
  {
    if (n2w->getWaysByNode(from->getId()).size() != 0)
    {
      throw HootException("Trying to replace a node with multiple nodes or a non-node when the "
        "node is part of a way.");
    }
  }

  if (from->getElementType() == ElementType::Node && to.size() == 1 &&
    to[0]->getElementType() == ElementType::Node)
  {
    replaceNode(from->getId(), to[0]->getId());
  }
  else
  {
    for (int i = 0; i < to.size(); ++i)
    {
      if (!containsElement(to[i]))
      {
        addElement(to[i]);
      }
    }

    // create a copy of the set b/c we may modify it with replace commands.
    const set<long> rids = getIndex().getElementToRelationMap()->getRelationByElement(from.get());
    for (set<long>::const_iterator it = rids.begin(); it != rids.end(); it++)
    {
      const shared_ptr<Relation>& r = getRelation(*it);
      r->replaceElement(from, to);
    }

    removeElementNoCheck(from->getElementType(), from->getId());
  }
}

void OsmMap::replaceNode(long oldId, long newId)
{
  // nothing to do
  if (oldId == newId)
  {
    return;
  }

  for (size_t i = 0; i < _listeners.size(); i++)
  {
    _listeners[i]->replaceNodePre(oldId, newId);
  }

  const shared_ptr<NodeToWayMap>& n2w = getIndex().getNodeToWayMap();

  // get a copy of the ways so our changes below don't modify the working set.
  const set<long> ways = n2w->getWaysByNode(oldId);

  VALIDATE(getIndex().getNodeToWayMap()->validate(*this));

  for (set<long>::iterator it = ways.begin(); it != ways.end(); it++)
  {
    const shared_ptr<Way>& w = getWay(*it);


#   ifdef DEBUG
      if (w.get() == NULL)
      {
        LOG_WARN("Way for way id: " << *it << " is null.");
      }
#   endif

    w->replaceNode(oldId, newId);
  }

  // Replace node in any relations it exists in
  _replaceNodeInRelations(oldId, newId);

  if (containsNode(oldId))
  {
    removeNodeNoCheck(oldId);
  }

  VALIDATE(getIndex().getNodeToWayMap()->validate(*this));
}

void OsmMap::setProjection(shared_ptr<OGRSpatialReference> srs)
{
  _srs = srs;
  _index->reset();
}

bool OsmMap::validate(bool strict) const
{
  bool result = true;
  result &= getIndex().getNodeToWayMap()->validate(*this);

  const WayMap& allWays = (*this).getWays();
  for (WayMap::const_iterator it = allWays.begin(); it != allWays.end(); ++it)
  {
    const shared_ptr<const Way>& way = it->second;

    const vector<long>& nids = way->getNodeIds();
    vector<long> missingNodes;
    bool badWay = false;
    for (size_t i = 0; i < nids.size(); i++)
    {
      if (containsNode(nids[i]) == false)
      {
        badWay = true;
        result = false;
        missingNodes.push_back(nids[i]);
      }
    }

    if (badWay)
    {
      LOG_WARN("Way failed validation. Way: " << way->toString());
      LOG_WARN("  missing nodes: " << missingNodes);
    }
  }

  const RelationMap& allRelations = getRelationMap();
  for (RelationMap::const_iterator it = allRelations.begin(); it != allRelations.end(); ++it)
  {
    const shared_ptr<const Relation>& relation = it->second;

    const vector<RelationData::Entry>& members = relation->getMembers();
    vector<RelationData::Entry> missingElements;
    bool badRelation = false;
    for (size_t i = 0; i < members.size(); i++)
    {
      if (containsElement(members[i].getElementId()) == false)
      {
        badRelation = true;
        result = false;
        missingElements.push_back(members[i]);
      }
    }

    if (badRelation)
    {
      LOG_WARN("Relation failed validation. Relation: " << relation->toString());
      LOG_WARN("  missing members: " << missingElements);
    }
  }

  if (strict)
  {
    result &= _index->validate();
  }

  if (strict && result == false)
  {
    throw HootException("OsmMap validation failed. See log for details.");
  }

  if (result == false)
  {
    LOG_WARN("OsmMap is invalid.");
  }

  return result;
}

void OsmMap::visitRo(ElementVisitor& visitor) const
{
  ConstOsmMapConsumer* consumer = dynamic_cast<ConstOsmMapConsumer*>(&visitor);
  if (consumer != 0)
  {
    consumer->setOsmMap(this);
  }

  // make a copy so we can iterate through even if there are changes.
  const NodeMap& allNodes = getNodeMap();
  for (NodeMap::const_iterator it = allNodes.begin(); it != allNodes.end(); ++it)
  {
    if (containsNode(it->first))
    {
      visitor.visit(it->second);
    }
  }

  // make a copy so we can iterate through even if there are changes.
  const WayMap& allWays = getWays();
  for (WayMap::const_iterator it = allWays.begin(); it != allWays.end(); ++it)
  {
    if (containsWay(it->first))
    {
      visitor.visit(it->second);
    }
  }

  // make a copy so we can iterate through even if there are changes.
  const RelationMap& allRelations = getRelationMap();
  for (RelationMap::const_iterator it = allRelations.begin(); it != allRelations.end(); ++it)
  {
    if (containsRelation(it->first))
    {
      visitor.visit(it->second);
    }
  }
}

void OsmMap::visitRw(ElementVisitor& visitor)
{
  OsmMapConsumer* consumer = dynamic_cast<OsmMapConsumer*>(&visitor);
  if (consumer != 0)
  {
    consumer->setOsmMap(this);
  }

  // make a copy so we can iterate through even if there are changes.
  const NodeMap allNodes = getNodeMap();
  for (NodeMap::const_iterator it = allNodes.begin(); it != allNodes.end(); ++it)
  {
    if (containsNode(it->first))
    {
      visitor.visit(it->second);
    }
  }

  // make a copy so we can iterate through even if there are changes.
  const WayMap allWays = getWays();
  for (WayMap::const_iterator it = allWays.begin(); it != allWays.end(); ++it)
  {
    if (containsWay(it->first))
    {
      visitor.visit(it->second);
    }
  }

  // make a copy so we can iterate through even if there are changes.
  const RelationMap allRelations = getRelationMap();
  for (RelationMap::const_iterator it = allRelations.begin(); it != allRelations.end(); ++it)
  {
    if (containsRelation(it->first))
    {
      visitor.visit(it->second);
    }
  }
}

void OsmMap::_replaceNodeInRelations(long oldId, long newId)
{
  RelationMap allRelations = getRelationMap();
  const ElementId oldNodeId = ElementId::node(oldId);

  LOG_DEBUG("Replace node in relations: replace " << oldId << " with " << newId );

  ConstElementPtr emptyElement;
  NodeMap::iterator it;

  // Make sure both nodes exist; calling getNode on non-existent IDs causes failed assert

  it = _nodes.find(oldId);
  if (it == _nodes.end())
  {
    //LOG_WARN("Tried to replace a non-existent node " << oldId );
    return;
  }

  it = _nodes.find(newId);
  if ( it == _nodes.end() )
  {
    //LOG_WARN("Replacement node " << newId << "does not exist");
    return;
  }

  ConstElementPtr oldNode = getNode(oldId);
  ConstElementPtr newNode = getNode(newId);

  for ( RelationMap::iterator it = allRelations.begin(); it != allRelations.end(); ++it)
  {
    RelationPtr currRelation = it->second;

    if ( currRelation->contains(oldNodeId) == true )
    {
      LOG_DEBUG("Trying to replace node " << oldNode->getId() << " with node " <<
                newNode->getId() << " in relation " << currRelation->getId());

      currRelation->replaceElement(oldNode, newNode);
    }
  }
}

}
