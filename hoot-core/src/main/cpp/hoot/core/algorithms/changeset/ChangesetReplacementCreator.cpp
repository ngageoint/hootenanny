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
#include <hoot/core/criterion/ConflatableElementCriterion.h>
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
    // TODO: break this up into smaller methods

    // Fail if a filter with an unconflatable feature type was specified.

    std::shared_ptr<ConflatableElementCriterion> featureCrit =
      std::dynamic_pointer_cast<ConflatableElementCriterion>(
        std::shared_ptr<ElementCriterion>(
          Factory::getInstance().constructObject<ElementCriterion>(featureTypeFilterClassName)));
    if (!featureCrit)
    {
      throw IllegalArgumentException("TODO");
    }

    // Fail if the reader that supports either input doesn't implement Boundable.

    std::shared_ptr<Boundable> boundable =
      std::dynamic_pointer_cast<Boundable>(OsmMapReaderFactory::createReader(input1));
    if (!boundable)
    {
      throw IllegalArgumentException("TODO");
    }
    boundable = std::dynamic_pointer_cast<Boundable>(OsmMapReaderFactory::createReader(input2));
    if (!boundable)
    {
      throw IllegalArgumentException("TODO");
    }

    const QString boundsStr = GeometryUtils::envelopeToConfigString(bounds);
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
      input2.right(maxFilePrintLength) << " at bounds: " << boundsStr << ". " << lenientStr);

    _parseConfigOpts(lenientBounds, featureTypeFilterClassName);

    // TODO: the crop config options here could be removed by passing a bounds into IoUtils::loadMap
    // instead.

    // Load the ref dataset and crop to the specified aoi.

    conf().set(ConfigOptions::getConvertBoundingBoxKey(), boundsStr);
    conf().set(
      ConfigOptions::getConvertBoundingBoxKeepEntireFeaturesCrossingBoundsKey(),
      _convertRefKeepEntireCrossingBounds);
    conf().set(
      ConfigOptions::getConvertBoundingBoxKeepOnlyFeaturesInsideBoundsKey(),
      _convertRefKeepOnlyInsideBounds);
    OsmMapPtr refMap(new OsmMap());
    IoUtils::loadMap(refMap, input1, true, Status::Unknown1);
    OsmMapWriterFactory::writeDebugMap(refMap, "ref-after-cropped-load");

    // Keep a mapping of the original ref element ids to versions, as we'll need the original
    // versions later.

    LOG_DEBUG("Recoding ref ID to version mappings...");
    ElementIdToVersionMapper refIdToVersionMapper;
    refIdToVersionMapper.apply(refMap);
    const QMap<ElementId, long> refIdToVersionMappings = refIdToVersionMapper.getMappings();
    LOG_VARD(refIdToVersionMappings);

    // Load the sec dataset and crop to the specified aoi.

    conf().set(
      ConfigOptions::getConvertBoundingBoxKeepEntireFeaturesCrossingBoundsKey(),
      _convertSecKeepEntireCrossingBounds);
    conf().set(
      ConfigOptions::getConvertBoundingBoxKeepOnlyFeaturesInsideBoundsKey(),
      _convertSecKeepOnlyInsideBounds);
    OsmMapPtr secMap(new OsmMap());
    IoUtils::loadMap(secMap, input2, false, Status::Unknown2);
    OsmMapWriterFactory::writeDebugMap(secMap, "sec-after-cropped-load");

    // Prune down the elements to just the feature types specified by the filter.

    LOG_DEBUG("Filtering features based on input filter: " << featureTypeFilterClassName << "...");
    RemoveElementsVisitor elementsRemover(true);
    elementsRemover.addCriterion(featureCrit);
    elementsRemover.setRecursive(true);
    refMap->visitRw(elementsRemover);
    secMap->visitRw(elementsRemover);
    OsmMapWriterFactory::writeDebugMap(refMap, "ref-after-type-pruning");
    OsmMapWriterFactory::writeDebugMap(secMap, "sec-after-type-pruning");

    // Generate a cutter shape based on the cropped secondary map.

    LOG_DEBUG("Generating cutter shape map from secondary input...");
    OsmMapPtr cutterShapeOutlineMap = AlphaShapeGenerator(1000.0, 0.0).generateMap(secMap);
    // not exactly sure yet why this needs to be done
    MapProjector::projectToWgs84(cutterShapeOutlineMap);
    OsmMapWriterFactory::writeDebugMap(cutterShapeOutlineMap, "cutter-shape");

    // Cookie cut the shape of the cutter shape map out of the cropped ref map.

    LOG_DEBUG("Cookie cutting cutter shape out of reference map...");
    // TODO: the config options here could be removed by passing in a pre-configured MapCropper to
    // CookieCutter.
    conf().set(
      ConfigOptions::getCropKeepEntireFeaturesCrossingBoundsKey(), _cropKeepEntireCrossingBounds);
    conf().set(
      ConfigOptions::getCropKeepOnlyFeaturesInsideBoundsKey(), _cropKeepOnlyInsideBounds);
    OsmMapPtr cookieCutMap(new OsmMap(refMap));
    CookieCutter(false, 0.0).cut(cutterShapeOutlineMap, cookieCutMap);
    MapProjector::projectToWgs84(cookieCutMap); // not exactly sure yet why this needs to be done
    OsmMapWriterFactory::writeDebugMap(cookieCutMap, "cookie-cut");

    // Renumber the relations in the sec map, as they could have ID overlap with those in the cookie
    // cut ref map at this point. This is due to the fact that they have been modified independently
    // of each other during cropping, which can create new relations.

    // TODO: Is this actually needed?
    LOG_DEBUG("Remapping secondary map relation IDs...");
    ElementIdRemapper relationIdRemapper;
    relationIdRemapper.setIdGen(cookieCutMap->getIdGeneratorSp());
    relationIdRemapper.setRemapNodes(false);
    relationIdRemapper.setRemapRelations(true);
    relationIdRemapper.setRemapWays(false);
    relationIdRemapper.apply(secMap);
    OsmMapWriterFactory::writeDebugMap(secMap, "sec-relation-ids-remapped");

    // Add node hashes so we can append the cookie cut and sec maps together (needed for element
    // comparison during map appending).

    LOG_DEBUG("Adding hashes for node comparisons...");
    CalculateHashVisitor2 hashVis;
    cookieCutMap->visitRw(hashVis);
    secMap->visitRw(hashVis);
    OsmMapWriterFactory::writeDebugMap(cookieCutMap, "cookie-cut-after-node-hashes");
    OsmMapWriterFactory::writeDebugMap(secMap, "sec-after-node-hashes");

    // Combine the cookie cut map back with the secondary map, so we can conflate it with the ref
    // map.

    MapProjector::projectToWgs84(secMap);   // not exactly sure yet why this needs to be done
    cookieCutMap->append(secMap);
    OsmMapPtr conflatedMap = cookieCutMap;
    secMap.reset();
    OsmMapWriterFactory::writeDebugMap(conflatedMap, "combined-before-conflation");

    // Conflate the cookie cut ref map with the cropped sec map.

    LOG_DEBUG("Conflating the cookie cut reference map with the secondary map...");
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
    preOps.apply(conflatedMap);
    // TODO: restrict conflate matchers to only those relevant based on the filter?
    UnifyingConflator().apply(conflatedMap);
    NamedOp postOps(ConfigOptions().getConflatePostOps());
    postOps.apply(conflatedMap);
    OsmMapWriterFactory::writeDebugMap(conflatedMap, "conflated");

    if (!lenientBounds && _isLinearCrit(featureTypeFilterClassName))
    {
      // Snap secondary features back to reference features if dealing with linear features and a
      // non-lenient bounds where ref features may have been cut along it.

      LOG_DEBUG("Snapping unconnected linear secondary features back to reference features...");
      UnconnectedWaySnapper lineSnapper;
      lineSnapper.setConfiguration(conf());
      // override some of the default config
      lineSnapper.setSnapToWayStatus("Input1;Conflated");
      lineSnapper.setSnapWayStatus("Input2;Conflated");
      // TODO: hack - need a way to derive the way node crit from the input feature filter crit
      lineSnapper.setWayNodeToSnapToCriterionClassName(
        QString::fromStdString(WayNodeCriterion::className()));
      lineSnapper.setWayToSnapCriterionClassName(featureTypeFilterClassName);
      lineSnapper.setWayToSnapToCriterionClassName(featureTypeFilterClassName);
      lineSnapper.apply(conflatedMap);
      OsmMapWriterFactory::writeDebugMap(conflatedMap, "snapped");

      // After snapping, perform joining to prevent unnecessary create/delete statements for the ref
      // data in the resulting changeset and generate modify statements instead.

      LOG_DEBUG("Rejoining features after unconnected linear feature snapping...");
      ReplacementSnappedWayJoiner(refIdToVersionMappings).join(conflatedMap);
      OsmMapWriterFactory::writeDebugMap(conflatedMap, "joined");
    }

    MapProjector::projectToWgs84(conflatedMap);  // conflation and snapping work in planar

    // Crop the original ref map appropriately for changeset derivation. Also, clean up straggling
    // nodes (maybe need to figure out how to prevent this from occurring during cropping).

    MapCropper cropper(bounds);
    LOG_DEBUG("Cropping reference map for changeset derivation...");
    cropper.setKeepEntireFeaturesCrossingBounds(_changesetRefKeepEntireCrossingBounds);
    cropper.setKeepOnlyFeaturesInsideBounds(_changesetRefKeepOnlyInsideBounds);
    cropper.apply(refMap);
    OsmMapWriterFactory::writeDebugMap(refMap, "ref-cropped-for-changeset");
    SuperfluousNodeRemover::removeNodes(refMap);
    OsmMapWriterFactory::writeDebugMap(refMap, "ref-after-orphaned-node-removal");

    // same type of cropping as previously done, but for the conflated map

    LOG_DEBUG("Cropping conflated map for changeset derivation...");
    cropper.setKeepEntireFeaturesCrossingBounds(_changesetSecKeepEntireCrossingBounds);
    cropper.setKeepOnlyFeaturesInsideBounds(_changesetSecKeepOnlyInsideBounds);
    cropper.apply(conflatedMap);
    OsmMapWriterFactory::writeDebugMap(conflatedMap, "conflated-cropped-for-changeset");
    SuperfluousNodeRemover::removeNodes(conflatedMap);
    OsmMapWriterFactory::writeDebugMap(conflatedMap, "conflated-after-orphaned-node-removal");

    // TODO: setting these two config options won't be necessary once the old multiple command tests
    // are deactivated
    conf().set(
      ConfigOptions::getChangesetAllowDeletingReferenceFeaturesOutsideBoundsKey(),
      _changesetAllowDeletingRefOutsideBounds);
    conf().set(ConfigOptions::getInBoundsCriterionStrictKey(), _inBoundsStrict);
    if (!ConfigOptions().getChangesetAllowDeletingReferenceFeaturesOutsideBounds())
    {
      // If we're not allowing the changeset deriver to generate delete statements for reference
      // features outside of the bounds, we need to mark all corresponding ref ways with a custom
      // tag that will cause the deriver to skip deleting them.

      // TODO: let's pass the bounds in the constructor instead
      conf().set("in.bounds.criterion.bounds", boundsStr);
      std::shared_ptr<InBoundsCriterion> boundsCrit(new InBoundsCriterion());
      boundsCrit->setOsmMap(refMap.get());
      std::shared_ptr<NotCriterion> notInBoundsCrit(new NotCriterion(boundsCrit));
      std::shared_ptr<ChainCriterion> elementCrit(
        new ChainCriterion(std::shared_ptr<WayCriterion>(new WayCriterion()), notInBoundsCrit));

      LOG_DEBUG("Marking reference features for exclusion from deletion...");
      RecursiveSetTagValueOp tagSetter(elementCrit, MetadataTags::HootChangeExcludeDelete(), "yes");
      tagSetter.apply(refMap);

      OsmMapWriterFactory::writeDebugMap(refMap, "ref-after-delete-exclude-tags");
    }

    // Derive a changeset between the ref and conflated maps that replaces ref features with
    // secondary features within the bounds and write it out.

    LOG_DEBUG("Deriving replacement changeset...");
    _changesetCreator->create(refMap, conflatedMap, output);
}

bool ChangesetReplacementCreator::_isNetworkConflate() const
{
  return
    ConfigOptions().getMatchCreators().contains(
      QString::fromStdString(NetworkMatchCreator::className()));
}

// hardcode these geometry type identification methods for now; only one of the following should
// return true for any input; later can come up with a more extensible solution

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
                                                   const QString& critClassName)
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

  LOG_VARD(_convertRefKeepEntireCrossingBounds);
  LOG_VARD(_convertRefKeepOnlyInsideBounds);
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
