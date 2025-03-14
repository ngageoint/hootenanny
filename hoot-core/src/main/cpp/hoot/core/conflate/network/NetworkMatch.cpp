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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#include "NetworkMatch.h"

// hoot
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>

// Standard
#include <math.h>

using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(Match, NetworkMatch)

NetworkMatch::NetworkMatch(const ConstNetworkDetailsPtr& details, ConstEdgeMatchPtr edgeMatch, double score,
                           ConstMatchThresholdPtr mt, double scoringFunctionMax, double scoringFunctionCurveMidpointX,
                           double scoringFunctionCurveSteepness)
  : Match(mt),
    _details(details),
    _edgeMatch(edgeMatch)
{
  double p;

  // don't know enough about it this constant yet to know if should be a config value (should it be
  // set the same as x0?)
  if (score > 0.5)
  {
    // Send the score through a logistic function to keep the values in range.
    // TODO: These values are arbitrary and may need tweaking. - #3080

    // steepness
    double k = scoringFunctionCurveSteepness;
    // max value
    double L = scoringFunctionMax;
    // score of 0.5 gives a probability of 0.5
    double x0 = scoringFunctionCurveMidpointX;
    p = L / (1 + pow(M_E, -k * (score - x0)));
  }
  else
    p = score;

  _classification.setMatchP(p);
  _classification.setMissP(1.0 - p);

  LOG_VART(edgeMatch);
  LOG_VART(score);
  LOG_VART(p);

  // find all the match pairs
  _discoverWayPairs(details->getMap(), edgeMatch);
}

void NetworkMatch::_discoverWayPairs(ConstOsmMapPtr map, ConstEdgeMatchPtr edgeMatch)
{
  LOG_TRACE("Discovering way pairs...");

  // traverse the match and determine all the match pairs.
  ConstEdgeStringPtr string1 = edgeMatch->getString1();
  ConstEdgeStringPtr string2 = edgeMatch->getString2();
  Meters length1 = string1->calculateLength(map);
  Meters length2 = string2->calculateLength(map);

  // TODO: These loops assume that equal portions of a line equal the same point on the line. Said
  // another way, if you're 10% down line 1, then that is equivalent to 10% down line 2.
  // Unfortunately, this can be a very coarse estimate. Something like Frechet distance may improve
  // this matching. - see #3158

  // Its not obvious why we would allow node/way match pairs here, but forcing way/way match pairs
  // does lead to worse snapping at some road intersections as was found while working on #3386.
  // NetworkMatchCreator does have a candidate check for highways, so its also not completely
  // clear when nodes would be getting into the mix here.

  Meters d1 = 0.0;
  for (int i = 0; i < string1->getMembers().size(); ++i)
  {     
    ElementId element1Id = _toElement(string1->getEdge(i))->getElementId();
    LOG_VART(element1Id);
    ElementId element2Id = _toElement(string2->getEdgeAtOffset(map, d1 / length1 * length2))->getElementId();
    LOG_VART(element2Id);
    _pairs.emplace(element1Id, element2Id);

    d1 += string1->getEdge(i)->calculateLength(map);

    element1Id = _toElement(string1->getEdge(i))->getElementId();
    LOG_VART(element1Id);
    element2Id = _toElement(string2->getEdgeAtOffset(map, d1 / length1 * length2))->getElementId();
    LOG_VART(element2Id);
    _pairs.emplace(element1Id, element2Id);
  }

  Meters d2 = 0.0;
  for (int i = 0; i < string2->getMembers().size(); ++i)
  {
    ElementId element1Id = _toElement(string1->getEdgeAtOffset(map, d2 / length2 * length2))->getElementId();
    LOG_VART(element1Id);
    ElementId element2Id = _toElement(string2->getEdge(i))->getElementId();
    LOG_VART(element2Id);
    _pairs.emplace(element1Id, element2Id);

    d2 += string2->getEdge(i)->calculateLength(map);

    element1Id = _toElement(string1->getEdgeAtOffset(map, d2 / length2 * length2))->getElementId();
    LOG_VART(element1Id);
    element2Id = _toElement(string2->getEdge(i))->getElementId();
    LOG_VART(element2Id);
    _pairs.emplace(element1Id, element2Id);
  }

  LOG_VART(_pairs);
}

bool NetworkMatch::isConflicting(const ConstMatchPtr& other, const ConstOsmMapPtr& /*map*/,
                                 const QHash<QString, ConstMatchPtr>& /*matches*/) const
{
  set<pair<ElementId, ElementId>> s = other->getMatchPairs();

  // if any element ids overlap then they're conflicting.
  for (const auto& ip : s)
  {
    for (const auto& jp : _pairs)
    {
      if (ip.first == jp.first  || ip.first == jp.second || ip.second == jp.first || ip.second == jp.second)
      {
        LOG_TRACE("conflicting: " << other);
        return true;
      }
    }
  }

  return false;
}

QString NetworkMatch::toString() const
{
  return QString("Network Match (%1) pairs: %2 score:%3")
          .arg(getName(),  hoot::toString(_pairs), QString::number(getScore()));
}

ConstElementPtr NetworkMatch::_toElement(ConstNetworkEdgePtr edge) const
{
  QList<ConstElementPtr> members = edge->getMembers();
  if (members.size() != 1)
    throw NotImplementedException("Only one member is support in the network edge at this time.");

  return members[0];
}

}
