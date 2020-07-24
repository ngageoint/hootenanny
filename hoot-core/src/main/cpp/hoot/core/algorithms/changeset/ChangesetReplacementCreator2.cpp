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
#include "ChangesetReplacementCreator2.h"

// Hoot
#include <hoot/core/algorithms/ReplacementSnappedWayJoiner.h>
#include <hoot/core/algorithms/WayJoinerAdvanced.h>
#include <hoot/core/algorithms/WayJoinerBasic.h>

#include <hoot/core/algorithms/alpha-shape/AlphaShapeGenerator.h>

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
#include <hoot/core/criterion/RoundaboutCriterion.h>
#include <hoot/core/criterion/TagCriterion.h>
#include <hoot/core/criterion/TagKeyCriterion.h>
#include <hoot/core/criterion/WayNodeCriterion.h>

#include <hoot/core/elements/ElementDeduplicator.h>
#include <hoot/core/elements/MapUtils.h>

#include <hoot/core/index/OsmMapIndex.h>

#include <hoot/core/io/IoUtils.h>
#include <hoot/core/io/OsmGeoJsonReader.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>

#include <hoot/core/ops/ElementIdToVersionMapper.h>
#include <hoot/core/ops/MapCleaner.h>
#include <hoot/core/ops/MapCropper.h>
#include <hoot/core/ops/NamedOp.h>
#include <hoot/core/ops/PointsToPolysConverter.h>
#include <hoot/core/ops/SuperfluousNodeRemover.h>
#include <hoot/core/ops/SuperfluousWayRemover.h>
#include <hoot/core/ops/RecursiveElementRemover.h>
#include <hoot/core/ops/RecursiveSetTagValueOp.h>
#include <hoot/core/ops/RemoveEmptyRelationsOp.h>
#include <hoot/core/ops/UnconnectedWaySnapper.h>
#include <hoot/core/ops/WayJoinerOp.h>

#include <hoot/core/util/Boundable.h>
#include <hoot/core/util/CollectionUtils.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/ConfigUtils.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/GeometryUtils.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/util/MemoryUsageChecker.h>

#include <hoot/core/visitors/ApiTagTruncateVisitor.h>
#include <hoot/core/visitors/FilteredVisitor.h>
#include <hoot/core/visitors/RemoveElementsVisitor.h>
#include <hoot/core/visitors/RemoveDuplicateRelationMembersVisitor.h>
#include <hoot/core/visitors/RemoveInvalidMultilineStringMembersVisitor.h>
#include <hoot/core/visitors/RemoveMissingElementsVisitor.h>
#include <hoot/core/visitors/RemoveTagsVisitor.h>
#include <hoot/core/visitors/ReportMissingElementsVisitor.h>
#include <hoot/core/visitors/SetTagValueVisitor.h>

