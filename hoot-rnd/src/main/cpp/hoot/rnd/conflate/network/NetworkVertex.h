#ifndef NETWORKVERTEX_H
#define NETWORKVERTEX_H

#include <tgs/SharedPtr.h>

namespace hoot
{

/**
 * Why NetworkVertex? To avoid confusion with the OSM Node.
 *
 * A node is not necessarily associated with just a single OSM node. A node could represent an
 * entire roudabout instead of each of the intersections that make up a roudabout. Whatever makes
 * the conflation more effective.
 */
class NetworkVertex
{
public:
  NetworkVertex();
};

typedef shared_ptr<NetworkVertex> NetworkVertexPtr;
typedef shared_ptr<const NetworkVertex> ConstNetworkVertexPtr;

}

#endif // NETWORKVERTEX_H
