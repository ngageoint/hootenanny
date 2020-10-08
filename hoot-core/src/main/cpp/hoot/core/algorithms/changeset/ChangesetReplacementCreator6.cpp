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
#include "ChangesetReplacementCreator6.h"

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

HOOT_FACTORY_REGISTER(ChangesetReplacementCreator, ChangesetReplacementCreator6)

ChangesetReplacementCreator6::ChangesetReplacementCreator6() :
_changesetId("1"),
_maxFilePrintLength(ConfigOptions().getProgressVarPrintLengthMax() * 2),
_numChanges(0)
{
}

void ChangesetReplacementCreator6::setChangesetOptions(
  const bool printStats, const QString& statsOutputFile, const QString osmApiDbUrl)
{
  _changesetCreator.reset(new ChangesetCreator(printStats, statsOutputFile, osmApiDbUrl));
}

void ChangesetReplacementCreator6::_validateInputs()
{
  // Fail if the reader that supports either input doesn't implement Boundable.
  std::shared_ptr<Boundable> boundable =
    std::dynamic_pointer_cast<Boundable>(OsmMapReaderFactory::createReader(_input1));
  if (!boundable)
  {
    throw IllegalArgumentException(
      "Reader for " + _input1 + " must implement Boundable for replacement changeset derivation.");
    }
  boundable = std::dynamic_pointer_cast<Boundable>(OsmMapReaderFactory::createReader(_input2));
  if (!boundable)
  {
    throw IllegalArgumentException(
      "Reader for " + _input2 + " must implement Boundable for replacement changeset derivation.");
  }

  // Fail for GeoJSON - GeoJSON coming from Overpass does not have way nodes, so their versions
  // are lost when new way nodes are added to existing ways. For that reason, we can't support it
  // (or at least not sure how to yet).
  OsmGeoJsonReader geoJsonReader;
  if (geoJsonReader.isSupported(_input1) || geoJsonReader.isSupported(_input2))
  {
    throw IllegalArgumentException(
      "GeoJSON inputs are not supported by replacement changeset derivation.");
  }
  QFile outputFile(_output);
  if (outputFile.exists())
  {
    if (!outputFile.remove())
    {
      throw HootException("Unable to remove changeset output file: " + _output);
    }
  }

  if (ConfigOptions().getConvertOps().size() > 0)
  {
    throw IllegalArgumentException(
      "Replacement changeset derivation does not support convert operations.");
  }
}

void ChangesetReplacementCreator6::_setGlobalOpts()
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

  conf().set(
    ConfigOptions::getChangesetReplacementAllowDeletingReferenceFeaturesOutsideBoundsKey(),
    true);

  // turn on for testing only
  //conf().set(ConfigOptions::getDebugMapsWriteKey(), true);
}

void ChangesetReplacementCreator6::_printJobDescription() const
{
  QString cropDbInputOnReadStr = "is ";
  if (!ConfigOptions().getApidbReaderReadFullThenCropOnBounded())
  {
    cropDbInputOnReadStr += "not ";
  }
  cropDbInputOnReadStr += "enabled";

  QString str;
  str += "Deriving replacement output changeset:";
  str += "\nBeing replaced: ..." + _input1.right(_maxFilePrintLength);
  str += "\nReplacing with ..." + _input2.right(_maxFilePrintLength);
  str += "\nAt Bounds: " + _replacementBounds;
  str += "\nOutput Changeset: ..." + _output.right(_maxFilePrintLength);
  LOG_STATUS(str);

  str = "";
  str += "\nCropping database inputs after read: " + cropDbInputOnReadStr;
  LOG_INFO(str);
}

