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
#include <hoot/core/algorithms/alpha-shape/AlphaShapeGenerator.h>
#include <hoot/core/algorithms/ReplacementSnappedWayJoiner.h>
#include <hoot/core/algorithms/WayJoinerAdvanced.h>
#include <hoot/core/algorithms/WayJoinerBasic.h>

#include <hoot/core/conflate/CookieCutter.h>
#include <hoot/core/conflate/UnifyingConflator.h>
#include <hoot/core/conflate/network/NetworkMatchCreator.h>

#include <hoot/core/criterion/ConflatableElementCriterion.h>
#include <hoot/core/criterion/ElementTypeCriterion.h>
#include <hoot/core/criterion/InBoundsCriterion.h>
#include <hoot/core/criterion/LinearCriterion.h>
#include <hoot/core/criterion/NotCriterion.h>
#include <hoot/core/criterion/OrCriterion.h>
#include <hoot/core/criterion/PointCriterion.h>
#include <hoot/core/criterion/PolygonCriterion.h>
#include <hoot/core/criterion/TagCriterion.h>
#include <hoot/core/criterion/TagKeyCriterion.h>
#include <hoot/core/criterion/WayNodeCriterion.h>

#include <hoot/core/elements/OsmUtils.h>
#include <hoot/core/io/OsmGeoJsonReader.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>

#include <hoot/core/ops/ElementIdToVersionMapper.h>
#include <hoot/core/ops/UnconnectedWaySnapper.h>

#include <hoot/core/ops/MapCropper.h>
#include <hoot/core/ops/NamedOp.h>
#include <hoot/core/ops/PointsToPolysConverter.h>
#include <hoot/core/ops/SuperfluousNodeRemover.h>
#include <hoot/core/ops/SuperfluousWayRemover.h>
#include <hoot/core/ops/RecursiveSetTagValueOp.h>
#include <hoot/core/ops/RemoveEmptyRelationsOp.h>
#include <hoot/core/ops/WayJoinerOp.h>

#include <hoot/core/util/Boundable.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/GeometryUtils.h>
#include <hoot/core/io/IoUtils.h>
#include <hoot/core/util/MapProjector.h>

#include <hoot/core/visitors/ApiTagTruncateVisitor.h>
#include <hoot/core/visitors/FilteredVisitor.h>
#include <hoot/core/visitors/RemoveElementsVisitor.h>
#include <hoot/core/visitors/RemoveMissingElementsVisitor.h>
#include <hoot/core/visitors/SetTagValueVisitor.h>

namespace hoot
{

ChangesetReplacementCreator::ChangesetReplacementCreator(const bool printStats,
                                                         const QString osmApiDbUrl) :
_fullReplacement(false),
_lenientBounds(true),
_geometryFiltersSpecified(false),
_chainReplacementFilters(false),
_chainRetainmentFilters(false),
_waySnappingEnabled(true),
_conflationEnabled(true)
{
  _changesetCreator.reset(new ChangesetCreator(printStats, osmApiDbUrl));
  setGeometryFilters(QStringList());
}

void ChangesetReplacementCreator::setGeometryFilters(const QStringList& filterClassNames)
{
  LOG_VART(filterClassNames);
  if (!filterClassNames.isEmpty())
  {
    _geometryFiltersSpecified = true;
    _geometryTypeFilters.clear();
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

      ElementCriterionPtr currentFilter = _geometryTypeFilters[filter->getGeometryType()];
      if (!currentFilter)
      {
        _geometryTypeFilters[filter->getGeometryType()] = filter;
      }
      else
      {
        _geometryTypeFilters[filter->getGeometryType()] =
          std::shared_ptr<OrCriterion>(new OrCriterion(currentFilter, filter));
      }

      if (filter->getGeometryType() == GeometryTypeCriterion::GeometryType::Line)
      {
        _linearFilterClassNames.append(filterClassName);
      }
    }
  }

  // TODO: have to call this method to keep filtering from erroring...shouldn't have to...should
  // init itself internally when no geometry filters are specified
  LOG_VART(_geometryTypeFilters.size());
  if (_geometryTypeFilters.isEmpty())
  {
    _geometryFiltersSpecified = false;
    _geometryTypeFilters = _getDefaultGeometryFilters();
    _linearFilterClassNames =
      ConflatableElementCriterion::getCriterionClassNamesByGeometryType(
        GeometryTypeCriterion::GeometryType::Line);
  }

  LOG_VARD(_geometryTypeFilters.size());
  LOG_VART(_linearFilterClassNames);
}

void ChangesetReplacementCreator::_setInputFilter(
  std::shared_ptr<ChainCriterion>& inputFilter, const QStringList& filterClassNames,
  const bool chainFilters)
{
  LOG_VARD(filterClassNames.size());
  if (!filterClassNames.isEmpty())
  {
    LOG_VARD(chainFilters);
    if (!chainFilters)
    {
      inputFilter.reset(new OrCriterion());
    }
    else
    {
      inputFilter.reset(new ChainCriterion());
    }

    for (int i = 0; i < filterClassNames.size(); i++)
    {
      const QString filterClassName = filterClassNames.at(i);
      LOG_VARD(filterClassName);

      ElementCriterionPtr crit;
      try
      {
        crit.reset(Factory::getInstance().constructObject<ElementCriterion>(filterClassName));
      }
      catch (const boost::bad_any_cast&)
      {
      }
      if (!crit)
      {
        throw IllegalArgumentException(
          "Invalid additional input filter: " + filterClassName +
          ". Filter must be a ElementCriterion.");
      }

      // Fail if the filter maps to a geometry type.
      std::shared_ptr<GeometryTypeCriterion> geometryTypeFilter;
      try
      {
        geometryTypeFilter = std::dynamic_pointer_cast<GeometryTypeCriterion>(crit);
      }
      catch (const boost::bad_any_cast&)
      {
      }
      if (geometryTypeFilter)
      {
        throw IllegalArgumentException(
          "Invalid additional input filter: " + filterClassName +
          ". May not be a GeometryTypeCriterion.");
      }

      inputFilter->addCriterion(crit);
    }

    LOG_VARD(inputFilter->toString());
  }
}

