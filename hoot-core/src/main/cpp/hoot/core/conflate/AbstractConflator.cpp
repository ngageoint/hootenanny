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
 * @copyright Copyright (C) 2020, 2021 Maxar (http://www.maxar.com/)
 */
#include "AbstractConflator.h"

// hoot
#include <hoot/core/algorithms/subline-matching/MultipleMatcherSublineStringMatcher.h>
#include <hoot/core/conflate/matching/GreedyConstrainedMatches.h>
#include <hoot/core/conflate/matching/MatchFactory.h>
#include <hoot/core/conflate/matching/MatchThreshold.h>
#include <hoot/core/conflate/matching/OptimalConstrainedMatches.h>
#include <hoot/core/conflate/merging/LinearDiffMerger.h>
#include <hoot/core/conflate/merging/MarkForReviewMergerCreator.h>
#include <hoot/core/conflate/merging/MergerFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/MemoryUsageChecker.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/conflate/poi-polygon/PoiPolygonMergerCreator.h>
#include <hoot/core/conflate/matching/MatchClassification.h>

// Qt
#include <QElapsedTimer>

// standard
#include <algorithm>

// tgs
#include <tgs/System/SystemInfo.h>
#include <tgs/System/Time.h>

namespace hoot
{

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
  if (_mergerFactory == nullptr)
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
  //LOG_VART(_matches);
  LOG_DEBUG(Tgs::SystemInfo::getCurrentProcessMemoryUsageString());
  OsmMapWriterFactory::writeDebugMap(_map, "after-matching");

  double findMatchesTime = _timer.getElapsedAndRestart();
  _stats.append(SingleStat("Find Matches Time (sec)", findMatchesTime));
  _stats.append(SingleStat("Number of Matches Found", _matches.size()));
  _stats.append(
    SingleStat("Number of Matches Found per Second", (double)_matches.size() / findMatchesTime));
}

MatchSetVector AbstractConflator::_optimizeMatches(std::vector<ConstMatchPtr>& matches)
{
  LOG_DEBUG("Optimizing matches...");

  LOG_DEBUG("Pre-constraining match count: " << StringUtils::formatLargeNumber(matches.size()));
  _stats.append(SingleStat("Number of Matches Before Whole Groups", (double)matches.size()));
  LOG_DEBUG(
    "Number of Matches Before Whole Groups: " << StringUtils::formatLargeNumber(matches.size()));
  // If there are groups of matches that should not be optimized, remove them before optimization.
  MatchSetVector matchSets;
  _removeWholeGroups(matches, matchSets);
  MemoryUsageChecker::getInstance().check();
  _stats.append(SingleStat("Number of Whole Groups", matchSets.size()));
  LOG_DEBUG("Number of Whole Groups: " << StringUtils::formatLargeNumber(matchSets.size()));
  LOG_DEBUG(
    "Number of Matches After Whole Groups: " << StringUtils::formatLargeNumber(matches.size()));
  //LOG_VART(_matches);
  OsmMapWriterFactory::writeDebugMap(_map, "after-whole-group-removal");

  // Globally optimize the set of matches to maximize the conflation score.
  {
    OptimalConstrainedMatches cm(_map);
    std::vector<ConstMatchPtr> cmMatches;

    if (ConfigOptions().getUnifyEnableOptimalConstrainedMatches())
    {
      cm.addMatches(matches.begin(), matches.end());
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
    gm.addMatches(matches.begin(), matches.end());
    double gmStart = Tgs::Time::getTime();
    std::vector<ConstMatchPtr> gmMatches = gm.calculateSubset();
    MemoryUsageChecker::getInstance().check();
    LOG_TRACE("GM took: " << Tgs::Time::getTime() - gmStart << "s.");
    LOG_DEBUG("GM Score: " << gm.getScore());

    if (gm.getScore() > cm.getScore())
    {
      matches = gmMatches;
      LOG_DEBUG("Using greedy matches with a higher score of: " << gm.getScore());
    }
    else
    {
      matches = cmMatches;
      LOG_DEBUG(
        "Using matches obtained by the an Integer Programming solution with a higher score of: " <<
        cm.getScore());
    }
  }
  double optimizeMatchesTime = _timer.getElapsedAndRestart();
  _stats.append(SingleStat("Optimize Matches Time (sec)", optimizeMatchesTime));
  _stats.append(SingleStat("Number of Optimized Matches", (double)matches.size()));
  _stats.append(SingleStat("Number of Matches Optimized per Second",
    (double)matches.size() / optimizeMatchesTime));
  LOG_DEBUG(Tgs::SystemInfo::getCurrentProcessMemoryUsageString());
  // TODO: this stat isn't right for Network
  LOG_DEBUG("Post constraining match count: " << matches.size());
  //LOG_VART(_matches);
  OsmMapWriterFactory::writeDebugMap(_map, "after-match-optimization");

  {
    // Search the matches for groups (subgraphs) of matches. In other words, groups where all the
    // matches are interrelated by element id
    MatchGraph mg;
    mg.addMatches(matches.begin(), matches.end());
    std::vector<std::set<ConstMatchPtr, MatchPtrComparator>> tmpMatchSets = mg.findSubgraphs(_map);
    matchSets.insert(matchSets.end(), tmpMatchSets.begin(), tmpMatchSets.end());
    LOG_DEBUG(Tgs::SystemInfo::getCurrentProcessMemoryUsageString());
  }
  LOG_DEBUG("Match sets count: " << matchSets.size());
  OsmMapWriterFactory::writeDebugMap(_map, "after-match-optimization-2");

  return matchSets;
}

void AbstractConflator::_removeWholeGroups(
  std::vector<ConstMatchPtr>& matches, MatchSetVector& matchSets) const
{
  LOG_DEBUG("Removing whole group matches...");

  // Search the matches for groups (subgraphs) of matches. In other words, groups where all the
  // matches are interrelated by element id
  MatchGraph mg;
  mg.setCheckForConflicts(false);
  mg.addMatches(matches.begin(), matches.end());
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

  matches = leftovers;
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
  LOG_VART(_e2m.size());
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
      for (size_t j = 0; j < mergers.size(); ++j)
      {
        mergers[j]->replace(replaced[j].first, replaced[j].second);
        _e2m[replaced[j].second].push_back(mergers[j]);
      }
      // Don't need to hold on to the old reference any more.
      _e2m.erase(it->first);
    }
  }
}

