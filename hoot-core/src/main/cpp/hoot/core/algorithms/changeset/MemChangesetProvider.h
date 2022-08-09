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

// hoot
#include <hoot/core/algorithms/changeset/ChangesetProvider.h>

//  std
#include <map>

namespace hoot
{

/**
 * @brief The MemChangesetProvider class is a simple memory-bound changeset provider.
 *
 * It's basically used to collect a set of changes, then feed them to a writer later.
 */
class MemChangesetProvider : public ChangesetProvider
{

public:

  explicit MemChangesetProvider(const std::shared_ptr<OGRSpatialReference>& pProjection)
    : _projection(pProjection) { }
  ~MemChangesetProvider() = default;

  /**
   * @see ChangesetProvider
   */
  std::shared_ptr<OGRSpatialReference> getProjection() const override;

  /**
   * @see ChangesetProvider
   */
  void close() override;

  /**
   * @see ChangesetProvider
   */
  bool hasMoreChanges() override;

  /**
   * @see ChangesetProvider
   */
  Change readNextChange() override;

  /**
   * @see ChangesetProvider
   */
  int getNumChanges() const override;

  /**
   * @brief addChange Add a new change to the in-memory provider
   * @param newChange New change information
   */
  void addChange(const Change& newChange);

  /**
   * @brief removeChange Remove an old change from the in-memory provider
   * @param change Change information
   */
  void removeChange(const Change& change);

  /**
   * @brief containsChange Search the set of changes for the element ID in question
   * @param eid Element ID to search for
   * @return True if eid was found in the provider
   */
  bool containsChange(ElementId eid);

  /**
   * @brief getChange Get change information for the element ID in question
   * @param eid Element ID to get info for
   * @return Change information for the element ID
   */
  const Change& getChange(ElementId eid);

private:

  std::shared_ptr<OGRSpatialReference> _projection;
  std::map<ElementId, Change> _changes;
};

using MemChangesetProviderPtr = std::shared_ptr<MemChangesetProvider>;

}

#endif // MEMCHANGESETPROVIDER_H
