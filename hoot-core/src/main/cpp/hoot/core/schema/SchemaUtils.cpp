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
 * @copyright Copyright (C) 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "SchemaUtils.h"

// Hoot
#include <hoot/core/visitors/ElementCountVisitor.h>
#include <hoot/core/visitors/FilteredVisitor.h>
#include <hoot/core/criterion/HasTypeCriterion.h>
#include <hoot/core/criterion/NoInformationCriterion.h>

namespace hoot
{

bool SchemaUtils::anyElementsHaveType(const ConstOsmMapPtr& map)
{
  return
    (int)FilteredVisitor::getStat(
      std::shared_ptr<HasTypeCriterion>(new HasTypeCriterion()),
      std::shared_ptr<ElementCountVisitor>(new ElementCountVisitor()), map) > 0;
}

bool SchemaUtils::allElementsHaveType(const ConstOsmMapPtr& map)
{
  const int elementsWithNoInfoCount =
    (int)FilteredVisitor::getStat(
       std::shared_ptr<NoInformationCriterion>(new NoInformationCriterion()),
       std::shared_ptr<ElementCountVisitor>(new ElementCountVisitor()), map);
  const int elementsWithTypeCount =
    (int)FilteredVisitor::getStat(
       std::shared_ptr<HasTypeCriterion>(new HasTypeCriterion()),
       std::shared_ptr<ElementCountVisitor>(new ElementCountVisitor()), map);
  return (elementsWithTypeCount - elementsWithNoInfoCount) == (int)map->size();
}

}
