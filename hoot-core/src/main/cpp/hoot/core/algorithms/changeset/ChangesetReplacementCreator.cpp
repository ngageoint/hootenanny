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
 * @copyright Copyright (C) 2019, 2020, 2021 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "ChangesetReplacementCreator.h"

// Hoot
#include <hoot/core/algorithms/ReplacementSnappedWayJoiner.h>

#include <hoot/core/algorithms/changeset/ChangesetCreator.h>

#include <hoot/core/criterion/ConflatableElementCriterion.h>
#include <hoot/core/criterion/ElementTypeCriterion.h>
#include <hoot/core/criterion/LinearCriterion.h>
#include <hoot/core/criterion/NotCriterion.h>
#include <hoot/core/criterion/OrCriterion.h>
#include <hoot/core/criterion/StatusCriterion.h>
#include <hoot/core/criterion/TagCriterion.h>
#include <hoot/core/criterion/TagKeyCriterion.h>
#include <hoot/core/criterion/WayNodeCriterion.h>

#include <hoot/core/elements/CommonElementIdFinder.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/elements/MapUtils.h>

#include <hoot/core/geometry/GeometryUtils.h>

#include <hoot/core/io/OsmMapWriterFactory.h>

#include <hoot/core/ops/ElementIdRemapper.h>
#include <hoot/core/ops/MapCropper.h>
#include <hoot/core/ops/UnconnectedWaySnapper.h>

#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/MemoryUsageChecker.h>

#include <hoot/core/visitors/RemoveElementsVisitor.h>

// Qt
#include <QElapsedTimer>