void ChangesetReplacementCreator6::createSimple(
  const QString& input1, const QString& input2, const geos::geom::Envelope& bounds,
  const QString& output)
{
  QElapsedTimer timer;
  timer.start();

  // INPUT VALIDATION AND SETUP

  _input1 = input1;
  _input2 = input2;
  _output = output;
  // TODO: It makes more sense to store the bounds and then just convert it to a string as needed.
  // The default string stores six decimal places, which should be fine for a bounds. Strangely,
  // when I store the bounds or try to increase the precision of the bounds string, I'm getting a
  // lot of test output issues...needs to be looked into.
  _replacementBounds = GeometryUtils::envelopeToConfigString(bounds);

  _validateInputs();
  _setGlobalOpts();
  _printJobDescription();

  // DATA LOAD AND INITIAL PREP

  OsmMapPtr refMap =
    _loadInputMap(
      "ref",
      _input1,
      true,  // useFileIds
      Status::Unknown1,
      true, // keepEntireFeaturesCrossingBounds
      false,  // keepOnlyFeaturesInsideBounds
      true, // keepImmediatelyConnectedWaysOutsideBounds
      true);
  OsmMapPtr secMap =
    _loadInputMap(
      "sec",
      _input2,
      false, // useFileIds
      Status::Unknown2,
      true, // keepEntireFeaturesCrossingBounds
      false,  // keepOnlyFeaturesInsideBounds
      true, // keepImmediatelyConnectedWaysOutsideBounds
      false);

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

  OsmMapPtr cutMap(refMap);
  cutMap->setName("cut");
  _cut(
    cutMap,
    true, // setKeepEntireFeaturesCrossingBounds
    false); // setKeepOnlyFeaturesInsideBounds

  // CLEANUP



  // CHANGESET GENERATION

  LOG_STATUS("Generating changeset...");

  _changesetCreator->setClean(true);
  _changesetCreator->create(cutMap, secMap, _output);
  _numChanges = _changesetCreator->getNumTotalChanges();

  LOG_STATUS(
    "Derived replacement changeset: ..." << _output.right(_maxFilePrintLength) << " with " <<
    StringUtils::formatLargeNumber(_numChanges) << " changes in " <<
    StringUtils::millisecondsToDhms(timer.elapsed()) << " total.");
}

void ChangesetReplacementCreator6::create(
  const QString& input1, const QString& input2, const geos::geom::Envelope& bounds,
  const QString& output)
{
  QElapsedTimer timer;
  timer.start();

  // INPUT VALIDATION AND SETUP

  _input1 = input1;
  _input2 = input2;
  _output = output;
  // TODO: It makes more sense to store the bounds and then just convert it to a string as needed.
  // The default string stores six decimal places, which should be fine for a bounds. Strangely,
  // when I store the bounds or try to increase the precision of the bounds string, I'm getting a
  // lot of test output issues...needs to be looked into.
  _replacementBounds = GeometryUtils::envelopeToConfigString(bounds);

  _validateInputs();
  _setGlobalOpts();
  _printJobDescription();

  // DATA LOAD AND INITIAL PREP

  OsmMapPtr refMap =
    _loadInputMap(
      "ref",
      _input1,
      true,  // useFileIds
      Status::Unknown1,
      true, // keepEntireFeaturesCrossingBounds
      false,  // keepOnlyFeaturesInsideBounds
      true, // keepImmediatelyConnectedWaysOutsideBounds
      true); // warnOnZeroVersions
  OsmMapPtr secMap =
    _loadInputMap(
      "sec",
      _input2,
      false, // useFileIds
      Status::Unknown2,
      true, // keepEntireFeaturesCrossingBounds
      false,  // keepOnlyFeaturesInsideBounds
      false, // keepImmediatelyConnectedWaysOutsideBounds
      false); // warnOnZeroVersions

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

  // Drop all C&R specific metadata tags which should not exist yet, just in case they got in the
  // input somehow.
//  _removeMetadataTags(refMap);
//  _removeMetadataTags(secMap);
//  const bool markMissing =
//    ConfigOptions().getChangesetReplacementMarkElementsWithMissingChildren();
//  if (markMissing)
//  {
//    // Find any elements with missing children and tag them with a custom tag, as its possible we'll
//    // break them during this process. There's really nothing that can be done about that, since we
//    // don't have access to the missing children elements. Any elements with missing children may
//    // require manual cleanup after the resulting changeset is applied.
//    _markElementsWithMissingChildren(refMap);
//    _markElementsWithMissingChildren(secMap);
//  }

  // CUT

  OsmMapPtr cutMap(refMap);
  cutMap->setName("cut");
  _cut(
    cutMap,
    secMap,
    true, // keepEntireFeaturesCrossingBounds // this to false result in a delete version=0 error
    false); // keepOnlyFeaturesInsideBounds
  MapProjector::projectToWgs84(cutMap);
  MapProjector::projectToWgs84(secMap);

  // CLEANUP

  // clean up any mistakes introduced
//  _cleanup(refMap);
//  _cleanup(secMap);
//  _cleanup(cutMap);

//  _dedupeMaps(refMap, secMap);
//  _dedupeMaps(cutMap, secMap);

  // Synchronize IDs between the two maps in order to cut down on unnecessary changeset
  // create/delete statements. This must be done with the ref/sec maps separated to avoid ID
  // conflicts.
  //_synchronizeIds(refMap, secMap);

  // CHANGESET GENERATION

  LOG_STATUS("Generating changeset...");

  _changesetCreator->setClean(true);
  _changesetCreator->create(cutMap, secMap, _output);
  _numChanges = _changesetCreator->getNumTotalChanges();

  LOG_STATUS(
    "Derived replacement changeset: ..." << _output.right(_maxFilePrintLength) << " with " <<
    StringUtils::formatLargeNumber(_numChanges) << " changes in " <<
    StringUtils::millisecondsToDhms(timer.elapsed()) << " total.");
}

