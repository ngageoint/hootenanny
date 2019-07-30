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
#include <hoot/core/ops/ElementIdRemapper.h>
#include <hoot/core/visitors/CalculateHashVisitor2.h>
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
#include <hoot/core/ops/Boundable.h>
#include <hoot/core/io/OsmMapReader.h>
#include <hoot/core/criterion/WayNodeCriterion.h>
#include <hoot/core/ops/ElementIdToVersionMapper.h>
#include <hoot/core/conflate/network/NetworkMatchCreator.h>
#include <hoot/core/algorithms/WayJoinerAdvanced.h>
#include <hoot/core/algorithms/WayJoinerBasic.h>
#include <hoot/core/visitors/SetTagValueVisitor.h>
#include <hoot/core/visitors/FilteredVisitor.h>
#include <hoot/core/criterion/TagKeyCriterion.h>
#include <hoot/core/ops/CopyMapSubsetOp.h>
#include <hoot/core/visitors/RemoveElementsVisitor.h>
#include <hoot/core/criterion/TagCriterion.h>
#include <hoot/core/io/OsmGeoJsonReader.h>

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
  // INPUT VALIDATION

  _validateInputs(input1, input2);
  std::shared_ptr<ConflatableElementCriterion> featureCrit =
    _validateFilter(featureTypeFilterClassName);
  const QString boundsStr = GeometryUtils::envelopeToConfigString(bounds);
  _parseConfigOpts(lenientBounds, featureTypeFilterClassName, boundsStr);
  // TODO: add error checking for OsmApiDbReader ref input that throws if any of the feature
  // versions are less than one (this would eventually also be applied to OsmApiReader and
  // OverpassReader as well)

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
  // Keep a mapping of the original ref element ids to versions, as we'll need the original
  // versions later.
  const QMap<ElementId, long> refIdToVersionMappings = _getIdToVersionMappings(refMap);
  if (lenientBounds && _isLinearCrit(featureTypeFilterClassName))
  {
    // If we have a lenient bounds requirement and linear features, we need to exclude all ways
    // outside of the bounds but immediately connected to a way crossing the bounds from deletion.
    _addDeleteExclusionTags(refMap);
  }
  // Prune down the elements to just the feature types specified by the filter.
  _filterFeatures(refMap, featureCrit, "ref-after-type-pruning");

  // Load the sec dataset and crop to the specified aoi.
  OsmMapPtr secMap = _loadSecMap(input2);
  // Prune down the elements to just the feature types specified by the filter.
  _filterFeatures(secMap, featureCrit, "sec-after-type-pruning");

  // COOKIE CUT

  // Cut the secondary data out of the reference data.
  OsmMapPtr cookieCutRefMap = _getCookieCutMap(refMap, secMap);
  // Renumber the relations in the sec map, as they could have ID overlap with those in the cookie
  // cut ref map at this point. This is due to the fact that they have been modified independently
  // of each other during cropping, which can create new relations.
  // TODO: Is this actually needed?
  _remapRelationIds(secMap, cookieCutRefMap->getIdGeneratorSp());
  // Combine the cookie cut map back with the secondary map, so we can conflate it with the ref map.
  _combineMaps(cookieCutRefMap, secMap, false, "combined-before-conflation");
  secMap.reset();

  // CONFLATE

  // Conflate the cookie cut ref map with the cropped sec map.
  OsmMapPtr conflatedMap = cookieCutRefMap;
  // TODO: do something with reviews
  _conflate(conflatedMap, lenientBounds);
  if (_isLinearCrit(featureTypeFilterClassName))
  {
    // Snap secondary features back to reference features if dealing with linear features where
    // ref features may have been cut along the bounds.
    _snapUnconnectedWays(
      conflatedMap, "Input2;Conflated", "Input1;Conflated", featureTypeFilterClassName, false,
      "conflated-snapped-sec-to-ref-1");

    // After snapping, perform joining to prevent unnecessary create/delete statements for the ref
    // data in the resulting changeset and generate modify statements instead.
    ReplacementSnappedWayJoiner(refIdToVersionMappings).join(conflatedMap);
  }

  // PRE-CHANGESET DERIVATION DATA PREP

  OsmMapPtr immediatelyConnectedOutOfBoundsWays;
  if (lenientBounds && _isLinearCrit(featureTypeFilterClassName))
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
    _isLinearCrit(featureTypeFilterClassName), "ref-cropped-for-changeset");
  _cropMapForChangesetDerivation(
    conflatedMap, bounds, _changesetSecKeepEntireCrossingBounds, _changesetSecKeepOnlyInsideBounds,
    _isLinearCrit(featureTypeFilterClassName), "sec-cropped-for-changeset");
  if (lenientBounds && _isLinearCrit(featureTypeFilterClassName))
  {
    // The non-strict way replacement workflow benefits from a second snapping run right before
    // changeset derivation due to there being ways connected to replacement ways that fall
    // completely outside of the bounds. Joining after this snapping caused changeset errors with
    // some datasets and hasn't seem to be needed for now...so skipping it.
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
  if (!ConfigOptions().getChangesetAllowDeletingReferenceFeaturesOutsideBounds())
  {
    // If we're not allowing the changeset deriver to generate delete statements for reference
    // features outside of the bounds, we need to mark all corresponding ref ways with a custom
    // tag that will cause the deriver to skip deleting them.
    _excludeFeaturesFromDeletion(refMap, boundsStr);
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
  std::shared_ptr<ConflatableElementCriterion> featureCrit =
    std::dynamic_pointer_cast<ConflatableElementCriterion>(
      std::shared_ptr<ElementCriterion>(
        Factory::getInstance().constructObject<ElementCriterion>(featureTypeFilterClassName)));
  if (!featureCrit)
  {
    throw IllegalArgumentException("Invalid feature type filter: " + featureTypeFilterClassName);
  }
  return featureCrit;
}

OsmMapPtr ChangesetReplacementCreator::_loadRefMap(const QString& input)
{
  // TODO: the crop config options use here for the data loading could be removed by passing
  // bounds options into IoUtils::loadMap instead (?)
  conf().set(
    ConfigOptions::getConvertBoundingBoxKeepEntireFeaturesCrossingBoundsKey(),
    _convertRefKeepEntireCrossingBounds);
  conf().set(
    ConfigOptions::getConvertBoundingBoxKeepOnlyFeaturesInsideBoundsKey(),
    _convertRefKeepOnlyInsideBounds);
  conf().set(
    ConfigOptions::getConvertBoundingBoxKeepImmediateConnectedWaysOutsideBoundsKey(),
    _convertRefKeepImmediateConnectedWaysOutsideBounds);
  OsmMapPtr refMap(new OsmMap());
  refMap->setName("ref");
  IoUtils::loadMap(refMap, input, true, Status::Unknown1);
  OsmMapWriterFactory::writeDebugMap(refMap, "ref-after-cropped-load");
  return refMap;
}

QMap<ElementId, long> ChangesetReplacementCreator::_getIdToVersionMappings(const OsmMapPtr& map)
{
  LOG_DEBUG("Recording ID to version mappings for: " << map->getName() << "...");
  ElementIdToVersionMapper idToVersionMapper;
  LOG_DEBUG(idToVersionMapper.getInitStatusMessage());
  idToVersionMapper.apply(map);
  LOG_DEBUG(idToVersionMapper.getCompletedStatusMessage());
  const QMap<ElementId, long> idToVersionMappings = idToVersionMapper.getMappings();
  LOG_VARD(idToVersionMappings);
  return idToVersionMappings;
}

void ChangesetReplacementCreator::_addDeleteExclusionTags(OsmMapPtr& map)
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
    childAddTagCrit, MetadataTags::HootChangeExcludeDelete(), "yes");
  LOG_DEBUG(childDeletionExcludeTagOp.getInitStatusMessage());
  childDeletionExcludeTagOp.apply(map);
  LOG_DEBUG(childDeletionExcludeTagOp.getCompletedStatusMessage());

  OsmMapWriterFactory::writeDebugMap(map, map->getName() + "-after-delete-exclusion-tagging");
}

