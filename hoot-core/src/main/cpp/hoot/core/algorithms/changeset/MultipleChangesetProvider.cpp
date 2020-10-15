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
 * @copyright Copyright (C) 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "MultipleChangesetProvider.h"

#include <hoot/core/util/Log.h>

namespace hoot
{

void MultipleChangesetProvider::close()
{
  // nothing to do here
}

std::shared_ptr<OGRSpatialReference> MultipleChangesetProvider::getProjection() const
{
  return _projection;
}

bool MultipleChangesetProvider::hasMoreChanges()
{
  if (_changesets.size() < 1)
  {
    return false;
  }
  else if (_changesets.front()->hasMoreChanges())
  {
    return true;
  }
  else
  {
    // Advance to next changeset and check
    _changesets.pop_front();
    return hasMoreChanges();
  }
}

Change MultipleChangesetProvider::readNextChange()
{
  return _changesets.front()->readNextChange();
}

void MultipleChangesetProvider::addChangesetProvider(ChangesetProviderPtr newChangeset)
{
  _changesets.push_back(newChangeset);
}

size_t MultipleChangesetProvider::getNumChangesets()
{
  return _changesets.size();
}

}
