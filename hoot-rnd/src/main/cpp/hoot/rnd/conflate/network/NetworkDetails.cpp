#include "NetworkDetails.h"

// hoot
#include <hoot/core/algorithms/DirectionFinder.h>
#include <hoot/core/algorithms/ProbabilityOfMatch.h>
#include <hoot/core/conflate/polygon/extractors/AngleHistogramExtractor.h>
#include <hoot/core/conflate/polygon/extractors/EuclideanDistanceExtractor.h>
#include <hoot/core/conflate/polygon/extractors/HausdorffDistanceExtractor.h>

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

  double result;

  if (isCandidateMatch(e1, e2) == false)
  {
    result = 0.0;
  }
  else
  {
    result = ProbabilityOfMatch::getInstance().expertProbability(_map, w1, w2);
  }

  return result;
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

bool NetworkDetails::isCandidateMatch(ConstNetworkEdgePtr e1, ConstNetworkEdgePtr e2)
{
  Meters ce1 = getSearchRadius(e1);
  Meters ce2 = getSearchRadius(e2);
  Meters ce = sqrt(ce1 * ce1 + ce2 * ce2);

  assert(e1->getMembers().size() == 1 && e2->getMembers().size() == 1);

  ConstElementPtr ep1 = e1->getMembers()[0];
  ConstElementPtr ep2 = e2->getMembers()[0];

  double d = EuclideanDistanceExtractor().distance(*_map, ep1, ep2);
  bool result = d <= ce;

  double hd = HausdorffDistanceExtractor().distance(*_map, ep1, ep2);
  result = result && hd <= ce;

  AngleHistogramExtractor ahe;
  ahe.setSmoothing(toRadians(20.0));
  double ah = ahe.extract(*_map, ep1, ep2);
  result = result && ah >= 0.5;

  return result;
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