void ChangesetReplacementCreator::setReplacementFilters(const QStringList& filterClassNames)
{
  LOG_VART(filterClassNames.size());
  if (filterClassNames.size() > 0)
  {
    LOG_TRACE("Creating replacement filter...");
    _setInputFilter(_replacementFilter, filterClassNames, _chainReplacementFilters);
  }
}

void ChangesetReplacementCreator::setRetainmentFilters(const QStringList& filterClassNames)
{
  LOG_VARD(filterClassNames.size());
  if (filterClassNames.size() > 0)
  {
    LOG_DEBUG("Creating retainment filter...");
    _setInputFilter(_retainmentFilter, filterClassNames, _chainRetainmentFilters);
  }
}

void ChangesetReplacementCreator::_setInputFilterOptions(Settings& opts,
                                                         const QStringList& optionKvps)
{
  LOG_VART(optionKvps.size());
  opts = conf();
  const int optsSizeBefore = opts.size();
  for (int i = 0; i < optionKvps.size(); i++)
  {
    const QString kvp = optionKvps.at(i);
    // split on the first occurrence of '=' since the opt value itself could have an '=' in it
    const int firstEqualOccurrence = kvp.indexOf("=");
    if (firstEqualOccurrence == -1)
    {
      throw IllegalArgumentException("Invalid filter configuration option: " + kvp);
    }
    const QString key = kvp.mid(0, firstEqualOccurrence).trimmed().remove("\"").remove("'");
    LOG_VART(key);
    const QString val = kvp.mid(firstEqualOccurrence + 2).trimmed().remove("\"").remove("'");
    LOG_VART(val);
    opts.set(key, val);
  }
  const int optsSizeAfter = opts.size() - optsSizeBefore;
  LOG_DEBUG("Opts size after adding custom opts: " << optsSizeAfter);
}

void ChangesetReplacementCreator::setReplacementFilterOptions(const QStringList& optionKvps)
{
  LOG_DEBUG("Creating replacement filter options...");
  _setInputFilterOptions(_replacementFilterOptions, optionKvps);
}

void ChangesetReplacementCreator::setRetainmentFilterOptions(const QStringList& optionKvps)
{
  LOG_DEBUG("Creating retainment filter options...");
  _setInputFilterOptions(_retainmentFilterOptions, optionKvps);
}

void ChangesetReplacementCreator::create(
  const QString& input1, const QString& input2, const geos::geom::Envelope& bounds,
  const QString& output)
{
  LOG_VARD(_chainReplacementFilters);
  LOG_VARD(_lenientBounds);
  LOG_VARD(_fullReplacement);

  // INPUT VALIDATION AND SETUP

  _validateInputs(input1, input2);
  const QString boundsStr = GeometryUtils::envelopeToConfigString(bounds);
  _setGlobalOpts(boundsStr);
  // If a retainment filter was specified, we'll AND it together with each geometry type filter to
  // further restrict what reference data gets replaced in the final changeset.
  const QMap<GeometryTypeCriterion::GeometryType, ElementCriterionPtr> refFilters =
   _getCombinedFilters(_retainmentFilter);
  // If a replacement filter was specified, we'll AND it together with each geometry type filter to
  // further restrict what secondary replacement data goes into the final changeset.
  const QMap<GeometryTypeCriterion::GeometryType, ElementCriterionPtr> secFilters =
    _getCombinedFilters(_replacementFilter);

  const int maxFilePrintLength = ConfigOptions().getProgressVarPrintLengthMax();
  QString lenientStr = "Bounds calculation is ";
  if (!_lenientBounds)
  {
    lenientStr += "not ";
  }
  lenientStr += "lenient.";
  LOG_INFO(
    "Deriving replacement output changeset: ..." << output.right(maxFilePrintLength) <<
    " from inputs: ..." << input1.right(maxFilePrintLength) << " and ..." <<
    input2.right(maxFilePrintLength) << "" << ", at bounds: " << boundsStr << ". " << lenientStr);

  // CHANGESET CALCULATION

  // Since data with different geometry types require different settings, we'll calculate a separate
  // pair of before/after maps for each geometry type.

  QList<OsmMapPtr> refMaps;
  QList<OsmMapPtr> conflatedMaps;
  int passCtr = 1;
  for (QMap<GeometryTypeCriterion::GeometryType, ElementCriterionPtr>::const_iterator itr =
         refFilters.begin(); itr != refFilters.end(); ++itr)
  {
    LOG_VARD("******************************************");
    LOG_INFO(
      "Preparing maps for changeset derivation given geometry type: "<<
      GeometryTypeCriterion::typeToString(itr.key()) << ". Pass: " << passCtr << " / " <<
      refFilters.size() << "...");

    OsmMapPtr refMap;
    OsmMapPtr conflatedMap;
    QStringList linearFilterClassNames;
    //LOG_VARD(itr.value().get());
    if (itr.key() == GeometryTypeCriterion::GeometryType::Line)
    {
      linearFilterClassNames = _linearFilterClassNames;
    }
    _getMapsForGeometryType(
      refMap, conflatedMap, input1, input2, boundsStr, itr.value(), secFilters[itr.key()],
      itr.key(), linearFilterClassNames);

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
        "Adding ref map of size: " << refMap->size() << " and conflated map of size: " <<
        conflatedMap->size() << " to changeset derivation queue for geometry type: " <<
        GeometryTypeCriterion::typeToString(itr.key()) << "...");
      refMap->setName(refMap->getName() + "-" + GeometryTypeCriterion::typeToString(itr.key()));
      refMaps.append(refMap);
      conflatedMap->setName(
        conflatedMap->getName() + "-" + GeometryTypeCriterion::typeToString(itr.key()));
      conflatedMaps.append(conflatedMap);
    }

    passCtr++;
  }

  LOG_VART(refMaps.size());
  LOG_VART(conflatedMaps.size());
  if (refMaps.size() == 0 && conflatedMaps.size() == 0)
  {
    LOG_WARN("No features remain after filtering so no changeset will be generated.");
    return;
  }
  assert(refMaps.size() == conflatedMaps.size());

  // CHANGESET GENERATION

  // Derive a changeset between the ref and conflated maps that replaces ref features with
  // secondary features within the bounds and write it out.

  _changesetCreator->create(refMaps, conflatedMaps, output);

  LOG_INFO("Derived replacement changeset: ..." << output.right(maxFilePrintLength));
}

