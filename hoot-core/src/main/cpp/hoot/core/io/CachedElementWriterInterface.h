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
 * @copyright Copyright (C) 2023 Maxar (http://www.maxar.com/)
 */

#ifndef CACHED_ELEMENT_WRITER_INTERFACE_H
#define CACHED_ELEMENT_WRITER_INTERFACE_H

// hoot
#include <hoot/core/io/ElementCache.h>

namespace hoot
{

/**
 * Interface for writers that must cache nodes, ways, and relations.
 */
class CachedElementWriterInterface
{
public:

  static QString className() { return "OgrWriter"; }

  CachedElementWriterInterface();
  virtual ~CachedElementWriterInterface() = default;

  void setCache(ElementCachePtr cachePtr) { _elementCache = cachePtr; }
  /** Get max size of the cache for each element type */
  unsigned long getNodeCacheSize() const { return _elementCache->getNodeCacheSize(); }
  unsigned long getWayCacheSize() const { return _elementCache->getWayCacheSize(); }
  unsigned long getRelationCacheSize() const { return _elementCache->getRelationCacheSize(); }
  /** Check the cache for a specific element */
  bool cacheContainsElement(const ElementId& eid) const { return _elementCache->containsElement(eid); }
  /** Get elements from the cache by ID or ElementId */
  NodePtr getNode(long id) const { return _elementCache->getNode(id); }
  WayPtr getWay(long id) const { return _elementCache->getWay(id); }
  RelationPtr getRelation(long id) const { return _elementCache->getRelation(id); }
  ConstElementPtr getElement(const ElementId& eid) const { return _elementCache->getElement(eid); }

protected:
  /** Internal function to add an element to the cache */
  void _addElementToCache(const ConstElementPtr& element);
  /** Internal function to convert the element cache into an element provider */
  ElementProviderPtr _getElementProvider() const;
  /** Element cache pointer, see ElementCacheLRU */
  ElementCachePtr _elementCache;
};

}

#endif // CACHED_ELEMENT_WRITER_INTERFACE_H
