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
#include "ChangesetCutOnlyCreator.h"

// Hoot
#include <hoot/core/algorithms/alpha-shape/AlphaShapeGenerator.h>
#include <hoot/core/algorithms/changeset/ChangesetCreator.h>
#include <hoot/core/algorithms/changeset/ChangesetReplacementElementIdSynchronizer.h>

#include <hoot/core/conflate/CookieCutter.h>

#include <hoot/core/criterion/ChainCriterion.h>
#include <hoot/core/criterion/WayCriterion.h>
#include <hoot/core/criterion/InBoundsCriterion.h>
#include <hoot/core/criterion/NotCriterion.h>
#include <hoot/core/criterion/TagKeyCriterion.h>

#include <hoot/core/elements/MapUtils.h>
#include <hoot/core/elements/MapProjector.h>

#include <hoot/core/geometry/GeometryUtils.h>

#include <hoot/core/index/OsmMapIndex.h>

#include <hoot/core/io/IoUtils.h>
#include <hoot/core/io/OsmGeoJsonReader.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>

#include <hoot/core/ops/ElementIdToVersionMapper.h>
#include <hoot/core/ops/MapCleaner.h>
#include <hoot/core/ops/MapCropper.h>
#include <hoot/core/ops/PointsToPolysConverter.h>
#include <hoot/core/ops/RecursiveSetTagValueOp.h>
#include <hoot/core/ops/RemoveEmptyRelationsOp.h>
#include <hoot/core/ops/SuperfluousNodeRemover.h>

#include <hoot/core/util/Boundable.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/DbUtils.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/util/MemoryUsageChecker.h>

#include <hoot/core/visitors/FilteredVisitor.h>
#include <hoot/core/visitors/RemoveElementsVisitor.h>
#include <hoot/core/visitors/RemoveDuplicateRelationMembersVisitor.h>
#include <hoot/core/visitors/RemoveTagsVisitor.h>
#include <hoot/core/visitors/ReportMissingElementsVisitor.h>
#include <hoot/core/visitors/SetTagValueVisitor.h>

// tgs
#include <tgs/RStarTree/MemoryPageStore.h>

// Qt
#include <QElapsedTimer>

