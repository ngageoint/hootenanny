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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef ELEMENTCACHELRU_H
#define ELEMENTCACHELRU_H

#include <map>
#include <utility>

#include <boost/date_time/posix_time/posix_time.hpp>

#include <hoot/core/elements/ElementType.h>
#include <hoot/core/elements/ElementId.h>
#include <hoot/core/io/ElementCache.h>

#include <ogr_spatialref.h>

namespace hoot
{

/**
 * Implements the ElementCache interface, where the least-recently-used
 * items get pushed out of the cache when it is full. Element insert,
 * retrieval, and expiration are all constant-time operations.
 */
class ElementCacheLRU : public ElementCache
{

public:

  /**
   * @brief ElementCacheLRU
   * @param maxNodeCount Total size of node cache before items start being replaced.
   * @param maxWayCount Total size of ways cache before items start being replaced.
   * @param maxRelationCount Total size of relation cache before items start being replaced.
   */
  ElementCacheLRU(const unsigned long maxNodeCount,
                  const unsigned long maxWayCount,
                  const unsigned long maxRelationCount);

  /**
   * @brief ~ElementCache
   */
  virtual ~ElementCacheLRU() = default;

  virtual bool isEmpty() const;

  virtual unsigned long size() const;

  virtual unsigned long typeCount(const ElementType::Type typeToCount) const;

  /**
   * @brief addElement
   * @param newElement
   * @note The last access time for an element added to the cache will be initialized to the
   *    current time
   */
  virtual void addElement(ConstElementPtr& newElement);

  virtual void resetElementIterators();

  virtual ConstNodePtr getNextNode();

  virtual ConstWayPtr getNextWay();

  virtual ConstRelationPtr getNextRelation();

  // Functions for ElementInputStream
  virtual void close();
  virtual bool hasMoreElements();

  virtual ElementPtr readNextElement();

  // Functions for ElementOutputStream
  virtual void writeElement(ElementPtr& element);

  // Functions for ElementProvider

  virtual std::shared_ptr<OGRSpatialReference> getProjection() const;

  virtual bool containsElement(const ElementId& eid) const;

  virtual ConstElementPtr getElement(const ElementId& id) const;

  virtual const ConstNodePtr getNode(long id) const;

  virtual const NodePtr getNode(long id);

  virtual const ConstRelationPtr getRelation(long id) const;

  virtual const RelationPtr getRelation(long id);

  virtual const ConstWayPtr getWay(long id) const;

  virtual const WayPtr getWay(long id);

  virtual bool containsNode(long id) const;

  virtual bool containsRelation(long id) const;

  virtual bool containsWay(long id) const;

  // Cache-specific items
  virtual void removeElement(const ElementId& eid);

  virtual void removeElements(const ElementType::Type type);

  virtual unsigned long getNodeCacheSize() { return _maxNodeCount; }

  virtual unsigned long getWayCacheSize() { return _maxWayCount; }

  virtual unsigned long getRelationCacheSize() { return _maxRelationCount; }

  // For testing - gets a comma-seperated list of IDs of the Least Recently Used
  // cache items, from most recent to least recent.
  std::string getLRUString(const ElementType::Type type);

private:

  unsigned long _maxNodeCount;
  unsigned long _maxWayCount;
  unsigned long _maxRelationCount;

  std::shared_ptr<OGRSpatialReference> _projection;

  // List used to keep track of least-recently used nodes
  std::list<long> _nodeList;

  // Nodes in the cache (key is node ID, then value is pair for node and its pos in the nodeList)
  std::map<long, std::pair<ConstNodePtr, std::list<long>::iterator>> _nodes;

  // Iterator used to walk nodes in cache
  std::map<long, std::pair<ConstNodePtr, std::list<long>::iterator>>::iterator _nodesIter;

  // List used to keep track of least-recently used nodes
  std::list<long> _wayList;

  // Ways in the cache (key is way ID, then value is pair for way and its pos in the wayList)
  std::map<long, std::pair<ConstWayPtr, std::list<long>::iterator>> _ways;

  // Iterator used to walk ways in cache
  std::map<long, std::pair<ConstWayPtr, std::list<long>::iterator>>::iterator _waysIter;

  // List used to keep track of least-recently used nodes
  std::list<long> _relationList;

  // Relations in the cache (key is relation ID, then value is pair for relation and access time)
  std::map<long, std::pair<ConstRelationPtr, std::list<long>::iterator>>  _relations;

  // Iterator used to walk relations in cache
  std::map<long, std::pair<ConstRelationPtr, std::list<long>::iterator>>::iterator _relationsIter;

  // Removes the least recently used item from the cache
  void _removeOldest(const ElementType::Type typeToRemove);

  // These functions update the lists that keep the order of when items
  // were last used/accessed
  void _updateNodeAccess(long id);
  void _updateWayAccess(long id);
  void _updateRelationAccess(long id);
};

using ElementCacheLRUPtr = std::shared_ptr<ElementCacheLRU>;

}

#endif // ELEMENTCACHELRU_H
