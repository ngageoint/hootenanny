#ifndef NETWORKEDGESCORE_H
#define NETWORKEDGESCORE_H

#include "EdgeMatch.h"

namespace hoot
{

class NetworkEdgeScore
{
public:
  NetworkEdgeScore(ConstEdgeMatchPtr em, double score12, double score21) :
    _em(em),
    _score12(score12),
    _score21(score21)
  {
  }

  ConstEdgeMatchPtr getEdgeMatch() const { return _em; }

  double getScore() const { return _score12 * _score21; }

  double getScore12() const { return _score12; }

  double getScore21() const { return _score21; }

  QString getUid() const { return _uid; }

  void setUid(const QString uid) { _uid = uid; }

  QString toString() const
  {
    return QString("em: %1 score: %3").arg(_em->toString()).
      arg(getScore());
  }

private:
  ConstEdgeMatchPtr _em;
  double _score12, _score21;
  QString _uid;

};

typedef shared_ptr<NetworkEdgeScore> NetworkEdgeScorePtr;

}

#endif // NETWORKEDGESCORE_H
