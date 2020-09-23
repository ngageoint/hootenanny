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
 * @copyright Copyright (C) 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef CHANGESET_CLEANER_H
#define CHANGESET_CLEANER_H

// Hoot
#include <hoot/core/algorithms/changeset/ChangesetProvider.h>

namespace hoot
{

/**
 * Removes mistakes from changesets. This is meant to be used when algorithms create erroneous
 * changesets and the cause isn't immediately discernible. Whenever possible, the changeset
 * generation itself should be fixed. The cleaning operation is memory bound.
 *
 * Current cleaning ops:
 *
 * 1) If there is a delete change and either a create or modify change assigned to the same element,
 * the delete change is discarded.
 *
 * The cleaning may be able to be extended to not allow duplicate changes for any element, but want
 * to wait to do that until the problem is actually seen in the wild.
 */
class ChangesetCleaner : public ChangesetProvider
{

public:

  /**
   * Constructor - cleans the changeset summed together from the input providers
   *
   * @param changesetProviders a collection of changeset providers
   */
  ChangesetCleaner(const QList<ChangesetProviderPtr>& changesetProviders);

  virtual ~ChangesetCleaner();

  /**
   * @see ChangeSetProvider
   */
  virtual std::shared_ptr<OGRSpatialReference> getProjection() const override;

  /**
   * @see ChangeSetProvider
   */
  virtual void close() override {}

  /**
   * @see ChangeSetProvider
   */
  virtual bool hasMoreChanges() override;

  /**
   * @see ChangeSetProvider
   */
  virtual Change readNextChange() override;

  /**
   * @see ChangeSetProvider
   */
  virtual int getNumFromElementsParsed() const override;

  /**
   * @see ChangeSetProvider
   */
  virtual int getNumToElementsParsed() const override;

  /**
   * @see ChangeSetProvider
   */
  virtual int getNumCreateChanges() const override;

  /**
   * @see ChangeSetProvider
   */
  virtual int getNumModifyChanges() const override;

  /**
   * @see ChangeSetProvider
   */
  virtual int getNumDeleteChanges() const override;

  /**
   * @see ChangeSetProvider
   */
  virtual int getNumChanges() const override;

private:

  // providers of the changes being cleaned
  QList<ChangesetProviderPtr> _changesetProviders;
  // iterator for the cleaned changes
  std::vector<Change>::const_iterator _changeItr;
  // maps changes by element ID of the element involved in the change
  QMap<ElementId, Change> _changesById;
  // cleaned changes
  std::vector<Change> _changes;
  // number of delete changes removed from the changes coming from the providers
  int _numDeleteChangesRemoved;

  void _clean();
};

}

#endif // CHANGESET_CLEANER_H
