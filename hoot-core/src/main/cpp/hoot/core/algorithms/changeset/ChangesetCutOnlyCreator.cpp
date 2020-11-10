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
#include "ChangesetCutOnlyCreator.h"

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
#include <hoot/core/criterion/StatusCriterion.h>
#include <hoot/core/criterion/TagCriterion.h>
#include <hoot/core/criterion/TagKeyCriterion.h>
#include <hoot/core/criterion/WayNodeCriterion.h>

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
#include <hoot/core/geometry/GeometryUtils.h>
#include <hoot/core/elements/MapProjector.h>
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

HOOT_FACTORY_REGISTER(ChangesetReplacement, ChangesetCutOnlyCreator)

ChangesetCutOnlyCreator::ChangesetCutOnlyCreator() :
_boundsInterpretation(BoundsInterpretation::Lenient),
_conflationEnabled(false),
_changesetId("1"),
_currentChangeDerivationPassIsLinear(false),
_maxFilePrintLength(ConfigOptions().getProgressVarPrintLengthMax() * 2),
_fullReplacement(false),
_geometryFiltersSpecified(false),
_chainReplacementFilters(false),
_chainRetainmentFilters(false),
_numChanges(0)
{
}

void ChangesetCutOnlyCreator::setChangesetOptions(
  const bool printStats, const QString& statsOutputFile, const QString osmApiDbUrl)
{
  LOG_VARD(printStats);
  LOG_VARD(statsOutputFile);
  LOG_VARD(osmApiDbUrl);
  _changesetCreator.reset(new ChangesetCreator(printStats, statsOutputFile, osmApiDbUrl));
}

void ChangesetCutOnlyCreator::setGeometryFilters(const QStringList& filterClassNames)
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

