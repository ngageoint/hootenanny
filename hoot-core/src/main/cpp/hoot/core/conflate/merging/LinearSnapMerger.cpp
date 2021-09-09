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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#include "LinearSnapMerger.h"

// geos
#include <geos/geom/CoordinateSequence.h>
#include <geos/geom/CoordinateSequenceFactory.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/LineString.h>

// hoot
#include <hoot/core/algorithms/DirectionFinder.h>
#include <hoot/core/algorithms/linearreference/WaySublineCollection.h>
#include <hoot/core/algorithms/splitter/MultiLineStringSplitter.h>
#include <hoot/core/algorithms/subline-matching/SublineStringMatcher.h>
#include <hoot/core/conflate/highway/HighwayMatch.h>
#include <hoot/core/conflate/merging/WayNodeCopier.h>
#include <hoot/core/criterion/NotCriterion.h>
#include <hoot/core/criterion/NoInformationCriterion.h>
#include <hoot/core/criterion/OneWayCriterion.h>
#include <hoot/core/elements/ElementComparer.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/elements/NodeToWayMap.h>
#include <hoot/core/elements/RelationMemberUtils.h>
#include <hoot/core/geometry/ElementToGeometryConverter.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/ops/IdSwapOp.h>
#include <hoot/core/ops/ReplaceElementOp.h>
#include <hoot/core/ops/RemoveElementByEid.h>
#include <hoot/core/ops/RemoveReviewsByEidOp.h>
#include <hoot/core/ops/RelationMemberSwapper.h>
#include <hoot/core/schema/TagMergerFactory.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Validate.h>
#include <hoot/core/visitors/ElementOsmMapVisitor.h>
#include <hoot/core/visitors/WaysVisitor.h>

using namespace geos::geom;
using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(Merger, LinearSnapMerger)

int LinearSnapMerger::logWarnCount = 0;

LinearSnapMerger::LinearSnapMerger() :
LinearMergerAbstract(),
_removeTagsFromWayMembers(true),
_markAddedMultilineStringRelations
  (ConfigOptions().getConflateMarkMergeCreatedMultilinestringRelations())
{
}

LinearSnapMerger::LinearSnapMerger(
  const set<pair<ElementId, ElementId>>& pairs,
  const std::shared_ptr<SublineStringMatcher>& sublineMatcher) :
LinearMergerAbstract(pairs, sublineMatcher),
_removeTagsFromWayMembers(true),
_markAddedMultilineStringRelations
  (ConfigOptions().getConflateMarkMergeCreatedMultilinestringRelations())
{
}

