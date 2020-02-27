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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef ELEMENT_IDS_VISITOR_H
#define ELEMENT_IDS_VISITOR_H

#include <hoot/core/elements/ConstElementVisitor.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/criterion/TagCriterion.h>

namespace hoot
{

/**
 * Used to get a collection of IDs for the elements that satisfy the specified criterion
 */
class ElementIdsVisitor : public ConstElementVisitor
{
public:

  static std::string className() { return "hoot::ElementIdsVisitor"; }

  ElementIdsVisitor(const ElementType& elementType, ElementCriterion* pCrit);

  void visit(const std::shared_ptr<const Element>& e) override;

  // Get matching IDs
  std::vector<long> getIds() { return _elementIds; }

  static std::vector<long> findElements(const ConstOsmMapPtr& map, const ElementType& elementType,
                                        ElementCriterion* pCrit);

  static std::vector<long> findNodes(const ConstOsmMapPtr& map, ElementCriterion* pCrit,
                                     const geos::geom::Coordinate& refCoord, Meters maxDistance);

  static std::vector<long> findWays(const ConstOsmMapPtr& map, ElementCriterion* pCrit,
                                    ConstWayPtr refWay, Meters maxDistance, bool addError);

  // Convenience method for finding elements that contain the given tag
  static std::vector<long> findElementsByTag(const ConstOsmMapPtr& map,
                                             const ElementType& elementType, const QString& key,
                                             const QString& value);

  static std::vector<long> findWaysByNode(const ConstOsmMapPtr& map, long nodeId);

  virtual QString getDescription() const { return "Collects the element IDs visited"; }

  virtual std::string getClassName() const { return className(); }

private:

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
