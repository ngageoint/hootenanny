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
 * @copyright Copyright (C) 2020, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef CRITERION_UTILS_H
#define CRITERION_UTILS_H

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/criterion/ElementCriterion.h>
#include <hoot/core/elements/ConstElementVisitor.h>
#include <hoot/core/visitors/ElementCountVisitor.h>
#include <hoot/core/visitors/FilteredVisitor.h>
#include <hoot/core/visitors/UniqueElementIdVisitor.h>

namespace hoot
{

/**
 * Helper function for using ElementCriterion with elements
 */
class CriterionUtils
{

public:

  /**
   * Determines whether a map contains a minimum or a fixed amount of elements matching the
   * criterion type. Only objects of type ElementCriterion are allowed, all others will return
   * false.
   *
   * @param map the map to examine
   * @param minCount the minmal count of elements required (if exactCount == false)
   * @param exactCount if true, the count must be exactly minCount
   * @return true if the map meets the specified criteria; false otherwise
   * @todo move this to MapUtils?
   */
  template<class C>
  static bool containsSatisfyingElements(
     const ConstOsmMapPtr& map, int minCount = 1, bool exactCount = false)
  {
    if (!std::is_base_of<ElementCriterion,C>::value)
    {
      throw IllegalArgumentException(
        "Non-criterion passed to CriterionUtils::containsSatisfyingElements");
    }

    ElementCriterionPtr crit(new C());
    std::shared_ptr<ConstOsmMapConsumer> mapConsumer =
      std::dynamic_pointer_cast<ConstOsmMapConsumer>(crit);
    if (mapConsumer)
    {
      mapConsumer->setOsmMap(map.get());
    }

    const long count =
      (long)FilteredVisitor::getStat(
        crit, ConstElementVisitorPtr(new ElementCountVisitor()), map);
    LOG_VART(count);
    return exactCount ? (count == minCount) : (count >= minCount);
  }

  /**
   * Determines whether a collection of elements meet a criterion a minimum or a fixed amount of
   * times. Only objects of type ElementCriterion are allowed, all others will return false
   *
   * @param elements the elements to examine
   * @param map required to satisfy OsmMapConsumers
   * @param minCount the minimal count of elements required (if exactCount == false)
   * @param exactCount if true, the count must be exactly minCount
   * @return true if the elements meet the specified criterion the specified number of times
   * @todo move this to MapUtils?
   */
  template<class C>
  static bool containsSatisfyingElements(
    const std::vector<ConstElementPtr>& elements, const ConstOsmMapPtr& map, int minCount = 1,
    bool exactCount = false)
  {
    if (!std::is_base_of<ElementCriterion,C>::value)
    {
      throw IllegalArgumentException(
        "Non-criterion passed to CriterionUtils::getSatisfyingElementIds");
    }

    ElementCriterionPtr crit(new C());
    std::shared_ptr<ConstOsmMapConsumer> mapConsumer =
      std::dynamic_pointer_cast<ConstOsmMapConsumer>(crit);
    if (mapConsumer)
    {
      mapConsumer->setOsmMap(map.get());
    }

    int count = 0;
    for (std::vector<ConstElementPtr>::const_iterator itr = elements.begin(); itr != elements.end();
         ++itr)
    {
      if (crit->isSatisfied(*itr))
      {
        count++;
      }
    }

    LOG_VART(count);
    return exactCount ? (count == minCount) : (count >= minCount);
  }

  /**
   * Returns all element IDs that satisfy a criterion
   *
   * @param map the map to examine
   * @return a collection of element IDs
   */
  template<class C>
  static std::set<ElementId> getSatisfyingElementIds(const ConstOsmMapPtr& map)
  {
    if (!std::is_base_of<ElementCriterion,C>::value)
    {
      throw IllegalArgumentException(
        "Non-criterion passed to CriterionUtils::getSatisfyingElementIds");
    }

    ElementCriterionPtr crit(new C());
    std::shared_ptr<ConstOsmMapConsumer> mapConsumer =
      std::dynamic_pointer_cast<ConstOsmMapConsumer>(crit);
    if (mapConsumer)
    {
      mapConsumer->setOsmMap(map.get());
    }

    std::shared_ptr<UniqueElementIdVisitor> idVis(new UniqueElementIdVisitor());
    FilteredVisitor filteredVis(crit, idVis);
    map->visitRo(filteredVis);
    return idVis->getElementSet();
  }

  /**
   * Determines if an element has a given criterion
   *
   * @param element the element to examine
   * @param criterionClassName class name of the ElementCriterion to determine membership of
   * @return true if the element has the criterion; false otherwise
   * @throws if the criterion class name is invalid
   * @todo merge this with isSatisifed
   */
  static bool hasCriterion(const ConstElementPtr& element, const QString& criterionClassName);

private:

  static ElementCriterionPtr _getCrit(const QString& criterionClassName);
};

}

#endif // CRITERION_UTILS_H
