#ifndef NETWORKEDGE_H
#define NETWORKEDGE_H

// hoot
#include <hoot/core/elements/Element.h>
#include <hoot/rnd/conflate/network/NetworkVertex.h>

// Qt
#include <QSet>

namespace hoot
{

/**
 * An edge doesn't necessarily represent a single way. For instance an edge may represent both
 * roads that make up a divided highway. It is up to the network extractor to determine how this
 * is represented.
 */
class NetworkEdge
{
public:
  NetworkEdge();

  NetworkEdge(NetworkVertexPtr from, NetworkVertexPtr to, bool directed);

  void addMember(ConstElementPtr e) { _members.append(e); }

  NetworkVertexPtr getFrom() { return _from; }

  QList<ConstElementPtr> getMembers() const { return _members; }

  NetworkVertexPtr getTo() { return _to; }

  bool isDirected() { return _directed; }

  void setMembers(QList<ConstElementPtr> members) { _members = members; }

  QString toString() const;

private:
  NetworkVertexPtr _from, _to;
  bool _directed;
  QList<ConstElementPtr> _members;
};

typedef shared_ptr<NetworkEdge> NetworkEdgePtr;
typedef shared_ptr<const NetworkEdge> ConstNetworkEdgePtr;

inline uint qHash(const NetworkEdgePtr& v)
{
  uint result = 0;
  QList<ConstElementPtr> l = v->getMembers();
  for (int i = 0; i < l.size(); ++i)
  {
    result = qHash(l[i]->getElementId()) ^ result;
  }
  result = qHash(v->getFrom()) ^ result;
  result = qHash(v->getTo()) ^ result;

  return result;
}

}

#endif // NETWORKEDGE_H