namespace hoot
{

const QString ChangesetReplacementCreatorAbstract::JOB_SOURCE = "ChangesetDeriveReplacement";

ChangesetReplacementCreatorAbstract::ChangesetReplacementCreatorAbstract() :
_maxFilePrintLength(ConfigOptions().getProgressVarPrintLengthMax() * 1.5),
_geometryFiltersSpecified(false),
_enableWaySnapping(false),
_changesetId("1"),
_numChanges(0),
_numTotalTasks(-1),
_currentTask(1)
{
}

QString ChangesetReplacementCreatorAbstract::_boundsInterpretationToString(
  const BoundsInterpretation& boundsInterpretation) const
{
  switch (boundsInterpretation)
  {
    case BoundsInterpretation::Lenient:
      return "lenient";
    case BoundsInterpretation::Strict:
      return "strict";
    case BoundsInterpretation::Hybrid:
      return "hybrid";
    default:
      return "";
  }
}

void ChangesetReplacementCreatorAbstract::setChangesetOptions(
  const bool printStats, const QString& statsOutputFile, const QString osmApiDbUrl)
{
  LOG_VARD(printStats);
  LOG_VARD(statsOutputFile);
  LOG_VARD(osmApiDbUrl);
  _changesetCreator = std::make_shared<ChangesetCreator>(printStats, statsOutputFile, osmApiDbUrl);
}

void ChangesetReplacementCreatorAbstract::_printJobDescription() const
{
  QString boundsStr = "Bounds calculation is " +
    _boundsInterpretationToString(_boundsInterpretation);
  const QString replacementTypeStr = _fullReplacement ? "full" : "overlapping only";
  QString waySnappingStr = "is ";
  if (!_enableWaySnapping)
  {
    waySnappingStr += "not ";
  }
  waySnappingStr += "enabled";
  QString cropDbInputOnReadStr = "is ";
  if (!ConfigOptions().getApidbReaderReadFullThenCropOnBounded())
  {
    cropDbInputOnReadStr += "not ";
  }
  cropDbInputOnReadStr += "enabled";

  QString str;
  str += "Deriving replacement output changeset:";
  str += "\nBeing replaced: ..." + FileUtils::toLogFormat(_input1, _maxFilePrintLength);
  str += "\nReplacing with ..." + FileUtils::toLogFormat(_input2, _maxFilePrintLength);
  str +=
    "\nAt Bounds: ..." +
    GeometryUtils::polygonToString(_replacementBounds)
      .right(ConfigOptions().getProgressVarPrintLengthMax() * 2);
  str += "\nOutput Changeset: ..." + FileUtils::toLogFormat(_output, _maxFilePrintLength);
  LOG_STATUS(str);

  str = "";
  str += "\nBounds interpretation: " + boundsStr;
  str += "\nReplacement is: " + replacementTypeStr;
  str += "\nWay snapping: " + waySnappingStr;
  str += "\nCropping database inputs after read: " + cropDbInputOnReadStr;
  LOG_INFO(str);
}

float ChangesetReplacementCreatorAbstract::_getJobPercentComplete() const
{
  return (float)(_currentTask - 1) / (float)_numTotalTasks;
}

void ChangesetReplacementCreatorAbstract::_validateInputs() const
{
  if (!_replacementBounds)
  {
    throw IllegalArgumentException(
      "Invalid replacement bounds passed to changeset replacement derivation.");
  }

  // Fail if the reader that supports either input doesn't implement Boundable.
  std::shared_ptr<Boundable> boundable =
    std::dynamic_pointer_cast<Boundable>(OsmMapReaderFactory::createReader(_input1));
  if (!boundable)
  {
    throw IllegalArgumentException(
      "Reader for " + _input1 + " must implement Boundable for replacement changeset derivation.");
    }
  if (!_input2.isEmpty())
  {
    boundable = std::dynamic_pointer_cast<Boundable>(OsmMapReaderFactory::createReader(_input2));
    if (!boundable)
    {
      throw IllegalArgumentException(
        "Reader for " + _input2 + " must implement Boundable for replacement changeset derivation.");
    }
  }

  // Fail for GeoJSON - GeoJSON coming from Overpass does not have way nodes, so their versions
  // are lost when new way nodes are added to existing ways. For that reason, we can't support it
  // (or at least not sure how to yet).
  OsmGeoJsonReader geoJsonReader;
  if (geoJsonReader.isSupported(_input1) ||
      (!_input2.isEmpty() && geoJsonReader.isSupported(_input2)))
  {
    throw IllegalArgumentException(
      "GeoJSON inputs are not supported by replacement changeset derivation.");
  }
  QFile outputFile(_output);
  if (outputFile.exists() && !outputFile.remove())
  {
    throw HootException("Unable to remove changeset output file: " + _output);
  }

  if (!ConfigOptions().getConvertOps().empty())
  {
    throw IllegalArgumentException(
      "Replacement changeset derivation does not support convert operations.");
  }
}

OsmMapPtr ChangesetReplacementCreatorAbstract::_loadInputMap(
  const QString& mapName, const QString& inputUrl, const bool useFileIds, const Status& status,
  const bool keepEntireFeaturesCrossingBounds, const bool keepOnlyFeaturesInsideBounds,
  const bool keepImmediatelyConnectedWaysOutsideBounds, const bool warnOnZeroVersions,
  OsmMapPtr& cachedMap) const
{
  conf().set(
    ConfigOptions::getBoundsKeepEntireFeaturesCrossingBoundsKey(),
    keepEntireFeaturesCrossingBounds);
  conf().set(
    ConfigOptions::getBoundsKeepOnlyFeaturesInsideBoundsKey(),
   keepOnlyFeaturesInsideBounds);
  conf().set(
    ConfigOptions::getBoundsKeepImmediatelyConnectedWaysOutsideBoundsKey(),
    keepImmediatelyConnectedWaysOutsideBounds);

  if (warnOnZeroVersions)
  {
    // We want to alert the user to the fact their ref versions *could* be being populated
    // incorrectly to avoid difficulties during changeset application at the end. Its likely if they
    // are incorrect at this point the changeset derivation will fail at the end anyway, but let's
    // warn now to give the chance to back out of the replacement earlier.
    conf().set(ConfigOptions::getReaderWarnOnZeroVersionElementKey(), true);
  }

  OsmMapPtr map;
  if (DbUtils::isDbUrl(inputUrl))
  {
    // Caching inputs only helps with file based inputs and isn't really possible for db inputs due
    // to the potential size, so skip caching if the source is a db. Our db query crops
    // automatically for us without reading all of the source data in.
    LOG_STATUS(
      "Loading " << mapName << " map from: ..." << FileUtils::toLogFormat(inputUrl, _maxFilePrintLength) << "...");
    map = std::make_shared<OsmMap>();
    IoUtils::loadMap(map, inputUrl, useFileIds, status);
  }
  else
  {
    // File inputs always have to read in completely before cropping. Rather than read the file
    // completely for each processing pass, we'll read it once and crop it down as needed per pass.
    // Note that this is only important if the implementation is running separate cutting passes
    // per geometry type, like ChangesetCutOnlyCreator. For single geometry pass implementations
    // this doesn't optimize anything and just results in an extra map copy.
    if (!cachedMap)
    {
      // Clear out the bounding box param temporarily, so that we can read the full map here. Kind
      // of kludgy, but there is no access to it from here via IoUtils::loadMap.
      const QString bbox = conf().getString(ConfigOptions::getBoundsKey());
      conf().set(ConfigOptions::getBoundsKey(), "");

      LOG_STATUS("Loading map from: ..." << FileUtils::toLogFormat(inputUrl, _maxFilePrintLength) << "...");
      cachedMap = std::make_shared<OsmMap>();
      cachedMap->setName(mapName);
      IoUtils::loadMap(cachedMap, inputUrl, useFileIds, status);
      // Restore it back to original.
      conf().set(ConfigOptions::getBoundsKey(), bbox);
    }
    LOG_STATUS(
      "Copying map of size: " << StringUtils::formatLargeNumber(cachedMap->size()) <<
      " from: " << cachedMap->getName() << "...");
    map = std::make_shared<OsmMap>(cachedMap);
  }
  // Formerly, we let ApiDbReader do the cropping and only if the crop options here were different
  // than the default. However, its been found that the way ApiDbReader returns relations can result
  // in returning far more data than needed, so now we crop after load regardless of the input
  // format.
  // UPDATE 12/16/20: After working on #4299, I think this type of cropping here is leading to
  // dropped features, although it does prevent unnecessary processing of data. This may need some
  // rethinking of how this is implemented.
  IoUtils::cropToBounds(map, _replacementBounds, keepImmediatelyConnectedWaysOutsideBounds);
  LOG_STATUS(
    "Loaded " << mapName << " map from: ..." << FileUtils::toLogFormat(inputUrl, _maxFilePrintLength) << " with " <<
    StringUtils::formatLargeNumber(map->size()) << " features...");

  if (warnOnZeroVersions)
  {
    // Restore default setting value.
    conf().set(ConfigOptions::getReaderWarnOnZeroVersionElementKey(), false);
  }
  map->setName(mapName);

  LOG_VART(MapProjector::toWkt(map->getProjection()));
  OsmMapWriterFactory::writeDebugMap(
    map, className(), _changesetId + "-" + map->getName() + "-after-cropped-load");

  MemoryUsageChecker::getInstance().check();

  return map;
}

void ChangesetReplacementCreatorAbstract::_removeMetadataTags(const OsmMapPtr& map) const
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

void ChangesetReplacementCreatorAbstract::_markElementsWithMissingChildren(
  const OsmMapPtr& map) const
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
    map, className(), _changesetId + "-" + map->getName() + "-after-missing-marked");
}

