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
#include "ChangesetReplacementCreator7.h"

// Hoot
#include <hoot/core/algorithms/ReplacementSnappedWayJoiner.h>
#include <hoot/core/algorithms/WayJoinerAdvanced.h>
#include <hoot/core/algorithms/WayJoinerBasic.h>

#include <hoot/core/algorithms/alpha-shape/AlphaShapeGenerator.h>

#include <hoot/core/algorithms/changeset/ChangesetCreator.h>
#include <hoot/core/algorithms/changeset/ChangesetReplacementElementIdSynchronizer.h>

#include <hoot/core/conflate/CookieCutter.h>
#include <hoot/core/conflate/SuperfluousConflateOpRemover.h>
#include <hoot/core/conflate/UnifyingConflator.h>

#include <hoot/core/criterion/ConflatableElementCriterion.h>
#include <hoot/core/criterion/ElementTypeCriterion.h>
#include <hoot/core/criterion/HighwayCriterion.h>
#include <hoot/core/criterion/InBoundsCriterion.h>
#include <hoot/core/criterion/LinearCriterion.h>
#include <hoot/core/criterion/NotCriterion.h>
#include <hoot/core/criterion/OrCriterion.h>
#include <hoot/core/criterion/PointCriterion.h>
#include <hoot/core/criterion/PolygonCriterion.h>
#include <hoot/core/criterion/RelationWithLinearMembersCriterion.h>
#include <hoot/core/criterion/RelationWithPointMembersCriterion.h>
#include <hoot/core/criterion/RelationWithPolygonMembersCriterion.h>
//#include <hoot/core/criterion/RoundaboutCriterion.h>
#include <hoot/core/criterion/StatusCriterion.h>
#include <hoot/core/criterion/TagCriterion.h>
#include <hoot/core/criterion/TagKeyCriterion.h>
#include <hoot/core/criterion/WayNodeCriterion.h>

#include <hoot/core/elements/ElementDeduplicator.h>
#include <hoot/core/elements/ElementGeometryUtils.h>
#include <hoot/core/elements/MapUtils.h>

#include <hoot/core/index/OsmMapIndex.h>

#include <hoot/core/io/IoUtils.h>
#include <hoot/core/io/OsmGeoJsonReader.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>

#include <hoot/core/ops/CopyMapSubsetOp.h>
#include <hoot/core/ops/ElementIdToVersionMapper.h>
#include <hoot/core/ops/MapCleaner.h>
#include <hoot/core/ops/MapCropper.h>
#include <hoot/core/ops/NamedOp.h>
#include <hoot/core/ops/PointsToPolysConverter.h>
#include <hoot/core/ops/RecursiveElementRemover.h>
#include <hoot/core/ops/RecursiveSetTagValueOp.h>
#include <hoot/core/ops/RemoveElementByEid.h>
#include <hoot/core/ops/RemoveEmptyRelationsOp.h>
#include <hoot/core/ops/SuperfluousNodeRemover.h>
#include <hoot/core/ops/UnconnectedWaySnapper.h>
#include <hoot/core/ops/WayJoinerOp.h>

#include <hoot/core/schema/OsmSchema.h>

#include <hoot/core/util/Boundable.h>
#include <hoot/core/util/CollectionUtils.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/ConfigUtils.h>
#include <hoot/core/util/DbUtils.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/GeometryUtils.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/util/MemoryUsageChecker.h>

#include <hoot/core/visitors/ApiTagTruncateVisitor.h>
#include <hoot/core/visitors/ElementIdsVisitor.h>
#include <hoot/core/visitors/FilteredVisitor.h>
#include <hoot/core/visitors/RemoveElementsVisitor.h>
#include <hoot/core/visitors/RemoveDuplicateAreasVisitor.h>
#include <hoot/core/visitors/RemoveDuplicateRelationMembersVisitor.h>
#include <hoot/core/visitors/RemoveInvalidMultilineStringMembersVisitor.h>
#include <hoot/core/visitors/RemoveMissingElementsVisitor.h>
#include <hoot/core/visitors/RemoveTagsVisitor.h>
#include <hoot/core/visitors/SetTagValueVisitor.h>
#include <hoot/core/visitors/SpatialIndexer.h>

// tgs
#include <tgs/RStarTree/MemoryPageStore.h>

// Qt
#include <QElapsedTimer>

