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
 * @copyright Copyright (C) 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "MemChangesetProvider.h"

#include <hoot/core/util/Log.h>

namespace hoot
{

MemChangesetProvider::MemChangesetProvider(boost::shared_ptr<OGRSpatialReference> pProjection):
  _projection(pProjection)
{
}

MemChangesetProvider::~MemChangesetProvider()
{
}

void MemChangesetProvider::close()
{
  // nothing to do here
}

boost::shared_ptr<OGRSpatialReference> MemChangesetProvider::getProjection() const
{
  return _projection;
}

bool MemChangesetProvider::hasMoreChanges()
{
  return (_changes.size() > 0);
}

Change MemChangesetProvider::readNextChange()
{
  Change nextChange = _changes.front();
  _changes.pop_front();
  return nextChange;
}

void MemChangesetProvider::addChange(Change newChange)
{
  _changes.push_back(newChange);
}

size_t MemChangesetProvider::getNumChanges()
{
  return _changes.size();
}

// "premature optimization is the root of all evil" - Donald Knuth
// Possibly use a hash or something here, if this is taking too much time
bool MemChangesetProvider::containsChange(ElementId eID)
{
  for (std::list<Change>::iterator it = _changes.begin(); it != _changes.end(); ++it)
  {
    if (eID == it->getElement()->getElementId())
    {
      return true;
    }
  }

  // Return false if not found
  return false;
}

}
