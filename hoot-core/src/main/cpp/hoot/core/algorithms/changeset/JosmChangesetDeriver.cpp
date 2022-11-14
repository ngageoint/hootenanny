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
 * @copyright Copyright (C) 2022 Maxar (http://www.maxar.com/)
 */

#include "JosmChangesetDeriver.h"

namespace hoot
{

JosmChangesetDeriver::JosmChangesetDeriver(ElementInputStreamPtr from, ElementInputStreamPtr to)
  : ChangesetDeriver(from, to)
{
}

Change JosmChangesetDeriver::_nextChange()
{
  Change result;

  if (!_fromE && _from->hasMoreElements())
  {
    _fromE = _from->readNextElement();
    _numFromElementsParsed++;
  }
  if (!_toE && _to->hasMoreElements())
  {
    _toE = _to->readNextElement();
    _numToElementsParsed++;
  }

  if (!_fromE && _toE)  // If we've run out of "from" elements, create all the remaining elements in "to".
  {
    result = Change(Change::ChangeType::Create, _toE);
    _advanceTo();
  }
  else if (_fromE && !_toE) // If we've run out of "to" elements, delete all the remaining elements in "from".
  {
    result = Change(Change::ChangeType::Delete, _fromE);
    _advanceFrom();
  }
  else if (!_fromE && !_toE)  //  We've run out of both elements
  {
    //  Pass
  }
  else if (_fromE->getElementId() == _toE->getElementId())  //  Both elements have the same ID
  {
    if (_fromE->getTags().hasAnyKey(_metadataAllowKeys) != _toE->getTags().hasAnyKey(_metadataAllowKeys))
      result = Change(Change::ChangeType::Modify, _toE, _fromE->getStatus());
    else if (_elementComparer.isSame(_fromE, _toE))
      result = Change(Change::ChangeType::NoChange, _toE, _fromE->getStatus());
    else
      result = Change(Change::ChangeType::Modify, _toE, _fromE->getStatus());

    _advanceTo();
    _advanceFrom();
  }
  else if (_fromE->getElementId() < _toE->getElementId())
  {
    if ((_allowDeletingReferenceFeatures ||
        // this assumes the 'from' dataset was loaded as unknown1
        // TODO: Don't understand the use case for this and its been around for awhile now...need
        // to define and add a test
        (!_allowDeletingReferenceFeatures && _fromE->getStatus() != Status::Unknown1)) &&
        !_fromE->getTags().contains(MetadataTags::HootChangeExcludeDelete()))
    {
      result = Change(Change::ChangeType::Delete, _fromE, _toE->getStatus());
    }
    else
    {
      // The changeset provider will return no more changes if a null change is returned here.  We
      // want to force no changes for this particular element, so we're going to use the unknown
      // change type, which ends up being a no-op.  Skipping an element delete in this situation
      // minimizes the changeset impact on reference datasets in certain situations.
      result = Change(Change::ChangeType::Unknown, _fromE, _toE->getStatus());
    }
    _advanceFrom();
  }
  else
  {
    result = Change(Change::ChangeType::Create, _toE, _fromE->getStatus());
    _advanceTo();
  }

  LOG_VART(result);
  return result;
}

}
