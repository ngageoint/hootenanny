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
 * @copyright Copyright (C) 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef CHANGESET_REPLACEMENT_CREATOR_H
#define CHANGESET_REPLACEMENT_CREATOR_H

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/algorithms/changeset/ChangesetCreator.h>
#include <hoot/core/criterion/ConflatableElementCriterion.h>

//GEOS
#include <geos/geom/Envelope.h>

namespace hoot
{

/**
 * TODO
 */
class ChangesetReplacementCreator
{

public:

  ChangesetReplacementCreator(const bool printStats = false, const QString osmApiDbUrl = "");

  /**
   * TODO
   *
   * @param input1
   * @param input2
   * @param bounds
   * @param featureTypeFilterClassName
   * @param lenientBounds
   * @param output
   * @todo support empty feature filters and filter with multiple types
   */
  void create(
    const QString& input1, const QString& input2, const geos::geom::Envelope& bounds,
    const QString& featureTypeFilterClassName, const bool lenientBounds, const QString& output);

private:

  // TODO
  bool _convertRefKeepEntireCrossingBounds;
  // TODO
  bool _convertRefKeepOnlyInsideBounds;
  // TODO
  bool _convertRefKeepImmediateConnectedWaysOutsideBounds;
  // TODO
  bool _convertSecKeepEntireCrossingBounds;
  // TODO
  bool _convertSecKeepOnlyInsideBounds;
  // TODO
  bool _cropKeepEntireCrossingBounds;
  // TODO
  bool _cropKeepOnlyInsideBounds;
  // TODO
  bool _changesetRefKeepEntireCrossingBounds;
  // TODO
  bool _changesetSecKeepEntireCrossingBounds;
  // TODO
  bool _changesetRefKeepOnlyInsideBounds;
  // TODO
  bool _changesetSecKeepOnlyInsideBounds;
  // TODO
  bool _changesetAllowDeletingRefOutsideBounds;
  // TODO
  bool _inBoundsStrict;

  std::shared_ptr<ChangesetCreator> _changesetCreator;

  bool _isPointCrit(const QString& critClassName) const;
  bool _isLinearCrit(const QString& critClassName) const;
  bool _isPolyCrit(const QString& critClassName) const;

  bool _isNetworkConflate() const;

  void _validateInputs(const QString& input1, const QString& input2);
  std::shared_ptr<ConflatableElementCriterion> _validateFilter(
    const QString& featureTypeFilterClassName);
  void _filterFeatures(
    OsmMapPtr& map, const std::shared_ptr<ConflatableElementCriterion>& featureCrit,
    const QString& debugFileName);
  void _parseConfigOpts(const bool lenientBounds, const QString& critClassName,
                        const QString& boundsStr);

  OsmMapPtr _loadRefMap(const QString& input);
  OsmMapPtr _loadSecMap(const QString& input);

  /*
   * TODO
   */
  int _versionLessThanOneCount(const OsmMapPtr& map) const;
  /*
   * TODO
   */
  QMap<ElementId, long> _getIdToVersionMappings(const OsmMapPtr& map);
  void _addDeleteExclusionTags(OsmMapPtr& map);
  OsmMapPtr _getCookieCutMap(OsmMapPtr doughMap, OsmMapPtr cutterMap);
  /*
   * TODO
   */
  OsmMapPtr _getImmediatelyConnectedOutOfBoundsWays(const ConstOsmMapPtr& map);
  /*
   * TODO
   */
  void _excludeFeaturesFromDeletion(OsmMapPtr& map, const QString& boundsStr);
  /*
   * TODO
   */
  void _addNodeHashes(OsmMapPtr& map);
  void _combineMaps(OsmMapPtr& map1, OsmMapPtr& map2, const bool throwOutDupes,
                    const QString& debugFileName);
  /*
   * TODO
   */
  void _removeUnsnappedImmediatelyConnectedOutOfBoundsWays(OsmMapPtr& map);

  void _conflate(OsmMapPtr& map, const bool lenientBounds);
  void _snapUnconnectedWays(OsmMapPtr& map, const QString& snapWayStatus,
                            const QString& snapToWayStatus,
                            const QString& featureTypeFilterClassName, const bool markSnappedWays,
                            const QString& debugFileName);
  /*
   * TODO
   */
  void _cropMapForChangesetDerivation(
    OsmMapPtr& map, const geos::geom::Envelope& bounds, const bool keepEntireFeaturesCrossingBounds,
    const bool keepOnlyFeaturesInsideBounds, const bool isLinearMap, const QString& debugFileName);
};

}

#endif // CHANGESET_REPLACEMENT_CREATOR_H