namespace hoot
{

ChangesetReplacementCreator2::ChangesetReplacementCreator2(
  const bool printStats, const QString& statsOutputFile, const QString osmApiDbUrl)
{
  _changesetCreator.reset(new ChangesetCreator(printStats, statsOutputFile, osmApiDbUrl));
}

QString ChangesetReplacementCreator2::_getJobDescription(
  const QString& input1, const QString& input2, const QString& bounds,
  const QString& output) const
{
  const int maxFilePrintLength = ConfigOptions().getProgressVarPrintLengthMax();
  QString str;
  str += "Deriving replacement output changeset:";
  str += "\nBeing replaced: ..." + input1.right(maxFilePrintLength);
  str += "\nReplacing with ..." + input2.right(maxFilePrintLength);
  str += "\nOutput Changeset: ..." + output.right(maxFilePrintLength);
  str += "\nBounds: " + bounds;
  return str;
}

void ChangesetReplacementCreator2::create(
  const QString& input1, const QString& input2, const geos::geom::Envelope& bounds,
  const QString& output)
{
  // INPUT VALIDATION AND SETUP

  _validateInputs(input1, input2);
  const QString boundsStr = GeometryUtils::envelopeToConfigString(bounds);
  _setGlobalOpts(boundsStr);
  _parseConfigOpts();

  LOG_DEBUG(_getJobDescription(input1, input2, boundsStr, output));

  // DATA LOAD AND INITIAL PREP

  OsmMapPtr mapToReplace = _loadRefMap(input1);
  MemoryUsageChecker::getInstance().check();

  // always remove any existing missing child tags
  RemoveTagsVisitor missingChildTagRemover(QStringList(MetadataTags::HootMissingChild()));
  mapToReplace->visitRw(missingChildTagRemover);
  const bool markMissing =
    ConfigOptions().getChangesetReplacementMarkElementsWithMissingChildren();
  if (markMissing)
  {
    // Find any elements with missing children and tag them with a custom tag, as its possible we'll
    // break them during this process. There's really nothing that can be done about that, since we
    // don't have access to the missing children elements. Any elements with missing children may
    // require manual cleanup after the resulting changeset is applied.
    _markElementsWithMissingChildren(mapToReplace);
  }

  // load the sec dataset and crop to the specified aoi
  OsmMapPtr replacementMap = _loadSecMap(input2);
  MemoryUsageChecker::getInstance().check();

  replacementMap->visitRw(missingChildTagRemover);
  if (markMissing)
  {
    _markElementsWithMissingChildren(replacementMap);
  }

  // CUT

  // cut the secondary data out of the reference data
  OsmMapPtr cookieCutRefMap = _getCookieCutMap(mapToReplace, replacementMap);

  // Combine the cookie cut ref map back with the secondary map, so we can conflate the two
  // together.
  OsmMapPtr combinedMap = cookieCutRefMap;
  _combineMaps(combinedMap, replacementMap, false, "combined");
  replacementMap.reset();

  // clean up introduced mistakes
  _cleanup(mapToReplace);
  _cleanup(combinedMap);

  // CHANGESET GENERATION

  // Derive a changeset between the ref and conflated maps that replaces ref features with
  // secondary features within the bounds and write it out.
  _changesetCreator->create(mapToReplace, combinedMap, output);

  LOG_STATUS(
    "Derived replacement changeset: ..." <<
    output.right(ConfigOptions().getProgressVarPrintLengthMax()));
}

void ChangesetReplacementCreator2::_validateInputs(const QString& input1, const QString& input2)
{
  // Fail if the reader that supports either input doesn't implement Boundable.
  std::shared_ptr<Boundable> boundable =
    std::dynamic_pointer_cast<Boundable>(OsmMapReaderFactory::createReader(input1));
  if (!boundable)
  {
    throw IllegalArgumentException(
      "Reader for " + input1 + " must implement Boundable for replacement changeset derivation.");
  }
  boundable = std::dynamic_pointer_cast<Boundable>(OsmMapReaderFactory::createReader(input2));
  if (!boundable)
  {
    throw IllegalArgumentException(
      "Reader for " + input2 + " must implement Boundable for replacement changeset derivation.");
  }

  // Fail for GeoJSON - GeoJSON coming from Overpass does not have way nodes, so their versions
  // are lost when new way nodes are added to existing ways. For that reason, we can't support it
  // (or at least not sure how to yet).
  OsmGeoJsonReader geoJsonReader;
  if (geoJsonReader.isSupported(input1) || geoJsonReader.isSupported(input2))
  {
    throw IllegalArgumentException(
      "GeoJSON inputs are not supported by replacement changeset derivation.");
  }

  if (ConfigOptions().getConvertOps().size() > 0)
  {
    throw IllegalArgumentException(
      "Replacement changeset derivation does not support convert operations.");
  }
}

void ChangesetReplacementCreator2::_setGlobalOpts(const QString& boundsStr)
{
  conf().set(ConfigOptions::getChangesetXmlWriterAddTimestampKey(), false);
  conf().set(ConfigOptions::getReaderAddSourceDatetimeKey(), false);
  conf().set(ConfigOptions::getWriterIncludeCircularErrorTagsKey(), false);
  conf().set(ConfigOptions::getConvertBoundingBoxKey(), boundsStr);

  // will have to see if setting this to false causes problems in the future...
  conf().set(ConfigOptions::getConvertRequireAreaForPolygonKey(), false);

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
    QStringList metadataAllowTagKeys(MetadataTags::HootMissingChild());
    conf().set(ConfigOptions::getChangesetMetadataAllowedTagKeysKey(), metadataAllowTagKeys);
  }

  // These don't change between scenarios (or at least we haven't needed to change them yet).
  _boundsOpts.loadRefKeepOnlyInsideBounds = false;
  _boundsOpts.cookieCutKeepOnlyInsideBounds = false;
  //_boundsOpts.changesetRefKeepOnlyInsideBounds = false;

  // turn on for testing only
  //conf().set(ConfigOptions::getDebugMapsWriteKey(), true);
}

