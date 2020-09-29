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
#include "ChangesetReplacementCreator5.h"

// Hoot
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/ConfigUtils.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/GeometryUtils.h>
#include <hoot/core/elements/MapUtils.h>
#include <hoot/core/criterion/RelationWithLinearMembersCriterion.h>
#include <hoot/core/criterion/RelationWithPointMembersCriterion.h>
#include <hoot/core/criterion/RelationWithPolygonMembersCriterion.h>
#include <hoot/core/criterion/PointCriterion.h>
#include <hoot/core/criterion/PolygonCriterion.h>
#include <hoot/core/criterion/LinearCriterion.h>
#include <hoot/core/criterion/OrCriterion.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/visitors/RemoveMissingElementsVisitor.h>
#include <hoot/core/visitors/RemoveInvalidMultilineStringMembersVisitor.h>
#include <hoot/core/visitors/RemoveDuplicateAreasVisitor.h>
#include <hoot/core/algorithms/changeset/ChangesetCreator.h>
#include <hoot/core/elements/ElementDeduplicator.h>

// Qt
#include <QElapsedTimer>

namespace hoot
{

HOOT_FACTORY_REGISTER(ChangesetReplacementCreator, ChangesetReplacementCreator5)

ChangesetReplacementCreator5::ChangesetReplacementCreator5() :
ChangesetReplacementCreator1()
{
}

void ChangesetReplacementCreator5::_setGlobalOpts()
{
  conf().set(ConfigOptions::getChangesetXmlWriterAddTimestampKey(), false);
  conf().set(ConfigOptions::getReaderAddSourceDatetimeKey(), false);
  conf().set(ConfigOptions::getWriterIncludeCircularErrorTagsKey(), false);
  conf().set(ConfigOptions::getConvertBoundingBoxKey(), _replacementBounds);

  // For this being enabled to have any effect,
  // convert.bounding.box.keep.immediately.connected.ways.outside.bounds must be enabled as well.
  conf().set(ConfigOptions::getConvertBoundingBoxTagImmediatelyConnectedOutOfBoundsWaysKey(), true);

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
  conf().set(ConfigOptions::getConvertBoundingBoxRemoveMissingElementsKey(), false);
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
  ConfigUtils::removeListOpEntry(
    ConfigOptions::getMapCleanerTransformsKey(),
    QString::fromStdString(RemoveDuplicateAreasVisitor::className()));

  // These don't change between scenarios (or at least we haven't needed to change them yet).
  _boundsOpts.loadRefKeepOnlyInsideBounds = false;
  _boundsOpts.cookieCutKeepOnlyInsideBounds = false;
  _boundsOpts.changesetRefKeepOnlyInsideBounds = false;

  // turn on for testing only
  //conf().set(ConfigOptions::getDebugMapsWriteKey(), true);
}

void ChangesetReplacementCreator5::create(
  const QString& input1, const QString& input2, const geos::geom::Envelope& bounds,
  const QString& output)
{
  QElapsedTimer timer;
  timer.start();

  // INPUT VALIDATION AND SETUP

  _input1 = input1;
  _input1Map.reset();
  _input2 = input2;
  _input2Map.reset();
  _output = output;
  //_replacementBounds = bounds;
  // TODO: It makes more sense to store the bounds and then just convert it to a string as needed.
  // The default string stores six decimal places, which should be fine for a bounds. Strangely,
  // when I store the bounds or try to increase the precision of the bounds string, I'm getting a
  // lot of test output issues...needs to be looked into.
  _replacementBounds = GeometryUtils::envelopeToConfigString(bounds);
  _secIdMappings.clear();
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

  // UPDATE 8/17/20: This de-duplication appears no longer necessary after applying the ID
  // synchronization just after it. More testing needs to happen before verifying that, though.

  // If we have the maps for only one geometry type, then there isn't a possibility of duplication
  // created by the replacement operation.
//  if (refMaps.size() > 1)
//  {
//    // Not completely sure at this point if we need to dedupe ref maps. Doing so breaks the
//    // roundabouts test and adds an extra relation to the out of spec test when we do intra-map
//    // de-duping. Mostly worried that not doing so could break the overlapping only replacement
//    // (non-full) scenario...we'll see...
//    //_dedupeMaps(refMaps);
//    _dedupeMaps(conflatedMaps);
//  }

  _combineGeometryTypeMaps(refMaps);
  _combineGeometryTypeMaps(conflatedMaps);

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

void ChangesetReplacementCreator5::_combineGeometryTypeMaps(QList<OsmMapPtr>& maps)
{
  OsmMapPtr combinedMap;
  for (int i = 0; i < maps.size(); i++)
  {
    OsmMapPtr map = maps.at(i);
    if (map)
    {
      if (!combinedMap)
      {
        combinedMap = map;
      }
      else
      {
        MapUtils::combineMaps(combinedMap, map, true);
      }
    }
  }
  if (combinedMap)
  {
    _intraDedupeMap(combinedMap);
    maps.clear();
    maps.append(combinedMap);
  }
}

QMap<GeometryTypeCriterion::GeometryType, ElementCriterionPtr>
  ChangesetReplacementCreator5::_getDefaultGeometryFilters() const
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
  relationPointCrit->setAllowMixedChildren(true);
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
  polyCrit->setAllowMixedChildren(true);
  featureFilters[GeometryTypeCriterion::GeometryType::Polygon] = polyCrit;

  return featureFilters;
}

void ChangesetReplacementCreator5::_intraDedupeMap(OsmMapPtr& map)
{
  ElementDeduplicator deduper;
  deduper.setDedupeIntraMap(true);
  std::shared_ptr<PointCriterion> pointCrit(new PointCriterion());
  deduper.setNodeCriterion(pointCrit);
  deduper.setFavorMoreConnectedWays(true);
  pointCrit->setOsmMap(map.get());
  OsmMapWriterFactory::writeDebugMap(
    map, _changesetId + "-" + map->getName() + "-before-deduping");
  deduper.dedupe(map);
  OsmMapWriterFactory::writeDebugMap(
    map, _changesetId + "-" + map->getName() + "-after-deduping");
}

}