void AbstractConflator::_applyMergers(const std::vector<MergerPtr>& mergers, OsmMapPtr& map)
{
  if (mergers.empty())
  {
    return;
  }

  QElapsedTimer mergersTimer;
  mergersTimer.start();

  LOG_STATUS("Applying " << StringUtils::formatLargeNumber(mergers.size()) << " mergers...");

  std::vector<std::pair<ElementId, ElementId>> replaced;
  LOG_VART(mergers.size());
  for (size_t i = 0; i < mergers.size(); ++i)
  {
    MergerPtr merger = mergers[i];
    const QString msg =
      "Applying merger: " + merger->getName() + " " + StringUtils::formatLargeNumber(i + 1) +
      " / " + StringUtils::formatLargeNumber(mergers.size());
    // There are way more log statements generated from this than we normally want to see, so just
    // write out a subset. If running in debug, then you'll see all of them which can be useful.
    if (i != 0 && i % 10 == 0)
    {
      PROGRESS_STATUS(msg);
    }
    else
    {
      LOG_DEBUG(msg);
    }
    LOG_VART(merger->getImpactedElementIds());

    merger->apply(map, replaced);
    LOG_VART(map->size());

    // update any mergers that reference the replaced values
    _replaceElementIds(replaced);
    // Not completely sure why the replaced IDs get cleared out each time here by default. That
    // causes problems at this time for LinearDiffMerger, so skipping clearing them out only when
    // its being used.
    if (merger->getName() != LinearDiffMerger::className())
    {
      replaced.clear();
    }
    LOG_VART(merger->getImpactedElementIds());

    if (ConfigOptions().getDebugMapsWrite() && ConfigOptions().getDebugMapsWriteDetailed())
    {
      OsmMapWriterFactory::writeDebugMap(
        map, "after-merge-" + merger->getName() + "-#" + StringUtils::formatLargeNumber(i + 1) +
        "-" + StringUtils::setToString<ElementId>(merger->getImpactedElementIds()));
    }
  }

  LOG_INFO(
    "Applied " << StringUtils::formatLargeNumber(mergers.size()) << " mergers in " <<
    StringUtils::millisecondsToDhms(mergersTimer.elapsed()) << ".");
  LOG_VARD(MultipleMatcherSublineStringMatcher::getNumTimesBackupMatcherUsed());
}