void ChangesetReplacementCreator2::_parseConfigOpts()
{
  _boundsOpts.loadRefKeepEntireCrossingBounds = true;
  _boundsOpts.loadRefKeepImmediateConnectedWaysOutsideBounds = true; //*
  _boundsOpts.loadSecKeepEntireCrossingBounds = true;
  _boundsOpts.loadSecKeepOnlyInsideBounds = false;
  _boundsOpts.cookieCutKeepEntireCrossingBounds = false; //*
  //_boundsOpts.changesetRefKeepEntireCrossingBounds = true;
  //_boundsOpts.changesetSecKeepEntireCrossingBounds = true;
  //_boundsOpts.changesetSecKeepOnlyInsideBounds = false;
  _boundsOpts.changesetAllowDeletingRefOutsideBounds = true;
  _boundsOpts.inBoundsStrict = false;

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
  //LOG_VART(_boundsOpts.changesetRefKeepEntireCrossingBounds);
  //LOG_VART(_boundsOpts.changesetRefKeepOnlyInsideBounds);
  //LOG_VART(_boundsOpts.changesetSecKeepEntireCrossingBounds);
  //LOG_VART(_boundsOpts.changesetSecKeepOnlyInsideBounds);
  LOG_VART(_boundsOpts.changesetAllowDeletingRefOutsideBounds);
  LOG_VART(_boundsOpts.inBoundsStrict);
}

OsmMapPtr ChangesetReplacementCreator2::_loadRefMap(const QString& input)
{ 
  LOG_STATUS("Loading reference map: " << input << "...");

  // We want to alert the user to the fact their ref versions *could* be being populated incorrectly
  // to avoid difficulties during changeset application at the end. Its likely if they are incorrect
  // at this point the changeset derivation will fail at the end anyway, but let's warn now to give
  // the chance to back out earlier.
  conf().set(ConfigOptions::getReaderWarnOnZeroVersionElementKey(), true);

  conf().set(
    ConfigOptions::getConvertBoundingBoxKeepEntireFeaturesCrossingBoundsKey(),
    _boundsOpts.loadRefKeepEntireCrossingBounds);
  conf().set(
    ConfigOptions::getConvertBoundingBoxKeepOnlyFeaturesInsideBoundsKey(),
    _boundsOpts.loadRefKeepOnlyInsideBounds);
  conf().set(
    ConfigOptions::getConvertBoundingBoxKeepImmediatelyConnectedWaysOutsideBoundsKey(),
    _boundsOpts.loadRefKeepImmediateConnectedWaysOutsideBounds);

  // Here and with sec map loading, attempted to cache the initial map to avoid unnecessary
  // reloading, but it wreaked havoc on the element IDs. May try debugging it again later.
  OsmMapPtr refMap;
  refMap.reset(new OsmMap());
  refMap->setName("ref");
  IoUtils::loadMap(refMap, input, true, Status::Unknown1);

  conf().set(ConfigOptions::getReaderWarnOnZeroVersionElementKey(), false);

  LOG_VART(MapProjector::toWkt(refMap->getProjection()));
  OsmMapWriterFactory::writeDebugMap(refMap, "ref-after-cropped-load");

  return refMap;
}

OsmMapPtr ChangesetReplacementCreator2::_loadSecMap(const QString& input)
{
  LOG_STATUS("Loading secondary map: " << input << "...");

  conf().set(
    ConfigOptions::getConvertBoundingBoxKeepEntireFeaturesCrossingBoundsKey(),
    _boundsOpts.loadSecKeepEntireCrossingBounds);
  conf().set(
    ConfigOptions::getConvertBoundingBoxKeepOnlyFeaturesInsideBoundsKey(),
    _boundsOpts.loadSecKeepOnlyInsideBounds);
  conf().set(
    ConfigOptions::getConvertBoundingBoxKeepImmediatelyConnectedWaysOutsideBoundsKey(), false);

  OsmMapPtr secMap;
  secMap.reset(new OsmMap());
  secMap->setName("sec");
  IoUtils::loadMap(secMap, input, false, Status::Unknown2);

  LOG_VART(MapProjector::toWkt(secMap->getProjection()));
  OsmMapWriterFactory::writeDebugMap(secMap, "sec-after-cropped-load");

  return secMap;
}