void ChangesetReplacementCreatorAbstract::_filterFeatures(
  const OsmMapPtr& map, const ElementCriterionPtr& featureFilter,
  const GeometryTypeCriterion::GeometryType& /*geometryType*/, const Settings& config,
  const QString& debugFileName) const
{
  LOG_STATUS(
    "Filtering " << StringUtils::formatLargeNumber(map->size()) << " features for: " <<
    map->getName() << " with filter: " << featureFilter->toString() << "...");

  // Negate the input filter, since we're removing everything but what passes the input filter.
  RemoveElementsVisitor elementPruner(true);
  // The criteria must be added before the config or map is set. We may want to change
  // MultipleCriterionConsumerVisitor and RemoveElementsVisitor to make this behavior less brittle.
  elementPruner.addCriterion(featureFilter);
  elementPruner.setConfiguration(config);
  elementPruner.setOsmMap(map.get());
  // If recursion isn't used here, nasty crashes occur that are hard to track down occur at times.
  // Not completely convinced recursion should be used here for relations, though.
  elementPruner.setRecursive(true);
  map->visitRw(elementPruner);
  LOG_INFO(elementPruner.getCompletedStatusMessage());

  LOG_VART(MapProjector::toWkt(map->getProjection()));
  OsmMapWriterFactory::writeDebugMap(map, className(), debugFileName);
}

