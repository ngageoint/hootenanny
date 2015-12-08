#ifndef NETWORKEDGESCORE_H
#define NETWORKEDGESCORE_H

#include "NetworkEdge.h"

namespace hoot
{

class NetworkEdgeScore
{
public:
  NetworkEdgeScore(ConstNetworkEdgePtr e1, ConstNetworkEdgePtr e2, double score12,
    double score21) :
    _e1(e1),
    _e2(e2),
    _score12(score12),
    _score21(score21)
  {
  }

  ConstNetworkEdgePtr getE1() const { return _e1; }

  ConstNetworkEdgePtr getE2() const { return _e2; }

  double getScore() const { return _score12 * _score21; }

  double getScore12() const { return _score12; }

  double getScore21() const { return _score21; }

  QString toString() const
  {
    return QString("e1: %1 e2: %2 score: %3").arg(_e1->toString()).arg(_e2->toString()).
      arg(getScore());
  }

private:
  ConstNetworkEdgePtr _e1, _e2;
  double _score12, _score21;

};

typedef shared_ptr<NetworkEdgeScore> NetworkEdgeScorePtr;

}

#endif // NETWORKEDGESCORE_H
