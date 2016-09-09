#ifndef SEARCHRADIUSPROVIDER_H
#define SEARCHRADIUSPROVIDER_H

// hoot
#include <hoot/rnd/conflate/network/NetworkEdge.h>

namespace hoot
{

class SearchRadiusProvider
{
public:

  virtual Envelope getEnvelope(ConstNetworkEdgePtr e) const = 0;

  virtual Envelope getEnvelope(ConstNetworkVertexPtr v) const = 0;

  virtual Meters getSearchRadius(ConstNetworkEdgePtr e1) const = 0;

  virtual Meters getSearchRadius(ConstNetworkEdgePtr e1, ConstNetworkEdgePtr e2) const = 0;

  virtual Meters getSearchRadius(ConstNetworkVertexPtr v1) const = 0;

  virtual Meters getSearchRadius(ConstNetworkVertexPtr v1, ConstNetworkVertexPtr v2) const = 0;
};

}

#endif // SEARCHRADIUSPROVIDER_H
