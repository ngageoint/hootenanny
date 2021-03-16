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
#ifndef MEMCHANGESETPROVIDER_H
#define MEMCHANGESETPROVIDER_H

#include <hoot/core/algorithms/changeset/ChangesetProvider.h>

namespace hoot
{

/**
 * This is a simple memory-bound changeset provider. It's basically used to collect a set of
 * changes, then feed them to a writer later.
 */
class MemChangesetProvider : public ChangesetProvider
{

public:

  explicit MemChangesetProvider(const std::shared_ptr<OGRSpatialReference>& pProjection)
    : _projection(pProjection) { }

  virtual ~MemChangesetProvider() = default;

  /**
   * @see ChangeSetProvider
   */
  std::shared_ptr<OGRSpatialReference> getProjection() const override;

  /**
   * @see ChangeSetProvider
   */
  void close() override;

  /**
   * @see ChangeSetProvider
   */
  bool hasMoreChanges() override;

  /**
   * @see ChangeSetProvider
   */
  Change readNextChange() override;

  /**
   * @see ChangeSetProvider
   */
  int getNumChanges() const override;

  void addChange(const Change& newChange);

  bool containsChange(ElementId eID);

private:

  std::shared_ptr<OGRSpatialReference> _projection;
  std::list<Change> _changes;
};

using MemChangesetProviderPtr = std::shared_ptr<MemChangesetProvider>;

}

#endif // MEMCHANGESETPROVIDER_H
