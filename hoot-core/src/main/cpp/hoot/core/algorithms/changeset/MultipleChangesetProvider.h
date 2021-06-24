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
#ifndef MULTIPLECHANGESETPROVIDER_H
#define MULTIPLECHANGESETPROVIDER_H

#include <hoot/core/algorithms/changeset/ChangesetProvider.h>

namespace hoot
{

/**
 * @brief The MultipleChangesetProvider class is essentially a changeset provider container,
 * allowing multiple changeset providers to be chained together
 */
class MultipleChangesetProvider : public ChangesetProvider
{

public:

  explicit MultipleChangesetProvider(const std::shared_ptr<OGRSpatialReference>& pProjection)
    : _projection(pProjection) { }

  ~MultipleChangesetProvider() = default;

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

  void addChangesetProvider(ChangesetProviderPtr newChangeset);

  size_t getNumChangesets() const;

private:

  std::shared_ptr<OGRSpatialReference> _projection;
  std::list<ChangesetProviderPtr> _changesets;
};

using MultipleChangesetProviderPtr = std::shared_ptr<MultipleChangesetProvider>;

}

#endif // MULTIPLECHANGESETPROVIDER_H
