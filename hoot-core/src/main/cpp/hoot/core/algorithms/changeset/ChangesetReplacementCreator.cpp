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

ChangesetReplacementCreator::ChangesetReplacementCreator(
  const bool printStats, const QString& statsOutputFile, const QString osmApiDbUrl) :
_fullReplacement(false),
_boundsInterpretation(BoundsInterpretation::Lenient),
_geometryFiltersSpecified(false),
_chainReplacementFilters(false),
_chainRetainmentFilters(false),
_waySnappingEnabled(true),
_conflationEnabled(true),
_cleaningEnabled(true),
_tagOobConnectedWays(false),
_currentChangeDerivationPassIsLinear(false)
{
  _changesetCreator.reset(new ChangesetCreator(printStats, statsOutputFile, osmApiDbUrl));

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
          OrCriterionPtr(new OrCriterion(currentFilter, filter));
      }

      if (filter->getGeometryType() == GeometryTypeCriterion::GeometryType::Line)
      {
        _linearFilterClassNames.append(filterClassName);
      } 
    } 
  }

  // have to call this method to keep filtering from erroring...shouldn't have to...should just init
  // itself internally when no geometry filters are specified
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

QString ChangesetReplacementCreator::_boundsInterpretationToString(
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

QString ChangesetReplacementCreator::_getJobDescription(
  const QString& input1, const QString& input2, const QString& bounds,
  const QString& output) const
{
  const int maxFilePrintLength = ConfigOptions().getProgressVarPrintLengthMax();
  QString boundsStr = "Bounds calculation is " +
    _boundsInterpretationToString(_boundsInterpretation);
  const QString replacementTypeStr = _fullReplacement ? "full" : "overlapping only";
  QString geometryFiltersStr = "are ";
  if (!_geometryFiltersSpecified)
  {
    geometryFiltersStr += "not ";
  }
  geometryFiltersStr += "specified";
  QString replacementFiltersStr = "is ";
  if (!_replacementFilter)
  {
    replacementFiltersStr += "not ";
  }
  replacementFiltersStr += "specified";
  QString retainmentFiltersStr = "is ";
  if (!_retainmentFilter)
  {
    retainmentFiltersStr += "not ";
  }
  retainmentFiltersStr += "specified";
  QString conflationStr = "is ";
  if (!_conflationEnabled)
  {
    conflationStr += "not ";
  }
  conflationStr += "enabled";
  QString cleaningStr = "is ";
  if (!_cleaningEnabled)
  {
    cleaningStr += "not ";
  }
  cleaningStr += "enabled";
  QString waySnappingStr = "is ";
  if (!_waySnappingEnabled)
  {
    waySnappingStr += "not ";
  }
  waySnappingStr += "enabled";
  QString oobWayHandlingStr = "is ";
  if (!_tagOobConnectedWays)
  {
    oobWayHandlingStr += "not ";
  }
  oobWayHandlingStr += "enabled";

  QString str;
  str += "Deriving replacement output changeset:";
  str += "\nBeing replaced: ..." + input1.right(maxFilePrintLength);
  str += "\nReplacing with ..." + input2.right(maxFilePrintLength);
  str += "\nOutput Changeset: ..." + output.right(maxFilePrintLength);
  str += "\nBounds: " + bounds + "; " + boundsStr;
  str += "\nReplacement is: " + replacementTypeStr;
  str += "\nGeometry filters: " + geometryFiltersStr;
  str += "\nReplacement filter: " + replacementFiltersStr;
  str += "\nRetainment filter: " + retainmentFiltersStr;
  str += "\nConflation: " + conflationStr;
  str += "\nCleaning: " + cleaningStr;
  str += "\nWay snapping: " + waySnappingStr;
  str += "\nOut of bounds way handling: " + oobWayHandlingStr;
  return str;
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
  // INPUT VALIDATION AND SETUP

  _validateInputs(input1, input2);
  const QString boundsStr = GeometryUtils::envelopeToConfigString(bounds);
  _setGlobalOpts(boundsStr);

  LOG_DEBUG(_getJobDescription(input1, input2, boundsStr, output));

  // If a retainment filter was specified, we'll AND it together with each geometry type filter to
  // further restrict what reference data gets replaced in the final changeset.
  const QMap<GeometryTypeCriterion::GeometryType, ElementCriterionPtr> refFilters =
    _getCombinedFilters(_retainmentFilter);
  // If a replacement filter was specified, we'll AND it together with each geometry type filter to
  // further restrict what secondary replacement data goes into the final changeset.
  const QMap<GeometryTypeCriterion::GeometryType, ElementCriterionPtr> secFilters =
    _getCombinedFilters(_replacementFilter);

  // DIFF CALCULATION

  // Since data with different geometry types require different settings, we'll calculate a separate
  // pair of before/after maps for each geometry type.

  QList<OsmMapPtr> refMaps;
  QList<OsmMapPtr> conflatedMaps;
  int passCtr = 1;
  for (QMap<GeometryTypeCriterion::GeometryType, ElementCriterionPtr>::const_iterator itr =
         refFilters.begin(); itr != refFilters.end(); ++itr)
  {
    LOG_STATUS("******************************************");
    LOG_STATUS(
      "Preparing maps for changeset derivation given geometry type: "<<
      GeometryTypeCriterion::typeToString(itr.key()) << ". Pass: " << passCtr << " / " <<
      refFilters.size() << "...");

    OsmMapPtr refMap;
    // This is a bit of a misnomer after recent changes, as this map may have only been cleaned by
    // this point and not actually conflated with anything.
    OsmMapPtr conflatedMap;
    QStringList linearFilterClassNames;
    if (itr.key() == GeometryTypeCriterion::GeometryType::Line)
    {
      linearFilterClassNames = _linearFilterClassNames;
    }

    ElementCriterionPtr refFilter = itr.value();
    ElementCriterionPtr secFilter = secFilters[itr.key()];

    _getMapsForGeometryType(
      refMap, conflatedMap, input1, input2, boundsStr, refFilter, secFilter, itr.key(),
      linearFilterClassNames);

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
    LOG_WARN("No features remain after filtering, so no changeset will be generated.");
    return;
  }
  if (refMaps.size() != conflatedMaps.size())
  {
    throw HootException("Replacement changeset derivation internal map count mismatch error.");
  }

  // CLEANUP

  // Due to the mixed relations processing explained in _getDefaultGeometryFilters, we may have
  // some duplicated features that need to be cleaned up before we generate the changesets. This
  // is kind of a band-aid :-(

  // If we have the maps for only one geometry type, then there isn't a possibility of duplication
  // created by the replacement operation.
  if (ConfigOptions().getChangesetReplacementDeduplicateCalculatedMaps() && refMaps.size() > 1)
  {
    // Not completely sure at this point if we need to dedupe ref maps. Doing so breaks the
    // roundabouts test and adds an extra relation to the out of spec test when we do intra-map
    // de-duping. Mostly worried that not doing so could break the overlapping only replacement
    // (non-full) scenario...we'll see...
    //_dedupeMaps(refMaps);
    _dedupeMaps(conflatedMaps);
  }

  // CHANGESET GENERATION

  // Derive a changeset between the ref and conflated maps that replaces ref features with
  // secondary features within the bounds and write it out.
  _changesetCreator->setIncludeReviews(
    _conflationEnabled && ConfigOptions().getChangesetReplacementPassConflateReviews());
  _changesetCreator->create(refMaps, conflatedMaps, output);

  LOG_STATUS(
    "Derived replacement changeset: ..." <<
    output.right(ConfigOptions().getProgressVarPrintLengthMax()));
}

void ChangesetReplacementCreator::_getMapsForGeometryType(
  OsmMapPtr& refMap, OsmMapPtr& conflatedMap, const QString& input1, const QString& input2,
  const QString& boundsStr, const ElementCriterionPtr& refFeatureFilter,
  const ElementCriterionPtr& secFeatureFilter,
  const GeometryTypeCriterion::GeometryType& geometryType,
  const QStringList& linearFilterClassNames)
{
  LOG_VARD(linearFilterClassNames);
  LOG_VARD(refFeatureFilter->toString());
  LOG_VARD(secFeatureFilter->toString());

  // INPUT VALIDATION AND SETUP

  _parseConfigOpts(geometryType);

  // DATA LOAD AND INITIAL PREP

  // load the ref dataset and crop to the specified aoi
  refMap = _loadRefMap(input1);
  MemoryUsageChecker::getInstance().check();

  // always remove any existing missing child tags
  RemoveTagsVisitor missingChildTagRemover(QStringList(MetadataTags::HootMissingChild()));
  refMap->visitRw(missingChildTagRemover);
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

  _currentChangeDerivationPassIsLinear = !linearFilterClassNames.isEmpty();
  LOG_VART(_currentChangeDerivationPassIsLinear);
  if (_tagOobConnectedWays && _currentChangeDerivationPassIsLinear)
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

  // load the sec dataset and crop to the specified aoi
  OsmMapPtr secMap = _loadSecMap(input2);
  MemoryUsageChecker::getInstance().check();

  secMap->visitRw(missingChildTagRemover);
  if (markMissing)
  {
    _markElementsWithMissingChildren(secMap);
  }

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

  // CUT

  const geos::geom::Envelope replacementBounds = GeometryUtils::envelopeFromConfigString(boundsStr);

  // cut the secondary data out of the reference data
  OsmMapPtr cookieCutRefMap = _getCookieCutMap(refMap, secMap, geometryType, replacementBounds);

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

  // CONFLATE / CLEAN

  // conflate the cookie cut ref map with the sec map if conflation is enabled

  conflatedMap = cookieCutRefMap;
  if (secMapSize > 0)
  {
    if (_conflationEnabled)
    {
      // conflation cleans beforehand
      _conflate(conflatedMap);
      conflatedMap->setName("conflated");

      if (!ConfigOptions().getChangesetReplacementPassConflateReviews())
      {
        // remove all conflate reviews
        _removeConflateReviews(conflatedMap);
      }
    }
    // This is a little misleading to only clean when the sec map has elements, however a test fails
    // if we don't. May need further investigation.
    else if (_cleaningEnabled)
    {
      _clean(conflatedMap);
      conflatedMap->setName("cleaned");
    }
  }

  // SNAP

  if (_currentChangeDerivationPassIsLinear && _waySnappingEnabled)
  {
    // Snap secondary features back to reference features if dealing with linear features where
    // ref features may have been cut along the bounds. We're being lenient here by snapping
    // secondary to reference *and* allowing conflated data to be snapped to either dataset. We only
    // want to snap ways of like types together, so we'll loop through each applicable linear type
    // and snap them separately.

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
  if (_boundsInterpretation == BoundsInterpretation::Lenient &&
      _currentChangeDerivationPassIsLinear)
  {
    // If we're conflating linear features with the lenient bounds requirement, copy the
    // immediately connected out of bounds ways to a new temp map. We'll lose those ways once we
    // crop in preparation for changeset derivation. If we don't introduce them back during
    // changeset derivation, they may not end up being snapped back to the replacement data.

    immediatelyConnectedOutOfBoundsWays = _getImmediatelyConnectedOutOfBoundsWays(refMap);
  }

  // Crop the original ref and conflated maps appropriately for changeset derivation.
  _cropMapForChangesetDerivation(
    refMap, replacementBounds, _boundsOpts.changesetRefKeepEntireCrossingBounds,
    _boundsOpts.changesetRefKeepOnlyInsideBounds, "ref-cropped-for-changeset");
  _cropMapForChangesetDerivation(
    conflatedMap, replacementBounds, _boundsOpts.changesetSecKeepEntireCrossingBounds,
    _boundsOpts.changesetSecKeepOnlyInsideBounds, "sec-cropped-for-changeset");

  if (_boundsInterpretation == BoundsInterpretation::Lenient &&
      _currentChangeDerivationPassIsLinear)
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

    // combine the conflated map with the immediately connected out of bounds ways
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

    // remove any ways that weren't snapped
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

  // clean up introduced mistakes
  _cleanup(refMap);
  _cleanup(conflatedMap);

  LOG_VART(refMap->getElementCount());
  LOG_VART(conflatedMap->getElementCount());
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

  LOG_VARD(_fullReplacement);
  if (_fullReplacement && _retainmentFilter)
  {
    throw IllegalArgumentException(
      "Both full reference data replacement and a reference data retainment filter may not "
      "be specified for replacement changeset derivation.");
  }

  if (ConfigOptions().getConvertOps().size() > 0)
  {
    throw IllegalArgumentException(
      "Replacement changeset derivation does not support convert operations.");
  }
}

void ChangesetReplacementCreator::_setGlobalOpts(const QString& boundsStr)
{
  conf().set(ConfigOptions::getChangesetXmlWriterAddTimestampKey(), false);
  conf().set(ConfigOptions::getReaderAddSourceDatetimeKey(), false);
  conf().set(ConfigOptions::getWriterIncludeCircularErrorTagsKey(), false);
  conf().set(ConfigOptions::getConvertBoundingBoxKey(), boundsStr);

  // For this being enabled to have any effect,
  // convert.bounding.box.keep.immediately.connected.ways.outside.bounds must be enabled as well.
  conf().set(
    ConfigOptions::getConvertBoundingBoxTagImmediatelyConnectedOutOfBoundsWaysKey(),
    _tagOobConnectedWays);

  // will have to see if setting this to false causes problems in the future...
  conf().set(ConfigOptions::getConvertRequireAreaForPolygonKey(), false);

  // This needs to be lowered a bit from the default of 7 to make feature de-duping work...a little
  // concerning, why?
  conf().set(ConfigOptions::getNodeComparisonCoordinateSensitivityKey(), 6);

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

  // Came across a very odd bug in #4101, where if RemoveInvalidMultilineStringMembersVisitor ran
  // as part of the pre-conflate map cleaning during replacement with conflation enabled, the match
  // conflict resolution would slow down to a crawl. When it was removed from the cleaning ops, the
  // conflate operation ran very quickly. So as a not so great workaround (aka hack), removing that
  // pre-op here when running conflation. It still will run post conflate, though. This change had
  // a very minor affect on changeset replacement test output where one test got slightly better
  // output after the change and another slightly worse. See more details in #4101, which is closed,
  // but if we can figure out what's going on at some point maybe this situation can be handled
  // properly.
  if (_conflationEnabled)
  {
    ConfigUtils::removeListOpEntry(
      ConfigOptions::getMapCleanerTransformsKey(),
      QString::fromStdString(RemoveInvalidMultilineStringMembersVisitor::className()));
  }

  // These don't change between scenarios (or at least we haven't needed to change them yet).
  _boundsOpts.loadRefKeepOnlyInsideBounds = false;
  _boundsOpts.cookieCutKeepOnlyInsideBounds = false;
  _boundsOpts.changesetRefKeepOnlyInsideBounds = false;

  // turn on for testing only
  //conf().set(ConfigOptions::getDebugMapsWriteKey(), true);
}

void ChangesetReplacementCreator::_parseConfigOpts(
  const GeometryTypeCriterion::GeometryType& geometryType)
{
  if (!_cleaningEnabled && _conflationEnabled)
  {
    throw IllegalArgumentException(
      "If conflation is enabled during changeset replacement derivation, cleaning cannot be "
      "disabled.");
  }

  // These settings have been are customized for each geometry type and bounds handling preference.
  // They were derived from small test cases, so we may need to do some tweaking as we encounter
  // real world data.

  if (geometryType == GeometryTypeCriterion::GeometryType::Point)
  {
    // TODO: explain these and the rest
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
    if (_boundsInterpretation == BoundsInterpretation::Lenient)
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
      // Changing the default ordering of the post ops to accommodate this had detrimental effects
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
    if (_boundsInterpretation == BoundsInterpretation::Lenient ||
        _boundsInterpretation == BoundsInterpretation::Hybrid)
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

QMap<GeometryTypeCriterion::GeometryType, ElementCriterionPtr>
  ChangesetReplacementCreator::_getDefaultGeometryFilters() const
{
  QMap<GeometryTypeCriterion::GeometryType, ElementCriterionPtr> featureFilters;

  // Unfortunately, trying to process feature types separately by geometry type breaks down when
  // you have relations with mixed geometry types and/or features that belong to multiple relations
  // having different geometry types. The single relation with mixed geometry type membership case
  // is handled with use of the RelationWithGeometryMembersCriterion implementations below. However,
  // bugs may occur during cropping if, say, a polygon geometry was procesed in the line geometry
  // processing loop b/c a line and poly belonged to the same geometry. Haven't seen this actual
  // bug occur yet, but I believe it can...not sure how to prevent it yet.
  //
  // Furthermore, if a feature belongs to two relations with different geometry types, it may be
  // duplicated in the output. This is why we run a de-duplication routine just before changeset
  // derivation...kind of a band-aid unfortunately :-(

  // The maps will get set on the crits here that need them by the RemoveElementsVisitor later on,
  // right before its needed.

  ElementCriterionPtr pointCrit(new PointCriterion());
  std::shared_ptr<RelationWithPointMembersCriterion> relationPointCrit(
    new RelationWithPointMembersCriterion());
  relationPointCrit->setAllowMixedChildren(false);
  OrCriterionPtr pointOr(new OrCriterion(pointCrit, relationPointCrit));
  featureFilters[GeometryTypeCriterion::GeometryType::Point] = pointOr;

  ElementCriterionPtr lineCrit(new LinearCriterion());
  std::shared_ptr<RelationWithLinearMembersCriterion> relationLinearCrit(
    new RelationWithLinearMembersCriterion());
  relationLinearCrit->setAllowMixedChildren(true);
  OrCriterionPtr lineOr(new OrCriterion(lineCrit, relationLinearCrit));
  featureFilters[GeometryTypeCriterion::GeometryType::Line] = lineOr;

  // Poly crit has been converted over to encapsulate RelationWithGeometryMembersCriterion, while
  // the other types have not yet (#4151).
  std::shared_ptr<PolygonCriterion> polyCrit(new PolygonCriterion());
  polyCrit->setAllowMixedChildren(false);
  featureFilters[GeometryTypeCriterion::GeometryType::Polygon] = polyCrit;

  return featureFilters;
}

bool ChangesetReplacementCreator::_roadFilterExists() const
{
  ElementCriterionPtr lineFilter = _geometryTypeFilters[GeometryTypeCriterion::GeometryType::Line];
  if (lineFilter)
  {
    return
      lineFilter->toString()
        .contains(QString::fromStdString(HighwayCriterion::className()).remove("hoot::"));
  }
  return false;
}

QMap<GeometryTypeCriterion::GeometryType, ElementCriterionPtr>
  ChangesetReplacementCreator::_getCombinedFilters(
    std::shared_ptr<ChainCriterion> nonGeometryFilter)
{
  QMap<GeometryTypeCriterion::GeometryType, ElementCriterionPtr> combinedFilters;
  LOG_VART(nonGeometryFilter.get());
  // TODO: may be able to consolidate this duplicated filter handling code inside the if/else
  if (nonGeometryFilter)
  {
    for (QMap<GeometryTypeCriterion::GeometryType, ElementCriterionPtr>::const_iterator itr =
         _geometryTypeFilters.begin(); itr != _geometryTypeFilters.end(); ++itr)
    {
      GeometryTypeCriterion::GeometryType geomType = itr.key();
      LOG_VART(GeometryTypeCriterion::typeToString(geomType));
      ElementCriterionPtr geometryCrit = itr.value();

      // Roundabouts are classified in hoot as a poly type due to being closed ways. We want to
      // make sure they get procesed with the linear features, however, and not with the polys. If
      // they don't, they won't get snapped back to other roads in the output. So if roads were part
      // of the specified geometry filter, we'll move roundabouts from the poly to the linear
      // filter. If no roads were specified in the geometry filter, then roads have been explicitly
      // excluded and we do nothing here. So far this is the only instance where a geometry
      // re-classification for a feature is necessary. If other instances of this occur things could
      // get messy really quick, but we'll only worry about that if it actually happens.

      ElementCriterionPtr updatedGeometryCrit;
      LOG_VART(_roadFilterExists());
      if (_roadFilterExists())
      {
        if (geomType == GeometryTypeCriterion::GeometryType::Line)
        {
          LOG_TRACE("Adding roundabouts to line filter due to presence of road filter...");
          updatedGeometryCrit.reset(
            new OrCriterion(
              geometryCrit, std::shared_ptr<RoundaboutCriterion>(new RoundaboutCriterion())));
        }
        else if (geomType == GeometryTypeCriterion::GeometryType::Polygon)
        {
          LOG_TRACE("Removing roundabouts from polygon filter due to presence of road filter...");
          updatedGeometryCrit.reset(
            new ChainCriterion(
              geometryCrit,
              NotCriterionPtr(
                new NotCriterion(
                  std::shared_ptr<RoundaboutCriterion>(new RoundaboutCriterion())))));
        }
      }
      else
      {
        updatedGeometryCrit = geometryCrit;
      }
      LOG_VART(updatedGeometryCrit->toString());

      combinedFilters[geomType] =
        ChainCriterionPtr(new ChainCriterion(updatedGeometryCrit, nonGeometryFilter));
      LOG_TRACE("New combined filter: " << combinedFilters[geomType]->toString());
    }
  }
  else
  {
    LOG_VARD(_geometryTypeFilters.size());
    if (_geometryTypeFilters.isEmpty())
    {
      _geometryTypeFilters = _getDefaultGeometryFilters();
      // It should be ok that the roundabout filter doesn't get added here, since this list is only
      // for by unconnected way snapping and roundabouts don't fall into that category.
      _linearFilterClassNames =
        ConflatableElementCriterion::getCriterionClassNamesByGeometryType(
          GeometryTypeCriterion::GeometryType::Line);
    }

    for (QMap<GeometryTypeCriterion::GeometryType, ElementCriterionPtr>::const_iterator itr =
         _geometryTypeFilters.begin(); itr != _geometryTypeFilters.end(); ++itr)
    {
      GeometryTypeCriterion::GeometryType geomType = itr.key();
      LOG_VART(GeometryTypeCriterion::typeToString(geomType));
      ElementCriterionPtr geometryCrit = itr.value();

      // See roundabouts handling note in the preceding if statement for more detail. Here we're
      // doing the same thing, except we don't care if a road filter was specified or not since this
      // block of code only gets executed if no geometry filters were specified at all and we're
      // using the defaults.

      ElementCriterionPtr updatedGeometryCrit;
      if (geomType == GeometryTypeCriterion::GeometryType::Line)
      {
        LOG_TRACE("Adding roundabouts to line filter...");
        updatedGeometryCrit.reset(
          new OrCriterion(
            geometryCrit, std::shared_ptr<RoundaboutCriterion>(new RoundaboutCriterion())));
      }
      else if (geomType == GeometryTypeCriterion::GeometryType::Polygon)
      {
        LOG_TRACE("Removing roundabouts from polygon filter...");
        updatedGeometryCrit.reset(
          new ChainCriterion(
            geometryCrit,
            NotCriterionPtr(
              new NotCriterion(
                std::shared_ptr<RoundaboutCriterion>(new RoundaboutCriterion())))));
      }
      else
      {
        updatedGeometryCrit = geometryCrit;
      }
      LOG_VART(updatedGeometryCrit->toString());

      combinedFilters[geomType] = updatedGeometryCrit;
      LOG_TRACE("New combined filter: " << combinedFilters[geomType]->toString());
    }
  }
  LOG_VARD(combinedFilters.size());
  return combinedFilters;
}

OsmMapPtr ChangesetReplacementCreator::_loadRefMap(const QString& input)
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

OsmMapPtr ChangesetReplacementCreator::_loadSecMap(const QString& input)
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

void ChangesetReplacementCreator::_markElementsWithMissingChildren(OsmMapPtr& map)
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

void ChangesetReplacementCreator::_filterFeatures(
  OsmMapPtr& map, const ElementCriterionPtr& featureFilter, const Settings& config,
  const QString& debugFileName)
{
  LOG_STATUS(
    "Filtering features for: " << map->getName() << " based on input filter: " +
    featureFilter->toString() << "...");

  // Negate the input filter, since we're removing everything but what passes the input filter.
  RemoveElementsVisitor elementPruner(true);
  // The criteria must be added before the config or map is set. We may want to change
  // MultipleCriterionConsumerVisitor and RemoveElementsVisitor to make this behavior less brittle.
  elementPruner.addCriterion(featureFilter);
  elementPruner.setConfiguration(config);
  elementPruner.setOsmMap(map.get());
  // If recursion isn't used here, nasty crashes that are hard to track down occur at times. Not
  // completely convinced recursion should be used here, though.
  elementPruner.setRecursive(true);
  LOG_STATUS("\t" << elementPruner.getInitStatusMessage());
  map->visitRw(elementPruner);
  LOG_STATUS("\t" << elementPruner.getCompletedStatusMessage());

  LOG_VART(MapProjector::toWkt(map->getProjection()));
  OsmMapWriterFactory::writeDebugMap(map, debugFileName);
}

OsmMapPtr ChangesetReplacementCreator::_getCookieCutMap(
  OsmMapPtr doughMap, OsmMapPtr cutterMap, const GeometryTypeCriterion::GeometryType& geometryType,
  const geos::geom::Envelope& replacementBounds)
{
  // TODO: This logic has become extremely complicated over time to handle all the different cut
  // and replace use cases. There may be way to simplify some of this logic related to
  // strict/lenient bounds in here by changing some of the initial crop related opts set in
  // _parseConfigOpts...not sure.

  LOG_VARD(_fullReplacement);
  LOG_VARD(_boundsInterpretationToString(_boundsInterpretation));
  LOG_VARD(_currentChangeDerivationPassIsLinear);
  LOG_VARD(doughMap->getElementCount());
  LOG_VARD(cutterMap->size());

  /*
   * lenient/overlapping - cutter shape is all overlapping sec data inside the bounds and
   *                       immediately connected outside the bounds OR all ref data inside the
   *                       bounds and immediately connected outside the bounds (if linear) if sec
   *                       map is empty
     lenient/full        - cutter shape is all ref data inside the bounds and immediately connected
                           outside the bounds (if linear)
     strict/overlapping  - cutter shape is all overlapping sec data inside the bounds
     strict/full         - cutter shape is all ref data inside the bounds

     TODO: add for hybrid?
   */

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
        return OsmMapPtr(new OsmMap());
      }
      else
      {
        // If the sec map is empty and we're not doing full replacement, there's nothing in the sec
        // to overlap with the ref, so leave the ref untouched.
        LOG_DEBUG(
          "Nothing in cutter map. Full replacement not enabled, so returning the entire dough " <<
          "map as the map after cutting: " << doughMap->getName() << "...");
        OsmMapWriterFactory::writeDebugMap(doughMap, "cookie-cut");
        return doughMap;
      }
    }
    else
    {
      if (_fullReplacement && _boundsInterpretation == BoundsInterpretation::Lenient)
      {
        // If our map contains linear features only, the sec map is empty, we're doing full
        // replacement, AND there isn't a strict interpretation of the bounds, we want everything
        // deleted out of the ref inside the replacement bounds and featres immediately connected
        // outside of the bounds. So, return an empty map.
        LOG_DEBUG(
          "Nothing in cutter map for linear features. Full replacement and lenient bounds "
          "interpretation, so returning an empty map as the map after cutting...");
        return OsmMapPtr(new OsmMap());
      }
      else if (_fullReplacement && _boundsInterpretation != BoundsInterpretation::Lenient )
      {
        // With the strict bounds interpretation, full replacement, and an empty secondary map,
        // we want simply the rectangular replacement bounds cut out. No need to use the cookie
        // cutter here. Just use the map cropper.
        LOG_DEBUG(
          "Nothing in cutter map. Full replacement with strict boundsenabled, so cropping out " <<
          "the rectangular bounds area of the dough map to be the map after cutting: " <<
          doughMap->getName() << "...");
        OsmMapPtr cookieCutMap(new OsmMap(doughMap));
        cookieCutMap->setName("cookie-cut");
        MapCropper cropper(replacementBounds);
        cropper.setRemoveSuperflousFeatures(false);
        cropper.setKeepEntireFeaturesCrossingBounds(false);
        cropper.setKeepOnlyFeaturesInsideBounds(false);
        cropper.setInvert(true);
        // We're not going to remove missing elements, as we want to have as minimal of an impact on
        // the resulting changeset as possible.
        cropper.setRemoveMissingElements(false);
        LOG_STATUS("\t" << cropper.getInitStatusMessage());
        cropper.apply(cookieCutMap);
        LOG_STATUS("\t" << cropper.getCompletedStatusMessage());
        OsmMapWriterFactory::writeDebugMap(cookieCutMap, "cookie-cut");
        return cookieCutMap;
      }
      else
      {
        // If the sec map is empty and we're not doing full replacement, there's nothing in the sec
        // to overlap with the ref, so leave the ref untouched.
        LOG_DEBUG(
          "Nothing in cutter map for linear features. Full replacement not enabled, so returning the "
          "entire dough map as the map after cutting: " << doughMap->getName() << "...");
        OsmMapWriterFactory::writeDebugMap(doughMap, "cookie-cut");
        return doughMap;
      }
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
      // TODO: riverbank test fails with missing POIs without this and the single point test has
      // extra POIs in output without this; explain
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
      // TODO: riverbank test fails with missing POIs without this and the single point test has
      // extra POIs in output without this; explain
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
        "No cut shape generated from secondary data when processing geometry type: " <<
        GeometryTypeCriterion::typeToString(geometryType) << ". Is your secondary data empty " <<
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

QMap<ElementId, long> ChangesetReplacementCreator::_getIdToVersionMappings(
  const OsmMapPtr& map) const
{
  LOG_DEBUG("Mapping element IDs to element versions for: " << map->getName() << "...");

  ElementIdToVersionMapper idToVersionMapper;
  LOG_STATUS("\t" << idToVersionMapper.getInitStatusMessage());
  idToVersionMapper.apply(map);
  MemoryUsageChecker::getInstance().check();
  LOG_STATUS("\t" << idToVersionMapper.getCompletedStatusMessage());
  const QMap<ElementId, long> idToVersionMappings = idToVersionMapper.getMappings();
  LOG_VART(idToVersionMappings.size());
  return idToVersionMappings;
}

void ChangesetReplacementCreator::_addChangesetDeleteExclusionTags(OsmMapPtr& map)
{
  LOG_STATUS(
    "Setting connected way features outside of bounds to be excluded from deletion for: " <<
    map->getName() << "...");

  // Add the changeset deletion exclusion tag to all connected ways previously tagged upon load.

  SetTagValueVisitor addTagVis(MetadataTags::HootChangeExcludeDelete(), "yes");
  LOG_STATUS("\t" << addTagVis.getInitStatusMessage());
  ChainCriterion addTagCrit(
    std::shared_ptr<WayCriterion>(new WayCriterion()),
    std::shared_ptr<TagKeyCriterion>(
      new TagKeyCriterion(MetadataTags::HootConnectedWayOutsideBounds())));
  FilteredVisitor deleteExcludeTagVis(addTagCrit, addTagVis);
  map->visitRw(deleteExcludeTagVis);
  LOG_STATUS("\t" << addTagVis.getCompletedStatusMessage());

  // Add the changeset deletion exclusion tag to all children of those connected ways.

  std::shared_ptr<ChainCriterion> childAddTagCrit(
    new ChainCriterion(
      std::shared_ptr<WayCriterion>(new WayCriterion()),
      std::shared_ptr<TagKeyCriterion>(
        new TagKeyCriterion(MetadataTags::HootChangeExcludeDelete()))));
  RecursiveSetTagValueOp childDeletionExcludeTagOp(
    MetadataTags::HootChangeExcludeDelete(), "yes", childAddTagCrit);
  LOG_STATUS("\t" << childDeletionExcludeTagOp.getInitStatusMessage());
  childDeletionExcludeTagOp.apply(map);
  LOG_STATUS("\t" << childDeletionExcludeTagOp.getCompletedStatusMessage());

  MemoryUsageChecker::getInstance().check();
  OsmMapWriterFactory::writeDebugMap(map, map->getName() + "-after-delete-exclusion-tagging");
}

void ChangesetReplacementCreator::_combineMaps(
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

void ChangesetReplacementCreator::_conflate(OsmMapPtr& map)
{
  map->setName("conflated");
  LOG_STATUS(
    "Conflating the cookie cut reference map with the secondary map into " << map->getName() <<
    "...");

  conf().set(ConfigOptions::getWayJoinerLeaveParentIdKey(), true);
  if (_boundsInterpretation != BoundsInterpretation::Lenient)
  {
    // not exactly sure yet why this needs to be done
    conf().set(ConfigOptions::getWayJoinerKey(), WayJoinerAdvanced::className());
  }
  else
  {
    conf().set(ConfigOptions::getWayJoinerKey(), WayJoinerBasic::className());
  }
  conf().set(
    ConfigOptions::getWayJoinerAdvancedStrictNameMatchKey(),
    !UnifyingConflator::isNetworkConflate());

  if (ConfigOptions().getConflateRemoveSuperfluousOps())
  {
    SuperfluousConflateOpRemover::removeSuperfluousOps();
  }

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

void ChangesetReplacementCreator::_removeConflateReviews(OsmMapPtr& map)
{
  LOG_STATUS("Removing reviews added during conflation from " << map->getName() << "...");

  RemoveElementsVisitor removeVis;
  removeVis.addCriterion(ElementCriterionPtr(new RelationCriterion("review")));
  removeVis.addCriterion(
    ElementCriterionPtr(
      new NotCriterion(
        std::shared_ptr<TagCriterion>(
          new TagCriterion(
            MetadataTags::HootReviewType(),
            QString::fromStdString(ReportMissingElementsVisitor::className()))))));
  removeVis.setChainCriteria(true);
  removeVis.setRecursive(false);
  LOG_STATUS("\t" << removeVis.getInitStatusMessage());
  map->visitRw(removeVis);
  LOG_STATUS("\t" << removeVis.getCompletedStatusMessage());

  MemoryUsageChecker::getInstance().check();
  LOG_VART(MapProjector::toWkt(map->getProjection()));
  OsmMapWriterFactory::writeDebugMap(map, map->getName() + "-conflate-reviews-removed");
}

void ChangesetReplacementCreator::_clean(OsmMapPtr& map)
{
  map->setName("cleaned");
  LOG_STATUS(
    "Cleaning the combined cookie cut reference and secondary maps: " << map->getName() << "...");

  // TODO: since we're never conflating when we call clean, should we remove cleaning ops like
  // IntersectionSplitter?
  MapCleaner().apply(map);

  MapProjector::projectToWgs84(map);  // cleaning works in planar
  LOG_VART(MapProjector::toWkt(map->getProjection()));
  OsmMapWriterFactory::writeDebugMap(map, "cleaned");
  LOG_DEBUG("Cleaned map size: " << map->size());
}

void ChangesetReplacementCreator::_snapUnconnectedWays(
  OsmMapPtr& map, const QStringList& snapWayStatuses, const QStringList& snapToWayStatuses,
  const QString& typeCriterionClassName, const bool markSnappedWays, const QString& debugFileName)
{
  LOG_STATUS(
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
  LOG_STATUS("\t" << lineSnapper.getInitStatusMessage());
  lineSnapper.apply(map);
  LOG_STATUS("\t" << lineSnapper.getCompletedStatusMessage());

  MapProjector::projectToWgs84(map);   // snapping works in planar
  LOG_VART(MapProjector::toWkt(map->getProjection()));
  MemoryUsageChecker::getInstance().check();
  OsmMapWriterFactory::writeDebugMap(map, debugFileName);
}

OsmMapPtr ChangesetReplacementCreator::_getImmediatelyConnectedOutOfBoundsWays(
  const ConstOsmMapPtr& map) const
{
  const QString outputMapName = "connected-ways";
  LOG_STATUS(
    "Copying immediately connected out of bounds ways from: " << map->getName() <<
    " to new map: " << outputMapName << "...");

  std::shared_ptr<ChainCriterion> copyCrit(
    new ChainCriterion(
      std::shared_ptr<WayCriterion>(new WayCriterion()),
      std::shared_ptr<TagKeyCriterion>(
        new TagKeyCriterion(MetadataTags::HootConnectedWayOutsideBounds()))));
  OsmMapPtr connectedWays = MapUtils::getMapSubset(map, copyCrit);
  connectedWays->setName(outputMapName);
  LOG_VART(MapProjector::toWkt(connectedWays->getProjection()));
  MemoryUsageChecker::getInstance().check();
  OsmMapWriterFactory::writeDebugMap(connectedWays, "connected-ways");
  return connectedWays;
}

void ChangesetReplacementCreator::_cropMapForChangesetDerivation(
  OsmMapPtr& map, const geos::geom::Envelope& bounds, const bool keepEntireFeaturesCrossingBounds,
  const bool keepOnlyFeaturesInsideBounds, const QString& debugFileName)
{
  if (map->size() == 0)
  {
    LOG_DEBUG("Skipping cropping empty map: " << map->getName() << "...");
    return;
  }

  LOG_STATUS("Cropping map: " << map->getName() << " for changeset derivation...");
  LOG_VART(MapProjector::toWkt(map->getProjection()));

  MapCropper cropper(bounds);
  cropper.setKeepEntireFeaturesCrossingBounds(keepEntireFeaturesCrossingBounds);
  cropper.setKeepOnlyFeaturesInsideBounds(keepOnlyFeaturesInsideBounds);
  // We're not going to remove missing elements, as we want to have as minimal of an impact on
  // the resulting changeset as possible.
  cropper.setRemoveMissingElements(false);
  LOG_STATUS("\t" << cropper.getInitStatusMessage());
  cropper.apply(map);
  LOG_STATUS("\t" << cropper.getCompletedStatusMessage());

  MemoryUsageChecker::getInstance().check();
  LOG_VART(MapProjector::toWkt(map->getProjection()));
  OsmMapWriterFactory::writeDebugMap(map, debugFileName);
  LOG_DEBUG("Cropped map: " << map->getName() << " size: " << map->size());
}

void ChangesetReplacementCreator::_removeUnsnappedImmediatelyConnectedOutOfBoundsWays(
  OsmMapPtr& map)
{
  LOG_STATUS(
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
  LOG_STATUS("\t" << removeVis.getInitStatusMessage());
  map->visitRw(removeVis);
  LOG_STATUS("\t" << removeVis.getCompletedStatusMessage());

  MemoryUsageChecker::getInstance().check();
  LOG_VART(MapProjector::toWkt(map->getProjection()));
  OsmMapWriterFactory::writeDebugMap(map, map->getName() + "-unsnapped-removed");
}

void ChangesetReplacementCreator::_excludeFeaturesFromChangesetDeletion(
  OsmMapPtr& map, const QString& boundsStr)
{
  if (map->size() == 0)
  {
    return;
  }

  LOG_STATUS(
    "Marking reference features in: " << map->getName() << " for exclusion from deletion...");

  std::shared_ptr<InBoundsCriterion> boundsCrit(new InBoundsCriterion(_boundsOpts.inBoundsStrict));
  boundsCrit->setBounds(GeometryUtils::envelopeFromConfigString(boundsStr));
  boundsCrit->setOsmMap(map.get());
  std::shared_ptr<NotCriterion> notInBoundsCrit(new NotCriterion(boundsCrit));
  std::shared_ptr<ChainCriterion> elementCrit(
    new ChainCriterion(std::shared_ptr<WayCriterion>(new WayCriterion()), notInBoundsCrit));

  RecursiveSetTagValueOp tagSetter(MetadataTags::HootChangeExcludeDelete(), "yes", elementCrit);
  LOG_STATUS("\t" << tagSetter.getInitStatusMessage());
  tagSetter.apply(map);
  LOG_STATUS("\t" << tagSetter.getCompletedStatusMessage());

  MemoryUsageChecker::getInstance().check();
  LOG_VART(MapProjector::toWkt(map->getProjection()));
  OsmMapWriterFactory::writeDebugMap(map, map->getName() + "-after-delete-exclude-tags");
}

void ChangesetReplacementCreator::_dedupeMaps(const QList<OsmMapPtr>& maps)
{
  ElementDeduplicator deduper;
  // intra-map de-duping breaks the roundabouts test when ref maps are de-duped
  deduper.setDedupeIntraMap(true);
  // When nodes are removed (cleaned/conflated only), out of spec, single point, and riverbank tests
  // fail, so being a little more strict by removing points instead (node + not a way node).
  std::shared_ptr<PointCriterion> pointCrit(new PointCriterion());
  deduper.setNodeCriterion(pointCrit);
  // this prevents connected ways separated by geometry type from being broken up in the output
  deduper.setFavorMoreConnectedWays(true);

  // See notes in _getDefaultGeometryFilters, but basically the point and poly geometry maps may
  // have duplicates and the line geometry map will not. So dedupe each of the others compared to
  // the line map.

  OsmMapPtr lineMap;
  QList<OsmMapPtr> otherMaps;
  for (int i = 0; i < maps.size(); i++)
  {
    OsmMapPtr map = maps.at(i);
    if (map->getName().contains("line"))
    {
      lineMap = map;
    }
    else
    {
      otherMaps.append(map);
    }
  }

  for (int i = 0; i < otherMaps.size(); i++)
  {
    OsmMapPtr otherMap = otherMaps.at(i);
    // set the point's map to be the map we're removing features from
    pointCrit->setOsmMap(otherMap.get());
    LOG_DEBUG(
      "De-duping map: " << lineMap->getName() << " and " << otherMap->getName() << "...");
    deduper.dedupe(lineMap, otherMap);
  }
}

void ChangesetReplacementCreator::_cleanup(OsmMapPtr& map)
{
  LOG_STATUS("Cleaning up duplicated elements for " << map->getName() << "...");

  // Due to mixed geometry type relations explained in _getDefaultGeometryFilters, we may have
  // introduced some duplicate relation members by this point.
  RemoveDuplicateRelationMembersVisitor dupeMembersRemover;
  LOG_STATUS("\t" << dupeMembersRemover.getInitStatusMessage());
  map->visitRw(dupeMembersRemover);
  LOG_STATUS("\t" << dupeMembersRemover.getCompletedStatusMessage());

  // get rid of straggling nodes
  SuperfluousNodeRemover orphanedNodeRemover;
  LOG_STATUS("\t" << orphanedNodeRemover.getInitStatusMessage());
  orphanedNodeRemover.apply(map);
  LOG_STATUS("\t" << orphanedNodeRemover.getCompletedStatusMessage());

  // This will remove any relations that were already empty or became empty after we removed
  // duplicated members.
  RemoveEmptyRelationsOp emptyRelationRemover;
  LOG_STATUS("\t" << emptyRelationRemover.getInitStatusMessage());
  emptyRelationRemover.apply(map);
  LOG_STATUS("\t" << emptyRelationRemover.getCompletedStatusMessage());

  // get out of orthographic
  MapProjector::projectToWgs84(map);
}

}
