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

#include "RelationMemberComparison.h"

namespace hoot
{

RelationMemberComparison::RelationMemberComparison(ElementPtr element, const OsmMap& sourceMap,
                                                   const QString& role, const bool ignoreElementId) :
ElementComparison(element, sourceMap, ignoreElementId),
_role(role)
{
}

bool RelationMemberComparison::operator==(const RelationMemberComparison& memberComp) const
{
  const bool equals =
    this->_role == memberComp.getRole() &&
    _elementComparer.isSame(this->_element, memberComp.getElement());
  if (equals)
  {
    LOG_TRACE(
      "Roles and members equal: " << this->_element->getElementId() << " and " <<
      memberComp.getElement()->getElementId())
  }
  return equals;
}

QString RelationMemberComparison::toString() const
{
  return _role + " " + _element->getElementId().toString();
}

}
