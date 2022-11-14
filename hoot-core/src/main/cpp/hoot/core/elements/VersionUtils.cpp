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
 * @copyright Copyright (C) 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */

#include "VersionUtils.h"

// Hoot
#include <hoot/core/criterion/AttributeValueCriterion.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/visitors/ElementCountVisitor.h>
#include <hoot/core/visitors/FilteredVisitor.h>
#include <hoot/core/visitors/UniqueElementIdVisitor.h>

namespace hoot
{

int VersionUtils::versionLessThanOneCount(const OsmMapPtr& map)
{
  return
    (int)FilteredVisitor::getStat(
      std::make_shared<AttributeValueCriterion>(
        ElementAttributeType(ElementAttributeType::Version), 1, NumericComparisonType::LessThan),
      std::make_shared<ElementCountVisitor>(),
      map);
}

bool VersionUtils::checkVersionLessThanOneCountAndLogWarning(const OsmMapPtr& map)
{
  const int numberOfRefElementsWithVersionLessThan1 = versionLessThanOneCount(map);
  if (numberOfRefElementsWithVersionLessThan1 > 0)
  {
    LOG_WARN(
      StringUtils::formatLargeNumber(numberOfRefElementsWithVersionLessThan1) << " features in " <<
      "the reference map have a version less than one. This could lead to difficulties when " <<
      "applying the resulting changeset back to an authoritative data store. Are the versions " <<
      "on the features being populated correctly?");
    return true;
  }
  return false;
}

}
