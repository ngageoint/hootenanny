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
#include <hoot/core/MapProjector.h>
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
#include <hoot/core/ops/RemoveElementOp.h>
#include <hoot/core/ops/RemoveNodeOp.h>
using namespace hoot::elements;

// Qt
#include <QDebug>

namespace hoot {

shared_ptr<OGRSpatialReference> OsmMap::_wgs84;

OsmMap::OsmMap()
{
  setIdGenerator(IdGenerator::getInstance());
  _index.reset(new OsmMapIndex(*this));
  _srs = MapProjector::createWgs84Projection();
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

void OsmMap::clear()
{
  _srs = MapProjector::createWgs84Projection();

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

    RemoveElementOp::removeElementNoCheck(shared_from_this(), from->getElementId());
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

    RemoveElementOp::removeElementNoCheck(shared_from_this(), from->getElementId());
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
    RemoveNodeOp::removeNodeNoCheck(shared_from_this(), oldId);
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
  visitNodesRo(visitor);
  visitWaysRo(visitor);
  visitRelationsRo(visitor);
}

void OsmMap::visitNodesRo(ElementVisitor& visitor) const
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
}

void OsmMap::visitWaysRo(ElementVisitor& visitor) const
{
  ConstOsmMapConsumer* consumer = dynamic_cast<ConstOsmMapConsumer*>(&visitor);
  if (consumer != 0)
  {
    consumer->setOsmMap(this);
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
}

void OsmMap::visitRelationsRo(ElementVisitor& visitor) const
{
  ConstOsmMapConsumer* consumer = dynamic_cast<ConstOsmMapConsumer*>(&visitor);
  if (consumer != 0)
  {
    consumer->setOsmMap(this);
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

void OsmMap::visitWaysRw(ElementVisitor& visitor)
{
  OsmMapConsumer* consumer = dynamic_cast<OsmMapConsumer*>(&visitor);
  if (consumer != 0)
  {
    consumer->setOsmMap(this);
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