OsmMapPtr ChangesetReplacementCreatorAbstract::_getCookieCutMap(
  OsmMapPtr doughMap, OsmMapPtr cutterMap, const GeometryTypeCriterion::GeometryType& geometryType) const
{
  // This logic has become extremely complicated over time to handle all the different cut
  // and replace use cases. There may be way to simplify some of this logic related to
  // strict/lenient bounds in here by changing some of the initial crop related opts set in
  // _parseConfigOpts...not sure.

  LOG_DEBUG(
    "Calculating the cutting of replacement data area out of data to be replaced for geometry: " <<
    GeometryTypeCriterion::typeToString(geometryType) << "...");
  LOG_VARD(_fullReplacement);
  LOG_VARD(_boundsInterpretationToString(_boundsInterpretation));
  LOG_VARD(_currentChangeDerivationPassIsLinear);
  LOG_VARD(doughMap->getName());
  LOG_VARD(doughMap->size());
  LOG_VARD(cutterMap->getName());
  LOG_VARD(cutterMap->size());
  OsmMapWriterFactory::writeDebugMap(doughMap, className(), _changesetId + "-dough-map-input");
  OsmMapWriterFactory::writeDebugMap(cutterMap, className(), _changesetId + "-cutter-map-input");

  // It could just be an byproduct of how data is being read out by core scripts during testing, but
  // when doing adjacent cell updates I'm getting cropped data with a bunch of empty relations in
  // them as input. That eventually needs to be dealt with, but regardless, checking node/way count
  // and not including relations in the size count is a better check for the total map size so that
  // the alpha shape gets calculated correctly.
  const int doughMapInputSize = doughMap->getWayCount() + doughMap->getNodeCount();
  const int cutterMapInputSize = cutterMap->getWayCount() + cutterMap->getNodeCount();

  /*
   * lenient/overlapping - cutter shape is all overlapping sec data inside the bounds and
   *                       immediately connected outside the bounds OR all ref data inside the
   *                       bounds and immediately connected outside the bounds (if linear) if sec
   *                       map is empty
     lenient/full        - cutter shape is all ref data inside the bounds and immediately connected
                           outside the bounds (if linear)
     strict/overlapping  - cutter shape is all overlapping sec data inside the bounds
     strict/full         - cutter shape is all ref data inside the bounds

     hybrid bounds acts like strict for linear features and lenient for polygon features.
   */

  QString mapName;
  // If the passed in dough map is empty, there's nothing to be cut out. So, just return the empty
  // ref map.
  if (doughMapInputSize == 0)
  {
    LOG_DEBUG(
      "Nothing to cut from dough map, so returning the empty dough map as the map after " <<
      "cutting: " << doughMap->getName() << "...");
    OsmMapWriterFactory::writeDebugMap(doughMap, className(), _changesetId + "-cookie-cut");
    // Copy here to avoid changing the ref map passed in as the dough map input.
    return std::make_shared<OsmMap>(doughMap);
  }
  else if (cutterMapInputSize == 0)
  {
    // Linear features need to be handled slightly differently, due to the need to snap cut features
    // back to reference features when the strict bounds interpretation is enabled.
    if (!_currentChangeDerivationPassIsLinear)
    {
      // The bounds interpretation doesn't seem to matter here for non-linear features.
      if (_fullReplacement)
      {
        // If the sec map is empty and we're doing full replacement on non-linear features, we want
        // everything deleted out of the ref inside the the replacement bounds. So, return an empty
        // map.
        LOG_DEBUG(
          "Nothing in cutter map. Full replacement enabled, so returning an empty map " <<
          "as the map after cutting...");
        return std::make_shared<OsmMap>();
      }
      else
      {
        // If the sec map is empty and we're not doing full replacement, there's nothing in the sec
        // to overlap with the ref, so leave the ref untouched.
        LOG_DEBUG(
          "Nothing in cutter map. Full replacement not enabled, so returning the entire dough " <<
          "map as the map after cutting: " << doughMap->getName() << "...");
        OsmMapWriterFactory::writeDebugMap(doughMap, className(), _changesetId + "-cookie-cut");
        // Copy here to avoid changing the ref map passed in as the dough map input.
        return std::make_shared<OsmMap>(doughMap);
      }
    }
    else
    {
      if (_fullReplacement && _boundsInterpretation == BoundsInterpretation::Lenient)
      {
        // If our map contains linear features only, the sec map is empty, we're doing full
        // replacement, AND there isn't a strict interpretation of the bounds, we want everything
        // deleted out of the ref inside the replacement bounds and features immediately connected
        // outside of the bounds. So, return an empty map.
        LOG_DEBUG(
          "Nothing in cutter map for linear features. Full replacement and lenient bounds "
          "interpretation, so returning an empty map as the map after cutting...");
        return std::make_shared<OsmMap>();
      }
      else if (_fullReplacement && _boundsInterpretation != BoundsInterpretation::Lenient )
      {
        // With the strict bounds interpretation, full replacement, and an empty secondary map,
        // we want simply to cut the replacement bounds out, since we can't calc an alpha shape off
        // of no replacement data. No need to use the cookie cutter here if no alpha shape is
        // involved. Just use the map cropper.
        LOG_DEBUG(
          "Nothing in cutter map. Full replacement with strict bounds enabled, so cropping out " <<
          "the bounds area of the dough map to be the map after cutting: " <<
          doughMap->getName() << "...");
        OsmMapPtr cookieCutMap = std::make_shared<OsmMap>(doughMap);
        mapName = "cookie-cut";
        if (geometryType != GeometryTypeCriterion::Unknown)
        {
          mapName += "-" + GeometryTypeCriterion::typeToString(geometryType);
        }
        cookieCutMap->setName(mapName);
        MapCropper cropper;
        cropper.setBounds(_replacementBounds);
        cropper.setRemoveSuperflousFeatures(false);
        cropper.setKeepEntireFeaturesCrossingBounds(false);
        cropper.setKeepOnlyFeaturesInsideBounds(false);
        cropper.setInvert(true);
        // We're not going to remove missing elements, as we want to have as minimal of an impact on
        // the resulting changeset as possible.
        cropper.setRemoveMissingElements(false);
        LOG_STATUS(cropper.getInitStatusMessage());
        cropper.apply(cookieCutMap);
        LOG_INFO(cropper.getCompletedStatusMessage());
        OsmMapWriterFactory::writeDebugMap(cookieCutMap, className(), _changesetId + "-cookie-cut");
        return cookieCutMap;
      }
      else
      {
        // If the sec map is empty and we're not doing full replacement, there's nothing in the sec
        // to overlap with the ref, so leave the ref untouched.
        LOG_DEBUG(
          "Nothing in cutter map for linear features. Full replacement not enabled, so returning "
          "the entire dough map as the map after cutting: " << doughMap->getName() << "...");
        OsmMapWriterFactory::writeDebugMap(doughMap, className(), _changesetId + "-cookie-cut");
        // Copy here to avoid changing the ref map passed in as the dough map input.
        return std::make_shared<OsmMap>(doughMap);
      }
    }
  }

  LOG_VART(doughMap->size());
  LOG_VART(MapProjector::toWkt(doughMap->getProjection()));
  OsmMapWriterFactory::writeDebugMap(doughMap, className(), _changesetId + "-dough-map");
  LOG_VART(MapProjector::toWkt(cutterMap->getProjection()));

  OsmMapPtr cookieCutMap = std::make_shared<OsmMap>(doughMap);
  mapName = "cookie-cut";
  if (geometryType != GeometryTypeCriterion::Unknown)
  {
    mapName += "-" + GeometryTypeCriterion::typeToString(geometryType);
  }
  cookieCutMap->setName(mapName);
  LOG_VART(MapProjector::toWkt(cookieCutMap->getProjection()));
  LOG_DEBUG("Preparing to cookie cut: " << cookieCutMap->getName() << "...");

  OsmMapPtr cutterMapToUse;
  ConfigOptions opts(conf());
  LOG_VART(MapUtils::mapIsPointsOnly(cutterMap));
  const double cookieCutterAlpha = opts.getCookieCutterAlpha();
  double cookieCutterAlphaShapeBuffer = opts.getCookieCutterAlphaShapeBuffer();
  if (_currentChangeDerivationPassIsLinear) // See related note above when the cutter map is empty.
  {
    if (_boundsInterpretation == BoundsInterpretation::Lenient && _fullReplacement)
    {
      // Generate a cutter shape based on the ref map, which will cause all the ref data to be
      // replaced.
      LOG_DEBUG("Using dough map: " << doughMap->getName() << " as cutter shape map...");
      cutterMapToUse = doughMap;
      // riverbank test fails with missing POIs without this and the single point test has
      // extra POIs in output without this; why?
      cookieCutterAlphaShapeBuffer = 10.0;
    }
    else
    {
      LOG_DEBUG("Using cutter map: " << cutterMap->getName() << " as cutter shape map...");
      cutterMapToUse = cutterMap;
    }
  }
  else
  {
    if (_fullReplacement)
    {
      // Generate a cutter shape based on the ref map, which will cause all the ref data to be
      // replaced.
      LOG_DEBUG("Using dough map: " << doughMap->getName() << " as cutter shape map...");
      cutterMapToUse = doughMap;
      // TODO: see similar note above
      cookieCutterAlphaShapeBuffer = 10.0;
    }
    else
    {
      // Generate a cutter shape based on the cropped secondary map, which will cause only
      // overlapping data between the two datasets to be replaced.
      LOG_DEBUG("Using cutter map: " << cutterMap->getName() << " as cutter shape map...");
      cutterMapToUse = cutterMap;
    }
  }

  // Found that if a map only has less than a few points, generating an alpha shape from them may
  // not be possible (or at least don't know how to yet). So instead, go through the points in the
  // map and replace them with small square shaped polys...from that we can generate the alpha
  // shape.
  const int cutterMapToUseSize = cutterMapToUse->getNodeCount();
  if (cutterMapToUseSize < 3 && MapUtils::mapIsPointsOnly(cutterMapToUse))
  {
    LOG_DEBUG("Creating a cutter shape map transformation for point map...");
    // Make a copy here since we're making destructive changes to the geometry here for alpha shape
    // generation purposes only.
    cutterMapToUse = std::make_shared<OsmMap>(cutterMap);
    PointsToPolysConverter pointConverter;
    LOG_INFO(pointConverter.getInitStatusMessage());
    pointConverter.apply(cutterMapToUse);
    LOG_DEBUG(pointConverter.getCompletedStatusMessage());
    MapProjector::projectToWgs84(cutterMapToUse);
  }

  LOG_VART(cutterMapToUse->size());
  OsmMapWriterFactory::writeDebugMap(
    cutterMapToUse, className(), _changesetId + "-cutter-map-to-use");

  LOG_STATUS(
    "Generating cutter shape map from: " << cutterMapToUse->getName() << " of size: " <<
    StringUtils::formatLargeNumber(cutterMapToUse->size()) << "...");

  // Alpha shape generation and cookie cutting for line features is our current bottleneck for C&R.
  // Not sure yet if anything else can be done to improve the performance. Turning off covering
  // stragglers can help a lot in certain situations for alpha shape generation. For cookie cutting,
  // cropping specifically can slow, especially with node removal.

  LOG_VART(cookieCutterAlpha);
  LOG_VART(cookieCutterAlphaShapeBuffer);
  OsmMapPtr cutterShapeOutlineMap;
  AlphaShapeGenerator alphaShapeGenerator(cookieCutterAlpha, cookieCutterAlphaShapeBuffer);
  // Covering stragglers here can be very slow for linear features. So far, have only needed it for
  // point passes, which don't seem to be slow.
  alphaShapeGenerator.setManuallyCoverSmallPointClusters(
    geometryType == GeometryTypeCriterion::Point);
  try
  {
    cutterShapeOutlineMap = alphaShapeGenerator.generateMap(cutterMapToUse);
  }
  catch (const HootException& e)
  {
    if (e.getWhat().contains("Alpha Shape area is zero"))
    {
      QString errorMsg = "No cut shape generated from secondary data";
      if (geometryType != GeometryTypeCriterion::Unknown)
      {
        errorMsg +=
          " when processing geometry type: " + GeometryTypeCriterion::typeToString(geometryType);
      }
      errorMsg +=
        ". Is your secondary data empty or have you filtered it to be empty? error: " + e.getWhat();
      LOG_ERROR(errorMsg);
    }
    throw;
  }

  // not exactly sure yet why this projection needs to be done
  MapProjector::projectToWgs84(cutterShapeOutlineMap);
  LOG_VART(MapProjector::toWkt(cutterShapeOutlineMap->getProjection()));
  OsmMapWriterFactory::writeDebugMap(
    cutterShapeOutlineMap, className(), _changesetId + "-cutter-shape");

  // Cookie cut the shape of the cutter shape map out of the cropped ref map.
  LOG_STATUS(
    "Cutting cutter shape out of: " << cookieCutMap->getName() << " with size: " <<
    StringUtils::formatLargeNumber(cookieCutMap->size()) << "...");

  // We're not going to remove missing elements, as we want to have as minimal of an impact on
  // the resulting changeset as possible.
  CookieCutter(
    false, 0.0, _boundsOpts.cookieCutKeepEntireCrossingBounds,
    _boundsOpts.cookieCutKeepOnlyInsideBounds, false)
    .cut(cutterShapeOutlineMap, cookieCutMap);
  // not exactly sure yet why this projection needs to be done
  MapProjector::projectToWgs84(cookieCutMap);
  LOG_VARD(cookieCutMap->size());
  MapProjector::projectToWgs84(doughMap);
  LOG_VART(MapProjector::toWkt(cookieCutMap->getProjection()));
  MemoryUsageChecker::getInstance().check();
  OsmMapWriterFactory::writeDebugMap(cookieCutMap, className(), _changesetId + "-cookie-cut");

  return cookieCutMap;
}

