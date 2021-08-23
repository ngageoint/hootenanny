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
 * @copyright Copyright (C) 2020, 2021 Maxar (http://www.maxar.com/)
 */
#include "ChangesetCleaner.h"

// Hoot


namespace hoot
{

ChangesetCleaner::ChangesetCleaner(const QList<ChangesetProviderPtr>& changesetProviders) :
_changesetProviders(changesetProviders),
_numDeleteChangesRemoved(0)
{
  if (_changesetProviders.isEmpty())
  {
    throw IllegalArgumentException("No changeset providers were sent to ChangesetCleaner.");
  }
  _clean();
}

ChangesetCleaner::~ChangesetCleaner()
{
  close();
}

void ChangesetCleaner::_clean()
{
  LOG_DEBUG("Deriving and cleaning " << _changesetProviders.size() << " changeset(s)...");

  for (QList<ChangesetProviderPtr>::const_iterator itr =
         _changesetProviders.begin(); itr != _changesetProviders.end(); ++itr)
  {
    ChangesetProviderPtr changesetProvider = *itr;
    while (changesetProvider->hasMoreChanges())
    {
      const Change change = changesetProvider->readNextChange();
      if (change.getElement())
      {
        LOG_VART(change);
        const ElementId id = change.getElement()->getElementId();
        if (_changesById.contains(id))
        {
          const Change existingChange = _changesById[id];
          LOG_VART(existingChange);
          // We already have a modify or create change for this element, so ignore the delete
          // change.
          if (existingChange.getType() != Change::Delete && change.getType() == Change::Delete)
          {
            _numDeleteChangesRemoved++;
            continue;
          }
          else
          {
            // If the existing change is a delete, then we'll overwrite it with a create or
            // modify.
            _changesById[id] = change;
            _changes.push_back(change);
          }
        }
        else
        {
          _changesById[id] = change;
          _changes.push_back(change);
        }
      }
    }
  }
  LOG_VARD(_changesById.size());
  LOG_VARD(_changes.size());
  _changeItr = _changes.begin();
}

void ChangesetCleaner::close()
{
  _changesById.clear();
  _changes.clear();
}

std::shared_ptr<OGRSpatialReference> ChangesetCleaner::getProjection() const
{
  return _changesetProviders.front()->getProjection();
}

bool ChangesetCleaner::hasMoreChanges()
{
  return _changeItr != _changes.end();
}

Change ChangesetCleaner::readNextChange()
{
  Change change = *_changeItr;;
  ++_changeItr;;
  return change;
}

int ChangesetCleaner::getNumFromElementsParsed() const
{
  int total = 0;
  for (QList<ChangesetProviderPtr>::const_iterator itr =
       _changesetProviders.begin(); itr != _changesetProviders.end(); ++itr)
  {
    total += (*itr)->getNumFromElementsParsed();
  }
  return total;
}

int ChangesetCleaner::getNumToElementsParsed() const
{
  int total = 0;
  for (QList<ChangesetProviderPtr>::const_iterator itr =
       _changesetProviders.begin(); itr != _changesetProviders.end(); ++itr)
  {
    total += (*itr)->getNumToElementsParsed();
  }
  return total;
}

int ChangesetCleaner::getNumCreateChanges() const
{
  int total = 0;
  for (QList<ChangesetProviderPtr>::const_iterator itr =
       _changesetProviders.begin(); itr != _changesetProviders.end(); ++itr)
  {
    total += (*itr)->getNumCreateChanges();
  }
  return total;
}

int ChangesetCleaner::getNumModifyChanges() const
{
  int total = 0;
  for (QList<ChangesetProviderPtr>::const_iterator itr =
       _changesetProviders.begin(); itr != _changesetProviders.end(); ++itr)
  {
    total += (*itr)->getNumModifyChanges();
  }
  return total;
}

int ChangesetCleaner::getNumDeleteChanges() const
{
  int total = 0;
  for (QList<ChangesetProviderPtr>::const_iterator itr =
       _changesetProviders.begin(); itr != _changesetProviders.end(); ++itr)
  {
    total += (*itr)->getNumDeleteChanges();
  }
  total -= _numDeleteChangesRemoved;
  return total;
}

int ChangesetCleaner::getNumChanges() const
{
  int total = 0;
  for (QList<ChangesetProviderPtr>::const_iterator itr =
       _changesetProviders.begin(); itr != _changesetProviders.end(); ++itr)
  {
    total += (*itr)->getNumChanges();
  }
  return total;
}

}
