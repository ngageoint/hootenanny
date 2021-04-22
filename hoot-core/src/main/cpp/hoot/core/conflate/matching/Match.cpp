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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#include "Match.h"

// hoot
#include <hoot/core/conflate/matching/MatchClassification.h>
#include <hoot/core/conflate/matching/MatchThreshold.h>
#include <hoot/core/conflate/matching/MatchType.h>
#include <hoot/core/elements/ElementId.h>

namespace hoot
{

long Match::_orderCount = 0;

QString Match::explain() const
{
  return toString();
}

MatchType Match::getType() const
{
  return _threshold->getType(*this);
}

QHash<QString, ConstMatchPtr> Match::getIdIndexedMatches(
  const std::vector<ConstMatchPtr>& matches)
{
  QHash<QString, ConstMatchPtr> indexedMatches;
  for (std::vector<ConstMatchPtr>::const_iterator it = matches.begin(); it != matches.end(); ++it)
  {
    ConstMatchPtr match = *it;
    indexedMatches[matchPairsToString(match->getMatchPairs())] = match;
  }
  return indexedMatches;
}

QString Match::matchPairsToString(
  const std::set<std::pair<ElementId, ElementId>>& matchPairs)
{
  // sorting each individual pair by element id for consistency; For multiple pairs, collection
  // order must match, which may end up being too strict.
  QString str;
  for (std::set<std::pair<ElementId, ElementId>>::const_iterator it = matchPairs.begin();
       it != matchPairs.end(); ++it)
  {
    std::pair<ElementId, ElementId> idPair = *it;
    const ElementId firstId = idPair.first;
    const ElementId secondId = idPair.second;
    if (firstId < secondId)
    {
      str += firstId.toString() + "," + secondId.toString();
    }
    else
    {
      str += secondId.toString() + "," + firstId.toString();
    }
    str += ";";
  }
  str.chop(1);
  return str;
}

}
