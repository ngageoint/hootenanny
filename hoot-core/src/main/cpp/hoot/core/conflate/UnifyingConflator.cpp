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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#include "UnifyingConflator.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/conflate/matching/MatchThreshold.h>
#include <hoot/core/conflate/merging/MergerFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/conflate/matching/MatchClassification.h>
#include <hoot/core/elements/ElementId.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/util/MemoryUsageChecker.h>
#include <hoot/core/schema/SchemaUtils.h>
#include <hoot/core/conflate/poi-polygon/PoiPolygonMergerCreator.h>
#include <hoot/core/visitors/RemoveTagsVisitor.h>

// tgs
#include <tgs/System/SystemInfo.h>
#include <tgs/System/Time.h>

// Qt
#include <QElapsedTimer>

namespace hoot
{

// ONLY ENABLE THIS DURING DEBUGGING; We don't want to tie it to debug.maps.write, as it may
// produce a very large number of output files.
const bool UnifyingConflator::WRITE_DETAILED_DEBUG_MAPS = false;

HOOT_FACTORY_REGISTER(OsmMapOperation, UnifyingConflator)

UnifyingConflator::UnifyingConflator() :
AbstractConflator()
{
}

UnifyingConflator::UnifyingConflator(const std::shared_ptr<MatchThreshold>& matchThreshold) :
AbstractConflator::AbstractConflator(matchThreshold)
{
}

void UnifyingConflator::apply(OsmMapPtr& map)
{
  _reset();
  _map = map;
  _currentStep = 1;  // tracks the current job task step for progress reporting

  // Check to see if all the data is untyped. If so, log a warning so the user knows they may not
  // be getting the best conflate results in case types could be added to the input.
  if (_map->size() > 0 && !SchemaUtils::anyElementsHaveType(_map))
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

  // will reproject if necessary
  MapProjector::projectToPlanar(_map);
  _stats.append(SingleStat("Project to Planar Time (sec)", _timer.getElapsedAndRestart()));

  _updateProgress(_currentStep - 1, "Matching features...");
  _createMatches();
  // Add score tags to all matches that have some score component.
  _addReviewAndScoreTags();
  _currentStep++;

  _updateProgress(_currentStep - 1, "Optimizing feature matches...");
  MatchSetVector matchSets = _optimizeMatches();
  _currentStep++;

  _updateProgress(_currentStep - 1, "Merging feature matches...");

  std::vector<MergerPtr> relationMergers;
  _createMergers(matchSets, relationMergers);

  _mergeFeatures(relationMergers);
  _currentStep++;

  // cleanup

  if (!ConfigOptions().getWriterIncludeDebugTags())
  {
    QStringList tagKeysToRemove;
    tagKeysToRemove.append(MetadataTags::HootMultilineString());
    RemoveTagsVisitor tagRemover(tagKeysToRemove);
    _map->visitRw(tagRemover);
  }
}

void UnifyingConflator::_createMergers(
  MatchSetVector& matchSets, std::vector<MergerPtr>& relationMergers)
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
  // seen as necessary for buildings so far.

  if (!ConfigOptions().getPoiPolygonMatchTakesPrecedenceOverPoiToPoiReview())
  {
    // TODO: need a way to not hardcode the POI match name...get it from ScriptMatch somehow?
    PoiPolygonMergerCreator::convertSharedMatchesToReviews(matchSets, _mergers, QStringList("POI"));
  }

  // TODO: Would it help to sort the matches so the biggest or best ones get merged first? (#2912)

  LOG_DEBUG(
    "Converting " << StringUtils::formatLargeNumber(matchSets.size()) <<
    " match sets to mergers...");
  for (size_t i = 0; i < matchSets.size(); ++i)
  {
    PROGRESS_INFO(
      "Converting match set " << StringUtils::formatLargeNumber(i + 1) << " / " <<
      StringUtils::formatLargeNumber(matchSets.size()) << " to a merger...");

    _mergerFactory->createMergers(_map, matchSets[i], _mergers);

    LOG_TRACE(
      "Converted match set " << StringUtils::formatLargeNumber(i + 1) << " to " <<
      StringUtils::formatLargeNumber(_mergers.size()) << " merger(s).")
  }
  MemoryUsageChecker::getInstance().check();
  LOG_VART(_mergers.size());

  LOG_DEBUG(Tgs::SystemInfo::getCurrentProcessMemoryUsageString());
  // don't need the matches any more
  _matches.clear();

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

void UnifyingConflator::_mergeFeatures(const std::vector<MergerPtr>& relationMergers)
{
  QElapsedTimer mergersTimer;
  mergersTimer.start();

  LOG_INFO(
    "Applying " << StringUtils::formatLargeNumber(_mergers.size() + relationMergers.size()) <<
    " mergers...");
  _applyMergers(_mergers, _map);
  _applyMergers(relationMergers, _map);

  MemoryUsageChecker::getInstance().check();
  OsmMapWriterFactory::writeDebugMap(_map, "after-merging");

  LOG_DEBUG(Tgs::SystemInfo::getCurrentProcessMemoryUsageString());
  const size_t mergerCount = _mergers.size() + relationMergers.size();
  // free up any used resources
  _reset();
  LOG_DEBUG(Tgs::SystemInfo::getCurrentProcessMemoryUsageString());
  double mergersTime = _timer.getElapsedAndRestart();
  _stats.append(SingleStat("Apply Mergers Time (sec)", mergersTime));
  _stats.append(SingleStat("Mergers Applied per Second", (double)mergerCount / mergersTime));

  LOG_INFO(
    "Applied " << StringUtils::formatLargeNumber(mergerCount) << " mergers in " <<
    StringUtils::millisecondsToDhms(mergersTimer.elapsed()) << ".");
}

void UnifyingConflator::_addScoreTags(const ElementPtr& e, const MatchClassification& mc)
{
  Tags& tags = e->getTags();
  tags.appendValue(MetadataTags::HootScoreReview(), mc.getReviewP());
  tags.appendValue(MetadataTags::HootScoreMatch(), mc.getMatchP());
  tags.appendValue(MetadataTags::HootScoreMiss(), mc.getMissP());
}

void UnifyingConflator::_addReviewAndScoreTags()
{
  if (ConfigOptions(_settings).getWriterIncludeConflateScoreTags())
  {
    for (size_t i = 0; i < _matches.size(); i++)
    {
      ConstMatchPtr m = _matches[i];
      const MatchClassification& mc = m->getClassification();
      std::set<std::pair<ElementId, ElementId>> pairs = m->getMatchPairs();
      for (std::set<std::pair<ElementId, ElementId>>::const_iterator it = pairs.begin();
           it != pairs.end(); ++it)
      {
        if (mc.getReviewP() > 0.0)
        {
          ElementPtr e1 = _map->getElement(it->first);
          ElementPtr e2 = _map->getElement(it->second);

          LOG_TRACE(
            "Adding score tags to " << e1->getElementId() << " and " << e2->getElementId() <<
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

}
