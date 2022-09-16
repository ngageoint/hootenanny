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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
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

Change::Change(ChangeType type, ConstElementPtr element, ConstElementPtr previousElement)
  : _type(type),
    _element(element),
    _previousElement(previousElement)
{
  //  Update the change status based on the two elements and the change type
  _updateStatus(element, previousElement->getStatus());
}

Change::Change(ChangeType type, ConstElementPtr element, Status status)
  : _type(type),
    _element(element)
{
  //  Update the change status based on the two elements and the change type
  _updateStatus(element, status);
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
  case ChangeType::NoChange:
    return "NoChange";
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

void Change::_updateStatus(ConstElementPtr element, Status status)
{
  Status::Type element_status = element->getStatus().getEnum();
  Status::Type previous_status = status.getEnum();
  switch(_type)
  {
  case ChangeType::Create:
    //  Can't create anything from Unknown1
    if (element_status == Status::Unknown1)
      _setElementStatus(previous_status);
    else if (previous_status == Status::Conflated)
      _setElementStatus(previous_status);
    break;
  case ChangeType::Delete:
    //  Deleted should always be Unknown1
    if (element_status != Status::Unknown1)
      _setElementStatus(Status::Unknown1);
    break;
  case ChangeType::Modify:
    //  Modified should either be Unknown1 or Conflated
  case ChangeType::NoChange:
    //  This element isn't conflated but the other is, use conflated status
    if (element_status != Status::Conflated && previous_status == Status::Conflated)
      _setElementStatus(status);
    else if (element_status == Status::Unknown2)  //  No change came as Unknown2, change it to Unknown1
      _setElementStatus(Status::Unknown1);
    break;
  default:
    break;
  }
}

void Change::_setElementStatus(Status status)
{
  ElementPtr e = _element->clone();
  e->setStatus(status);
  _element = e;
}

}
