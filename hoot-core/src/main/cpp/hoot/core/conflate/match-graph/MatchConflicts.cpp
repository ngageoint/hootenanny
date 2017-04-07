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
 * @copyright Copyright (C) 2015, 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "MatchConflicts.h"

// hoot
#include <hoot/core/conflate/Match.h>
#include <hoot/core/conflate/MergerFactory.h>
#include <hoot/core/elements/ElementId.h>

// Standard
#include <map>
#include <set>

namespace hoot
{
using namespace std;

MatchConflicts::MatchConflicts(const ConstOsmMapPtr& map) :
  _map(map)
{
  LOG_DEBUG("Start MatchConflicts");
}

MatchConflicts::EidIndexMap MatchConflicts::calculateEidIndexMap(
  const vector<const Match*>& matches) const
{
  EidIndexMap eidToMatches;

  // map each eid to all corresponding matches.
  for (size_t i = 0; i < matches.size(); i++)
  {
    set< pair<ElementId, ElementId> > eids = matches[i]->getMatchPairs();
    for (set< pair<ElementId, ElementId> >::iterator it = eids.begin(); it != eids.end(); ++it)
    {
      eidToMatches.insert(pair<ElementId, size_t>(it->first, i));
      eidToMatches.insert(pair<ElementId, size_t>(it->second, i));
    }
  }

  return eidToMatches;
}

void MatchConflicts::calculateMatchConflicts(const vector<const Match*>& matches,
  ConflictMap &conflicts)
{
  LOG_DEBUG("clear conflicts");
  conflicts.clear();

  LOG_DEBUG("eid to matches");
  // go through all the matches and map from eid to the match index.
  EidIndexMap eidToMatches = calculateEidIndexMap(matches);

  // go through all the eids with matches ordered by eid
  ElementId lastEid;
  // the set of indexes to all the matches that use a common ElementId
  vector<int> matchSet;

  LOG_DEBUG("Loop through EidIndex");
  for (EidIndexMap::iterator it = eidToMatches.begin(); it != eidToMatches.end(); ++it)
  {
    // if we got a new Eid.
    if (it->first != lastEid)
    {
      LOG_DEBUG("f neq last: calc subset");
      calculateSubsetConflicts(matches, conflicts, matchSet);

      // start over with a new match set
      LOG_DEBUG("f neq last: Clear Matchset");
      matchSet.clear();
      LOG_DEBUG("f neq last: End");
    }

    LOG_DEBUG("matchset pushback");
    matchSet.push_back(it->second);
    LOG_DEBUG("lastEid=first");
    lastEid = it->first;
    LOG_DEBUG("End Loop");
  }

  LOG_DEBUG("Calculate SubSetConflicts");
  calculateSubsetConflicts(matches, conflicts, matchSet);
  LOG_DEBUG("Done");
}

void MatchConflicts::calculateSubsetConflicts(const vector<const Match *> &matches, ConflictMap& conflicts,
  const vector<int>& matchSet)
{
  // search for all possible match pair conflicts within a set.
  for (size_t i = 0; i < matchSet.size(); i++)
  {
    size_t m1 = matchSet[i];
    for (size_t j = i + 1; j < matchSet.size(); j++)
    {
      size_t m2 = matchSet[j];
      LOG_DEBUG("if m1 neq m2");
      // if these aren't the same match and it is conflicting in one direction or the other
      if (m1 != m2 &&
          (MergerFactory::getInstance().isConflicting(_map, matches[m1], matches[m2]) ||
           MergerFactory::getInstance().isConflicting(_map, matches[m2], matches[m1])))
      {
        // make sure we're consistent and put the smaller one first.
        if (m2 < m1)
        {
          LOG_DEBUG("Swap");
          swap(m1, m2);
        }
        LOG_DEBUG("Conflict Insert");
        conflicts.insert(m1, m2);
      }
    }
  }
}


}
