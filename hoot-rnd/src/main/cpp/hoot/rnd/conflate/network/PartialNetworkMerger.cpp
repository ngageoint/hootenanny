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
 * @copyright Copyright (C) 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "PartialNetworkMerger.h"

// hoot
#include <hoot/core/algorithms/linearreference/NaiveWayMatchStringMapping.h>
#include <hoot/core/algorithms/WayMatchStringSplitter.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/io/OsmJsonWriter.h>
#include <hoot/core/conflate/NodeToWayMap.h>
#include <hoot/core/conflate/highway/HighwayMatch.h>
#include <hoot/core/conflate/ReviewMarker.h>
#include <hoot/core/ops/RecursiveElementRemover.h>
#include <hoot/core/ops/ReplaceElementOp.h>
#include <hoot/core/schema/TagMergerFactory.h>
#include <hoot/core/visitors/ExtractNodesVisitor.h>

namespace hoot
{

PartialNetworkMerger::PartialNetworkMerger(const set< pair<ElementId, ElementId> >& pairs,
  QSet<ConstEdgeMatchPtr> edgeMatches,
  ConstNetworkDetailsPtr details) :
  _pairs(pairs),
  _edgeMatches(edgeMatches),
  _details(details)
{
  assert(_pairs.size() >= 1);
}

void PartialNetworkMerger::_appendSublineMappings(
  QList<WayMatchStringMerger::SublineMappingPtr> mappings) const
{
  foreach (WayMatchStringMerger::SublineMappingPtr sm, mappings)
  {
    foreach (WayMatchStringMerger::SublineMappingPtr other, _allSublineMappings)
    {
      if (other->getSubline1().overlaps(sm->getSubline1()))
      {
        throw NeedsReviewException("Internal Error: Overlapping partial matches were found. To "
          "resolve please make the logical conflation operations manually. Please report this as "
          "an error and include sample data to recreate.");
      }
    }
    _allSublineMappings.append(sm);
  }
}

void PartialNetworkMerger::apply(const OsmMapPtr& map,
  vector< pair<ElementId, ElementId> >& replaced) const
{
  _mergerList.clear();

  if (_edgeMatches.size() == 1 && (*_edgeMatches.begin())->containsStub())
  {
    _processStubMatch(map, replaced, (*_edgeMatches.begin()));
  }
  else
  {
    _processFullMatch(map, replaced);
  }
}

void PartialNetworkMerger::_applyMerger(const OsmMapPtr& map, WayMatchStringMergerPtr merger) const
{
  // we changed the sublines so we must update the indices.
  merger->updateSublineMapping();

  WayStringPtr str2 = merger->getMapping()->getWayString2();

  merger->mergeTags();

  // set the status on all keeper ways to conflated.
  merger->setKeeperStatus(Status::Conflated);

  // go through all the nodes in the scrap
  QList<ConstNodePtr> scrapNodeList;
  ExtractNodesVisitor extractVisitor(scrapNodeList);
  str2->visitRo(*map, extractVisitor);
  shared_ptr<NodeToWayMap> n2w = map->getIndex().getNodeToWayMap();
  LOG_VAR(str2);
  LOG_VAR(scrapNodeList);
  LOG_VAR(n2w->getWaysByNode(-36));
  QSet<ConstNodePtr> scrapNodeSet = QSet<ConstNodePtr>::fromList(scrapNodeList);
  foreach (ConstNodePtr n, scrapNodeSet)
  {
    // if the node contains informational tags or is part of another way
    if (n2w->getWaysByNode(n->getId()).size() >= 2)
    {
      // move corresponding intersection nodes and non-empty nodes into the keeper segments and
      // make sure the interesection snaps to a start/end node of a way
      merger->mergeIntersection(n->getElementId());
    }
    else if (n->getTags().getInformationCount() > 0)
    {
      // move corresponding intersection nodes and non-empty nodes into the keeper segments.
      merger->mergeNode(n->getElementId());
    }
  }

  /// @todo this will need to replace one scrap with possibly multiple keeper elements
  /// - think about the case when the way is part of an interstate or bus relation
  // remove the duplicate element.
  merger->replaceScraps();
}

WayMatchStringMergerPtr PartialNetworkMerger::_createMatchStringMerger(
  const OsmMapPtr& map,
  vector< pair<ElementId, ElementId> >& replaced,
  ConstEdgeMatchPtr edgeMatch) const
{
  // convert the EdgeStrings into WaySublineStrings
  WayStringPtr str1 = _details->toWayString(edgeMatch->getString1(), *this);
  WayStringPtr str2 = _details->toWayString(edgeMatch->getString2(), *this);

  WayMatchStringMappingPtr mapping(new NaiveWayMatchStringMapping(str1, str2));

  /******************
   * At the beginning the merger should identify where the primary way should be broken into bits
   * then each way in the secondary will match 1 or more bits in the primary. This mapping can be
   * created at the beginning in the merger and used throughout the rest of the operations.
   ******************/

  WayMatchStringMergerPtr merger(new WayMatchStringMerger(map, mapping, replaced));

  // merge the tags in the keeper segments
  merger->setTagMerger(TagMergerFactory::getInstance().getDefaultPtr());

  return merger;
}

ElementId PartialNetworkMerger::mapEid(const ElementId &oldEid) const
{
  if (_substitions.contains(oldEid))
  {
    return mapEid(_substitions[oldEid]);
  }

  return oldEid;
}

void PartialNetworkMerger::_processFullMatch(const OsmMapPtr& map,
  vector<pair<ElementId, ElementId> > &replaced) const
{
  foreach (ConstEdgeMatchPtr e, _edgeMatches)
  {
    if (e->getString1()->isStub() || e->getString2()->isStub())
    {
      LOG_VARE(_edgeMatches);
      LOG_VARE(e);
      throw IllegalArgumentException("Didn't expect a stub in a full match.");
    }
  }

  ///
  /// This approach may seem a little odd at first. We need to accomodate the following problems:
  ///
  /// - Merging while using relations to keep track of scraps is a no-go. This becomes horribly
  ///   fragmented and difficult to manage for both the user and in code.
  /// - All the interrelated matches must be aware of scraps that are created by other merges and
  ///   create a consistent output.
  /// - A single set of edge matches could be arbitrarily large. Don't use any algorithms that
  ///   are unreasonably computationally complex.
  ///
  /// The approach is documented in the code as well, but it boils down to:
  ///
  /// 1. Find all the matching bits in the ways and record them for each edge match.
  /// 2. Split all the ways at one time so we don't have to keep track of scraps with relations or
  ///    similar.
  /// 3. Merge the matched sublines in turn.
  ///

  QList<WayStringPtr> string2List;

  // calculate all the mappings and split points for all matches.
  foreach (ConstEdgeMatchPtr em, _edgeMatches)
  {
    _mergerList.append(_createMatchStringMerger(map, replaced, em));

    // Put all split points and mappings into a single structure that can be used to split ways
    _appendSublineMappings(_mergerList.back()->getAllSublineMappings());
  }

  try
  {
    // split the ways in such a way that the mappings are updated appropriately.
    _splitAllWays(map, replaced, _allSublineMappings);

    // apply merge operations on the split ways.
    foreach (WayMatchStringMergerPtr merger, _mergerList)
    {
      _applyMerger(map, merger);
    }
  }
  catch (NeedsReviewException& e)
  {
    set<ElementId> reviews;

    foreach (WayMatchStringMerger::SublineMappingPtr mapping, _allSublineMappings)
    {
      if (mapping->getNewWay2())
      {
        reviews.insert(mapping->getNewWay2()->getElementId());
      }
      else
      {
        reviews.insert(mapping->getStart2().getWay()->getElementId());
      }

      if (mapping->newWay1)
      {
        reviews.insert(mapping->newWay1->getElementId());
      }
      else
      {
        reviews.insert(mapping->getStart1().getWay()->getElementId());
      }
    }

    ReviewMarker::mark(map, reviews, e.getWhat(), HighwayMatch::getHighwayMatchName());
  }
}

void PartialNetworkMerger::_processStubMatch(const OsmMapPtr& map,
  vector<pair<ElementId, ElementId> > &/*replaced*/, ConstEdgeMatchPtr edgeMatch) const
{
  if (edgeMatch->getString1()->isStub())
  {
    // If the feature we're merging into is a stub, then just delete the secondary feature.
    // Attributes may be lost, but there isn't really anywhere to put them.
    foreach (ConstElementPtr e, edgeMatch->getString2()->getMembers())
    {
      RecursiveElementRemover(mapEid(e->getElementId())).apply(map);
    }
  }
  else if (edgeMatch->getString2()->isStub())
  {
    // if the feature we're merging is a stub, then things get a little more complicated. So far
    // our best option is to disconnect the intersection that is being merged. Then the edges should
    // be merged for us properly as long as all the ways have matches. If they don't have matches
    // we've got a problem and they should be reviewed. Possibly identify these situations in the
    // match creator?
    /// @todo add more logic in the match creator that handles this in a more elegant way.

    set<ElementId> eids;

    foreach (ConstElementPtr e, edgeMatch->getString2()->getMembers())
    {
      eids.insert(mapEid(e->getElementId()));
    }
    foreach (ConstElementPtr e, edgeMatch->getString1()->getMembers())
    {
      eids.insert(mapEid(e->getElementId()));
    }

    ReviewMarker().mark(map, eids, "Ambiguous intersection match. Possible dogleg? Very short "
      "segment? Please verify merge and fix as needed.", HighwayMatch::getHighwayMatchName());
  }
  else
  {
    throw IllegalArgumentException("Invalid parameter. Expected a stub.");
  }
}

void PartialNetworkMerger::replace(ElementId oldEid, ElementId newEid)
{
  MergerBase::replace(oldEid, newEid);
  _substitions[oldEid] = newEid;
}

void PartialNetworkMerger::_splitAllWays(const OsmMapPtr& map,
  vector< pair<ElementId, ElementId> >& replaced,
  QList<WayMatchStringMerger::SublineMappingPtr> mappings) const
{
  // refactor WayMatchStringMerger split methods into a new class.
  WayMatchStringSplitter().applySplits(map, replaced, mappings);
}

QString PartialNetworkMerger::toString() const
{
  QString s = hoot::toString(getPairs());
  return QString("PartialNetworkMerger %1").arg(s);
}

}