bool LinearSnapMerger::_mergePair(
  ElementId eid1, ElementId eid2, vector<pair<ElementId, ElementId>>& replaced)
{
  // An element null check is performed by this call.
  if (LinearMergerAbstract::_mergePair(eid1, eid2, replaced))
  {
    return true;
  }

  LOG_TRACE("Snap merging " << eid1 << " and " << eid2 << "...");

  ElementPtr e1 = _map->getElement(eid1);
  LOG_VART(e1->getStatus());
  ElementPtr e2 = _map->getElement(eid2);
  LOG_VART(e2->getStatus());

  // If the two elements being merged are identical, then there's no point of going through
  // splitting and trying to match sections of them together. Just set the match equal to the
  // first element.
  const bool areIdentical = _checkForIdenticalElements(e1, e2);
  if (areIdentical)
  {
    return false;
  }

  // Match the elements and split them into sublines.
  WaySublineMatchString match;
  try
  {
    match = _matchSubline(e1, e2);
  }
  // TODO: These exceptions can involve other types of reviews as well (river, railway, etc). So,
  // don't just use the highway match name.
  catch (const NeedsReviewException& e)
  {
    LOG_VART(e.getWhat());
    _markNeedsReview(e1, e2, e.getWhat(), HighwayMatch::getHighwayMatchName());
    return true;
  }
  if (!match.isValid())
  {
    LOG_TRACE("Complex conflict causes an empty match");
    _markNeedsReview(
      e1, e2, "Complex conflict causes an empty match", HighwayMatch::getHighwayMatchName());
    if (ConfigOptions().getDebugMapsWriteDetailed())
    {
      OsmMapWriterFactory::writeDebugMap(
        _map, className(), "after-complex-conflict-" + _eidLogString);
    }
    return true;
  }
  LOG_VART(match);

  ElementPtr e1Match;
  ElementPtr e2Match;
  ElementPtr scraps1;
  ElementPtr scraps2;

  // Split the first element and don't reverse any of the geometries.
  _splitElement(
    match.getSublineString1(), match.getReverseVector1(), replaced, e1, e1Match, scraps1);

  // Split the second element and reverse any geometries to make the matches work.
  _splitElement(
    match.getSublineString2(), match.getReverseVector2(), replaced, e2, e2Match, scraps2);

  LOG_VART(e1Match->getElementId());
  LOG_VART(e2Match->getElementId());
  if (scraps1)
  {
    LOG_VART(scraps1->getElementId());
  }
  if (scraps2)
  {
    LOG_VART(scraps2->getElementId());
  }

  // Remove any ways that directly connect from e1Match to e2Match.
  _removeSpans(e1Match, e2Match);

  // Snap the ends together.
  _snapEnds(e2Match, e1Match);

  //if (_mergeTags)
  //{
    // Merge the attributes appropriately.
    _mergeElementTags(e1->getTags(), e2->getTags(), e1Match);
  //}
\
  // Do some ID handling for ways.
  bool swapWayIds = false;
  if (e1Match->getElementType() == ElementType::Way && e1->getElementType() == ElementType::Way &&
      e2->getElementType() == ElementType::Way)
  {
    WayPtr w1 = std::dynamic_pointer_cast<Way>(e1);
    WayPtr w2 = std::dynamic_pointer_cast<Way>(e2);
    WayPtr wMatch = std::dynamic_pointer_cast<Way>(e1Match);
    LOG_VART(w1->getId());
    LOG_VART(w2->getId());
    LOG_VART(wMatch->getId());

    //  Keep the original ID from e1 for e1Match
    swapWayIds = true;

    // Handle necessary ID and parent ID updates.
    _manageElementIds(w1, w2, wMatch, scraps1, scraps2);

    // Reverse the way if w2 is one way and w1 isn't the similar direction as w2.
    if (OneWayCriterion().isSatisfied(w2) &&
        !DirectionFinder::isSimilarDirection(_map->shared_from_this(), w1, w2))
    {
      LOG_TRACE("Reversing " << wMatch->getElementId() << "...");
      wMatch->reverseOrder();
    }
  }

  // Deal with the old way that was split and snapped.
  _handleSplitWay(e1, scraps1, e1Match, swapWayIds);

  // If there is something left to review against,
  if (scraps2)
  {
    // ...swap the elements with the scraps.
    _swapSecondaryElementWithScraps(eid2, e2Match, scraps2);
  }
  else
  {
    // Otherwise, drop the element and any reviews its in.
    _dropSecondaryElements(eid1, e1Match->getElementId(), eid2, e2Match->getElementId());
    // Do some additional processing on the way that was split and snapped.
    _handleSplitWay(eid1, scraps1, e1Match, replaced);
  }

  if (_markAddedMultilineStringRelations)
  {
    // This is a sanity check to make sure elements other than relations aren't marked incorrectly.
    _validateMarkedMultilineStringRelations(e1Match);
    _validateMarkedMultilineStringRelations(scraps1);
    _validateMarkedMultilineStringRelations(e2Match);
    _validateMarkedMultilineStringRelations(scraps2);
  }

  LOG_VART(replaced);

  return false;
}

bool LinearSnapMerger::_checkForIdenticalElements(const ElementPtr& e1, const ElementPtr& e2) const
{
  LOG_TRACE(
    "Checking " << e1->getElementId() << " and " << e2->getElementId() <<
    " to see if they are identical...");

  ElementComparer elementComparer;
  elementComparer.setIgnoreElementId(true);
  elementComparer.setOsmMap(_map.get());
  if (elementComparer.isSame(e1, e2))
  {
    ElementPtr keep = e1;
    ElementPtr remove = e2;
    LOG_VART(keep->getElementId());
    LOG_VART(remove->getElementId());
    //  Favor positive IDs, swap the keeper when e2 has a positive ID and e1 doesn't.
    if (e2->getId() > 0 && e1->getId() < 0)
    {
      keep = e2;
      remove = e1;
    }
    LOG_TRACE(
      "Merging identical elements: " << keep->getElementId() << " and " << remove->getElementId() <<
      "...");
    keep->setStatus(Status::Conflated);
    // remove the second element and any reviews that contain the element
    RemoveReviewsByEidOp(remove->getElementId(), true, true).apply(_map);

    if (ConfigOptions().getDebugMapsWriteDetailed())
    {
      OsmMapWriterFactory::writeDebugMap(
       _map, className(), "merged-identical-elements" + _eidLogString);
    }

    return true;
  }
  return false;
}

