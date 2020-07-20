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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "UnifyingConflator.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/conflate/merging/Merger.h>
#include <hoot/core/conflate/merging/MarkForReviewMergerCreator.h>
#include <hoot/core/conflate/matching/MatchFactory.h>
#include <hoot/core/conflate/matching/MatchThreshold.h>
#include <hoot/core/conflate/merging/MergerFactory.h>
#include <hoot/core/conflate/matching/GreedyConstrainedMatches.h>
#include <hoot/core/conflate/matching/OptimalConstrainedMatches.h>
#include <hoot/core/conflate/polygon/BuildingMergerCreator.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/conflate/matching/MatchClassification.h>
#include <hoot/core/elements/ElementId.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/criterion/NotCriterion.h>
#include <hoot/core/ops/CopyMapSubsetOp.h>
#include <hoot/core/util/MemoryUsageChecker.h>
#include <hoot/core/conflate/network/NetworkMatchCreator.h>
#include <hoot/core/schema/SchemaUtils.h>

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

HOOT_FACTORY_REGISTER(OsmMapOperation, UnifyingConflator)

UnifyingConflator::UnifyingConflator() :
  _matchFactory(MatchFactory::getInstance()),
  _settings(Settings::getInstance()),
  _taskStatusUpdateInterval(ConfigOptions().getTaskStatusUpdateInterval())
{
  _reset();
}

UnifyingConflator::UnifyingConflator(const std::shared_ptr<MatchThreshold>& matchThreshold) :
  _matchFactory(MatchFactory::getInstance()),
  _settings(Settings::getInstance()),
  _taskStatusUpdateInterval(ConfigOptions().getTaskStatusUpdateInterval())
{
  _matchThreshold = matchThreshold;
  _reset();
}

UnifyingConflator::~UnifyingConflator()
{
  _reset();
}

void UnifyingConflator::_addScoreTags(const ElementPtr& e, const MatchClassification& mc)
{
  Tags& tags = e->getTags();
  tags.appendValue(MetadataTags::HootScoreReview(), mc.getReviewP());
  tags.appendValue(MetadataTags::HootScoreMatch(), mc.getMatchP());
  tags.appendValue(MetadataTags::HootScoreMiss(), mc.getMissP());
}

void UnifyingConflator::_addReviewTags(const OsmMapPtr& map,
                                       const std::vector<ConstMatchPtr>& matches)
{
  if (ConfigOptions(_settings).getWriterIncludeConflateScoreTags())
  {
    for (size_t i = 0; i < matches.size(); i++)
    {
      ConstMatchPtr m = matches[i];
      const MatchClassification& mc = m->getClassification();
      set<pair<ElementId, ElementId>> pairs = m->getMatchPairs();
      for (set<pair<ElementId, ElementId>>::const_iterator it = pairs.begin();
           it != pairs.end(); ++it)
      {
        if (mc.getReviewP() > 0.0)
        {
          ElementPtr e1 = map->getElement(it->first);
          ElementPtr e2 = map->getElement(it->second);

          LOG_TRACE(
            "Adding review tags to " << e1->getElementId() << " and " << e2->getElementId() <<
            "...");

          _addScoreTags(e1, mc);
          _addScoreTags(e2, mc);
          e1->getTags().appendValue(MetadataTags::HootScoreUuid(), e2->getTags().getCreateUuid());
          e2->getTags().appendValue(MetadataTags::HootScoreUuid(), e1->getTags().getCreateUuid());
        }
      }
    }
  }
}

void UnifyingConflator::_updateProgress(const int currentStep, const QString message)
{
  // Always check for a valid task weight and that the job was set to running. Otherwise, this is
  // just an empty progress object, and we shouldn't log progress.
  if (_progress.getTaskWeight() != 0.0 && _progress.getState() == Progress::JobState::Running)
  {
    _progress.setFromRelative(
      (float)currentStep / (float)getNumSteps(), Progress::JobState::Running, message);
  }
}

