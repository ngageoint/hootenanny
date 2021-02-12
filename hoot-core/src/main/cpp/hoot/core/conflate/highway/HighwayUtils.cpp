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
 * @copyright Copyright (C) 2020 Maxar (http://www.maxar.com/)
 */

#include "HighwayUtils.h"

// Hoot
#include <hoot/core/criterion/OneWayCriterion.h>

namespace hoot
{

bool HighwayUtils::oneWayConflictExists(const ConstElementPtr& element1,
                                        const ConstElementPtr& element2)
{
  // Technically, this should also take into account reverse one ways and check direction.  Since
  // we have a map pre-op standardizing all the ways to not be reversed, not worrying about it for
  // now.
  OneWayCriterion isAOneWayStreet;
  return
    (isAOneWayStreet.isSatisfied(element1) && explicitlyNotAOneWayStreet(element2)) ||
    (isAOneWayStreet.isSatisfied(element2) && explicitlyNotAOneWayStreet(element1));
}

bool HighwayUtils::explicitlyNotAOneWayStreet(const ConstElementPtr& element)
{
  // TODO: use Tags::isFalse here instead
  return element->getTags().get("oneway") == "no";
}

bool HighwayUtils::nonGenericHighwayConflictExists(const ConstElementPtr& element1,
                                                   const ConstElementPtr& element2)
{
  const QString element1HighwayVal = element1->getTags().get("highway");
  const QString element2HighwayVal = element2->getTags().get("highway");
  return
    element1HighwayVal != "road" && element2HighwayVal != "road" &&
    element1HighwayVal != element2HighwayVal;
}

}