OsmMapPtr ChangesetReplacementCreator6::_loadInputMap(
  const QString& mapName, const QString& inputUrl, const bool useFileIds, const Status& status,
  const bool keepEntireFeaturesCrossingBounds, const bool keepOnlyFeaturesInsideBounds,
  const bool keepImmediatelyConnectedWaysOutsideBounds, const bool warnOnZeroVersions)
{
  conf().set(
    ConfigOptions::getConvertBoundingBoxKeepEntireFeaturesCrossingBoundsKey(),
    keepEntireFeaturesCrossingBounds);
  conf().set(
    ConfigOptions::getConvertBoundingBoxKeepOnlyFeaturesInsideBoundsKey(),
   keepOnlyFeaturesInsideBounds);
  conf().set(
    ConfigOptions::getConvertBoundingBoxKeepImmediatelyConnectedWaysOutsideBoundsKey(),
    keepImmediatelyConnectedWaysOutsideBounds);

  if (warnOnZeroVersions)
  {
    // We want to alert the user to the fact their ref versions *could* be being populated
    // incorrectly to avoid difficulties during changeset application at the end. Its likely if they
    // are incorrect at this point the changeset derivation will fail at the end anyway, but let's
    // warn now to give the chance to back out of the replacement earlier.
    conf().set(ConfigOptions::getReaderWarnOnZeroVersionElementKey(), true);
  }

  OsmMapPtr map(new OsmMap());
  map->setName(mapName);
  LOG_STATUS(
    "Loading map from: ..." << inputUrl.right(_maxFilePrintLength) << "...");
  IoUtils::loadMap(map, inputUrl, useFileIds, status);

  if (warnOnZeroVersions)
  {
    // Restore default setting value.
    conf().set(ConfigOptions::getReaderWarnOnZeroVersionElementKey(), false);
  }

  LOG_VART(MapProjector::toWkt(map->getProjection()));
  OsmMapWriterFactory::writeDebugMap(
    map, _changesetId + "-" + map->getName() + "-after-cropped-load");

  return map;
}

void ChangesetReplacementCreator6::_removeMetadataTags(const OsmMapPtr& map)
{
  QStringList changesetReplacementMetadataTags;
  changesetReplacementMetadataTags.append(MetadataTags::HootChangeExcludeDelete());
  changesetReplacementMetadataTags.append(MetadataTags::HootSnapped());
  changesetReplacementMetadataTags.append(MetadataTags::HootMissingChild());
  RemoveTagsVisitor tagRemover(changesetReplacementMetadataTags);
  LOG_INFO(tagRemover.getInitStatusMessage());
  map->visitRw(tagRemover);
  LOG_DEBUG(tagRemover.getCompletedStatusMessage());
}

