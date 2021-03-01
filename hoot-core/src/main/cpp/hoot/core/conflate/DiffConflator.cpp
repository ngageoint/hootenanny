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
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#include "DiffConflator.h"

// hoot
#include <hoot/core/algorithms/changeset/MultipleChangesetProvider.h>
#include <hoot/core/conflate/matching/MatchThreshold.h>
#include <hoot/core/conflate/poi-polygon/PoiPolygonMatch.h>
#include <hoot/core/criterion/BuildingCriterion.h>
#include <hoot/core/criterion/PoiCriterion.h>
#include <hoot/core/criterion/StatusCriterion.h>
#include <hoot/core/criterion/TagKeyCriterion.h>
#include <hoot/core/elements/InMemoryElementSorter.h>
#include <hoot/core/elements/OsmUtils.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/ops/RecursiveElementRemover.h>
#include <hoot/core/ops/NonConflatableElementRemover.h>
#include <hoot/core/ops/UnconnectedWaySnapper.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/schema/TagComparator.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/visitors/AddRef1Visitor.h>
#include <hoot/core/visitors/CriterionCountVisitor.h>
#include <hoot/core/visitors/LengthOfWaysVisitor.h>
#include <hoot/core/visitors/RemoveElementsVisitor.h>
#include <hoot/core/io/OsmChangesetFileWriterFactory.h>
#include <hoot/core/ops/CopyMapSubsetOp.h>
#include <hoot/core/criterion/NotCriterion.h>
#include <hoot/core/util/MemoryUsageChecker.h>
#include <hoot/core/visitors/RemoveTagsVisitor.h>
#include <hoot/core/criterion/ChainCriterion.h>
#include <hoot/core/ops/WayJoinerOp.h>
#include <hoot/core/util/ConfigUtils.h>
#include <hoot/core/io/OsmChangesetFileWriter.h>
#include <hoot/core/criterion/HighwayCriterion.h>
#include <hoot/core/conflate/ConflateInfoCache.h>

// Qt
#include <QElapsedTimer>

