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
#include "ChangesetReplacementCreator3.h"

// Hoot
#include <hoot/core/util/MemoryUsageChecker.h>
#include <hoot/core/algorithms/ReplacementSnappedWayJoiner.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/elements/MapUtils.h>
#include <hoot/core/criterion/OrCriterion.h>
#include <hoot/core/criterion/RelationWithLinearMembersCriterion.h>
#include <hoot/core/criterion/RelationWithPointMembersCriterion.h>
#include <hoot/core/criterion/RelationWithPolygonMembersCriterion.h>
#include <hoot/core/criterion/PointCriterion.h>
#include <hoot/core/criterion/PolygonCriterion.h>
#include <hoot/core/criterion/LinearCriterion.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ChangesetReplacementCreator, ChangesetReplacementCreator3)

ChangesetReplacementCreator3::ChangesetReplacementCreator3() :
ChangesetReplacementCreator2()
{
}

void ChangesetReplacementCreator3::_processMaps(
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

  // remove the relations temporarily from the map
  OsmMapPtr refRelationsMap = _removeRelations(refMap);
  LOG_VARD(refRelationsMap->size());

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

  // remove the relations temporarily from the map
  OsmMapPtr secRelationsMap = _removeRelations(secMap);
  LOG_VARD(secRelationsMap->size());

  QList<OsmMapPtr> relationMaps;
  relationMaps.append(refRelationsMap);
  relationMaps.append(secRelationsMap);

  // Prune the sec dataset down to just the feature types specified by the filter, so we don't end
  // up modifying anything else.
  const Settings secFilterSettings =
    _replacementFilterOptions.size() == 0 ? conf() : _replacementFilterOptions;
  _filterFeatures(
    secMap, secFeatureFilter, geometryType, secFilterSettings,
    "sec-after-" + GeometryTypeCriterion::typeToString(geometryType) + "-pruning");

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

  // restore the relations previously removed
  _restoreRelations(refMap, refRelationsMap);
  refRelationsMap.reset();
  _restoreRelations(secMap, secRelationsMap);
  secRelationsMap.reset();

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
      _conflate(conflatedMap, relationMaps);
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
      _clean(conflatedMap, relationMaps);
      conflatedMap->setName("cleaned-" + GeometryTypeCriterion::typeToString(geometryType));
    }
  }

  // SNAP

  // TODO: only snapping near the replacement bounds may help prevent some bad snaps in the output

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
    // keep track of relation member IDs that change during way joining
    _syncJoinedMemberWays(relationMaps, wayJoiner.getJoinedWayIdMappings());
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
  _cropMapForChangesetDerivation(
    refMap, _boundsOpts.changesetRefKeepEntireCrossingBounds,
    _boundsOpts.changesetRefKeepOnlyInsideBounds, _changesetId +
    "-ref-" + GeometryTypeCriterion::typeToString(geometryType) + "-cropped-for-changeset");
  _cropMapForChangesetDerivation(
    conflatedMap, _boundsOpts.changesetSecKeepEntireCrossingBounds,
    _boundsOpts.changesetSecKeepOnlyInsideBounds, _changesetId +
    "-sec-" + GeometryTypeCriterion::typeToString(geometryType) + "-cropped-for-changeset");

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

}
