#ifndef NETWORKEDGEMATCH_H
#define NETWORKEDGEMATCH_H

// tgs
#include <tgs/SharedPtr.h>

namespace hoot
{

/**
 * Represents a n:m match between edges in two networks.
 */
class NetworkEdgeMatch
{
public:
  NetworkEdgeMatch();
};

typedef shared_ptr<NetworkEdgeMatch> NetworkEdgeMatchPtr;
typedef shared_ptr<const NetworkEdgeMatch> ConstNetworkEdgeMatchPtr;

}

#endif // NETWORKEDGEMATCH_H
