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
#include <hoot/core/criterion/GeometryTypeCriterion.h>
#include <hoot/core/criterion/ChainCriterion.h>

//GEOS
#include <geos/geom/Envelope.h>

namespace hoot
{

/**
 * Options used to control cropping at various stages of the replacement changeset workflow
 */
struct BoundsOptions
{
  // Determines whether features crossing the bounds should be kept when loading reference data.
  bool loadRefKeepEntireCrossingBounds;
  // Determines whether only features completely inside the bounds should be kept when loading
  // reference data.
  bool loadRefKeepOnlyInsideBounds;
  // Determines whether ways immediately connected to other ways being kept but completely outside
  // of the bounds should also be kept
  bool loadRefKeepImmediateConnectedWaysOutsideBounds;
  // Determines whether features crossing the bounds should be kept when loading secondary data.
  bool loadSecKeepEntireCrossingBounds;
  // Determines whether only features completely inside the bounds should be kept when loading
  // secondary data.
  bool loadSecKeepOnlyInsideBounds;

  // Determines whether features crossing the bounds should be kept when cookie cutting reference
  // data.
  bool cookieCutKeepEntireCrossingBounds;
  // Determines whether only features completely inside the bounds should be kept when cookie
  // cutting reference data.
  bool cookieCutKeepOnlyInsideBounds;

  // Determines whether reference features crossing the bounds should be kept when deriving a
  // changeset.
  bool changesetRefKeepEntireCrossingBounds;
  // Determines whether secondary features crossing the bounds should be kept when deriving a
  // changeset.
  bool changesetSecKeepEntireCrossingBounds;
  // Determines whether only reference features completely inside the bounds should be kept when
  // deriving a changeset.
  bool changesetRefKeepOnlyInsideBounds;
  // Determines whether only secondary features completely inside the bounds should be kept when
  // deriving a changeset.
  bool changesetSecKeepOnlyInsideBounds;
  // Determines whether deleting reference features existing either partially of completely outside
  // of the bounds is allowed during changeset generation
  bool changesetAllowDeletingRefOutsideBounds;
  // the strictness of the bounds calculation used in conjunction with
  // _changesetAllowDeletingRefOutsideBounds
  bool inBoundsStrict;
};

/**
 * High level class for prepping data for replacement changeset generation (changesets which
 * completely replace features inside of a specified bounds) and then calls on appropriate
 * changeset file writers.
 *
 * This class uses a customized workflow that depends upon the feature type the changeset is being
 * generated for and how strict the AOI is to be interpreted. ChangesetCreator is used for the
 * actual changeset generation and file output. This class handles the cookie cutting, conflation,
 * and a host of other things that need to happen before the changeset generation.
 *
 * TODO: implement progress
 */
class ChangesetReplacementCreator
{

public:

  ChangesetReplacementCreator(const bool printStats = false, const QString osmApiDbUrl = "");

  /**
   * Creates a changeset that completely replaces features in the first input with features from
   * the second input.
   *
   * @param input1 the target data for the changeset in which to replace features; must support
   * Boundable
   * @param input2 the source data for the changeset to get replacement features from; must support
   * Boundable
   * @param bounds the bounds over which features are to be replaced
   * @param output the changeset file output locationn
   */
  void create(
    const QString& input1, const QString& input2, const geos::geom::Envelope& bounds,
    const QString& output);

  void setLenientBounds(const bool lenient) { _lenientBounds = lenient; }
  void setGeometryFilters(const QStringList& filterClassNames);
  void setAdditionalFilters(const QStringList& filterClassNames);
  void setChainAdditionalFilters(const bool chain) { _chainAdditionalFilters = chain; }

private:

  friend class ChangesetReplacementCreatorTest;

  // determines how strict the handling of the bounds is during replacement
  bool _lenientBounds;
  // TODO
  QMap<GeometryTypeCriterion::GeometryType, ElementCriterionPtr> _geometryTypeFilters;
  // TODO
  QStringList _linearFilterClassNames;
  // TODO
  std::shared_ptr<ChainCriterion> _additionalFilter;
  // TODO
  bool _chainAdditionalFilters;

  BoundsOptions _boundsOpts;

  // handles changeset generation and output
  std::shared_ptr<ChangesetCreator> _changesetCreator;

  bool _isNetworkConflate() const;

  void _validateInputs(const QString& input1, const QString& input2);

  QMap<GeometryTypeCriterion::GeometryType, ElementCriterionPtr>
    _getDefaultGeometryFilters() const;
  QMap<GeometryTypeCriterion::GeometryType, ElementCriterionPtr> _getCombinedFilters();
  void _filterFeatures(
    OsmMapPtr& map, const ElementCriterionPtr& featureFilter, const QString& debugFileName);

  void _setGlobalOpts(const QString& boundsStr);
  void _parseConfigOpts(
    const bool lenientBounds, const GeometryTypeCriterion::GeometryType& geometryType);

  OsmMapPtr _loadRefMap(const QString& input);
  OsmMapPtr _loadSecMap(const QString& input);

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

  void _snapUnconnectedWays(
    OsmMapPtr& map, const QStringList& snapWayStatuses, const QStringList& snapToWayStatuses,
    const QStringList& geometryTypeFilterClassNames, const bool markSnappedWays,
    const QString& debugFileName);

  /*
   * Performs cropping to prepare a map for changeset derivation. This is potentially different
   * cropping than done during initial load and cookie cutting.
   */
  void _cropMapForChangesetDerivation(
    OsmMapPtr& map, const geos::geom::Envelope& bounds, const bool keepEntireFeaturesCrossingBounds,
    const bool keepOnlyFeaturesInsideBounds, const bool isLinearMap, const QString& debugFileName);

  /*
   * Populates a reference and a conflated map based on the geometry type being replaced. The maps
   * can then used to derive the replacement changeset.
   */
  void _getMapsForGeometryType(
    OsmMapPtr& refMap, OsmMapPtr& conflatedMap, const QString& input1, const QString& input2,
    const QString& boundsStr, const ElementCriterionPtr& featureFilter,
    const GeometryTypeCriterion::GeometryType& geometryType,
    const QStringList& linearFilterClassNames = QStringList());
};

}

#endif // CHANGESET_REPLACEMENT_CREATOR_H
