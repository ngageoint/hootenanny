#ifndef NETWORKVERTEXSCORE_H
#define NETWORKVERTEXSCORE_H

#include "NetworkVertex.h"

namespace hoot
{

class NetworkVertexScore
{
public:
  NetworkVertexScore(ConstNetworkVertexPtr v1, ConstNetworkVertexPtr v2, double score12,
    double score21) :
    _v1(v1),
    _v2(v2),
    _score12(score12),
    _score21(score21)
  {
  }

  ConstNetworkVertexPtr getV1() const { return _v1; }

  ConstNetworkVertexPtr getV2() const { return _v2; }

  double getScore() const { return _score12 * _score21; }

  double getScore12() const { return _score12; }

  double getScore21() const { return _score21; }

  QString toString() const
  {
    return QString("v1: %1 v2: %2 score: %3").arg(_v1->toString()).arg(_v2->toString()).
      arg(getScore());
  }

private:
  ConstNetworkVertexPtr _v1, _v2;
  double _score12, _score21;

};

typedef shared_ptr<NetworkVertexScore> NetworkVertexScorePtr;

}

#endif // NETWORKVERTEXSCORE_H
