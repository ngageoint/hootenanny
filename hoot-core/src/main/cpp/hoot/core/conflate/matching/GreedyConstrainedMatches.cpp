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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020 Maxar (http://www.maxar.com/)
 */
#include "GreedyConstrainedMatches.h"

// hoot
#include <hoot/core/conflate/matching/Match.h>
#include <hoot/core/util/Log.h>

using namespace std;

namespace hoot
{

GreedyConstrainedMatches::GreedyConstrainedMatches(const ConstOsmMapPtr &map) :
  ConstrainedMatches(map)
{
  _score = -1;
  _timeLimit = -1;
}

class MatchComparator
{
public:

  MatchComparator(const vector<ConstMatchPtr>& matches) : _matches(matches) { }

  bool operator()(size_t i, size_t j)
  {
    return _matches[i]->getScore() > _matches[j]->getScore();
  }

private:

  const vector<ConstMatchPtr>& _matches;
};

vector<ConstMatchPtr> GreedyConstrainedMatches::calculateSubset()
{
  _score = -1;
  vector<ConstMatchPtr> result;

  if (_matches.size() == 0)
  {
    LOG_DEBUG("No match conflicts found.");
    return result;
  }

  // figure out all the pairs of matches that conflict.
  LOG_INFO("\tCalculating match conflicts using a greedy algorithm...");
  _calculateMatchConflicts();

  // if there are no conflicts, then there is nothing to solve.
  if (_conflicts.size() == 0)
  {
    return _matches;
  }

  // vector of the matches that we'll keep. The value in keepers corresponds to the index in
  // _matches.
  set<size_t> keepers;
  // vector of the match order. We won't actually re-order the matches since the conflict map
  // references those indices.
  vector<size_t> matchOrder(_matches.size());
  for (size_t i = 0; i < matchOrder.size(); i++)
  {
    matchOrder[i] = i;
  }

  MatchComparator mc(_matches);
  sort(matchOrder.begin(), matchOrder.end(), mc);

  // make a new match conflict map that goes in both directions.
  MatchConflicts::ConflictMap cm;
  for (MatchConflicts::ConflictMap::const_iterator it = _conflicts.constBegin();
       it != _conflicts.constEnd(); ++it)
  {
    cm.insert(it.key(), it.value());
    cm.insert(it.value(), it.key());
  }

  _score = 0;
  for (size_t i = 0; i < matchOrder.size(); i++)
  {
    bool keep = true;
    size_t mi = matchOrder[i];

    // see if any of our conflicting matches are already being kept
    for (MatchConflicts::ConflictMap::const_iterator it = cm.find(mi);
         it != cm.end() && it.key() == mi; ++it)
    {
      if (keepers.count(it.value()))
      {
        keep = false;
      }
    }

    if (keep)
    {
      keepers.insert(mi);
      result.push_back(_matches[mi]);
      // while adding EPSILON isn't strictly necessary here it does give consistent scores when
      // comparing between optimal and greedy.
      _score += _matches[mi]->getScore() + EPSILON;
    }
    else
    {
      LOG_TRACE("Removing match: " << _matches[mi]);
    }
  }

  return result;
}

}