void ChangesetReplacementCreator::_getMapsForGeometryType(
  OsmMapPtr& refMap, OsmMapPtr& conflatedMap, const QString& input1, const QString& input2,
  const QString& boundsStr, const ElementCriterionPtr& refFeatureFilter,
  const ElementCriterionPtr& secFeatureFilter,
  const GeometryTypeCriterion::GeometryType& geometryType,
  const QStringList& linearFilterClassNames)
{
  LOG_VART(linearFilterClassNames);
  LOG_VART(secFeatureFilter);

  // INPUT VALIDATION AND SETUP

  _parseConfigOpts(_lenientBounds, geometryType);

  // DATA LOAD AND INITIAL PREP

  // Load the ref dataset and crop to the specified aoi.

  refMap = _loadRefMap(input1);

  // Keep a mapping of the original ref element ids to versions, as we'll need the original
  // versions later.

  const QMap<ElementId, long> refIdToVersionMappings = _getIdToVersionMappings(refMap);
  const bool isLinearCrit = !linearFilterClassNames.isEmpty();
  LOG_VART(isLinearCrit);
  if (_lenientBounds && isLinearCrit)
  {
    // If we have a lenient bounds requirement and linear features, we need to exclude all ways
    // outside of the bounds but immediately connected to a way crossing the bounds from deletion.
    _addChangesetDeleteExclusionTags(refMap);
  }

  // Prune the ref dataset down to just the geometry types specified by the filter, so we don't end
  // up modifying anything else.

  _filterFeatures(
    refMap, refFeatureFilter, conf(),
    "ref-after-" + GeometryTypeCriterion::typeToString(geometryType) + "-pruning");

  // Load the sec dataset and crop to the specified aoi.

  OsmMapPtr secMap = _loadSecMap(input2);

  // Prune the sec dataset down to just the feature types specified by the filter, so we don't end
  // up modifying anything else.

  _filterFeatures(
    secMap, secFeatureFilter, _replacementFilterOptions,
    "sec-after-" + GeometryTypeCriterion::typeToString(geometryType) + "-pruning");

  const int refMapSize = refMap->size();
  // If the secondary dataset is empty here and the ref dataset isn't, then we'll end up with a
  // changeset with delete statements if the option to allow deleting reference features is enabled.
  const int secMapSize = secMap->size();
  LOG_VARD(refMapSize);
  LOG_VARD(secMapSize);

  // COOKIE CUT

  // Cut the secondary data out of the reference data.

  OsmMapPtr cookieCutRefMap = _getCookieCutMap(refMap, secMap);

  // At one point it was necessary to re-number the relations in the sec map, as they could have ID
  // overlap with those in the cookie cut ref map at this point. It seemed that this was due to the
  // fact that relations in the two maps had been added independently of each other during cropping.
  // However, after some refactoring this doesn't seem to be the case anymore. If we run into this
  // situation again, we can go back in the history to resurrect the use of the ElementIdRemapper
  // for relations here, which has since been removed from the codebase.

  // Combine the cookie cut ref map back with the secondary map, so we can conflate the two
  // together.

  _combineMaps(cookieCutRefMap, secMap, false, "combined-before-conflation");
  secMap.reset();

  // CONFLATE

  // Conflate the cookie cut ref map with the sec map.

  conflatedMap = cookieCutRefMap;
  if (_conflationEnabled && secMapSize > 0)
  {
    // TODO: do something with reviews - #3361
    _conflate(conflatedMap, _lenientBounds);
  }
  conflatedMap->setName("conflated");

  if (isLinearCrit && _waySnappingEnabled)
  {
    // Snap secondary features back to reference features if dealing with linear features where
    // ref features may have been cut along the bounds. We're being lenient here by snapping
    // secondary to reference *and* allowing conflated data to be snapped to either dataset.

    // We only want to snap ways of like types together, so we'll loop through each applicable
    // linear type and snap them separately.

    QStringList snapWayStatuses("Input2");
    snapWayStatuses.append("Conflated");
    QStringList snapToWayStatuses("Input1");
    snapToWayStatuses.append("Conflated");
    for (int i = 0; i < linearFilterClassNames.size(); i++)
    {
      _snapUnconnectedWays(
        conflatedMap, snapWayStatuses, snapToWayStatuses, linearFilterClassNames.at(i), false,
        "conflated-snapped-sec-to-ref-1");
    }

    // After snapping, perform joining to prevent unnecessary create/delete statements for the ref
    // data in the resulting changeset and generate modify statements instead.

    ReplacementSnappedWayJoiner(refIdToVersionMappings).join(conflatedMap);
    LOG_VART(MapProjector::toWkt(conflatedMap->getProjection()));
  }

  // PRE-CHANGESET DERIVATION DATA PREP

  OsmMapPtr immediatelyConnectedOutOfBoundsWays;
  if (_lenientBounds && isLinearCrit)
  {
    // If we're conflating linear features with the lenient bounds requirement, copy the
    // immediately connected out of bounds ways to a new temp map. We'll lose those ways once we
    // crop in preparation for changeset derivation. If we don't introduce them back during
    // changeset derivation, they may not end up being snapped back to the replacement data.

    immediatelyConnectedOutOfBoundsWays = _getImmediatelyConnectedOutOfBoundsWays(refMap);
  }

  // Crop the original ref and conflated maps appropriately for changeset derivation.

  const geos::geom::Envelope bounds = GeometryUtils::envelopeFromConfigString(boundsStr);
  _cropMapForChangesetDerivation(
    refMap, bounds, _boundsOpts.changesetRefKeepEntireCrossingBounds,
    _boundsOpts.changesetRefKeepOnlyInsideBounds, isLinearCrit, "ref-cropped-for-changeset");
  _cropMapForChangesetDerivation(
    conflatedMap, bounds, _boundsOpts.changesetSecKeepEntireCrossingBounds,
    _boundsOpts.changesetSecKeepOnlyInsideBounds, isLinearCrit, "sec-cropped-for-changeset");
  LOG_VART(_lenientBounds);
  LOG_VART(isLinearCrit);
  if (_lenientBounds && isLinearCrit)
  {
    if (_waySnappingEnabled)
    {
      // The non-strict way replacement workflow benefits from a second snapping run right before
      // changeset derivation due to there being ways connected to replacement ways that fall
      // completely outside of the bounds. However, joining after this snapping caused changeset
      // errors with some datasets and hasn't seem to be needed for now...so skipping it. Note that
      // we're being as lenient as possible with the snapping here, allowing basically anything to
      // join to anything else, which could end up causing problems...we'll go with it for now.

      QStringList snapWayStatuses("Input2");
      snapWayStatuses.append("Conflated");
      snapWayStatuses.append("Input1");
      QStringList snapToWayStatuses("Input1");
      snapToWayStatuses.append("Conflated");
      snapToWayStatuses.append("Input2");
      LOG_VARD(linearFilterClassNames);
      for (int i = 0; i < linearFilterClassNames.size(); i++)
      {
        _snapUnconnectedWays(
          conflatedMap, snapWayStatuses, snapToWayStatuses, linearFilterClassNames.at(i), false,
          "conflated-snapped-sec-to-ref-2");
      }
    }

    // Combine the conflated map with the immediately connected out of bounds ways.

    _combineMaps(
      conflatedMap, immediatelyConnectedOutOfBoundsWays, true, "conflated-connected-combined");

    // Snap only the connected ways to other ways in the conflated map. Mark the ways that were
    // snapped, as we'll need that info in the next step.

    if (_waySnappingEnabled)
    {
      LOG_VART(linearFilterClassNames);
      for (int i = 0; i < linearFilterClassNames.size(); i++)
      {
        _snapUnconnectedWays(
          conflatedMap, QStringList("Input1"), QStringList("Input1"), linearFilterClassNames.at(i),
          true, "conflated-snapped-immediately-connected-out-of-bounds");
      }
    }

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

  _cleanupMissingElements(refMap);
  _cleanupMissingElements(conflatedMap);

  LOG_VART(refMap->getElementCount());
  LOG_VART(conflatedMap->getElementCount());
}

void ChangesetReplacementCreator::_cleanupMissingElements(OsmMapPtr& map)
{
  //SuperfluousWayRemover::removeWays(map);
  //SuperfluousNodeRemover::removeNodes(map);

  // This will handle removing refs in relation members we've cropped out.
  RemoveMissingElementsVisitor missingElementsRemover;
  LOG_INFO("\t" << missingElementsRemover.getInitStatusMessage());
  map->visitRw(missingElementsRemover);
  LOG_DEBUG("\t" << missingElementsRemover.getCompletedStatusMessage());

  // This will remove any relations that were already empty or became empty after the previous step.
  RemoveEmptyRelationsOp emptyRelationRemover;
  LOG_INFO("\t" << emptyRelationRemover.getInitStatusMessage());
  emptyRelationRemover.apply(map);
  LOG_DEBUG("\t" << emptyRelationRemover.getCompletedStatusMessage());
}

void ChangesetReplacementCreator::_validateInputs(const QString& input1, const QString& input2)
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

  if (_fullReplacement && _retainmentFilter)
  {
    throw IllegalArgumentException(
      "Both full reference data replacement and a reference data retainment filter may not "
      "be specified for replacement changeset derivation.");
  }

  if (ConfigOptions().getConvertOps().size())
  {
    throw IllegalArgumentException(
      "Replacement changeset derivation does not support convert operations.");
  }
}