WaySublineMatchString LinearSnapMerger::_matchSubline(ElementPtr e1, ElementPtr e2)
{
  LOG_TRACE(
    "Matching sublines for: " << e1->getElementId() << " and " << e2->getElementId() << "...");
  // Some attempts were made to use cached subline matches pased in from LinearSnapMergerJs for
  // performance reasons, but the results were unstable. See branch 3969b.
  return _sublineMatcher->findMatch(_map, e1, e2);
}

void LinearSnapMerger::_mergeElementTags(
  const Tags& e1Tags, const Tags& e2Tags, const ElementPtr& e1Match) const
{
  LOG_TRACE("Merging tags...");

  Tags newTags = TagMergerFactory::mergeTags(e1Tags, e2Tags, ElementType::Way);
  e1Match->setTags(newTags);
  e1Match->setStatus(Status::Conflated);
  ConfigOptions conf;
  if (conf.getWriterIncludeDebugTags() && conf.getWriterIncludeMatchedByTag())
  {
    e1Match->setTag(MetadataTags::HootMatchedBy(), _matchedBy);
  }
  if (ConfigOptions().getDebugMapsWriteDetailed())
  {
    OsmMapWriterFactory::writeDebugMap(_map, className(), "after-tag-merging" + _eidLogString);
  }
}

void LinearSnapMerger::_snapEnds(ElementPtr snapee, ElementPtr snapTo) const
{
  // TODO: get rid of this and replace with visitors/WaysVisitor
  class WaysVisitor : public ElementOsmMapVisitor
  {
  public:

    WaysVisitor(vector<WayPtr>& w) : _w(w) { }

    static vector<WayPtr> getWays(const OsmMapPtr& map, const ElementPtr& e)
    {
      vector<WayPtr> result;
      if (e)
      {
        if (e->getElementType() == ElementType::Way)
        {
          result.push_back(std::dynamic_pointer_cast<Way>(e));
        }
        else
        {
          WaysVisitor v(result);
          v.setOsmMap(map.get());
          e->visitRo(*map, v);
        }
      }
      return result;
    }

    QString getDescription() const override { return ""; }
    QString getName() const override { return ""; }
    QString getClassName() const override { return ""; }

    void visit(const std::shared_ptr<Element>& e) override
    {
      if (e->getElementType() == ElementType::Way)
      {
        WayPtr w = std::dynamic_pointer_cast<Way>(e);
        _w.push_back(w);
      }
    }

    vector<WayPtr>& _w;
  };

  LOG_TRACE(
    "Snapping end of " << snapee->getElementId() << " to " << snapTo->getElementId() << "...");

  // Convert all the elements into arrays of ways. If it is a way already, then it creates a vector
  // of size 1 with that way. If they're relations w/ complex multilinestrings, then you'll get all
  // the component ways.
  vector<WayPtr> snapeeWays = WaysVisitor::getWays(_map, snapee);
  vector<WayPtr> snapToWays = WaysVisitor::getWays(_map, snapTo);
  LOG_VART(snapeeWays.size());
  LOG_VART(snapToWays.size());
  assert(snapToWays.size() == snapeeWays.size());

  std::shared_ptr<NodeToWayMap> n2w = _map->getIndex().getNodeToWayMap();

  for (size_t i = 0; i < snapeeWays.size(); i++)
  {
    // Find all the ways that connect to the beginning or end of w1. There shouldn't be any that
    // connect in the middle.
    set<long> wids = n2w->getWaysByNode(snapeeWays[i]->getNodeIds()[0]);
    const set<long>& wids2 = n2w->getWaysByNode(snapeeWays[i]->getLastNodeId());
    wids.insert(wids2.begin(), wids2.end());

    for (set<long>::const_iterator it = wids.begin(); it != wids.end(); ++it)
    {
      if (snapeeWays[i]->getId() != *it)
      {
        WayPtr w = _map->getWay(*it);
        if (w->getStatus() == Status::Unknown2)
        {
          _snapEnds(_map->getWay(*it), snapeeWays[i], snapToWays[i]);
        }
      }
    }
    _snapEnds(snapeeWays[i], snapeeWays[i], snapToWays[i]);
  }

  if (ConfigOptions().getDebugMapsWriteDetailed())
  {
    OsmMapWriterFactory::writeDebugMap(_map, className(), "after-snap-ends" + _eidLogString);
  }
}

