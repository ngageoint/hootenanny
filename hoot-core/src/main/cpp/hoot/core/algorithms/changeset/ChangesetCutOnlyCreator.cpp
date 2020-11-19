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
#include "ChangesetCutOnlyCreator.h"

// Hoot
#include <hoot/core/algorithms/ReplacementSnappedWayJoiner.h>
#include <hoot/core/algorithms/changeset/ChangesetCreator.h>

#include <hoot/core/criterion/ConflatableElementCriterion.h>
#include <hoot/core/criterion/ElementTypeCriterion.h>
#include <hoot/core/criterion/HighwayCriterion.h>
#include <hoot/core/criterion/LinearCriterion.h>
#include <hoot/core/criterion/NotCriterion.h>
#include <hoot/core/criterion/OrCriterion.h>
#include <hoot/core/criterion/PointCriterion.h>
#include <hoot/core/criterion/PolygonCriterion.h>
#include <hoot/core/criterion/RelationWithLinearMembersCriterion.h>
#include <hoot/core/criterion/RelationWithPointMembersCriterion.h>
#include <hoot/core/criterion/RelationWithPolygonMembersCriterion.h>

#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/elements/MapUtils.h>

#include <hoot/core/geometry/GeometryUtils.h>

#include <hoot/core/index/OsmMapIndex.h>

#include <hoot/core/io/OsmMapWriterFactory.h>

#include <hoot/core/ops/WayJoinerOp.h>

#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/ConfigUtils.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/MemoryUsageChecker.h>

#include <hoot/core/visitors/ApiTagTruncateVisitor.h>
#include <hoot/core/visitors/RemoveMissingElementsVisitor.h>

// tgs
#include <tgs/RStarTree/MemoryPageStore.h>

// Qt
#include <QElapsedTimer>

namespace hoot
{

HOOT_FACTORY_REGISTER(ChangesetReplacement, ChangesetCutOnlyCreator)

ChangesetCutOnlyCreator::ChangesetCutOnlyCreator() :
ChangesetReplacementCreatorAbstract()
{
  _boundsInterpretation = BoundsInterpretation::Lenient;
  _currentChangeDerivationPassIsLinear = false;
  _fullReplacement = false;

  // Every capitalized labeled section of the "create" method (e.g. "DIFF CALCULATION"), except
  // input validation is being considered a separate step for progress. These steps are multiplied
  // by the number of per geometry map creation passes (3) to get the total number of tasks. In
  // addition, there is also one additional task for element ID synchronization and one for
  // changeset derivation. The number of steps must be updated as you add/remove job steps in the
  // create logic.
  const int numTasksPerPass = 5;
  const int numPasses = 3;
  _numTotalTasks = numTasksPerPass * numPasses;
  _numTotalTasks++;  // element ID synchronization
  _numTotalTasks++; // changeset derivation
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

  _currentTask = 1;
  _progress.reset(
    new Progress(ConfigOptions().getJobId(), JOB_SOURCE, Progress::JobState::Running));

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
  _progress->set(_getJobPercentComplete(), "Synchonizing element IDs...");
  _synchronizeIds(refMaps, conflatedMaps);
  _currentTask++;

  // CHANGESET GENERATION

  LOG_STATUS("Generating changeset for " << refMaps.size() << " sets of maps...");

  // Derive a changeset between the ref and conflated maps that replaces ref features with
  // secondary features within the bounds and write it out.
  _progress->set(_getJobPercentComplete(), "Generating changeset...");
  _changesetCreator->setIncludeReviews(false);
  // We have some instances where modify and delete changes are being generated for the same
  // element, which causes error during changeset application. Eventually, we should eliminate their
  // causes, but for now we'll activate changeset cleaning to get rid of the delete statements.
  // Unfortunately, this will make the changeset writing memory bound.
  _changesetCreator->setClean(true);
  _changesetCreator->create(refMaps, conflatedMaps, _output);
  _numChanges = _changesetCreator->getNumTotalChanges();
  _currentTask++;

  _progress->set(
    1.0, Progress::JobState::Successful,
    "Derived replacement changeset: ..." + _output.right(_maxFilePrintLength) + " with " +
    StringUtils::formatLargeNumber(_numChanges) + " changes in " +
    StringUtils::millisecondsToDhms(timer.elapsed()) + " total.");
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

  // INPUT PARSING

  _parseConfigOpts(geometryType);

  // DATA LOAD AND INITIAL PREP

  _progress->set(_getJobPercentComplete(), "Loading input data...");

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

  _currentTask++;

  // CUT

  _progress->set(_getJobPercentComplete(), "Cutting out features...");

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

  _currentTask++;

  // CLEAN

  _progress->set(_getJobPercentComplete(), "Cleaning data...");

  // TODO: remove this combining
  // Combine the cookie cut ref map back with the secondary map for cleaning purposes.
  MapUtils::combineMaps(cookieCutRefMap, secMap, false);
  OsmMapWriterFactory::writeDebugMap(cookieCutRefMap, _changesetId + "-combined-before-conflation");
  secMap.reset();
  LOG_VARD(cookieCutRefMap->size());

  // clean the data

  // TODO: rename var since this map isn't necessary conflated; also rename everything in terms of
  // "toReplace" and "replacement"
  conflatedMap = cookieCutRefMap;
  if (secMapSize > 0)
  {
    // This is a little misleading to only clean when the sec map has elements, however a test fails
    // if we don't. May need further investigation.
    _clean(conflatedMap);
    conflatedMap->setName("cleaned-" + GeometryTypeCriterion::typeToString(geometryType));
  }

  _currentTask++;

  // SNAP

  _progress->set(_getJobPercentComplete(), "Snapping linear features...");

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

  // Used to do an additional round of cropping here to prepare for changeset derivation. After the
  // change to support polygon bounds it was found it wasn't needed anymore when only cutting data,
  // and actually had negative effects on the output. So, removing it completely for cut only. Its
  // worth noting that we may end up needing it again as we expand the cut only tests cases.

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

  _currentTask++;

  // CLEANUP

  // clean up any mistakes introduced
  _progress->set(_getJobPercentComplete(), "Cleaning up erroneous features...");
  _cleanup(refMap);
  _cleanup(conflatedMap);
  _currentTask++;

  LOG_VART(refMap->getElementCount());
  LOG_VART(conflatedMap->getElementCount());
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

}