namespace hoot
{

HOOT_FACTORY_REGISTER(ChangesetReplacementCreator, ChangesetReplacementCreator7)

ChangesetReplacementCreator7::ChangesetReplacementCreator7() :
ChangesetReplacementCreator1()
{
  _currentChangeDerivationPassIsLinear = true;
  _boundsInterpretation = BoundsInterpretation::Lenient;
}

void ChangesetReplacementCreator7::setGeometryFilters(const QStringList& filterClassNames)
{
  LOG_VART(filterClassNames);
  if (!filterClassNames.isEmpty())
  {
    _geometryFiltersSpecified = true;
    //_geometryTypeFilters.clear();
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
  // itself internally when no geometry filters are specified
  //LOG_VART(_geometryTypeFilters.size());
  //if (_geometryTypeFilters.isEmpty())
  if (!_geometryTypeFilter)
  {
    _geometryFiltersSpecified = false;
    //_geometryTypeFilters = _getDefaultGeometryFilters();
    _linearFilterClassNames =
      ConflatableElementCriterion::getCriterionClassNamesByGeometryType(
        GeometryTypeCriterion::GeometryType::Line);
  }
  _linearFilterClassNames.removeAll(QString::fromStdString(LinearCriterion::className()));

  LOG_VARD(_geometryTypeFilters.size());
  LOG_VART(_linearFilterClassNames);
}

void ChangesetReplacementCreator7::create(
  const QString& input1, const QString& input2, const geos::geom::Envelope& bounds,
  const QString& output)
{
  QElapsedTimer timer;
  timer.start();

  // INPUT VALIDATION AND SETUP

  _input1 = input1;
  _input1Map.reset();
  _input2 = input2;
  _input2Map.reset();
  _output = output;
  //_replacementBounds = bounds;
  // TODO: It makes more sense to store the bounds and then just convert it to a string as needed.
  // The default string stores six decimal places, which should be fine for a bounds. Strangely,
  // when I store the bounds or try to increase the precision of the bounds string, I'm getting a
  // lot of test output issues...needs to be looked into.
  _replacementBounds = GeometryUtils::envelopeToConfigString(bounds);
  _validateInputs();
  _setGlobalOpts();
  _printJobDescription();

  // DIFF CALCULATION

  LOG_INFO("******************************************");
  LOG_STATUS("Generating diff maps for changeset derivation with ID: " << _changesetId << "...");

  OsmMapPtr refMap;
  // This is a bit of a misnomer after recent changes, as this map may have only been cleaned by
  // this point and not actually conflated with anything.
  OsmMapPtr conflatedMap;

  LOG_VARD(toString());

  // DATA LOAD AND INITIAL PREP

  // load the data to replace
  refMap =
    _loadInputMap(
      "ref", _input1, true, Status::Unknown1, _boundsOpts.loadRefKeepEntireCrossingBounds,
      _boundsOpts.loadRefKeepOnlyInsideBounds,
      _boundsOpts.loadRefKeepImmediateConnectedWaysOutsideBounds, true, _input1Map);
  MemoryUsageChecker::getInstance().check();

  // Drop all C&R specific metadata tags which should not exist yet, just in case they got in the
  // input somehow.
  _removeMetadataTags(refMap);

  const bool markMissing =
    ConfigOptions().getChangesetReplacementMarkElementsWithMissingChildren();
  if (markMissing)
  {
    // Find any elements with missing children and tag them with a custom tag, as its possible we'll
    // break them during this process. There's really nothing that can be done about that, since we
    // don't have access to the missing children elements. Any elements with missing children may
    // require manual cleanup after the resulting changeset is applied.
    _markElementsWithMissingChildren(refMap);
  }

  // Keep a mapping of the original ref element ids to versions, as we'll need the original
  // versions later.
  const QMap<ElementId, long> refIdToVersionMappings = _getIdToVersionMappings(refMap);

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

  // load the data that we're replacing with
  OsmMapPtr secMap =
    _loadInputMap(
      "sec", _input2, false, Status::Unknown2, _boundsOpts.loadSecKeepEntireCrossingBounds,
      _boundsOpts.loadSecKeepOnlyInsideBounds, false, true, _input2Map);
  MemoryUsageChecker::getInstance().check();

  _removeMetadataTags(secMap);

  if (markMissing)
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

  const int refMapSize = refMap->size();
  // If the secondary dataset is empty here and the ref dataset isn't, then we'll end up with a
  // changeset with only delete statements if the option to allow deleting reference features is
  // enabled.
  const int secMapSize = secMap->size();
  LOG_VARD(refMapSize);
  LOG_VARD(secMapSize);

  //const QString geometryTypeStr = GeometryTypeCriterion::typeToString(geometryType);
  bool bothMapsEmpty = false;
  if (refMapSize == 0 && secMapSize == 0)
  {
    LOG_STATUS("Both maps empty, so skipping data removal...");
    bothMapsEmpty = true;
  }

  // CUT

  // cut the shape of the secondary data out of the reference data
  OsmMapPtr cookieCutRefMap =
    _getCookieCutMap(refMap, secMap, GeometryTypeCriterion::GeometryType::Unknown);
  const int cookieCutSize = cookieCutRefMap->size();
  LOG_VARD(cookieCutSize);
  const int dataRemoved = refMapSize - cookieCutSize;
  LOG_VARD(dataRemoved);

  if (!bothMapsEmpty)
  {
    // sec map size may have changed after call to _getCookieCutMap
    LOG_STATUS(
      "Replacing " << StringUtils::formatLargeNumber(dataRemoved) << " feature(s) with " <<
      StringUtils::formatLargeNumber(secMap->size()) << " feature(s)...");
  }

  // At one point it was necessary to re-number the relations in the sec map, as they could have ID
  // overlap with those in the cookie cut ref map at this point. It seemed that this was due to the
  // fact that relations in the two maps had been added independently of each other during cropping.
  // However, after some refactoring this doesn't seem to be the case anymore. If we run into this
  // situation again, we can go back in the history to resurrect the use of the ElementIdRemapper
  // for relations here, which has since been removed from the codebase.

  // Combine the cookie cut ref map back with the secondary map, so we can conflate the two
  // together if needed.
  MapUtils::combineMaps(cookieCutRefMap, secMap, false);
  OsmMapWriterFactory::writeDebugMap(cookieCutRefMap, _changesetId + "-combined-before-conflation");
  secMap.reset();
  LOG_VARD(cookieCutRefMap->size());

  // CONFLATE / CLEAN

  // conflate the cookie cut ref map with the sec map if conflation is enabled

  // TODO: rename var since this map isn't necessary conflated; also rename everything in terms of
  // "toReplace" and "replacement"
  conflatedMap = cookieCutRefMap;
  if (secMapSize > 0)
  {
    _clean(conflatedMap);
    conflatedMap->setName("cleaned");
  }

  // SNAP

  // TODO: maybe only snapping near the replacement bounds may help prevent some bad snaps in the
  // output

  // Snap secondary features back to reference features if dealing with linear features where
  // ref features may have been cut along the bounds. We're being lenient here by snapping
  // secondary to reference *and* allowing conflated data to be snapped to either dataset. We only
  // want to snap ways of like types together, so we'll loop through each applicable linear type
  // and snap them separately.

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
      conflatedMap, snapWayStatuses, snapToWayStatuses, _linearFilterClassNames.at(i), false,
      _changesetId + "-conflated-snapped-sec-to-ref-1");
  }