QMap<GeometryTypeCriterion::GeometryType, ElementCriterionPtr>
  ChangesetReplacementCreator::_getDefaultGeometryFilters() const
{
  QMap<GeometryTypeCriterion::GeometryType, ElementCriterionPtr> featureFilters;
  featureFilters[GeometryTypeCriterion::GeometryType::Point] =
    std::shared_ptr<ElementCriterion>(new PointCriterion());
  featureFilters[GeometryTypeCriterion::GeometryType::Line] =
    std::shared_ptr<ElementCriterion>(new LinearCriterion());
  featureFilters[GeometryTypeCriterion::GeometryType::Polygon] =
    std::shared_ptr<ElementCriterion>(new PolygonCriterion());
  return featureFilters;
}

QMap<GeometryTypeCriterion::GeometryType, ElementCriterionPtr>
  ChangesetReplacementCreator::_getCombinedFilters(
    std::shared_ptr<ChainCriterion> nonGeometryFilter)
{
  QMap<GeometryTypeCriterion::GeometryType, ElementCriterionPtr> combinedFilters;
  LOG_VART(nonGeometryFilter.get());
  if (nonGeometryFilter)
  {
    for (QMap<GeometryTypeCriterion::GeometryType, ElementCriterionPtr>::const_iterator itr =
         _geometryTypeFilters.begin(); itr != _geometryTypeFilters.end(); ++itr)
    {
      combinedFilters[itr.key()] =
        std::shared_ptr<ChainCriterion>(new ChainCriterion(itr.value(), nonGeometryFilter));
      LOG_TRACE("New combined filter: " << combinedFilters[itr.key()]->toString());
    }
  }
  else
  {
    if (_geometryTypeFilters.isEmpty())
    {
      _geometryTypeFilters = _getDefaultGeometryFilters();
      _linearFilterClassNames =
        ConflatableElementCriterion::getCriterionClassNamesByGeometryType(
          GeometryTypeCriterion::GeometryType::Line);
    }
    combinedFilters = _geometryTypeFilters;
  }
  LOG_VART(combinedFilters.size());
  return combinedFilters;
}