void UnifyingConflator::apply(OsmMapPtr& map)
{
  Timer timer;
  _reset();
  int currentStep = 1;  // tracks the current job task step for progress reporting

  // Check to see if all the data is untyped. If so, log a warning so the user knows they may not
  // be getting the best conflate results in case types could be added to the input.
  if (map->size() > 0 && !SchemaUtils::anyElementsHaveType(map))
  {
    const QString msg =
      "No elements in the input map have a recognizable schema type. Generic conflation "
      "routines will be used.";
    if (ConfigOptions().getLogWarningsForCompletelyUntypedInputMaps())
    {
      LOG_WARN(msg);
    }
    else
    {
      LOG_INFO(msg);
    }
  }

  _stats.append(SingleStat("Apply Pre Ops Time (sec)", timer.getElapsedAndRestart()));

  // will reproject if necessary.
  MapProjector::projectToPlanar(map);

  _stats.append(SingleStat("Project to Planar Time (sec)", timer.getElapsedAndRestart()));

  // This status progress reporting could get way more granular, but we'll go with this for now to
  // avoid overloading users with status.

  _updateProgress(currentStep - 1, "Matching features...");

  OsmMapWriterFactory::writeDebugMap(map, "before-matching");
  // find all the matches in this map
  if (_matchThreshold.get())
  {
    //ScoreMatches logic seems to be the only one that needs to pass in the match threshold now when
    //the optimize param is activated.  Otherwise, we get the match threshold information from the
    //config.
    _matchFactory.createMatches(map, _matches, _bounds, _matchThreshold);
  }
  else
  {
    _matchFactory.createMatches(map, _matches, _bounds);
  }
  MemoryUsageChecker::getInstance().check();
  LOG_DEBUG("Match count: " << StringUtils::formatLargeNumber(_matches.size()));
  LOG_VART(_matches);
  LOG_DEBUG(SystemInfo::getCurrentProcessMemoryUsageString());
  OsmMapWriterFactory::writeDebugMap(map, "after-matching");

  double findMatchesTime = timer.getElapsedAndRestart();
  _stats.append(SingleStat("Find Matches Time (sec)", findMatchesTime));
  _stats.append(SingleStat("Number of Matches Found", _matches.size()));
  _stats.append(
    SingleStat("Number of Matches Found per Second", (double)_matches.size() / findMatchesTime));

  vector<ConstMatchPtr> allMatches = _matches;

  // add review tags to all matches that have some review component
  _addReviewTags(map, allMatches);

  LOG_DEBUG("Pre-constraining match count: " << StringUtils::formatLargeNumber(allMatches.size()));
  _stats.append(SingleStat("Number of Matches Before Whole Groups", _matches.size()));
  LOG_DEBUG(
    "Number of Matches Before Whole Groups: " << StringUtils::formatLargeNumber(_matches.size()));
  // If there are groups of matches that should not be optimized, remove them before optimization.
  MatchSetVector matchSets;
  _removeWholeGroups(_matches, matchSets, map);
  MemoryUsageChecker::getInstance().check();
  _stats.append(SingleStat("Number of Whole Groups", matchSets.size()));
  LOG_DEBUG("Number of Whole Groups: " << StringUtils::formatLargeNumber(matchSets.size()));
  LOG_DEBUG(
    "Number of Matches After Whole Groups: " << StringUtils::formatLargeNumber(_matches.size()));
  LOG_VART(_matches);
  OsmMapWriterFactory::writeDebugMap(map, "after-whole-group-removal");

  currentStep++;

  _updateProgress(currentStep - 1, "Optimizing feature matches...");

  // Globally optimize the set of matches to maximize the conflation score.
  {
    OptimalConstrainedMatches cm(map);
    vector<ConstMatchPtr> cmMatches;

    if (ConfigOptions(_settings).getUnifyEnableOptimalConstrainedMatches())
    {
      cm.addMatches(_matches.begin(), _matches.end());
      cm.setTimeLimit(ConfigOptions(_settings).getUnifyOptimizerTimeLimit());
      double cmStart = Time::getTime();
      try
      {
        cmMatches = cm.calculateSubset();
      }
      catch (const Exception& exp)
      {
        LOG_WARN(exp.what());
      }
      MemoryUsageChecker::getInstance().check();
      LOG_TRACE("CM took: " << Time::getTime() - cmStart << "s.");
      LOG_DEBUG("CM Score: " << cm.getScore());
      LOG_DEBUG(SystemInfo::getCurrentProcessMemoryUsageString());
    }

    GreedyConstrainedMatches gm(map);
    gm.addMatches(_matches.begin(), _matches.end());
    double gmStart = Time::getTime();
    vector<ConstMatchPtr> gmMatches = gm.calculateSubset();
    MemoryUsageChecker::getInstance().check();
    LOG_TRACE("GM took: " << Time::getTime() - gmStart << "s.");
    LOG_DEBUG("GM Score: " << gm.getScore());

    if (gm.getScore() > cm.getScore())
    {
      _matches = gmMatches;
      LOG_DEBUG("Using greedy matches with a higher score of: " << gm.getScore());
    }
    else
    {
      _matches = cmMatches;
      LOG_DEBUG(
        "Using matches obtained by the an Integer Programming solution with a higher score of: " <<
        cm.getScore());
    }

  }
  double optimizeMatchesTime = timer.getElapsedAndRestart();
  _stats.append(SingleStat("Optimize Matches Time (sec)", optimizeMatchesTime));
  _stats.append(SingleStat("Number of Optimized Matches", _matches.size()));
  _stats.append(SingleStat("Number of Matches Optimized per Second",
    (double)allMatches.size() / optimizeMatchesTime));
  LOG_DEBUG(SystemInfo::getCurrentProcessMemoryUsageString());
  // TODO: this stat isn't right for Network
  LOG_DEBUG("Post constraining match count: " << _matches.size());
  LOG_VART(_matches);
  OsmMapWriterFactory::writeDebugMap(map, "after-match-optimization");

  {
    // search the matches for groups (subgraphs) of matches. In other words, groups where all the
    // matches are interrelated by element id
    MatchGraph mg;
    mg.addMatches(_matches.begin(), _matches.end());
    vector<set<ConstMatchPtr, MatchPtrComparator>> tmpMatchSets = mg.findSubgraphs(map);
    matchSets.insert(matchSets.end(), tmpMatchSets.begin(), tmpMatchSets.end());
    LOG_DEBUG(SystemInfo::getCurrentProcessMemoryUsageString());
  }
  LOG_DEBUG("Match sets count: " << matchSets.size());
  OsmMapWriterFactory::writeDebugMap(map, "after-match-optimization-2");

  currentStep++;

  _updateProgress(currentStep - 1, "Merging feature matches...");

  // Would it help to sort the matches so the biggest or best ones get merged first?
  // convert all the match sets into mergers - #2912
  LOG_DEBUG(
    "Converting " << StringUtils::formatLargeNumber(matchSets.size()) <<
    " match sets to mergers...");
  for (size_t i = 0; i < matchSets.size(); ++i)
  {
    PROGRESS_INFO(
      "Converting match set " << StringUtils::formatLargeNumber(i + 1) << " / " <<
      StringUtils::formatLargeNumber(matchSets.size()) << " to a merger...");

    _mergerFactory->createMergers(map, matchSets[i], _mergers);

    LOG_TRACE(
      "Converted match set " << StringUtils::formatLargeNumber(i + 1) << " to " <<
      StringUtils::formatLargeNumber(_mergers.size()) << " merger(s).")
  }
  MemoryUsageChecker::getInstance().check();
  LOG_VART(_mergers.size());

  LOG_DEBUG(SystemInfo::getCurrentProcessMemoryUsageString());
  // don't need the matches any more
  allMatches.clear();
  _matches.clear();

  LOG_DEBUG(SystemInfo::getCurrentProcessMemoryUsageString());
  _mapElementIdsToMergers();
  LOG_DEBUG(SystemInfo::getCurrentProcessMemoryUsageString());

  _stats.append(SingleStat("Create Mergers Time (sec)", timer.getElapsedAndRestart()));

  QElapsedTimer mergersTimer;
  mergersTimer.start();

  LOG_INFO("Applying " << StringUtils::formatLargeNumber(_mergers.size()) << " mergers...");
  vector<pair<ElementId, ElementId>> replaced;
  for (size_t i = 0; i < _mergers.size(); ++i)
  {
    MergerPtr merger = _mergers[i];
    const QString msg =
      "Applying merger: " + merger->getName() + " " + StringUtils::formatLargeNumber(i + 1) +
      " / " + StringUtils::formatLargeNumber(_mergers.size());
    if (i != 0 && i % 10 == 0)
    {
      PROGRESS_INFO(msg);
    }
    else
    {
      LOG_TRACE(msg);
    }

    merger->apply(map, replaced);

    LOG_VART(replaced);
    LOG_VART(map->size());

    // update any mergers that reference the replaced values
    _replaceElementIds(replaced);
    replaced.clear();
    LOG_VART(merger->getImpactedElementIds());

    // WARNING: Enabling this could result in a lot of files being generated.
    //if (i % 30 == 0)
//    {
//      OsmMapWriterFactory::writeDebugMap(
//        map, "after-merge-" + merger->getName() + "-#" + StringUtils::formatLargeNumber(i + 1));
//    }
  }
  MemoryUsageChecker::getInstance().check();
  OsmMapWriterFactory::writeDebugMap(map, "after-merging");

  LOG_DEBUG(SystemInfo::getCurrentProcessMemoryUsageString());
  size_t mergerCount = _mergers.size();
  // free up any used resources.
  _reset();
  LOG_DEBUG(SystemInfo::getCurrentProcessMemoryUsageString());

  double mergersTime = timer.getElapsedAndRestart();
  _stats.append(SingleStat("Apply Mergers Time (sec)", mergersTime));
  _stats.append(SingleStat("Mergers Applied per Second", (double)mergerCount / mergersTime));

  LOG_INFO(
    "Applied " << StringUtils::formatLargeNumber(mergerCount) << " mergers in " <<
    StringUtils::millisecondsToDhms(mergersTimer.elapsed()) << ".");

  currentStep++;
}

