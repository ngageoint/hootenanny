#include "NetworkMatch.h"

// hoot
#include <hoot/core/util/Log.h>

// Standard
#include <math.h>

namespace hoot
{

NetworkMatch::NetworkMatch(const ConstOsmMapPtr& map, ConstEdgeMatchPtr edgeMatch, double score,
  ConstMatchThresholdPtr mt) :
  Match(mt),
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
  _discoverWayPairs(map, edgeMatch);
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
        _toWay(string1->getEdge(i))->getElementId(),
        _toWay(string2->getEdgeAtOffset(map, d1 / length1 * length2))->getElementId()
      ));

    d1 += string1->getEdge(i)->calculateLength(map);

    _pairs.insert(pair<ElementId, ElementId>(
        _toWay(string1->getEdge(i))->getElementId(),
        _toWay(string2->getEdgeAtOffset(map, d1 / length1 * length2))->getElementId()
      ));
  }

  Meters d2 = 0.0;
  for (int i = 0; i < string2->getMembers().size(); ++i)
  {
    _pairs.insert(pair<ElementId, ElementId>(
        _toWay(string1->getEdgeAtOffset(map, d2 / length2 * length2))->getElementId(),
        _toWay(string2->getEdge(i))->getElementId()
      ));

    d2 += string2->getEdge(i)->calculateLength(map);

    _pairs.insert(pair<ElementId, ElementId>(
        _toWay(string1->getEdgeAtOffset(map, d2 / length2 * length2))->getElementId(),
        _toWay(string2->getEdge(i))->getElementId()
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
  return QString("Network Match (%1) pairs: %2 score:%3").arg(getMatchName()).
    arg(hoot::toString(_pairs)).arg(getScore());
}

ConstWayPtr NetworkMatch::_toWay(ConstNetworkEdgePtr edge) const
{
  QList<ConstElementPtr> members = edge->getMembers();

  if (members.size() != 1)
  {
    throw NotImplementedException("Only one member is support in the network edge at this time.");
  }
  if (members[0]->getElementType() != ElementType::Way)
  {
    throw NotImplementedException("Only way members are supported in the network edge at this"
      " time.");
  }

  return dynamic_pointer_cast<const Way>(members[0]);
}

}
