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
#include "ChangesetReplacementCreator.h"

// Hoot
#include <hoot/core/algorithms/ReplacementSnappedWayJoiner.h>

#include <hoot/core/algorithms/changeset/ChangesetCreator.h>

#include <hoot/core/criterion/ConflatableElementCriterion.h>
#include <hoot/core/criterion/LinearCriterion.h>
#include <hoot/core/criterion/OrCriterion.h>

#include <hoot/core/elements/MapUtils.h>

#include <hoot/core/io/OsmMapWriterFactory.h>

//#include <hoot/core/ops/ElementIdRemapper.h>

#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/GeometryUtils.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/util/MemoryUsageChecker.h>

// Qt
#include <QElapsedTimer>

namespace hoot
{

HOOT_FACTORY_REGISTER(ChangesetReplacement, ChangesetReplacementCreator)

ChangesetReplacementCreator::ChangesetReplacementCreator() :
ChangesetCutOnlyCreator()
{
  _currentChangeDerivationPassIsLinear = true;
  _boundsInterpretation = BoundsInterpretation::Lenient;
  _fullReplacement = true;

  _setGlobalOpts();
}

void ChangesetReplacementCreator::setGeometryFilters(const QStringList& filterClassNames)
{
  LOG_VART(filterClassNames);
  if (!filterClassNames.isEmpty())
  {
    _geometryFiltersSpecified = true;
    _geometryTypeFilter.reset();
    _linearFilterClassNames.clear();

    for (int i = 0; i < filterClassNames.size(); i++)
    {
      const QString filterClassName = filterClassNames.at(i);
      LOG_VART(filterClassName);

      // Fail if the filter doesn't map to a geometry type.
      std::shared_ptr<GeometryTypeCriterion> filter =
        std::dynamic_pointer_cast<GeometryTypeCriterion>(
          std::shared_ptr<ElementCriterion>(
            Factory::getInstance().constructObject<ElementCriterion>(filterClassName)));
      if (!filter)
      {
        throw IllegalArgumentException(
          "Invalid feature geometry type filter: " + filterClassName +
          ". Filter must be a GeometryTypeCriterion.");
      }

      if (!_geometryTypeFilter)
      {
        _geometryTypeFilter = filter;
      }
      else
      {
        _geometryTypeFilter = OrCriterionPtr(new OrCriterion(_geometryTypeFilter, filter));
      }

      if (filter->getGeometryType() == GeometryTypeCriterion::GeometryType::Line)
      {
        _linearFilterClassNames.append(filterClassName);
      }
    }
  }

  // have to call this method to keep filtering from erroring...shouldn't have to...should just init
  // itself internally when no geometry filters are specified)
  if (!_geometryTypeFilter)
  {
    _geometryFiltersSpecified = false;
    _linearFilterClassNames =
      ConflatableElementCriterion::getCriterionClassNamesByGeometryType(
        GeometryTypeCriterion::GeometryType::Line);
  }
  _linearFilterClassNames.removeAll(QString::fromStdString(LinearCriterion::className()));

  LOG_VARD(_geometryTypeFilters.size());
  LOG_VART(_linearFilterClassNames);
}

void ChangesetReplacementCreator::create(
  const QString& input1, const QString& input2, const geos::geom::Envelope& bounds,
  const QString& output)
{
  QElapsedTimer timer;
  timer.start();

  // VALIDATION

  _input1 = input1;
  _input1Map.reset();
  _input2 = input2;
  _input2Map.reset();
  _output = output;
  // TODO: It makes more sense to store the bounds and then just convert it to a string as needed.
  // The default string stores six decimal places, which should be fine for a bounds. Strangely,
  // when I store the bounds or try to increase the precision of the bounds string, I'm getting a
  // lot of test output issues...needs to be looked into.
  _replacementBounds = GeometryUtils::envelopeToConfigString(bounds);
  conf().set(ConfigOptions::getConvertBoundingBoxKey(), _replacementBounds);
  _validateInputs();
  _printJobDescription();

  LOG_INFO("******************************************");
  LOG_STATUS("Generating diff maps for changeset derivation with ID: " << _changesetId << "...");
  LOG_VARD(toString());

  // LOAD AND FILTER

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

  // CUT

  // cut the shape of the secondary data out of the reference data; pass in an unknown geometry
  // type since we're replacing all types and that also prevents alpha shapes from trying to cover
  // stragglers, which can be expensive
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

  // At one point it was necessary to re-number the relations in the sec map, as they could have ID
  // overlap with those in the cookie cut ref map at this point. It seemed that this was due to the
  // fact that relations in the two maps had been added independently of each other during cropping.
  // However, after some refactoring this doesn't seem to be the case anymore. If we run into this
  // situation again, we can go back in the history to resurrect the use of the ElementIdRemapper
  // for relations here.

  // CLEAN

  // It seems unnecessary to need to clean the data after replacement. However, without the
  // cleaning, the output can become mangled in strange ways. The cleaning ops have been reduced
  // down to only those needed, but its probably worth to determine exactly why the remaining ops
  // are actually needed at some point. This cleaning *probably* still needs to occur after cutting,
  // though, as it seems to get rid of some of the artifacts produced by that process.

  if (secMap->size() > 0)
  {
    _clean(secMap);
  }
  if (cookieCutRefMap->size() > 0)
  {
    _clean(cookieCutRefMap);
  }

  // SNAP BEFORE CHANGESET DERIVATION CROPPING

  // Had an idea here to try to load source IDs for sec data, remap sec IDs to be unique just before
  // the ref and sec have to be combined, and then restore the original sec IDs after the snapping
  // is complete. The idea was to reduce the need for ID synchronization, which doesn't work
  // perfectly yet (of course, if you're replacing with data from a different data source the ID
  // sync would happen regardless...just not needed for OSM to OSM replacement). Unfortunately, this
  // leads to all kinds of duplicate ID errors when the resulting changesets are applied.
  //ElementIdRemapper secIdRemapper;
  //secIdRemapper.apply(secMap);

  // Combine the cookie cut ref map back with the secondary map, which is needed for way snapping.
  MapUtils::combineMaps(cookieCutRefMap, secMap, false);
  OsmMapWriterFactory::writeDebugMap(cookieCutRefMap, _changesetId + "-combined-before-conflation");
  secMap.reset();
  LOG_VARD(cookieCutRefMap->size());
  OsmMapPtr combinedMap = cookieCutRefMap; // rename to reflect the state of the dataset

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

  // PRE-CHANGESET DERIVATION CROP

  // If we're conflating linear features with the lenient bounds requirement, copy the
  // immediately connected out of bounds ref ways to a new temp map. We'll lose those ways once we
  // crop in preparation for changeset derivation. If we don't introduce them back during
  // changeset derivation, they may not end up being snapped back to the replacement data.
  OsmMapPtr immediatelyConnectedOutOfBoundsWays = _getImmediatelyConnectedOutOfBoundsWays(refMap);

  // Crop the original ref and conflated maps appropriately for changeset derivation.
  _cropMapForChangesetDerivation(
    refMap, _boundsOpts.changesetRefKeepEntireCrossingBounds,
    _boundsOpts.changesetRefKeepOnlyInsideBounds, _changesetId + "-ref-cropped-for-changeset");
  _cropMapForChangesetDerivation(
    combinedMap, _boundsOpts.changesetSecKeepEntireCrossingBounds,
    _boundsOpts.changesetSecKeepOnlyInsideBounds, _changesetId + "-sec-cropped-for-changeset");

  // SNAP AFTER CHANGESET DERIVATION CROPPING

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

  if (!ConfigOptions().getChangesetReplacementAllowDeletingReferenceFeaturesOutsideBounds())
  {
    // If we're not allowing the changeset deriver to generate delete statements for reference
    // features outside of the bounds, we need to mark all corresponding ref ways with a custom
    // tag that will cause the deriver to skip deleting them.
    _excludeFeaturesFromChangesetDeletion(refMap);
  }

  //secIdRemapper.restore(combinedMap);

  // CLEANUP

  // clean up any mistakes introduced
  _cleanup(refMap);
  _cleanup(combinedMap);

  // Synchronize IDs between the two maps in order to cut down on unnecessary changeset
  // create/delete statements. It also prevents ways from becoming incorrectly disconnected from
  // each other in the output. This must be done with the ref/sec maps separated to avoid ID
  // conflicts.
  _synchronizeIds(refMap, combinedMap);

  // CHANGESET GENERATION

  _generateChangeset(refMap, combinedMap);

  LOG_STATUS(
    "Derived replacement changeset: ..." << _output.right(_maxFilePrintLength) << " with " <<
    StringUtils::formatLargeNumber(_numChanges) << " changes in " <<
    StringUtils::millisecondsToDhms(timer.elapsed()) << " total.");
}

void ChangesetReplacementCreator::_setGlobalOpts()
{
  conf().set(ConfigOptions::getChangesetXmlWriterAddTimestampKey(), false);
  conf().set(ConfigOptions::getReaderAddSourceDatetimeKey(), false);
  conf().set(ConfigOptions::getWriterIncludeCircularErrorTagsKey(), false);

  // For this being enabled to have any effect,
  // convert.bounding.box.keep.immediately.connected.ways.outside.bounds must be enabled as well.
  conf().set(ConfigOptions::getConvertBoundingBoxTagImmediatelyConnectedOutOfBoundsWaysKey(), true);

  // will have to see if setting this to false causes problems in the future...
  conf().set(ConfigOptions::getConvertRequireAreaForPolygonKey(), false);

  // This needs to be lowered a bit to make feature de-duping and/or ID synchronization work.
  // Otherwise, we'll miss several potential ID syncs. At five decimal places, we're topping out at
  // a little over 1m of difference. The original setting of 7 is a little too precise.
  conf().set(ConfigOptions::getNodeComparisonCoordinateSensitivityKey(), 5);

  // Having to set multiple different settings to prevent missing elements from being dropped here
  // is convoluted...may need to look into changing at some point.
  conf().set(ConfigOptions::getConvertBoundingBoxRemoveMissingElementsKey(), false);
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

OsmMapPtr ChangesetReplacementCreator::_loadAndFilterRefMap(
  QMap<ElementId, long>& refIdToVersionMappings)
{
  // load the data to replace
  OsmMapPtr refMap =
    _loadInputMap(
     "ref", _input1, true, Status::Unknown1, _boundsOpts.loadRefKeepEntireCrossingBounds,
     _boundsOpts.loadRefKeepOnlyInsideBounds,
     _boundsOpts.loadRefKeepImmediateConnectedWaysOutsideBounds, true, _input1Map);
  MemoryUsageChecker::getInstance().check();

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
  // load the data that we're replacing with; We don't keep source IDs here to avoid conflict with
  // the reference data. Data from the two maps will have to be combined during snapping.
  OsmMapPtr secMap =
    _loadInputMap(
      "sec", _input2, false, Status::Unknown2, _boundsOpts.loadSecKeepEntireCrossingBounds,
      _boundsOpts.loadSecKeepOnlyInsideBounds, false, true, _input2Map);
  MemoryUsageChecker::getInstance().check();

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
