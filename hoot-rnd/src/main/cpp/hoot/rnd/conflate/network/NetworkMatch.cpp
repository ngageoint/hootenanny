#include "NetworkMatch.h"

// Standard
#include <math.h>

namespace hoot
{

NetworkMatch::NetworkMatch(const ConstOsmMapPtr& /*map*/, const ElementId& eid1,
  const ElementId& eid2, double score, ConstMatchThresholdPtr mt) :
  Match(mt),
  _eid1(eid1),
  _eid2(eid2)
{
  // Send the score through a logistic function to keep the values in range. These values are
  // arbitrary and may need tweaking.
  // steepness
  double k = 2.0;
  // max value
  double L = 1.0;
  // score of 0.5 gives a probability of 0.5
  double x0 = 0.5;
  double p = L / (1 + pow(M_E, -k * (score - x0)));
  _classification.setMatchP(p);
  _classification.setMissP(1.0 - p);
  LOG_VAR(score);
  LOG_VAR(p);
  _pairs.insert(pair<ElementId, ElementId>(eid1, eid2));
}

bool NetworkMatch::isConflicting(const Match& other, const ConstOsmMapPtr& /*map*/) const
{
  set< pair<ElementId, ElementId> > s = other.getMatchPairs();

  for (set< pair<ElementId, ElementId> >::const_iterator it = s.begin(); it != s.end(); ++it)
  {
    const pair<ElementId, ElementId>& p = *it;
    if (p.first == _eid1 || p.second == _eid1 ||
      p.first == _eid2 || p.second == _eid2)
    {
      return true;
    }
  }

  return false;
}

QString NetworkMatch::toString() const
{
  return QString("Network Match (%1) e1: %2 e2: %3 score:%4").arg(getMatchName()).
    arg(_eid1.toString()).arg(_eid2.toString()).arg(getScore());
}

}