void ChangesetReplacementCreator2::_markElementsWithMissingChildren(OsmMapPtr& map)
{
  ReportMissingElementsVisitor elementMarker;
  // Originally, this was going to add reviews rather than tagging elements, but there was an ID
  // provenance problem when using reviews.
  elementMarker.setMarkRelationsForReview(false);
  elementMarker.setMarkWaysForReview(false);
  elementMarker.setRelationKvp(MetadataTags::HootMissingChild() + "=yes");
  elementMarker.setWayKvp(MetadataTags::HootMissingChild() + "=yes");
  LOG_STATUS("\tMarking elements with missing child elements...");
  map->visitRelationsRw(elementMarker);
  LOG_STATUS(
    "\tMarked " << elementMarker.getNumWaysTagged() << " ways with missing child elements.");
  LOG_STATUS(
    "\tMarked " << elementMarker.getNumRelationsTagged() <<
    " relations with missing child elements.");

  OsmMapWriterFactory::writeDebugMap(map, map->getName() + "-after-missing-marked");
}

OsmMapPtr ChangesetReplacementCreator2::_getCookieCutMap(OsmMapPtr doughMap, OsmMapPtr cutterMap)
{
  LOG_VARD(doughMap->getElementCount());
  LOG_VARD(cutterMap->size());

  // If the passed in dough map is empty, there's nothing to be cut out. So, just return the empty
  // ref map.
  if (doughMap->getElementCount() == 0)
  {
    LOG_DEBUG(
      "Nothing to cut from dough map, so returning the empty dough map as the map after " <<
      "cutting: " << doughMap->getName() << "...");
    OsmMapWriterFactory::writeDebugMap(doughMap, "cookie-cut");
    return doughMap;
  }
  else if (cutterMap->size() == 0)
  {
    // If the sec map is empty and we're doing full replacement on non-linear features, we want
    // everything deleted out of the ref inside the the replacement bounds. So, return an empty
    // map.
    LOG_DEBUG(
      "Nothing in cutter map. Full replacement enabled, so returning an empty map " <<
      "as the map after cutting...");
    return OsmMapPtr(new OsmMap());
  }

  LOG_VART(doughMap->getElementCount());
  LOG_VART(MapProjector::toWkt(doughMap->getProjection()));
  OsmMapWriterFactory::writeDebugMap(doughMap, "dough-map");
  LOG_VART(cutterMap->getElementCount());
  LOG_VART(MapProjector::toWkt(cutterMap->getProjection()));

  OsmMapPtr cookieCutMap(new OsmMap(doughMap));
  cookieCutMap->setName("cookie-cut");
  LOG_VART(MapProjector::toWkt(cookieCutMap->getProjection()));
  LOG_DEBUG("Preparing to cookie cut: " << cookieCutMap->getName() << "...");

  OsmMapPtr cutterMapToUse;
  LOG_VART(cutterMap->getElementCount());
  ConfigOptions opts(conf());
  LOG_VART(MapUtils::mapIsPointsOnly(cutterMap));
  const double cookieCutterAlpha = opts.getCookieCutterAlpha();
  double cookieCutterAlphaShapeBuffer = opts.getCookieCutterAlphaShapeBuffer();

    // Generate a cutter shape based on the ref map, which will cause all the ref data to be
    // replaced.
    LOG_DEBUG("Using dough map: " << doughMap->getName() << " as cutter shape map...");
    cutterMapToUse = doughMap;
    // TODO: riverbank test fails with missing POIs without this and the single point test has
    // extra POIs in output without this; explain
    cookieCutterAlphaShapeBuffer = 10.0;

  // Found that if a map only has a couple points or less, generating an alpha shape from them may
  // not be possible (or at least don't know how to yet). So instead, go through the points in the
  // map and replace them with small square shaped polys...from that we can generate the alpha
  // shape.
  if ((int)cutterMapToUse->getElementCount() < 3 && MapUtils::mapIsPointsOnly(cutterMapToUse))
  {
    LOG_DEBUG("Creating a cutter shape map transformation for point map...");
    // Make a copy here since we're making destructive changes to the geometry here for alpha shape
    // generation purposes only.
    cutterMapToUse.reset(new OsmMap(cutterMap));
    PointsToPolysConverter pointConverter;
    LOG_STATUS("\t" << pointConverter.getInitStatusMessage());
    pointConverter.apply(cutterMapToUse);
    LOG_STATUS("\t" << pointConverter.getCompletedStatusMessage());
    MapProjector::projectToWgs84(cutterMapToUse);
  }

  LOG_VART(cutterMapToUse->getElementCount());
  OsmMapWriterFactory::writeDebugMap(cutterMapToUse, "cutter-map");

  LOG_STATUS("Generating cutter shape map from: " << cutterMapToUse->getName() << "...");

  LOG_VART(cookieCutterAlpha);
  LOG_VART(cookieCutterAlphaShapeBuffer);
  OsmMapPtr cutterShapeOutlineMap;
  try
  {
    cutterShapeOutlineMap =
      AlphaShapeGenerator(cookieCutterAlpha, cookieCutterAlphaShapeBuffer)
        .generateMap(cutterMapToUse);
  }
  catch (const HootException& e)
  {
    if (e.getWhat().contains("Alpha Shape area is zero"))
    {
      LOG_ERROR(
        "No cut shape generated from secondary data. Is your secondary data empty " <<
        "or have you filtered it to be empty? error: " << e.getWhat());
    }
    // rethrow the original exception
    throw;
  }

  // not exactly sure yet why this projection needs to be done
  MapProjector::projectToWgs84(cutterShapeOutlineMap);
  LOG_VART(MapProjector::toWkt(cutterShapeOutlineMap->getProjection()));
  OsmMapWriterFactory::writeDebugMap(cutterShapeOutlineMap, "cutter-shape");

  // Cookie cut the shape of the cutter shape map out of the cropped ref map.
  LOG_STATUS("Cookie cutting cutter shape out of: " << cookieCutMap->getName() << "...");

  // We're not going to remove missing elements, as we want to have as minimal of an impact on
  // the resulting changeset as possible.
  CookieCutter(
    false, 0.0, _boundsOpts.cookieCutKeepEntireCrossingBounds,
    _boundsOpts.cookieCutKeepOnlyInsideBounds, false)
    .cut(cutterShapeOutlineMap, cookieCutMap);
  MapProjector::projectToWgs84(cookieCutMap); // not exactly sure yet why this needs to be done
  LOG_VARD(cookieCutMap->getElementCount());
  MapProjector::projectToWgs84(doughMap);
  LOG_VART(doughMap->getElementCount());
  LOG_VART(MapProjector::toWkt(cookieCutMap->getProjection()));
  MemoryUsageChecker::getInstance().check();
  OsmMapWriterFactory::writeDebugMap(cookieCutMap, "cookie-cut");

  return cookieCutMap;
}

