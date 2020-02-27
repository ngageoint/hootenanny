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
 * @copyright Copyright (C) 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "DiffConflator.h"

// hoot
#include <hoot/core/algorithms/changeset/MultipleChangesetProvider.h>
#include <hoot/core/conflate/matching/GreedyConstrainedMatches.h>
#include <hoot/core/conflate/matching/MatchClassification.h>
#include <hoot/core/conflate/matching/MatchFactory.h>
#include <hoot/core/conflate/matching/MatchThreshold.h>
#include <hoot/core/conflate/matching/OptimalConstrainedMatches.h>
#include <hoot/core/conflate/poi-polygon/PoiPolygonMatch.h>
#include <hoot/core/criterion/BuildingCriterion.h>
#include <hoot/core/criterion/PoiCriterion.h>
#include <hoot/core/criterion/StatusCriterion.h>
#include <hoot/core/criterion/TagKeyCriterion.h>
#include <hoot/core/elements/ElementId.h>
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
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/visitors/AddRef1Visitor.h>
#include <hoot/core/visitors/CriterionCountVisitor.h>
#include <hoot/core/visitors/LengthOfWaysVisitor.h>
#include <hoot/core/visitors/RemoveElementsVisitor.h>
#include <hoot/core/io/OsmChangesetFileWriterFactory.h>
#include <hoot/core/io/OsmChangesetFileWriter.h>
#include <hoot/core/ops/CopyMapSubsetOp.h>
#include <hoot/core/criterion/NotCriterion.h>

// standard
#include <algorithm>

// tgs
#include <tgs/System/SystemInfo.h>
#include <tgs/System/Time.h>
#include <tgs/System/Timer.h>

// Qt
#include <QElapsedTimer>

using namespace std;
using namespace Tgs;