void ChangesetReplacementCreatorAbstract::_clean(OsmMapPtr& map) const
{
  LOG_STATUS("Cleaning map: " << map->getName() << "...");

  MapCleaner().apply(map);

  MapProjector::projectToWgs84(map);  // cleaning works in planar
  LOG_VART(MapProjector::toWkt(map->getProjection()));
  OsmMapWriterFactory::writeDebugMap(map, className(), _changesetId + "-cleaned");
  LOG_DEBUG("Cleaned map size: " << map->size());
}

QMap<ElementId, long> ChangesetReplacementCreatorAbstract::_getIdToVersionMappings(
  const OsmMapPtr& map) const
{
  LOG_INFO("Mapping element IDs to element versions for: " << map->getName() << "...");

  ElementIdToVersionMapper idToVersionMapper;
  idToVersionMapper.apply(map);
  LOG_DEBUG(idToVersionMapper.getCompletedStatusMessage());
  const QMap<ElementId, long> idToVersionMappings = idToVersionMapper.getMappings();
  LOG_VART(idToVersionMappings.size());
  return idToVersionMappings;
}

void ChangesetReplacementCreatorAbstract::_addChangesetDeleteExclusionTags(OsmMapPtr& map) const
{
  LOG_INFO(
    "Setting connected way features outside of bounds to be excluded from deletion for: " <<
    map->getName() << "...");

  // Add the changeset deletion exclusion tag to all connected ways previously tagged during load.

  SetTagValueVisitor addTagVis(MetadataTags::HootChangeExcludeDelete(), "yes");
  ChainCriterion addTagCrit(
      std::make_shared<WayCriterion>(),
      std::make_shared<TagKeyCriterion>(MetadataTags::HootConnectedWayOutsideBounds()));
  FilteredVisitor deleteExcludeTagVis(addTagCrit, addTagVis);
  map->visitRw(deleteExcludeTagVis);
  LOG_DEBUG(addTagVis.getCompletedStatusMessage());

  // Add the changeset deletion exclusion tag to all children of those connected ways.

  std::shared_ptr<ChainCriterion> childAddTagCrit =
    std::make_shared<ChainCriterion>(
      std::make_shared<WayCriterion>(),
      std::make_shared<TagKeyCriterion>(MetadataTags::HootChangeExcludeDelete()));
  RecursiveSetTagValueOp childDeletionExcludeTagOp(
    MetadataTags::HootChangeExcludeDelete(), "yes", childAddTagCrit);
  childDeletionExcludeTagOp.apply(map);
  LOG_DEBUG(childDeletionExcludeTagOp.getCompletedStatusMessage());

  MemoryUsageChecker::getInstance().check();
  OsmMapWriterFactory::writeDebugMap(
    map, className(), _changesetId + "-" + map->getName() + "-after-delete-exclusion-tagging-1");
}

