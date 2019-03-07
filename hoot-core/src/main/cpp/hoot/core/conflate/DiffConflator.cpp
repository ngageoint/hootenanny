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
 * @copyright Copyright (C) 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "DiffConflator.h"

// hoot
#include <hoot/core/algorithms/changeset/InMemoryElementSorter.h>
#include <hoot/core/algorithms/changeset/MultipleChangesetProvider.h>
#include <hoot/core/conflate/matching/MatchFactory.h>
#include <hoot/core/conflate/matching/MatchThreshold.h>
#include <hoot/core/conflate/matching/GreedyConstrainedMatches.h>
#include <hoot/core/conflate/matching/OptimalConstrainedMatches.h>
#include <hoot/core/criterion/BuildingCriterion.h>
#include <hoot/core/criterion/PoiCriterion.h>
#include <hoot/core/criterion/RelationCriterion.h>
#include <hoot/core/criterion/StatusCriterion.h>
#include <hoot/core/criterion/TagKeyCriterion.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/io/OsmXmlChangesetFileWriter.h>
#include <hoot/core/ops/NamedOp.h>
#include <hoot/core/ops/RecursiveElementRemover.h>
#include <hoot/core/ops/NonConflatableElementRemover.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/conflate/matching/MatchClassification.h>
#include <hoot/core/elements/ElementId.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/schema/TagComparator.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/visitors/AddRef1Visitor.h>
#include <hoot/core/visitors/CriterionCountVisitor.h>
#include <hoot/core/visitors/LengthOfWaysVisitor.h>
#include <hoot/core/visitors/RemoveElementsVisitor.h>

// standard
#include <algorithm>

// tgs
#include <tgs/System/SystemInfo.h>
#include <tgs/System/Time.h>
#include <tgs/System/Timer.h>

using namespace std;
using namespace Tgs;

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, DiffConflator)

DiffConflator::DiffConflator() :
  _matchFactory(MatchFactory::getInstance()),
  _settings(Settings::getInstance())
{
  _reset();
}

DiffConflator::DiffConflator(boost::shared_ptr<MatchThreshold> matchThreshold) :
  _matchFactory(MatchFactory::getInstance()),
  _settings(Settings::getInstance())
{
  _matchThreshold = matchThreshold;
  _reset();
}


DiffConflator::~DiffConflator()
{
  _reset();
}

void DiffConflator::apply(OsmMapPtr& map)
{
  Timer timer;
  _reset();

  // Store the map - we might need it for tag diff later.
  _pMap = map;

  LOG_INFO("Discarding relations...");
  boost::shared_ptr<RelationCriterion> pRelationCrit(new RelationCriterion());
  RemoveElementsVisitor removeRelationsVisitor(pRelationCrit);
  _pMap->visitRw(removeRelationsVisitor);
  _stats.append(SingleStat("Remove Relations Time (sec)", timer.getElapsedAndRestart()));

  LOG_INFO("Discarding non-conflatable elements...");
  NonConflatableElementRemover nonConflateRemover;
  nonConflateRemover.apply(_pMap);
  _stats.append(SingleStat("Remove Non-conflatable Elements Time (sec)", timer.getElapsedAndRestart()));

  // Will reproject if necessary.
  LOG_INFO("Projecting to planar...");
  MapProjector::projectToPlanar(_pMap);
  _stats.append(SingleStat("Project to Planar Time (sec)", timer.getElapsedAndRestart()));

  // find all the matches in this _pMap
  LOG_INFO("Finding matches...");
  if (_matchThreshold.get())
  {
    _matchFactory.createMatches(_pMap, _matches, _bounds, _matchThreshold);
  }
  else
  {
    _matchFactory.createMatches(_pMap, _matches, _bounds);
  }
  LOG_DEBUG("Match count: " << _matches.size());

  double findMatchesTime = timer.getElapsedAndRestart();
  _stats.append(SingleStat("Find Matches Time (sec)", findMatchesTime));
  _stats.append(SingleStat("Number of Matches Found", _matches.size()));
  _stats.append(SingleStat("Number of Matches Found per Second",
    (double)_matches.size() / findMatchesTime));

  // Use matches to calculate and store tag diff. We must do this before we
  // create the map diff, because that operation deletes all of the info needed
  // for calculating the tag diff.
  MapProjector::projectToWgs84(_pMap);
  _calcAndStoreTagChanges();

  // We have matches. Here's what we are going to do: because our _pMap contains
  // all of input1 and input2, we are going to delete everthing that belongs to
  // a match pair. Then we will delete all remaining input1 items... leaving us
  // with the differential that we want.
  for (std::vector<const Match*>::iterator mit = _matches.begin(); mit != _matches.end(); ++mit)
  {
    std::set<std::pair<ElementId, ElementId>> pairs = (*mit)->getMatchPairs();

    for (std::set<std::pair<ElementId, ElementId>>::iterator pit = pairs.begin();
         pit != pairs.end(); ++pit)
    {
      RecursiveElementRemover(pit->first).apply(_pMap);
      RecursiveElementRemover(pit->second).apply(_pMap);
    }
  }

  // Now remove input1 elements
  boost::shared_ptr<ElementCriterion> pTagKeyCrit(new TagKeyCriterion(MetadataTags::Ref1()));
  RemoveElementsVisitor removeRef1Visitor(pTagKeyCrit);
  removeRef1Visitor.setRecursive(true);
  _pMap->visitRw(removeRef1Visitor);
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
  ElementCriterionPtr pStatusCrit(new StatusCriterion(Status::Unknown2));
  CriterionCountVisitor countVtor(pStatusCrit);
  pMap->visitRo(countVtor);

  if (countVtor.getCount() > 0)
  {
    // Not something a user can generally cause - more likely it's a misuse
    // of this class.
    LOG_ERROR("Map elements with Status::Unknown2 found when storing "
              "original map for diff conflation. This can cause unpredictable "
              "results. The original map should contain only Status::Unknown1 "
              "elements. ");
  }

  // Use the copy constructor
  _pOriginalMap.reset(new OsmMap(pMap));
}

