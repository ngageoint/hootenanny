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
#include "ChangesetReplacementCreator.h"

// Hoot
#include <hoot/core/util/GeometryUtils.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/IoUtils.h>
#include <hoot/core/visitors/RemoveElementsVisitor.h>
#include <hoot/core/algorithms/alpha-shape/AlphaShapeGenerator.h>
#include <hoot/core/conflate/CookieCutter.h>
#include <hoot/core/conflate/UnifyingConflator.h>
#include <hoot/core/ops/UnconnectedWaySnapper.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/algorithms/ReplacementSnappedWayJoiner.h>
#include <hoot/core/ops/NamedOp.h>
#include <hoot/core/visitors/RemoveUnknownVisitor.h>
#include <hoot/core/ops/MapCropper.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/ops/RecursiveSetTagValueOp.h>
#include <hoot/core/criterion/InBoundsCriterion.h>
#include <hoot/core/criterion/ChainCriterion.h>
#include <hoot/core/criterion/NotCriterion.h>
#include <hoot/core/criterion/ElementTypeCriterion.h>
#include <hoot/core/ops/SuperfluousNodeRemover.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/util/Boundable.h>
#include <hoot/core/io/OsmMapReader.h>
#include <hoot/core/criterion/WayNodeCriterion.h>
#include <hoot/core/ops/ElementIdToVersionMapper.h>
#include <hoot/core/conflate/network/NetworkMatchCreator.h>
#include <hoot/core/algorithms/WayJoinerAdvanced.h>
#include <hoot/core/algorithms/WayJoinerBasic.h>
#include <hoot/core/visitors/SetTagValueVisitor.h>
#include <hoot/core/visitors/FilteredVisitor.h>
#include <hoot/core/criterion/TagKeyCriterion.h>
#include <hoot/core/visitors/RemoveElementsVisitor.h>
#include <hoot/core/criterion/TagCriterion.h>
#include <hoot/core/io/OsmGeoJsonReader.h>
#include <hoot/core/ops/WayJoinerOp.h>
#include <hoot/core/visitors/ApiTagTruncateVisitor.h>
#include <hoot/core/visitors/ElementCountVisitor.h>
#include <hoot/core/criterion/AttributeValueCriterion.h>
#include <hoot/core/elements/OsmUtils.h>

