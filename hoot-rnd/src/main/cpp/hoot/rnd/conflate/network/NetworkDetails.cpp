#include "NetworkDetails.h"

// hoot
#include <hoot/core/algorithms/DirectionFinder.h>
#include <hoot/core/algorithms/ProbabilityOfMatch.h>
#include <hoot/core/conflate/polygon/extractors/EuclideanDistanceExtractor.h>

namespace hoot
{

NetworkDetails::NetworkDetails(ConstOsmMapPtr map, ConstOsmNetworkPtr network) :
  _map(map),
  _network(network)
{
}

double NetworkDetails::getEdgeMatchScore(ConstNetworkEdgePtr e1, ConstNetworkEdgePtr e2)
{
  assert(e1->getMembers().size() == 1);
  assert(e2->getMembers().size() == 1);

  ConstWayPtr w1 = dynamic_pointer_cast<const Way>(e1->getMembers()[0]);
  ConstWayPtr w2 = dynamic_pointer_cast<const Way>(e2->getMembers()[0]);

  return ProbabilityOfMatch::getInstance().expertProbability(_map, w1, w2);
}

Envelope NetworkDetails::getEnvelope(ConstNetworkEdgePtr e)
{
  auto_ptr<Envelope> env(e->getMembers()[0]->getEnvelope(_map));
  for (int i = 1; i < e->getMembers().size(); ++i)
  {
    auto_ptr<Envelope> env2(e->getMembers()[i]->getEnvelope(_map));
    env->expandToInclude(env2.get());
  }

  return *env;
}

Envelope NetworkDetails::getEnvelope(ConstNetworkVertexPtr v)
{
  auto_ptr<Envelope> env(v->getElement()->getEnvelope(_map));

  return *env;
}

Meters NetworkDetails::getSearchRadius(ConstNetworkEdgePtr e)
{
  double ce = -1;

  for (int i = 0; i < e->getMembers().size(); ++i)
  {
    if (ce < e->getMembers()[0]->getCircularError())
    {
      ce = e->getMembers()[0]->getCircularError();
    }
  }

  return ce;
}

Meters NetworkDetails::getSearchRadius(ConstNetworkVertexPtr v)
{
  return v->getElement()->getCircularError();
}

bool NetworkDetails::isCandidateMatch(ConstNetworkVertexPtr v1, ConstNetworkVertexPtr v2)
{
  Meters ce = max(getSearchRadius(v1), getSearchRadius(v2));

  double d = EuclideanDistanceExtractor().distance(*_map, v1->getElement(), v2->getElement());

  return d <= ce;
}

bool NetworkDetails::isReversed(ConstNetworkEdgePtr e1, ConstNetworkEdgePtr e2)
{
  assert(e1->getMembers().size() == 1);
  assert(e2->getMembers().size() == 1);

  ConstWayPtr w1 = dynamic_pointer_cast<const Way>(e1->getMembers()[0]);
  ConstWayPtr w2 = dynamic_pointer_cast<const Way>(e2->getMembers()[0]);

  return !DirectionFinder::isSimilarDirection(_map, w1, w2);
}

}
