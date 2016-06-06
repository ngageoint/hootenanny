#ifndef NETWORKDETAILS_H
#define NETWORKDETAILS_H

// hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/algorithms/SublineStringMatcher.h>
#include <hoot/core/conflate/highway/HighwayClassifier.h>
#include <hoot/rnd/conflate/network/EdgeString.h>
#include <hoot/rnd/conflate/network/OsmNetwork.h>

namespace hoot
{

/**
 * This abstracts the details of a network (e.g. positions, bounding boxes, etc.).
 *
 * The advantage is that we don't link concepts such as OsmMap and ElementIds directly to the
 * network algorithms.
 */
class NetworkDetails
{
public:
  NetworkDetails(ConstOsmMapPtr map, ConstOsmNetworkPtr network);

  Meters calculateLength(ConstNetworkEdgePtr e);

  ConstOsmMapPtr getMap() const { return _map; }

  double getEdgeMatchScore(ConstNetworkEdgePtr e1, ConstNetworkEdgePtr e2);
  
  double getEdgeStringMatchScore(ConstEdgeStringPtr e1, ConstEdgeStringPtr e2);

  double getPartialEdgeMatchScore(ConstNetworkEdgePtr e1, ConstNetworkEdgePtr e2);

  Envelope getEnvelope(ConstNetworkEdgePtr e);

  Envelope getEnvelope(ConstNetworkVertexPtr v);

  Meters getSearchRadius(ConstNetworkEdgePtr e1) const;

  Meters getSearchRadius(ConstNetworkEdgePtr e1, ConstNetworkEdgePtr e2) const;

  Meters getSearchRadius(ConstNetworkVertexPtr v1) const;

  Meters getSearchRadius(ConstNetworkVertexPtr v1, ConstNetworkVertexPtr v2) const;

  bool isReversed(ConstNetworkEdgePtr e1, ConstNetworkEdgePtr e2);

  bool isCandidateMatch(ConstNetworkEdgePtr e1, ConstNetworkEdgePtr e2);

  bool isCandidateMatch(ConstNetworkVertexPtr v1, ConstNetworkVertexPtr v2);

  WayStringPtr toWayString(ConstEdgeStringPtr e) const;

private:
  shared_ptr<HighwayClassifier> _classifier;
  ConstOsmMapPtr _map;
  ConstOsmNetworkPtr _network;
  shared_ptr<SublineStringMatcher> _sublineMatcher;

  class SublineCache
  {
  public:
    bool reversed;
    double p;
  };

  QHash< ElementId, QHash<ElementId, SublineCache> > _sublineCache;

  const SublineCache& _getSublineCache(ConstWayPtr w1, ConstWayPtr w2);
};

typedef shared_ptr<NetworkDetails> NetworkDetailsPtr;
typedef shared_ptr<const NetworkDetails> ConstNetworkDetailsPtr;

}

#endif // NETWORKDETAILS_H
