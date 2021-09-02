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
 * @copyright Copyright (C) 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef CHANGESET_REPLACEMENT_CREATOR_H
#define CHANGESET_REPLACEMENT_CREATOR_H

// Hoot
#include <hoot/core/algorithms/changeset/ChangesetReplacementCreatorAbstract.h>

namespace hoot
{

/**
 * @brief The ChangesetReplacementCreator class is a single geometry pass version of
 * ChangesetReplacement, which solves the bug in handling relations with children of mixed geometry
 * types. This drops support for overlapping only replacement and strict bounds handling, as they
 * are not useful for replacements within a task grid.
 */
class ChangesetReplacementCreator : public ChangesetReplacementCreatorAbstract
{

public:

  static QString className() { return "ChangesetReplacementCreator"; }

  ChangesetReplacementCreator();

  /**
   * @brief create creates a changeset that replaces features in the first input with features from
   * the second input.
   * @param input1 the target data file path for the changeset in which to replace features; must
   * support Boundable
   * @param input2 the source data file path for the changeset to get replacement features from;
   * must support Boundable
   * @param bounds the rectangular bounds over which features are to be replaced
   * @param output the changeset file output location
   */
  void create(const QString& input1, const QString& input2, const geos::geom::Envelope& bounds,
    const QString& output) override;
  /**
   * @brief create creates a changeset that replaces features in the first input with features from
   * the second input.
   * @param input1 the target data file path for the changeset in which to replace features; must
   * support Boundable
   * @param input2 the source data file path for the changeset to get replacement features from;
   * must support Boundable
   * @param bounds the bounds over which features are to be replaced
   * @param output the changeset file output location
   */
  void create(const QString& input1, const QString& input2,
    const std::shared_ptr<geos::geom::Polygon>& bounds, const QString& output) override;

  QString toString() const override { return className(); }

protected:

  ElementCriterionPtr _geometryTypeFilter;

  void _setGlobalOpts() override;

private:

  /*
   * For any element in the sec dataset with a lower version than the corresponding element in the
   * ref dataset, updates the version of the sec element.
   */
  void _syncInputVersions(const OsmMapPtr& refMap, const OsmMapPtr& secMap) const;

  OsmMapPtr _loadAndFilterRefMap(QMap<ElementId, long>& refIdToVersionMappings);
  OsmMapPtr _loadAndFilterSecMap();

  /*
   * Copies all ways that are tagged with MetadataTags::HootConnectedWayOutsideBounds() out of a map
   */
  OsmMapPtr _getImmediatelyConnectedOutOfBoundsWays(const ConstOsmMapPtr& map) const;

  /*
   * Snaps unnconnected ways with a map to each other
   *
   * @param map the map to snap ways within
   * @param snapWayStatuses the statuses the ways being snapped must have
   * @param snapToWayStatuses the statuses the ways being snapped to must have
   * @param typeCriterionClassName optional filter criteria that snapped/snapped to ways must have
   * @param markSnappedWays if true, snapped ways are marked with a custom metadata tag
   * @param debugFileName name prefix for any debug map files generated during snapping
   */
  void _snapUnconnectedWays(
    OsmMapPtr& map, const QStringList& snapWayStatuses, const QStringList& snapToWayStatuses,
    const QString& typeCriterionClassName, const bool markSnappedWays,
    const QString& debugFileName) const;

  /*
   * Removes all ways from the map with both MetadataTags::HootConnectedWayOutsideBounds() and
   * MetadataTags::HootSnapped() tags
   */
  void _removeUnsnappedImmediatelyConnectedOutOfBoundsWays(const OsmMapPtr& map) const;

  void _snapUnconnectedPreChangesetMapCropping(OsmMapPtr& combinedMap) const;
  void _snapUnconnectedPostChangesetMapCropping(
    const OsmMapPtr& refMap, OsmMapPtr& combinedMap,
    const OsmMapPtr& immediatelyConnectedOutOfBoundsWays) const;

  /*
   * Performs cropping to prepare a map for changeset derivation. This is potentially different
   * cropping than done during initial load and cookie cutting.
   */
  void _cropMapForChangesetDerivation(
    OsmMapPtr& map, const bool keepEntireFeaturesCrossingBounds,
    const bool keepOnlyFeaturesInsideBounds, const QString& debugFileName) const;

  void _generateChangeset(const OsmMapPtr& refMap, const OsmMapPtr& combinedMap);
};

}

#endif // CHANGESET_REPLACEMENT_CREATOR_H
