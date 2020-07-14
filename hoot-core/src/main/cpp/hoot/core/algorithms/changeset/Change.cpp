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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "Change.h"

#include <hoot/core/util/Log.h>

namespace hoot
{

Change::Change() :
_type(Unknown)
{
}

Change::Change(ChangeType type, ConstElementPtr element) :
_type(type),
_element(element)
{
}

Change::Change(ChangeType type, ConstElementPtr element, ConstElementPtr reviousElement) :
_type(type),
_element(element),
_previousElement(reviousElement)
{
}

QString Change::changeTypeToString(const ChangeType changeType)
{
  switch (changeType)
  {
    case Create:
      return "Create";
    case Modify:
      return "Modify";
    case Delete:
      return "Delete";
    case Unknown:
      return "Unknown";
    default:
      throw HootException("Invalid change type.");
  }
}

bool Change::operator==(const Change& other) const
{
  return _type == other.getType() && other.getElement()->getElementId() == _element->getElementId();
}

QString Change::toString() const
{
  return
    "Change type: " + changeTypeToString(_type) + ", ID: " + _element->getElementId().toString();
}

}