void LinearSnapMerger::_snapEnds(WayPtr snapee, WayPtr middle, WayPtr snapTo) const
{
  LOG_TRACE("Snapping ends...");

  NodePtr replacedNode = _map->getNode(middle->getNodeId(0));
  NodePtr replacementNode = _map->getNode(snapTo->getNodeId(0));
  _snapEnd(snapee, replacedNode, replacementNode);

  replacedNode = _map->getNode(middle->getLastNodeId());
  replacementNode = _map->getNode(snapTo->getLastNodeId());
  _snapEnd(snapee, replacedNode, replacementNode);
}

void LinearSnapMerger::_snapEnd(
  WayPtr snapee, NodePtr replacedNode, NodePtr replacementNode) const
{
  LOG_TRACE(
    "Replacing " << replacedNode->getElementId() << " with " << replacementNode->getElementId() <<
    " on " << snapee->getElementId() << "...");
  // replace the node
  snapee->replaceNode(replacedNode->getId(), replacementNode->getId());

  // TODO: This is a hack for now, as one single test (highway-search-radius-1) has highway=road
  // which makes no sense on a node. Want to think about it a little more before deciding to modify
  // the test's input data.
  const QStringList nodeKvpExcludeList("highway=road");
  // If the node we just replaced has info and the one we're replacing it with does not, let's copy
  // that info over to the replacement.
  if (/*_mergeTags &&*/ replacedNode->getTags().hasInformationTag() &&
      !replacementNode->getTags().hasInformationTag() &&
      !replacedNode->getTags().hasAnyKvp(nodeKvpExcludeList))
  {
    replacementNode->setTags(
      TagMergerFactory::mergeTags(
        replacementNode->getTags(), replacedNode->getTags(), ElementType::Node));
  }
  // Let's also preserve relation membership.
  RelationMemberSwapper::swap(
    replacedNode->getElementId(), replacementNode->getElementId(), _map, false);
}

