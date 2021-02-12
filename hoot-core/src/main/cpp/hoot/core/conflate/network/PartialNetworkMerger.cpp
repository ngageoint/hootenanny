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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#include "PartialNetworkMerger.h"

// hoot
#include <hoot/core/algorithms/linearreference/NaiveWayMatchStringMapping.h>
#include <hoot/core/algorithms/WayMatchStringSplitter.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/io/OsmJsonWriter.h>
#include <hoot/core/elements/NodeToWayMap.h>
#include <hoot/core/conflate/highway/HighwayMatch.h>
#include <hoot/core/conflate/review/ReviewMarker.h>
#include <hoot/core/ops/RecursiveElementRemover.h>
#include <hoot/core/ops/ReplaceElementOp.h>
#include <hoot/core/schema/TagMergerFactory.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/visitors/NodesVisitor.h>
#include <hoot/core/util/Factory.h>

using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(Merger, PartialNetworkMerger)

PartialNetworkMerger::PartialNetworkMerger() :
MergerBase(),
_needsReview(false)
{
}

PartialNetworkMerger::PartialNetworkMerger(const set<pair<ElementId, ElementId>>& pairs,
  QSet<ConstEdgeMatchPtr> edgeMatches, ConstNetworkDetailsPtr details) :
  _pairs(pairs),
  _edgeMatches(edgeMatches),
  _details(details)
{
  assert(_pairs.size() >= 1);
}

void PartialNetworkMerger::_appendSublineMappings(
  QList<WayMatchStringMerger::SublineMappingPtr> mappings) const
{
  LOG_TRACE("Appending subline mappings...");
  foreach (WayMatchStringMerger::SublineMappingPtr sm, mappings)
  {
    foreach (WayMatchStringMerger::SublineMappingPtr other, _allSublineMappings)
    {
      if (other->getSubline1().overlaps(sm->getSubline1()))
      {
        // We punt on this review, so no point in updating _needsReview.
        throw NeedsReviewException("Internal Error: Overlapping partial matches were found. To "
          "resolve please make the logical conflation operations manually. Please report this as "
          "an error and include sample data to recreate.");
      }
    }
    _allSublineMappings.append(sm);
  }
  LOG_VART(_allSublineMappings.size());
}

void PartialNetworkMerger::apply(const OsmMapPtr& map,
                                 vector<pair<ElementId, ElementId>>& replaced)
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
  LOG_DEBUG("Applying PartialNetworkMerger...");

  // we changed the sublines so we must update the indices.
  merger->updateSublineMapping();

  WayStringPtr str2 = merger->getMapping()->getWayString2();

  merger->mergeTags();
  // set the status on all keeper ways to conflated.
  merger->setKeeperStatus(Status::Conflated);
  ConfigOptions conf;
  if (conf.getWriterIncludeDebugTags() && conf.getWriterIncludeMatchedByTag())
  {
    Tags tagsToAdd(MetadataTags::HootMatchedBy(), HighwayMatch::MATCH_NAME);
    merger->addKeeperTags(tagsToAdd);
  }

  // go through all the nodes in the scrap
  QList<ConstNodePtr> scrapNodeList;
  NodesVisitor extractVisitor(scrapNodeList);
  str2->visitRo(*map, extractVisitor);
  std::shared_ptr<NodeToWayMap> n2w = map->getIndex().getNodeToWayMap();
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

  // TODO: this will need to replace one scrap with possibly multiple keeper elements
  // - think about the case when the way is part of an interstate or bus relation
  // remove the duplicate element.
  merger->replaceScraps();
}

