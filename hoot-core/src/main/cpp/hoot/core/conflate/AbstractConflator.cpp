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
 * @copyright Copyright (C) 2021 Maxar (http://www.maxar.com/)
 */
#include "AbstractConflator.h"

// hoot
#include <hoot/core/conflate/merging/MarkForReviewMergerCreator.h>
#include <hoot/core/conflate/matching/MatchFactory.h>
#include <hoot/core/conflate/matching/MatchThreshold.h>
#include <hoot/core/conflate/merging/MergerFactory.h>
#include <hoot/core/conflate/matching/GreedyConstrainedMatches.h>
#include <hoot/core/conflate/matching/OptimalConstrainedMatches.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/util/MemoryUsageChecker.h>

// standard
#include <algorithm>

// tgs
#include <tgs/System/SystemInfo.h>
#include <tgs/System/Time.h>

namespace hoot
{

// ONLY ENABLE THIS DURING DEBUGGING; We don't want to tie it to debug.maps.write, as it may
// produce a very large number of output files.
const bool AbstractConflator::WRITE_DETAILED_DEBUG_MAPS = false;

AbstractConflator::AbstractConflator() :
_matchFactory(MatchFactory::getInstance()),
_taskStatusUpdateInterval(ConfigOptions().getTaskStatusUpdateInterval()),
_currentStep(1)
{
  _reset();
}

AbstractConflator::AbstractConflator(const std::shared_ptr<MatchThreshold>& matchThreshold) :
_matchFactory(MatchFactory::getInstance()),
_matchThreshold(matchThreshold),
_taskStatusUpdateInterval(ConfigOptions().getTaskStatusUpdateInterval()),
_currentStep(1)
{
  _reset();
}

AbstractConflator::~AbstractConflator()
{
  _reset();
}

void AbstractConflator::_reset()
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

void AbstractConflator::_createMatches()
{
  OsmMapWriterFactory::writeDebugMap(_map, "before-matching");
  // find all the matches in this map
  if (_matchThreshold.get())
  {
    // Match scoring logic seems to be the only one that needs to pass in the match threshold now
    // when the optimize param is activated.  Otherwise, we get the match threshold information from
    // the config.
    _matchFactory.createMatches(_map, _matches, _bounds, _matchThreshold);
  }
  else
  {
    _matchFactory.createMatches(_map, _matches, _bounds);
  }
  MemoryUsageChecker::getInstance().check();
  LOG_DEBUG("Match count: " << StringUtils::formatLargeNumber(_matches.size()));
  LOG_VART(_matches);
  LOG_DEBUG(Tgs::SystemInfo::getCurrentProcessMemoryUsageString());
  OsmMapWriterFactory::writeDebugMap(_map, "after-matching");

  double findMatchesTime = _timer.getElapsedAndRestart();
  _stats.append(SingleStat("Find Matches Time (sec)", findMatchesTime));
  _stats.append(SingleStat("Number of Matches Found", _matches.size()));
  _stats.append(
    SingleStat("Number of Matches Found per Second", (double)_matches.size() / findMatchesTime));
}

MatchSetVector AbstractConflator::_optimizeMatches()
{
  LOG_DEBUG("Pre-constraining match count: " << StringUtils::formatLargeNumber(_matches.size()));
  _stats.append(SingleStat("Number of Matches Before Whole Groups", _matches.size()));
  LOG_DEBUG(
    "Number of Matches Before Whole Groups: " << StringUtils::formatLargeNumber(_matches.size()));
  // If there are groups of matches that should not be optimized, remove them before optimization.
  MatchSetVector matchSets;
  _removeWholeGroups(matchSets);
  MemoryUsageChecker::getInstance().check();
  _stats.append(SingleStat("Number of Whole Groups", matchSets.size()));
  LOG_DEBUG("Number of Whole Groups: " << StringUtils::formatLargeNumber(matchSets.size()));
  LOG_DEBUG(
    "Number of Matches After Whole Groups: " << StringUtils::formatLargeNumber(_matches.size()));
  LOG_VART(_matches);
  OsmMapWriterFactory::writeDebugMap(_map, "after-whole-group-removal");

  // Globally optimize the set of matches to maximize the conflation score.
  {
    OptimalConstrainedMatches cm(_map);
    std::vector<ConstMatchPtr> cmMatches;

    if (ConfigOptions().getUnifyEnableOptimalConstrainedMatches())
    {
      cm.addMatches(_matches.begin(), _matches.end());
      cm.setTimeLimit(ConfigOptions().getUnifyOptimizerTimeLimit());
      double cmStart = Tgs::Time::getTime();
      try
      {
        cmMatches = cm.calculateSubset();
      }
      catch (const Tgs::Exception& exp)
      {
        LOG_WARN(exp.what());
      }
      MemoryUsageChecker::getInstance().check();
      LOG_TRACE("CM took: " << Tgs::Time::getTime() - cmStart << "s.");
      LOG_DEBUG("CM Score: " << cm.getScore());
      LOG_DEBUG(Tgs::SystemInfo::getCurrentProcessMemoryUsageString());
    }

    GreedyConstrainedMatches gm(_map);
    gm.addMatches(_matches.begin(), _matches.end());
    double gmStart = Tgs::Time::getTime();
    std::vector<ConstMatchPtr> gmMatches = gm.calculateSubset();
    MemoryUsageChecker::getInstance().check();
    LOG_TRACE("GM took: " << Tgs::Time::getTime() - gmStart << "s.");
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
  double optimizeMatchesTime = _timer.getElapsedAndRestart();
  _stats.append(SingleStat("Optimize Matches Time (sec)", optimizeMatchesTime));
  _stats.append(SingleStat("Number of Optimized Matches", _matches.size()));
  _stats.append(SingleStat("Number of Matches Optimized per Second",
    (double)_matches.size() / optimizeMatchesTime));
  LOG_DEBUG(Tgs::SystemInfo::getCurrentProcessMemoryUsageString());
  // TODO: this stat isn't right for Network
  LOG_DEBUG("Post constraining match count: " << _matches.size());
  LOG_VART(_matches);
  OsmMapWriterFactory::writeDebugMap(_map, "after-match-optimization");

  {
    // Search the matches for groups (subgraphs) of matches. In other words, groups where all the
    // matches are interrelated by element id
    MatchGraph mg;
    mg.addMatches(_matches.begin(), _matches.end());
    std::vector<std::set<ConstMatchPtr, MatchPtrComparator>> tmpMatchSets = mg.findSubgraphs(_map);
    matchSets.insert(matchSets.end(), tmpMatchSets.begin(), tmpMatchSets.end());
    LOG_DEBUG(Tgs::SystemInfo::getCurrentProcessMemoryUsageString());
  }
  LOG_DEBUG("Match sets count: " << matchSets.size());
  OsmMapWriterFactory::writeDebugMap(_map, "after-match-optimization-2");

  return matchSets;
}

void AbstractConflator::_removeWholeGroups(MatchSetVector& matchSets)
{
  LOG_DEBUG("Removing whole group matches...");

  // Search the matches for groups (subgraphs) of matches. In other words, groups where all the
  // matches are interrelated by element id
  MatchGraph mg;
  mg.setCheckForConflicts(false);
  mg.addMatches(_matches.begin(), _matches.end());
  MatchSetVector tmpMatchSets = mg.findSubgraphs(_map);

  matchSets.reserve(matchSets.size() + tmpMatchSets.size());
  std::vector<ConstMatchPtr> leftovers;

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

  _matches = leftovers;
}

void AbstractConflator::_mapElementIdsToMergers()
{
  _e2m.clear();
  for (size_t i = 0; i < _mergers.size(); ++i)
  {
    std::set<ElementId> impacted = _mergers[i]->getImpactedElementIds();
    for (std::set<ElementId>::const_iterator it = impacted.begin(); it != impacted.end(); ++it)
    {
      _e2m[*it].push_back(_mergers[i]);
    }
  }
}

void AbstractConflator::_replaceElementIds(
  const std::vector<std::pair<ElementId, ElementId>>& replaced)
{
  for (size_t i = 0; i < replaced.size(); ++i)
  {
    HashMap<ElementId, std::vector<MergerPtr>>::const_iterator it = _e2m.find(replaced[i].first);
    if (it != _e2m.end())
    {
      const std::vector<MergerPtr>& mergers = it->second;
      // Replace the element id in all mergers.
      for (size_t i = 0; i < mergers.size(); ++i)
      {
        mergers[i]->replace(replaced[i].first, replaced[i].second);
        _e2m[replaced[i].second].push_back(mergers[i]);
      }
      // Don't need to hold on to the old reference any more.
      _e2m.erase(it->first);
    }
  }
}

void AbstractConflator::_applyMergers(const std::vector<MergerPtr>& mergers, OsmMapPtr& map)
{
  std::vector<std::pair<ElementId, ElementId>> replaced;
  for (size_t i = 0; i < mergers.size(); ++i)
  {
    MergerPtr merger = mergers[i];
    const QString msg =
      "Applying merger: " + merger->getName() + " " + StringUtils::formatLargeNumber(i + 1) +
      " / " + StringUtils::formatLargeNumber(mergers.size());
    // There are way more log statements generated from this than we normally want to see, so just
    // info out a subset. If running in debug, then you'll see all of them which can be useful.
    if (i != 0 && i % 10 == 0)
    {
      PROGRESS_INFO(msg);
    }
    else
    {
      LOG_DEBUG(msg);
    }

    // We require that each individual merger set the option to mark merge created relations, so
    // disable this option as each merger is processed.
    conf().set(ConfigOptions::getConflateMarkMergeCreatedMultilinestringRelationsKey(), false);

    merger->apply(map, replaced);

    LOG_VART(replaced);
    LOG_VART(map->size());

    // update any mergers that reference the replaced values
    _replaceElementIds(replaced);
    replaced.clear();
    LOG_VART(merger->getImpactedElementIds());

    if (WRITE_DETAILED_DEBUG_MAPS)
    {
      OsmMapWriterFactory::writeDebugMap(
        map, "after-merge-" + merger->getName() + "-#" + StringUtils::formatLargeNumber(i + 1));
    }
  }
}

QString AbstractConflator::_matchSetToString(const MatchSet& matchSet) const
{
  QString str;
  for (std::set<ConstMatchPtr, MatchPtrComparator>::const_iterator itr = matchSet.begin();
       itr != matchSet.end(); ++itr)
  {
    ConstMatchPtr match = *itr;
    std::set<std::pair<ElementId, ElementId>> matchPairs = match->getMatchPairs();
    for (std::set<std::pair<ElementId, ElementId>>::const_iterator itr2 = matchPairs.begin();
         itr2 != matchPairs.end(); ++itr2)
    {
       std::pair<ElementId, ElementId> elementIdPair = *itr2;
       str += elementIdPair.first.toString() + " " + elementIdPair.second.toString() + ", ";
    }
  }
  str.chop(2);
  return str;
}

void AbstractConflator::_updateProgress(const int currentStep, const QString message)
{
  // Always check for a valid task weight and that the job was set to running. Otherwise, this is
  // just an empty progress object, and we shouldn't log progress.
  if (_progress.getTaskWeight() != 0.0 && _progress.getState() == Progress::JobState::Running)
  {
    _progress.setFromRelative(
      (float)currentStep / (float)getNumSteps(), Progress::JobState::Running, message);
  }
}

}
