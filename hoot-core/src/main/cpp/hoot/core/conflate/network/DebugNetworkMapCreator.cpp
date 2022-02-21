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
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#include "DebugNetworkMapCreator.h"

// hoot
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/visitors/MedianNodeVisitor.h>

namespace hoot
{

DebugNetworkMapCreator::DebugNetworkMapCreator()
  : _matchThreshold(ConfigOptions().getNetworkMatchThreshold())
{
}

void DebugNetworkMapCreator::addDebugElements(OsmMapPtr map, QList<NetworkEdgeScorePtr> edgeScores,
                                              QList<NetworkVertexScorePtr> vertexScores) const
{
  for (const auto& edge : qAsConst(edgeScores))
    _addEdgeLink(map, edge);

  for (const auto& vertex : qAsConst(vertexScores))
    _addVertexLink(map, vertex);
}

void DebugNetworkMapCreator::_addEdgeLink(OsmMapPtr map, NetworkEdgeScorePtr edgeScore) const
{
  if (edgeScore->getScore() >= 1e-4)
  {
    ConstNodePtr n1 = _getMedianNode(map, edgeScore->getEdgeMatch()->getString1()->getMembers());
    ConstNodePtr n2 = _getMedianNode(map, edgeScore->getEdgeMatch()->getString2()->getMembers());

    WayPtr w =
      std::make_shared<Way>(
        Status::Invalid, map->createNextWayId(), ElementData::CIRCULAR_ERROR_EMPTY);
    w->addNode(n1->getId());
    w->addNode(n2->getId());
    Tags tags;
    tags.set(MetadataTags::HootEdgeScore12(), edgeScore->getScore12());
    tags.set(MetadataTags::HootEdgeScore21(), edgeScore->getScore21());
    tags.set(MetadataTags::HootEdgeScore(), edgeScore->getScore());
    tags.set(MetadataTags::HootEdgeId(), edgeScore->getEdgeMatch()->getUid());
    tags.set(MetadataTags::HootEdge(), edgeScore->toString());

    RelationPtr r =
      std::make_shared<Relation>(
        Status::Invalid, map->createNextRelationId(), ElementData::CIRCULAR_ERROR_EMPTY, "match");
    r->setTags(tags);
    r->addElement("visual", w);

    for (const auto& ee : edgeScore->getEdgeMatch()->getString1()->getAllEdges())
    {
      for (const auto& e : ee.getEdge()->getMembers())
        r->addElement("string1", e);
    }

    for (const auto& ee : edgeScore->getEdgeMatch()->getString2()->getAllEdges())
    {
      for (const auto& e : ee.getEdge()->getMembers())
        r->addElement("string2", e);
    }

    map->addRelation(r);

    if (edgeScore->getScore() >= _matchThreshold)
      tags.set("highway", "cycleway");
    else  // a hack to make it easier to look at.
      tags.set("highway", "dashpurple");

    w->setTags(tags);

    map->addWay(w);
  }
}

void DebugNetworkMapCreator::_addVertexLink(OsmMapPtr map, NetworkVertexScorePtr vertexScore) const
{
  ConstNodePtr n1 = _getMedianNode(map, vertexScore->getV1()->getElement());
  ConstNodePtr n2 = _getMedianNode(map, vertexScore->getV2()->getElement());

  if (vertexScore->getScore() >= 0.001)
  {
    WayPtr w =
      std::make_shared<Way>(Status::Invalid, map->createNextWayId(), ElementData::CIRCULAR_ERROR_EMPTY);
    w->addNode(n1->getId());
    w->addNode(n2->getId());
    w->getTags().set(MetadataTags::HootVertexScore12(), vertexScore->getScore12());
    w->getTags().set(MetadataTags::HootVertexScore21(), vertexScore->getScore21());
    w->getTags().set(MetadataTags::HootVertexScore(), vertexScore->getScore());
    w->getTags().set(MetadataTags::HootVertex(), vertexScore->toString());
    if (vertexScore->getScore() >= _matchThreshold)
      w->getTags().set("highway", "footway");
    else
      w->getTags().set("highway", "path");

    map->addWay(w);
  }
}

ConstNodePtr DebugNetworkMapCreator::_getMedianNode(ConstOsmMapPtr map, const QList<ConstElementPtr>& e) const
{
  MedianNodeVisitor v;
  for (const auto& element : qAsConst(e))
    element->visitRo(*map, v);
  return v.calculateMedianNode();
}

ConstNodePtr DebugNetworkMapCreator::_getMedianNode(ConstOsmMapPtr map, ConstElementPtr e) const
{
  QList<ConstElementPtr> l;
  l.append(e);
  return _getMedianNode(map, l);
}


}
