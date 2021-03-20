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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef ELEMENT_IDS_VISITOR_H
#define ELEMENT_IDS_VISITOR_H

#include <hoot/core/visitors/ConstElementVisitor.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/criterion/TagCriterion.h>

namespace hoot
{

/**
 * Used to get a collection of unordered IDs for the elements that satisfy the specified criterion
 *
 * @todo Some of the utility methods may be redundant with related methods in WayUtils and TagUtils.
 * Regardless, they should all be moved to other util classes that then call into this class.
 * @todo I think these methods should be returning set<long> instead of a vector.
 */
class ElementIdsVisitor : public ConstElementVisitor
{
public:

  static QString className() { return "hoot::ElementIdsVisitor"; }

  ElementIdsVisitor(const ElementType& elementType);
  ElementIdsVisitor(const ElementType& elementType, ElementCriterion* pCrit);
  ~ElementIdsVisitor() = default;

  void visit(const std::shared_ptr<const Element>& e) override;

  std::vector<long> getIds() { return _elementIds; }

  /**
   * Retrieves the IDs of all elements of a given type
   *
   * @param map map owning the elements
   * @param elementType type of element to retrieve
   * @return a collection of numerical element IDs
   */
  static std::vector<long> findElements(const ConstOsmMapPtr& map, const ElementType& elementType);

  /**
   * Retrieves the IDs of all elements of a given type passing specified criteria
   *
   * @param map map owning the elements
   * @param elementType type of element to retrieve
   * @param pCrit criteria to satisfy
   * @return a collection of numerical element IDs
   */
  static std::vector<long> findElements(const ConstOsmMapPtr& map, const ElementType& elementType,
                                        ElementCriterion* pCrit);

  /**
   * Retrieves the IDs of nodes satifying specified criteria and within a radius of a specified
   * location
   *
   * @param map map owning the elements
   * @param pCrit criteria to satisfy
   * @param refCoord the point out from which to search
   * @param maxDistance the furthest distance away from the source point to search
   * @return a collection of numerical node IDs
   */
  static std::vector<long> findNodes(const ConstOsmMapPtr& map, ElementCriterion* pCrit,
                                     const geos::geom::Coordinate& refCoord, Meters maxDistance);

  /**
   * Retrieves the IDs of ways satifying specified criteria and within a radius of a specified
   * location
   *
   * @param map map owning the elements
   * @param pCrit criteria to satisfy
   * @param refCoord the point out from which to search
   * @param maxDistance the furthest distance away from the source point to search
   * @param addError adds element circular error to the search radius
   * @return a collection of numerical way IDs
   */
  static std::vector<long> findWays(const ConstOsmMapPtr& map, ElementCriterion* pCrit,
                                    ConstWayPtr refWay, Meters maxDistance, bool addError);

  /**
   * Retrieves the IDs of elements of a given type having a specified tag
   *
   * @param map map owning the elements
   * @param elementType ype of element to retrieve
   * @param key tag key
   * @param value tag value
   * @return a collection of numerical element IDs
   */
  static std::vector<long> findElementsByTag(const ConstOsmMapPtr& map,
                                             const ElementType& elementType, const QString& key,
                                             const QString& value);

  /**
   * Retrieves the IDs of all ways owning a specified node
   *
   * @param map map owning the elements
   * @param nodeId node ID to search for
   * @return a collection of numerical way IDs
   */
  static std::vector<long> findWaysByNode(const ConstOsmMapPtr& map, long nodeId);

  QString getDescription() const override { return "Collects the element IDs visited"; }

  QString getName() const override { return className(); }

  QString getClassName() const override { return className(); }

private:

  // matching IDs; This should probably be a set, b/c there shouldn't be any dupe element IDs for
  // the same element type in a map.
  std::vector<long> _elementIds;
  ElementType _elementType;
  ElementCriterion* _pCrit;

  static std::vector<long> _findCloseNodes(const ConstOsmMapPtr& map,
                                           const geos::geom::Coordinate& refCoord,
                                           Meters maxDistance);
  static std::vector<long> _findCloseWays(const ConstOsmMapPtr& map, ConstWayPtr refWay,
                                          Meters maxDistance, bool addError);
  static std::vector<long> _findElements(const ConstOsmMapPtr& map, ElementCriterion* pCrit,
                                         const std::vector<long>& closeElementIds);
};

}

#endif // ELEMENT_IDS_VISITOR_H