void DiffConflator::markInputElements(OsmMapPtr pMap)
{
  // Mark input1 elements (Use Ref1 visitor, because it's already coded up)
  Settings visitorConf;
  visitorConf.set(ConfigOptions::getAddRefVisitorInformationOnlyKey(), "false");
  boost::shared_ptr<AddRef1Visitor> pRef1v(new AddRef1Visitor());
  pRef1v->setConfiguration(visitorConf);
  pMap->visitRw(*pRef1v);
}

void DiffConflator::addChangesToMap(OsmMapPtr pMap, ChangesetProviderPtr pChanges)
{
  while (pChanges->hasMoreChanges())
  {
    Change c = pChanges->readNextChange();

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
      ConstWayPtr pTempWay = boost::dynamic_pointer_cast<const Way>(c.getElement());
      WayPtr pNewWay(new Way(*pTempWay));
      pNewWay->setStatus(Status::TagChange);
      pMap->addWay(pNewWay);
    }
    else if (ElementType::Relation == c.getElement()->getElementType().getEnum())
    {
      // Diff conflation doesn't do relations
      throw HootException("Relation handling not implemented with differential "
                          "conflation yet.");
    }
  }
}

void DiffConflator::_calcAndStoreTagChanges()
{
  // Make sure we have a container for our changes
  if (!_pTagChanges)
  {
    _pTagChanges.reset(new MemChangesetProvider(_pMap->getProjection()));
  }

  for (std::vector<const Match*>::iterator mit = _matches.begin(); mit != _matches.end(); ++mit)
  {
    std::set< std::pair<ElementId, ElementId>> pairs = (*mit)->getMatchPairs();

    // Go through our match pairs, calculate tag diff for elements. We only
    // consider the "Original" elements when we do this - we want to ignore
    // elements created during map cleaning operations (e.g. intersection splitting)
    // because the map that the changeset operates on won't have those elements.
    for (std::set< std::pair<ElementId, ElementId> >::iterator pit = pairs.begin();
         pit != pairs.end(); ++pit)
    {
      // If it's a POI-Poly match, the poi always comes first, even if it's from
      // the secondary dataset, so we can't always count on the first being
      // the old element
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

      // Double check to make sure we don't create multiple changes for the
      // same element
      if (!_pTagChanges->containsChange(pOldElement->getElementId())
          && _compareTags(pOldElement->getTags(), pNewElement->getTags()))
      {
        // Make new change
        Change newChange = _getChange(pOldElement, pNewElement);

        // Add it to our list
        _pTagChanges->addChange(newChange);
      }
    }
  }
}

// See if tags are the same
bool DiffConflator::_compareTags (const Tags &oldTags, const Tags &newTags)
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

