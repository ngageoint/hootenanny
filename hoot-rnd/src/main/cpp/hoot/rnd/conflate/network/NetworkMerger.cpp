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
#include <hoot/core/ops/RecursiveElementRemover.h>
#include <hoot/core/ops/ReplaceElementOp.h>
#include <hoot/core/schema/TagMergerFactory.h>

namespace hoot
{

NetworkMerger::NetworkMerger(const set< pair<ElementId, ElementId> >& pairs,
  ConstEdgeMatchPtr edgeMatch) :
  _pairs(pairs),
  _edgeMatch(edgeMatch)
{
  assert(_pairs.size() == 1);
}

void NetworkMerger::apply(const OsmMapPtr& /*map*/,
  vector< pair<ElementId, ElementId> >& /*replaced*/) const
{
  // put the matched edges into a format where we can map any point on one edge to the
  // corresponding point on the other edge.

  /// @todo throw an exception if the input edge match contains partial matches. We will need to
  /// review this method and add more code with partial matches. E.g.
  /// - Will we still create relations out of the scrap bits?

  WaySublineMatchStringPtr matchString = _createMatchString();
  WayMatchStringMappingPtr mapping(new NaiveWayMatchStringMapping(matchString));

  /******************
   * At the beginning the merger should identify where the primary way should be broken into bits
   * then each way in the secondary will match 1 or more bits in the primary. This mapping can be
   * created at the beginning in the merger and used throughout the rests of the operations.
   ******************/

  WayMatchStringMergerPtr merger(new WayMatchStringMerger(matchString, mapping));

  /// @todo can this be used in some way? HighwaySnapMerger::_splitElement
  /// how about this? MultiLineStringSplitter

  // merge the tags in the keeper segments
  merger->mergeTags(TagMergerFactory::getInstance().getDefault());

  // set the status on all keeper ways to conflated.
  merger->setKeeperStatus(Status::Conflated);

  // go through all the nodes in the scrap
    // if the node contains informational tags or is part of a larger intersection
      // move corresponding intersection nodes and non-empty nodes into the keeper segments.
      /// @todo
      //merger->mergeNode(scrapNode);

  /// @todo this will need to replace one scrap with possibly multiple keeper elements
  /// - think about the case when the way is part of an interstate or bus relation
  // remove the duplicate element.
  /// @todo ReplaceElementOp(keeper->getElementId(), scrap->getElementId()).apply(map);

  // we don't want to modify any of the scrap geometries until all merging is complete this will
  // avoid issues with the scrap lengths changing.

  /// @todo RecursiveElementRemover(scrap->getElementId()).apply(map);
  /// @todo scrap->getTags().clear();

  /// @todo replaced.push_back(pair<ElementId, ElementId>(scrap->getElementId(), keeper->getElementId()));
}

QString NetworkMerger::toString() const
{
  QString s = hoot::toString(getPairs());
  return QString("NetworkMerger %1").arg(s);
}

}
