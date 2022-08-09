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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#include "Change.h"

namespace hoot
{

Change::Change()
  : _type(ChangeType::Unknown)
{
}

Change::Change(ChangeType type, ConstElementPtr element)
  : _type(type),
    _element(element)
{
}

Change::Change(ChangeType type, ConstElementPtr element, ConstElementPtr reviousElement)
  : _type(type),
    _element(element),
    _previousElement(reviousElement)
{
}

QString Change::changeTypeToString(const ChangeType changeType)
{
  switch (changeType)
  {
  case ChangeType::Create:
    return "Create";
  case ChangeType::Modify:
    return "Modify";
  case ChangeType::Delete:
    return "Delete";
  case ChangeType::Unknown:
    return "Unknown";
  default:
    throw HootException("Invalid change type.");
  }
}

QString Change::toString() const
{
  QString str =  "Change type: " + changeTypeToString(_type);
  if (_element)
    str += ", ID: " + _element->getElementId().toString();
  return str;
}

void Change::combineChange(const Change& tag_change)
{
  //  Validate the two changes before attempting to combine them
  if (_type != tag_change._type ||
      _element == nullptr || tag_change._element == nullptr ||
      _element->getElementId() != tag_change._element->getElementId())
    return;
  //  Combine this element of geometry and the tag changes
  ElementPtr result = _element->clone();
  result->setTags(tag_change.getElement()->getTags());
  _element = result;
}

}
