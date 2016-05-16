#include "EdgePair.h"

namespace hoot
{

int EdgePair::count = 0;

EdgePair::EdgePair(ConstNetworkEdgePtr ep1, ConstNetworkEdgePtr ep2) : e1(ep1), e2(ep2)
{
  count++;
  id = count;
}

/**
 * Returns from for the second edge accounting for the reversal. E.g. getFrom1() will always
 * match to getFrom2().
 */
ConstNetworkVertexPtr EdgePair::getFrom2() const
{
  if (reversed)
  {
    return e2->getTo();
  }
  else
  {
    return e2->getFrom();
  }
}

/**
 * Returns from for the second edge accounting for the reversal. E.g. getTo1() will always
 * match to getTo2().
 */
ConstNetworkVertexPtr EdgePair::getTo2() const
{
  if (reversed)
  {
    return e2->getFrom();
  }
  else
  {
    return e2->getTo();
  }
}

QString EdgePair::toString() const
{
  return QString::number(id) + " " + e1->toString() + ", " + e2->toString() +
    (reversed ? "(rev)" : "");
}

}