OsmMapPtr ChangesetReplacementCreator::_loadRefMap(const QString& input)
{ 
  LOG_INFO("Loading reference map: " << input << "...");

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

  OsmMapPtr refMap(new OsmMap());
  refMap->setName("ref");
  IoUtils::loadMap(refMap, input, true, Status::Unknown1);

  conf().set(ConfigOptions::getReaderWarnOnZeroVersionElementKey(), false);

  LOG_VART(MapProjector::toWkt(refMap->getProjection()));
  OsmMapWriterFactory::writeDebugMap(refMap, "ref-after-cropped-load");

  return refMap;
}

QMap<ElementId, long> ChangesetReplacementCreator::_getIdToVersionMappings(
  const OsmMapPtr& map) const
{
  LOG_DEBUG("Mapping element IDs to element versions for: " << map->getName() << "...");

  ElementIdToVersionMapper idToVersionMapper;
  LOG_DEBUG(idToVersionMapper.getInitStatusMessage());
  idToVersionMapper.apply(map);
  LOG_DEBUG(idToVersionMapper.getCompletedStatusMessage());
  const QMap<ElementId, long> idToVersionMappings = idToVersionMapper.getMappings();
  LOG_VART(idToVersionMappings.size());
  return idToVersionMappings;
}

void ChangesetReplacementCreator::_addChangesetDeleteExclusionTags(OsmMapPtr& map)
{
  LOG_INFO(
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
  LOG_INFO("Loading secondary map: " << input << "...");

  conf().set(
    ConfigOptions::getConvertBoundingBoxKeepEntireFeaturesCrossingBoundsKey(),
    _boundsOpts.loadSecKeepEntireCrossingBounds);
  conf().set(
    ConfigOptions::getConvertBoundingBoxKeepOnlyFeaturesInsideBoundsKey(),
    _boundsOpts.loadSecKeepOnlyInsideBounds);
  conf().set(
    ConfigOptions::getConvertBoundingBoxKeepImmediatelyConnectedWaysOutsideBoundsKey(), false);

  OsmMapPtr secMap(new OsmMap());
  secMap->setName("sec");
  IoUtils::loadMap(secMap, input, false, Status::Unknown2);

  LOG_VART(MapProjector::toWkt(secMap->getProjection()));
  OsmMapWriterFactory::writeDebugMap(secMap, "sec-after-cropped-load");

  return secMap;
}

void ChangesetReplacementCreator::_filterFeatures(
  OsmMapPtr& map, const ElementCriterionPtr& featureFilter, const Settings& config,
  const QString& debugFileName)
{
  LOG_INFO(
    "Filtering features for: " << map->getName() << " based on input filter: " +
    featureFilter->toString() << "...");

  RemoveElementsVisitor elementPruner(true);
  // The criteria must be added before the config or map is set. We may want to change
  // MultipleCriterionConsumerVisitor and RemoveElementsVisitor to make this behavior less brittle.
  elementPruner.addCriterion(featureFilter);
  elementPruner.setConfiguration(config);
  elementPruner.setOsmMap(map.get());
  elementPruner.setRecursive(true);
  LOG_DEBUG(elementPruner.getInitStatusMessage());
  map->visitRw(elementPruner);
  LOG_DEBUG(elementPruner.getCompletedStatusMessage());

  LOG_VART(MapProjector::toWkt(map->getProjection()));
  OsmMapWriterFactory::writeDebugMap(map, debugFileName);
}

OsmMapPtr ChangesetReplacementCreator::_getCookieCutMap(OsmMapPtr doughMap, OsmMapPtr cutterMap)
{
  // could use some refactoring here after the addition of _fullReplacement

  // If the passed in dough map is empty, there's nothing to be cut out.
  if (doughMap->getElementCount() == 0)
  {
    LOG_DEBUG("Nothing to cut from dough map, so returning the dough map as the cut map...");
    return doughMap;
  }
  else if (cutterMap->size() == 0)
  {
    if (_fullReplacement)
    {
      // If the sec map is empty and we're doing full replacement, we want everything deleted out
      // of the ref for the current feature type in the changeset. So, return an empty map.
      LOG_DEBUG(
        "Nothing in cutter map. Full replacement not enabled, so returning an empty map " <<
        "as the cut map...");
      return OsmMapPtr(new OsmMap());
    }
    else
    {
      // If the sec map is empty and we're not doing full replacement, there's nothing in the sec
      // to overlap with the ref, so leave the ref untouched.
      LOG_DEBUG(
        "Nothing in cutter map. Full replacement enabled, so returning the entire dough map " <<
        "as the cut map...");
      return doughMap;
    }
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
  LOG_VART(OsmUtils::mapIsPointsOnly(cutterMap));
  const double cookieCutterAlpha = opts.getCookieCutterAlpha();
  double cookieCutterAlphaShapeBuffer = opts.getCookieCutterAlphaShapeBuffer();
  LOG_VART(_fullReplacement);
  if (_fullReplacement)
  {
    // Generate a cutter shape based on the ref map, which will cause all the ref data to be
    // replaced.
    LOG_DEBUG("Using dough map as cutter shape map...");
    cutterMapToUse = doughMap;
    cookieCutterAlphaShapeBuffer = 10.0;
  }
  else if (cutterMap->getElementCount() < 3 && OsmUtils::mapIsPointsOnly(cutterMap))
  {
    // Generate a cutter shape based on a transformation of the cropped secondary map.

    // Found that if a map only has a couple points or less, generating an alpha shape from them may
    // not be possible (or at least I don't know how to yet). So instead, go through the points in
    // the map and replace them with small square polys...from that we can generate the alpha shape.

    LOG_DEBUG("Generating cutter shape map from sec map transformation...");
    cutterMapToUse.reset(new OsmMap(cutterMap));
    PointsToPolysConverter pointConverter;
    LOG_INFO(pointConverter.getInitStatusMessage());
    pointConverter.apply(cutterMapToUse);
    LOG_INFO(pointConverter.getCompletedStatusMessage());
    MapProjector::projectToWgs84(cutterMapToUse);
  }
  else
  {
    // Generate a cutter shape based on the cropped secondary map.
    LOG_DEBUG("Using cuter map as cutter shape map...");
    cutterMapToUse = cutterMap;
  }
  LOG_VART(cutterMapToUse->getElementCount());
  OsmMapWriterFactory::writeDebugMap(cutterMapToUse, "cutter-map");

  LOG_INFO("Generating cutter shape map from: " << cutterMapToUse->getName() << "...");

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
        "No cut shape generated from secondary data. Is your secondary data empty or have you " <<
        "filtered it to be empty? error: " << e.getWhat());
    }
    // rethrow the original exception
    throw;
  }

  // not exactly sure yet why this projection needs to be done
  MapProjector::projectToWgs84(cutterShapeOutlineMap);
  LOG_VART(MapProjector::toWkt(cutterShapeOutlineMap->getProjection()));
  OsmMapWriterFactory::writeDebugMap(cutterShapeOutlineMap, "cutter-shape");

  // Cookie cut the shape of the cutter shape map out of the cropped ref map.
  LOG_INFO("Cookie cutting cutter shape out of: " << cookieCutMap->getName() << "...");

  CookieCutter(
    false, 0.0, _boundsOpts.cookieCutKeepEntireCrossingBounds,
    _boundsOpts.cookieCutKeepOnlyInsideBounds)
    .cut(cutterShapeOutlineMap, cookieCutMap);
  MapProjector::projectToWgs84(cookieCutMap); // not exactly sure yet why this needs to be done
  LOG_VARD(cookieCutMap->getElementCount());
  MapProjector::projectToWgs84(doughMap);
  LOG_VART(doughMap->getElementCount());
  LOG_VART(MapProjector::toWkt(cookieCutMap->getProjection()));
  OsmMapWriterFactory::writeDebugMap(cookieCutMap, "cookie-cut");

  return cookieCutMap;
}

