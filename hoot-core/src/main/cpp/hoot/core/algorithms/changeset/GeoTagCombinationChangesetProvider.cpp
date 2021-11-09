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
  return (_geoChangeset != nullptr && _geoChangeset->hasMoreChanges()) ||
         (_tagChangeset != nullptr && _tagChangeset->hasMoreChanges());
}

Change GeoTagCombinationChangesetProvider::readNextChange()
{
  Change result;
  if (_geoChangeset->hasMoreChanges())
  {
    //  Get the next change from the geo changeset
    result = _geoChangeset->readNextChange();
    ElementId eid = result.getElement()->getElementId();
    //  Check if the tag changeset has additional changes
    if (_tagChangeset->containsChange(eid))
    {
      //  Combine the two changes into one
      Change tags = _tagChangeset->getChange(eid);
      result.combineChange(tags);
      _tagChangeset->removeChange(tags);
    }
  }
  else if (_tagChangeset->hasMoreChanges())
    result = _tagChangeset->readNextChange();
  return result;
}

}
