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
#include "NetworkMerger.h"

// hoot
#include <hoot/core/algorithms/linearreference/NaiveWayMatchStringMapping.h>
#include <hoot/core/algorithms/WayMatchStringMerger.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/conflate/NodeToWayMap.h>
#include <hoot/core/ops/RecursiveElementRemover.h>
#include <hoot/core/ops/ReplaceElementOp.h>
#include <hoot/core/schema/TagMergerFactory.h>
#include <hoot/core/visitors/ExtractNodesVisitor.h>

namespace hoot
{

NetworkMerger::NetworkMerger(const set< pair<ElementId, ElementId> >& pairs,
  ConstEdgeMatchPtr edgeMatch, ConstNetworkDetailsPtr details) :
  _pairs(pairs),
  _edgeMatch(edgeMatch),
  _details(details)
{
  assert(_pairs.size() >= 1);
}

void NetworkMerger::apply(const OsmMapPtr& map,
  vector< pair<ElementId, ElementId> >& replaced) const
{
  // put the matched edges into a format where we can map any point on one edge to the
  // corresponding point on the other edge.

  /// @todo throw an exception if the input edge match contains partial matches. We will need to
  /// review this method and add more code with partial matches. E.g.
  /// - Will we still create relations out of the scrap bits?

  // convert the EdgeStrings into WaySublineStrings
  WayStringPtr str1 = _details->toWayString(_edgeMatch->getString1());
  WayStringPtr str2 = _details->toWayString(_edgeMatch->getString2());

  WayMatchStringMappingPtr mapping(new NaiveWayMatchStringMapping(str1, str2));

  /******************
   * At the beginning the merger should identify where the primary way should be broken into bits
   * then each way in the secondary will match 1 or more bits in the primary. This mapping can be
   * created at the beginning in the merger and used throughout the rest of the operations.
   ******************/

  WayMatchStringMergerPtr merger(new WayMatchStringMerger(map, mapping, replaced));

  // merge the tags in the keeper segments
  merger->setTagMerger(TagMergerFactory::getInstance().getDefaultPtr());
  merger->mergeTags();

  // set the status on all keeper ways to conflated.
  merger->setKeeperStatus(Status::Conflated);

  // go through all the nodes in the scrap
  QList<ConstNodePtr> scrapNodes;
  ExtractNodesVisitor extractVisitor(scrapNodes);
  str2->visitRo(*map, extractVisitor);
  shared_ptr<NodeToWayMap> n2w = map->getIndex().getNodeToWayMap();
  for (int i = 0; i < scrapNodes.size(); ++i)
  {
    // if the node contains informational tags or is part of another way
    if (scrapNodes[i]->getTags().getInformationCount() > 0 ||
      n2w->getWaysByNode(scrapNodes[i]->getId()).size() >= 2)
    {
      // move corresponding intersection nodes and non-empty nodes into the keeper segments.
      merger->mergeNode(scrapNodes[i]->getElementId());
    }
  }

  /// @todo this will need to replace one scrap with possibly multiple keeper elements
  /// - think about the case when the way is part of an interstate or bus relation
  // remove the duplicate element.
  merger->replaceScraps();
}

QString NetworkMerger::toString() const
{
  QString s = hoot::toString(getPairs());
  return QString("NetworkMerger %1").arg(s);
}

}
