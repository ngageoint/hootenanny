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
 * TODO
 */
class ChangesetCleaner : public ChangesetProvider
{

public:

  /**
   * TODO
   *
   * @param changesetProviders
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
  virtual void close() override;

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

  // TODO
  QList<ChangesetProviderPtr> _changesetProviders;
  // TODO
  std::vector<Change>::const_iterator _changeItr;
  // TODO
  QMap<ElementId, Change> _changesById;
  // TODO
  std::vector<Change> _changes;
  // TODO
  int _numDeleteChangesRemoved;

  void _clean();
};

}

#endif // CHANGESET_CLEANER_H
