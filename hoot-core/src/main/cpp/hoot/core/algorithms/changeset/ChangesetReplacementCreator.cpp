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

    _setConfigOpts(lenientBounds, featureTypeFilterClassName);

    // load each dataset separately and crop to the specified aoi

    conf().set(
      ConfigOptions::getConvertBoundingBoxKey(), GeometryUtils::envelopeToConfigString(bounds));

    conf().set(
      ConfigOptions::getConvertBoundingBoxKeepEntireFeaturesCrossingBoundsKey(),
      _convertRefKeepEntireCrossingBounds);
    conf().set(
      ConfigOptions::getConvertBoundingBoxKeepOnlyFeaturesInsideBoundsKey(),
      _convertRefKeepOnlyInsideBounds);
    OsmMapPtr refMap(new OsmMap());
    IoUtils::loadMap(refMap, input1, true, Status::Unknown1);

    conf().set(
      ConfigOptions::getConvertBoundingBoxKeepEntireFeaturesCrossingBoundsKey(),
      _convertSecKeepEntireCrossingBounds);
    conf().set(
      ConfigOptions::getConvertBoundingBoxKeepOnlyFeaturesInsideBoundsKey(),
      _convertSecKeepOnlyInsideBounds);
    OsmMapPtr secMap(new OsmMap());
    IoUtils::loadMap(secMap, input2, true, Status::Unknown2);

    // prune down to just the feature types specified by the filter

    RemoveElementsVisitor elementsRemover(true);
    elementsRemover.addCriterion(featureCrit);
    elementsRemover.setRecursive(true);
    refMap->visitRw(elementsRemover);
    secMap->visitRw(elementsRemover);

    // generate a cutter shape based on the cropped secondary map

    OsmMapPtr cutterShapeMap = AlphaShapeGenerator(1000.0, 0.0).generateMap(secMap);

    // cookie cut the shape of the cutter shape map out of the cropped ref map

    conf().set(
      ConfigOptions::getCropKeepEntireFeaturesCrossingBoundsKey(), _cropKeepEntireCrossingBounds);
    conf().set(
      ConfigOptions::getCropKeepOnlyFeaturesInsideBoundsKey(), _cropKeepOnlyInsideBounds);
    CookieCutter(false, 0.0).cut(cutterShapeMap, refMap);
    OsmMapPtr cookieCutMap = refMap;

    // Renumber the relations in the sec map, as they could have ID overlap with those in the cookie
    // cut ref map at this point. This is due to the fact that they have been modified independently
    // of each other with cropping, which may create new relations.

    ElementIdRemapper relationIdRemapper;
    relationIdRemapper.setIdGen(cookieCutMap->getIdGeneratorSp());
    relationIdRemapper.setRemapNodes(false);
    relationIdRemapper.setRemapRelations(true);
    relationIdRemapper.setRemapWays(false);
    relationIdRemapper.apply(secMap);

    // add node hashes so we can append the cookie cut and sec maps together (needed for element
    // comparison)

    CalculateHashVisitor2 hashVis;
    cookieCutMap->visitRw(hashVis);
    secMap->visitRw(hashVis);

    // conflate the cookie cut ref map with the cropped sec map

    cookieCutMap->append(secMap);
    OsmMapPtr conflateMap = cookieCutMap;
    secMap.reset();
    NamedOp preOps(ConfigOptions().getConflatePreOps());
    preOps.apply(conflateMap);
    // TODO: restrict conflate matchers to only those relevant based on the filter?
    // TODO: set configuration on UnifyingConflator?
    UnifyingConflator().apply(conflateMap);
    NamedOp postOps(ConfigOptions().getConflatePostOps());
    postOps.apply(conflateMap);

    if (!lenientBounds && _isLinearCrit(featureTypeFilterClassName))
    {
      // Snap secondary features back to reference features if dealing with linear features and a
      // non-lenient bounds where ref features may have been cut along it.

      UnconnectedWaySnapper snapper;
      snapper.setConfiguration(conf());
      snapper.setSnapToWayStatus("Input1");
      snapper.setSnapWayStatus("Input2;Conflated");
      snapper.setWayNodeToSnapToCriterionClassName(featureTypeFilterClassName);
      snapper.setWayToSnapCriterionClassName(featureTypeFilterClassName);
      snapper.setWayToSnapToCriterionClassName(featureTypeFilterClassName);
      snapper.apply(conflateMap);

      // After snapping, perform way joining to prevent unnecessary create/delete statements for the
      // ref data in the resulting changeset and generate modify statements instead.

      ReplacementSnappedWayJoiner().join(conflateMap);
    }

    OsmMapPtr refChangesetMap;
    OsmMapPtr secChangesetMap;

    // break the ref and sec data out into separate maps from the conflated map

    refChangesetMap.reset(new OsmMap(conflateMap));
    RemoveUnknown2Visitor removeSecVis;
    refChangesetMap->visitRw(removeSecVis);

    secChangesetMap.reset(new OsmMap(conflateMap));
    RemoveUnknown2Visitor removeRefVis;
    secChangesetMap->visitRw(removeRefVis);

    conflateMap.reset();

    // crop the ref/sec maps appropriately for changeset derivation

    MapCropper cropper;

    cropper.setKeepEntireFeaturesCrossingBounds(_changesetRefKeepEntireCrossingBounds);
    cropper.setKeepOnlyFeaturesInsideBounds(_changesetRefKeepOnlyInsideBounds);
    cropper.apply(refChangesetMap);

    cropper.setKeepEntireFeaturesCrossingBounds(_changesetSecKeepEntireCrossingBounds);
    cropper.setKeepOnlyFeaturesInsideBounds(_changesetSecKeepOnlyInsideBounds);
    cropper.apply(secChangesetMap);

    // derive a changeset that replaces ref features with secondary features within the bounds

    conf().set(
      ConfigOptions::getChangesetAllowDeletingReferenceFeaturesOutsideBoundsKey(),
      _changesetAllowDeletingRefOutsideBounds);
    conf().set(
      ConfigOptions::getInBoundsCriterionStrictKey(),
      _inBoundsStrict);
    _changesetCreator->create(refChangesetMap, secChangesetMap, output);
}

// hardcode these for now; only one of the following should return true for any input

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