void ChangesetReplacementCreator2::_combineMaps(
  OsmMapPtr& map1, OsmMapPtr& map2, const bool throwOutDupes, const QString& debugFileName)
{
  LOG_VART(map1.get());
  LOG_VART(map2.get());

  MapProjector::projectToWgs84(map1);
  MapProjector::projectToWgs84(map2);   // not exactly sure yet why this needs to be done

  if (map2->size() == 0)
  {
    LOG_DEBUG("Combined map size: " << map1->size());
    return;
  }

  LOG_STATUS("Combining maps: " << map1->getName() << " and " << map2->getName() << "...");

  map1->append(map2, throwOutDupes);
  LOG_VART(MapProjector::toWkt(map1->getProjection()));
  LOG_DEBUG("Combined map size: " << map1->size());

  MemoryUsageChecker::getInstance().check();
  OsmMapWriterFactory::writeDebugMap(map1, debugFileName);
}

void ChangesetReplacementCreator2::_cleanup(OsmMapPtr& map)
{
  LOG_STATUS("Cleaning up duplicated elements for " << map->getName() << "...");

  // Due to mixed geometry type relations explained in _getDefaultGeometryFilters, we may have
  // introduced some duplicate relation members by this point.
//  RemoveDuplicateRelationMembersVisitor dupeMembersRemover;
//  LOG_STATUS("\t" << dupeMembersRemover.getInitStatusMessage());
//  map->visitRw(dupeMembersRemover);
//  LOG_STATUS("\t" << dupeMembersRemover.getCompletedStatusMessage());

  // get rid of straggling nodes
  SuperfluousNodeRemover orphanedNodeRemover;
  LOG_STATUS("\t" << orphanedNodeRemover.getInitStatusMessage());
  orphanedNodeRemover.apply(map);
  LOG_STATUS("\t" << orphanedNodeRemover.getCompletedStatusMessage());

  // This will remove any relations that were already empty or became empty after we removed
  // duplicated members.
//  RemoveEmptyRelationsOp emptyRelationRemover;
//  LOG_STATUS("\t" << emptyRelationRemover.getInitStatusMessage());
//  emptyRelationRemover.apply(map);
//  LOG_STATUS("\t" << emptyRelationRemover.getCompletedStatusMessage());

  // get out of orthographic
  MapProjector::projectToWgs84(map);
}

}
