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
 * @copyright Copyright (C) 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef CHANGESET_CUT_ONLY_CREATOR_H
#define CHANGESET_CUT_ONLY_CREATOR_H

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/criterion/GeometryTypeCriterion.h>
#include <hoot/core/ops/ElementIdRemapper.h>
#include <hoot/core/algorithms/changeset/ChangesetReplacement.h>

namespace hoot
{

class ChangesetCreator;
class ChainCriterion;
class Settings;
class Change;
class ConstOsmMapConsumer;

/**
 * High level class for prepping data for replacement changeset generation (changesets which
 * replace features inside of a specified bounds) and then calls on the appropriate changeset file
 * writer to output a changeset file.
 *
 * This class uses a customized workflow that depends upon the feature type the changeset is being
 * generated for, whether all the reference features or just those that overlap secondary features
 * are to be replaced, and how strict the AOI is to be interpreted. ChangesetCreator is used for the
 * actual changeset generation and file output. This class handles the cookie cutting, conflation,
 * and a host of other things that need to happen before the replacement changeset is generated.
 * The secondary data added to the output changeset, as well as the reference data removed from the
 * changeset can be further restricted with a non-geometry type filter.
 *
 * Occasionally, relations will be passed in with some members missing. This class will optionally
 * tag those with a custom metadata tag to be passed to the changeset output. This allows for
 * potential manual repairing of those relations after the changeset is written, and after that the
 * tag can then be removed. This is also a configurable feature, which can be turned off.
 *
 * UPDATE 10/22/20: This has been refactored to be the cut only version of C&R. The replacement
 * version inherits from this. @todo This needs to be refactored into an abstract base class which
 * the cut only and replacement versions inherit from.
 *
 * @todo implement progress
 * @todo break this up into separate classes by function:
 *  - input prep
 *  - data replacement
 *  - conflation
 *  - snapping?
 *  - changeset derivation
 *  - cleanup
 * @todo rename all references to conflated data since maps aren't conflated anymore; also rename
 * all datasets in terms of "toReplace" and "replacement"
 */
class ChangesetCutOnlyCreator : public ChangesetReplacement
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
    // Determines whether deleting reference features existing either partially of completely
    // outside of the bounds is allowed during changeset generation
    bool changesetAllowDeletingRefOutsideBounds;
    // the strictness of the bounds calculation used in conjunction with
    // _changesetAllowDeletingRefOutsideBounds
    bool inBoundsStrict;
  };

public:

  static std::string className() { return "hoot::ChangesetCutOnlyCreator"; }

  ChangesetCutOnlyCreator();

  /**
   * @see ChangesetReplacement
   */
  virtual void create(
    const QString& input1, const QString& input2, const geos::geom::Envelope& bounds,
    const QString& output);

  /**
   * @see ChangesetReplacement
   */
  virtual void create(
    const QString& input1, const QString& input2,
    const std::shared_ptr<geos::geom::Polygon>& bounds, const QString& output);

  virtual int getNumChanges() const { return _numChanges; }

  virtual void setFullReplacement(const bool full) { _fullReplacement = full; }
  virtual void setBoundsInterpretation(const BoundsInterpretation& interpretation)
  { _boundsInterpretation = interpretation; }
  virtual void setGeometryFilters(const QStringList& filterClassNames);
  virtual void setReplacementFilters(const QStringList& filterClassNames);
  virtual void setChainReplacementFilters(const bool chain) { _chainReplacementFilters = chain; }
  virtual void setReplacementFilterOptions(const QStringList& optionKvps);
  virtual void setRetainmentFilters(const QStringList& filterClassNames);
  virtual void setChainRetainmentFilters(const bool chain) { _chainRetainmentFilters = chain; }
  virtual void setRetainmentFilterOptions(const QStringList& optionKvps);
  virtual void setConflationEnabled(const bool enabled) { _conflationEnabled = enabled; }
  virtual void setChangesetId(const QString& id) { _changesetId = id; }
  /**
   * Sets changeset options
   *
   * @param printStats prints statistics for the output changeset
   * @param outputStatsFile optional file to output the changeset statistics to
   * @param osmApiDbUrl URL to an OSM API database used to calculate element IDs; required only if
   * the output changeset is of type .osc.sql.
   */
  virtual void setChangesetOptions(
    const bool printStats, const QString& statsOutputFile, const QString osmApiDbUrl);

  virtual QString toString() const override
    { return QString::fromStdString(className()).remove("hoot::"); }