namespace hoot
{

int DiffConflator::logWarnCount = 0;

HOOT_FACTORY_REGISTER(OsmMapOperation, DiffConflator)

DiffConflator::DiffConflator() :
_matchFactory(MatchFactory::getInstance()),
_settings(Settings::getInstance()),
_taskStatusUpdateInterval(ConfigOptions().getTaskStatusUpdateInterval()),
_numSnappedWays(0)
{
  _reset();
}

DiffConflator::DiffConflator(const std::shared_ptr<MatchThreshold>& matchThreshold) :
_matchFactory(MatchFactory::getInstance()),
_matchThreshold(matchThreshold),
_settings(Settings::getInstance()),
_taskStatusUpdateInterval(ConfigOptions().getTaskStatusUpdateInterval()),
_numSnappedWays(0)
{
  _reset();
}

DiffConflator::~DiffConflator()
{
  _reset();
}

void DiffConflator::_updateProgress(const int currentStep, const QString message)
{
  // Always check for a valid task weight and that the job was set to running. Otherwise, this is
  // just an empty progress object, and we shouldn't log progress.
  if (_progress.getTaskWeight() != 0.0 && _progress.getState() == Progress::JobState::Running)
  {
    _progress.setFromRelative(
      (float)currentStep / (float)getNumSteps(), Progress::JobState::Running, message);
  }
}

void DiffConflator::apply(OsmMapPtr& map)
{
  LOG_INFO("Calculating differential output...");

  Timer timer;
  _reset();
  int currentStep = 1;  // tracks the current job task step for progress reporting
  _numSnappedWays = 0;

  // Store the map - we might need it for tag diff later.
  _pMap = map;

  // This status progress reporting could get way more granular, but we'll go with this for now to
  // avoid overloading users with status.

  _updateProgress(currentStep - 1, "Matching features...");

  // If we skip this part, then any non-matchable data will simply pass through to output.
  if (ConfigOptions().getDifferentialRemoveUnconflatableData())
  {
    LOG_STATUS("Discarding unconflatable elements...");
    const int mapSizeBefore = _pMap->size();
    NonConflatableElementRemover().apply(_pMap);
    _stats.append(
      SingleStat("Remove Non-conflatable Elements Time (sec)", timer.getElapsedAndRestart()));
    OsmMapWriterFactory::writeDebugMap(_pMap, "after-removing non-conflatable");
    LOG_STATUS(
      "Discarded " << StringUtils::formatLargeNumber(mapSizeBefore - _pMap->size()) <<
      " unconflatable elements.");
  }

  // will reproject only if necessary
  MapProjector::projectToPlanar(_pMap);
  _stats.append(SingleStat("Project to Planar Time (sec)", timer.getElapsedAndRestart()));
  OsmMapWriterFactory::writeDebugMap(_pMap, "after-projecting-to-planar");

  // find all the matches in this _pMap
  if (_matchThreshold.get())
  {
    _matchFactory.createMatches(_pMap, _matches, _bounds, _matchThreshold);
  }
  else
  {
    _matchFactory.createMatches(_pMap, _matches, _bounds);
  }
  LOG_STATUS(
    "Found: " << StringUtils::formatLargeNumber(_matches.size()) <<
    " Differential Conflation match conflicts to be removed.");
  double findMatchesTime = timer.getElapsedAndRestart();
  _stats.append(SingleStat("Find Matches Time (sec)", findMatchesTime));
  _stats.append(SingleStat("Number of Matches Found", _matches.size()));
  _stats.append(SingleStat("Number of Matches Found per Second",
    (double)_matches.size() / findMatchesTime));
  OsmMapWriterFactory::writeDebugMap(_pMap, "after-matching");

  currentStep++;

  if (_conflateTags)
  {
    // Use matches to calculate and store tag diff. We must do this before we create the map diff,
    // because that operation deletes all of the info needed for calculating the tag diff.
    _updateProgress(currentStep - 1, "Storing tag differentials...");
    _calcAndStoreTagChanges();
    currentStep++;
  }

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

  if (ConfigOptions().getDifferentialSnapUnconnectedRoads())
  {
    // Let's try to snap disconnected ref2 roads back to ref1 roads.  This has to done before
    // dumping the ref elements in the matches, or the roads we need to snap back to won't be there
    // anymore.
    _numSnappedWays = _snapSecondaryRoadsBackToRef();
  }

  if (ConfigOptions().getDifferentialRemoveReferenceData())
  {
    // _pMap at this point contains all of input1, we are going to delete everything left that
    // belongs to a match pair. Then we will delete all remaining input1 items...leaving us with the
    // differential that we want.
    _removeMatches(Status::Unknown1);

    // Now remove input1 elements
    LOG_STATUS("\tRemoving all reference elements...");
    const int mapSizeBefore = _pMap->size();
    ElementCriterionPtr pTagKeyCrit(new TagKeyCriterion(MetadataTags::Ref1()));
    RemoveElementsVisitor removeRef1Visitor;
    removeRef1Visitor.setRecursive(true);
    removeRef1Visitor.addCriterion(pTagKeyCrit);
    _pMap->visitRw(removeRef1Visitor);
    OsmMapWriterFactory::writeDebugMap(_pMap, "after-removing-ref-elements");
    LOG_STATUS(
      "Removed " << StringUtils::formatLargeNumber(mapSizeBefore - _pMap->size()) <<
      " reference elements...");
  }
}

long DiffConflator::_snapSecondaryRoadsBackToRef()
{
  UnconnectedWaySnapper roadSnapper;
  roadSnapper.setConfiguration(conf());
  LOG_INFO("\t" << roadSnapper.getInitStatusMessage());
  roadSnapper.apply(_pMap);
  LOG_INFO("\t" << roadSnapper.getCompletedStatusMessage());
  OsmMapWriterFactory::writeDebugMap(_pMap, "after-road-snapping");
  return roadSnapper.getNumFeaturesAffected();
}

void DiffConflator::_removeMatches(const Status& status)
{
  LOG_DEBUG("\tRemoving match elements with status: " << status.toString() << "...");

  const bool treatReviewsAsMatches = ConfigOptions().getDifferentialTreatReviewsAsMatches();
  LOG_VARD(treatReviewsAsMatches);
  for (std::vector<ConstMatchPtr>::iterator mit = _matches.begin(); mit != _matches.end(); ++mit)
  {
    ConstMatchPtr match = *mit;
    if (treatReviewsAsMatches || match->getType() != MatchType::Review)
    {
      std::set<std::pair<ElementId, ElementId>> pairs = (*mit)->getMatchPairs();
      for (std::set<std::pair<ElementId, ElementId>>::iterator pit = pairs.begin();
           pit != pairs.end(); ++pit)
      {
        if (!pit->first.isNull())
        {
          LOG_VART(pit->first);
          ElementPtr e = _pMap->getElement(pit->first);
          if (e && e->getStatus() == status)
          {
            //LOG_VART(e->getTags().get("name"));
            RecursiveElementRemover(pit->first).apply(_pMap);
          }
        }
        if (!pit->second.isNull())
        {
          LOG_VART(pit->second);
          ElementPtr e = _pMap->getElement(pit->second);
          if (e && e->getStatus() == status)
          {
            //LOG_VART(e->getTags().get("name"));
            RecursiveElementRemover(pit->second).apply(_pMap);
          }
        }
      }
    }
  }

  OsmMapWriterFactory::writeDebugMap(_pMap, "after-removing-" + status.toString() + "-matches");
}

void DiffConflator::setConfiguration(const Settings &conf)
{
  _settings = conf;
  _matchThreshold.reset();
  _reset();
}

MemChangesetProviderPtr DiffConflator::getTagDiff()
{
  return _pTagChanges;
}

void DiffConflator::storeOriginalMap(OsmMapPtr& pMap)
{
  // Check map to make sure it contains only Unknown1 elements
  // TODO: valid and conflated could be in here too, should we check for them as well?
  ElementCriterionPtr pStatusCrit(new StatusCriterion(Status::Unknown2));
  CriterionCountVisitor countVtor(pStatusCrit);
  pMap->visitRo(countVtor);

  if (countVtor.getCount() > 0)
  {
    // Not something a user can generally cause - more likely it's a misuse of this class.
    throw IllegalArgumentException(
      "Map elements with Status::Unknown2 found when storing original map for diff conflation. "
      "This can cause unpredictable results. The original map should contain only Status::Unknown1 "
      "elements. ");
  }

  // Use the copy constructor to copy the entire map.
  _pOriginalMap.reset(new OsmMap(pMap));

  // We're storing this off for potential use later on if any roads get snapped after conflation.
  // Get rid of ref2 and children. See additional comments in _getChangesetFromMap.
  // TODO: Can we filter this down to whatever feature type the snapping is configured for?
  std::shared_ptr<NotCriterion> crit(
    new NotCriterion(ElementCriterionPtr(new TagKeyCriterion(MetadataTags::Ref2()))));
  CopyMapSubsetOp mapCopier(pMap, crit);
  _pOriginalRef1Map.reset(new OsmMap());
  mapCopier.apply(_pOriginalRef1Map);
}

void DiffConflator::markInputElements(OsmMapPtr pMap)
{
  // Mark input1 elements
  Settings visitorConf;
  visitorConf.set(ConfigOptions::getAddRefVisitorInformationOnlyKey(), "false");
  std::shared_ptr<AddRef1Visitor> pRef1v(new AddRef1Visitor());
  pRef1v->setConfiguration(visitorConf);
  pMap->visitRw(*pRef1v);
}

void DiffConflator::addChangesToMap(OsmMapPtr pMap, ChangesetProviderPtr pChanges)
{
  while (pChanges->hasMoreChanges())
  {
    Change c = pChanges->readNextChange();
    LOG_VART(c);

    // Need to add children
    if (ElementType::Way == c.getElement()->getElementType().getEnum())
    {
      WayPtr pWay = _pOriginalMap->getWay(c.getElement()->getId());

      // Add nodes if need to
      vector<long> nIds = pWay->getNodeIds();
      for (vector<long>::iterator it = nIds.begin(); it != nIds.end(); ++it)
      {
        if (!pMap->containsNode(*it))
        {
          // Add a copy
          NodePtr pNewNode(new Node(*(_pOriginalMap->getNode(*it))));
          pNewNode->setStatus(Status::TagChange);
          pMap->addNode(pNewNode);
        }
      }

      // Add the changed way with merged tags
      ConstWayPtr pTempWay = std::dynamic_pointer_cast<const Way>(c.getElement());
      WayPtr pNewWay(new Way(*pTempWay));
      pNewWay->setStatus(Status::TagChange);
      pMap->addWay(pNewWay);
    }
    else if (ElementType::Relation == c.getElement()->getElementType().getEnum())
    {
      // Diff conflation w/ tags doesn't handle relations. Changed this to log that the relations
      // are being skipped for now. #3449 was created to deal with adding relation support and then
      // closed since we lack a use case currently that requires it. If we ever get one, then we
      // can re-open that issue.

      LOG_DEBUG("Relation handling not implemented with differential conflation: " << c);
      if (Log::getInstance().getLevel() <= Log::Trace)
      {
        ConstRelationPtr relation = std::dynamic_pointer_cast<const Relation>(c.getElement());
        LOG_VART(relation->getElementId());
        LOG_VART(OsmUtils::getRelationDetailedString(relation, _pOriginalMap));
      }
    }
  }
  OsmMapWriterFactory::writeDebugMap(pMap, "after-adding-diff-tag-changes");
}

void DiffConflator::_calcAndStoreTagChanges()
{
  QElapsedTimer timer;
  timer.start();
  LOG_DEBUG("Storing tag changes...");

  MapProjector::projectToWgs84(_pMap);

  // Make sure we have a container for our changes
  if (!_pTagChanges)
  {
    _pTagChanges.reset(new MemChangesetProvider(_pMap->getProjection()));
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
      if (_pOriginalMap->containsElement(pit->first))
      {
        pOldElement = _pOriginalMap->getElement(pit->first);
        pNewElement = _pMap->getElement(pit->second);
      }
      else if (_pOriginalMap->containsElement(pit->second))
      {
        pOldElement = _pOriginalMap->getElement(pit->second);
        pNewElement = _pMap->getElement(pit->first);
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

      if (match->getMatchName() != PoiPolygonMatch().getMatchName() &&
          pOldElement->getElementType() != pNewElement->getElementType())
      {
        LOG_TRACE("Skipping conflate match with differing element types: " << match << "...");
        continue;
      }

      // Double check to make sure we don't create multiple changes for the same element
      if (!_pTagChanges->containsChange(pOldElement->getElementId())
          && _tagsAreDifferent(pOldElement->getTags(), pNewElement->getTags()))
      {
        // Make new change
        Change newChange = _getChange(pOldElement, pNewElement);
        LOG_VART(newChange);
        //OsmUtils::logElementDetail(pOldElement, _pMap, Log::Trace, "Old element: ");
        //OsmUtils::logElementDetail(pNewElement, _pMap, Log::Trace, "New element: ");

        // Add it to our list
        _pTagChanges->addChange(newChange);
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

  OsmMapWriterFactory::writeDebugMap(_pMap, "after-storing-tag-changes");
}

bool DiffConflator::_tagsAreDifferent(const Tags& oldTags, const Tags& newTags)
{
  QStringList ignoreList = ConfigOptions().getDifferentialTagIgnoreList();
  for (Tags::const_iterator newTagIt = newTags.begin(); newTagIt != newTags.end(); ++newTagIt)
  {
    QString newTagKey = newTagIt.key();
    if (newTagKey != MetadataTags::Ref1() // Make sure not ref1
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
    TagComparator::getInstance().overwriteMerge(pNewElement->getTags(), pOldElement->getTags());
  pChangeElement->setTags(newTags);

  // Create the change
  return Change(Change::Modify, pChangeElement);
}

void DiffConflator::_reset()
{
  _matches.clear();
  _pMap.reset();
  _pTagChanges.reset();
  _numSnappedWays = 0;
}

void DiffConflator::_printMatches(vector<ConstMatchPtr> matches)
{
  for (size_t i = 0; i < matches.size(); i++)
  {
    LOG_DEBUG(matches[i]->toString());
  }
}

void DiffConflator::_printMatches(std::vector<ConstMatchPtr> matches, const MatchType& typeFilter)
{
  for (size_t i = 0; i < matches.size(); i++)
  {
    ConstMatchPtr match = matches[i];
    if (match->getType() == typeFilter)
    {
      LOG_DEBUG(match);
    }
  }
}

std::shared_ptr<ChangesetDeriver> DiffConflator::_sortInputs(OsmMapPtr pMap1, OsmMapPtr pMap2)
{
  // Conflation requires all data to be in memory, so no point in adding support for the
  // ExternalMergeElementSorter here.

  InMemoryElementSorterPtr sorted1(new InMemoryElementSorter(pMap1));
  InMemoryElementSorterPtr sorted2(new InMemoryElementSorter(pMap2));
  std::shared_ptr<ChangesetDeriver> delta(new ChangesetDeriver(sorted1, sorted2));
  return delta;
}

ChangesetProviderPtr DiffConflator::_getChangesetFromMap(OsmMapPtr pMap)
{
  if (_numSnappedWays == 0)
  {
    return _sortInputs(OsmMapPtr(new OsmMap()), pMap);
  }
  else
  {
    // If any secondary ways were snapped back to reference ways, we need to generate a changeset
    // against the original reference data (which may have been dropped by the output map by this
    // point) instead of against an empty map. If we don't, then we could end up generating create
    // statements for elements which already exist in the ref dataset. Its arguable that we could
    // use this approach regardless whether roads are snapped or not. This approach has also not
    // been tested with much data, so may not pan out in the long run.

    return _sortInputs(_pOriginalRef1Map, pMap);
  }
}

void DiffConflator::writeChangeset(OsmMapPtr pResultMap, QString& output, bool separateOutput,
                                   const QString& osmApiDbUrl)
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
  ChangesetProviderPtr pGeoChanges = _getChangesetFromMap(pResultMap);

  std::shared_ptr<OsmChangesetFileWriter> writer =
    OsmChangesetFileWriterFactory::getInstance().createWriter(output, osmApiDbUrl);
  LOG_VARD(writer.get());
  if (!_conflateTags)
  {
    // only one changeset to write
    LOG_DEBUG("Writing single changeset...");
    writer->write(output, pGeoChanges);
  }
  else if (separateOutput)
  {
    // write two changesets
    LOG_DEBUG("Writing separate changesets...");
    writer->write(output, pGeoChanges);

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
    writer->write(outFileName, _pTagChanges);
  }
  else
  {
    // write unified output
    LOG_DEBUG("Writing unified changesets...");
    MultipleChangesetProviderPtr pChanges(
      new MultipleChangesetProvider(pResultMap->getProjection()));
    pChanges->addChangesetProvider(pGeoChanges);
    pChanges->addChangesetProvider(_pTagChanges);
    writer->write(output, pChanges);
  }
}

void DiffConflator::calculateStats(OsmMapPtr pResultMap, QList<SingleStat>& stats)
{
  // Differential specific stats - get some numbers for our output

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

}