WayMatchStringMergerPtr PartialNetworkMerger::_createMatchStringMerger(const OsmMapPtr& map,
  vector<pair<ElementId, ElementId>>& replaced, ConstEdgeMatchPtr edgeMatch) const
{
  // convert the EdgeStrings into WaySublineStrings
  WayStringPtr str1;
  WayStringPtr str2;
  try
  {
    str1 = _details->toWayString(edgeMatch->getString1(), *this);
    str2 = _details->toWayString(edgeMatch->getString2(), *this);
  }
  catch (const IllegalArgumentException& /*e*/)
  {
    return WayMatchStringMergerPtr();
  }
  LOG_VART(str1);
  LOG_VART(str2);

  WayMatchStringMappingPtr mapping(new NaiveWayMatchStringMapping(str1, str2));
  LOG_VART(mapping->toString());

  /******************
   * At the beginning, the merger should identify where the primary way should be broken into bits
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
    return _substitions[oldEid];
  }
  return oldEid;
}

void PartialNetworkMerger::_processFullMatch(const OsmMapPtr& map,
  vector<pair<ElementId, ElementId>>& replaced)
{
  LOG_TRACE("Processing full match...");

  foreach (ConstEdgeMatchPtr e, _edgeMatches)
  {
    if (e->getString1()->isStub() || e->getString2()->isStub())
    {
      LOG_VART(_edgeMatches);
      LOG_VART(e);
      throw IllegalArgumentException("Didn't expect a stub in a full match.");
    }
  }
  LOG_VART(_edgeMatches);

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

  // calculate all the mappings and split points for all matches.
  LOG_TRACE("Calculating mappings and split points for matches...");
  foreach (ConstEdgeMatchPtr em, _edgeMatches)
  {
    WayMatchStringMergerPtr merger = _createMatchStringMerger(map, replaced, em);
    if (merger)
    {
      _mergerList.append(merger);
      // Put all split points and mappings into a single structure that can be used to split ways
      _appendSublineMappings(_mergerList.back()->getAllSublineMappings());
    }
  }
  LOG_VART(_mergerList.size());

  try
  {
    // split the ways in such a way that the mappings are updated appropriately.
    LOG_TRACE("Applying way splits...");
    WayMatchStringSplitter splitter;
    splitter.applySplits(map, replaced, _allSublineMappings);

    // apply merge operations on the split ways.
    LOG_TRACE("Merging split ways...");
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

      if (mapping->getNewWay1())
      {
        reviews.insert(mapping->getNewWay1()->getElementId());
      }
      else
      {
        reviews.insert(mapping->getStart1().getWay()->getElementId());
      }
    }

    ReviewMarker().mark(map, reviews, e.getWhat(), HighwayMatch::getHighwayMatchName());
    _needsReview = true;
  }
}

void PartialNetworkMerger::_processStubMatch(const OsmMapPtr& map,
  vector<pair<ElementId, ElementId>>& /*replaced*/, ConstEdgeMatchPtr edgeMatch)
{
  LOG_TRACE("Processing stub match...");
  LOG_VART(edgeMatch);

  if (edgeMatch->getString1()->isStub())
  {
    LOG_TRACE("Removing secondary features...");
    LOG_VART(edgeMatch->getString2()->getMembers());

    // If the feature we're merging into is a stub, then just delete the secondary feature.
    // Attributes may be lost, but there isn't really anywhere to put them.
    foreach (ConstElementPtr e, edgeMatch->getString2()->getMembers())
    {
      RecursiveElementRemover(mapEid(e->getElementId())).apply(map);
    }
  }
  else if (edgeMatch->getString2()->isStub())
  {
    LOG_VART(edgeMatch->getString1()->getMembers().size());
    LOG_VART(edgeMatch->getString1()->getMembers());
    LOG_VART(edgeMatch->getString2()->getMembers().size());
    LOG_VART(edgeMatch->getString2()->getMembers());

    // If the feature we're merging is a stub, then things get a little more complicated. So far
    // our best option is to disconnect the intersection that is being merged. Then the edges should
    // be merged for us properly as long as all the ways have matches. If they don't have matches
    // we've got a problem and they should be reviewed. Possibly identify these situations in the
    // match creator?

    // TODO: add more logic in the match creator that handles this in a more elegant way.

    set<ElementId> eids;
    foreach (ConstElementPtr e, edgeMatch->getString2()->getMembers())
    {
      LOG_VART(e);
      eids.insert(mapEid(e->getElementId()));
    }
    foreach (ConstElementPtr e, edgeMatch->getString1()->getMembers())
    {
      LOG_VART(e);
      eids.insert(mapEid(e->getElementId()));
    }
    ReviewMarker().mark(
      map, eids,
      "Ambiguous intersection match. Possible dogleg? Very short segment? Please verify merge "
      "and fix as needed.", HighwayMatch::getHighwayMatchName());
    _needsReview = true;
  }
  else
  {
    throw IllegalArgumentException("Invalid parameter. Expected a stub.");
  }
}

void PartialNetworkMerger::replace(ElementId oldEid, ElementId newEid)
{
  LOG_TRACE("Replacing " << oldEid << " with " << newEid);
  MergerBase::replace(oldEid, newEid);
  _substitions[oldEid] = newEid;
}

QString PartialNetworkMerger::toString() const
{
  QString s = hoot::toString(_getPairs());
  return QString("PartialNetworkMerger %1").arg(s);
}

}