void ChangesetReplacementCreatorAbstract::_excludeFeaturesFromChangesetDeletion(OsmMapPtr& map) const
{
  if (map->size() == 0)
  {
    return;
  }

  LOG_INFO(
    "Marking reference features in: " << map->getName() << " for exclusion from deletion...");

  // Exclude ways and all their children from deletion that are not entirely within the replacement
  // bounds.

  std::shared_ptr<InBoundsCriterion> boundsCrit =
    std::make_shared<InBoundsCriterion>(_boundsOpts.inBoundsStrict);
  boundsCrit->setBounds(_replacementBounds);
  boundsCrit->setOsmMap(map.get());
  std::shared_ptr<NotCriterion> notInBoundsCrit = std::make_shared<NotCriterion>(boundsCrit);
  std::shared_ptr<ChainCriterion> elementCrit =
    std::make_shared<ChainCriterion>(std::make_shared<WayCriterion>(), notInBoundsCrit);

  RecursiveSetTagValueOp tagSetter(MetadataTags::HootChangeExcludeDelete(), "yes", elementCrit);
  tagSetter.apply(map);
  LOG_DEBUG(tagSetter.getCompletedStatusMessage());

  MemoryUsageChecker::getInstance().check();
  LOG_VART(MapProjector::toWkt(map->getProjection()));
  OsmMapWriterFactory::writeDebugMap(
    map, className(), _changesetId + "-" + map->getName() + "-after-delete-exclusion-tagging-2");
}

