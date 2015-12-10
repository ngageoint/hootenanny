#ifndef NETWORKDETAILS_H
#define NETWORKDETAILS_H

// hoot
#include <hoot/core/OsmMap.h>
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

  double getEdgeMatchScore(ConstNetworkEdgePtr e1, ConstNetworkEdgePtr e2);

  Envelope getEnvelope(ConstNetworkEdgePtr e);

  Envelope getEnvelope(ConstNetworkVertexPtr v);

  Meters getSearchRadius(ConstNetworkEdgePtr e);

  Meters getSearchRadius(ConstNetworkVertexPtr v);

  bool isReversed(ConstNetworkEdgePtr e1, ConstNetworkEdgePtr e2);

  bool isCandidateMatch(ConstNetworkVertexPtr v1, ConstNetworkVertexPtr v2);

private:
  ConstOsmMapPtr _map;
  ConstOsmNetworkPtr _network;
};

typedef shared_ptr<NetworkDetails> NetworkDetailsPtr;
typedef shared_ptr<const NetworkDetails> ConstNetworkDetailsPtr;

}

#endif // NETWORKDETAILS_H