// Create a new change object based on the original element, with new tags
Change DiffConflator::_getChange(ConstElementPtr pOldElement,
                                 ConstElementPtr pNewElement)
{
  // This may seem a little weird, but we want something very specific here.
  // We want the old element as it was... with new tags.

  // Copy the old one to get the geometry
  ElementPtr pChangeElement(pOldElement->clone());

  assert(pChangeElement->getId() == pOldElement->getId());

  // Need to merge tags into the new element
  // Keeps all names, chooses tags1 in event of a conflict.
  Tags newTags = TagComparator::getInstance().overwriteMerge(pNewElement->getTags(),
                                                             pOldElement->getTags());
  pChangeElement->setTags(newTags);

  // Create the change
  Change newChange(Change::Modify, pChangeElement);

  return newChange;
}

void DiffConflator::_reset()
{
  _deleteAll(_matches);
  _pMap.reset();
  _pTagChanges.reset();
}

void DiffConflator::_printMatches(vector<const Match*> matches)
{
  for (size_t i = 0; i < matches.size(); i++)
  {
    LOG_DEBUG(matches[i]->toString());
  }
}

void DiffConflator::_printMatches(vector<const Match*> matches, const MatchType& typeFilter)
{
  for (size_t i = 0; i < matches.size(); i++)
  {
    const Match* match = matches[i];
    if (match->getType() == typeFilter)
    {
      LOG_DEBUG(match);
    }
  }
}

// Convenience function used when deriving a changeset
boost::shared_ptr<ChangesetDeriver> DiffConflator::_sortInputs(OsmMapPtr pMap1, OsmMapPtr pMap2)
{
  //Conflation requires all data to be in memory, so no point in adding support for
  //ExternalMergeElementSorter here.

  InMemoryElementSorterPtr sorted1(new InMemoryElementSorter(pMap1));
  InMemoryElementSorterPtr sorted2(new InMemoryElementSorter(pMap2));
  boost::shared_ptr<ChangesetDeriver> delta(new ChangesetDeriver(sorted1, sorted2));
  return delta;
}

ChangesetProviderPtr DiffConflator::_getChangesetFromMap(OsmMapPtr pMap)
{
  // Make empty map
  OsmMapPtr pEmptyMap(new OsmMap());

  // Get Changeset Deriver
  boost::shared_ptr<ChangesetDeriver> pDeriver = _sortInputs(pEmptyMap, pMap);

  // Return the provider
  return pDeriver;
}

void DiffConflator::writeChangeset( OsmMapPtr pResultMap, QString &output, bool separateOutput)
{
  // Write a changeset
  ChangesetProviderPtr pGeoChanges = _getChangesetFromMap(pResultMap);

  if (!_conflateTags)
  {
    // only one changeset to write
    OsmXmlChangesetFileWriter writer;
    writer.write(output, pGeoChanges);
  }
  else if (separateOutput)
  {
    // write two changesets
    OsmXmlChangesetFileWriter writer;
    writer.write(output, pGeoChanges);

    QString outFileName = output;
    outFileName.replace(".osc", "");
    outFileName.append(".tags.osc");
    OsmXmlChangesetFileWriter tagChangeWriter;
    tagChangeWriter.write(outFileName, _pTagChanges);
  }
  else
  {
    // write unified output
    MultipleChangesetProviderPtr pChanges(new MultipleChangesetProvider(pResultMap->getProjection()));
    pChanges->addChangesetProvider(pGeoChanges);
    pChanges->addChangesetProvider(_pTagChanges);
    OsmXmlChangesetFileWriter writer;
    writer.write(output, pChanges);
  }
}

void DiffConflator::calculateStats(OsmMapPtr pResultMap, QList<SingleStat>& stats)
{
  // Differential specific stats - get some numbers for our output

  ElementCriterionPtr pPoiCrit(new PoiCriterion());
  CriterionCountVisitor poiCounter;
  poiCounter.addCriterion(pPoiCrit);
  pResultMap->visitRo(poiCounter);
  stats.append((SingleStat("Count of New POIs", poiCounter.getCount())));

  ElementCriterionPtr pBuildingCrit(new BuildingCriterion(pResultMap));
  CriterionCountVisitor buildingCounter;
  buildingCounter.addCriterion(pBuildingCrit);
  pResultMap->visitRo(buildingCounter);
  stats.append((SingleStat("Count of New Buildings", buildingCounter.getCount())));

  LengthOfWaysVisitor lengthVisitor;
  pResultMap->visitRo(lengthVisitor);
  stats.append((SingleStat("Km of New Road", lengthVisitor.getStat() / 1000.0)));
}

}
