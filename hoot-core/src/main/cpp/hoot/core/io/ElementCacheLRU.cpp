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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2021, 2022 Maxar (http://www.maxar.com/)
 */

#include <hoot/core/elements/Element.h>


#include "ElementCacheLRU.h"

namespace hoot
{

ElementCacheLRU::ElementCacheLRU(const unsigned long maxNodeCount,
                                 const unsigned long maxWayCount,
                                 const unsigned long maxRelationCount)
  : _maxNodeCount(maxNodeCount),
    _maxWayCount(maxWayCount),
    _maxRelationCount(maxRelationCount),
    _projection(),
    _nodes(),
    _nodesIter(_nodes.begin()),
    _ways(),
    _waysIter(_ways.begin()),
    _relations(),
    _relationsIter(_relations.begin()),
    _diskCache()
{
  LOG_TRACE(
    "New LRU cache created, " << _maxNodeCount << " max entries for nodes, " <<
    _maxWayCount << " max entries for ways, " << _maxRelationCount << " max entries for relations.");
}

bool ElementCacheLRU::isEmpty() const
{
  return (_nodes.empty() && _ways.empty() && _relations.empty());
}

unsigned long ElementCacheLRU::size() const
{
  return (_nodes.size() + _ways.size() + _relations.size());
}

void ElementCacheLRU::addElement(ConstElementPtr &newElement)
{
  LOG_TRACE("Adding element: " + newElement->toString() + " to cache...");

  ConstNodePtr newNode;
  ConstWayPtr newWay;
  ConstRelationPtr newRelation;

  switch (newElement->getElementType().getEnum())
  {
  case ElementType::Node:
    newNode = std::dynamic_pointer_cast<const Node>(newElement);
    if (newNode != ConstNodePtr())
    {
      // Do we have to replace an entry?
      if (_nodes.size() == _maxNodeCount)
        _removeOldest(ElementType::Node);
      _nodeList.push_front(newNode->getId());
      _nodes.insert(std::make_pair(newNode->getId(), std::make_pair(newNode, _nodeList.begin())));
      resetNodeIterator(); // maintain interface contract
    }
    break;
  case ElementType::Way:
    newWay = std::dynamic_pointer_cast<const Way>(newElement);
    if (newWay != ConstWayPtr())
    {
      // Do we have to replace an entry?
      if (_ways.size() == _maxWayCount)
        _removeOldest(ElementType::Way);
      _wayList.push_front(newWay->getId());
      _ways.insert(std::make_pair(newWay->getId(), std::make_pair(newWay, _wayList.begin())));
      resetWayIterator(); // maintain interface contract
    }
    break;
  case ElementType::Relation:
    newRelation = std::dynamic_pointer_cast<const Relation>(newElement);
    if (newRelation != ConstRelationPtr())
    {
      // Do we have to replace an entry?
      if (_relations.size() == _maxRelationCount)
        _removeOldest(ElementType::Relation);
      _relationList.push_front(newRelation->getId());
      _relations.insert(std::make_pair(newRelation->getId(), std::make_pair(newRelation, _relationList.begin())));
      resetRelationIterator(); // maintain interface contract
    }
    break;
  default:
    throw HootException(QString("Unexpected element type: %1").arg(newElement->getElementType().toString()));
    break;
  }
}

void ElementCacheLRU::close()
{
  _nodes.clear();
  _nodeList.clear();
  _ways.clear();
  _wayList.clear();
  _relations.clear();
  _relationList.clear();
  resetElementIterators();
}

bool ElementCacheLRU::hasMoreElements()
{
  return ((_nodesIter != _nodes.end()) || (_waysIter != _ways.end()) || (_relationsIter != _relations.end()));
}

ElementPtr ElementCacheLRU::readNextElement()
{
  ConstElementPtr returnElement;
  if (hasMoreElements())
  {
    if (_nodesIter != _nodes.end())
      returnElement = std::dynamic_pointer_cast<const Element>(getNextNode());
    else if (_waysIter != _ways.end())
      returnElement = std::dynamic_pointer_cast<const Element>(getNextWay());
    else
      returnElement = std::dynamic_pointer_cast<const Element>(getNextRelation());
  }
  return std::const_pointer_cast<Element>(returnElement);
}

void ElementCacheLRU::writeElement(ElementPtr& element)
{
  ConstElementPtr el = element;
  addElement(el);
}

ConstNodePtr ElementCacheLRU::getNextNode()
{
  ConstNodePtr returnPtr;
  if (_nodesIter != _nodes.end())
  {
    // Update our usage
    _updateNodeAccess(_nodesIter->first);
    // Get return node
    returnPtr = _nodesIter->second.first;
    ++_nodesIter;
  }
  return returnPtr;
}

ConstWayPtr ElementCacheLRU::getNextWay()
{
  ConstWayPtr returnPtr;
  if (_waysIter != _ways.end())
  {
    // Update our usage
    _updateWayAccess(_waysIter->first);
    // Get return way
    returnPtr = _waysIter->second.first;
    ++_waysIter;
  }
  return returnPtr;
}

ConstRelationPtr ElementCacheLRU::getNextRelation()
{
  ConstRelationPtr returnPtr;
  if (_relationsIter != _relations.end())
  {
    // Update our usage
    _updateRelationAccess(_relationsIter->first);
    // Get return relation
    returnPtr = _relationsIter->second.first;
    ++_relationsIter;
  }
  return returnPtr;
}

void ElementCacheLRU::resetElementIterators()
{
  _nodesIter = _nodes.begin();
  _waysIter = _ways.begin();
  _relationsIter = _relations.begin();
}

void ElementCacheLRU::_removeOldest(const ElementType::Type typeToRemove)
{
  long id = 0;
  ConstElementPtr tmpElement;
  switch (typeToRemove)
  {
  case ElementType::Node:
    // Get ID of oldest & remove
    id = _nodeList.back();
    tmpElement = std::dynamic_pointer_cast<const Element>(_nodes[id].first);
    _diskCache.addElement(tmpElement);
    _nodeList.pop_back();
    _nodes.erase(id);
    LOG_TRACE("Removed node: " << id << " from cache.");
    break;
  case ElementType::Way:
    id = _wayList.back();
    tmpElement = std::dynamic_pointer_cast<const Element>(_ways[id].first);
    _diskCache.addElement(tmpElement);
    _wayList.pop_back();
    _ways.erase(id);
    LOG_TRACE("Removed way: " << id << " from cache.");
    break;
  case ElementType::Relation:
    id = _relationList.back();
    tmpElement = std::dynamic_pointer_cast<const Element>(_relations[id].first);
    _diskCache.addElement(tmpElement);
    _relationList.pop_back();
    _relations.erase(id);
    LOG_TRACE("Removed relation: " << id << " from cache.");
    break;
  default:
    throw HootException(QString("Tried to remove oldest of invalid type"));
    break;
  }
}

std::shared_ptr<OGRSpatialReference> ElementCacheLRU::getProjection() const
{
  return _projection;
}

bool ElementCacheLRU::containsElement(const ElementId& eid) const
{
  return (_isInMem(eid) || _isOnDisk(eid));
}

ConstElementPtr ElementCacheLRU::getElement(const ElementId& eid) const
{
  const long id = eid.getId();
  switch (eid.getType().getEnum())
  {
  case ElementType::Node:
    return getNode(id);
    break;
  case ElementType::Way:
    return getWay(id);
    break;
  case ElementType::Relation:
    return getRelation(id);
    break;
  default:
    break;
  }
  return ConstElementPtr();
}

// This function is const, but it needs to modify some members
// to achieve the LRU functionality, as well as use the disk cache.
// So it calls some non-const functions in a way that I'm not proud of.
ConstNodePtr ElementCacheLRU::getNode(long id) const
{
  ElementCacheLRU * pThis = const_cast<ElementCacheLRU *>(this);
  if (_isInMem(ElementId(ElementType::Node, id)))
  {
    pThis->_updateNodeAccess(id);
    return _nodes.find(id)->second.first;
  }
  else
  {
    LOG_TRACE("Disk Cache for node (const): " << id );
    return pThis->_diskCache.getNode(id);
  }
}

NodePtr ElementCacheLRU::getNode(long id)
{
  if (_isInMem(ElementId(ElementType::Node, id)))
  {
    _updateNodeAccess(id);
    return std::const_pointer_cast<Node>(_nodes.find(id)->second.first);
  }
  else
  {
    LOG_TRACE("Disk cache for node: " << id);
    return _diskCache.getNode(id);
  }
}

// This function is const, but it needs to modify some members
// to achieve the LRU functionality, as well as use the disk cache.
// So it calls some non-const functions in a way that I'm not proud of.
ConstWayPtr ElementCacheLRU::getWay(long id) const
{
  ElementCacheLRU * pThis = const_cast<ElementCacheLRU *>(this);
  if (_isInMem(ElementId(ElementType::Way, id)))
  {
    pThis->_updateWayAccess(id);
    return _ways.find(id)->second.first;
  }
  else
  {
    LOG_TRACE("Disk cache for way (const): " << id);
    return pThis->_diskCache.getWay(id);
  }
}

WayPtr ElementCacheLRU::getWay(long id)
{
  if (_isInMem(ElementId(ElementType::Way, id)))
  {
    _updateWayAccess(id);
    return std::const_pointer_cast<Way>(_ways.find(id)->second.first);
  }
  else
  {
    LOG_TRACE("Disk cache for way: " << id);
    return _diskCache.getWay(id);
  }
}

// This function is const, but it needs to modify some members
// to achieve the LRU functionality, as well as use the disk cache.
// So it calls some non-const functions in a way that I'm not proud of.
ConstRelationPtr ElementCacheLRU::getRelation(long id) const
{
  ElementCacheLRU * pThis = const_cast<ElementCacheLRU *>(this);
  if (_isInMem(ElementId(ElementType::Relation, id)))
  {
    pThis->_updateRelationAccess(id);
    return _relations.find(id)->second.first;
  }
  else
  {
    LOG_TRACE("Disk cache for relation (const): " << id);
    return pThis->_diskCache.getRelation(id);
  }
}

RelationPtr ElementCacheLRU::getRelation(long id)
{
  _updateRelationAccess(id);
  if (_isInMem(ElementId(ElementType::Relation, id)))
  {
    return std::const_pointer_cast<Relation>(_relations.find(id)->second.first);
  }
  else
  {
    LOG_TRACE("Disk cache for relation: " << id);
    return _diskCache.getRelation(id);
  }
}

bool ElementCacheLRU::containsNode(long id) const
{
  ElementId eid(ElementType::Node, id);
  return (_isInMem(eid) || _isOnDisk(eid));
}

bool ElementCacheLRU::containsWay(long id) const
{
  ElementId eid(ElementType::Way, id);
  return (_isInMem(eid) || _isOnDisk(eid));
}

bool ElementCacheLRU::containsRelation(long id) const
{
  ElementId eid(ElementType::Relation, id);
  return (_isInMem(eid) || _isOnDisk(eid));
}

unsigned long ElementCacheLRU::typeCount(const ElementType::Type typeToCount) const
{
  switch (typeToCount)
  {
  case ElementType::Node:
    return _nodes.size();
    break;
  case ElementType::Way:
    return _ways.size();
    break;
  case ElementType::Relation:
    return _relations.size();
    break;
  default:
    throw HootException("Invalid type passed");
    break;
  }
  return 0;
}

void ElementCacheLRU::removeElement(const ElementId &eid)
{
  switch (eid.getType().getEnum())
  {
  case ElementType::Node:
    _nodeList.erase(_nodes.find(eid.getId())->second.second);
    _nodes.erase(_nodes.find(eid.getId()));
    resetNodeIterator();
    break;
  case ElementType::Way:
    _wayList.erase(_ways.find(eid.getId())->second.second);
    _ways.erase(_ways.find(eid.getId()));
    resetWayIterator();
    break;
  case ElementType::Relation:
    _relationList.erase(_relations.find(eid.getId())->second.second);
    _relations.erase(_relations.find(eid.getId()));
    resetRelationIterator();
    break;
  default:
    throw HootException("Invalid type passed");
    break;
  }
}

void ElementCacheLRU::removeElements(const ElementType::Type type)
{
  switch (type)
  {
  case ElementType::Node:
    _nodeList.clear();
    _nodes.erase(_nodes.begin(), _nodes.end());
    resetNodeIterator();
    break;
  case ElementType::Way:
    _wayList.clear();
    _ways.erase(_ways.begin(), _ways.end());
    resetWayIterator();
    break;
  case ElementType::Relation:
    _relationList.clear();
    _relations.erase(_relations.begin(), _relations.end());
    resetRelationIterator();
    break;
  default:
    throw HootException("Invalid type passed");
    break;
  }
}

void ElementCacheLRU::_updateNodeAccess(long id)
{
  auto nodeItr = _nodes.find(id);
  _nodeList.erase(nodeItr->second.second);
  _nodeList.push_front(nodeItr->first);
  nodeItr->second.second = _nodeList.begin();
}

void ElementCacheLRU::_updateWayAccess(long id)
{
  auto wayItr = _ways.find(id);
  _wayList.erase(wayItr->second.second);
  _wayList.push_front(wayItr->first);
  wayItr->second.second = _wayList.begin();
}

void ElementCacheLRU::_updateRelationAccess(long id)
{
  auto relItr = _relations.find(id);
  _relationList.erase(relItr->second.second);
  _relationList.push_front(relItr->first);
  relItr->second.second = _relationList.begin();
}

std::string ElementCacheLRU::getLRUString(const ElementType::Type type) const
{
  std::ostringstream list;
  switch (type)
  {
  case ElementType::Node:
    for (auto node_id : _nodeList)
      list << node_id << ",";
    break;
  case ElementType::Way:
    for (auto way_id : _wayList)
      list << way_id << ",";
    break;
  case ElementType::Relation:
    for (auto relation_id : _relationList)
      list << relation_id << ",";
    break;
  default:
    throw HootException("Invalid type passed");
    break;
  }
  return list.str();
}

bool ElementCacheLRU::_isInMem(const ElementId& eid) const
{
  bool found = false;
  const ElementType::Type type = eid.getType().getEnum();
  const long id = eid.getId();
  switch (type)
  {
  case ElementType::Node:
    found = (_nodes.find(id) != _nodes.end());
    break;
  case ElementType::Way:
    found = (_ways.find(id) != _ways.end());
    break;
  case ElementType::Relation:
    found = (_relations.find(id) != _relations.end());
    break;
  default:
    break;
  }

  return found;
}

bool ElementCacheLRU::_isOnDisk(const ElementId& eid) const
{
  return _diskCache.containsElement(eid);
}

}