namespace hoot
{

HOOT_FACTORY_REGISTER(ChangesetReplacement, ChangesetReplacementCreator)

ChangesetReplacementCreator::ChangesetReplacementCreator() :
ChangesetReplacementCreatorAbstract()
{
  _currentChangeDerivationPassIsLinear = true;
  _boundsInterpretation = BoundsInterpretation::Lenient;
  _fullReplacement = true;

  _setGlobalOpts();
}

void ChangesetReplacementCreator::create(
  const QString& input1, const QString& input2, const geos::geom::Envelope& bounds,
  const QString& output)
{
  // This is kind of klunky to set this here, imo. However, its currently the only way to get this
  // bounds to the readers.
  create(input1, input2, GeometryUtils::envelopeToPolygon(bounds), output);
}

void ChangesetReplacementCreator::create(
  const QString& input1, const QString& input2, const std::shared_ptr<geos::geom::Polygon>& bounds,
  const QString& output)
{
  QElapsedTimer timer;
  timer.start();

  LOG_INFO("******************************************");
  _currentTask = 1;
  _progress.reset(
    new Progress(ConfigOptions().getJobId(), JOB_SOURCE, Progress::JobState::Running));
  _progress->set(
    0.0, "Generating diff maps for changeset derivation with ID: " + _changesetId + "...");

  // VALIDATION

  _input1 = input1;
  _input1Map.reset();
  _input2 = input2;
  _input2Map.reset();
  _output = output;
  _replacementBounds = bounds;
  _validateInputs();
  // This is kind of klunky to set this here, imo. However, its currently the only way to get this
  // bounds to the readers.
  conf().set(
    ConfigOptions::getBoundsKey(), GeometryUtils::polygonToString(_replacementBounds));
  _printJobDescription();

  // Every capitalized labeled section of the "create" method (e.g. "LOAD AND FILTER"), except input
  // validation is being considered a separate step for progress. The number of tasks per pass must
  // be updated as you add/remove job steps in the create logic.
  _numTotalTasks = 9;
  if (!_enableWaySnapping)
  {
    _numTotalTasks -= 2;
  }

  // LOAD AND FILTER

  _progress->set(_getJobPercentComplete(), "Loading input data...");
  QMap<ElementId, long> refIdToVersionMappings; // This is needed during snapping.
  OsmMapPtr refMap = _loadAndFilterRefMap(refIdToVersionMappings);
  OsmMapPtr secMap = _loadAndFilterSecMap();
  const int refMapSize = refMap->size();
  // If the secondary dataset is empty here and the ref dataset isn't, then we'll end up with a
  // changeset with only delete statements if the option to allow deleting reference features is
  // enabled.
  const int secMapSize = secMap->size();
  LOG_VARD(refMapSize);
  LOG_VARD(secMapSize);
  if (refMapSize == 0 && secMapSize == 0)
  {
    LOG_STATUS("Both maps empty, so skipping data removal...");
    return;
  }

  // As replacement data is added to the ref db, the ref db will have its own versions of the
  // elements added. Doing this makes sure that if we replace an overlapping element more than once
  // that we have the correct version for it when doing so.
  _syncInputVersions(refMap, secMap);

  _currentTask++;

  // CUT

  // cut the shape of the secondary data out of the reference data; pass in an unknown geometry
  // type since we're replacing all types and that also prevents alpha shapes from trying to cover
  // stragglers, which can be expensive
  _progress->set(_getJobPercentComplete(), "Cutting out features...");
  OsmMapPtr cookieCutRefMap =
    _getCookieCutMap(refMap, secMap, GeometryTypeCriterion::GeometryType::Unknown);
  const int cookieCutSize = cookieCutRefMap->size();
  LOG_VARD(cookieCutSize);
  const int dataRemoved = refMapSize - cookieCutSize;
  LOG_VARD(dataRemoved);
  // sec map size may have changed after call to _getCookieCutMap
  LOG_STATUS(
    "Replacing " << StringUtils::formatLargeNumber(dataRemoved) << " feature(s) with " <<
    StringUtils::formatLargeNumber(secMap->size()) << " feature(s)...");
  _currentTask++;

  // CLEAN

  // It seems unnecessary to need to clean the data after replacement. However, without the
  // cleaning, the output can become mangled in strange ways. The cleaning ops have been reduced
  // down to only those needed, but its probably worth determining exactly why the remaining ops
  // are actually needed at some point. This cleaning *probably* still needs to occur after cutting,
  // though, as it seems to get rid of some of the artifacts produced by that process.

  _progress->set(_getJobPercentComplete(), "Cleaning data...");
  if (secMap->size() > 0)
  {
    _clean(secMap);
  }
  if (cookieCutRefMap->size() > 0)
  {
    _clean(cookieCutRefMap);
  }
  _currentTask++;

  // COMBINE

  _progress->set(_getJobPercentComplete(), "Combining maps...");

  // This remapper will remap the IDs of all the sec elements.
  ElementIdRemapper secIdRemapper(ElementCriterionPtr(new StatusCriterion(Status::Unknown2)));
  if (ConfigOptions().getChangesetReplacementRetainReplacingDataIds())
  {
    // If we're configured to retain the sec IDs, we need to remap them here to avoid conflicts with
    // the ref data. This needs to be done b/c we're going to combine the data from both datasets in
    // in order to calculate the diff and to perform way snapping.
    LOG_INFO(secIdRemapper.getInitStatusMessage());
    secIdRemapper.apply(secMap);
    LOG_INFO(secIdRemapper.getCompletedStatusMessage());
    OsmMapWriterFactory::writeDebugMap(secMap, _changesetId + "-sec-after-id-remapping");
  }

  // Combine the cookie cut ref map back with the secondary map, which is needed to generate the
  // diff and for way snapping.
  MapUtils::combineMaps(cookieCutRefMap, secMap, false);
  OsmMapWriterFactory::writeDebugMap(cookieCutRefMap, _changesetId + "-combined-before-conflation");
  secMap.reset();
  LOG_VARD(cookieCutRefMap->size());
  OsmMapPtr combinedMap = cookieCutRefMap; // rename to reflect the state of the dataset

  _currentTask++;

  // SNAP BEFORE CHANGESET DERIVATION CROPPING

  /* After recent element ID preservation improvements, way snapping only needs to occur only when
   * the linear replacement data is very different from the data being replaced in order to stitch
   * ways at the replacement boundary seam.
   */
   if (_enableWaySnapping)
   {
    _progress->set(_getJobPercentComplete(), "Snapping linear features...");

    // Snap secondary features back to reference features if dealing with linear features where
    // ref features may have been cut along the bounds. We're being lenient here by snapping
    // secondary to reference *and* allowing conflated data to be snapped to either dataset. We only
    // want to snap ways of like types together, so we'll loop through each applicable linear type
    // and snap them separately.
    _snapUnconnectedPreChangesetMapCropping(combinedMap);

    // After snapping, perform joining to prevent unnecessary create/delete statements for the ref
    // data in the resulting changeset and generate modify statements instead.
    ReplacementSnappedWayJoiner wayJoiner(refIdToVersionMappings);
    wayJoiner.join(combinedMap);
    LOG_VART(MapProjector::toWkt(combinedMap->getProjection()));
    OsmMapWriterFactory::writeDebugMap(combinedMap, _changesetId + "-after-way-joining");

    _currentTask++;
  }

  // PRE-CHANGESET DERIVATION CROP

  _progress->set(_getJobPercentComplete(), "Cropping maps for changeset derivation...");

  // get these before the next cropping
  OsmMapPtr immediatelyConnectedOutOfBoundsWays;
  if (_enableWaySnapping)
  {
    // If we're conflating linear features with the lenient bounds requirement, copy the
    // immediately connected out of bounds ref ways to a new temp map. We'll lose those ways once we
    // crop in preparation for changeset derivation. If we don't introduce them back during
    // changeset derivation, they may not end up being snapped back to the replacement data.
    immediatelyConnectedOutOfBoundsWays = _getImmediatelyConnectedOutOfBoundsWays(refMap);
  }

  // Crop the original ref and conflated maps appropriately for changeset derivation.
  _cropMapForChangesetDerivation(
    refMap, _boundsOpts.changesetRefKeepEntireCrossingBounds,
    _boundsOpts.changesetRefKeepOnlyInsideBounds, _changesetId + "-ref-cropped-for-changeset");
  _cropMapForChangesetDerivation(
    combinedMap, _boundsOpts.changesetSecKeepEntireCrossingBounds,
    _boundsOpts.changesetSecKeepOnlyInsideBounds, _changesetId + "-sec-cropped-for-changeset");

  _currentTask++;

  // SNAP AFTER CHANGESET DERIVATION CROPPING

  if (_enableWaySnapping)
  {
    _progress->set(_getJobPercentComplete(), "Snapping linear features...");

    // The non-strict bounds interpretation way replacement workflow benefits from a second
    // set of snapping runs right before changeset derivation due to there being ways connected to
    // replacement ways that fall completely outside of the replacement bounds. However, joining
    // after this snapping caused changeset errors with some datasets and hasn't seem to be needed
    // so far...so skipping it. Note that we're being as lenient as possible with the snapping
    // here, allowing basically anything to join to anything else, which *could* end up causing
    // problems...we'll go with it for now.
    _snapUnconnectedPostChangesetMapCropping(
        refMap, combinedMap, immediatelyConnectedOutOfBoundsWays);
    immediatelyConnectedOutOfBoundsWays.reset();

    _currentTask++;
  }

  // CLEANUP

  _progress->set(_getJobPercentComplete(), "Cleaning up erroneous features...");

  if (ConfigOptions().getChangesetReplacementRetainReplacingDataIds())
  {
    // Restore the remapped relation IDs.
    secIdRemapper.restore(combinedMap);
    LOG_INFO(secIdRemapper.getRestoreCompletedStatusMessage());
    OsmMapWriterFactory::writeDebugMap(combinedMap, _changesetId + "-combined-after-id-restoring");
  }

  if (!ConfigOptions().getChangesetReplacementAllowDeletingReferenceFeaturesOutsideBounds())
  {
    // If we're not allowing the changeset deriver to generate delete statements for reference
    // features outside of the bounds, we need to mark all corresponding ref ways with a custom
    // tag that will cause the deriver to skip deleting them.
    _excludeFeaturesFromChangesetDeletion(refMap);
  }

  // clean up any mistakes introduced
  _cleanup(refMap);
  _cleanup(combinedMap);

  // Synchronize IDs between the two maps in order to cut down on unnecessary changeset
  // create/delete statements. It also prevents ways from becoming incorrectly disconnected from
  // each other in the output. This must be done with the ref/sec maps separated to avoid ID
  // conflicts.
  _synchronizeIds(refMap, combinedMap);

  _currentTask++;

  // CHANGESET GENERATION

  _progress->set(_getJobPercentComplete(), "Generating changeset...");
  _generateChangeset(refMap, combinedMap);
  _currentTask++;

  _progress->set(
    1.0, Progress::JobState::Successful,
    "Derived replacement changeset: ..." + _output.right(_maxFilePrintLength) + " with " +
    StringUtils::formatLargeNumber(_numChanges) + " changes for " +
    StringUtils::formatLargeNumber(refMapSize) + " features to replace and " +
    StringUtils::formatLargeNumber(secMapSize)  + " replacement features in " +
    StringUtils::millisecondsToDhms(timer.elapsed()) + " total.");
}

void ChangesetReplacementCreator::_setGlobalOpts()
{
  conf().set(ConfigOptions::getChangesetXmlWriterAddTimestampKey(), false);
  conf().set(ConfigOptions::getReaderAddSourceDatetimeKey(), false);
  conf().set(ConfigOptions::getWriterIncludeCircularErrorTagsKey(), false);
  // Don't let the changeset writer use the bounds. We'll handle it internally as part of C&R.
  conf().set(ConfigOptions::getChangesetIgnoreBoundsKey(), true);

  // For this being enabled to have any effect,
  // bounds.keep.immediately.connected.ways.outside.bounds must be enabled as well.
  conf().set(ConfigOptions::getBoundsTagImmediatelyConnectedOutOfBoundsWaysKey(), true);

  // will have to see if setting this to false causes problems in the future...
  conf().set(ConfigOptions::getConvertRequireAreaForPolygonKey(), false);

  // This needs to be lowered a bit to make feature de-duping and/or ID synchronization work.
  // Otherwise, we'll miss several potential ID syncs. At five decimal places, we're topping out at
  // a little over 1m of difference. The original setting of 7 is a little too precise.
  conf().set(ConfigOptions::getNodeComparisonCoordinateSensitivityKey(), 5);

  // Having to set multiple different settings to prevent missing elements from being dropped here
  // is convoluted...may need to look into changing at some point.
  conf().set(ConfigOptions::getBoundsRemoveMissingElementsKey(), false);
  conf().set(ConfigOptions::getMapReaderAddChildRefsWhenMissingKey(), true);
  conf().set(ConfigOptions::getLogWarningsForMissingElementsKey(), false);

  // If we're adding missing child element tags to parents, then we need to explicitly specify that
  // they are allowed to pass through to the changeset output. See notes where
  // _markElementsWithMissingChildren is called for more info on why this tag is added.
  if (ConfigOptions().getChangesetReplacementMarkElementsWithMissingChildren())
  {
    conf().set(
      ConfigOptions::getChangesetMetadataAllowedTagKeysKey(),
      QStringList(MetadataTags::HootMissingChild()));
  }

  // We run a custom set of cleaning ops for changeset replacement.
  conf().set(
    ConfigOptions::getMapCleanerTransformsKey(),
    conf().getList(ConfigOptions::getChangesetReplacementMapCleanerTransformsKey()));

  _boundsOpts.loadRefKeepOnlyInsideBounds = false;
  _boundsOpts.cookieCutKeepOnlyInsideBounds = false;
  _boundsOpts.changesetRefKeepOnlyInsideBounds = false;

  _boundsOpts.loadRefKeepEntireCrossingBounds = true;
  _boundsOpts.loadSecKeepOnlyInsideBounds = false;
  _boundsOpts.cookieCutKeepEntireCrossingBounds = false;
  _boundsOpts.changesetRefKeepEntireCrossingBounds = true;
  _boundsOpts.changesetSecKeepEntireCrossingBounds = true;
  _boundsOpts.changesetSecKeepOnlyInsideBounds = false;
  _boundsOpts.inBoundsStrict = false;

  _boundsOpts.loadRefKeepImmediateConnectedWaysOutsideBounds = true;
  _boundsOpts.loadSecKeepEntireCrossingBounds = true;
  _boundsOpts.changesetAllowDeletingRefOutsideBounds = true;

  conf().set(
    ConfigOptions::getChangesetReplacementAllowDeletingReferenceFeaturesOutsideBoundsKey(),
    _boundsOpts.changesetAllowDeletingRefOutsideBounds);

  LOG_VART(_boundsOpts.loadRefKeepEntireCrossingBounds);
  LOG_VART(_boundsOpts.loadRefKeepOnlyInsideBounds);
  LOG_VART(_boundsOpts.loadRefKeepImmediateConnectedWaysOutsideBounds);
  LOG_VART(_boundsOpts.loadSecKeepEntireCrossingBounds);
  LOG_VART(_boundsOpts.loadSecKeepOnlyInsideBounds);
  LOG_VART(_boundsOpts.cookieCutKeepEntireCrossingBounds);
  LOG_VART(_boundsOpts.cookieCutKeepOnlyInsideBounds);
  LOG_VART(_boundsOpts.changesetRefKeepEntireCrossingBounds);
  LOG_VART(_boundsOpts.changesetRefKeepOnlyInsideBounds);
  LOG_VART(_boundsOpts.changesetSecKeepEntireCrossingBounds);
  LOG_VART(_boundsOpts.changesetSecKeepOnlyInsideBounds);
  LOG_VART(_boundsOpts.changesetAllowDeletingRefOutsideBounds);
  LOG_VART(_boundsOpts.inBoundsStrict);
}

void ChangesetReplacementCreator::_syncInputVersions(const OsmMapPtr& refMap,
                                                     const OsmMapPtr& secMap)
{
  LOG_STATUS("Synchronizing elements...");

  const QSet<ElementId> idsInCommon = CommonElementIdFinder::findCommonElementIds(refMap, secMap);

  int ctr = 0;
  for ( QSet<ElementId>::const_iterator itr = idsInCommon.begin(); itr != idsInCommon.end(); ++itr)
  {
    ConstElementPtr refElement = refMap->getElement(*itr);
    assert(refElement);
    ElementPtr secElement = secMap->getElement(*itr);
    assert(secElement);
    if (refElement->getVersion() > secElement->getVersion())
    {
      secElement->setVersion(refElement->getVersion());
      ctr++;
    }
  }

  LOG_INFO("Synchronized " << ctr << " element versions.");
}

OsmMapPtr ChangesetReplacementCreator::_loadAndFilterRefMap(
  QMap<ElementId, long>& refIdToVersionMappings)
{
  // load the data to replace
  OsmMapPtr refMap =
    _loadInputMap(
     "ref", _input1, true, Status::Unknown1, _boundsOpts.loadRefKeepEntireCrossingBounds,
     _boundsOpts.loadRefKeepOnlyInsideBounds,
     _boundsOpts.loadRefKeepImmediateConnectedWaysOutsideBounds, true, _input1Map);

  // Drop all C&R specific metadata tags which should not exist yet, just in case they got in the
  // input somehow.
  _removeMetadataTags(refMap);

  if (ConfigOptions().getChangesetReplacementMarkElementsWithMissingChildren())
  {
    // Find any elements with missing children and tag them with a custom tag, as its possible we'll
    // break them during this process. There's really nothing that can be done about that, since we
    // don't have access to the missing children elements. Any elements with missing children may
    // require manual cleanup after the resulting changeset is applied.
    _markElementsWithMissingChildren(refMap);
  }

  // Keep a mapping of the original ref element ids to versions, as we'll need the original
  // versions later.
  refIdToVersionMappings = _getIdToVersionMappings(refMap);

  // If we have a lenient bounds requirement and linear features, we need to exclude all ways
  // outside of the bounds but immediately connected to a way crossing the bounds from deletion.
  _addChangesetDeleteExclusionTags(refMap);

  // Prune the ref dataset down to just the geometry types specified by the filter, so we don't end
  // up modifying anything else.
  if (_geometryTypeFilter)
  {
    _filterFeatures(
      refMap, _geometryTypeFilter, GeometryTypeCriterion::Unknown, conf(),
      _changesetId + "-ref-after-pruning");
  }

  return refMap;
}

OsmMapPtr ChangesetReplacementCreator::_loadAndFilterSecMap()
{
  // load the data that we're replacing with; Depending on how we're configured, we may keep source
  // IDs here initially and then need to later remap them to avoid conflict when this data needs to
  // be combined with data from the ref map.
  OsmMapPtr secMap =
    _loadInputMap(
      "sec", _input2, ConfigOptions().getChangesetReplacementRetainReplacingDataIds(),
      Status::Unknown2, _boundsOpts.loadSecKeepEntireCrossingBounds,
      _boundsOpts.loadSecKeepOnlyInsideBounds, false, true, _input2Map);

  _removeMetadataTags(secMap);

  if (ConfigOptions().getChangesetReplacementMarkElementsWithMissingChildren())
  {
    _markElementsWithMissingChildren(secMap);
  }

  // Prune the sec dataset down to just the feature types specified by the filter, so we don't end
  // up modifying anything else.
  if (_geometryTypeFilter)
  {
    const Settings secFilterSettings =
      _replacementFilterOptions.size() == 0 ? conf() : _replacementFilterOptions;
    _filterFeatures(
      secMap, _geometryTypeFilter, GeometryTypeCriterion::Unknown, secFilterSettings,
      _changesetId + "-sec-after-pruning");
  }

  return secMap;
}

void ChangesetReplacementCreator::_snapUnconnectedPreChangesetMapCropping(OsmMapPtr& combinedMap)
{
  LOG_INFO("Snapping unconnected ways to each other in replacement map...");

  QStringList snapWayStatuses("Input2");
  snapWayStatuses.append("Conflated");
  QStringList snapToWayStatuses("Input1");
  snapToWayStatuses.append("Conflated");
  // TODO: Now that we're more strict on what can be snapped type-wise, we may not need to
  // break these snaps out per feature type.
  for (int i = 0; i < _linearFilterClassNames.size(); i++)
  {
    _snapUnconnectedWays(
      combinedMap, snapWayStatuses, snapToWayStatuses, _linearFilterClassNames.at(i), false,
      _changesetId + "-conflated-snapped-sec-to-ref-1");
  }
}

void ChangesetReplacementCreator::_snapUnconnectedPostChangesetMapCropping(
  OsmMapPtr& refMap, OsmMapPtr& combinedMap, OsmMapPtr& immediatelyConnectedOutOfBoundsWays)
{
  QStringList snapWayStatuses;
  snapWayStatuses.append("Input2");
  snapWayStatuses.append("Conflated");
  snapWayStatuses.append("Input1");
  QStringList snapToWayStatuses;
  snapToWayStatuses.append("Input1");
  snapToWayStatuses.append("Conflated");
  snapToWayStatuses.append("Input2");
  LOG_VARD(_linearFilterClassNames);

  // Snap anything that can be snapped per feature type.
  LOG_INFO("Snapping unconnected ways to each other in replacement map...");
  for (int i = 0; i < _linearFilterClassNames.size(); i++)
  {
    _snapUnconnectedWays(
      combinedMap, snapWayStatuses, snapToWayStatuses, _linearFilterClassNames.at(i), false,
      _changesetId + "-conflated-snapped-sec-to-ref-2");
  }

  // combine the conflated map with the immediately connected out of bounds ways
  MapUtils::combineMaps(combinedMap, immediatelyConnectedOutOfBoundsWays, true);
  OsmMapWriterFactory::writeDebugMap(combinedMap, _changesetId + "-conflated-connected-combined");

  // Snap the connected ways to other ways in the conflated map. Mark the ways that were
  // snapped, as we'll need that info in the next step.
  LOG_INFO("Snapping unconnected ways to each other in replacement map...");
  for (int i = 0; i < _linearFilterClassNames.size(); i++)
  {
    _snapUnconnectedWays(
      combinedMap, snapWayStatuses, snapToWayStatuses, _linearFilterClassNames.at(i),
      true, _changesetId + "-conflated-snapped-immediately-connected-out-of-bounds");
  }

  // remove any ways that weren't snapped
  _removeUnsnappedImmediatelyConnectedOutOfBoundsWays(combinedMap);

  // Copy the connected ways back into the ref map as well, so the changeset will derive
  // properly.
  MapUtils::combineMaps(refMap, immediatelyConnectedOutOfBoundsWays, true);
  OsmMapWriterFactory::writeDebugMap(refMap, _changesetId + "-ref-connected-combined");
}

void ChangesetReplacementCreator::_snapUnconnectedWays(
  OsmMapPtr& map, const QStringList& snapWayStatuses, const QStringList& snapToWayStatuses,
  const QString& typeCriterionClassName, const bool markSnappedWays, const QString& debugFileName)
{
  LOG_DEBUG(
    "Snapping ways for map: " << map->getName() << ", with filter type: " <<
    typeCriterionClassName << ", snap way statuses: " << snapWayStatuses <<
    ", snap to way statuses: " << snapToWayStatuses << " ...");

  UnconnectedWaySnapper lineSnapper;
  lineSnapper.setConfiguration(conf());
  // override some of the default config
  lineSnapper.setSnapToWayStatuses(snapToWayStatuses);
  lineSnapper.setSnapWayStatuses(snapWayStatuses);
  lineSnapper.setMarkSnappedWays(markSnappedWays);
  // TODO: Do we need a way to derive the way node crit from the input feature filter crit?
  lineSnapper.setWayNodeToSnapToCriterionClassName(WayNodeCriterion::className());
  lineSnapper.setWayToSnapCriterionClassName(typeCriterionClassName);
  lineSnapper.setWayToSnapToCriterionClassName(typeCriterionClassName);
  // This prevents features of different types snapping to each other that shouldn't do so.
  // Arbitrarily picking a score here...may require further tweaking.
  lineSnapper.setMinTypeMatchScore(0.8);
  // Here, we're excluding things unlikely to ever need to be snapped to each other. Arguably, this
  // list could be made part of UnconnectedWaySnapper's config instead.
  lineSnapper.setTypeExcludeKvps(ConfigOptions().getChangesetReplacementSnapExcludeTypes());
  lineSnapper.apply(map);
  LOG_DEBUG(lineSnapper.getCompletedStatusMessage());

  MapProjector::projectToWgs84(map);   // snapping works in planar
  LOG_VART(MapProjector::toWkt(map->getProjection()));
  MemoryUsageChecker::getInstance().check();
  OsmMapWriterFactory::writeDebugMap(map, debugFileName);
}

OsmMapPtr ChangesetReplacementCreator::_getImmediatelyConnectedOutOfBoundsWays(
  const ConstOsmMapPtr& map) const
{
  const QString outputMapName = "connected-ways";
  LOG_INFO(
    "Copying immediately connected out of bounds ways from: " << map->getName() <<
    " to new map: " << outputMapName << "...");

  std::shared_ptr<ChainCriterion> copyCrit(
    new ChainCriterion(
      std::shared_ptr<WayCriterion>(new WayCriterion()),
      std::shared_ptr<TagKeyCriterion>(
        new TagKeyCriterion(MetadataTags::HootConnectedWayOutsideBounds()))));
  OsmMapPtr connectedWays = MapUtils::getMapSubset(map, copyCrit);
  connectedWays->setName(outputMapName);
  LOG_VART(MapProjector::toWkt(connectedWays->getProjection()));
  MemoryUsageChecker::getInstance().check();
  OsmMapWriterFactory::writeDebugMap(connectedWays, _changesetId + "-connected-ways");
  return connectedWays;
}

void ChangesetReplacementCreator::_removeUnsnappedImmediatelyConnectedOutOfBoundsWays(
  OsmMapPtr& map)
{
  LOG_INFO(
    "Removing any immediately connected ways that were not previously snapped in: " <<
    map->getName() << "...");

  RemoveElementsVisitor removeVis;
  removeVis.addCriterion(ElementCriterionPtr(new WayCriterion()));
  removeVis.addCriterion(
    ElementCriterionPtr(new TagKeyCriterion(MetadataTags::HootConnectedWayOutsideBounds())));
  removeVis.addCriterion(
    ElementCriterionPtr(
      new NotCriterion(
        std::shared_ptr<TagKeyCriterion>(new TagKeyCriterion(MetadataTags::HootSnapped())))));
  removeVis.setChainCriteria(true);
  removeVis.setRecursive(true);
  map->visitRw(removeVis);
  LOG_DEBUG(removeVis.getCompletedStatusMessage());

  MemoryUsageChecker::getInstance().check();
  LOG_VART(MapProjector::toWkt(map->getProjection()));
  OsmMapWriterFactory::writeDebugMap(
    map, _changesetId + "-" + map->getName() + "-unsnapped-removed");
}

void ChangesetReplacementCreator::_cropMapForChangesetDerivation(
  OsmMapPtr& map, const bool keepEntireFeaturesCrossingBounds,
  const bool keepOnlyFeaturesInsideBounds, const QString& debugFileName)
{
  if (map->size() == 0)
  {
    LOG_DEBUG("Skipping cropping empty map: " << map->getName() << "...");
    return;
  }

  LOG_INFO("Cropping map: " << map->getName() << " for changeset derivation...");
  LOG_VART(MapProjector::toWkt(map->getProjection()));
  LOG_VARD(keepEntireFeaturesCrossingBounds);
  LOG_VARD(keepOnlyFeaturesInsideBounds);

  MapCropper cropper;
  cropper.setBounds(_replacementBounds);
  cropper.setKeepEntireFeaturesCrossingBounds(keepEntireFeaturesCrossingBounds);
  cropper.setKeepOnlyFeaturesInsideBounds(keepOnlyFeaturesInsideBounds);
  // We're not going to remove missing elements, as we want to have as minimal of an impact on
  // the resulting changeset as possible.
  cropper.setRemoveMissingElements(false);
  cropper.apply(map);
  LOG_DEBUG(cropper.getCompletedStatusMessage());

  MemoryUsageChecker::getInstance().check();
  LOG_VART(MapProjector::toWkt(map->getProjection()));
  OsmMapWriterFactory::writeDebugMap(map, debugFileName);
  LOG_DEBUG("Cropped map: " << map->getName() << " size: " << map->size());
}

void ChangesetReplacementCreator::_generateChangeset(OsmMapPtr& refMap, OsmMapPtr& combinedMap)
{
  LOG_STATUS(
    "Generating changeset for ref map of size: " <<
    StringUtils::formatLargeNumber(refMap->size()) << " and sec map of size: " <<
    StringUtils::formatLargeNumber(combinedMap->size()) << "...");

  // Since we're not removing missing elements, it may be possible we have null elements passed to
  // changeset generation. This is a little concerning, b/c that would seem to affect the accuracy
  // of the generated changeset since ChangesetDeriver relies on whether an element is null or not
  // as part of its logic. However, haven't found any bugs related to this yet.

  // We have some instances where modify and delete changes are being generated for the same
  // element, which causes error during changeset application. Eventually, we should eliminate their
  // causes, but for now we'll activate changeset cleaning to get rid of the delete statements.
  // Unfortunately, this will make the changeset writing memory bound.
  _changesetCreator->setClean(true);
  _changesetCreator->create(refMap, combinedMap, _output);
  _numChanges = _changesetCreator->getNumTotalChanges();
}

}
