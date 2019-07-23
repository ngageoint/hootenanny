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


namespace hoot
{

ChangesetReplacementCreator::ChangesetReplacementCreator(const bool printStats,
                                                         const QString osmApiDbUrl)
{
  _changesetCreator.reset(new ChangesetCreator(printStats, osmApiDbUrl));
}

void ChangesetReplacementCreator::create(const QString& input1, const QString& input2,
                                         const geos::geom::Envelope& bounds,
                                         const QString& featureTypeFilterClassName,
                                         const bool lenientBounds, const QString& output)
{
    std::shared_ptr<ConflatableElementCriterion> featureCrit =
      std::dynamic_pointer_cast<ConflatableElementCriterion>(
        std::shared_ptr<ElementCriterion>(
          Factory::getInstance().constructObject<ElementCriterion>(featureTypeFilterClassName)));
    if (!featureCrit)
    {
      throw IllegalArgumentException("TODO");
    }

    const QString boundsStr = GeometryUtils::envelopeToConfigString(bounds);

    const int maxFilePrintLength = ConfigOptions().getProgressVarPrintLengthMax();
    QString lenientStr = "Bounds calculation ";
    if (lenientBounds)
    {
      lenientStr += "is ";
    }
    else
    {
      lenientStr += "is not ";
    }
    lenientStr += "lenient.";
    LOG_INFO(
      "Deriving replacement output changeset: ..." << output.right(maxFilePrintLength) <<
      " from inputs: ..." << input1.right(maxFilePrintLength) + " and ..." <<
      input2.right(maxFilePrintLength) << " at bounds: " << boundsStr << ". " << lenientStr);

    _setConfigOpts(lenientBounds, featureTypeFilterClassName);

    // load each dataset separately and crop to the specified aoi

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

    conf().set(
      ConfigOptions::getConvertBoundingBoxKeepEntireFeaturesCrossingBoundsKey(),
      _convertSecKeepEntireCrossingBounds);
    conf().set(
      ConfigOptions::getConvertBoundingBoxKeepOnlyFeaturesInsideBoundsKey(),
      _convertSecKeepOnlyInsideBounds);
    OsmMapPtr secMap(new OsmMap());
    IoUtils::loadMap(secMap, input2, true, Status::Unknown2);
    OsmMapWriterFactory::writeDebugMap(secMap, "sec-after-cropped-load");

    // prune down to just the feature types specified by the filter

    LOG_DEBUG("Filtering features based on input filter: " << featureTypeFilterClassName << "...");
    RemoveElementsVisitor elementsRemover(true);
    elementsRemover.addCriterion(featureCrit);
    elementsRemover.setRecursive(true);
    refMap->visitRw(elementsRemover);
    secMap->visitRw(elementsRemover);
    OsmMapWriterFactory::writeDebugMap(refMap, "ref-after-type-pruning");
    OsmMapWriterFactory::writeDebugMap(secMap, "sec-after-type-pruning");

    // generate a cutter shape based on the cropped secondary map

    LOG_DEBUG("Generating cutter shape map from secondary input...");
    OsmMapPtr cutterShapeOutlineMap = AlphaShapeGenerator(1000.0, 0.0).generateMap(secMap);
    MapProjector::projectToWgs84(cutterShapeOutlineMap); // not exactly sure why this needs to be done
    OsmMapWriterFactory::writeDebugMap(cutterShapeOutlineMap, "cutter-shape");

    // cookie cut the shape of the cutter shape map out of the cropped ref map

    LOG_DEBUG("Cookie cutting cutter shape out of reference map...");
    conf().set(
      ConfigOptions::getCropKeepEntireFeaturesCrossingBoundsKey(), _cropKeepEntireCrossingBounds);
    conf().set(
      ConfigOptions::getCropKeepOnlyFeaturesInsideBoundsKey(), _cropKeepOnlyInsideBounds);
    CookieCutter(false, 0.0).cut(cutterShapeOutlineMap, refMap);
    OsmMapPtr cookieCutMap = refMap;
    MapProjector::projectToWgs84(cookieCutMap); // not exactly sure why this needs to be done
    OsmMapWriterFactory::writeDebugMap(cookieCutMap, "cookie-cut");

    // Renumber the relations in the sec map, as they could have ID overlap with those in the cookie
    // cut ref map at this point. This is due to the fact that they have been modified independently
    // of each other with cropping, which may create new relations.

    LOG_DEBUG("Remapping secondary map relation IDs...");
    ElementIdRemapper relationIdRemapper;
    relationIdRemapper.setIdGen(cookieCutMap->getIdGeneratorSp());
    relationIdRemapper.setRemapNodes(false);
    relationIdRemapper.setRemapRelations(true);
    relationIdRemapper.setRemapWays(false);
    relationIdRemapper.apply(secMap);
    OsmMapWriterFactory::writeDebugMap(secMap, "sec-relation-ids-remapped");

    // add node hashes so we can append the cookie cut and sec maps together (needed for element
    // comparison)

    LOG_DEBUG("Adding hashes for node comparisons...");
    CalculateHashVisitor2 hashVis;
    cookieCutMap->visitRw(hashVis);
    secMap->visitRw(hashVis);
    OsmMapWriterFactory::writeDebugMap(secMap, "cookie-cut-after-node-hashes");
    OsmMapWriterFactory::writeDebugMap(secMap, "sec-after-node-hashes");

    // combine the cookie cut map back with the secondary map, so we can conflate

    MapProjector::projectToWgs84(secMap);   // not exactly sure why this needs to be done
    cookieCutMap->append(secMap);
    OsmMapPtr conflateMap = cookieCutMap;
    secMap.reset();
    OsmMapWriterFactory::writeDebugMap(conflateMap, "combined-before-conflation");

    // conflate the cookie cut ref map with the cropped sec map

    LOG_DEBUG("Conflating the cookie cut reference map with the secondary map...");
    NamedOp preOps(ConfigOptions().getConflatePreOps());
    preOps.apply(conflateMap);
    // TODO: restrict conflate matchers to only those relevant based on the filter?
    // TODO: set configuration on UnifyingConflator?
    UnifyingConflator().apply(conflateMap);
    NamedOp postOps(ConfigOptions().getConflatePostOps());
    postOps.apply(conflateMap);
    OsmMapWriterFactory::writeDebugMap(conflateMap, "conflated");

    if (!lenientBounds && _isLinearCrit(featureTypeFilterClassName))
    {
      // Snap secondary features back to reference features if dealing with linear features and a
      // non-lenient bounds where ref features may have been cut along it.

      LOG_DEBUG("Snapping unconnected linear secondary features back to reference features...");
      UnconnectedWaySnapper lineSnapper;
      lineSnapper.setConfiguration(conf());
      lineSnapper.setSnapToWayStatus("Input1");
      lineSnapper.setSnapWayStatus("Input2;Conflated");
      lineSnapper.setWayNodeToSnapToCriterionClassName(featureTypeFilterClassName);
      lineSnapper.setWayToSnapCriterionClassName(featureTypeFilterClassName);
      lineSnapper.setWayToSnapToCriterionClassName(featureTypeFilterClassName);
      lineSnapper.apply(conflateMap);
      OsmMapWriterFactory::writeDebugMap(conflateMap, "snapped");

      // After snapping, perform way joining to prevent unnecessary create/delete statements for the
      // ref data in the resulting changeset and generate modify statements instead.

      LOG_DEBUG("Rejoining features after unconnected linear feature snapping...");
      ReplacementSnappedWayJoiner().join(conflateMap);
      OsmMapWriterFactory::writeDebugMap(conflateMap, "joined");
    }

    MapProjector::projectToWgs84(conflateMap);  // conflation and snapping work in planar

    // break the ref and sec data out into separate maps from the conflated map for changeset
    // derivation

    // TODO: could there be any conflated data still in here as well, or did way joining change
    // ways with conflated status back to an input status?

    LOG_DEBUG("Separating reference data out of conflated map...");
    OsmMapPtr refChangesetMap(new OsmMap(conflateMap));
    RemoveUnknown2Visitor removeSecVis;
    refChangesetMap->visitRw(removeSecVis);
    OsmMapWriterFactory::writeDebugMap(refChangesetMap, "ref-separated");

    LOG_DEBUG("Separating secondary data out of conflated map...");
    OsmMapPtr secChangesetMap(new OsmMap(conflateMap));
    RemoveUnknown1Visitor removeRefVis;
    secChangesetMap->visitRw(removeRefVis);
    OsmMapWriterFactory::writeDebugMap(secChangesetMap, "sec-separated");

    conflateMap.reset();

    // crop the ref/sec maps appropriately for changeset derivation

    MapCropper cropper(bounds);

    LOG_DEBUG("Cropping reference map for changeset derivation...");
    cropper.setKeepEntireFeaturesCrossingBounds(_changesetRefKeepEntireCrossingBounds);
    cropper.setKeepOnlyFeaturesInsideBounds(_changesetRefKeepOnlyInsideBounds);
    cropper.apply(refChangesetMap);
    OsmMapWriterFactory::writeDebugMap(refChangesetMap, "ref-cropped-for-changeset");

    LOG_DEBUG("Cropping secondary map for changeset derivation...");
    cropper.setKeepEntireFeaturesCrossingBounds(_changesetSecKeepEntireCrossingBounds);
    cropper.setKeepOnlyFeaturesInsideBounds(_changesetSecKeepOnlyInsideBounds);
    cropper.apply(secChangesetMap);
    OsmMapWriterFactory::writeDebugMap(secChangesetMap, "sec-cropped-for-changeset");

    if (!ConfigOptions().getChangesetAllowDeletingReferenceFeaturesOutsideBounds())
    {
      // If we're not allowing the changeset deriver to generate delete statements for reference
      // features outside of the bounds, we need to mark all corresponding ref ways with a custom
      // tag that will cause the deriver to skip deleting them.

      // The strictness of the bounds check is governed by a config option. Generally, would use
      // not strict for linear features and strict for point or poly features.
      conf().set("in.bounds.criterion.bounds", boundsStr);
      std::shared_ptr<InBoundsCriterion> boundsCrit(new InBoundsCriterion());
      boundsCrit->setOsmMap(refChangesetMap.get());
      std::shared_ptr<NotCriterion> notInBoundsCrit(new NotCriterion(boundsCrit));
      std::shared_ptr<ChainCriterion> elementCrit(
        new ChainCriterion(std::shared_ptr<WayCriterion>(new WayCriterion()), notInBoundsCrit));

      RecursiveSetTagValueOp tagSetter(elementCrit, MetadataTags::HootChangeExcludeDelete(), "yes");
      tagSetter.apply(refChangesetMap);

      OsmMapWriterFactory::writeDebugMap(refChangesetMap, "after-adding-ref-delete-exclude-tags");
    }

    // derive a changeset that replaces ref features with secondary features within the bounds

    LOG_DEBUG("Deriving replacement changeset...");
    conf().set(
      ConfigOptions::getChangesetAllowDeletingReferenceFeaturesOutsideBoundsKey(),
      _changesetAllowDeletingRefOutsideBounds);
    conf().set(
      ConfigOptions::getInBoundsCriterionStrictKey(),
      _inBoundsStrict);
    _changesetCreator->create(refChangesetMap, secChangesetMap, output);
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

void ChangesetReplacementCreator::_setConfigOpts(const bool lenientBounds,
                                                 const QString& critClassName)
{
  // changeset and db opts should have already been set when calling this command

  // global opts

  conf().set(ConfigOptions::getChangesetXmlWriterAddTimestampKey(), "false");
  conf().set(ConfigOptions::getReaderAddSourceDatetimeKey(), "false");
  conf().set(ConfigOptions::getWriterIncludeCircularErrorTagsKey(), "false");

  // dataset specific opts

  // only one of these should ever be true
  const bool replacingPoints = _isPointCrit(critClassName);
  const bool replacingLines = _isLinearCrit(critClassName);
  const bool replacingPolys = _isPolyCrit(critClassName);

  // TODO: Hopefully, these three sets of bounds options can be collapsed (??).

  _convertRefKeepOnlyInsideBounds = false;
  _cropKeepOnlyInsideBounds = false;
  _changesetRefKeepOnlyInsideBounds = false;

  if (replacingPoints)
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
  else if (replacingLines)
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
  else if (replacingPolys)
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
}

}