void LinearSnapMerger::_splitElement(
  const WaySublineCollection& s, const vector<bool>& reverse,
  vector<pair<ElementId, ElementId>>& replaced, const ConstElementPtr& splitee, ElementPtr& match,
  ElementPtr& scrap) const
{
  LOG_TRACE("Splitting " << splitee->getElementId() << "...");
  LOG_VART(s);

  MultiLineStringSplitter(_markAddedMultilineStringRelations).split(_map, s, reverse, match, scrap);

  // The subline string split should always result in a match section.
  assert(match);
  LOG_VART(match->getElementId());
  if (scrap)
  {
    LOG_VART(scrap->getElementId());
  }

  vector<ConstWayPtr> waysV = WaysVisitor::extractWays(_map, splitee);
  set<ConstWayPtr, WayPtrCompare> ways;
  ways.insert(waysV.begin(), waysV.end());
  LOG_TRACE("Extracted ways: " << ways);

  // Remove all the ways that are part of the subline. This leaves us with a list of ways that
  // aren't going to be modified.
  for (size_t i = 0; i < s.getSublines().size(); i++)
  {
    ways.erase(s.getSublines()[i].getWay());
  }
  LOG_TRACE("Non-modifiable extracted ways: " << ways);

  // If there are ways that aren't part of the way subline string,
  if (!ways.empty())
  {
    // ...add the ways to the scrap relation.
    RelationPtr r;
    if (!scrap || scrap->getElementType() == ElementType::Way)
    {
      r =
        std::make_shared<Relation>(
          splitee->getStatus(), _map->createNextRelationId(), splitee->getCircularError(),
          MetadataTags::RelationMultilineString());
      if (scrap)
      {
        r->addElement("", scrap);
      }
      if (_markAddedMultilineStringRelations)
      {
        r->getTags().set(MetadataTags::HootMultilineString(), "yes");
      }
      LOG_TRACE("Created scrap relation: " << r->getElementId());
      scrap = r;
      _map->addElement(r);
    }
    else
    {
      r = std::dynamic_pointer_cast<Relation>(scrap);
    }

    for (set<ConstWayPtr, WayPtrCompare>::iterator it = ways.begin(); it != ways.end(); ++it)
    {
      r->addElement("", *it);
    }
    LOG_VART(r->getElementId());
  }

  //if (_mergeTags)
  //{
    match->setTags(splitee->getTags());
  //}
  match->setCircularError(splitee->getCircularError());
  match->setStatus(splitee->getStatus());

  if (scrap)
  {
    LOG_VART(scrap->getElementId());

    /*
     * In this example we have a foot path that goes on top of a wall (x) that is being matched with
     * another path (o).
     *
     *      footway relation
     *       /         \
     *     x---x------wall------x
     * o-------------o
     *
     * The expected output:
     *
     *              footway relation
     *                     |
     *    -x---x-wall-x---wall--x
     * o-/   \    /
     *        \  /
     *  conflated path relation
     *
     * x is the splitee in this case
     */
    if (splitee->getElementType() == ElementType::Relation &&
        scrap->getTags().getInformationCount() > 0 && scrap->getElementType() == ElementType::Way)
    {
      // Create a new relation to contain this single way (footway relation).
      RelationPtr r =
        std::make_shared<Relation>(
          splitee->getStatus(), _map->createNextRelationId(), splitee->getCircularError(),
          MetadataTags::RelationMultilineString());
      r->addElement("", scrap->getElementId());
      if (_markAddedMultilineStringRelations)
      {
        r->getTags().set(MetadataTags::HootMultilineString(), "yes");
      }
      scrap = r;
      LOG_TRACE("Created multilinestring relation for footway: " << r->getElementId());
      _map->addElement(r);
    }
    /*
     * In this example we have a road that is split into two roads and a new relation is created for
     * the split pieces. We need to make sure the tags get moved around properly.
     *
     * x-------w1--------x
     *      o--w2---o
     *
     * The expected output:
     *
     *     w1 relation
     *    /           \
     * x----x-w1;w2-x----x
     */
    else if (_removeTagsFromWayMembers && splitee->getElementType() == ElementType::Way &&
             scrap->getElementType() == ElementType::Relation)
    {
      RelationPtr r = std::dynamic_pointer_cast<Relation>(scrap);
      // Make sure none of the child ways have tags.
      for (size_t i = 0; i < r->getMembers().size(); i++)
      {
        LOG_TRACE(
          "multilinestring: removing member tags from: " << r->getMembers()[i].getElementId() <<
          " belonging to : " << r->getElementId() << "...");
        _map->getElement(r->getMembers()[i].getElementId())->getTags().clear();
      }
    }

    bool multiLineStringAdded = false;
    if (_markAddedMultilineStringRelations &&
        (match->getTags().contains(MetadataTags::HootMultilineString()) ||
         scrap->getTags().contains(MetadataTags::HootMultilineString())))
    {
      multiLineStringAdded = true;
    }

    //if (_mergeTags)
    //{
      // Make sure the tags are still legit on the scrap.
      scrap->setTags(splitee->getTags());
    //}
    // With the merging switching between split ways and relations, it gets a little hard to keep
    // track of where this tag is needed, so one final check here to make sure it gets added
    // correctly.
    if (_markAddedMultilineStringRelations && multiLineStringAdded &&
        scrap->getElementType() == ElementType::Relation)
    {
      scrap->getTags().set(MetadataTags::HootMultilineString(), "yes");
    }

    // Add the scraps way to whatever relations the way that was split is in.
    std::vector<RelationPtr> relationsContainingSplitWay =
      RelationMemberUtils::getContainingRelations(splitee->getElementId(), _map);
    for (std::vector<RelationPtr>::const_iterator itr = relationsContainingSplitWay.begin();
         itr != relationsContainingSplitWay.end(); ++itr)
    {
      RelationPtr relation = *itr;
      if (relation)
      {
        relation->addElement(relation->getRole(splitee->getElementId()), scrap->getElementId());
      }
    }

    replaced.emplace_back(splitee->getElementId(), scrap->getElementId());
    LOG_VART(replaced);
  }

  if (ConfigOptions().getDebugMapsWriteDetailed())
  {
    OsmMapWriterFactory::writeDebugMap(
      _map, className(),
      "after-way-split-" + splitee->getElementId().toString() + "-" + _eidLogString);
  }
}