bool elementIdPairCompare(const pair<ElementId, ElementId>& pair1,
                          const pair<ElementId, ElementId>& pair2)
{
  return pair1.first.getId() > pair2.first.getId();
}

void UnifyingConflator::_mapElementIdsToMergers()
{
  _e2m.clear();
  for (size_t i = 0; i < _mergers.size(); ++i)
  {
    set<ElementId> impacted = _mergers[i]->getImpactedElementIds();
    for (set<ElementId>::const_iterator it = impacted.begin(); it != impacted.end(); ++it)
    {
      _e2m[*it].push_back(_mergers[i]);
    }
  }
}

QString UnifyingConflator::_matchSetToString(const MatchSet& matchSet) const
{
  QString str;
  for (std::set<ConstMatchPtr, MatchPtrComparator>::const_iterator itr = matchSet.begin();
       itr != matchSet.end(); ++itr)
  {
    ConstMatchPtr match = *itr;
    set<pair<ElementId, ElementId>> matchPairs = match->getMatchPairs();
    for (std::set<pair<ElementId, ElementId>>::const_iterator itr2 = matchPairs.begin();
         itr2 != matchPairs.end(); ++itr2)
    {
       pair<ElementId, ElementId> elementIdPair = *itr2;
       str += elementIdPair.first.toString() + " " + elementIdPair.second.toString() + ", ";
    }
  }
  str.chop(2);
  return str;
}

