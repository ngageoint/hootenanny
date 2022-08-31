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
 * @copyright Copyright (C) 2016, 2017, 2018, 2021 Maxar (http://www.maxar.com/)
 */
#include "PowerLineCriterion.h"

// hoot
#include <hoot/core/criterion/PowerLineWayNodeCriterion.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/Factory.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementCriterion, PowerLineCriterion)

bool PowerLineCriterion::isSatisfied(const ConstElementPtr& e) const
{
  if (e->getElementType() == ElementType::Way)
  {
    const Tags& tags = e->getTags();
    const QString powerVal = tags.get("power").toLower().trimmed();
    return (powerVal == "line" || powerVal == "minor_line" || powerVal == "cable");
  }
  return false;
}

QStringList PowerLineCriterion::getChildCriteria() const
{
  return QStringList(PowerLineWayNodeCriterion::className());
}

}