void AbstractConflator::_createMergers(std::vector<MergerPtr>& relationMergers)
{
  // POI/Polygon matching is unique in that it is the only non-generic geometry type matcher that
  // can duplicate matches with other non-generic geometry type matchers (namely POI and Building).
  // For all intra-dataset matching, the default is to convert all matches for a feature to a review
  // if it is also involved in another match. It has been found that POI/Polygon Conflation performs
  // better if these matches are kept when they have features that overlap with a POI/POI
  // match/review, as the overall number of reviews can increase too substantially otherwise (see
  // poi.polygon.match.takes.precedence.over.poi.to.poi.review).

  // An alternative way to deal with this is if there are POI/Polygon matches sharing elements with
  // a POI matcher, then mark all matches involved as reviews before having each MergerCreator
  // create Mergers (poi.polygon.match.takes.precedence.over.poi.to.poi.review=false). Note that
  // we're only doing this right now for POI matches and not Building matches, as it hasn't been
  // seen as necessary for building matches so far.

  if (!ConfigOptions().getPoiPolygonMatchTakesPrecedenceOverPoiToPoiReview())
  {
    // TODO: need a way to not hardcode the POI match name...get it from ScriptMatch somehow?
    PoiPolygonMergerCreator::convertSharedMatchesToReviews(
      _matchSets, _mergers, QStringList("POI"));
  }

  // TODO: Would it help to sort the matches so the biggest or best ones get merged first? (#2912)

  LOG_DEBUG(
    "Converting " << StringUtils::formatLargeNumber(_matchSets.size()) <<
    " match sets to mergers...");
  for (size_t i = 0; i < _matchSets.size(); ++i)
  {
    PROGRESS_INFO(
      "Converting match set " << StringUtils::formatLargeNumber(i + 1) << " / " <<
      StringUtils::formatLargeNumber(_matchSets.size()) << " to a merger...");

    _mergerFactory->createMergers(_map, _matchSets[i], _mergers);

    LOG_TRACE(
      "Converted match set " << StringUtils::formatLargeNumber(i + 1) << " to " <<
      StringUtils::formatLargeNumber(_mergers.size()) << " merger(s).")
  }
  MemoryUsageChecker::getInstance().check();
  LOG_VART(_mergers.size());

  LOG_DEBUG(Tgs::SystemInfo::getCurrentProcessMemoryUsageString());
  _mapElementIdsToMergers();
  LOG_DEBUG(Tgs::SystemInfo::getCurrentProcessMemoryUsageString());

  // Separate mergers that merge relations from other mergers. We want to run them very last so that
  // they have the latest version of their members to work with.
  std::vector<MergerPtr> nonRelationMergers;
  for (std::vector<MergerPtr>::const_iterator mergersItr = _mergers.begin();
       mergersItr != _mergers.end(); ++mergersItr)
  {
    MergerPtr merger = *mergersItr;
    LOG_VART(merger->getName());
    if (merger->getName().contains("CollectionRelation"))
    {
      relationMergers.push_back(merger);
    }
    else
    {
      nonRelationMergers.push_back(merger);
    }
  }
  _mergers = nonRelationMergers;
  LOG_VARD(_mergers.size());
  LOG_VARD(relationMergers.size());

  _stats.append(SingleStat("Create Mergers Time (sec)", _timer.getElapsedAndRestart()));
}

void AbstractConflator::_mergeFeatures(const std::vector<MergerPtr>& relationMergers)
{
  // Apply all non-relation mergers first, then apply relation mergers.
  _applyMergers(_mergers, _map);
  _applyMergers(relationMergers, _map);

  MemoryUsageChecker::getInstance().check();
  OsmMapWriterFactory::writeDebugMap(_map, "after-merging");

  LOG_DEBUG(Tgs::SystemInfo::getCurrentProcessMemoryUsageString());
  const size_t mergerCount = _mergers.size() + relationMergers.size();
  LOG_DEBUG(Tgs::SystemInfo::getCurrentProcessMemoryUsageString());
  double mergersTime = _timer.getElapsedAndRestart();
  _stats.append(SingleStat("Apply Mergers Time (sec)", mergersTime));
  _stats.append(SingleStat("Mergers Applied per Second", (double)mergerCount / mergersTime));
}

void AbstractConflator::_addConflateScoreTags(
  const ElementPtr& e, const MatchClassification& matchClassification,
  const MatchThreshold& matchThreshold) const
{
  Tags& tags = e->getTags();
  tags.appendValue(MetadataTags::HootScoreMatch(), matchClassification.getMatchP());
  tags.appendValue(MetadataTags::HootScoreMiss(), matchClassification.getMissP());
  tags.appendValue(MetadataTags::HootScoreReview(), matchClassification.getReviewP());
  // The thresholds are global, so don't append.
  tags.set(MetadataTags::HootScoreMatchThreshold(), matchThreshold.getMatchThreshold());
  tags.set(MetadataTags::HootScoreMissThreshold(), matchThreshold.getMissThreshold());
  tags.set(MetadataTags::HootScoreReviewThreshold(), matchThreshold.getReviewThreshold());
}

void AbstractConflator::_addConflateScoreTags()
{
  for (size_t i = 0; i < _matches.size(); i++)
  {
    ConstMatchPtr match = _matches[i];
    const MatchClassification& matchClassification = match->getClassification();
    std::shared_ptr<const MatchThreshold> matchThreshold = match->getThreshold();
    std::set<std::pair<ElementId, ElementId>> pairs = match->getMatchPairs();
    for (std::set<std::pair<ElementId, ElementId>>::const_iterator it = pairs.begin();
         it != pairs.end(); ++it)
    {
      ElementPtr e1 = _map->getElement(it->first);
      ElementPtr e2 = _map->getElement(it->second);

      LOG_TRACE(
        "Adding score tags to " << e1->getElementId() << " and " << e2->getElementId() << "...");

      _addConflateScoreTags(e1, matchClassification, *matchThreshold);
      _addConflateScoreTags(e2, matchClassification, *matchThreshold);
      e1->getTags().appendValue(MetadataTags::HootScoreUuid(), e2->getTags().getCreateUuid());
      e2->getTags().appendValue(MetadataTags::HootScoreUuid(), e1->getTags().getCreateUuid());
    }
  }
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
