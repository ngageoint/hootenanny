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
 * @copyright Copyright (C) 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "NonConflatableCriterion.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/elements/Element.h>
#include <hoot/core/criterion/ConflatableElementCriterion.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementCriterion, NonConflatableCriterion)

NonConflatableCriterion::NonConflatableCriterion() :
_ignoreChildren(false),
_geometryTypeFilter(GeometryTypeCriterion::GeometryType::Unknown)
{
}

NonConflatableCriterion::NonConflatableCriterion(ConstOsmMapPtr map) :
_map(map),
_ignoreChildren(false),
_geometryTypeFilter(GeometryTypeCriterion::GeometryType::Unknown)
{
}

void NonConflatableCriterion::setConfiguration(const Settings& conf)
{
  ConfigOptions config = ConfigOptions(conf);
  _ignoreChildren = config.getNonConflatableCriterionIgnoreRelationMembers();
  LOG_VARD(_ignoreChildren);
}

bool NonConflatableCriterion::isSatisfied(const ConstElementPtr& e) const
{
  const QMap<QString, ElementCriterionPtr> conflatableCriteria =
    ConflatableElementCriterion::getConflatableCriteria();
  for (QMap<QString, ElementCriterionPtr>::const_iterator itr = conflatableCriteria.begin();
       itr != conflatableCriteria.end(); ++itr)
  {
    ElementCriterionPtr crit = itr.value();

    if (_map)
    {
      ConstOsmMapConsumer* mapConsumer = dynamic_cast<ConstOsmMapConsumer*>(crit.get());
      if (mapConsumer != 0)
      {
        mapConsumer->setOsmMap(_map.get());
      }
    }

    bool satisfiesGeometryFilter = true;
    if (_geometryTypeFilter != GeometryTypeCriterion::GeometryType::Unknown)
    {
      std::shared_ptr<GeometryTypeCriterion> geometryCrit =
        std::dynamic_pointer_cast<GeometryTypeCriterion>(crit);
      satisfiesGeometryFilter = geometryCrit->getGeometryType() == _geometryTypeFilter;
    }

    if (crit->isSatisfied(e) && satisfiesGeometryFilter)
    {
      // It is something we can conflate.
      LOG_TRACE("Element: " << e->getElementId() << " is conflatable with: " << itr.key());
      return false;
    }
  }

  // Technically, there could also be something like a building way with a POI child and you'd want
  // to check for ways here as well. Will wait to support that situation until an actual use case
  // is encountered.
  if (!_ignoreChildren && e->getElementType() == ElementType::Relation)
  {
    // We need to verify that none of the child relation members are conflatable in order to sign
    // off on this element not being conflatable.

    ConstRelationPtr relation = std::dynamic_pointer_cast<const Relation>(e);
    const std::vector<RelationData::Entry>& members = relation->getMembers();
    for (size_t i = 0; i < members.size(); i++)
    {
      const RelationData::Entry& member = members[i];
      ConstElementPtr memberElement = _map->getElement(member.getElementId());
      if (memberElement && isSatisfied(memberElement))
      {
        // It is something we can conflate.
        LOG_TRACE(
          "Element: " << e->getElementId() << " has a child that is conflatable: " <<
          memberElement->getElementId() << ", and member children are not being ignored, " <<
          "therefore it is conflatable.");
        return false;
      }
    }
  }

  // It is not something we can conflate.
  LOG_TRACE("Element: " << e->getElementId() << " is not conflatable.");
  LOG_VART(e);
  return true;
}

}
