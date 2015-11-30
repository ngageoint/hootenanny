#include "OsmNetwork.h"

namespace hoot
{

OsmNetwork::OsmNetwork()
{
}

void OsmNetwork::addEdge(NetworkEdgePtr edge)
{
  if (_eidToVertex.contains(edge->getFrom()->getElementId()) == false ||
    _eidToVertex.contains(edge->getTo()->getElementId()) == false)
  {
    throw IllegalArgumentException("Please add the vertices on an edge before adding the edge.");
  }
  foreach (const ConstElementPtr& e, edge->getMembers())
  {
    if (_eidToEdge.contains(e->getElementId()))
    {
      throw IllegalArgumentException(QString("A single ElementId is a member in multiple network "
        "edges. (%1 and %2)").arg(_eidToEdge[e->getElementId()]->toString()).arg(edge->toString()));
    }
    _eidToEdge[e->getElementId()] = edge;
  }
  _edges.append(edge);
}

void OsmNetwork::addVertex(NetworkVertexPtr node)
{
  _eidToVertex[node->getElementId()] = node;
}

QString OsmNetwork::toString()
{
  QStringList result;

  QSet<ElementId> touchedVertices;

  foreach (const NetworkEdgePtr& e, _edges)
  {
    result << e->toString();
    touchedVertices.insert(e->getFrom()->getElementId());
    touchedVertices.insert(e->getTo()->getElementId());
  }

  QSet<ElementId> untouchedVertices = QSet<ElementId>::fromList(_eidToVertex.keys()).
    subtract(touchedVertices);

  foreach (const ElementId& eid, untouchedVertices)
  {
    result << _eidToVertex[eid]->toString();
  }

  return result.join("\n");
}

}