  // After snapping, perform joining to prevent unnecessary create/delete statements for the ref
  // data in the resulting changeset and generate modify statements instead.
  ReplacementSnappedWayJoiner wayJoiner(refIdToVersionMappings);
  wayJoiner.join(conflatedMap);
  LOG_VART(MapProjector::toWkt(conflatedMap->getProjection()));

  // PRE-CHANGESET DERIVATION DATA PREP

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
    conflatedMap, _boundsOpts.changesetSecKeepEntireCrossingBounds,
    _boundsOpts.changesetSecKeepOnlyInsideBounds, _changesetId + "-sec-cropped-for-changeset");

  // The non-strict bounds interpretation way replacement workflow benefits from a second
  // set of snapping runs right before changeset derivation due to there being ways connected to
  // replacement ways that fall completely outside of the replacement bounds. However, joining
  // after this snapping caused changeset errors with some datasets and hasn't seem to be needed
  // so far...so skipping it. Note that we're being as lenient as possible with the snapping
  // here, allowing basically anything to join to anything else, which *could* end up causing
  // problems...we'll go with it for now.

  snapWayStatuses.clear();
  snapWayStatuses.append("Input2");
  snapWayStatuses.append("Conflated");
  snapWayStatuses.append("Input1");
  snapToWayStatuses.clear();
  snapToWayStatuses.append("Input1");
  snapToWayStatuses.append("Conflated");
  snapToWayStatuses.append("Input2");
  LOG_VARD(_linearFilterClassNames);

  // Snap anything that can be snapped per feature type.
  LOG_INFO("Snapping unconnected ways to each other in replacement map...");
  for (int i = 0; i < _linearFilterClassNames.size(); i++)
  {
    _snapUnconnectedWays(
      conflatedMap, snapWayStatuses, snapToWayStatuses, _linearFilterClassNames.at(i), false,
      _changesetId + "-conflated-snapped-sec-to-ref-2");
  }

  // combine the conflated map with the immediately connected out of bounds ways
  MapUtils::combineMaps(conflatedMap, immediatelyConnectedOutOfBoundsWays, true);
  OsmMapWriterFactory::writeDebugMap(
    conflatedMap, _changesetId + "-conflated-connected-combined");

  // Snap the connected ways to other ways in the conflated map. Mark the ways that were
  // snapped, as we'll need that info in the next step.
  LOG_INFO("Snapping unconnected ways to each other in replacement map...");
  for (int i = 0; i < _linearFilterClassNames.size(); i++)
  {
    _snapUnconnectedWays(
      conflatedMap, snapWayStatuses, snapToWayStatuses, _linearFilterClassNames.at(i),
      true, _changesetId + "-conflated-snapped-immediately-connected-out-of-bounds");
  }

  // remove any ways that weren't snapped
  _removeUnsnappedImmediatelyConnectedOutOfBoundsWays(conflatedMap);

  // Copy the connected ways back into the ref map as well, so the changeset will derive
  // properly.
  MapUtils::combineMaps(refMap, immediatelyConnectedOutOfBoundsWays, true);
  OsmMapWriterFactory::writeDebugMap(
    conflatedMap, _changesetId + "-ref-connected-combined");

  immediatelyConnectedOutOfBoundsWays.reset();

  if (!ConfigOptions().getChangesetReplacementAllowDeletingReferenceFeaturesOutsideBounds())
  {
    // If we're not allowing the changeset deriver to generate delete statements for reference
    // features outside of the bounds, we need to mark all corresponding ref ways with a custom
    // tag that will cause the deriver to skip deleting them.
    _excludeFeaturesFromChangesetDeletion(refMap);
  }

  // clean up any mistakes introduced
  _cleanup(refMap);
  _cleanup(conflatedMap);

  LOG_VART(refMap->getElementCount());
  LOG_VART(conflatedMap->getElementCount());

  if (!refMap)
  {
    LOG_DEBUG("ref map null");
  }
  else
  {
    LOG_VARD(refMap->size());
  }
  if (!conflatedMap)
  {
    LOG_DEBUG("conflated map null");
  }
  else
  {
    LOG_VARD(conflatedMap->size());
  }
  if (refMap && conflatedMap)
  {
    LOG_DEBUG(
      "Adding ref map of size: " << StringUtils::formatLargeNumber(refMap->size()) <<
      " and conflated map of size: " << StringUtils::formatLargeNumber(conflatedMap->size()) <<
      " to changeset derivation queue...");;
  }

  if (!refMap && !conflatedMap)
  {
    LOG_WARN("No features remain after filtering, so no changeset will be generated.");
    return;
  }

  // CLEANUP

  // Synchronize IDs between the two maps in order to cut down on unnecessary changeset
  // create/delete statements. This must be done with the ref/sec maps separated to avoid ID
  // conflicts.
  _synchronizeIds(refMap, conflatedMap);

  // CHANGESET GENERATION

  LOG_STATUS("Generating changeset...");

  // Derive a changeset between the ref and conflated maps that replaces ref features with
  // secondary features within the bounds and write it out.
  _changesetCreator->setIncludeReviews(
    /*_conflationEnabled && ConfigOptions().getChangesetReplacementPassConflateReviews()*/false);
  // We have some instances where modify and delete changes are being generated for the same
  // element, which causes error during changeset application. Eventually, we should eliminate their
  // causes, but for now we'll activate changeset cleaning to get rid of the delete statements.
  // Unfortunately, this will make the changeset writing memory bound.
  _changesetCreator->setClean(true);
  _changesetCreator->create(refMap, conflatedMap, _output);
  _numChanges = _changesetCreator->getNumTotalChanges();

  LOG_STATUS(
    "Derived replacement changeset: ..." << _output.right(_maxFilePrintLength) << " with " <<
    StringUtils::formatLargeNumber(_numChanges) << " changes in " <<
    StringUtils::millisecondsToDhms(timer.elapsed()) << " total.");
}

