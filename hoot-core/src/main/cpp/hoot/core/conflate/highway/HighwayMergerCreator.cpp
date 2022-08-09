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
 * @copyright Copyright (C) 2015, 2017, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#include "HighwayMergerCreator.h"

// hoot
#include <hoot/core/conflate/highway/HighwayMatch.h>
#include <hoot/core/conflate/highway/MedianToDividedRoadClassifier.h>
#include <hoot/core/conflate/merging/LinearMergerFactory.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>

using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(MergerCreator, HighwayMergerCreator)

HighwayMergerCreator::HighwayMergerCreator()
{
  setConfiguration(conf());
}

void HighwayMergerCreator::setConfiguration(const Settings &conf)
{
  _minSplitSize = ConfigOptions(conf).getWayMergerMinSplitSize();
}

bool HighwayMergerCreator::createMergers(const MatchSet& matches, vector<MergerPtr>& mergers) const
{
  LOG_TRACE("Creating mergers with " << className() << "...");

  bool result = false;
  assert(matches.size() > 0);

  set<pair<ElementId, ElementId>> eids;
  set<pair<ElementId, ElementId>> medianMatchedEids;

  const bool medianMatchEnabled = ConfigOptions().getHighwayMedianToDualHighwayMatch();
  std::shared_ptr<SublineStringMatcher> sublineMatcher;
  // Go through all the matches.
  for (const auto& match : matches)
  {
    LOG_VART(match->toString());
    const HighwayMatch* highwayMatch = dynamic_cast<const HighwayMatch*>(match.get());
    // Check to make sure all the input matches are highway matches.
    if (highwayMatch == nullptr)
    {
      // return an empty result
      LOG_TRACE(
        "Returning empty result due to match not being " << HighwayMatch::className() << ": " <<
        match->toString());
      return false;
    }
    // Add all the element to element pairs to a set.
    else
    {
      // There should only be one subline matcher used by the matches in a set.
      sublineMatcher = highwayMatch->getSublineMatcher();
      set<pair<ElementId, ElementId>> matchPairs = highwayMatch->getMatchPairs();
      // We're going to separate out regular road matches from those matched median to divided
      // roads, as the merging is different for each.
      if (medianMatchEnabled && highwayMatch->explain().contains(MedianToDividedRoadClassifier::MEDIAN_MATCHED_DESCRIPTION))
        medianMatchedEids.insert(matchPairs.begin(), matchPairs.end());
      else
        eids.insert(matchPairs.begin(), matchPairs.end());
    }
  }
  LOG_VART(eids.size());
  LOG_VART(medianMatchedEids.size());

  // Only add the highway merger if there are elements to merge.
  if (!eids.empty())
  {
    mergers.push_back(
      LinearMergerFactory::getMerger(eids, sublineMatcher, HighwayMatch::MATCH_NAME));
    result = true;
  }
  // Use a different merger for the one-to-many median matches.
  if (!medianMatchedEids.empty())
  {
    mergers.push_back(
      LinearMergerFactory::getMerger(
        medianMatchedEids, sublineMatcher, HighwayMatch::MATCH_NAME,
        MedianToDividedRoadClassifier::MEDIAN_MATCHED_SUBROUTINE_NAME));
    result = true;
  }

  return result;
}

vector<CreatorDescription> HighwayMergerCreator::getAllCreators() const
{
  vector<CreatorDescription> result;
  result.emplace_back(
    className(),
    "Generates mergers that merge roads with the 2nd Generation (Unifying) Algorithm",
    false);
  return result;
}

bool HighwayMergerCreator::isConflicting(const ConstOsmMapPtr& map, ConstMatchPtr m1, ConstMatchPtr m2,
                                         const QHash<QString, ConstMatchPtr>& /*matches*/) const
{
  const HighwayMatch* hm1 = dynamic_cast<const HighwayMatch*>(m1.get());
  const HighwayMatch* hm2 = dynamic_cast<const HighwayMatch*>(m2.get());
  if (hm1 && hm2)
  {
    const bool conflicting = m1->isConflicting(m2, map);
    if (conflicting)
    {
      LOG_TRACE("Conflicting matches: " << m1 << ", " << m2);
    }
    return conflicting;
  }
  else
  {
    return false;
  }
}

}
