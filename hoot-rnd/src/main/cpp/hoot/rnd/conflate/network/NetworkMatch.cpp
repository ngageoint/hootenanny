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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "NetworkMatch.h"

// hoot
#include <hoot/core/util/Log.h>

// Standard
#include <math.h>

namespace hoot
{

NetworkMatch::NetworkMatch(const ConstNetworkDetailsPtr &details, ConstEdgeMatchPtr edgeMatch,
  double score,
  ConstMatchThresholdPtr mt) :
  Match(mt),
  _details(details),
  _edgeMatch(edgeMatch)
{
  double p;

  if (score > 0.5)
  {
    // Send the score through a logistic function to keep the values in range. These values are
    // arbitrary and may need tweaking.
    // steepness
    double k = 2.0;
    // max value
    double L = 1.0;
    // score of 0.5 gives a probability of 0.5
    double x0 = 0.5;
    p = L / (1 + pow(M_E, -k * (score - x0)));
  }
  else
  {
    p = score;
  }
  _classification.setMatchP(p);
  _classification.setMissP(1.0 - p);
  LOG_VAR(score);
  LOG_VAR(p);

  // find all the match pairs
  _discoverWayPairs(details->getMap(), edgeMatch);
}

void NetworkMatch::_discoverWayPairs(ConstOsmMapPtr map, ConstEdgeMatchPtr edgeMatch)
{
  // traverse the match and determine all the match pairs.
  ConstEdgeStringPtr string1 = edgeMatch->getString1();
  ConstEdgeStringPtr string2 = edgeMatch->getString2();
  Meters length1 = string1->calculateLength(map);
  Meters length2 = string2->calculateLength(map);

  // These loops assume that equal portions of a line equal the same point on the line. Said another
  // way if you're 10% down line 1, then that is equivalent to 10% down line 2. Unfortunately this
  // can be a very coarse estimate. Something like [1] may improve this matching.
  //
  // 1. https://github.com/ngageoint/hootenanny/issues/426
  Meters d1 = 0.0;
  for (int i = 0; i < string1->getMembers().size(); ++i)
  {
    _pairs.insert(pair<ElementId, ElementId>(
        _toElement(string1->getEdge(i))->getElementId(),
        _toElement(string2->getEdgeAtOffset(map, d1 / length1 * length2))->getElementId()
      ));

    d1 += string1->getEdge(i)->calculateLength(map);

    _pairs.insert(pair<ElementId, ElementId>(
        _toElement(string1->getEdge(i))->getElementId(),
        _toElement(string2->getEdgeAtOffset(map, d1 / length1 * length2))->getElementId()
      ));
  }

  Meters d2 = 0.0;
  for (int i = 0; i < string2->getMembers().size(); ++i)
  {
    _pairs.insert(pair<ElementId, ElementId>(
        _toElement(string1->getEdgeAtOffset(map, d2 / length2 * length2))->getElementId(),
        _toElement(string2->getEdge(i))->getElementId()
      ));

    d2 += string2->getEdge(i)->calculateLength(map);

    _pairs.insert(pair<ElementId, ElementId>(
        _toElement(string1->getEdgeAtOffset(map, d2 / length2 * length2))->getElementId(),
        _toElement(string2->getEdge(i))->getElementId()
      ));
  }
}

bool NetworkMatch::isConflicting(const Match& other, const ConstOsmMapPtr& /*map*/) const
{
  set< pair<ElementId, ElementId> > s = other.getMatchPairs();

  // if any element ids overlap then they're conflicting.
  for (set< pair<ElementId, ElementId> >::const_iterator it = s.begin(); it != s.end(); ++it)
  {
    const pair<ElementId, ElementId>& ip = *it;

    for (set< pair<ElementId, ElementId> >::const_iterator jt = _pairs.begin(); jt != _pairs.end();
      ++jt)
    {
      const pair<ElementId, ElementId>& jp = *jt;

      if (ip.first == jp.first || ip.second == jp.first ||
        ip.second == jp.first || ip.second == jp.second)
      {
        return true;
      }
    }
  }

  return false;
}

QString NetworkMatch::toString() const
{
  LOG_VAR(_threshold->toString());
  return QString("Network Match (%1) pairs: %2 score:%3").arg(getMatchName()).
    arg(hoot::toString(_pairs)).arg(getScore());
}

ConstElementPtr NetworkMatch::_toElement(ConstNetworkEdgePtr edge) const
{
  QList<ConstElementPtr> members = edge->getMembers();

  if (members.size() != 1)
  {
    throw NotImplementedException("Only one member is support in the network edge at this time.");
  }

  return members[0];
}

}
