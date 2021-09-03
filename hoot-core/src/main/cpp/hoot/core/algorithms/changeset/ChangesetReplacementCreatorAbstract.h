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
 * @copyright Copyright (C) 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef CHANGESET_REPLACEMENT_CREATOR_ABSTRACT_H
#define CHANGESET_REPLACEMENT_CREATOR_ABSTRACT_H

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/criterion/GeometryTypeCriterion.h>
#include <hoot/core/algorithms/changeset/ChangesetReplacement.h>
#include <hoot/core/util/Progress.h>

namespace hoot
{

class ChangesetCreator;
class Settings;

/**
 * @brief The ChangesetReplacementCreatorAbstract class is a high level abstract class for prepping
 * data for replacement changeset generation (changesets which replace features inside of a
 * specified bounds) and then calls on the appropriate changeset file writer to output a changeset
 * file.
 *
 * This class uses a customized workflow that depends upon the feature type the changeset is being
 * generated for, whether all the reference features or just those that overlap secondary features
 * are to be replaced, and how strict the AOI is to be interpreted. ChangesetCreator is used for the
 * actual changeset generation and file output. This class handles the cookie cutting and a host of
 * other things that need to happen before the replacement changeset is generated. The secondary
 * data added to the output changeset, as well as the reference data removed from the changeset can
 * be further restricted with a non-geometry type filter.
 *
 * Occasionally, relations will be passed in with some members missing. This class will optionally
 * tag those with a custom metadata tag to be passed to the changeset output. This allows for
 * potential manual repairing of those relations after the changeset is written, and after that the
 * tag can then be removed. This is also a configurable feature, which can be turned off.
 *
 * @todo break this up into separate classes by function:
 *  - input prep
 *  - data replacement
 *  - snapping?
 *  - changeset derivation
 *  - cleanup
 * @todo rename all references to conflated data since maps aren't conflated anymore; also rename
 * all datasets in terms of "toReplace" and "replacement"
 */
class ChangesetReplacementCreatorAbstract : public ChangesetReplacement
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

  static QString className() { return "ChangesetReplacementCreatorAbstract"; }

  static const QString JOB_SOURCE;

  ChangesetReplacementCreatorAbstract();
  virtual ~ChangesetReplacementCreatorAbstract() = default;

  /**
   * @see ChangesetReplacement
   */
  void setChangesetOptions(const bool printStats, const QString& statsOutputFile,
                           const QString osmApiDbUrl) override;

  int getNumChanges() const  override{ return _numChanges; }

  void setFullReplacement(const bool full) override { _fullReplacement = full; }
  void setBoundsInterpretation(const BoundsInterpretation& interpretation) override
  { _boundsInterpretation = interpretation; }
  void setEnableWaySnapping(const bool enable) override { _enableWaySnapping = enable; }
  void setChangesetId(const QString& id) override { _changesetId = id; }

  QString toString() const override { return className(); }

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

  // determines if the current changeset map generation pass contains only linear features
  bool _currentChangeDerivationPassIsLinear;

  // used to keep log messages with urls in them shorter
  int _maxFilePrintLength;

  // If true, all the ref data gets replaced. If false, only the ref data that intersects with the
  // alpha shape of the sec data gets replaced.
  bool _fullReplacement;

  // A set of geometry type filters, organized by core geometry type (point, line, poly) to
  // separately filter the input datasets on.
  // TODO: move to cut only
  QMap<GeometryTypeCriterion::GeometryType, ElementCriterionPtr> _geometryTypeFilters;
  bool _geometryFiltersSpecified;

  // A list of linear geometry criterion classes to apply way snapping to.
  QStringList _linearFilterClassNames;

  bool _enableWaySnapping;

  // helpful to name the debug map files when doing successive replacements
  QString _changesetId;
  // handles changeset generation and output
  std::shared_ptr<ChangesetCreator> _changesetCreator;
  int _numChanges;

  std::shared_ptr<Progress> _progress;
  int _numTotalTasks;
  int _currentTask;

  float _getJobPercentComplete() const;

  virtual void _setGlobalOpts() = 0;
  void _validateInputs() const;
  void _printJobDescription() const;
  QString _boundsInterpretationToString(const BoundsInterpretation& boundsInterpretation) const;

  OsmMapPtr _loadInputMap(
    const QString& mapName, const QString& inputUrl, const bool useFileIds, const Status& status,
    const bool keepEntireFeaturesCrossingBounds, const bool keepOnlyFeaturesInsideBounds,
    const bool keepImmediatelyConnectedWaysOutsideBounds, const bool warnOnZeroVersions,
    OsmMapPtr& cachedMap) const;

  /*
   * Filters features down to just those that should be replaced in the ref dataset or used to
   * replace from the sec dataset.
   */
  void _filterFeatures(
    const OsmMapPtr& map, const ElementCriterionPtr& featureFilter,
    const GeometryTypeCriterion::GeometryType& geometryType, const Settings& config,
    const QString& debugFileName) const;

  /*
   * Removes changeset replacement metadata tags which should be seen in raw input
   */
  void _removeMetadataTags(const OsmMapPtr& map) const;

  /*
   * Adds a custom tag to any element from the input with a missing child. This is primarily useful
   * in repairing relations manually that were passed in without some of their child elements after
   * the replacement changeset is written.
   */
  void _markElementsWithMissingChildren(const OsmMapPtr& map) const;

  /*
   * Keeps track of the changeset versions for features
   */
  QMap<ElementId, long> _getIdToVersionMappings(const OsmMapPtr& map) const;

  /*
   * Adds tags to a feature that will prevent ChangesetDeriver from ever creating a delete
   * statement for it
   */
  void _addChangesetDeleteExclusionTags(OsmMapPtr& map) const;

  /*
   * Cut out of the reference map what you don't want, and if there is anything in the secondary
   * map, add that data in (not applicable in the cut only scenario).
   */
  OsmMapPtr _getCookieCutMap(OsmMapPtr doughMap, OsmMapPtr cutterMap,
                             const GeometryTypeCriterion::GeometryType& geometryType) const;

  /*
   * Excludes all features within the specified bounds from deletion during changeset derivation
   */
  void _excludeFeaturesFromChangesetDeletion(OsmMapPtr& map) const;

  /*
   * Final data cleanup after the changeset replacement maps have been generated to fix any errors
   * introduced.
   */
  void _cleanup(OsmMapPtr& map) const;

  /*
   * Replaces the IDs of elements in the replacment map that are identical with those in the map
   * being replaced with the IDs from the map being replaced.
   */
  void _synchronizeIds(OsmMapPtr mapBeingReplaced, OsmMapPtr replacementMap) const;

  /*
   * Replaces the IDs of elements in the replacment maps that are identical with those in the maps
   * being replaced with the IDs from the maps being replaced.
   */
  void _synchronizeIds(
    const QList<OsmMapPtr>& mapsBeingReplaced, const QList<OsmMapPtr>& replacementMaps) const;

  /*
   * Runs the default hoot cleaning on the data. This helps solve a lot of problems with output, but
   * its likely a subset of the cleaning ops could be run instead to be more efficient.
   */
  void _clean(OsmMapPtr& map) const;
};

}

#endif // CHANGESET_REPLACEMENT_CREATOR_ABSTRACT_H
