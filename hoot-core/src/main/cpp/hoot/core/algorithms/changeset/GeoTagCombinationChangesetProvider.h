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
#ifndef GEO_TAG_COMBINATION_CHANGESET_PROVIDER_H
#define GEO_TAG_COMBINATION_CHANGESET_PROVIDER_H

#include <hoot/core/algorithms/changeset/ChangesetProvider.h>
#include <hoot/core/algorithms/changeset/MemChangesetProvider.h>

#include <array>

namespace hoot
{

/**
 * @brief The GeoTagCombinationChangesetProvider class is essentially a changeset provider container,
 * combining a geometry changeset provider with a tag changeset provider
 */
class GeoTagCombinationChangesetProvider : public ChangesetProvider
{

public:

  explicit GeoTagCombinationChangesetProvider(const std::shared_ptr<OGRSpatialReference>& pProjection)
    : _projection(pProjection) { }
  ~GeoTagCombinationChangesetProvider() override = default;

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

  void setGeoChangesetProvider(const ChangesetProviderPtr& geoChangeset)
  { _geoChangeset = geoChangeset; }
  void setTagChangesetProvider(const MemChangesetProviderPtr& tagChangeset)
  { _tagChangeset = tagChangeset; }

private:

  std::shared_ptr<OGRSpatialReference> _projection;
  ChangesetProviderPtr _geoChangeset;
  MemChangesetProviderPtr _tagChangeset;
};

using GeoTagCombinationChangesetProviderPtr = std::shared_ptr<GeoTagCombinationChangesetProvider>;

}

#endif // GEO_TAG_COMBINATION_CHANGESET_PROVIDER_H
