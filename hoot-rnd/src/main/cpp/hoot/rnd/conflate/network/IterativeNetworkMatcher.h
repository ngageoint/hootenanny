#ifndef ITERATIVENETWORKMATCHER_H
#define ITERATIVENETWORKMATCHER_H

// hoot
#include <hoot/rnd/conflate/network/NetworkEdgeMatch.h>
#include <hoot/rnd/conflate/network/OsmNetwork.h>

namespace hoot
{

class IterativeNetworkMatcher
{
public:
  IterativeNetworkMatcher();

  void matchNetworks(OsmNetworkPtr n1, OsmNetworkPtr n2);

  QList<ConstNetworkEdgeMatchPtr> getEdgeMatches();
};

}

#endif // ITERATIVENETWORKMATCHER_H