namespace hoot
{

ChangesetReplacementCreator::ChangesetReplacementCreator(const bool printStats,
                                                         const QString osmApiDbUrl)
{
  _changesetCreator.reset(new ChangesetCreator(printStats, osmApiDbUrl));
}

void ChangesetReplacementCreator::create(
  const QString& input1, const QString& input2, const geos::geom::Envelope& bounds,
  const QString& featureTypeFilterClassName, const bool lenientBounds, const QString& output)
{
  // INPUT VALIDATION AND SETUP

  _validateInputs(input1, input2);
  std::shared_ptr<ConflatableElementCriterion> featureFilter =
    _validateFilter(featureTypeFilterClassName);
  const bool isLinearCrit =
    featureFilter->getGeometryType() == ConflatableElementCriterion::ConflatableGeometryType::Line;
  const QString boundsStr = GeometryUtils::envelopeToConfigString(bounds);
  _parseConfigOpts(lenientBounds, featureFilter, boundsStr);

  const int maxFilePrintLength = ConfigOptions().getProgressVarPrintLengthMax();
  QString lenientStr = "Bounds calculation is ";
  if (! lenientBounds)
  {
    lenientStr += "not ";
  }
  lenientStr += "lenient.";
  LOG_INFO(
    "Deriving replacement output changeset: ..." << output.right(maxFilePrintLength) <<
    " from inputs: ..." << input1.right(maxFilePrintLength) + " and ..." <<
    input2.right(maxFilePrintLength) << ", with filter: " << featureTypeFilterClassName <<
    ", at bounds: " << boundsStr << ". " << lenientStr);

  // DATA LOAD AND INITIAL PREP

  // Load the ref dataset and crop to the specified aoi.

  OsmMapPtr refMap = _loadRefMap(input1);

  // We want to alert the user to the fact their ref versions *could* be being populated incorectly
  // to avoid difficulties during changeset application at the end. Its likely if they are
  // incorrect at this point the changeset derivation will fail at the end anyway, but let's warn
  // now to give the chance to back out earlier.

  const int numberOfRefElementsWithVersionLessThan1 = _versionLessThanOneCount(refMap);
  if (numberOfRefElementsWithVersionLessThan1 > 0)
  {
    LOG_WARN(
      StringUtils::formatLargeNumber(numberOfRefElementsWithVersionLessThan1) << " features in " <<
      "the reference map have a version less than one. This could lead to difficulties when " <<
      "applying the resulting changeset back to an authoritative data store. Are the versions " <<
      "on the features being populated correctly?")
  }

  // Keep a mapping of the original ref element ids to versions, as we'll need the original
  // versions later.

  const QMap<ElementId, long> refIdToVersionMappings = _getIdToVersionMappings(refMap);
  if (lenientBounds && isLinearCrit)
  {
    // If we have a lenient bounds requirement and linear features, we need to exclude all ways
    // outside of the bounds but immediately connected to a way crossing the bounds from deletion.
    _addChangesetDeleteExclusionTags(refMap);
  }

  // Prune down the elements to just the feature types specified by the filter.

  _filterFeatures(refMap, featureFilter, "ref-after-type-pruning");

  // Load the sec dataset and crop to the specified aoi.

  OsmMapPtr secMap = _loadSecMap(input2);

  // Prune down the elements to just the feature types specified by the filter.

  _filterFeatures(secMap, featureFilter, "sec-after-type-pruning");

  // COOKIE CUT

  // Cut the secondary data out of the reference data.

  OsmMapPtr cookieCutRefMap = _getCookieCutMap(refMap, secMap);

  // At one point it was necessary to renumber the relations in the sec map, as they could have ID
  // overlap with those in the cookie cut ref map at this point. It seemed that this was due to the
  // fact that relations in the two maps had been added independently of each other during cropping.
  // However, after some refactoring this doesn't seem to be the case anymore. If we run into this
  // situation again, we can go back in the history to resurrect the use of the ElementIdRemapper
  // for relations here, which has since been removed from the codebase.

  // Combine the cookie cut map back with the secondary map, so we can conflate it with the ref map.

  _combineMaps(cookieCutRefMap, secMap, false, "combined-before-conflation");
  secMap.reset();

  // CONFLATE

  // Conflate the cookie cut ref map with the cropped sec map.

  OsmMapPtr conflatedMap = cookieCutRefMap;
  // TODO: do something with reviews - #3361
  _conflate(conflatedMap, lenientBounds);

  if (isLinearCrit)
  {
    // Snap secondary features back to reference features if dealing with linear features where
    // ref features may have been cut along the bounds. We're being lenient here by snapping
    // secondary to reference *and* allowing conflated data to be snapped to either dataset.

    _snapUnconnectedWays(
      conflatedMap, "Input2;Conflated", "Input1;Conflated", featureTypeFilterClassName, false,
      "conflated-snapped-sec-to-ref-1");

    // After snapping, perform joining to prevent unnecessary create/delete statements for the ref
    // data in the resulting changeset and generate modify statements instead.

    ReplacementSnappedWayJoiner(refIdToVersionMappings).join(conflatedMap);
    LOG_VARD(MapProjector::toWkt(conflatedMap->getProjection()));
  }

  // PRE-CHANGESET DERIVATION DATA PREP

  OsmMapPtr immediatelyConnectedOutOfBoundsWays;
  if (lenientBounds && isLinearCrit)
  {
    // If we're conflating linear features with the lenient bounds requirement, copy the
    // immediately connected out of bounds ways to a new temp map. We'll lose those ways once we
    // crop in preparation for changeset derivation. If we don't introduce them back during
    // changeset derivation, they may not end up being snapped back to the replacement data.

    immediatelyConnectedOutOfBoundsWays = _getImmediatelyConnectedOutOfBoundsWays(refMap);
  }
  // Crop the ref and conflated maps appropriately for changeset derivation.

  _cropMapForChangesetDerivation(
    refMap, bounds, _changesetRefKeepEntireCrossingBounds, _changesetRefKeepOnlyInsideBounds,
    isLinearCrit, "ref-cropped-for-changeset");
  _cropMapForChangesetDerivation(
    conflatedMap, bounds, _changesetSecKeepEntireCrossingBounds, _changesetSecKeepOnlyInsideBounds,
    isLinearCrit, "sec-cropped-for-changeset");
  if (lenientBounds && isLinearCrit)
  {
    // The non-strict way replacement workflow benefits from a second snapping run right before
    // changeset derivation due to there being ways connected to replacement ways that fall
    // completely outside of the bounds. However, joining after this snapping caused changeset
    // errors with some datasets and hasn't seem to be needed for now...so skipping it. Note that
    // we're being as lenient as possible with the snapping here, allowing basically anything to
    // join to anything else...could end up causing problems...but we'll go with it for now.

    _snapUnconnectedWays(
      conflatedMap, "Input2;Conflated;Input1", "Input1;Conflated;Input2",
      featureTypeFilterClassName, false, "conflated-snapped-sec-to-ref-2");

    // Combine the conflated map with the immediately connected out of bounds ways.

    _combineMaps(
      conflatedMap, immediatelyConnectedOutOfBoundsWays, true, "conflated-connected-combined");

    // Snap only the connected ways to other ways in the conflated map. Mark the ways that were
    // snapped, as we'll need that info in the next step.

    _snapUnconnectedWays(
      conflatedMap, "Input1", "Input1", featureTypeFilterClassName, true,
      "conflated-snapped-immediately-connected-out-of-bounds");

    // Remove any ways that weren't snapped.

    _removeUnsnappedImmediatelyConnectedOutOfBoundsWays(conflatedMap);

    // Copy the connected ways back into the ref map as well, so the changeset will derive
    // properly.

    _combineMaps(refMap, immediatelyConnectedOutOfBoundsWays, true, "ref-connected-combined");

    immediatelyConnectedOutOfBoundsWays.reset();
  }
  if (!ConfigOptions().getChangesetReplacementAllowDeletingReferenceFeaturesOutsideBounds())
  {
    // If we're not allowing the changeset deriver to generate delete statements for reference
    // features outside of the bounds, we need to mark all corresponding ref ways with a custom
    // tag that will cause the deriver to skip deleting them.

    _excludeFeaturesFromChangesetDeletion(refMap, boundsStr);
  }

  // CHANGESET DERIVATION

  // Derive a changeset between the ref and conflated maps that completely replaces ref features
  // with secondary features within the bounds and write it out.

  _changesetCreator->create(refMap, conflatedMap, output);
}

void ChangesetReplacementCreator::_validateInputs(const QString& input1, const QString& input2)
{
  // Fail if the reader that supports either input doesn't implement Boundable.
  std::shared_ptr<Boundable> boundable =
    std::dynamic_pointer_cast<Boundable>(OsmMapReaderFactory::createReader(input1));
  if (!boundable)
  {
    throw IllegalArgumentException("Reader for " + input1 + " must implement Boundable.");
  }
  boundable = std::dynamic_pointer_cast<Boundable>(OsmMapReaderFactory::createReader(input2));
  if (!boundable)
  {
    throw IllegalArgumentException("Reader for " + input2 + " must implement Boundable.");
  }

  // Fail for GeoJSON - GeoJSON coming from Overpass does not have way nodes, so their versions
  // are lost when new way nodes are added to existing ways. For that reason, we can't support it
  // (or at least not sure how to yet).
  OsmGeoJsonReader geoJsonReader;
  if (geoJsonReader.isSupported(input1) || geoJsonReader.isSupported(input2))
  {
    throw IllegalArgumentException("GeoJSON inputs are not supported.");
  }
}

std::shared_ptr<ConflatableElementCriterion> ChangesetReplacementCreator::_validateFilter(
  const QString& featureTypeFilterClassName)
{
  // Fail if a filter with an unconflatable feature type was specified.
  std::shared_ptr<ConflatableElementCriterion> featureFilter =
    std::dynamic_pointer_cast<ConflatableElementCriterion>(
      std::shared_ptr<ElementCriterion>(
        Factory::getInstance().constructObject<ElementCriterion>(featureTypeFilterClassName)));
  if (!featureFilter)
  {
    throw IllegalArgumentException("Invalid feature type filter: " + featureTypeFilterClassName);
  }
  return featureFilter;
}

OsmMapPtr ChangesetReplacementCreator::_loadRefMap(const QString& input)
{
  conf().set(
    ConfigOptions::getConvertBoundingBoxKeepEntireFeaturesCrossingBoundsKey(),
    _loadRefKeepEntireCrossingBounds);
  conf().set(
    ConfigOptions::getConvertBoundingBoxKeepOnlyFeaturesInsideBoundsKey(),
    _loadRefKeepOnlyInsideBounds);
  conf().set(
    ConfigOptions::getConvertBoundingBoxKeepImmediatelyConnectedWaysOutsideBoundsKey(),
    _loadRefKeepImmediateConnectedWaysOutsideBounds);
  OsmMapPtr refMap(new OsmMap());
  refMap->setName("ref");
  IoUtils::loadMap(refMap, input, true, Status::Unknown1);
  LOG_VARD(MapProjector::toWkt(refMap->getProjection()));
  OsmMapWriterFactory::writeDebugMap(refMap, "ref-after-cropped-load");
  return refMap;
}

int ChangesetReplacementCreator::_versionLessThanOneCount(const OsmMapPtr& map) const
{
  std::shared_ptr<AttributeValueCriterion> attrCrit(
    new AttributeValueCriterion(
      ElementAttributeType(ElementAttributeType::Version), 1, NumericComparisonType::LessThan));
  return
    (int)FilteredVisitor::getStat(
      attrCrit, std::shared_ptr<ElementCountVisitor>(new ElementCountVisitor()), map);
}

QMap<ElementId, long> ChangesetReplacementCreator::_getIdToVersionMappings(
  const OsmMapPtr& map) const
{
  LOG_DEBUG("Recording ID to version mappings for: " << map->getName() << "...");
  ElementIdToVersionMapper idToVersionMapper;
  LOG_DEBUG(idToVersionMapper.getInitStatusMessage());
  idToVersionMapper.apply(map);
  LOG_DEBUG(idToVersionMapper.getCompletedStatusMessage());
  const QMap<ElementId, long> idToVersionMappings = idToVersionMapper.getMappings();
  LOG_VART(idToVersionMappings);
  return idToVersionMappings;
}

void ChangesetReplacementCreator::_addChangesetDeleteExclusionTags(OsmMapPtr& map)
{
  LOG_DEBUG(
    "Setting connected way features outside of bounds to be excluded from deletion for: " <<
    map->getName() << "...");

  // Add the changeset deletion exclusion tag to all connected ways previously tagged upon load.

  SetTagValueVisitor addTagVis(MetadataTags::HootChangeExcludeDelete(), "yes");
  LOG_DEBUG(addTagVis.getInitStatusMessage());
  ChainCriterion addTagCrit(
    std::shared_ptr<WayCriterion>(new WayCriterion()),
    std::shared_ptr<TagKeyCriterion>(
      new TagKeyCriterion(MetadataTags::HootConnectedWayOutsideBounds())));
  FilteredVisitor deleteExcludeTagVis(addTagCrit, addTagVis);
  map->visitRw(deleteExcludeTagVis);
  LOG_DEBUG(addTagVis.getCompletedStatusMessage());

  // Add the changeset deletion exclusion tag to all children of those connected ways.

  std::shared_ptr<ChainCriterion> childAddTagCrit(
    new ChainCriterion(
      std::shared_ptr<WayCriterion>(new WayCriterion()),
      std::shared_ptr<TagKeyCriterion>(
        new TagKeyCriterion(MetadataTags::HootChangeExcludeDelete()))));
  RecursiveSetTagValueOp childDeletionExcludeTagOp(
    MetadataTags::HootChangeExcludeDelete(), "yes", childAddTagCrit);
  LOG_DEBUG(childDeletionExcludeTagOp.getInitStatusMessage());
  childDeletionExcludeTagOp.apply(map);
  LOG_DEBUG(childDeletionExcludeTagOp.getCompletedStatusMessage());

  OsmMapWriterFactory::writeDebugMap(map, map->getName() + "-after-delete-exclusion-tagging");
}

OsmMapPtr ChangesetReplacementCreator::_loadSecMap(const QString& input)
{
  conf().set(
    ConfigOptions::getConvertBoundingBoxKeepEntireFeaturesCrossingBoundsKey(),
    _loadSecKeepEntireCrossingBounds);
  conf().set(
    ConfigOptions::getConvertBoundingBoxKeepOnlyFeaturesInsideBoundsKey(),
    _loadSecKeepOnlyInsideBounds);
  conf().set(
    ConfigOptions::getConvertBoundingBoxKeepImmediatelyConnectedWaysOutsideBoundsKey(), false);
  OsmMapPtr secMap(new OsmMap());
  secMap->setName("sec");
  IoUtils::loadMap(secMap, input, false, Status::Unknown2);
  LOG_VARD(MapProjector::toWkt(secMap->getProjection()));
  OsmMapWriterFactory::writeDebugMap(secMap, "sec-after-cropped-load");
  return secMap;
}

void ChangesetReplacementCreator::_filterFeatures(
  OsmMapPtr& map, const std::shared_ptr<ConflatableElementCriterion>& featureFilter,
  const QString& debugFileName)
{
  LOG_DEBUG("Filtering features for: " << map->getName() << " based on input filter...");
  RemoveElementsVisitor elementPruner(true);
  elementPruner.addCriterion(featureFilter);
  elementPruner.setRecursive(true);
  LOG_DEBUG(elementPruner.getInitStatusMessage());
  map->visitRw(elementPruner);
  LOG_DEBUG(elementPruner.getCompletedStatusMessage());
  LOG_VARD(MapProjector::toWkt(map->getProjection()));
  OsmMapWriterFactory::writeDebugMap(map, debugFileName);
}

OsmMapPtr ChangesetReplacementCreator::_getCookieCutMap(OsmMapPtr doughMap, OsmMapPtr cutterMap)
{
  LOG_VARD(MapProjector::toWkt(doughMap->getProjection()));
  LOG_VARD(MapProjector::toWkt(cutterMap->getProjection()));

  // Generate a cutter shape based on the cropped secondary map.

  LOG_DEBUG("Generating cutter shape map from: " << cutterMap->getName() << "...");
  OsmMapPtr cutterShapeOutlineMap = AlphaShapeGenerator(1000.0, 0.0).generateMap(cutterMap);
  // not exactly sure yet why this needs to be done
  MapProjector::projectToWgs84(cutterShapeOutlineMap);
  LOG_VARD(MapProjector::toWkt(cutterShapeOutlineMap->getProjection()));
  OsmMapWriterFactory::writeDebugMap(cutterShapeOutlineMap, "cutter-shape");

  // Cookie cut the shape of the cutter shape map out of the cropped ref map.

  LOG_DEBUG("Cookie cutting cutter shape out of: " << doughMap->getName() << "...");
  OsmMapPtr cookieCutMap(new OsmMap(doughMap));
  //OsmMapPtr cookieCutMap(new OsmMap(doughMap, MapProjector::createWgs84Projection()));
  LOG_VARD(MapProjector::toWkt(cookieCutMap->getProjection()));
  cookieCutMap->setName("cookie-cut");
  CookieCutter(false, 0.0, _cookieCutKeepEntireCrossingBounds, _cookieCutKeepOnlyInsideBounds)
    .cut(cutterShapeOutlineMap, cookieCutMap);
  MapProjector::projectToWgs84(cookieCutMap); // not exactly sure yet why this needs to be done
  LOG_VARD(MapProjector::toWkt(cookieCutMap->getProjection()));
  OsmMapWriterFactory::writeDebugMap(cookieCutMap, "cookie-cut");

  return cookieCutMap;
}

void ChangesetReplacementCreator::_combineMaps(OsmMapPtr& map1, OsmMapPtr& map2,
                                               const bool throwOutDupes,
                                               const QString& debugFileName)
{
  LOG_VART(map1.get());
  LOG_VART(map2.get());
  LOG_DEBUG("Combining maps: " << map1->getName() << " and " << map2->getName() << "...");

  MapProjector::projectToWgs84(map1);
  MapProjector::projectToWgs84(map2);   // not exactly sure yet why this needs to be done

  map1->append(map2, throwOutDupes);
  LOG_VARD(MapProjector::toWkt(map1->getProjection()));

  OsmMapWriterFactory::writeDebugMap(map1, debugFileName);
}

void ChangesetReplacementCreator::_conflate(OsmMapPtr& map, const bool lenientBounds)
{
  map->setName("conflated");
  LOG_DEBUG(
    "Conflating the cookie cut reference map with the secondary map into " << map->getName() <<
    "...");

  conf().set(ConfigOptions::getWayJoinerLeaveParentIdKey(), true);
  if (!lenientBounds) // not exactly sure yet why this needs to be done
  {
    conf().set(ConfigOptions::getWayJoinerKey(), WayJoinerAdvanced::className());
  }
  else
  {
    conf().set(ConfigOptions::getWayJoinerKey(), WayJoinerBasic::className());
  }
  conf().set(ConfigOptions::getWayJoinerAdvancedStrictNameMatchKey(), !_isNetworkConflate());
  NamedOp preOps(ConfigOptions().getConflatePreOps());
  preOps.apply(map);
  // TODO: restrict conflate matchers to only those relevant based on the filter?
  UnifyingConflator().apply(map);
  NamedOp postOps(ConfigOptions().getConflatePostOps());
  postOps.apply(map);
  MapProjector::projectToWgs84(map);  // conflation works in planar
  LOG_VARD(MapProjector::toWkt(map->getProjection()));
  OsmMapWriterFactory::writeDebugMap(map, "conflated");
}

void ChangesetReplacementCreator::_snapUnconnectedWays(OsmMapPtr& map, const QString& snapWayStatus,
                                                       const QString& snapToWayStatus,
                                                       const QString& featureTypeFilterClassName,
                                                       const bool markSnappedWays,
                                                       const QString& debugFileName)
{
  LOG_DEBUG("Snapping ways for map: " << map->getName() <<" ...");

  UnconnectedWaySnapper lineSnapper;
  lineSnapper.setConfiguration(conf());
  // override some of the default config
  lineSnapper.setSnapToWayStatus(snapToWayStatus);
  lineSnapper.setSnapWayStatus(snapWayStatus);
  lineSnapper.setMarkSnappedWays(markSnappedWays);
  // TODO: hack - need a way to derive the way node crit from the input feature filter crit
  lineSnapper.setWayNodeToSnapToCriterionClassName(
    QString::fromStdString(WayNodeCriterion::className()));
  lineSnapper.setWayToSnapCriterionClassName(featureTypeFilterClassName);
  lineSnapper.setWayToSnapToCriterionClassName(featureTypeFilterClassName);
  LOG_DEBUG(lineSnapper.getInitStatusMessage());
  lineSnapper.apply(map);
  LOG_DEBUG(lineSnapper.getCompletedStatusMessage());

  MapProjector::projectToWgs84(map);   // snapping works in planar
  LOG_VARD(MapProjector::toWkt(map->getProjection()));

  OsmMapWriterFactory::writeDebugMap(map, debugFileName);
}

OsmMapPtr ChangesetReplacementCreator::_getImmediatelyConnectedOutOfBoundsWays(
  const ConstOsmMapPtr& map) const
{
  const QString outputMapName = "connected-ways";
  LOG_DEBUG(
    "Copying immediately connected out of bounds ways from: " << map->getName() <<
    " to new map: " << outputMapName << "...");

  std::shared_ptr<ChainCriterion> copyCrit(
    new ChainCriterion(
      std::shared_ptr<WayCriterion>(new WayCriterion()),
      std::shared_ptr<TagKeyCriterion>(
        new TagKeyCriterion(MetadataTags::HootConnectedWayOutsideBounds()))));
  OsmMapPtr connectedWays = OsmUtils::getMapSubset(map, copyCrit);
  connectedWays->setName(outputMapName);
  LOG_VARD(MapProjector::toWkt(connectedWays->getProjection()));
  OsmMapWriterFactory::writeDebugMap(connectedWays, "connected-ways");
  return connectedWays;
}

void ChangesetReplacementCreator::_cropMapForChangesetDerivation(
  OsmMapPtr& map, const geos::geom::Envelope& bounds, const bool keepEntireFeaturesCrossingBounds,
  const bool keepOnlyFeaturesInsideBounds, const bool isLinearMap, const QString& debugFileName)
{
  LOG_DEBUG("Cropping map: " << map->getName() << " for changeset derivation...");

  MapCropper cropper(bounds);
  cropper.setKeepEntireFeaturesCrossingBounds(keepEntireFeaturesCrossingBounds);
  cropper.setKeepOnlyFeaturesInsideBounds(keepOnlyFeaturesInsideBounds);
  LOG_DEBUG(cropper.getInitStatusMessage());
  cropper.apply(map);
  LOG_DEBUG(cropper.getCompletedStatusMessage());

  // Clean up straggling nodes in that are the result of cropping. Its ok to ignore info tags when
  // dealing with only linear features, as all nodes in the data being conflated should be way nodes
  // with no information.
  SuperfluousNodeRemover::removeNodes(map, isLinearMap);

  LOG_VARD(MapProjector::toWkt(map->getProjection()));
  OsmMapWriterFactory::writeDebugMap(map, debugFileName);
}

void ChangesetReplacementCreator::_removeUnsnappedImmediatelyConnectedOutOfBoundsWays(
  OsmMapPtr& map)
{
  LOG_DEBUG(
    "Removing any immediately connected ways that were not previously snapped in: " <<
    map->getName() << "...");
  RemoveElementsVisitor removeVis;
  removeVis.addCriterion(ElementCriterionPtr(new WayCriterion()));
  removeVis.addCriterion(
    ElementCriterionPtr(new TagKeyCriterion(MetadataTags::HootConnectedWayOutsideBounds())));
  removeVis.addCriterion(
    ElementCriterionPtr(
      new NotCriterion(
        std::shared_ptr<TagCriterion>(
          new TagCriterion(MetadataTags::HootSnapped(), "snapped_way")))));
  removeVis.setChainCriteria(true);
  removeVis.setRecursive(true);
  LOG_DEBUG(removeVis.getInitStatusMessage());
  map->visitRw(removeVis);
  LOG_DEBUG(removeVis.getCompletedStatusMessage());
  LOG_VARD(MapProjector::toWkt(map->getProjection()));
  OsmMapWriterFactory::writeDebugMap(map, map->getName() + "-unsnapped-removed");
}

void ChangesetReplacementCreator::_excludeFeaturesFromChangesetDeletion(OsmMapPtr& map,
                                                                        const QString& boundsStr)
{
  LOG_DEBUG(
    "Marking reference features in: " << map->getName() << " for exclusion from deletion...");

  std::shared_ptr<InBoundsCriterion> boundsCrit(new InBoundsCriterion(_inBoundsStrict));
  boundsCrit->setBounds(GeometryUtils::envelopeFromConfigString(boundsStr));
  boundsCrit->setOsmMap(map.get());
  std::shared_ptr<NotCriterion> notInBoundsCrit(new NotCriterion(boundsCrit));
  std::shared_ptr<ChainCriterion> elementCrit(
    new ChainCriterion(std::shared_ptr<WayCriterion>(new WayCriterion()), notInBoundsCrit));

  RecursiveSetTagValueOp tagSetter(MetadataTags::HootChangeExcludeDelete(), "yes", elementCrit);
  LOG_DEBUG(tagSetter.getInitStatusMessage());
  tagSetter.apply(map);
  LOG_DEBUG(tagSetter.getCompletedStatusMessage());

  LOG_VARD(MapProjector::toWkt(map->getProjection()));
  OsmMapWriterFactory::writeDebugMap(map, map->getName() + "-after-delete-exclude-tags");
}

bool ChangesetReplacementCreator::_isNetworkConflate() const
{
  return
    ConfigOptions().getMatchCreators().contains(
      QString::fromStdString(NetworkMatchCreator::className()));
}

void ChangesetReplacementCreator::_parseConfigOpts(
  const bool lenientBounds, const std::shared_ptr<ConflatableElementCriterion>& featureFilter,
  const QString& boundsStr)
{
  // global opts

  // TODO: this doesn't seem to work during testing for some reason
//  conf().set(
//    ConfigOptions::getLogClassFilterKey(),
//    "ChangesetReplacementCreator;MapCropper;ImmediatelyConnectedOutOfBoundsWayTagger");
  // TODO: remove
  //conf().set(ConfigOptions::getDebugMapsWriteKey(), true);

  conf().set(ConfigOptions::getChangesetXmlWriterAddTimestampKey(), false);
  conf().set(ConfigOptions::getReaderAddSourceDatetimeKey(), false);
  conf().set(ConfigOptions::getWriterIncludeCircularErrorTagsKey(), false);
  conf().set(ConfigOptions::getConvertBoundingBoxKey(), boundsStr);
  // For this being enabled to have any effect,
  // convert.bounding.box.keep.immediately.connected.ways.outside.bounds must be enabled as well.
  conf().set(ConfigOptions::getConvertBoundingBoxTagImmediatelyConnectedOutOfBoundsWaysKey(), true);

  // dataset specific opts

  // These don't change between scenarios (or at least haven't needed to yet).
  _loadRefKeepOnlyInsideBounds = false;
  _cookieCutKeepOnlyInsideBounds = false;
  _changesetRefKeepOnlyInsideBounds = false;

  // only one of these should ever be true

  if (featureFilter->getGeometryType() ==
        ConflatableElementCriterion::ConflatableGeometryType::Point)
  {
    if (lenientBounds)
    {
      LOG_WARN("--lenient-bounds option ignored with point datasets.");
    }

    _loadRefKeepEntireCrossingBounds = false;
    _loadRefKeepImmediateConnectedWaysOutsideBounds = false;
    _loadSecKeepEntireCrossingBounds = false;
    _loadSecKeepOnlyInsideBounds = false;
    _cookieCutKeepEntireCrossingBounds = false;
    _changesetRefKeepEntireCrossingBounds = false;
    _changesetSecKeepEntireCrossingBounds = false;
    _changesetSecKeepOnlyInsideBounds = true;
    _changesetAllowDeletingRefOutsideBounds = true;
    _inBoundsStrict = false;
  }
  else if (featureFilter->getGeometryType() ==
             ConflatableElementCriterion::ConflatableGeometryType::Line)
  {
    if (lenientBounds)
    {
      _loadRefKeepEntireCrossingBounds = true;
      _loadRefKeepImmediateConnectedWaysOutsideBounds = true;
      _loadSecKeepEntireCrossingBounds = true;
      _loadSecKeepOnlyInsideBounds = false;
      _cookieCutKeepEntireCrossingBounds = false;
      _changesetRefKeepEntireCrossingBounds = true;
      _changesetSecKeepEntireCrossingBounds = true;
      _changesetSecKeepOnlyInsideBounds = false;
      _changesetAllowDeletingRefOutsideBounds = true;
      _inBoundsStrict = false;
    }
    else
    {
      _loadRefKeepEntireCrossingBounds = true;
      _loadRefKeepImmediateConnectedWaysOutsideBounds = false;
      _loadSecKeepEntireCrossingBounds = false;
      _loadSecKeepOnlyInsideBounds = false;
      _cookieCutKeepEntireCrossingBounds = false;
      _changesetRefKeepEntireCrossingBounds = true;
      _changesetSecKeepEntireCrossingBounds = true;
      _changesetSecKeepOnlyInsideBounds = false;
      _changesetAllowDeletingRefOutsideBounds = false;
      _inBoundsStrict = false;

      // Conflate way joining needs to happen later in the post ops for strict linear replacements.
      // Changing the default ordering of the post ops to accomodate this had detrimental effects
      // on other conflation. The best location seems to be at the end just before tag truncation.
      // would like to get rid of this...isn't a foolproof fix by any means if the conflate post
      // ops end up getting reordered for some reason.

      LOG_VARD(conf().getList(ConfigOptions::getConflatePostOpsKey()));
      QStringList conflatePostOps = conf().getList(ConfigOptions::getConflatePostOpsKey());
      conflatePostOps.removeAll(QString::fromStdString(WayJoinerOp::className()));
      const int indexOfTagTruncater =
        conflatePostOps.indexOf(QString::fromStdString(ApiTagTruncateVisitor::className()));
      conflatePostOps.insert(
        indexOfTagTruncater - 1, QString::fromStdString(WayJoinerOp::className()));
      conf().set(ConfigOptions::getConflatePostOpsKey(), conflatePostOps);
      LOG_VARD(conf().getList(ConfigOptions::getConflatePostOpsKey()));
    }
  }
  else if (featureFilter->getGeometryType() ==
             ConflatableElementCriterion::ConflatableGeometryType::Polygon)
  {
    if (lenientBounds)
    {
      _loadRefKeepEntireCrossingBounds = true;
      _loadRefKeepImmediateConnectedWaysOutsideBounds = false;
      _loadSecKeepEntireCrossingBounds = true;
      _loadSecKeepOnlyInsideBounds = false;
      _cookieCutKeepEntireCrossingBounds = true;
      _changesetRefKeepEntireCrossingBounds = true;
      _changesetSecKeepEntireCrossingBounds = true;
      _changesetSecKeepOnlyInsideBounds = false;
      _changesetAllowDeletingRefOutsideBounds = true;
      _inBoundsStrict = false;
    }
    else
    {
      _loadRefKeepEntireCrossingBounds = true;
      _loadRefKeepImmediateConnectedWaysOutsideBounds = false;
      _loadSecKeepEntireCrossingBounds = false;
      _loadSecKeepOnlyInsideBounds = true;
      _cookieCutKeepEntireCrossingBounds = true;
      _changesetRefKeepEntireCrossingBounds = true;
      _changesetSecKeepEntireCrossingBounds = false;
      _changesetSecKeepOnlyInsideBounds = true;
      _changesetAllowDeletingRefOutsideBounds = false;
      _inBoundsStrict = true;
    }
  }
  else
  {
    // shouldn't ever get here
    throw IllegalArgumentException("Invalid geometry type.");
  }

  conf().set(
    ConfigOptions::getChangesetReplacementAllowDeletingReferenceFeaturesOutsideBoundsKey(),
    _changesetAllowDeletingRefOutsideBounds);

  LOG_VARD(_loadRefKeepEntireCrossingBounds);
  LOG_VARD(_loadRefKeepOnlyInsideBounds);
  LOG_VARD(_loadRefKeepImmediateConnectedWaysOutsideBounds);
  LOG_VARD(_loadSecKeepEntireCrossingBounds);
  LOG_VARD(_loadSecKeepOnlyInsideBounds);
  LOG_VARD(_cookieCutKeepEntireCrossingBounds);
  LOG_VARD(_cookieCutKeepOnlyInsideBounds);
  LOG_VARD(_changesetRefKeepEntireCrossingBounds);
  LOG_VARD(_changesetRefKeepOnlyInsideBounds);
  LOG_VARD(_changesetSecKeepEntireCrossingBounds);
  LOG_VARD(_changesetSecKeepOnlyInsideBounds);
  LOG_VARD(_changesetAllowDeletingRefOutsideBounds);
  LOG_VARD(_inBoundsStrict);
}

}