void LinearSnapMerger::_manageElementIds(
  const WayPtr& w1, const WayPtr& w2, const WayPtr& wMatch, const ElementPtr& scraps1,
  const ElementPtr& scraps2) const
{
  LOG_TRACE("Managing element IDs...");

  const long pid = Way::getPid(w1, w2);
  // If the the parent IDs for both matched ways are empty, we won't write the empty ID to the
  // match portion to possibly avoid overwriting a pre-existing valid parent ID.
  if (pid != WayData::PID_EMPTY)
  {
    wMatch->setPid(pid);
    LOG_TRACE("Set PID: " << pid << " on: " << wMatch->getElementId() << " (e1Match).");
  }

  if (scraps1)
  {
    _handleScrapsIds(scraps1, w1);
  }

  if (scraps2)
  {
    _handleScrapsIds(scraps2, w2);
  }
}

void LinearSnapMerger::_handleScrapsIds(const ElementPtr& scraps, const WayPtr& way) const
{
  LOG_TRACE(
    "Handling scrap IDs for scraps: " << scraps->getElementId() << " and " <<
    way->getElementId() << "...");

  if (scraps->getElementType() == ElementType::Way)
  {
    std::dynamic_pointer_cast<Way>(scraps)->setPid(way->getPid());
    LOG_TRACE(
      "Set PID: " << way->getPid() << " on: " << scraps->getElementId() << " (scraps).");
  }
  else if (scraps->getElementType() == ElementType::Relation)
  {
    RelationPtr r = std::dynamic_pointer_cast<Relation>(scraps);
    for (size_t i = 0; i < r->getMembers().size(); ++i)
    {
      ElementId eid = r->getMembers()[i].getElementId();
      if (eid.getType() == ElementType::Way)
      {
        _map->getWay(eid)->setPid(way->getPid());
        LOG_TRACE(
          "Set PID: " << way->getPid() << " on: " << eid << " (scraps).");
      }
    }
  }
}

void LinearSnapMerger::_updateScrapParent(long id, const ElementPtr& scrap)
{
  if (!scrap)
    return;

  LOG_TRACE(
    "Updating scrap parent: " << scrap->getElementId() << " with parent ID: " << id << "...");

  if (scrap->getElementType() == ElementType::Way)
    std::dynamic_pointer_cast<Way>(scrap)->setPid(id);
  else if (scrap->getElementType() == ElementType::Relation)
  {
    RelationPtr relation = std::dynamic_pointer_cast<Relation>(scrap);
    const vector<RelationData::Entry>& members = relation->getMembers();
    //  Iterate all of the members and update the parent id recursively.
    for (size_t i = 0; i < members.size(); ++i)
      _updateScrapParent(id, _map->getElement(members[i].getElementId()));
  }
}

void LinearSnapMerger::_dropSecondaryElements(
  const ElementId& eid1, const ElementId& eidMatch1, const ElementId& eid2,
  const ElementId& eidMatch2) const
{
  LOG_TRACE("Removing e2 match: " << eidMatch2 << " and e2: " << eid2 << "...");

  // Add any informational nodes from the ways being replaced to the merged output before deleting
  // them.
  WayNodeCopier nodeCopier;
  nodeCopier.setOsmMap(_map.get());
  nodeCopier.addCriterion(
    std::make_shared<NotCriterion>(std::make_shared<NoInformationCriterion>()));

  if (eid1.getType() == ElementType::Way && eidMatch2.getType() == ElementType::Way)
  {
    LOG_TRACE(
      "Copying information nodes from e2 match: " << eidMatch2 << " to e1: " << eid1 << "...");
    nodeCopier.copy(eidMatch2, eid1);
  }

  if (eidMatch1.getType() == ElementType::Way && eidMatch2.getType() == ElementType::Way)
  {
    LOG_TRACE(
      "Copying information nodes from e2 match: " << eidMatch2 << " to e1 match: " << eidMatch1 <<
      "...");
    nodeCopier.copy(eidMatch2, eidMatch1);
  }

  // Remove reviews e2Match is involved in.
  LOG_TRACE("Removing reviews " << eidMatch2 << " is involved in...");
  RemoveReviewsByEidOp(eidMatch2, true, false).apply(_map);

  // Make the way that we're keeping have membership in whatever relations the way we're removing
  // was in. I *think* this makes sense. This logic may also need to be replicated elsewhere
  // during merging.
  LOG_TRACE(
    "Swapping relation membership. Adding " << eidMatch1 << " to all relations " << eid2 <<
    " belongs in...");
  RelationMemberSwapper::swap(eid2, eidMatch1, _map, false);

  // Remove reviews e2 is involved in.
  LOG_TRACE("Removing reviews " << eid2 << " is involved...");
  RemoveReviewsByEidOp(eid2, true, false).apply(_map);

  if (ConfigOptions().getDebugMapsWriteDetailed())
  {
    OsmMapWriterFactory::writeDebugMap(
      _map, className(), "after-dropping-secondary-elements" + _eidLogString);
  }
}

