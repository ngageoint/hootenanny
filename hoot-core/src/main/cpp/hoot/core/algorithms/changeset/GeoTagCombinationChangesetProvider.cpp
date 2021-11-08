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
 * @copyright Copyright (C) 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#include "GeoTagCombinationChangesetProvider.h"



namespace hoot
{

void GeoTagCombinationChangesetProvider::close()
{
  // nothing to do here
}

std::shared_ptr<OGRSpatialReference> GeoTagCombinationChangesetProvider::getProjection() const
{
  return _projection;
}

bool GeoTagCombinationChangesetProvider::hasMoreChanges()
{
  return (_changesets[0] != nullptr && _changesets[1] != nullptr) &&
         (_changesets[0]->hasMoreChanges() || _nextChange[0] != nullptr ||
          _changesets[1]->hasMoreChanges() || _nextChange[1] != nullptr);
}

Change GeoTagCombinationChangesetProvider::readNextChange()
{
  //  Get the next change from each changeset
  if (_nextChange[0] == nullptr)
    _nextChange[0] = (_changesets[0]->hasMoreChanges() ? ChangePtr(new Change(_changesets[0]->readNextChange())) : nullptr);
  if (_nextChange[1] == nullptr)
    _nextChange[1] = (_changesets[1]->hasMoreChanges() ? ChangePtr(new Change(_changesets[1]->readNextChange())) : nullptr);
  //  Compare the element IDs
  Change result;
  if (_nextChange[0] == nullptr && _nextChange[1] == nullptr)
    throw hoot::IllegalArgumentException("Cannot read empty changeset elements.");
  else if (_nextChange[0] != nullptr && _nextChange[1] == nullptr)
  {
    //  Grab the next change and remove it
    result = *_nextChange[0];
    _nextChange[0] = nullptr;
  }
  else if (_nextChange[0] == nullptr && _nextChange[1] != nullptr)
  {
    //  Grab the next change and remove it
    result = *_nextChange[1];
    _nextChange[1] = nullptr;
  }
  else
  {
    ElementId geoId = _nextChange[0]->getElement()->getElementId();
    ElementId tagId = _nextChange[1]->getElement()->getElementId();
    if (geoId == tagId)
    {
      //  Combine the two changes
      result = *_nextChange[0];
      result.combineChange(*_nextChange[1]);
      //  Remove both changes from the next queue
      _nextChange[0] = nullptr;
      _nextChange[1] = nullptr;
    }
    else if (geoId < tagId)
    {
      //  Get the geo change next
      result = *_nextChange[0];
      _nextChange[0] = nullptr;
    }
    else
    {
      //  Get the tag change next
      result = *_nextChange[1];
      _nextChange[1] = nullptr;
    }
  }
  return result;
}

}