namespace hoot
{

int DiffConflator::logWarnCount = 0;
// ONLY ENABLE THIS DURING DEBUGGING; We don't want to tie it to debug.maps.write, as it may
// produce a very large number of output files.
const bool DiffConflator::WRITE_DETAILED_DEBUG_MAPS = false;

HOOT_FACTORY_REGISTER(OsmMapOperation, DiffConflator)

DiffConflator::DiffConflator() :
AbstractConflator(),
_intraDatasetElementIdsPopulated(false),
_numSnappedWays(0),
_numUnconflatableElementsDiscarded(0)
{
}

DiffConflator::DiffConflator(const std::shared_ptr<MatchThreshold>& matchThreshold) :
AbstractConflator(matchThreshold),
_intraDatasetElementIdsPopulated(false),
_numSnappedWays(0),
_numUnconflatableElementsDiscarded(0)
{
}

void DiffConflator::_reset()
{
  AbstractConflator::_reset();

  _map.reset();
  _tagChanges.reset();
  _numSnappedWays = 0;

  _geometryChangesetStats = "";
  _tagChangesetStats = "";
  _unifiedChangesetStats = "";
  _numUnconflatableElementsDiscarded = 0;
}

void DiffConflator::apply(OsmMapPtr& map)
{
  LOG_INFO("Calculating differential output...");

  // This status progress reporting could get way more granular, but we'll go with this for now to
  // avoid overloading users with status.
  int currentStep = 1;  // tracks the current job task step for progress reporting
  _updateProgress(currentStep - 1, "Matching features...");
  _reset();
  // Store the map, as we might need it for tag diff later.
  _map = map;
  std::shared_ptr<ConflateInfoCache> conflateInfoCache(new ConflateInfoCache(map));

  // If we skip this part, then any unmatchable data will simply pass through to output, which can
  // be useful during debugging.
  if (ConfigOptions().getDifferentialRemoveUnconflatableData())
  {
    _discardUnconflatableElements();
  }

  MapProjector::projectToPlanar(_map); // will actually reproject here only if necessary
  _stats.append(SingleStat("Project to Planar Time (sec)", _timer.getElapsedAndRestart()));
  OsmMapWriterFactory::writeDebugMap(_map, "after-projecting-to-planar");

  // find all the matches in this map
  _intraDatasetMatchOnlyElementIds.clear();
  _intraDatasetElementIdsPopulated = false;
  _createMatches();

  _currentStep++;

  if (_conflateTags)
  {
    // Use matches to calculate and store tag diff. We must do this before we create the map diff,
    // because that operation deletes all of the info needed for calculating the tag diff.
    _updateProgress(currentStep - 1, "Storing tag differentials...");
    _calcAndStoreTagChanges();
    _currentStep++;
  }

  // Get rid of everything from the ref1 map that matched something in the ref2 map. Note, there is
  // a deficiency here in that partial matches won't lead to only partial features in ref 1 being
  // dropped...the entire feature will be dropped, including the parts that didn't match (#4311).

  QString message = "Dropping match conflicts";
  if (ConfigOptions().getDifferentialSnapUnconnectedRoads())
  {
    message += " and snapping roads";
  }
  message += "...";
  _updateProgress(currentStep - 1, message);

  // We're eventually getting rid of all matches from the output, but in order to make the road
  // snapping work correctly we'll get rid of secondary elements in matches first.
  _removeMatches(Status::Unknown2);
  MemoryUsageChecker::getInstance().check();

  // Eventually, we could extend this snapping to all linear feature types.
  if (ConfigOptions().getDifferentialSnapUnconnectedRoads())
  {
    if (conflateInfoCache->elementCriterionInUseByActiveMatcher(HighwayCriterion::className()))
    {
      // Let's try to snap disconnected ref2 roads back to ref1 roads. This has to done before
      // dumping the ref elements in the matches, or the roads we need to snap back to won't be there
      // anymore.
      _numSnappedWays = _snapSecondaryRoadsBackToRef();
      MemoryUsageChecker::getInstance().check();
    }
    else
    {
      LOG_TRACE("Skipping road snapping as conflation is not configured for road features.")
    }
  }

  if (ConfigOptions().getDifferentialRemoveReferenceData())
  {
    _removeRefData();
  }

  if (!ConfigOptions().getWriterIncludeDebugTags())
  {
    _removeMetadataTags();
  }
}

void DiffConflator::_discardUnconflatableElements()
{
  LOG_STATUS("Discarding unconflatable elements...");
  const int mapSizeBefore = _map->size();
  NonConflatableElementRemover().apply(_map);
  MemoryUsageChecker::getInstance().check();
  _stats.append(
    SingleStat("Remove Non-conflatable Elements Time (sec)", _timer.getElapsedAndRestart()));
  OsmMapWriterFactory::writeDebugMap(_map, "after-removing non-conflatable");
  _numUnconflatableElementsDiscarded = mapSizeBefore - _map->size();
  LOG_INFO(
    "Discarded " << StringUtils::formatLargeNumber(_numUnconflatableElementsDiscarded) <<
    " unconflatable elements.");
}

void DiffConflator::storeOriginalMap(OsmMapPtr& map)
{
  // Check map to make sure it contains only Unknown1 elements
  // TODO: valid and conflated could be in here too, should we check for them as well?
  ElementCriterionPtr pStatusCrit(new StatusCriterion(Status::Unknown2));
  CriterionCountVisitor countVtor(pStatusCrit);
  map->visitRo(countVtor);

  if (countVtor.getCount() > 0)
  {
    // Not something a user can generally cause - more likely it's a misuse of this class.
    throw IllegalArgumentException(
      "Map elements with Status::Unknown2 found when storing original map for diff conflation. "
      "This can cause unpredictable results. The original map should contain only Status::Unknown1 "
      "elements. ");
  }

  // Use the copy constructor to copy the entire map.
  _originalMap.reset(new OsmMap(map));

  // We're storing this part off for potential use later on if any roads get snapped after
  // conflation. Get rid of ref2 and children. See additional comments in _getChangesetFromMap.
  // TODO: Can we filter this down to whatever feature type the snapping is configured for?
  std::shared_ptr<NotCriterion> crit(
    new NotCriterion(ElementCriterionPtr(new TagKeyCriterion(MetadataTags::Ref2()))));
  CopyMapSubsetOp mapCopier(map, crit);
  _originalRef1Map.reset(new OsmMap());
  mapCopier.apply(_originalRef1Map);
}

std::shared_ptr<ChangesetDeriver> DiffConflator::_sortInputs(OsmMapPtr map1, OsmMapPtr map2)
{
  // Conflation requires all data to be in memory, so no point in adding support for the
  // ExternalMergeElementSorter here.
  InMemoryElementSorterPtr sorted1(new InMemoryElementSorter(map1));
  InMemoryElementSorterPtr sorted2(new InMemoryElementSorter(map2));
  std::shared_ptr<ChangesetDeriver> delta(new ChangesetDeriver(sorted1, sorted2));
  //  Deriving changesets for differential shouldn't include any deletes, create and modify only
  delta->setAllowDeletingReferenceFeatures(false);
  return delta;
}

void DiffConflator::markInputElements(OsmMapPtr map)
{
  // Mark input1 elements
  Settings visitorConf;
  visitorConf.set(ConfigOptions::getAddRefVisitorInformationOnlyKey(), "false");
  std::shared_ptr<AddRef1Visitor> pRef1v(new AddRef1Visitor());
  pRef1v->setConfiguration(visitorConf);
  map->visitRw(*pRef1v);
}

QSet<ElementId> DiffConflator::_getElementIdsInvolvedInOnlyIntraDatasetMatches(
  const std::vector<ConstMatchPtr>& matches)
{
  QSet<ElementId> elementIds;

  const bool allowReviews = ConfigOptions().getDifferentialTreatReviewsAsMatches();

  // Go through and record any element's involved in an intra-dataset match, since we don't want
  // those types of matches from preventing an element from passing through to the diff output.

  for (std::vector<ConstMatchPtr>::const_iterator matchItr = matches.begin();
       matchItr != matches.end(); ++matchItr)
  {
    ConstMatchPtr match = *matchItr;
    if (match->getType() == MatchType::Match ||
        (allowReviews && match->getType() == MatchType::Review))
    {
      std::set<std::pair<ElementId, ElementId>> pairs = match->getMatchPairs();
      for (std::set<std::pair<ElementId, ElementId>>::const_iterator pairItr = pairs.begin();
           pairItr != pairs.end(); ++pairItr)
      {
        std::pair<ElementId, ElementId> pair = *pairItr;
        ConstElementPtr e1 = _map->getElement(pair.first);
        ConstElementPtr e2 = _map->getElement(pair.second);
        // Any match with elements having the same status (came from the same dataset) is an
        // intra-dataset match.
        if (e1 && e2 && e1->getStatus() == e2->getStatus())
        {
          elementIds.insert(pair.first);
          elementIds.insert(pair.second);
        }
      }
    }
  }

  // Now, go back through and exclude any previously added that are also involved in an
  // inter-dataset match, since we don't want those in the diff output.

  for (std::vector<ConstMatchPtr>::const_iterator matchItr = matches.begin();
       matchItr != matches.end(); ++matchItr)
  {
    ConstMatchPtr match = *matchItr;
    if (match->getType() == MatchType::Match ||
        (allowReviews && match->getType() == MatchType::Review))
    {
      std::set<std::pair<ElementId, ElementId>> pairs = match->getMatchPairs();
      for (std::set<std::pair<ElementId, ElementId>>::const_iterator pairItr = pairs.begin();
           pairItr != pairs.end(); ++pairItr)
      {
        std::pair<ElementId, ElementId> pair = *pairItr;
        ConstElementPtr e1 = _map->getElement(pair.first);
        ConstElementPtr e2 = _map->getElement(pair.second);
        // Any match with elements having a different status (came from different datasets) is an
        // inter-dataset match.
        if (e1 && e2 && e1->getStatus() != e2->getStatus())
        {
          elementIds.remove(pair.first);
          elementIds.remove(pair.second);
        }
      }
    }
  }

  return elementIds;
}

long DiffConflator::_snapSecondaryRoadsBackToRef()
{
  UnconnectedWaySnapper roadSnapper;
  roadSnapper.setConfiguration(conf());
  // The ref snapped features must be marked to prevent their removal before changeset generation
  // later on.
  roadSnapper.setMarkSnappedNodes(true);
  roadSnapper.setMarkSnappedWays(true);
  LOG_INFO("\t" << roadSnapper.getInitStatusMessage());
  roadSnapper.apply(_map);
  LOG_DEBUG("\t" << roadSnapper.getCompletedStatusMessage());
  OsmMapWriterFactory::writeDebugMap(_map, "after-road-snapping");

  // Since way splitting was done as part of the conflate pre ops previously run and we've now
  // snapped unconnected ways, we need to rejoin any split ways *before* we remove reference data.
  // If not, some ref linear data may incorrectly drop out of the diff.
  WayJoinerOp wayJoiner;
  wayJoiner.setConfiguration(conf());
  LOG_INFO("\t" << wayJoiner.getInitStatusMessage());
  wayJoiner.apply(_map);
  LOG_DEBUG("\t" << wayJoiner.getCompletedStatusMessage());
  OsmMapWriterFactory::writeDebugMap(_map, "after-way-joining");

  // No point in running way joining a second time in post conflate ops since we already did it here
  // (its configured in post ops by default), so let's remove it.
  ConfigUtils::removeListOpEntry(
    ConfigOptions::getConflatePostOpsKey(), WayJoinerOp::className());

  return roadSnapper.getNumFeaturesAffected();
}

void DiffConflator::_removeMatches(const Status& status)
{
  LOG_DEBUG("\tRemoving match elements with status: " << status.toString() << "...");

  const bool treatReviewsAsMatches = ConfigOptions().getDifferentialTreatReviewsAsMatches();
  LOG_VARD(treatReviewsAsMatches);

  if (!_intraDatasetElementIdsPopulated)
  {
    _intraDatasetMatchOnlyElementIds = _getElementIdsInvolvedInOnlyIntraDatasetMatches(_matches);
    _intraDatasetElementIdsPopulated = true;
  }

  ElementCriterionPtr notSnappedCrit(
    NotCriterionPtr(new NotCriterion(new TagKeyCriterion(MetadataTags::HootSnapped()))));

  for (std::vector<ConstMatchPtr>::iterator mit = _matches.begin(); mit != _matches.end(); ++mit)
  {
    ConstMatchPtr match = *mit;
    if (match->getType() == MatchType::Match ||
       (treatReviewsAsMatches && match->getType() == MatchType::Review))
    {
      LOG_VART(match);
      LOG_VART(match->getClassification().getMissP());
      LOG_VART(match->getClassification().getReviewP());

      std::set<std::pair<ElementId, ElementId>> pairs = match->getMatchPairs();
      for (std::set<std::pair<ElementId, ElementId>>::iterator pit = pairs.begin();
           pit != pairs.end(); ++pit)
      {
        ElementPtr e1;
        ElementPtr e2;

        if (!pit->first.isNull())
        {
          LOG_VART(pit->first);
          e1 = _map->getElement(pit->first);

        }
        if (!pit->second.isNull())
        {
          LOG_VART(pit->second);
          e2 = _map->getElement(pit->second);
        }

        if (e1 &&
            e1->getStatus() == status &&
            // We don't want to remove any ref snapped ways here. They need to be included in the
            // resulting diff in order to be properly updated in the final output.
            (status != Status::Unknown1 || notSnappedCrit->isSatisfied(e1)) &&
            // poi/poly is the only conflation type that allows intra-dataset matches. We don't want
            // these to be removed from the diff output.
            !(match->getName() == PoiPolygonMatch::MATCH_NAME &&
              _intraDatasetMatchOnlyElementIds.contains(pit->first)))
        {
          LOG_TRACE("Removing element involved in match: " << pit->first << "...");
          RecursiveElementRemover(pit->first).apply(_map);
        }
        if (e2 &&
            e2->getStatus() == status &&
            // see related comment above
            (status != Status::Unknown1 || notSnappedCrit->isSatisfied(e2)) &&
            // see related comment above
            !(match->getName() == PoiPolygonMatch::MATCH_NAME &&
             _intraDatasetMatchOnlyElementIds.contains(pit->second)))
        {
          LOG_TRACE("Removing element involved in match: " << pit->second << "...");
          RecursiveElementRemover(pit->second).apply(_map);
        }
      }
    }
  }

  OsmMapWriterFactory::writeDebugMap(_map, "after-removing-" + status.toString() + "-matches");
}

void DiffConflator::_removeRefData()
{
  LOG_INFO("\tRemoving all reference elements...");

  // _map at this point contains all of input1, we are going to delete everything left that
  // belongs to a match pair. Then we will delete all remaining input1 items...leaving us with the
  // differential that we want.
  _removeMatches(Status::Unknown1);
  MemoryUsageChecker::getInstance().check();

  // Now remove input1 elements. Don't remove any features involved in a snap, as they are needed
  // to properly generate the changeset and keep sec ways snapped in the final output.
  ElementCriterionPtr refCrit(new TagKeyCriterion(MetadataTags::Ref1()));
  ElementCriterionPtr notSnappedCrit(
    NotCriterionPtr(new NotCriterion(new TagKeyCriterion(MetadataTags::HootSnapped()))));
  ElementCriterionPtr removeCrit(ChainCriterionPtr(new ChainCriterion(refCrit, notSnappedCrit)));

  RemoveElementsVisitor removeRef1Visitor;
  removeRef1Visitor.setRecursive(true);
  removeRef1Visitor.addCriterion(removeCrit);
  const int mapSizeBefore = _map->size();
  _map->visitRw(removeRef1Visitor);
  MemoryUsageChecker::getInstance().check();
  OsmMapWriterFactory::writeDebugMap(_map, "after-removing-ref-elements");

  LOG_DEBUG(
    "Removed " << StringUtils::formatLargeNumber(mapSizeBefore - _map->size()) <<
    " reference elements...");
}

void DiffConflator::addChangesToMap(OsmMapPtr map, ChangesetProviderPtr pChanges)
{
  while (pChanges->hasMoreChanges())
  {
    Change c = pChanges->readNextChange();
    LOG_VART(c);

    // Need to add children
    if (ElementType::Way == c.getElement()->getElementType().getEnum())
    {
      WayPtr pWay = _originalMap->getWay(c.getElement()->getId());

      // Add nodes if need to
      std::vector<long> nIds = pWay->getNodeIds();
      for (std::vector<long>::iterator it = nIds.begin(); it != nIds.end(); ++it)
      {
        if (!map->containsNode(*it))
        {
          // Add a copy
          NodePtr pNewNode(new Node(*(_originalMap->getNode(*it))));
          pNewNode->setStatus(Status::TagChange);
          map->addNode(pNewNode);
        }
      }

      // Add the changed way with merged tags
      ConstWayPtr pTempWay = std::dynamic_pointer_cast<const Way>(c.getElement());
      WayPtr pNewWay(new Way(*pTempWay));
      pNewWay->setStatus(Status::TagChange);
      map->addWay(pNewWay);
    }
    else if (ElementType::Relation == c.getElement()->getElementType().getEnum())
    {
      // Diff conflation w/ tags doesn't handle relations. Changed this to log that the relations
      // are being skipped for now. #4298 would add support for relations.

      LOG_DEBUG("Relation handling not implemented with differential conflation: " << c);
      if (Log::getInstance().getLevel() <= Log::Trace)
      {
        ConstRelationPtr relation = std::dynamic_pointer_cast<const Relation>(c.getElement());
        LOG_VART(relation->getElementId());
        LOG_VART(OsmUtils::getRelationDetailedString(relation, _originalMap));
      }
    }
  }
  OsmMapWriterFactory::writeDebugMap(map, "after-adding-diff-tag-changes");
}

void DiffConflator::_calcAndStoreTagChanges()
{
  QElapsedTimer timer;
  timer.start();
  LOG_DEBUG("Storing tag changes...");

  MapProjector::projectToWgs84(_map);

  // Make sure we have a container for our changes
  if (!_tagChanges)
  {
    _tagChanges.reset(new MemChangesetProvider(_map->getProjection()));
  }

  int numMatchesProcessed = 0;
  for (std::vector<ConstMatchPtr>::iterator mit = _matches.begin(); mit != _matches.end(); ++mit)
  {
    ConstMatchPtr match = *mit;
    LOG_VART(match);
    std::set<std::pair<ElementId, ElementId>> pairs = match->getMatchPairs();

    // Go through our match pairs, calculate tag diff for elements. We only
    // consider the "Original" elements when we do this - we want to ignore
    // elements created during map cleaning operations (e.g. intersection splitting)
    // because the map that the changeset operates on won't have those elements.
    for (std::set<std::pair<ElementId, ElementId>>::iterator pit = pairs.begin();
         pit != pairs.end(); ++pit)
    {
      // If it's a POI-Poly match, the poi always comes first, even if it's from the secondary
      // dataset, so we can't always count on the first being the old element.
      ConstElementPtr pOldElement;
      ConstElementPtr pNewElement;
      if (_originalMap->containsElement(pit->first))
      {
        pOldElement = _originalMap->getElement(pit->first);
        pNewElement = _map->getElement(pit->second);
      }
      else if (_originalMap->containsElement(pit->second))
      {
        pOldElement = _originalMap->getElement(pit->second);
        pNewElement = _map->getElement(pit->first);
      }
      else
      {
        // How do you like me now, SonarQube?
        // Skip this element, because it's not in the OG map
        continue;
      }

      LOG_VART(pOldElement->getElementId());
      LOG_VART(pNewElement->getElementId());

      // Apparently, a NetworkMatch can be a node/way pair. See note in
      // NetworkMatch::_discoverWayPairs as to why its allowed. However, tag changes between
      // node/way match pairs other than poi/poly don't seem to make any sense. Clearly, if we end
      // up adding a conflation type other than poi/poly which matches differing geometry types at
      // some point then this will need to be updated.

      if (match->getName() != PoiPolygonMatch().getName() &&
          pOldElement->getElementType() != pNewElement->getElementType())
      {
        LOG_TRACE("Skipping conflate match with differing element types: " << match << "...");
        continue;
      }

      // Double check to make sure we don't create multiple changes for the same element
      if (!_tagChanges->containsChange(pOldElement->getElementId())
          && _tagsAreDifferent(pOldElement->getTags(), pNewElement->getTags()))
      {
        // Make new change
        Change newChange = _getChange(pOldElement, pNewElement);
        LOG_VART(newChange);
        //OsmUtils::logElementDetail(pOldElement, _map, Log::Trace, "Old element: ");
        //OsmUtils::logElementDetail(pNewElement, _map, Log::Trace, "New element: ");

        // Add it to our list
        _tagChanges->addChange(newChange);
      }
    }

    numMatchesProcessed++;
    if (numMatchesProcessed % (_taskStatusUpdateInterval * 10) == 0)
    {
      PROGRESS_INFO(
        "\tStored " << StringUtils::formatLargeNumber(numMatchesProcessed) << " / " <<
            StringUtils::formatLargeNumber(_matches.size()) << " match tag changes.");
    }
  }
  LOG_STATUS(
    "Stored tag changes for " << StringUtils::formatLargeNumber(numMatchesProcessed) <<
    " matches in: " << StringUtils::millisecondsToDhms(timer.elapsed()) << ".");

  OsmMapWriterFactory::writeDebugMap(_map, "after-storing-tag-changes");
  MemoryUsageChecker::getInstance().check();
}

bool DiffConflator::_tagsAreDifferent(const Tags& oldTags, const Tags& newTags) const
{
  // Always ignore metadata tags and then allow additional tags to be ignored on a case by case
  // basis.
  const QStringList ignoreList = ConfigOptions().getDifferentialTagIgnoreList();
  for (Tags::const_iterator newTagIt = newTags.begin(); newTagIt != newTags.end(); ++newTagIt)
  {
    QString newTagKey = newTagIt.key();
    if (newTagKey != MetadataTags::Ref1() // Make sure not ref1
        && !OsmSchema::getInstance().isMetaData(newTagIt.key(), newTagIt.value()) // not a metadata tag
        && !ignoreList.contains(newTagKey, Qt::CaseInsensitive) // Not in our ignore list
        && (!oldTags.contains(newTagIt.key()) // It's a new tag
            || oldTags[newTagIt.key()] != newTagIt.value())) // Or it has a different value
    {
      return true;
    }
  }

  return false;
}

Change DiffConflator::_getChange(ConstElementPtr pOldElement, ConstElementPtr pNewElement)
{
  // Create a new change object based on the original element, with new tags. This may seem a
  // little weird, but we want something very specific here. We want the old element as it was...
  // with new tags.

  // Copy the old one to get the geometry
  ElementPtr pChangeElement(pOldElement->clone());
  assert(pChangeElement->getId() == pOldElement->getId());

  // Need to merge tags into the new element. Keeps all names, chooses tags1 in event of a conflict.
  Tags newTags =
    TagComparator::getInstance().overwriteMerge(
      pNewElement->getTags(), pOldElement->getTags(),
      ConfigOptions().getTagMergerOverwriteExclude(),
      ConfigOptions().getTagMergerOverwriteAccumulateValuesKeys(),
      ConfigOptions().getDuplicateNameCaseSensitive());
  pChangeElement->setTags(newTags);

  // Create the change
  return Change(Change::Modify, pChangeElement);
}

ChangesetProviderPtr DiffConflator::_getChangesetFromMap(OsmMapPtr map)
{
  if (_numSnappedWays == 0)
  {
    return _sortInputs(OsmMapPtr(new OsmMap()), map);
  }
  else
  {
    // If any secondary ways were snapped back to reference ways, we need to generate a changeset
    // against the original reference data (which may have been dropped by the output map by this
    // point) instead of against an empty map. If we don't, then we could end up generating create
    // statements for elements which already exist in the ref dataset. Its arguable that we could
    // use this approach regardless whether roads are snapped or not. This approach has also not
    // been tested with much data, so may not pan out in the long run.

    return _sortInputs(_originalRef1Map, map);
  }
}

void DiffConflator::writeChangeset(
  OsmMapPtr pResultMap, QString& output, bool separateOutput,
  const ChangesetStatsFormat& changesetStatsFormat, const QString& osmApiDbUrl)
{
  if (output.endsWith(".osc.sql") && osmApiDbUrl.trimmed().isEmpty())
  {
    throw IllegalArgumentException(
      "Output to SQL changeset requires an OSM API database URL be specified.");
  }
  else if (!output.endsWith(".osc.sql") && !osmApiDbUrl.trimmed().isEmpty())
  {
    LOG_WARN(
      "Ignoring OSM API database URL: " << osmApiDbUrl << " for non-SQL changeset output...");
  }

  LOG_VARD(output);
  LOG_VARD(separateOutput);
  LOG_VARD(osmApiDbUrl);
  LOG_VARD(_conflateTags);

  // It seems like our tag changes should be sorted by element type before passing them along to the
  // changeset writer, as is done in for the geo changeset and also via ChangesetCreator when you
  // call changeset-derive. However, doing that here would require some refactoring so not worrying
  // about it unless not being sorted actually ends up causing any problems.

  // get the changeset
  ChangesetProviderPtr geoChanges = _getChangesetFromMap(pResultMap);

  std::shared_ptr<OsmChangesetFileWriter> writer =
    OsmChangesetFileWriterFactory::getInstance().createWriter(output, osmApiDbUrl);
  LOG_VARD(writer.get());
  if (!_conflateTags)
  {
    // only one changeset to write
    LOG_DEBUG("Writing single changeset...");
    writer->write(output, geoChanges);
    // ChangesetStatsFormat::Unknown is the default format setting, and we'll assume no stats are
    // to be output if that's the requested format.
    if (changesetStatsFormat != ChangesetStatsFormat::Unknown)
    {
      _geometryChangesetStats = writer->getStatsTable(changesetStatsFormat);
    }
  }
  else if (separateOutput)
  {
    // write two changesets
    LOG_DEBUG("Writing separate changesets...");
    writer->write(output, geoChanges);
    if (changesetStatsFormat != ChangesetStatsFormat::Unknown)
    {
      _geometryChangesetStats = writer->getStatsTable(changesetStatsFormat);
    }

    QString outFileName = output;
    if (outFileName.endsWith(".osc"))
    {
      outFileName.replace(".osc", "");
      outFileName.append(".tags.osc");
    }
    // There are only two changeset writers right now, so this works.
    else
    {
      outFileName.replace(".osc.sql", "");
      outFileName.append(".tags.osc.sql");
    }
    LOG_VARD(outFileName);
    writer->write(outFileName, _tagChanges);
    if (changesetStatsFormat != ChangesetStatsFormat::Unknown)
    {
      _tagChangesetStats = writer->getStatsTable(changesetStatsFormat);
    }
  }
  else
  {
    // write unified output
    LOG_DEBUG("Writing unified changesets...");
    MultipleChangesetProviderPtr pChanges(
      new MultipleChangesetProvider(pResultMap->getProjection()));
    pChanges->addChangesetProvider(geoChanges);
    pChanges->addChangesetProvider(_tagChanges);
    writer->write(output, pChanges);
    if (changesetStatsFormat != ChangesetStatsFormat::Unknown)
    {
      _unifiedChangesetStats = writer->getStatsTable(changesetStatsFormat);
    }
  }
}

void DiffConflator::calculateStats(OsmMapPtr pResultMap, QList<SingleStat>& stats)
{
  // Differential specific stats
  // TODO: These should be rolled in with the other conflate stats.

  ElementCriterionPtr pPoiCrit(new PoiCriterion());
  CriterionCountVisitor poiCounter;
  poiCounter.addCriterion(pPoiCrit);
  pResultMap->visitRo(poiCounter);
  stats.append((SingleStat("New POIs", poiCounter.getCount())));

  ElementCriterionPtr pBuildingCrit(new BuildingCriterion(pResultMap));
  CriterionCountVisitor buildingCounter;
  buildingCounter.addCriterion(pBuildingCrit);
  pResultMap->visitRo(buildingCounter);
  stats.append((SingleStat("New Buildings", buildingCounter.getCount())));

  LengthOfWaysVisitor lengthVisitor;
  pResultMap->visitRo(lengthVisitor);
  stats.append((SingleStat("Km of New Roads", lengthVisitor.getStat() / 1000.0)));
}

void DiffConflator::_removeMetadataTags()
{
  QStringList tagKeysToRemove;
  tagKeysToRemove.append(MetadataTags::Ref1());
  tagKeysToRemove.append(MetadataTags::Ref2());
  tagKeysToRemove.append(MetadataTags::HootSnapped());
  RemoveTagsVisitor tagRemover(tagKeysToRemove);
  _map->visitRw(tagRemover);
}

}