OsmMapPtr ChangesetReplacementCreator::_loadSecMap(const QString& input)
{
  conf().set(
    ConfigOptions::getConvertBoundingBoxKeepEntireFeaturesCrossingBoundsKey(),
    _convertSecKeepEntireCrossingBounds);
  conf().set(
    ConfigOptions::getConvertBoundingBoxKeepOnlyFeaturesInsideBoundsKey(),
    _convertSecKeepOnlyInsideBounds);
  conf().set(
    ConfigOptions::getConvertBoundingBoxKeepImmediateConnectedWaysOutsideBoundsKey(), false);
  OsmMapPtr secMap(new OsmMap());
  secMap->setName("sec");
  IoUtils::loadMap(secMap, input, false, Status::Unknown2);
  OsmMapWriterFactory::writeDebugMap(secMap, "sec-after-cropped-load");
  return secMap;
}

void ChangesetReplacementCreator::_filterFeatures(
  OsmMapPtr& map, const std::shared_ptr<ConflatableElementCriterion>& featureCrit,
  const QString& debugFileName)
{
  LOG_DEBUG("Filtering features for: " << map->getName() << " based on input filter...");
  RemoveElementsVisitor elementPruner(true);
  elementPruner.addCriterion(featureCrit);
  elementPruner.setRecursive(true);
  LOG_DEBUG(elementPruner.getInitStatusMessage());
  map->visitRw(elementPruner);
  LOG_DEBUG(elementPruner.getCompletedStatusMessage());
  OsmMapWriterFactory::writeDebugMap(map, debugFileName);
}

