/*
 * This file is part of Hootenanny.
 *
 * Hootenanny is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * --------------------------------------------------------------------
 *
 * The following copyright notices are generated automatically. If you
 * have a new notice to add, please use the format:
 * " * @copyright Copyright ..."
 * This will properly maintain the copyright information. DigitalGlobe
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "DebugNetworkMapCreator.h"

// hoot
#include <hoot/core/visitors/MedianNodeVisitor.h>

namespace hoot
{

DebugNetworkMapCreator::DebugNetworkMapCreator(double matchThreshold) :
  _matchThreshold(matchThreshold)
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
  if (edgeScore->getScore() >= 1e-4)
  {
    ConstNodePtr n1 = _getMedianNode(map, edgeScore->getEdgeMatch()->getString1()->getMembers());
    ConstNodePtr n2 = _getMedianNode(map, edgeScore->getEdgeMatch()->getString2()->getMembers());

    WayPtr w(new Way(Status::Invalid, map->createNextWayId(), -1));
    w->addNode(n1->getId());
    w->addNode(n2->getId());
    Tags tags;
    tags.set("hoot:edge:score12", edgeScore->getScore12());
    tags.set("hoot:edge:score21", edgeScore->getScore21());
    tags.set("hoot:edge:score", edgeScore->getScore());
    tags.set("hoot:edge:id", edgeScore->getUid());
    tags.set("hoot:edge", edgeScore->toString());

    RelationPtr r(new Relation(Status::Invalid, map->createNextRelationId(), -1, "match"));
    r->setTags(tags);
    r->addElement("visual", w);

    foreach (const EdgeString::EdgeEntry& ee,
      edgeScore->getEdgeMatch()->getString1()->getAllEdges())
    {
      foreach (ConstElementPtr e, ee.getEdge()->getMembers())
      {
        r->addElement("string1", e);
      }
    }

    foreach (const EdgeString::EdgeEntry& ee,
      edgeScore->getEdgeMatch()->getString2()->getAllEdges())
    {
      foreach (ConstElementPtr e, ee.getEdge()->getMembers())
      {
        r->addElement("string2", e);
      }
    }

    map->addRelation(r);

    if (edgeScore->getScore() >= _matchThreshold)
    {
      tags.set("highway", "cycleway");
    }
    else
    {
      // a hack to make it easier to look at.
      tags.set("highway", "dashpurple");
    }
    w->setTags(tags);

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
    w->getTags().set("hoot:vertex", vertexScore->toString());
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