void ChangesetReplacementCreator6::_markElementsWithMissingChildren(OsmMapPtr& map)
{
  ReportMissingElementsVisitor elementMarker;
  // Originally, this was going to add reviews for this rather than tagging elements, but there was
  // an ID provenance problem when using reviews.
  elementMarker.setMarkRelationsForReview(false);
  elementMarker.setMarkWaysForReview(false);
  elementMarker.setRelationKvp(MetadataTags::HootMissingChild() + "=yes");
  elementMarker.setWayKvp(MetadataTags::HootMissingChild() + "=yes");
  LOG_INFO("Marking elements with missing child elements...");
  map->visitRelationsRw(elementMarker);
  LOG_DEBUG(
    "Marked " << elementMarker.getNumWaysTagged() << " ways and " <<
    elementMarker.getNumRelationsTagged() << " relations with missing child elements.");

  OsmMapWriterFactory::writeDebugMap(
    map, _changesetId + "-" + map->getName() + "-after-missing-marked");
}

void ChangesetReplacementCreator6::_cut(
  OsmMapPtr& map, const bool keepEntireFeaturesCrossingBounds,
  const bool keepOnlyFeaturesInsideBounds)
{
  MapCropper cropper(GeometryUtils::envelopeFromConfigString(_replacementBounds));
  cropper.setRemoveSuperflousFeatures(false);
  cropper.setKeepEntireFeaturesCrossingBounds(keepEntireFeaturesCrossingBounds);
  cropper.setKeepOnlyFeaturesInsideBounds(keepOnlyFeaturesInsideBounds);
  cropper.setInvert(true);
  // We're not going to remove missing elements, as we want to have as minimal of an impact on
  // the resulting changeset as possible.
  cropper.setRemoveMissingElements(false);
  LOG_STATUS(cropper.getInitStatusMessage());
  cropper.apply(map);
  LOG_INFO(cropper.getCompletedStatusMessage());
  OsmMapWriterFactory::writeDebugMap(map, _changesetId + "-cut");
}

void ChangesetReplacementCreator6::_cut(
  OsmMapPtr mapToReplace, OsmMapPtr replacementMap,
  const bool keepEntireFeaturesCrossingBounds, const bool keepOnlyFeaturesInsideBounds)
{
  MapProjector::projectToWgs84(mapToReplace);
  MapProjector::projectToWgs84(replacementMap);

  OsmMapPtr cutterShapeOutlineMap;
  AlphaShapeGenerator alphaShapeGenerator(
    ConfigOptions().getCookieCutterAlpha(), ConfigOptions().getCookieCutterAlphaShapeBuffer());
  // Covering stragglers here can be very slow for linear features. So far, have only needed it for
  // point passes, which don't seem to be slow.
  alphaShapeGenerator.setManuallyCoverSmallPointClusters(true);
  cutterShapeOutlineMap = alphaShapeGenerator.generateMap(replacementMap);

  // not exactly sure yet why this projection needs to be done
  MapProjector::projectToWgs84(cutterShapeOutlineMap);
  LOG_VART(MapProjector::toWkt(cutterShapeOutlineMap->getProjection()));
  OsmMapWriterFactory::writeDebugMap(cutterShapeOutlineMap, _changesetId + "-cutter-shape");

  // Cookie cut the shape of the cutter shape map out of the cropped ref map.
  LOG_STATUS(
    "Cutting cutter shape out of: " << mapToReplace->getName() << " with size: " <<
    StringUtils::formatLargeNumber(mapToReplace->size()) << "...");

  // We're not going to remove missing elements, as we want to have as minimal of an impact on
  // the resulting changeset as possible.
  CookieCutter(
    false, 0.0, keepEntireFeaturesCrossingBounds,
    keepOnlyFeaturesInsideBounds, false)
    .cut(cutterShapeOutlineMap, mapToReplace);
  // not exactly sure yet why this projection needs to be done
  MapProjector::projectToWgs84(mapToReplace);
  LOG_VARD(mapToReplace->size());
  LOG_VART(MapProjector::toWkt(mapToReplace->getProjection()));
  OsmMapWriterFactory::writeDebugMap(mapToReplace, _changesetId + "-cut");
}