protected:

  // TODO: rename these input vars for clarity

  // path to the input with data being replaced; overrides use of _input1Map
  QString _input1;
  // cached data being replaced
  OsmMapPtr _input1Map;

  // path to the input with data used for replacement; overrides use of _input2Map
  QString _input2;
  // cached replacement data
  OsmMapPtr _input2Map;

  // path to the changeset output file
  QString _output;

  // the AOI over which the replacement is being performed
  std::shared_ptr<geos::geom::Polygon> _replacementBounds;

  // determines how strict the handling of the bounds is during replacement
  BoundsInterpretation _boundsInterpretation;

  // controls cropping
  BoundsOptions _boundsOpts;

  // Configuration options to pass to the filters in _replacementFilter.
  Settings _replacementFilterOptions;

  // turn on/off conflation of cookie cut data being replaced with replacement data
  bool _conflationEnabled;

  // helpful to name the debug map files when doing successive replacements
  QString _changesetId;

  // determines if the current changeset map generation pass contains only linear features
  bool _currentChangeDerivationPassIsLinear;

  friend class ChangesetReplacementCreatorTest;

  // used to keep log messages with urls in them shorter
  int _maxFilePrintLength;

  // If true, all the ref data gets replaced. If false, only the ref data that intersects with the
  // alpha shape of the sec data gets replaced.
  bool _fullReplacement;

  // A set of geometry type filters, organized by core geometry type (point, line, poly) to
  // separately filter the input datasets on.
  QMap<GeometryTypeCriterion::GeometryType, ElementCriterionPtr> _geometryTypeFilters;
  bool _geometryFiltersSpecified;

  // A list of linear geometry criterion classes to apply way snapping to.
  QStringList _linearFilterClassNames;

  // One or more non-geometry criteria to be combined with the geometry type filters for the
  // secondary input. Allows for further restriction of the secondary data that makes it to output.
  std::shared_ptr<ChainCriterion> _replacementFilter;

  // If true the filters specified in _replacementFilter are AND'd together. Otherwise, they're OR'd
  // together.
  bool _chainReplacementFilters;

  // One or more non-geometry criteria to be combined with the geometry type filters for the
  // reference input. Allows for further restriction of the ref data that gets replaced.
  std::shared_ptr<ChainCriterion> _retainmentFilter;

  // If true the filters specified in _retainmentFilter are AND'd together. Otherwise, they're OR'd
  // together.
  bool _chainRetainmentFilters;

  // Configuration options to pass to the filters in _retainmentFilter.
  Settings _retainmentFilterOptions;

  // handles changeset generation and output
  std::shared_ptr<ChangesetCreator> _changesetCreator;
  int _numChanges;

  QString _boundsInterpretationToString(const BoundsInterpretation& boundsInterpretation) const;

  bool _roadFilterExists() const;

  void _setInputFilter(
    std::shared_ptr<ChainCriterion>& inputFilter, const QStringList& filterClassNames,
    const bool chainFilters);

  void _setInputFilterOptions(Settings& opts, const QStringList& optionKvps);

  /*
   * Conflates data within the input map
   */
  void _conflate(OsmMapPtr& map);

  /*
   * Replaces the IDs of elements in the replacment maps that are identical with those in the maps
   * being replaced with the IDs from the maps being replaced.
   */
  void _synchronizeIds(
    const QList<OsmMapPtr>& mapsBeingReplaced, const QList<OsmMapPtr>& replacementMaps);
  void _synchronizeIds(OsmMapPtr mapBeingReplaced, OsmMapPtr replacementMap);

  OsmMapPtr _getMapByGeometryType(const QList<OsmMapPtr>& maps, const QString& geometryTypeStr);

  void _intraDedupeMap(OsmMapPtr& map);

  virtual void _setGlobalOpts();

  void _parseConfigOpts(const GeometryTypeCriterion::GeometryType& geometryType);

  void _validateInputs();

  void _printJobDescription() const;

  /*
   * Returns the default geometry filters (point, line, poly) to use when no other geometry filters
   * are specified
   */
  virtual QMap<GeometryTypeCriterion::GeometryType, ElementCriterionPtr>
    _getDefaultGeometryFilters() const;

  /*
   * Combines filters in _geometryTypeFilters with _replacementFilter.
   */
  QMap<GeometryTypeCriterion::GeometryType, ElementCriterionPtr> _getCombinedFilters(
    std::shared_ptr<ChainCriterion> nonGeometryFilter);

  OsmMapPtr _loadRefMap(const GeometryTypeCriterion::GeometryType& geometryType);
  OsmMapPtr _loadSecMap(const GeometryTypeCriterion::GeometryType& geometryType);
  OsmMapPtr _loadInputMap(
    const QString& mapName, const QString& inputUrl, const bool useFileIds, const Status& status,
    const bool keepEntireFeaturesCrossingBounds, const bool keepOnlyFeaturesInsideBounds,
    const bool keepImmediatelyConnectedWaysOutsideBounds, const bool warnOnZeroVersions,
    OsmMapPtr& cachedMap);

  /*
   * Filters features down to just those that should be replaced in the ref dataset or used to
   * replace from the sec dataset.
   */
  virtual void _filterFeatures(
    OsmMapPtr& map, const ElementCriterionPtr& featureFilter,
    const GeometryTypeCriterion::GeometryType& geometryType, const Settings& config,
    const QString& debugFileName);

  /*
   * Populates a reference and a conflated map based on the geometry type being replaced. The maps
   * can then used to derive the replacement changeset.
   */
  virtual void _processMaps(
    OsmMapPtr& refMap, OsmMapPtr& conflatedMap, const ElementCriterionPtr& refFeatureFilter,
    const ElementCriterionPtr& secFeatureFilter,
    const GeometryTypeCriterion::GeometryType& geometryType,
    const QStringList& linearFilterClassNames = QStringList());

  /*
   * Removes changeset replacement metadata tags which should be seen in raw input
   */
  void _removeMetadataTags(const OsmMapPtr& map);

  /*
   * Adds a custom tag to any element from the input with a missing child. This is primarily useful
   * in repairing relations manually that were passed in without some of their child elements after
   * the replacement changeset is written.
   */
  void _markElementsWithMissingChildren(OsmMapPtr& map);

  /*
   * Keeps track of the changeset versions for features
   */
  QMap<ElementId, long> _getIdToVersionMappings(const OsmMapPtr& map) const;

  /*
   * Adds tags to a feature that will prevent ChangesetDeriver from ever creating a delete
   * statement for it
   */
  void _addChangesetDeleteExclusionTags(OsmMapPtr& map);

  /*
   * Cut out of the reference map what you don't want, and if there is anything in the secondary
   * map, add that data in (not applicable in the cut only scenario).
   */
  OsmMapPtr _getCookieCutMap(OsmMapPtr doughMap, OsmMapPtr cutterMap,
                             const GeometryTypeCriterion::GeometryType& geometryType);

  /*
   * Removes all reviews from the map
   */
  void _removeConflateReviews(OsmMapPtr& map);

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
    const QString& debugFileName);

  /*
   * Removes all ways from the map with both MetadataTags::HootConnectedWayOutsideBounds() and
   * MetadataTags::HootSnapped()=snapped_way tags
   */
  void _removeUnsnappedImmediatelyConnectedOutOfBoundsWays(OsmMapPtr& map);

  /*
   * Excludes all features within the specified bounds from deletion during changeset derivation
   */
  void _excludeFeaturesFromChangesetDeletion(OsmMapPtr& map);

  /*
   * Final data cleanup after the changeset replacement maps have been generated to fix any errors
   * introduced.
   */
  virtual void _cleanup(OsmMapPtr& map);

  /*
   * Runs the default hoot cleaning on the data. This helps solve a lot of problems with output, but
   * its likely a subset of the cleaning ops could be run instead to be more efficient.
   */
  virtual void _clean(OsmMapPtr& map);
};

}

#endif // CHANGESET_CUT_ONLY_CREATOR_H
