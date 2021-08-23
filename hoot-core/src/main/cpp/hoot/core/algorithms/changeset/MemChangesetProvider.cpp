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
#include "MemChangesetProvider.h"



namespace hoot
{

void MemChangesetProvider::close()
{
  // nothing to do here
}

std::shared_ptr<OGRSpatialReference> MemChangesetProvider::getProjection() const
{
  return _projection;
}

bool MemChangesetProvider::hasMoreChanges()
{
  return !_changes.empty();
}

Change MemChangesetProvider::readNextChange()
{
  Change nextChange = _changes.front();
  _changes.pop_front();
  return nextChange;
}

void MemChangesetProvider::addChange(const Change& newChange)
{
  _changes.push_back(newChange);
}

int MemChangesetProvider::getNumChanges() const
{
  return static_cast<int>(_changes.size());
}

bool MemChangesetProvider::containsChange(ElementId eID)
{
  // "premature optimization is the root of all evil" - Donald Knuth
  // Possibly use a hash or something here, if this is taking too much time

  for (std::list<Change>::iterator it = _changes.begin(); it != _changes.end(); ++it)
  {
    if (eID == it->getElement()->getElementId())
    {
      return true;
    }
  }
  return false;
}

}
