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
#ifndef CHANGESET_REPLACEMENT_CREATOR_2_H
#define CHANGESET_REPLACEMENT_CREATOR_2_H

// Hoot
#include <hoot/core/algorithms/changeset/ChangesetReplacementCreator1.h>

namespace hoot
{

/**
 * This removes relations, processes the map, then adds them back in just before changeset
 * derivation.
 */
class ChangesetReplacementCreator2 : public ChangesetReplacementCreator1
{

public:

  static std::string className() { return "hoot::ChangesetReplacementCreator2"; }

  ChangesetReplacementCreator2() = default;

protected:

  /*
   * Returns the default geometry filters (point, line, poly) to use when no other geometry filters
   * are specified
   */
  virtual QMap<GeometryTypeCriterion::GeometryType, ElementCriterionPtr>
    _getDefaultGeometryFilters() const override;

  /*
   * Populates a reference and a conflated map based on the geometry type being replaced. The maps
   * can then used to derive the replacement changeset.
   */
  virtual void _processMaps(
    OsmMapPtr& refMap, OsmMapPtr& conflatedMap, const ElementCriterionPtr& refFeatureFilter,
    const ElementCriterionPtr& secFeatureFilter,
    const GeometryTypeCriterion::GeometryType& geometryType,
    const QStringList& linearFilterClassNames = QStringList()) override;

  virtual void _cleanup(OsmMapPtr& map) override;

  /*
   * TODO
   */
  virtual void _restoreRelations(OsmMapPtr& map, OsmMapPtr& relationsMap);

  /*
   * TODO
   */
  QStringList _getRelationMemberElementIdsForConfig(
    const QList<OsmMapPtr>& relationMaps, const ElementType::Type& elementType) const;

  /*
   * TODO
   */
  void _conflate(OsmMapPtr& map, const QList<OsmMapPtr>& relationMaps);

  /*
   * TODO
   */
  void _clean(OsmMapPtr& map, const QList<OsmMapPtr>& relationMaps);

  /*
   * TODO
   */
  OsmMapPtr _removeRelations(OsmMapPtr& map);

  /*
   * TODO
   */
  void _syncJoinedMemberWays(
    const QList<OsmMapPtr>& relationMaps, const QHash<long, long>& joinedWayIdMappings);
};

}

#endif // CHANGESET_REPLACEMENT_CREATOR_2_H
