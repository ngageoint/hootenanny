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
#include "MatchConflicts.h"

// hoot
#include <hoot/core/conflate/matching/Match.h>
#include <hoot/core/conflate/merging/MergerFactory.h>
#include <hoot/core/elements/ElementId.h>
#include <hoot/core/util/StringUtils.h>

// Qt
#include <QElapsedTimer>

namespace hoot
{
using namespace std;

MatchConflicts::MatchConflicts(const ConstOsmMapPtr& map) :
_map(map)
{
}

MatchConflicts::EidIndexMap MatchConflicts::_calculateEidIndexMap(
  const std::vector<ConstMatchPtr>& matches) const
{
  LOG_TRACE("Calculating element ID to index map...");
  EidIndexMap eidToMatches;

  // map each eid to all corresponding matches.
  for (size_t i = 0; i < matches.size(); i++)
  {
    set<pair<ElementId, ElementId>> eids = matches[i]->getMatchPairs();
    for (set<pair<ElementId, ElementId>>::iterator it = eids.begin(); it != eids.end(); ++it)
    {
      eidToMatches.insert(pair<ElementId, size_t>(it->first, i));
      eidToMatches.insert(pair<ElementId, size_t>(it->second, i));
    }

    if (i % 100 == 0)
    {
      PROGRESS_TRACE(i << " element ID indexes processed");
    }
  }

  return eidToMatches;
}

void MatchConflicts::calculateMatchConflicts(
  const std::vector<ConstMatchPtr>& matches, ConflictMap& conflicts) const
{
  QElapsedTimer timer;
  timer.start();
  LOG_DEBUG(
    "Calculating match conflicts for " << StringUtils::formatLargeNumber(matches.size()) <<
    " matches...");

  conflicts.clear();
  // Go through all the matches and map from eid to the match index.
  EidIndexMap eidToMatches = _calculateEidIndexMap(matches);
  const QHash<QString, ConstMatchPtr> idIndexedMatches = Match::getIdIndexedMatches(matches);
  LOG_VARD(idIndexedMatches.size());
  //LOG_VART(idIndexedMatches.keys());

  // go through all the eids with matches ordered by eid
  ElementId lastEid;
  // the set of indexes to all the matches that use a common ElementId
  vector<int> matchSet;
  long eidToMatchCount = 0;
  for (EidIndexMap::iterator it = eidToMatches.begin(); it != eidToMatches.end(); ++it)
  {
    // If we got a new Eid,
    if (it->first != lastEid)
    {
      _calculateSubsetConflicts(matches, conflicts, matchSet, idIndexedMatches);
      // start over with a new match set.
      matchSet.clear();
    }

    matchSet.push_back(it->second);
    lastEid = it->first;

    eidToMatchCount++;
    if (eidToMatchCount % 10 == 0)
    {
      // Would like this to be status, but it logs a separate line for each statement for some
      // reason unfortunately.
      PROGRESS_INFO(
        "Processed matches for " << StringUtils::formatLargeNumber(eidToMatchCount) << " of " <<
        StringUtils::formatLargeNumber(eidToMatches.size()) << " elements. Found " <<
        StringUtils::formatLargeNumber(conflicts.size()) << " match conflicts.");
    }
  }

  _calculateSubsetConflicts(matches, conflicts, matchSet, idIndexedMatches);

  LOG_INFO(
    "Found " << StringUtils::formatLargeNumber(conflicts.size()) << " match conflicts in " <<
    StringUtils::millisecondsToDhms(timer.elapsed()) << ".");
}

void MatchConflicts::_calculateSubsetConflicts(
  const std::vector<ConstMatchPtr>& matches, ConflictMap& conflicts, const vector<int>& matchSet,
  const QHash<QString, ConstMatchPtr>& idIndexedMatches) const
{
  LOG_TRACE("Calculating subset conflicts...");
  LOG_VART(matches.size());
  LOG_VART(conflicts.size());
  LOG_VART(matchSet.size());

  // Search for all possible match pair conflicts within a set.
  for (size_t i = 0; i < matchSet.size(); i++)
  {
    size_t m1 = matchSet[i];
    for (size_t j = i + 1; j < matchSet.size(); j++)
    {
      size_t m2 = matchSet[j];
      // If these aren't the same match and it is conflicting in one direction or the other,
      if (m1 != m2 &&
          (MergerFactory::getInstance().isConflicting(
             _map, matches[m1], matches[m2], idIndexedMatches) ||
           MergerFactory::getInstance().isConflicting(
             _map, matches[m2], matches[m1], idIndexedMatches)))
      {
        // Make sure we're consistent and put the smaller one first.
        if (m2 < m1)
        {
          swap(m1, m2);
        }
        conflicts.insert(m1, m2);
        LOG_TRACE("Conflicting subset matches: " << matches[m1] << ", " << matches[m2]);
      }
    }
  }
}

}