void UnifyingConflator::_removeWholeGroups(std::vector<ConstMatchPtr>& matches,
  MatchSetVector& matchSets, const OsmMapPtr& map)
{
  LOG_DEBUG("Removing whole group matches...");

  // search the matches for groups (subgraphs) of matches. In other words, groups where all the
  // matches are interrelated by element id
  MatchGraph mg;
  mg.setCheckForConflicts(false);
  mg.addMatches(_matches.begin(), _matches.end());
  MatchSetVector tmpMatchSets = mg.findSubgraphs(map);

  matchSets.reserve(matchSets.size() + tmpMatchSets.size());
  vector<ConstMatchPtr> leftovers;

  for (size_t i = 0; i < tmpMatchSets.size(); i++)
  {
    bool wholeGroup = false;
    for (MatchSet::const_iterator it = tmpMatchSets[i].begin(); it != tmpMatchSets[i].end(); ++it)
    {
      if ((*it)->isWholeGroup())
      {
        wholeGroup = true;
      }
    }

    if (wholeGroup)
    {
      LOG_TRACE("Removing whole group: " << _matchSetToString(tmpMatchSets[i]));
      matchSets.push_back(tmpMatchSets[i]);
    }
    else
    {
      leftovers.insert(leftovers.end(), tmpMatchSets[i].begin(), tmpMatchSets[i].end());
    }
  }

  matches = leftovers;
}

void UnifyingConflator::_replaceElementIds(const vector<pair<ElementId, ElementId>>& replaced)
{
  for (size_t i = 0; i < replaced.size(); ++i)
  {
    HashMap<ElementId, vector<MergerPtr>>::const_iterator it = _e2m.find(replaced[i].first);
    if (it != _e2m.end())
    {
      const vector<MergerPtr>& mergers = it->second;
      // replace the element id in all mergers.
      for (size_t i = 0; i < mergers.size(); ++i)
      {
        mergers[i]->replace(replaced[i].first, replaced[i].second);
        _e2m[replaced[i].second].push_back(mergers[i]);
      }
      // don't need to hold on to the old reference any more.
      _e2m.erase(it->first);
    }
  }
}

void UnifyingConflator::setConfiguration(const Settings &conf)
{
  _settings = conf;

  _matchThreshold.reset();
  _mergerFactory.reset();
  _reset();
}

void UnifyingConflator::_reset()
{
  if (_mergerFactory == 0)
  {
    _mergerFactory.reset(new MergerFactory());
    // register the mark for review merger first so all reviews get tagged before another merger
    // gets a chance.
    _mergerFactory->registerCreator(MergerCreatorPtr(new MarkForReviewMergerCreator()));
    _mergerFactory->registerDefaultCreators();
  }

  _e2m.clear();
  _matches.clear();
  _mergers.clear();
}

void UnifyingConflator::_printMatches(std::vector<ConstMatchPtr> matches)
{
  for (size_t i = 0; i < matches.size(); i++)
  {
    LOG_DEBUG(matches[i]->toString());
  }
}

void UnifyingConflator::_printMatches(std::vector<ConstMatchPtr> matches, const MatchType& typeFilter)
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

bool UnifyingConflator::isNetworkConflate()
{
  return
    ConfigOptions().getMatchCreators().contains(
      QString::fromStdString(NetworkMatchCreator::className()));
}

}