void ChangesetReplacementCreator::_combineMaps(OsmMapPtr& map1, OsmMapPtr& map2,
                                               const bool throwOutDupes,
                                               const QString& debugFileName)
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

  LOG_INFO("Combining maps: " << map1->getName() << " and " << map2->getName() << "...");

  map1->append(map2, throwOutDupes);
  LOG_VART(MapProjector::toWkt(map1->getProjection()));
  LOG_DEBUG("Combined map size: " << map1->size());

  OsmMapWriterFactory::writeDebugMap(map1, debugFileName);
}

void ChangesetReplacementCreator::_conflate(OsmMapPtr& map, const bool lenientBounds)
{
  map->setName("conflated");
  LOG_INFO(
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

  UnifyingConflator().apply(map);

  NamedOp postOps(ConfigOptions().getConflatePostOps());
  postOps.apply(map);

  MapProjector::projectToWgs84(map);  // conflation works in planar
  LOG_VART(MapProjector::toWkt(map->getProjection()));
  OsmMapWriterFactory::writeDebugMap(map, "conflated");
  LOG_DEBUG("Conflated map size: " << map->size());
}

void ChangesetReplacementCreator::_snapUnconnectedWays(
  OsmMapPtr& map, const QStringList& snapWayStatuses, const QStringList& snapToWayStatuses,
  const QString& typeCriterionClassName, const bool markSnappedWays, const QString& debugFileName)
{
  LOG_INFO(
    "Snapping ways for map: " << map->getName() << ", with filter type: " <<
    typeCriterionClassName << ", snap way statuses: " << snapWayStatuses <<
    ", snap to way statuses: " << snapToWayStatuses << " ...");

  UnconnectedWaySnapper lineSnapper;
  lineSnapper.setConfiguration(conf());
  // override some of the default config
  lineSnapper.setSnapToWayStatuses(snapToWayStatuses);
  lineSnapper.setSnapWayStatuses(snapWayStatuses);
  lineSnapper.setMarkSnappedWays(markSnappedWays);
  // TODO: Do we need a way to derive the way node crit from the input feature filter crit?
  lineSnapper.setWayNodeToSnapToCriterionClassName(
    QString::fromStdString(WayNodeCriterion::className()));
  lineSnapper.setWayToSnapCriterionClassName(typeCriterionClassName);
  lineSnapper.setWayToSnapToCriterionClassName(typeCriterionClassName);
  LOG_DEBUG(lineSnapper.getInitStatusMessage());
  lineSnapper.apply(map);
  LOG_INFO(lineSnapper.getCompletedStatusMessage());

  MapProjector::projectToWgs84(map);   // snapping works in planar
  LOG_VART(MapProjector::toWkt(map->getProjection()));

  OsmMapWriterFactory::writeDebugMap(map, debugFileName);
}

OsmMapPtr ChangesetReplacementCreator::_getImmediatelyConnectedOutOfBoundsWays(
  const ConstOsmMapPtr& map) const
{
  const QString outputMapName = "connected-ways";
  LOG_INFO(
    "Copying immediately connected out of bounds ways from: " << map->getName() <<
    " to new map: " << outputMapName << "...");

  std::shared_ptr<ChainCriterion> copyCrit(
    new ChainCriterion(
      std::shared_ptr<WayCriterion>(new WayCriterion()),
      std::shared_ptr<TagKeyCriterion>(
        new TagKeyCriterion(MetadataTags::HootConnectedWayOutsideBounds()))));
  OsmMapPtr connectedWays = OsmUtils::getMapSubset(map, copyCrit);
  connectedWays->setName(outputMapName);
  LOG_VART(MapProjector::toWkt(connectedWays->getProjection()));
  OsmMapWriterFactory::writeDebugMap(connectedWays, "connected-ways");
  return connectedWays;
}

void ChangesetReplacementCreator::_cropMapForChangesetDerivation(
  OsmMapPtr& map, const geos::geom::Envelope& bounds, const bool keepEntireFeaturesCrossingBounds,
  const bool keepOnlyFeaturesInsideBounds, const bool isLinearMap, const QString& debugFileName)
{
  if (map->size() == 0)
  {
    LOG_DEBUG("Skipping cropping empty map: " << map->getName() << "...");
    return;
  }

  LOG_INFO("Cropping map: " << map->getName() << " for changeset derivation...");
  LOG_VART(MapProjector::toWkt(map->getProjection()));

  MapCropper cropper(bounds);
  cropper.setKeepEntireFeaturesCrossingBounds(keepEntireFeaturesCrossingBounds);
  cropper.setKeepOnlyFeaturesInsideBounds(keepOnlyFeaturesInsideBounds);
  //LOG_DEBUG(cropper.getInitStatusMessage());
  cropper.apply(map);
  LOG_DEBUG(cropper.getCompletedStatusMessage());

  // Clean up straggling nodes in that are the result of cropping. Its ok to ignore info tags when
  // dealing with only linear features, as all nodes in the data being conflated should be way nodes
  // with no information.
  // TODO: This can be removed now, since its already happening in MapCropper, right?
  SuperfluousNodeRemover::removeNodes(map, isLinearMap);

  LOG_VART(MapProjector::toWkt(map->getProjection()));
  OsmMapWriterFactory::writeDebugMap(map, debugFileName);
  LOG_DEBUG("Cropped map: " << map->getName() << " size: " << map->size());
}

void ChangesetReplacementCreator::_removeUnsnappedImmediatelyConnectedOutOfBoundsWays(
  OsmMapPtr& map)
{
  LOG_INFO(
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
  //LOG_DEBUG(removeVis.getInitStatusMessage());
  map->visitRw(removeVis);
  LOG_DEBUG(removeVis.getCompletedStatusMessage());

  LOG_VART(MapProjector::toWkt(map->getProjection()));
  OsmMapWriterFactory::writeDebugMap(map, map->getName() + "-unsnapped-removed");
}

void ChangesetReplacementCreator::_excludeFeaturesFromChangesetDeletion(OsmMapPtr& map,
                                                                        const QString& boundsStr)
{
  if (map->size() == 0)
  {
    return;
  }

  LOG_INFO(
    "Marking reference features in: " << map->getName() << " for exclusion from deletion...");

  std::shared_ptr<InBoundsCriterion> boundsCrit(new InBoundsCriterion(_boundsOpts.inBoundsStrict));
  boundsCrit->setBounds(GeometryUtils::envelopeFromConfigString(boundsStr));
  boundsCrit->setOsmMap(map.get());
  std::shared_ptr<NotCriterion> notInBoundsCrit(new NotCriterion(boundsCrit));
  std::shared_ptr<ChainCriterion> elementCrit(
    new ChainCriterion(std::shared_ptr<WayCriterion>(new WayCriterion()), notInBoundsCrit));

  RecursiveSetTagValueOp tagSetter(MetadataTags::HootChangeExcludeDelete(), "yes", elementCrit);
  LOG_DEBUG(tagSetter.getInitStatusMessage());
  tagSetter.apply(map);
  LOG_DEBUG(tagSetter.getCompletedStatusMessage());

  LOG_VART(MapProjector::toWkt(map->getProjection()));
  OsmMapWriterFactory::writeDebugMap(map, map->getName() + "-after-delete-exclude-tags");
}

bool ChangesetReplacementCreator::_isNetworkConflate() const
{
  return
    ConfigOptions().getMatchCreators().contains(
      QString::fromStdString(NetworkMatchCreator::className()));
}

void ChangesetReplacementCreator::_setGlobalOpts(const QString& boundsStr)
{
  conf().set(ConfigOptions::getChangesetXmlWriterAddTimestampKey(), false);
  conf().set(ConfigOptions::getReaderAddSourceDatetimeKey(), false);
  conf().set(ConfigOptions::getWriterIncludeCircularErrorTagsKey(), false);
  conf().set(ConfigOptions::getConvertBoundingBoxKey(), boundsStr);
  // For this being enabled to have any effect,
  // convert.bounding.box.keep.immediately.connected.ways.outside.bounds must be enabled as well.
  conf().set(ConfigOptions::getConvertBoundingBoxTagImmediatelyConnectedOutOfBoundsWaysKey(), true);
  // will have to see if setting this to false causes problems in the future...
  conf().set(ConfigOptions::getConvertRequireAreaForPolygonKey(), false);
  // turn on for testing only
  //conf().set(ConfigOptions::getDebugMapsWriteKey(), true);

  // These don't change between scenarios (or at least haven't needed to yet).
  _boundsOpts.loadRefKeepOnlyInsideBounds = false;
  _boundsOpts.cookieCutKeepOnlyInsideBounds = false;
  _boundsOpts.changesetRefKeepOnlyInsideBounds = false;
}

void ChangesetReplacementCreator::_parseConfigOpts(
  const bool lenientBounds, const GeometryTypeCriterion::GeometryType& geometryType)
{
  // These settings have been are customized for each geometry type and bounds handling preference.
  // They were derived from small test cases, so we may need to do some tweaking as we encounter
  // real world data.

  if (geometryType == GeometryTypeCriterion::GeometryType::Point)
  {
    if (lenientBounds)
    {
      const QString msg = "--lenient-bounds option ignored with point datasets.";
      if (_geometryFiltersSpecified)
      {
        LOG_WARN(msg);
      }
      else
      {
        LOG_DEBUG(msg);
      }
    }

    _boundsOpts.loadRefKeepEntireCrossingBounds = false;
    _boundsOpts.loadRefKeepImmediateConnectedWaysOutsideBounds = false;
    _boundsOpts.loadSecKeepEntireCrossingBounds = false;
    _boundsOpts.loadSecKeepOnlyInsideBounds = false;
    _boundsOpts.cookieCutKeepEntireCrossingBounds = false;
    _boundsOpts.changesetRefKeepEntireCrossingBounds = false;
    _boundsOpts.changesetSecKeepEntireCrossingBounds = false;
    _boundsOpts.changesetSecKeepOnlyInsideBounds = true;
    _boundsOpts.changesetAllowDeletingRefOutsideBounds = true;
    _boundsOpts.inBoundsStrict = false;
  }
  else if (geometryType == GeometryTypeCriterion::GeometryType::Line)
  {
    if (lenientBounds)
    {
      _boundsOpts.loadRefKeepEntireCrossingBounds = true;
      _boundsOpts.loadRefKeepImmediateConnectedWaysOutsideBounds = true;
      _boundsOpts.loadSecKeepEntireCrossingBounds = true;
      _boundsOpts.loadSecKeepOnlyInsideBounds = false;
      _boundsOpts.cookieCutKeepEntireCrossingBounds = false;
      _boundsOpts.changesetRefKeepEntireCrossingBounds = true;
      _boundsOpts.changesetSecKeepEntireCrossingBounds = true;
      _boundsOpts.changesetSecKeepOnlyInsideBounds = false;
      _boundsOpts.changesetAllowDeletingRefOutsideBounds = true;
      _boundsOpts.inBoundsStrict = false;
    }
    else
    {
      _boundsOpts.loadRefKeepEntireCrossingBounds = true;
      _boundsOpts.loadRefKeepImmediateConnectedWaysOutsideBounds = false;
      _boundsOpts.loadSecKeepEntireCrossingBounds = false;
      _boundsOpts.loadSecKeepOnlyInsideBounds = false;
      _boundsOpts.cookieCutKeepEntireCrossingBounds = false;
      _boundsOpts.changesetRefKeepEntireCrossingBounds = true;
      _boundsOpts.changesetSecKeepEntireCrossingBounds = true;
      _boundsOpts.changesetSecKeepOnlyInsideBounds = false;
      _boundsOpts.changesetAllowDeletingRefOutsideBounds = false;
      _boundsOpts.inBoundsStrict = false;

      // Conflate way joining needs to happen later in the post ops for strict linear replacements.
      // Changing the default ordering of the post ops to accomodate this had detrimental effects
      // on other conflation. The best location seems to be at the end just before tag truncation.
      // would like to get rid of this...isn't a foolproof fix by any means if the conflate post
      // ops end up getting reordered for some reason.

      LOG_VART(conf().getList(ConfigOptions::getConflatePostOpsKey()));
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
  else if (geometryType == GeometryTypeCriterion::GeometryType::Polygon)
  {
    if (lenientBounds)
    {
      _boundsOpts.loadRefKeepEntireCrossingBounds = true;
      _boundsOpts.loadRefKeepImmediateConnectedWaysOutsideBounds = false;
      _boundsOpts.loadSecKeepEntireCrossingBounds = true;
      _boundsOpts.loadSecKeepOnlyInsideBounds = false;
      _boundsOpts.cookieCutKeepEntireCrossingBounds = true;
      _boundsOpts.changesetRefKeepEntireCrossingBounds = true;
      _boundsOpts.changesetSecKeepEntireCrossingBounds = true;
      _boundsOpts.changesetSecKeepOnlyInsideBounds = false;
      _boundsOpts.changesetAllowDeletingRefOutsideBounds = true;
      _boundsOpts.inBoundsStrict = false;
    }
    else
    {
      _boundsOpts.loadRefKeepEntireCrossingBounds = true;
      _boundsOpts.loadRefKeepImmediateConnectedWaysOutsideBounds = false;
      _boundsOpts.loadSecKeepEntireCrossingBounds = false;
      _boundsOpts.loadSecKeepOnlyInsideBounds = true;
      _boundsOpts.cookieCutKeepEntireCrossingBounds = true;
      _boundsOpts.changesetRefKeepEntireCrossingBounds = true;
      _boundsOpts.changesetSecKeepEntireCrossingBounds = false;
      _boundsOpts.changesetSecKeepOnlyInsideBounds = true;
      _boundsOpts.changesetAllowDeletingRefOutsideBounds = false;
      _boundsOpts.inBoundsStrict = true;
    }
  }
  else
  {
    // shouldn't ever get here
    throw IllegalArgumentException("Invalid geometry type.");
  }

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
