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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#include "HighwayCriterion.h"

// hoot
#include <hoot/core/criterion/AreaCriterion.h>
#include <hoot/core/criterion/BuildingCriterion.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/criterion/HighwayWayNodeCriterion.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementCriterion, HighwayCriterion)

HighwayCriterion::HighwayCriterion(const bool includeRelations) :
_includeRelations(includeRelations)
{
}

HighwayCriterion::HighwayCriterion(ConstOsmMapPtr map, const bool includeRelations) :
_map(map),
_includeRelations(includeRelations)
{
}

bool HighwayCriterion::isSatisfied(const ConstElementPtr& element) const
{
  if (!element)
  {
    return false;
  }

  LOG_VART(element->getElementId());
  const ElementType type = element->getElementType();

  if (type == ElementType::Node)
  {
    return false;
  }

  bool result = false;
  const Tags& tags = element->getTags();

  Tags::const_iterator it = tags.find("highway");
  const bool containsHighwayTag = it != tags.end() && it.value() != "";

  // Is it a legit highway?
  if ((type == ElementType::Way || (type == ElementType::Relation && _includeRelations)) &&
      containsHighwayTag)
  {
    result = true;
    LOG_VART(result);
  }
  //  Make sure it isn't a building or a part of a building.
  else if (_map && (BuildingCriterion(_map).isSatisfied(element) ||
                    BuildingCriterion(_map).isParentABuilding(element->getElementId())))
  {
    result = false;
    LOG_VART(result);
  }

  // Make sure this isn't an area highway section.
  if (result)
  {
    result = !AreaCriterion().isSatisfied(element);
    LOG_VART(result);
  }

  LOG_TRACE(element->getElementId() << " result: " << result);
  return result;
}

QStringList HighwayCriterion::getChildCriteria() const
{
  return QStringList(HighwayWayNodeCriterion::className());
}

}