void ChangesetReplacementCreatorAbstract::_cleanup(OsmMapPtr& map) const
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

  OsmMapWriterFactory::writeDebugMap(
    map, className(), _changesetId + "-" + map->getName() + "-after-cleanup");
}

void ChangesetReplacementCreatorAbstract::_synchronizeIds(
  const QList<OsmMapPtr>& mapsBeingReplaced, const QList<OsmMapPtr>& replacementMaps) const
{
  assert(mapsBeingReplaced.size() == replacementMaps.size());
  for (int i = 0; i < mapsBeingReplaced.size(); i++)
  {
    OsmMapPtr mapBeingReplaced = mapsBeingReplaced.at(i);
    OsmMapPtr replacementMap = replacementMaps.at(i);
    _synchronizeIds(mapBeingReplaced, replacementMap);
  }
}

void ChangesetReplacementCreatorAbstract::_synchronizeIds(
  OsmMapPtr mapBeingReplaced, OsmMapPtr replacementMap) const
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

  LOG_STATUS("Synchronizing element IDs between maps...");
  LOG_VART(MapProjector::toWkt(mapBeingReplaced->getProjection()));
  LOG_VART(MapProjector::toWkt(replacementMap->getProjection()));

  ChangesetReplacementElementIdSynchronizer idSync;
  OsmMapWriterFactory::writeDebugMap(
    mapBeingReplaced, className(), _changesetId + "-" + mapBeingReplaced->getName() +
    "-source-before-id-sync");
  OsmMapWriterFactory::writeDebugMap(
    replacementMap, className(),
    _changesetId + "-" + replacementMap->getName() + "-target-before-id-sync");

  idSync.synchronize(mapBeingReplaced, replacementMap);

  // get rid of straggling nodes
  SuperfluousNodeRemover orphanedNodeRemover;
  orphanedNodeRemover.apply(replacementMap);
  LOG_DEBUG(orphanedNodeRemover.getCompletedStatusMessage());
  OsmMapWriterFactory::writeDebugMap(
    replacementMap, className(), _changesetId + "-" + replacementMap->getName() + "-after-id-sync");
}

}