OsmMapPtr ChangesetReplacementCreator::_getCookieCutMap(OsmMapPtr doughMap, OsmMapPtr cutterMap)
{
  // Generate a cutter shape based on the cropped secondary map.

  LOG_DEBUG("Generating cutter shape map from: " << cutterMap->getName() << "...");
  OsmMapPtr cutterShapeOutlineMap = AlphaShapeGenerator(1000.0, 0.0).generateMap(cutterMap);
  // not exactly sure yet why this needs to be done
  MapProjector::projectToWgs84(cutterShapeOutlineMap);
  OsmMapWriterFactory::writeDebugMap(cutterShapeOutlineMap, "cutter-shape");

  // Cookie cut the shape of the cutter shape map out of the cropped ref map.

  LOG_DEBUG("Cookie cutting cutter shape out of: " << doughMap->getName() << "...");
  // TODO: the config options here could be removed by passing in a pre-configured MapCropper to
  // CookieCutter.
  conf().set(
    ConfigOptions::getCropKeepEntireFeaturesCrossingBoundsKey(), _cropKeepEntireCrossingBounds);
  conf().set(
    ConfigOptions::getCropKeepOnlyFeaturesInsideBoundsKey(), _cropKeepOnlyInsideBounds);
  OsmMapPtr cookieCutMap(new OsmMap(doughMap));
  cookieCutMap->setName("cookie-cut");
  CookieCutter(false, 0.0).cut(cutterShapeOutlineMap, cookieCutMap);
  MapProjector::projectToWgs84(cookieCutMap); // not exactly sure yet why this needs to be done
  OsmMapWriterFactory::writeDebugMap(cookieCutMap, "cookie-cut");

  return cookieCutMap;
}

void ChangesetReplacementCreator::_remapRelationIds(
  OsmMapPtr& map, const std::shared_ptr<IdGenerator>& idGenerator)
{
  LOG_DEBUG("Remapping secondary map relation IDs for: " << map->getName() << "...");
  ElementIdRemapper relationIdRemapper;
  relationIdRemapper.setIdGen(idGenerator);
  relationIdRemapper.setRemapNodes(false);
  relationIdRemapper.setRemapRelations(true);
  relationIdRemapper.setRemapWays(false);
  LOG_DEBUG(relationIdRemapper.getInitStatusMessage());
  relationIdRemapper.apply(map);
  LOG_DEBUG(relationIdRemapper.getCompletedStatusMessage());
  OsmMapWriterFactory::writeDebugMap(map, map->getName() + "-relation-ids-remapped");
}

void ChangesetReplacementCreator::_addNodeHashes(OsmMapPtr& map)
{
  LOG_DEBUG("Adding hashes for node comparisons to: " << map->getName() << "...");
  CalculateHashVisitor2 hashVis;
  map->visitRw(hashVis);
}

void ChangesetReplacementCreator::_combineMaps(OsmMapPtr& map1, OsmMapPtr& map2,
                                               const bool throwOutDupes,
                                               const QString& debugFileName)
{
  LOG_VART(map1.get());
  LOG_VART(map2.get());
  LOG_DEBUG("Combining maps: " << map1->getName() << " and " << map2->getName() << "...");

  // Add node hashes so we can append the maps together (needed for element comparison during map
  // appending).
  _addNodeHashes(map1);
  _addNodeHashes(map2);

  MapProjector::projectToWgs84(map1);
  MapProjector::projectToWgs84(map2);   // not exactly sure yet why this needs to be done

  map1->append(map2, throwOutDupes);

  OsmMapWriterFactory::writeDebugMap(map1, debugFileName);
}