void ChangesetReplacementCreator6::_synchronizeIds(
  OsmMapPtr mapBeingReplaced, OsmMapPtr replacementMap)
{
  // When replacing data, we always load the replacement data without its original IDs in case there
  // are overlapping IDs in the reference data. If you were only replacing unmodified data from one
  // source with updated data from another source with the same IDs (e.g. replacing newer OSM with
  // older OSM), this separation wouldn't be necessary, but we're not guaranteed that will be the
  // only scenario encountered. The downside to loading up a separate set of unique IDs for the
  // secondary data is that identical elements in the secondary can end up unnecessarily replacing
  // elements in the reference. This gets mitigated here where we find all identical elements
  // between the data being replaced and the replacement data and overwrite IDs in the replacement
  // data with the IDs of matching elements from the data being replaced to prevent unnecessary
  // changeset modifications from being generated. Its possible we could do this earlier in the
  // replacement process, however that has proven difficult to accomplish so far.

  assert(mapBeingReplaced.size() == replacementMaps.size());
  ChangesetReplacementElementIdSynchronizer idSync;
  OsmMapWriterFactory::writeDebugMap(
    mapBeingReplaced, _changesetId + "-" + mapBeingReplaced->getName() +
    "-source-before-id-sync");
  OsmMapWriterFactory::writeDebugMap(
    replacementMap, _changesetId + "-" + replacementMap->getName() + "-target-before-id-sync");

  idSync.synchronize(mapBeingReplaced, replacementMap);

  // get rid of straggling nodes
  // TODO: should we run _cleanup here instead and move it from its earlier call?
  SuperfluousNodeRemover orphanedNodeRemover;
  orphanedNodeRemover.apply(replacementMap);
  LOG_DEBUG(orphanedNodeRemover.getCompletedStatusMessage());
  OsmMapWriterFactory::writeDebugMap(
    replacementMap, _changesetId + "-" + replacementMap->getName() + "-after-id-sync");
}

void ChangesetReplacementCreator6::_cleanup(OsmMapPtr& map)
{
  LOG_INFO("Cleaning up changeset derivation input " << map->getName() << "...");

  // Due to mixed geometry type relations explained in _getDefaultGeometryFilters, we may have
  // introduced some duplicate relation members by this point.
  RemoveDuplicateRelationMembersVisitor dupeMembersRemover;
  map->visitRw(dupeMembersRemover);
  LOG_DEBUG(dupeMembersRemover.getCompletedStatusMessage());

  // get rid of straggling nodes
  SuperfluousNodeRemover orphanedNodeRemover;
  orphanedNodeRemover.apply(map);
  LOG_DEBUG(orphanedNodeRemover.getCompletedStatusMessage());

  // This will remove any relations that were already empty or became empty after we removed
  // duplicated members.
  RemoveEmptyRelationsOp emptyRelationRemover;
  emptyRelationRemover.apply(map);
  LOG_DEBUG(emptyRelationRemover.getCompletedStatusMessage());

  // get out of orthographic
  MapProjector::projectToWgs84(map);
}

void ChangesetReplacementCreator6::_intraDedupeMap(OsmMapPtr& map)
{
  ElementDeduplicator deduper;
  deduper.setDedupeIntraMap(true);
  std::shared_ptr<PointCriterion> pointCrit(new PointCriterion());
  pointCrit->setOsmMap(map.get());
  deduper.setNodeCriterion(pointCrit);
  deduper.setFavorMoreConnectedWays(true);
  OsmMapWriterFactory::writeDebugMap(
    map, _changesetId + "-" + map->getName() + "-before-deduping");
  deduper.dedupe(map);
  OsmMapWriterFactory::writeDebugMap(
    map, _changesetId + "-" + map->getName() + "-after-deduping");
}

void ChangesetReplacementCreator6::_dedupeMaps(OsmMapPtr& refMap, OsmMapPtr& secMap)
{
  ElementDeduplicator deduper;
  deduper.setDedupeIntraMap(true);
  std::shared_ptr<PointCriterion> pointCrit(new PointCriterion());
  pointCrit->setOsmMap(refMap.get());
  deduper.setNodeCriterion(pointCrit);
  deduper.setFavorMoreConnectedWays(true);
  OsmMapWriterFactory::writeDebugMap(
    refMap, _changesetId + "-" + refMap->getName() + "-before-deduping");
  OsmMapWriterFactory::writeDebugMap(
    secMap, _changesetId + "-" + secMap->getName() + "-before-deduping");
  deduper.dedupe(refMap, secMap);
  OsmMapWriterFactory::writeDebugMap(
    refMap, _changesetId + "-" + refMap->getName() + "-after-deduping");
  OsmMapWriterFactory::writeDebugMap(
    secMap, _changesetId + "-" + secMap->getName() + "-after-deduping");
}

}