void LinearSnapMerger::_swapSecondaryElementWithScraps(
  const ElementId& secElementId, const ElementPtr& matchElement, const ElementPtr& scraps) const
{
  LOG_TRACE(
    "Replacing e2 match: " << matchElement->getElementId() << " and e2: " << secElementId <<
    " with scraps2: " << scraps->getElementId() << "...");
  _map->addElement(scraps);
  ReplaceElementOp(matchElement->getElementId(), scraps->getElementId(), true).apply(_map);
  ReplaceElementOp(secElementId, scraps->getElementId(), true).apply(_map);

  if (ConfigOptions().getDebugMapsWriteDetailed())
  {
    OsmMapWriterFactory::writeDebugMap(
      _map, className(), "after-swap-secondary-elements-with-scraps" + _eidLogString);
  }
}

void LinearSnapMerger::_handleSplitWay(
  const ElementPtr& e1, const ElementPtr& scraps1, const ElementPtr& e1Match, const bool swapWayIds)
{
  LOG_TRACE("Handling split way...");

  const ElementId eid1 = e1->getElementId();
  if (e1 != e1Match && scraps1)
  {
    if (swapWayIds)
    {
      ElementId eidm1 = e1Match->getElementId();
      LOG_TRACE("Swapping e1 match ID: " << eidm1 << " with e1 ID: " << eid1 << "...");
      //  Swap the old way ID back into the match element.
      IdSwapOp(eid1, eidm1).apply(_map);
      //  Remove the old way with a new swapped out ID.
      RemoveElementByEid(eidm1).apply(_map);
      //  Add the scraps element to all the relations that the match is in.
      if (scraps1)
      {
        QList<ElementPtr> list;
        list.append(e1Match);
        list.append(scraps1);
        _map->replace(e1Match, list);
        //  Update the scraps
        _updateScrapParent(e1Match->getId(), scraps1);
      }
    }
    else if (scraps1)
    {
      LOG_TRACE("Replacing e1: " << eid1 << " with scraps1: " << scraps1->getElementId() << "...");
      ReplaceElementOp(eid1, scraps1->getElementId(), true).apply(_map);
    }
  }
  else
  {
    // Remove any reviews that contain this element. Don't remove the element itself yet. That may
    // be done later.
    LOG_TRACE("Removing reviews for e1: " << eid1 << "...");
    RemoveReviewsByEidOp(eid1, true, false).apply(_map);
  }

  if (ConfigOptions().getDebugMapsWriteDetailed())
  {
    OsmMapWriterFactory::writeDebugMap(
      _map, className(), "after-split-way-removal-1" + _eidLogString);
  }
}

void LinearSnapMerger::_handleSplitWay(
  const ElementId& eid1, const ElementPtr& scraps1, const ElementPtr& e1Match,
  std::vector<std::pair<ElementId, ElementId>>& replaced)
{
  if (!scraps1)
  {
    LOG_TRACE("Replacing " << eid1 << " with " << e1Match->getElementId() << "...");
    RemoveElementByEid(eid1).apply(_map);
    replaced.emplace_back(eid1, e1Match->getElementId());

    if (ConfigOptions().getDebugMapsWriteDetailed())
    {
      OsmMapWriterFactory::writeDebugMap(
        _map, className(), "after-split-way-removal-2" + _eidLogString);
    }
  }
}

void LinearSnapMerger::_validateMarkedMultilineStringRelations(const ElementPtr& element) const
{
  if (element && element->getElementType() != ElementType::Relation)
  {
    element->getTags().remove(MetadataTags::HootMultilineString());
  }
}

}