void ChangesetReplacementCreator::_conflate(OsmMapPtr& map, const bool lenientBounds)
{
  map->setName("conflated");
  LOG_DEBUG(
    "Conflating the cookie cut reference map with the secondary map into " << map->getName() <<
    "...");

  // TODO: Should we drop the default post conflate way joining here, since we're doing it later
  // again if the source is linear and the bounds isn't lenient?
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

  OsmMapWriterFactory::writeDebugMap(map, debugFileName);
}

OsmMapPtr ChangesetReplacementCreator::_getImmediatelyConnectedOutOfBoundsWays(
  const ConstOsmMapPtr& map)
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
  CopyMapSubsetOp wayCopier(map, copyCrit);
  OsmMapPtr connectedWays(new OsmMap());
  connectedWays->setName(outputMapName);
  wayCopier.apply(connectedWays);
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
          new TagCriterion(MetadataTags::HootSnappedWayNode(), "snapped_way")))));
  removeVis.setChainCriteria(true);
  removeVis.setRecursive(true);
  LOG_DEBUG(removeVis.getInitStatusMessage());
  map->visitRw(removeVis);
  LOG_DEBUG(removeVis.getCompletedStatusMessage());
  OsmMapWriterFactory::writeDebugMap(map, map->getName() + "-unsnapped-removed");
}

void ChangesetReplacementCreator::_excludeFeaturesFromDeletion(OsmMapPtr& map,
                                                               const QString& boundsStr)
{
  LOG_DEBUG(
    "Marking reference features in: " << map->getName() << " for exclusion from deletion...");

  conf().set(ConfigOptions::getInBoundsCriterionStrictKey(), _inBoundsStrict);
  // TODO: let's pass the bounds into the constructor instead
  conf().set(ConfigOptions::getInBoundsCriterionBoundsKey(), boundsStr);
  std::shared_ptr<InBoundsCriterion> boundsCrit(new InBoundsCriterion());
  boundsCrit->setOsmMap(map.get());
  std::shared_ptr<NotCriterion> notInBoundsCrit(new NotCriterion(boundsCrit));
  std::shared_ptr<ChainCriterion> elementCrit(
    new ChainCriterion(std::shared_ptr<WayCriterion>(new WayCriterion()), notInBoundsCrit));

  RecursiveSetTagValueOp tagSetter(elementCrit, MetadataTags::HootChangeExcludeDelete(), "yes");
  LOG_DEBUG(tagSetter.getInitStatusMessage());
  tagSetter.apply(map);
  LOG_DEBUG(tagSetter.getCompletedStatusMessage());

  OsmMapWriterFactory::writeDebugMap(map, map->getName() + "-after-delete-exclude-tags");
}

bool ChangesetReplacementCreator::_isNetworkConflate() const
{
  return
    ConfigOptions().getMatchCreators().contains(
      QString::fromStdString(NetworkMatchCreator::className()));
}

// hardcode these geometry type identification methods for now; only one of the following should
// return true for any input; TODO: later can come up with a more extensible solution

bool ChangesetReplacementCreator::_isPointCrit(const QString& critClassName) const
{
  return critClassName.contains("PoiCriterion");
}

bool ChangesetReplacementCreator::_isLinearCrit(const QString& critClassName) const
{
  return
    critClassName.contains("HighwayCriterion") ||
    critClassName.contains("LinearWaterwayCriterion") ||
    critClassName.contains("PowerLineCriterion") ||
    critClassName.contains("RailwayCriterion");
}

bool ChangesetReplacementCreator::_isPolyCrit(const QString& critClassName) const
{
  return
    critClassName.contains("AreaCriterion") ||
    critClassName.contains("BuildingCriterion");
}

