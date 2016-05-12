#include "DebugNetworkMapCreator.h"

// hoot
#include <hoot/core/visitors/MedianNodeVisitor.h>

namespace hoot
{

DebugNetworkMapCreator::DebugNetworkMapCreator()
{
}

void DebugNetworkMapCreator::addDebugElements(OsmMapPtr map, QList<NetworkEdgeScorePtr> edgeScores,
  QList<NetworkVertexScorePtr> vertexScores)
{

  for (int i = 0; i < edgeScores.size(); ++i)
  {
    _addEdgeLink(map, edgeScores[i]);
  }

  for (int i = 0; i < vertexScores.size(); ++i)
  {
    _addVertexLink(map, vertexScores[i]);
  }
}

void DebugNetworkMapCreator::_addEdgeLink(OsmMapPtr map, NetworkEdgeScorePtr edgeScore)
{
  if (edgeScore->getScore() >= 1e-6)
  {
    ConstNodePtr n1 = _getMedianNode(map, edgeScore->getEdgeMatch()->getString1()->getMembers());
    ConstNodePtr n2 = _getMedianNode(map, edgeScore->getEdgeMatch()->getString2()->getMembers());

    WayPtr w(new Way(Status::Invalid, map->createNextWayId(), -1));
    w->addNode(n1->getId());
    w->addNode(n2->getId());
    w->getTags().set("hoot:edge:score12", edgeScore->getScore12());
    w->getTags().set("hoot:edge:score21", edgeScore->getScore21());
    w->getTags().set("hoot:edge:score", edgeScore->getScore());
    w->getTags().set("hoot:edge:id", edgeScore->getUid());
    if (edgeScore->getScore() >= 0.15)
    {
      w->getTags().set("highway", "cycleway");
    }
    else
    {
      // a hack to make it easier to look at.
      w->getTags().set("highway", "dashpurple");
    }

    map->addWay(w);
  }
}

void DebugNetworkMapCreator::_addVertexLink(OsmMapPtr map, NetworkVertexScorePtr vertexScore)
{
  ConstNodePtr n1 = _getMedianNode(map, vertexScore->getV1()->getElement());
  ConstNodePtr n2 = _getMedianNode(map, vertexScore->getV2()->getElement());

  if (vertexScore->getScore() >= 0.001)
  {
    WayPtr w(new Way(Status::Invalid, map->createNextWayId(), -1));
    w->addNode(n1->getId());
    w->addNode(n2->getId());
    w->getTags().set("hoot:vertex:score12", vertexScore->getScore12());
    w->getTags().set("hoot:vertex:score21", vertexScore->getScore21());
    w->getTags().set("hoot:vertex:score", vertexScore->getScore());
    if (vertexScore->getScore() >= 0.15)
    {
      w->getTags().set("highway", "footway");
    }
    else
    {
      w->getTags().set("highway", "path");
    }

    map->addWay(w);
  }
}

ConstNodePtr DebugNetworkMapCreator::_getMedianNode(ConstOsmMapPtr map, QList<ConstElementPtr> e)
{
  MedianNodeVisitor v;
  v.setOsmMap(map.get());

  for (int i = 0; i < e.size(); ++i)
  {
    e[i]->visitRo(*map, v);
  }

  return v.calculateMedianNode();
}

ConstNodePtr DebugNetworkMapCreator::_getMedianNode(ConstOsmMapPtr map, ConstElementPtr e)
{
  QList<ConstElementPtr> l;
  l.append(e);
  return _getMedianNode(map, l);
}


}