void ChangesetReplacementCreator7::_setGlobalOpts()
{
  conf().set(ConfigOptions::getChangesetXmlWriterAddTimestampKey(), false);
  conf().set(ConfigOptions::getReaderAddSourceDatetimeKey(), false);
  conf().set(ConfigOptions::getWriterIncludeCircularErrorTagsKey(), false);
  conf().set(ConfigOptions::getConvertBoundingBoxKey(), _replacementBounds);

  // For this being enabled to have any effect,
  // convert.bounding.box.keep.immediately.connected.ways.outside.bounds must be enabled as well.
  conf().set(ConfigOptions::getConvertBoundingBoxTagImmediatelyConnectedOutOfBoundsWaysKey(), true);

  // will have to see if setting this to false causes problems in the future...
  conf().set(ConfigOptions::getConvertRequireAreaForPolygonKey(), false);

  // This needs to be lowered a bit to make feature de-duping and/or ID synchronization work. At
  // five decimal places, we're topping out at a little over 1m of difference. If this ends up
  // causing problems, then may need to go back to the original setting = 7.
  conf().set(ConfigOptions::getNodeComparisonCoordinateSensitivityKey(), 5);

  // We're not going to remove missing elements, as we want to have as minimal of an impact on
  // the resulting changeset as possible.
  ConfigUtils::removeListOpEntry(
    ConfigOptions::getConflatePreOpsKey(),
    QString::fromStdString(RemoveMissingElementsVisitor::className()));
  ConfigUtils::removeListOpEntry(
    ConfigOptions::getConflatePostOpsKey(),
    QString::fromStdString(RemoveMissingElementsVisitor::className()));
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

  // Came across a very odd bug in #4101, where if RemoveInvalidMultilineStringMembersVisitor ran
  // as part of the pre-conflate map cleaning during replacement with conflation enabled, the match
  // conflict resolution would slow down to a crawl. When it was removed from the cleaning ops, the
  // conflate operation ran very quickly. So as a not so great workaround (aka hack), removing that
  // pre-op here when running conflation. It still will run post conflate, though. This change had
  // a very minor affect on changeset replacement test output where one test got slightly better
  // output after the change and another slightly worse. See more details in #4101, which is closed,
  // but if we can figure out what's going on at some point maybe this situation can be handled
  // properly.
//  if (_conflationEnabled)
//  {
//    ConfigUtils::removeListOpEntry(
//      ConfigOptions::getMapCleanerTransformsKey(),
//      QString::fromStdString(RemoveInvalidMultilineStringMembersVisitor::className()));
//  }

  // These don't change between scenarios (or at least we haven't needed to change them yet).
  _boundsOpts.loadRefKeepOnlyInsideBounds = false;
  _boundsOpts.cookieCutKeepOnlyInsideBounds = false;
  _boundsOpts.changesetRefKeepOnlyInsideBounds = false;

  // turn on for testing only
  //conf().set(ConfigOptions::getDebugMapsWriteKey(), true);

  _boundsOpts.loadRefKeepEntireCrossingBounds = true;
  _boundsOpts.loadSecKeepOnlyInsideBounds = false;
  _boundsOpts.cookieCutKeepEntireCrossingBounds = false;
  _boundsOpts.changesetRefKeepEntireCrossingBounds = true;
  _boundsOpts.changesetSecKeepEntireCrossingBounds = true;
  _boundsOpts.changesetSecKeepOnlyInsideBounds = false;
  _boundsOpts.inBoundsStrict = false;

  //if (_boundsInterpretation == BoundsInterpretation::Lenient)
  //{
    _boundsOpts.loadRefKeepImmediateConnectedWaysOutsideBounds = true;
    _boundsOpts.loadSecKeepEntireCrossingBounds = true;
    _boundsOpts.changesetAllowDeletingRefOutsideBounds = true;
  //}
//  else
//  {
//    _boundsOpts.loadRefKeepImmediateConnectedWaysOutsideBounds = false;
//    _boundsOpts.loadSecKeepEntireCrossingBounds = false;
//    _boundsOpts.changesetAllowDeletingRefOutsideBounds = false;
//  }

  // Conflate way joining needs to happen later in the post ops for strict linear replacements.
  // Changing the default ordering of the post ops to accommodate this had detrimental effects
  // on other conflation. The best location seems to be at the end just before tag truncation.
  // Would like to get rid of this...isn't a foolproof fix by any means if the conflate post
  // ops end up getting re-ordered for some reason.

//  LOG_VART(conf().getList(ConfigOptions::getConflatePostOpsKey()));
//  QStringList conflatePostOps = conf().getList(ConfigOptions::getConflatePostOpsKey());
//  conflatePostOps.removeAll(QString::fromStdString(WayJoinerOp::className()));
//  const int indexOfTagTruncater =
//    conflatePostOps.indexOf(QString::fromStdString(ApiTagTruncateVisitor::className()));
//  conflatePostOps.insert(
//    indexOfTagTruncater - 1, QString::fromStdString(WayJoinerOp::className()));
//  conf().set(ConfigOptions::getConflatePostOpsKey(), conflatePostOps);
//  LOG_VARD(conf().getList(ConfigOptions::getConflatePostOpsKey()));

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

}
