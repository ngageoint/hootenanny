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
 * High level class for prepping data for replacement changeset generation (changesets which
 * completely replace features inside of a specified bounds) and then calls on appropriate
 * changeset file writers.
 *
 * This class uses a customized workflow that depends upon the feature type the changeset is being
 * generated for and how strict the AOI is to be interpreted. ChangesetCreator is used for the
 * actual changeset generation and file output. This class handles the cookie cutting, conflation,
 * and a host of other things that need to happen before the changeset generation.
 */
class ChangesetReplacementCreator
{

public:

  ChangesetReplacementCreator(const bool printStats = false, const QString osmApiDbUrl = "");

  /**
   * Creates a changeset that completely replaces features in the first input with features from
   * the second input.
   *
   * @param input1 the changeset target in which to replace features; must support Boundable
   * @param input2 the changeset source to get replacement features from; must support Boundable
   * @param bounds the bounds over which features are to be replaced
   * @param featureTypeFilterClassName the type of feature to replace; must be a class name
   * inheriting from ConflatableElementCriterion
   * @param lenientBounds determines how strict the handling of the bounds is during replacement
   * @param output the changeset file output location
   * @todo support empty feature filters and filter with multiple types - #3360
   */
  void create(
    const QString& input1, const QString& input2, const geos::geom::Envelope& bounds,
    const QString& featureTypeFilterClassName, const bool lenientBounds, const QString& output);

private:

  // Determines whether features crossing the bounds should be kept when loading reference data.
  bool _loadRefKeepEntireCrossingBounds;
  // Determines whether only features completely inside the bounds should be kept when loading
  // reference data.
  bool _loadRefKeepOnlyInsideBounds;
  // Determines whether ways immediately connected to other ways being kept but completely outside
  // of the bounds should also be kept
  bool _loadRefKeepImmediateConnectedWaysOutsideBounds;
  // Determines whether features crossing the bounds should be kept when loading secondary data.
  bool _loadSecKeepEntireCrossingBounds;
  // Determines whether only features completely inside the bounds should be kept when loading
  // secondary data.
  bool _loadSecKeepOnlyInsideBounds;

  // Determines whether features crossing the bounds should be kept when cookie cutting reference
  // data.
  bool _cookieCutKeepEntireCrossingBounds;
  // Determines whether only features completely inside the bounds should be kept when cookie
  // cutting reference data.
  bool _cookieCutKeepOnlyInsideBounds;

  // Determines whether reference features crossing the bounds should be kept when deriving a
  // changeset.
  bool _changesetRefKeepEntireCrossingBounds;
  // Determines whether secondary features crossing the bounds should be kept when deriving a
  // changeset.
  bool _changesetSecKeepEntireCrossingBounds;
  // Determines whether only reference features completely inside the bounds should be kept when
  // deriving a changeset.
  bool _changesetRefKeepOnlyInsideBounds;
  // Determines whether only secondary features completely inside the bounds should be kept when
  // deriving a changeset.
  bool _changesetSecKeepOnlyInsideBounds;
  // Determines whether deleting reference features existing either partially of completely outside
  // of the bounds is allowed during changeset generation
  bool _changesetAllowDeletingRefOutsideBounds;
  // the strictness of the bounds calculation used in conjunction with
  // _changesetAllowDeletingRefOutsideBounds
  bool _inBoundsStrict;

  // handles changeset generation and output
  std::shared_ptr<ChangesetCreator> _changesetCreator;

  // determine geometry type based on hoot criterion class; may need something better for these

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
   * Returns the number of features with a changeset version less than one
   */
  int _versionLessThanOneCount(const OsmMapPtr& map) const;

  /*
   * Keeps track of the changeset versions for features
   */
  QMap<ElementId, long> _getIdToVersionMappings(const OsmMapPtr& map) const;

  /*
   * Adds tags to a feature that will prevent ChangesetDeriver from ever creating a delete
   * statement for it
   */
  void _addChangesetDeleteExclusionTags(OsmMapPtr& map);

  OsmMapPtr _getCookieCutMap(OsmMapPtr doughMap, OsmMapPtr cutterMap);

  /*
   * Copies all ways that are tagged with MetadataTags::HootConnectedWayOutsideBounds() out of a map
   */
  OsmMapPtr _getImmediatelyConnectedOutOfBoundsWays(const ConstOsmMapPtr& map) const;

  /*
   * Excludes all features within the specified bounds from deletion during changeset derivation
   */
  void _excludeFeaturesFromChangesetDeletion(OsmMapPtr& map, const QString& boundsStr);

  /*
   * Combines two maps into one; throwOutDupes ignores any elements in the second map with the ID
   * as an element in the first map
   */
  void _combineMaps(OsmMapPtr& map1, OsmMapPtr& map2, const bool throwOutDupes,
                    const QString& debugFileName);
  /*
   * Removes all ways from the map with both MetadataTags::HootConnectedWayOutsideBounds() and
   * MetadataTags::HootSnapped()=snapped_way tags
   */
  void _removeUnsnappedImmediatelyConnectedOutOfBoundsWays(OsmMapPtr& map);

  void _conflate(OsmMapPtr& map, const bool lenientBounds);

  void _snapUnconnectedWays(OsmMapPtr& map, const QString& snapWayStatus,
                            const QString& snapToWayStatus,
                            const QString& featureTypeFilterClassName, const bool markSnappedWays,
                            const QString& debugFileName);
  /*
   * Performs cropping to prepare a map for changeset derivation. This is potentially different
   * cropping than done during initial load and cookie cutting.
   */
  void _cropMapForChangesetDerivation(
    OsmMapPtr& map, const geos::geom::Envelope& bounds, const bool keepEntireFeaturesCrossingBounds,
    const bool keepOnlyFeaturesInsideBounds, const bool isLinearMap, const QString& debugFileName);
};

}

#endif // CHANGESET_REPLACEMENT_CREATOR_H