void ChangesetReplacementCreator::_parseConfigOpts(const bool lenientBounds,
                                                   const QString& critClassName,
                                                   const QString& boundsStr)
{
  // changeset and db opts should have already been set when calling this command

  // global opts

  conf().set(ConfigOptions::getChangesetXmlWriterAddTimestampKey(), false);
  conf().set(ConfigOptions::getReaderAddSourceDatetimeKey(), false);
  conf().set(ConfigOptions::getWriterIncludeCircularErrorTagsKey(), false);

  // dataset specific opts

  // TODO: Hopefully, these three sets of bounds options can be collapsed down to at least two.

  // These don't change between scenarios.
  _convertRefKeepOnlyInsideBounds = false;
  _cropKeepOnlyInsideBounds = false;
  _changesetRefKeepOnlyInsideBounds = false;

  // only one of these should ever be true

  if (_isPointCrit(critClassName))
  {
    if (lenientBounds)
    {
      LOG_WARN("--lenient-bounds option ignored with point datasets.");
    }

    _convertRefKeepEntireCrossingBounds = false;
    _convertRefKeepImmediateConnectedWaysOutsideBounds = false;
    _convertSecKeepEntireCrossingBounds = false;
    _convertSecKeepOnlyInsideBounds = false;
    _cropKeepEntireCrossingBounds = false;
    _changesetRefKeepEntireCrossingBounds = false;
    _changesetSecKeepEntireCrossingBounds = false;
    _changesetSecKeepOnlyInsideBounds = true;
    _changesetAllowDeletingRefOutsideBounds = true;
    _inBoundsStrict = false;
  }
  else if (_isLinearCrit(critClassName))
  {
    if (lenientBounds)
    {
      _convertRefKeepEntireCrossingBounds = true;
      _convertRefKeepImmediateConnectedWaysOutsideBounds = true;
      _convertSecKeepEntireCrossingBounds = true;
      _convertSecKeepOnlyInsideBounds = false;
      _cropKeepEntireCrossingBounds = false;
      _changesetRefKeepEntireCrossingBounds = true;
      _changesetSecKeepEntireCrossingBounds = true;
      _changesetSecKeepOnlyInsideBounds = false;
      _changesetAllowDeletingRefOutsideBounds = true;
      _inBoundsStrict = false;
    }
    else
    {
      _convertRefKeepEntireCrossingBounds = true;
      _convertRefKeepImmediateConnectedWaysOutsideBounds = false;
      _convertSecKeepEntireCrossingBounds = false;
      _convertSecKeepOnlyInsideBounds = false;
      _cropKeepEntireCrossingBounds = false;
      _changesetRefKeepEntireCrossingBounds = true;
      _changesetSecKeepEntireCrossingBounds = true;
      _changesetSecKeepOnlyInsideBounds = false;
      _changesetAllowDeletingRefOutsideBounds = false;
      _inBoundsStrict = false;
    }
  }
  else if (_isPolyCrit(critClassName))
  {
    if (lenientBounds)
    {
      _convertRefKeepEntireCrossingBounds = true;
      _convertRefKeepImmediateConnectedWaysOutsideBounds = false;
      _convertSecKeepEntireCrossingBounds = true;
      _convertSecKeepOnlyInsideBounds = false;
      _cropKeepEntireCrossingBounds = true;
      _changesetRefKeepEntireCrossingBounds = true;
      _changesetSecKeepEntireCrossingBounds = true;
      _changesetSecKeepOnlyInsideBounds = false;
      _changesetAllowDeletingRefOutsideBounds = true;
      _inBoundsStrict = false;
    }
    else
    {
      _convertRefKeepEntireCrossingBounds = true;
      _convertRefKeepImmediateConnectedWaysOutsideBounds = false;
      _convertSecKeepEntireCrossingBounds = false;
      _convertSecKeepOnlyInsideBounds = true;
      _cropKeepEntireCrossingBounds = true;
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
    throw IllegalArgumentException("TODO");
  }

  conf().set(ConfigOptions::getConvertBoundingBoxKey(), boundsStr);
  conf().set(
    ConfigOptions::getChangesetAllowDeletingReferenceFeaturesOutsideBoundsKey(),
    _changesetAllowDeletingRefOutsideBounds);

  LOG_VARD(_convertRefKeepEntireCrossingBounds);
  LOG_VARD(_convertRefKeepOnlyInsideBounds);
  LOG_VARD(_convertRefKeepImmediateConnectedWaysOutsideBounds);
  LOG_VARD(_convertSecKeepEntireCrossingBounds);
  LOG_VARD(_convertSecKeepOnlyInsideBounds);
  LOG_VARD(_cropKeepEntireCrossingBounds);
  LOG_VARD(_cropKeepOnlyInsideBounds);
  LOG_VARD(_changesetRefKeepEntireCrossingBounds);
  LOG_VARD(_changesetRefKeepOnlyInsideBounds);
  LOG_VARD(_changesetSecKeepEntireCrossingBounds);
  LOG_VARD(_changesetSecKeepOnlyInsideBounds);
  LOG_VARD(_changesetAllowDeletingRefOutsideBounds);
  LOG_VARD(_inBoundsStrict);
}

}