void ChangesetCutOnlyCreator::_setInputFilter(
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

void ChangesetCutOnlyCreator::setReplacementFilters(const QStringList& filterClassNames)
{
  LOG_VART(filterClassNames.size());
  if (filterClassNames.size() > 0)
  {
    LOG_TRACE("Creating replacement filter...");
    _setInputFilter(_replacementFilter, filterClassNames, _chainReplacementFilters);
  }
}

void ChangesetCutOnlyCreator::setRetainmentFilters(const QStringList& filterClassNames)
{
  LOG_VARD(filterClassNames.size());
  if (filterClassNames.size() > 0)
  {
    LOG_DEBUG("Creating retainment filter...");
    _setInputFilter(_retainmentFilter, filterClassNames, _chainRetainmentFilters);
  }
}

void ChangesetCutOnlyCreator::_setInputFilterOptions(Settings& opts,
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

void ChangesetCutOnlyCreator::setReplacementFilterOptions(const QStringList& optionKvps)
{
  LOG_DEBUG("Creating replacement filter options...");
  _setInputFilterOptions(_replacementFilterOptions, optionKvps);
}

QString ChangesetCutOnlyCreator::_boundsInterpretationToString(
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

void ChangesetCutOnlyCreator::_printJobDescription() const
{
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
  str += "\nAt Bounds: " + GeometryUtils::polygonToString(_replacementBounds);
  str += "\nOutput Changeset: ..." + _output.right(_maxFilePrintLength);
  LOG_STATUS(str);

  str = "";
  str += "\nBounds interpretation: " + boundsStr;
  str += "\nReplacement is: " + replacementTypeStr;
  str += "\nGeometry filters: " + geometryFiltersStr;
  str += "\nReplacement filter: " + replacementFiltersStr;
  str += "\nRetainment filter: " + retainmentFiltersStr;
  str += "\nConflation: " + conflationStr;
  str += "\nCropping database inputs after read: " + cropDbInputOnReadStr;
  LOG_INFO(str);
}

void ChangesetCutOnlyCreator::setRetainmentFilterOptions(const QStringList& optionKvps)
{
  LOG_DEBUG("Creating retainment filter options...");
  _setInputFilterOptions(_retainmentFilterOptions, optionKvps);
}

void ChangesetCutOnlyCreator::create(
  const QString& input1, const QString& input2, const geos::geom::Envelope& bounds,
  const QString& output)
{
  create(input1, input2, GeometryUtils::envelopeToPolygon(bounds), output);
}

void ChangesetCutOnlyCreator::create(
  const QString& input1, const QString& input2,
  const std::shared_ptr<geos::geom::Polygon>& bounds, const QString& output)
{
  QElapsedTimer timer;
  timer.start();

  LOG_VARD(input1);
  LOG_VARD(input2);
  if (bounds)
  {
    LOG_VARD(GeometryUtils::polygonToString(bounds));
  }
  LOG_VARD(output);

  // INPUT VALIDATION AND SETUP

  _input1 = input1;
  _input1Map.reset();
  _input2 = input2;
  _input2Map.reset();
  _output = output;
  _replacementBounds = bounds;
  _validateInputs();
  _setGlobalOpts();
  _printJobDescription();

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
    LOG_INFO("******************************************");
    LOG_STATUS(
      "Generating " << GeometryTypeCriterion::typeToString(itr.key()) << " diff maps for " <<
      "changeset derivation with ID: " << _changesetId << ". Pass: " << passCtr << " / " <<
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

    _processMaps(refMap, conflatedMap, refFilter, secFilter, itr.key(), linearFilterClassNames);

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
        "Adding ref map of size: " << StringUtils::formatLargeNumber(refMap->size()) <<
        " and conflated map of size: " << StringUtils::formatLargeNumber(conflatedMap->size()) <<
        " to changeset derivation queue for geometry type: " <<
        GeometryTypeCriterion::typeToString(itr.key()) << "...");
      refMaps.append(refMap);
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

  // UPDATE 8/17/20: Feature de-duplication appears no longer necessary after applying the ID
  // synchronization below.

  // Synchronize IDs between the two maps in order to cut down on unnecessary changeset
  // create/delete statements. This must be done with the ref/sec maps separated to avoid ID
  // conflicts.
  // TODO: move this to inside the geometry pass loop?
  _synchronizeIds(refMaps, conflatedMaps);

  // CHANGESET GENERATION

  LOG_STATUS("Generating changeset for " << refMaps.size() << " sets of maps...");

  // Derive a changeset between the ref and conflated maps that replaces ref features with
  // secondary features within the bounds and write it out.
  _changesetCreator->setIncludeReviews(
    _conflationEnabled && ConfigOptions().getChangesetReplacementPassConflateReviews());
  // We have some instances where modify and delete changes are being generated for the same
  // element, which causes error during changeset application. Eventually, we should eliminate their
  // causes, but for now we'll activate changeset cleaning to get rid of the delete statements.
  // Unfortunately, this will make the changeset writing memory bound.
  _changesetCreator->setClean(true);
  _changesetCreator->create(refMaps, conflatedMaps, _output);
  _numChanges = _changesetCreator->getNumTotalChanges();

  LOG_STATUS(
    "Derived replacement changeset: ..." << _output.right(_maxFilePrintLength) << " with " <<
    StringUtils::formatLargeNumber(_numChanges) << " changes in " <<
    StringUtils::millisecondsToDhms(timer.elapsed()) << " total.");
}

OsmMapPtr ChangesetCutOnlyCreator::_getMapByGeometryType(const QList<OsmMapPtr>& maps,
                                                         const QString& geometryTypeStr)
{
  for (int i = 0; i < maps.size(); i++)
  {
    OsmMapPtr map = maps.at(i);
    // TODO: hackish
    if (map->getName().contains(geometryTypeStr))
    {
      return map;
    }
  }
  return OsmMapPtr();
}

void ChangesetCutOnlyCreator::_processMaps(
  OsmMapPtr& refMap, OsmMapPtr& conflatedMap, const ElementCriterionPtr& refFeatureFilter,
  const ElementCriterionPtr& secFeatureFilter,
  const GeometryTypeCriterion::GeometryType& geometryType,
  const QStringList& linearFilterClassNames)
{
  LOG_VARD(toString());

  LOG_VARD(linearFilterClassNames);
  LOG_VARD(refFeatureFilter->toString());
  LOG_VARD(secFeatureFilter->toString());

  // INPUT VALIDATION AND SETUP

  _parseConfigOpts(geometryType);

  // DATA LOAD AND INITIAL PREP

  // load the data to replace
  refMap = _loadRefMap(geometryType);
  MemoryUsageChecker::getInstance().check();

  // Drop all C&R specific metadata tags which should not exist yet, just in case they got in the
  // input somehow.
  _removeMetadataTags(refMap);

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
  if (_currentChangeDerivationPassIsLinear)
  {
    // If we have a lenient bounds requirement and linear features, we need to exclude all ways
    // outside of the bounds but immediately connected to a way crossing the bounds from deletion.
    _addChangesetDeleteExclusionTags(refMap);
  }

  // Prune the ref dataset down to just the geometry types specified by the filter, so we don't end
  // up modifying anything else.
  _filterFeatures(
    refMap, refFeatureFilter, geometryType, conf(),
    _changesetId + "-ref-after-" + GeometryTypeCriterion::typeToString(geometryType) + "-pruning");

  // load the data that we're replacing with
  OsmMapPtr secMap = _loadSecMap(geometryType);
  MemoryUsageChecker::getInstance().check();

  _removeMetadataTags(secMap);

  if (markMissing)
  {
    _markElementsWithMissingChildren(secMap);
  }

  // Prune the sec dataset down to just the feature types specified by the filter, so we don't end
  // up modifying anything else.
  const Settings secFilterSettings =
    _replacementFilterOptions.size() == 0 ? conf() : _replacementFilterOptions;
  _filterFeatures(
    secMap, secFeatureFilter, geometryType, secFilterSettings,
    _changesetId + "sec-after-" + GeometryTypeCriterion::typeToString(geometryType) + "-pruning");

  const int refMapSize = refMap->size();
  // If the secondary dataset is empty here and the ref dataset isn't, then we'll end up with a
  // changeset with only delete statements if the option to allow deleting reference features is
  // enabled.
  const int secMapSize = secMap->size();
  LOG_VARD(refMapSize);
  LOG_VARD(secMapSize);

  const QString geometryTypeStr = GeometryTypeCriterion::typeToString(geometryType);
  bool bothMapsEmpty = false;
  if (refMapSize == 0 && secMapSize == 0)
  {
    LOG_STATUS("Both maps empty, so skipping data removal...");
    bothMapsEmpty = true;
  }

  // CUT

  // cut the shape of the secondary data out of the reference data
  OsmMapPtr cookieCutRefMap = _getCookieCutMap(refMap, secMap, geometryType);
  const int cookieCutSize = cookieCutRefMap->size();
  LOG_VARD(cookieCutSize);
  const int dataRemoved = refMapSize - cookieCutSize;
  LOG_VARD(dataRemoved);

  if (!bothMapsEmpty)
  {
    // sec map size may have changed after call to _getCookieCutMap
    LOG_STATUS(
      "Replacing " << StringUtils::formatLargeNumber(dataRemoved) << " " << geometryTypeStr <<
      " feature(s) with " << StringUtils::formatLargeNumber(secMap->size()) << " " <<
      geometryTypeStr << " feature(s)...");
  }

  // At one point it was necessary to re-number the relations in the sec map, as they could have ID
  // overlap with those in the cookie cut ref map at this point. It seemed that this was due to the
  // fact that relations in the two maps had been added independently of each other during cropping.
  // However, after some refactoring this doesn't seem to be the case anymore. If we run into this
  // situation again, we can go back in the history to resurrect the use of the ElementIdRemapper
  // for relations here, which has since been removed from the codebase.

  // Combine the cookie cut ref map back with the secondary map, so we can conflate the two
  // together if needed.
  MapUtils::combineMaps(cookieCutRefMap, secMap, false);
  OsmMapWriterFactory::writeDebugMap(cookieCutRefMap, _changesetId + "-combined-before-conflation");
  secMap.reset();
  LOG_VARD(cookieCutRefMap->size());

  // CONFLATE / CLEAN

  // conflate the cookie cut ref map with the sec map if conflation is enabled

  // TODO: rename var since this map isn't necessary conflated; also rename everything in terms of
  // "toReplace" and "replacement"
  conflatedMap = cookieCutRefMap;
  if (secMapSize > 0)
  {
    if (_conflationEnabled)
    {
      // conflation cleans beforehand
      _conflate(conflatedMap);
      conflatedMap->setName("conflated-" + GeometryTypeCriterion::typeToString(geometryType));

      if (!ConfigOptions().getChangesetReplacementPassConflateReviews())
      {
        // remove all conflate reviews
        _removeConflateReviews(conflatedMap);
      }
    }
    // This is a little misleading to only clean when the sec map has elements, however a test fails
    // if we don't. May need further investigation.
    else
    {
      _clean(conflatedMap);
      conflatedMap->setName("cleaned-" + GeometryTypeCriterion::typeToString(geometryType));
    }
  }

  // SNAP

  if (_currentChangeDerivationPassIsLinear)
  {
    // Snap secondary features back to reference features if dealing with linear features where
    // ref features may have been cut along the bounds. We're being lenient here by snapping
    // secondary to reference *and* allowing conflated data to be snapped to either dataset. We only
    // want to snap ways of like types together, so we'll loop through each applicable linear type
    // and snap them separately.

    LOG_INFO("Snapping unconnected ways to each other in replacement map...");
    QStringList snapWayStatuses("Input2");
    snapWayStatuses.append("Conflated");
    QStringList snapToWayStatuses("Input1");
    snapToWayStatuses.append("Conflated");
    for (int i = 0; i < linearFilterClassNames.size(); i++)
    {
      _snapUnconnectedWays(
        conflatedMap, snapWayStatuses, snapToWayStatuses, linearFilterClassNames.at(i), false,
        _changesetId + "-conflated-snapped-sec-to-ref-1");
    }

    // After snapping, perform joining to prevent unnecessary create/delete statements for the ref
    // data in the resulting changeset and generate modify statements instead.
    ReplacementSnappedWayJoiner wayJoiner(refIdToVersionMappings);
    wayJoiner.join(conflatedMap);
    LOG_VART(MapProjector::toWkt(conflatedMap->getProjection()));
  }

  // PRE-CHANGESET DERIVATION DATA PREP

  OsmMapPtr immediatelyConnectedOutOfBoundsWays;
  if (_boundsInterpretation == BoundsInterpretation::Lenient &&
      _currentChangeDerivationPassIsLinear)
  {
    // If we're conflating linear features with the lenient bounds requirement, copy the
    // immediately connected out of bounds ref ways to a new temp map. We'll lose those ways once we
    // crop in preparation for changeset derivation. If we don't introduce them back during
    // changeset derivation, they may not end up being snapped back to the replacement data.
    immediatelyConnectedOutOfBoundsWays = _getImmediatelyConnectedOutOfBoundsWays(refMap);
  }

  // Crop the original ref and conflated maps appropriately for changeset derivation.
  //
  // After the change to support polygon bounds, doing this cropping on the secondary data causes
  // output features to be dropped, and it doesn't appear to help when used on the ref data, so
  // disabling it completely for cut only (only one that uses strict bounds). This probably needs
  // more investigation, as well as adding some more cut only test cases.
  if (_boundsInterpretation != ChangesetReplacement::BoundsInterpretation::Strict)
  {
    _cropMapForChangesetDerivation(
      refMap, _boundsOpts.changesetRefKeepEntireCrossingBounds,
      _boundsOpts.changesetRefKeepOnlyInsideBounds, _changesetId +
      "-ref-" + GeometryTypeCriterion::typeToString(geometryType) + "-cropped-for-changeset");
    _cropMapForChangesetDerivation(
      conflatedMap, _boundsOpts.changesetSecKeepEntireCrossingBounds,
      _boundsOpts.changesetSecKeepOnlyInsideBounds, _changesetId +
      "-sec-" + GeometryTypeCriterion::typeToString(geometryType) + "-cropped-for-changeset");
  }

  if (_boundsInterpretation == BoundsInterpretation::Lenient &&
      _currentChangeDerivationPassIsLinear)
  {
    // The non-strict bounds interpretation way replacement workflow benefits from a second
    // set of snapping runs right before changeset derivation due to there being ways connected to
    // replacement ways that fall completely outside of the replacement bounds. However, joining
    // after this snapping caused changeset errors with some datasets and hasn't seem to be needed
    // so far...so skipping it. Note that we're being as lenient as possible with the snapping
    // here, allowing basically anything to join to anything else, which *could* end up causing
    // problems...we'll go with it for now.

    QStringList snapWayStatuses("Input2");
    snapWayStatuses.append("Conflated");
    snapWayStatuses.append("Input1");
    QStringList snapToWayStatuses("Input1");
    snapToWayStatuses.append("Conflated");
    snapToWayStatuses.append("Input2");
    LOG_VARD(linearFilterClassNames);

    // Snap anything that can be snapped per feature type.
    LOG_INFO("Snapping unconnected ways to each other in replacement map...");
    for (int i = 0; i < linearFilterClassNames.size(); i++)
    {
      _snapUnconnectedWays(
        conflatedMap, snapWayStatuses, snapToWayStatuses, linearFilterClassNames.at(i), false,
       _changesetId + "-conflated-snapped-sec-to-ref-2");
    }

    // combine the conflated map with the immediately connected out of bounds ways
    MapUtils::combineMaps(conflatedMap, immediatelyConnectedOutOfBoundsWays, true);
    OsmMapWriterFactory::writeDebugMap(
      conflatedMap, _changesetId + "-conflated-connected-combined");

    // Snap the connected ways to other ways in the conflated map. Mark the ways that were
    // snapped, as we'll need that info in the next step.
    LOG_INFO("Snapping unconnected ways to each other in replacement map...");
    for (int i = 0; i < linearFilterClassNames.size(); i++)
    {
      _snapUnconnectedWays(
        conflatedMap, snapWayStatuses, snapToWayStatuses, linearFilterClassNames.at(i),
        true, _changesetId + "-conflated-snapped-immediately-connected-out-of-bounds");
    }

    // remove any ways that weren't snapped
    _removeUnsnappedImmediatelyConnectedOutOfBoundsWays(conflatedMap);

    // Copy the connected ways back into the ref map as well, so the changeset will derive
    // properly.
    MapUtils::combineMaps(refMap, immediatelyConnectedOutOfBoundsWays, true);
    OsmMapWriterFactory::writeDebugMap(
      conflatedMap, _changesetId + "-ref-connected-combined");

    immediatelyConnectedOutOfBoundsWays.reset();
  }
  if (!ConfigOptions().getChangesetReplacementAllowDeletingReferenceFeaturesOutsideBounds())
  {
    // If we're not allowing the changeset deriver to generate delete statements for reference
    // features outside of the bounds, we need to mark all corresponding ref ways with a custom
    // tag that will cause the deriver to skip deleting them.
    _excludeFeaturesFromChangesetDeletion(refMap);
  }

  // clean up any mistakes introduced
  _cleanup(refMap);
  _cleanup(conflatedMap);

  LOG_VART(refMap->getElementCount());
  LOG_VART(conflatedMap->getElementCount());
}

void ChangesetCutOnlyCreator::_validateInputs()
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
  if (outputFile.exists())
  {
    if (!outputFile.remove())
    {
      throw HootException("Unable to remove changeset output file: " + _output);
    }
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

void ChangesetCutOnlyCreator::_setGlobalOpts()
{
  conf().set(ConfigOptions::getChangesetXmlWriterAddTimestampKey(), false);
  conf().set(ConfigOptions::getReaderAddSourceDatetimeKey(), false);
  conf().set(ConfigOptions::getWriterIncludeCircularErrorTagsKey(), false);
  // This is kind of klunky to set this here, imo. However, its currently the only way to get this
  // bounds to the readers.
  conf().set(
    ConfigOptions::getConvertBoundsKey(), GeometryUtils::polygonToString(_replacementBounds));

  // For this being enabled to have any effect,
  // convert.bounds.keep.immediately.connected.ways.outside.bounds must be enabled as well.
  conf().set(ConfigOptions::getConvertBoundsTagImmediatelyConnectedOutOfBoundsWaysKey(), true);

  // will have to see if setting this to false causes problems in the future...
  conf().set(ConfigOptions::getConvertRequireAreaForPolygonKey(), false);

  // This needs to be lowered a bit to make feature de-duping and/or ID synchronization work. At
  // five decimal places, we're topping out at a little over 1m of difference. If this ends up
  // causing problems, then may need to go back to the original setting = 7.
  conf().set(ConfigOptions::getNodeComparisonCoordinateSensitivityKey(), 5);

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
  conf().set(ConfigOptions::getConvertBoundsRemoveMissingElementsKey(), false);
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

  // We run a custom set of cleaning ops for changeset replacement.
  conf().set(
    ConfigOptions::getMapCleanerTransformsKey(),
    conf().getList(ConfigOptions::getChangesetReplacementMapCleanerTransformsKey()));

  // These don't change between scenarios (or at least we haven't needed to change them yet).
  _boundsOpts.loadRefKeepOnlyInsideBounds = false;
  _boundsOpts.cookieCutKeepOnlyInsideBounds = false;
  _boundsOpts.changesetRefKeepOnlyInsideBounds = false;
}

void ChangesetCutOnlyCreator::_parseConfigOpts(
  const GeometryTypeCriterion::GeometryType& geometryType)
{
  // These settings have been are customized for each geometry type and bounds handling preference.

  if (geometryType == GeometryTypeCriterion::GeometryType::Point)
  {
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
    _boundsOpts.loadRefKeepEntireCrossingBounds = true;
    _boundsOpts.loadSecKeepOnlyInsideBounds = false;
    _boundsOpts.cookieCutKeepEntireCrossingBounds = false;
    _boundsOpts.changesetRefKeepEntireCrossingBounds = true;
    _boundsOpts.changesetSecKeepEntireCrossingBounds = true;
    _boundsOpts.changesetSecKeepOnlyInsideBounds = false;
    _boundsOpts.inBoundsStrict = false;
    if (_boundsInterpretation == BoundsInterpretation::Lenient)
    {
      _boundsOpts.loadRefKeepImmediateConnectedWaysOutsideBounds = true;
      _boundsOpts.loadSecKeepEntireCrossingBounds = true;
      _boundsOpts.changesetAllowDeletingRefOutsideBounds = true;
    }
    else
    {
      _boundsOpts.loadRefKeepImmediateConnectedWaysOutsideBounds = false;
      _boundsOpts.loadSecKeepEntireCrossingBounds = false;
      _boundsOpts.changesetAllowDeletingRefOutsideBounds = false;
    }

    // Conflate way joining needs to happen later in the post ops for strict linear replacements.
    // Changing the default ordering of the post ops to accommodate this had detrimental effects
    // on other conflation. The best location seems to be at the end just before tag truncation.
    // Would like to get rid of this...isn't a foolproof fix by any means if the conflate post
    // ops end up getting re-ordered for some reason.

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
  else if (geometryType == GeometryTypeCriterion::GeometryType::Polygon)
  {
    _boundsOpts.loadRefKeepEntireCrossingBounds = true;
    _boundsOpts.loadRefKeepImmediateConnectedWaysOutsideBounds = false;
    _boundsOpts.cookieCutKeepEntireCrossingBounds = true;
    _boundsOpts.changesetRefKeepEntireCrossingBounds = true;
    if (_boundsInterpretation == BoundsInterpretation::Lenient ||
        _boundsInterpretation == BoundsInterpretation::Hybrid)
    {
      _boundsOpts.loadSecKeepEntireCrossingBounds = true;
      _boundsOpts.loadSecKeepOnlyInsideBounds = false;
      _boundsOpts.changesetSecKeepEntireCrossingBounds = true;
      _boundsOpts.changesetSecKeepOnlyInsideBounds = false;
      _boundsOpts.changesetAllowDeletingRefOutsideBounds = true;
      _boundsOpts.inBoundsStrict = false;
    }
    else
    {
      _boundsOpts.loadSecKeepEntireCrossingBounds = false;
      _boundsOpts.loadSecKeepOnlyInsideBounds = true;
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
  ChangesetCutOnlyCreator::_getDefaultGeometryFilters() const
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
  // duplicated in the output. We used to run a de-duplication routine just before changeset
  // derivation...kind of a band-aid unfortunately...but fortunately we aren't currently doing that
  // anymore :-|

  // The maps will get set on the crits here that need them by the RemoveElementsVisitor later on,
  // right before they are needed.

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

bool ChangesetCutOnlyCreator::_roadFilterExists() const
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
  ChangesetCutOnlyCreator::_getCombinedFilters(
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

      // UPDATE 8/17/20: Used to do this just for roundabouts, but there are some highway types
      // (e.g. highway=pedstrian) that can be polys. Really, probably just need some generic way to
      // make sure no elements of the same type get processed in separate geometry handling loops
      // instead of code like this.

      ElementCriterionPtr updatedGeometryCrit;
      LOG_VART(_roadFilterExists());
      if (_roadFilterExists())
      {
        if (geomType == GeometryTypeCriterion::GeometryType::Line)
        {
          LOG_TRACE("Adding roundabouts to line filter due to presence of road filter...");
          updatedGeometryCrit.reset(
            new OrCriterion(
              geometryCrit,
              std::shared_ptr</*Roundabout*/HighwayCriterion>(new /*Roundabout*/HighwayCriterion())));
        }
        else if (geomType == GeometryTypeCriterion::GeometryType::Polygon)
        {
          LOG_TRACE("Removing roundabouts from polygon filter due to presence of road filter...");
          updatedGeometryCrit.reset(
            new ChainCriterion(
              geometryCrit,
              NotCriterionPtr(
                new NotCriterion(
                  std::shared_ptr</*Roundabout*/HighwayCriterion>(new /*Roundabout*/HighwayCriterion())))));
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
            geometryCrit,
            std::shared_ptr</*Roundabout*/HighwayCriterion>(new /*Roundabout*/HighwayCriterion())));
      }
      else if (geomType == GeometryTypeCriterion::GeometryType::Polygon)
      {
        LOG_TRACE("Removing roundabouts from polygon filter...");
        updatedGeometryCrit.reset(
          new ChainCriterion(
            geometryCrit,
            NotCriterionPtr(
              new NotCriterion(
                std::shared_ptr</*Roundabout*/HighwayCriterion>(new /*Roundabout*/HighwayCriterion())))));
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

OsmMapPtr ChangesetCutOnlyCreator::_loadInputMap(
  const QString& mapName, const QString& inputUrl, const bool useFileIds, const Status& status,
  const bool keepEntireFeaturesCrossingBounds, const bool keepOnlyFeaturesInsideBounds,
  const bool keepImmediatelyConnectedWaysOutsideBounds, const bool warnOnZeroVersions,
  OsmMapPtr& cachedMap)
{
  conf().set(
    ConfigOptions::getConvertBoundsKeepEntireFeaturesCrossingBoundsKey(),
    keepEntireFeaturesCrossingBounds);
  conf().set(
    ConfigOptions::getConvertBoundsKeepOnlyFeaturesInsideBoundsKey(),
   keepOnlyFeaturesInsideBounds);
  conf().set(
    ConfigOptions::getConvertBoundsKeepImmediatelyConnectedWaysOutsideBoundsKey(),
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
      "Loading " << mapName << " map from: ..." << inputUrl.right(_maxFilePrintLength) << "...");
    map.reset(new OsmMap());
    IoUtils::loadMap(map, inputUrl, useFileIds, status);
  }
  else
  {
    // File inputs always have to read in completely before cropping. Rather than read the file
    // completely for each processing pass, we'll read it once and crop it down as needed per pass.
    if (!cachedMap)
    {
      // Clear out the bounding box param temporarily, so that we can read the full map here. Kind
      // of kludgy, but there is no access to it from here via IoUtils::loadMap.
      const QString bbox = conf().getString(ConfigOptions::getConvertBoundsKey());
      conf().set(ConfigOptions::getConvertBoundsKey(), "");

      LOG_STATUS("Loading map from: ..." << inputUrl.right(_maxFilePrintLength) << "...");
      cachedMap.reset(new OsmMap());
      cachedMap->setName(mapName);
      IoUtils::loadMap(cachedMap, inputUrl, useFileIds, status);

      // Restore it back to original.
      conf().set(ConfigOptions::getConvertBoundsKey(), bbox);
    }
    LOG_STATUS(
      "Copying map of size: " << StringUtils::formatLargeNumber(cachedMap->size()) <<
      " from: " << cachedMap->getName() << "...");
    map.reset(new OsmMap(cachedMap));
    IoUtils::cropToBounds(map, _replacementBounds, keepImmediatelyConnectedWaysOutsideBounds);
  }

  if (warnOnZeroVersions)
  {
    // Restore default setting value.
    conf().set(ConfigOptions::getReaderWarnOnZeroVersionElementKey(), false);
  }
  map->setName(mapName);

  LOG_VART(MapProjector::toWkt(map->getProjection()));
  OsmMapWriterFactory::writeDebugMap(
    map, _changesetId + "-" + map->getName() + "-after-cropped-load");

  return map;
}

OsmMapPtr ChangesetCutOnlyCreator::_loadRefMap(
  const GeometryTypeCriterion::GeometryType& geometryType)
{
  // We only care about the zero version warning for the ref data.
  return
    _loadInputMap(
      "ref-" + GeometryTypeCriterion::typeToString(geometryType), _input1, true, Status::Unknown1,
      _boundsOpts.loadRefKeepEntireCrossingBounds, _boundsOpts.loadRefKeepOnlyInsideBounds,
      _boundsOpts.loadRefKeepImmediateConnectedWaysOutsideBounds, true, _input1Map);
}

OsmMapPtr ChangesetCutOnlyCreator::_loadSecMap(
  const GeometryTypeCriterion::GeometryType& geometryType)
{
  if (!_input2.isEmpty())
  {
    return
      _loadInputMap(
        "sec-" + GeometryTypeCriterion::typeToString(geometryType), _input2, false, Status::Unknown2,
        _boundsOpts.loadSecKeepEntireCrossingBounds, _boundsOpts.loadSecKeepOnlyInsideBounds, false,
        true, _input2Map);
  }
  else
  {
    // return an empty map for the cut only workflow
    return OsmMapPtr(new OsmMap());
  }
}

void ChangesetCutOnlyCreator::_removeMetadataTags(const OsmMapPtr& map)
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

void ChangesetCutOnlyCreator::_markElementsWithMissingChildren(OsmMapPtr& map)
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

void ChangesetCutOnlyCreator::_filterFeatures(
  OsmMapPtr& map, const ElementCriterionPtr& featureFilter,
  const GeometryTypeCriterion::GeometryType& /*geometryType*/, const Settings& config,
  const QString& debugFileName)
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
  OsmMapWriterFactory::writeDebugMap(map, debugFileName);
}

OsmMapPtr ChangesetCutOnlyCreator::_getCookieCutMap(
  OsmMapPtr doughMap, OsmMapPtr cutterMap, const GeometryTypeCriterion::GeometryType& geometryType)
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
  OsmMapWriterFactory::writeDebugMap(doughMap, _changesetId + "-dough-map-input");
  OsmMapWriterFactory::writeDebugMap(cutterMap, _changesetId + "-cutter-map-input");

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
    OsmMapWriterFactory::writeDebugMap(doughMap, _changesetId + "-cookie-cut");
    // copy here to avoid changing the ref map passed in as the dough map input
    return OsmMapPtr(new OsmMap(doughMap));
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
        return OsmMapPtr(new OsmMap());
      }
      else
      {
        // If the sec map is empty and we're not doing full replacement, there's nothing in the sec
        // to overlap with the ref, so leave the ref untouched.
        LOG_DEBUG(
          "Nothing in cutter map. Full replacement not enabled, so returning the entire dough " <<
          "map as the map after cutting: " << doughMap->getName() << "...");
        OsmMapWriterFactory::writeDebugMap(doughMap, _changesetId + "-cookie-cut");
        // copy here to avoid changing the ref map passed in as the dough map input
        return OsmMapPtr(new OsmMap(doughMap));
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
        return OsmMapPtr(new OsmMap());
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
        OsmMapPtr cookieCutMap(new OsmMap(doughMap));
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
        // TODO: temp
        //Log::WarningLevel logLevel = Log::getInstance().getLevel();
        //Log::getInstance().setLevel(Log::Trace);
        LOG_STATUS(cropper.getInitStatusMessage());
        cropper.apply(cookieCutMap);
        LOG_INFO(cropper.getCompletedStatusMessage());
        //Log::getInstance().setLevel(logLevel);
        OsmMapWriterFactory::writeDebugMap(cookieCutMap, _changesetId + "-cookie-cut");
        return cookieCutMap;
      }
      else
      {
        // If the sec map is empty and we're not doing full replacement, there's nothing in the sec
        // to overlap with the ref, so leave the ref untouched.
        LOG_DEBUG(
          "Nothing in cutter map for linear features. Full replacement not enabled, so returning "
          "the entire dough map as the map after cutting: " << doughMap->getName() << "...");
        OsmMapWriterFactory::writeDebugMap(doughMap, _changesetId + "-cookie-cut");
        // copy here to avoid changing the ref map passed in as the dough map input
        return OsmMapPtr(new OsmMap(doughMap));
      }
    }
  }

  LOG_VART(doughMap->size());
  LOG_VART(MapProjector::toWkt(doughMap->getProjection()));
  OsmMapWriterFactory::writeDebugMap(doughMap, _changesetId + "-dough-map");;
  LOG_VART(MapProjector::toWkt(cutterMap->getProjection()));

  OsmMapPtr cookieCutMap(new OsmMap(doughMap));
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
  if ((int)cutterMapToUseSize < 3 && MapUtils::mapIsPointsOnly(cutterMapToUse))
  {
    LOG_DEBUG("Creating a cutter shape map transformation for point map...");
    // Make a copy here since we're making destructive changes to the geometry here for alpha shape
    // generation purposes only.
    cutterMapToUse.reset(new OsmMap(cutterMap));
    PointsToPolysConverter pointConverter;
    LOG_INFO(pointConverter.getInitStatusMessage());
    pointConverter.apply(cutterMapToUse);
    LOG_DEBUG(pointConverter.getCompletedStatusMessage());
    MapProjector::projectToWgs84(cutterMapToUse);
  }

  LOG_VART(cutterMapToUse->size());
  OsmMapWriterFactory::writeDebugMap(cutterMapToUse, _changesetId + "-cutter-map-to-use");

  LOG_STATUS(
    "Generating cutter shape map from: " << cutterMapToUse->getName() << " of size: " <<
    StringUtils::formatLargeNumber(cutterMapToUse->size()) << "...");

  // TODO: Alpha shape generation and cookie cutting for line features is our current bottleneck for
  // C&R. Not sure yet if anything else can be done to improve the performance. Turning off covering
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
  OsmMapWriterFactory::writeDebugMap(cutterShapeOutlineMap, _changesetId + "-cutter-shape");

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
  OsmMapWriterFactory::writeDebugMap(cookieCutMap, _changesetId + "-cookie-cut");

  return cookieCutMap;
}

void ChangesetCutOnlyCreator::_conflate(OsmMapPtr& map)
{
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

  LOG_STATUS("Applying pre-conflate operations...");
  NamedOp preOps(ConfigOptions().getConflatePreOps());
  preOps.apply(map);

  LOG_STATUS(
    "Conflating the cookie cut reference map with the secondary map into " << map->getName() <<
    "...");
  UnifyingConflator().apply(map);

  LOG_STATUS("Applying post-conflate operations...");
  NamedOp postOps(ConfigOptions().getConflatePostOps());
  postOps.apply(map);

  MapProjector::projectToWgs84(map);  // conflation works in planar
  LOG_VART(MapProjector::toWkt(map->getProjection()));
  OsmMapWriterFactory::writeDebugMap(map, _changesetId + "-conflated");
  LOG_DEBUG("Conflated map size: " << map->size());
}

void ChangesetCutOnlyCreator::_clean(OsmMapPtr& map)
{
  LOG_STATUS(
    "Cleaning the combined cookie cut reference and secondary maps: " << map->getName() << "...");

  // TODO: since we're never conflating when we call clean, should we remove cleaning ops like
  // IntersectionSplitter?
  MapCleaner().apply(map);

  MapProjector::projectToWgs84(map);  // cleaning works in planar
  LOG_VART(MapProjector::toWkt(map->getProjection()));
  OsmMapWriterFactory::writeDebugMap(map, _changesetId + "-cleaned");
  LOG_DEBUG("Cleaned map size: " << map->size());
}

QMap<ElementId, long> ChangesetCutOnlyCreator::_getIdToVersionMappings(const OsmMapPtr& map) const
{
  LOG_INFO("Mapping element IDs to element versions for: " << map->getName() << "...");

  ElementIdToVersionMapper idToVersionMapper;
  idToVersionMapper.apply(map);
  LOG_DEBUG(idToVersionMapper.getCompletedStatusMessage());
  const QMap<ElementId, long> idToVersionMappings = idToVersionMapper.getMappings();
  LOG_VART(idToVersionMappings.size());
  return idToVersionMappings;
}

void ChangesetCutOnlyCreator::_addChangesetDeleteExclusionTags(OsmMapPtr& map)
{
  LOG_INFO(
    "Setting connected way features outside of bounds to be excluded from deletion for: " <<
    map->getName() << "...");

  // Add the changeset deletion exclusion tag to all connected ways previously tagged during load.

  SetTagValueVisitor addTagVis(MetadataTags::HootChangeExcludeDelete(), "yes");
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
  childDeletionExcludeTagOp.apply(map);
  LOG_DEBUG(childDeletionExcludeTagOp.getCompletedStatusMessage());

  MemoryUsageChecker::getInstance().check();
  OsmMapWriterFactory::writeDebugMap(
    map, _changesetId + "-" + map->getName() + "-after-delete-exclusion-tagging-1");
}

void ChangesetCutOnlyCreator::_snapUnconnectedWays(
  OsmMapPtr& map, const QStringList& snapWayStatuses, const QStringList& snapToWayStatuses,
  const QString& typeCriterionClassName, const bool markSnappedWays, const QString& debugFileName)
{
  LOG_DEBUG(
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
  // This prevents features of different types snapping to each other that shouldn't do so.
  // Arbitrarily picking a score here...may require further tweaking.
  lineSnapper.setMinTypeMatchScore(0.8);
  // Here, we're excluding things unlikely to ever need to be snapped to each other. Arguably, this
  // list could be made part of UnconnectedWaySnapper's config instead.
  lineSnapper.setTypeExcludeKvps(ConfigOptions().getChangesetReplacementSnapExcludeTypes());
  lineSnapper.apply(map);
  LOG_DEBUG(lineSnapper.getCompletedStatusMessage());

  MapProjector::projectToWgs84(map);   // snapping works in planar
  LOG_VART(MapProjector::toWkt(map->getProjection()));
  MemoryUsageChecker::getInstance().check();
  OsmMapWriterFactory::writeDebugMap(map, debugFileName);
}

OsmMapPtr ChangesetCutOnlyCreator::_getImmediatelyConnectedOutOfBoundsWays(
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
  OsmMapPtr connectedWays = MapUtils::getMapSubset(map, copyCrit);
  connectedWays->setName(outputMapName);
  LOG_VART(MapProjector::toWkt(connectedWays->getProjection()));
  MemoryUsageChecker::getInstance().check();
  OsmMapWriterFactory::writeDebugMap(connectedWays, _changesetId + "-connected-ways");
  return connectedWays;
}

void ChangesetCutOnlyCreator::_cropMapForChangesetDerivation(
  OsmMapPtr& map, const bool keepEntireFeaturesCrossingBounds,
  const bool keepOnlyFeaturesInsideBounds, const QString& debugFileName)
{
  if (map->size() == 0)
  {
    LOG_DEBUG("Skipping cropping empty map: " << map->getName() << "...");
    return;
  }

  LOG_INFO("Cropping map: " << map->getName() << " for changeset derivation...");
  LOG_VART(MapProjector::toWkt(map->getProjection()));
  LOG_VARD(keepEntireFeaturesCrossingBounds);
  LOG_VARD(keepOnlyFeaturesInsideBounds);

  MapCropper cropper;
  cropper.setBounds(_replacementBounds);
  cropper.setKeepEntireFeaturesCrossingBounds(keepEntireFeaturesCrossingBounds);
  cropper.setKeepOnlyFeaturesInsideBounds(keepOnlyFeaturesInsideBounds);
  // We're not going to remove missing elements, as we want to have as minimal of an impact on
  // the resulting changeset as possible.
  cropper.setRemoveMissingElements(false);
  cropper.apply(map);
  LOG_DEBUG(cropper.getCompletedStatusMessage());

  MemoryUsageChecker::getInstance().check();
  LOG_VART(MapProjector::toWkt(map->getProjection()));
  OsmMapWriterFactory::writeDebugMap(map, debugFileName);
  LOG_DEBUG("Cropped map: " << map->getName() << " size: " << map->size());
}

void ChangesetCutOnlyCreator::_removeUnsnappedImmediatelyConnectedOutOfBoundsWays(OsmMapPtr& map)
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
  map->visitRw(removeVis);
  LOG_DEBUG(removeVis.getCompletedStatusMessage());

  MemoryUsageChecker::getInstance().check();
  LOG_VART(MapProjector::toWkt(map->getProjection()));
  OsmMapWriterFactory::writeDebugMap(
    map, _changesetId + "-" + map->getName() + "-unsnapped-removed");
}

void ChangesetCutOnlyCreator::_excludeFeaturesFromChangesetDeletion(OsmMapPtr& map)
{
  if (map->size() == 0)
  {
    return;
  }

  LOG_INFO(
    "Marking reference features in: " << map->getName() << " for exclusion from deletion...");

  // Exclude ways and all their children from deletion that are not entirely within the replacement
  // bounds.

  std::shared_ptr<InBoundsCriterion> boundsCrit(new InBoundsCriterion(_boundsOpts.inBoundsStrict));
  boundsCrit->setBounds(_replacementBounds);
  boundsCrit->setOsmMap(map.get());
  std::shared_ptr<NotCriterion> notInBoundsCrit(new NotCriterion(boundsCrit));
  std::shared_ptr<ChainCriterion> elementCrit(
    new ChainCriterion(std::shared_ptr<WayCriterion>(new WayCriterion()), notInBoundsCrit));

  RecursiveSetTagValueOp tagSetter(MetadataTags::HootChangeExcludeDelete(), "yes", elementCrit);
  tagSetter.apply(map);
  LOG_DEBUG(tagSetter.getCompletedStatusMessage());

  MemoryUsageChecker::getInstance().check();
  LOG_VART(MapProjector::toWkt(map->getProjection()));
  OsmMapWriterFactory::writeDebugMap(
    map, _changesetId + "-" + map->getName() + "-after-delete-exclusion-tagging-2");
}

void ChangesetCutOnlyCreator::_cleanup(OsmMapPtr& map)
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

void ChangesetCutOnlyCreator::_removeConflateReviews(OsmMapPtr& map)
{
  LOG_INFO("Removing reviews added during conflation from " << map->getName() << "...");

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
  //LOG_STATUS("\t" << removeVis.getInitStatusMessage());
  map->visitRw(removeVis);
  LOG_DEBUG(removeVis.getCompletedStatusMessage());

  MemoryUsageChecker::getInstance().check();
  LOG_VART(MapProjector::toWkt(map->getProjection()));
  OsmMapWriterFactory::writeDebugMap(
    map, _changesetId + "-" + map->getName() + "-conflate-reviews-removed");
}

void ChangesetCutOnlyCreator::_synchronizeIds(
  const QList<OsmMapPtr>& mapsBeingReplaced, const QList<OsmMapPtr>& replacementMaps)
{
  assert(mapsBeingReplaced.size() == replacementMaps.size());
  for (int i = 0; i < mapsBeingReplaced.size(); i++)
  {
    OsmMapPtr mapBeingReplaced = mapsBeingReplaced.at(i);
    OsmMapPtr replacementMap = replacementMaps.at(i);
    _synchronizeIds(mapBeingReplaced, replacementMap);
  }
}

void ChangesetCutOnlyCreator::_synchronizeIds(
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

  LOG_STATUS("Synchronizing element IDs between maps...");
  LOG_VART(MapProjector::toWkt(mapBeingReplaced->getProjection()));
  LOG_VART(MapProjector::toWkt(replacementMap->getProjection()));

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

}
