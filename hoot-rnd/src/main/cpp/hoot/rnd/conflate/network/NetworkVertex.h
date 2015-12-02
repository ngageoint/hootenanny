#ifndef NETWORKVERTEX_H
#define NETWORKVERTEX_H

// hoot
#include <hoot/core/elements/Element.h>

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
  NetworkVertex(ConstElementPtr e) : _e(e) {}

  ConstElementPtr getElement() const { return _e; }

  ElementId getElementId() const { return _e->getElementId(); }

  QString toString() const;

private:
  ConstElementPtr _e;
};

typedef shared_ptr<NetworkVertex> NetworkVertexPtr;
typedef shared_ptr<const NetworkVertex> ConstNetworkVertexPtr;

inline uint qHash(const ConstNetworkVertexPtr& v)
{
  return qHash(v->getElementId());
}

inline uint qHash(const NetworkVertexPtr& v)
{
  return qHash(v->getElementId());
}

}

#endif // NETWORKVERTEX_H
