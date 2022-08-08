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
 * @copyright Copyright (C) 2015, 2017, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#ifndef ELEMENTTORELATIONMAP_H
#define ELEMENTTORELATIONMAP_H

// Tgs
#include <tgs/HashMap.h>

// Hoot
#include <hoot/core/elements/ElementId.h>

namespace hoot
{

class OsmMap;
class Relation;
class Element;

/**
 * Provides a mapping from all elements that are directly or indirectly part of a relation back to
 * the parent relation. An element may be part of multiple relations.
 */
class ElementToRelationMap
{
public:

  ElementToRelationMap() = default;

  /**
   * Recursively traverses the relation and adds all child elements to the reference.
   */
  void addRelation(const OsmMap& map, const std::shared_ptr<const Relation>& r);

  /**
   * Returns a set of relation ids that have the specified element as a member, explicity or
   * implicitly through the ancestry. E.g. If there is a building relation that contains a
   * multipolygon and the multipolygon contains a way w/ id 3, when this is called with
   * (ElementType::Way, 3) you will get both the multipolygon and the building relation.
   */
  const std::set<long>& getRelationByElement(ElementId eid) const;
  const std::set<long>& getRelationByElement(const std::shared_ptr<const Element>& e) const;
  const std::set<long>& getRelationByElement(const Element* e) const;

  /**
   * This function assumes that the elements that make up the relation haven't changed since it was
   * last added.
   */
  void removeRelation(const OsmMap &map, const std::shared_ptr<const Relation>& r);

  /**
   * Checks to make sure the index is consistent with the specified map. All inconsistencies are
   * logged as warning. If it checks out true is returned, otherwise false is returned. No
   * exceptions should be thrown.
   */
  bool validate(const OsmMap& map) const;

  size_t size() const { return _mapping.size(); }

private:

  std::set<long> _emptySet;
  HashMap<ElementId, std::set<long>> _mapping;
};

}

#endif // ELEMENTTORELATIONMAP_H
