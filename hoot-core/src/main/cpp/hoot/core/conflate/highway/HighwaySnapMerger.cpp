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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "HighwaySnapMerger.h"

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
#include <hoot/core/criterion/OneWayCriterion.h>
#include <hoot/core/elements/ElementConverter.h>
#include <hoot/core/elements/NodeToWayMap.h>
#include <hoot/core/elements/OsmUtils.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/ops/IdSwapOp.h>
#include <hoot/core/ops/RecursiveElementRemover.h>
#include <hoot/core/ops/ReplaceElementOp.h>
#include <hoot/core/ops/RemoveElementByEid.h>
#include <hoot/core/ops/RemoveReviewsByEidOp.h>
#include <hoot/core/schema/TagMergerFactory.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/util/Validate.h>
#include <hoot/core/visitors/ElementOsmMapVisitor.h>
#include <hoot/core/visitors/WaysVisitor.h>

// Qt
#include <QSet>

using namespace geos::geom;
using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(Merger, HighwaySnapMerger)

int HighwaySnapMerger::logWarnCount = 0;

HighwaySnapMerger::HighwaySnapMerger() :
HighwayMergerAbstract(),
_matchedBy(HighwayMatch::MATCH_NAME)
{
}

HighwaySnapMerger::HighwaySnapMerger(
  const set<pair<ElementId, ElementId>>& pairs,
  const std::shared_ptr<SublineStringMatcher>& sublineMatcher) :
_removeTagsFromWayMembers(true),
_markAddedMultilineStringRelations(false),
_sublineMatcher(sublineMatcher),
_matchedBy(HighwayMatch::MATCH_NAME)
{
  _pairs = pairs;
}

void HighwaySnapMerger::apply(const OsmMapPtr& map, vector<pair<ElementId, ElementId>>& replaced)
{
  LOG_VART(hoot::toString(_pairs));
  LOG_VART(hoot::toString(replaced));

  vector<pair<ElementId, ElementId>> pairs;
  pairs.reserve(_pairs.size());

  for (set<pair<ElementId, ElementId>>::const_iterator it = _pairs.begin(); it != _pairs.end();
       ++it)
  {
    ElementId eid1 = it->first;
    ElementId eid2 = it->second;

    if (map->containsElement(eid1) && map->containsElement(eid2))
    {
      pairs.push_back(pair<ElementId, ElementId>(eid1, eid2));
    }
    else
    {
      LOG_TRACE(
        "Map doesn't contain one or more of the following elements: " << eid1 << ", " << eid2);
    }
  }
  LOG_VART(hoot::toString(pairs));

  ShortestFirstComparator shortestFirst;
  shortestFirst.map = map;
  sort(pairs.begin(), pairs.end(), shortestFirst);
  for (vector<pair<ElementId, ElementId>>::const_iterator it = pairs.begin();
       it != pairs.end(); ++it)
  {
    ElementId eid1 = it->first;
    ElementId eid2 = it->second;

    for (size_t i = 0; i < replaced.size(); i++)
    {
      LOG_VART(eid1);
      LOG_VART(eid2);
      LOG_VART(replaced[i].first);
      LOG_VART(replaced[i].second);

      //LOG_TRACE("eid1 before replacement check: " << eid1);
      //LOG_TRACE("eid2 before replacement check: " << eid2);
      LOG_TRACE("e1 before replacement check: " << map->getElement(eid1));
      LOG_TRACE("e2 before replacement check: " << map->getElement(eid2));

      if (eid1 == replaced[i].first)
      {
        LOG_TRACE("Changing " << eid1 << " to " << replaced[i].second << "...");
        eid1 = replaced[i].second;
      }
      if (eid2 == replaced[i].first)
      {
        LOG_TRACE("Changing " << eid2 << " to " << replaced[i].second << "...");
        eid2 = replaced[i].second;
      }

      //LOG_TRACE("eid1 after replacement check: " << eid1);
      //LOG_TRACE("eid2 after replacement check: " << eid2);
      LOG_TRACE("e1 after replacement check: " << map->getElement(eid1));
      LOG_TRACE("e2 after replacement check: " << map->getElement(eid2));
    }

    _mergePair(map, eid1, eid2, replaced);
  }
}

bool HighwaySnapMerger::_directConnect(const ConstOsmMapPtr& map, WayPtr w) const
{
  std::shared_ptr<LineString> ls = ElementConverter(map).convertToLineString(w);

  CoordinateSequence* cs = GeometryFactory::getDefaultInstance()->getCoordinateSequenceFactory()->
    create(2, 2);

  cs->setAt(map->getNode(w->getNodeId(0))->toCoordinate(), 0);
  cs->setAt(map->getNode(w->getLastNodeId())->toCoordinate(), 1);

  // create a straight line and buffer it
  std::shared_ptr<LineString> straight(GeometryFactory::getDefaultInstance()->createLineString(cs));
  std::shared_ptr<Geometry> g(straight->buffer(w->getCircularError()));

  // is the way in question completely contained in the buffer?
  return g->contains(ls.get());
}

bool HighwaySnapMerger::_doesWayConnect(long node1, long node2, const ConstWayPtr& w) const
{
  return (w->getNodeId(0) == node1 && w->getLastNodeId() == node2) ||
      (w->getNodeId(0) == node2 && w->getLastNodeId() == node1);
}

bool HighwaySnapMerger::_mergePair(const OsmMapPtr& map, ElementId eid1, ElementId eid2,
  vector<pair<ElementId, ElementId>>& replaced)
{
  LOG_VART(eid1);
  LOG_VART(eid2);

  if (HighwayMergerAbstract::_mergePair(map, eid1, eid2, replaced))
  {
    return true;
  }

  OsmMapPtr result = map;

  ElementPtr e1 = result->getElement(eid1);
  ElementPtr e2 = result->getElement(eid2);
  LOG_TRACE("HighwaySnapMerger: e1\n" << OsmUtils::getElementDetailString(e1, map));
  LOG_TRACE("HighwaySnapMerger: e2\n" << OsmUtils::getElementDetailString(e2, map));

  // split w2 into sublines
  WaySublineMatchString match;
  try
  {
    match = _sublineMatcher->findMatch(result, e1, e2);
  }
  catch (const NeedsReviewException& e)
  {
    LOG_VART(e.getWhat());
    _markNeedsReview(result, e1, e2, e.getWhat(), HighwayMatch::getHighwayMatchName());
    return true;
  }
  LOG_VART(match);

  if (!match.isValid())
  {
    LOG_TRACE("Complex conflict causes an empty match");
    _markNeedsReview(result, e1, e2, "Complex conflict causes an empty match",
                     HighwayMatch::getHighwayMatchName());
    return true;
  }

  LOG_VART(match.toString());
  ElementPtr e1Match;
  ElementPtr e2Match;
  ElementPtr scraps1;
  ElementPtr scraps2;
  // split the first element and don't reverse any of the geometries.
  _splitElement(map, match.getSublineString1(), match.getReverseVector1(), replaced, e1, e1Match,
                scraps1);
  // split the second element and reverse any geometries to make the matches work.
  _splitElement(map, match.getSublineString2(), match.getReverseVector2(), replaced, e2, e2Match,
                scraps2);

  LOG_VART(e1Match->getElementId());
  if (scraps1)
  {
    LOG_VART(scraps1->getElementId());
  }
  LOG_VART(e2Match->getElementId());
  if (scraps2)
  {
    LOG_VART(scraps2->getElementId());
  }

  // remove any ways that directly connect from e1Match to e2Match
  _removeSpans(result, e1Match, e2Match);
  _snapEnds(map, e2Match, e1Match);

  // merge the attributes appropriately
  Tags newTags = TagMergerFactory::mergeTags(e1->getTags(), e2->getTags(), ElementType::Way);
  e1Match->setTags(newTags);
  e1Match->setStatus(Status::Conflated);
  ConfigOptions conf;
  if (conf.getWriterIncludeDebugTags() && conf.getWriterIncludeMatchedByTag())
  {
    e1Match->setTag(MetadataTags::HootMatchedBy(), _matchedBy);
  }

  LOG_VART(e1Match->getElementType());
  LOG_VART(e1->getElementId());
  LOG_VART(e2->getElementId());

  bool swapWayIds = false;

  if (e1Match->getElementType() == ElementType::Way)
  {
    if (e1->getElementType() == ElementType::Way && e2->getElementType() == ElementType::Way)
    {
      WayPtr w1 = std::dynamic_pointer_cast<Way>(e1);
      WayPtr w2 = std::dynamic_pointer_cast<Way>(e2);
      WayPtr wMatch = std::dynamic_pointer_cast<Way>(e1Match);
      LOG_VART(w1->getId());
      LOG_VART(w2->getId());
      LOG_VART(wMatch->getId());

      const long pid = Way::getPid(w1, w2);
      wMatch->setPid(pid);
      LOG_TRACE("Set PID: " << pid << " on: " << wMatch->getElementId() << " (e1Match).");

      //  Keep the original ID from e1 for e1Match
      swapWayIds = true;

      if (scraps1)
      {
        if (scraps1->getElementType() == ElementType::Way)
        {
          std::dynamic_pointer_cast<Way>(scraps1)->setPid(w1->getPid());
          LOG_TRACE(
            "Set PID: " << w1->getPid() << " on: " << scraps1->getElementId() << " (scraps1).");
        }
        else if (scraps1->getElementType() == ElementType::Relation)
        {
          RelationPtr r = std::dynamic_pointer_cast<Relation>(scraps1);
          for (size_t i = 0; i < r->getMembers().size(); ++i)
          {
            ElementId eid = r->getMembers()[i].getElementId();
            if (eid.getType() == ElementType::Way)
            {
              map->getWay(eid)->setPid(w1->getPid());
              LOG_TRACE(
                "Set PID: " << w1->getPid() << " on: " << eid << " (scraps1).");
            }
          }
        }
      }

      if (scraps2)
      {
        if (scraps2->getElementType() == ElementType::Way)
        {
          std::dynamic_pointer_cast<Way>(scraps2)->setPid(w2->getPid());
          LOG_TRACE(
            "Set PID: " << w2->getPid() << " on: " << scraps2->getElementId() << " (scraps2).");
        }
        else if (scraps2->getElementType() == ElementType::Relation)
        {
          RelationPtr r = std::dynamic_pointer_cast<Relation>(scraps2);
          for (size_t i = 0; i < r->getMembers().size(); ++i)
          {
            ElementId eid = r->getMembers()[i].getElementId();
            if (eid.getType() == ElementType::Way)
            {
              map->getWay(eid)->setPid(w2->getPid());
              LOG_TRACE(
                "Set PID: " << w2->getPid() << " on: " << eid << " (scraps2).");
            }
          }
        }
      }

      // Reverse the way if w2 is one way and w1 isn't the similar direction as w2
      if (OneWayCriterion().isSatisfied(w2) &&
          !DirectionFinder::isSimilarDirection(map->shared_from_this(), w1, w2))
      {
        LOG_TRACE("Reversing " << wMatch->getElementId() << "...");
        wMatch->reverseOrder();
      }
    }
  }

  if (e1Match)
  {
    LOG_VART(e1Match->getElementId());
  }
  if (scraps1)
  {
    LOG_VART(scraps1->getElementId());
  }
  if (e2Match)
  {
    LOG_VART(e2Match->getElementId());
  }
  if (scraps2)
  {
    LOG_VART(scraps2->getElementId());
  }

  if (_markAddedMultilineStringRelations)
  {
    // sanity check to make sure elements other than relations aren't marked incorrectly
    if (e1Match && e1Match->getElementType() != ElementType::Relation)
    {
      e1Match->getTags().remove(MetadataTags::HootMultilineString());
    }
    if (scraps1 && scraps1->getElementType() != ElementType::Relation)
    {
      scraps1->getTags().remove(MetadataTags::HootMultilineString());
    }
    if (e2Match && e2Match->getElementType() != ElementType::Relation)
    {
      e2Match->getTags().remove(MetadataTags::HootMultilineString());
    }
    if (scraps2 && scraps2->getElementType() != ElementType::Relation)
    {
      scraps2->getTags().remove(MetadataTags::HootMultilineString());
    }
  }

  // remove the old way that was split and snapped
  if (e1 != e1Match && scraps1)
  {
    if (swapWayIds)
    {
      ElementId eidm1 = e1Match->getElementId();
      //  Swap the old way ID back into the match element
      IdSwapOp(eid1, eidm1).apply(result);
      //  Remove the old way with a new swapped out ID
      RemoveElementByEid(eidm1).apply(result);
      //  Add the scraps element to all the relations that the match is in
      if (scraps1)
      {
        QList<ElementPtr> list;
        list.append(e1Match);
        list.append(scraps1);
        result->replace(e1Match, list);
        //  Update the scraps
        _updateScrapParent(result, e1Match->getId(), scraps1);
      }
    }
    else if (scraps1)
      ReplaceElementOp(eid1, scraps1->getElementId(), true).apply(result);
  }
  else
  {
    // remove any reviews that contain this element.
    RemoveReviewsByEidOp(eid1, true).apply(result);
  }

  // if there is something left to review against
  if (scraps2)
  {
    map->addElement(scraps2);
    ReplaceElementOp(e2Match->getElementId(), scraps2->getElementId(), true).apply(result);
    ReplaceElementOp(eid2, scraps2->getElementId(), true).apply(result);
//    _updateScrapParent(result, e2Match->getId(), scraps2);
  }
  // if there is nothing to review against, drop the reviews.
  else
  {
    RemoveReviewsByEidOp(e2Match->getElementId(), true).apply(result);
    RemoveReviewsByEidOp(eid2, true).apply(result);
  }

  if (e1Match)
  {
    LOG_VART(e1Match->getElementId());
  }
  if (scraps1)
  {
    LOG_VART(scraps1->getElementId());
  }
  if (e2Match)
  {
    LOG_VART(e2Match->getElementId());
  }
  if (scraps2)
  {
    LOG_VART(scraps2->getElementId());
  }

  LOG_VART(map->getElement(eid1));
  LOG_VART(map->getElement(eid2));

  return false;
}

void HighwaySnapMerger::_removeSpans(OsmMapPtr map, const ElementPtr& e1,
  const ElementPtr& e2) const
{
  if (e1->getElementType() != e2->getElementType())
  {
    throw InternalErrorException("Expected both elements to have the same type "
                                 "when removing spans.");
  }

  if (e1->getElementType() == ElementType::Way)
  {
    WayPtr w1 = std::dynamic_pointer_cast<Way>(e1);
    WayPtr w2 = std::dynamic_pointer_cast<Way>(e2);

    _removeSpans(map, w1, w2);
  }
  else
  {
    RelationPtr r1 = std::dynamic_pointer_cast<Relation>(e1);
    RelationPtr r2 = std::dynamic_pointer_cast<Relation>(e2);

    if (r1->getMembers().size() != r2->getMembers().size())
    {
      throw InternalErrorException("Expected both relations to have the same number of children "
                                   "when removing spans.");
    }

    for (size_t i = 0; i < r1->getMembers().size(); i++)
    {
      ElementId m1 = r1->getMembers()[i].getElementId();
      ElementId m2 = r2->getMembers()[i].getElementId();
      assert(m1.getType() == ElementType::Way && m2.getType() == ElementType::Way);
      _removeSpans(map, map->getWay(m1), map->getWay(m2));
    }
  }
}

void HighwaySnapMerger::_removeSpans(OsmMapPtr map, const WayPtr& w1, const WayPtr& w2) const
{
  LOG_TRACE("Removing spans...");

  std::shared_ptr<NodeToWayMap> n2w = map->getIndex().getNodeToWayMap();

  // find all the ways that connect to the beginning or end of w1. There shouldn't be any that
  // connect in the middle.
  set<long> wids = n2w->getWaysByNode(w1->getNodeIds()[0]);
  const set<long>& wids2 = n2w->getWaysByNode(w1->getLastNodeId());
  wids.insert(wids2.begin(), wids2.end());

  for (set<long>::const_iterator it = wids.begin(); it != wids.end(); ++it)
  {
    if (*it != w1->getId() && *it != w2->getId())
    {
      const WayPtr& w = map->getWay(*it);
      // if this isn't one of the ways we're evaluating for a connection between and it is part of
      // the data set we're conflating in
      if (w->getElementId() != w1->getElementId() &&
          w->getElementId() != w2->getElementId() &&
          w->getStatus() == Status::Unknown2)
      {
        // if this way connects w1 to w2 at the beginning or the end
        if (_doesWayConnect(w1->getNodeId(0), w2->getNodeId(0), w) ||
            _doesWayConnect(w1->getLastNodeId(), w2->getLastNodeId(), w))
        {
          // if the connection is more or less a straight shot. Don't want to worry about round
          // about connections (e.g. lollipop style).
          if (_directConnect(map, w))
          {
            // This should likely remove the way even if it is part of another relation - #2938
            RecursiveElementRemover(w->getElementId()).apply(map);
          }
        }
      }
    }
  }
}

void HighwaySnapMerger::_snapEnds(const OsmMapPtr& map, ElementPtr snapee,  ElementPtr snapTo) const
{
  // TODO: get rid of this?
  class WaysVisitor : public ElementOsmMapVisitor
  {
  public:

    WaysVisitor(vector<WayPtr>& w) : _w(w) {}

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

    virtual QString getDescription() const { return ""; }
    virtual std::string getClassName() const { return ""; }

    virtual void visit(const std::shared_ptr<Element>& e)
    {
      if (e->getElementType() == ElementType::Way)
      {
        WayPtr w = std::dynamic_pointer_cast<Way>(e);
        _w.push_back(w);
      }
    }

    vector<WayPtr>& _w;
  };

  LOG_TRACE("Snapping ends...");

  // convert all the elements into arrays of ways. If it is a way already then it creates a vector
  // of size 1 with that way, if they're relations w/ complex multilinestrings then you'll get all
  // the component ways.
  vector<WayPtr> snapeeWays = WaysVisitor::getWays(map, snapee);
  vector<WayPtr> snapToWays = WaysVisitor::getWays(map, snapTo);

  assert(snapToWays.size() == snapeeWays.size());

  std::shared_ptr<NodeToWayMap> n2w = map->getIndex().getNodeToWayMap();

  for (size_t i = 0; i < snapeeWays.size(); i++)
  {
    // find all the ways that connect to the beginning or end of w1. There shouldn't be any that
    // connect in the middle.
    set<long> wids = n2w->getWaysByNode(snapeeWays[i]->getNodeIds()[0]);
    const set<long>& wids2 = n2w->getWaysByNode(snapeeWays[i]->getLastNodeId());
    wids.insert(wids2.begin(), wids2.end());

    for (set<long>::const_iterator it = wids.begin(); it != wids.end(); ++it)
    {
      if (snapeeWays[i]->getId() != *it)
      {
        WayPtr w = map->getWay(*it);
        if (w->getStatus() == Status::Unknown2)
        {
          _snapEnds(map->getWay(*it), snapeeWays[i], snapToWays[i]);
        }
      }
    }
    _snapEnds(snapeeWays[i], snapeeWays[i], snapToWays[i]);
  }
}

void HighwaySnapMerger::_snapEnds(WayPtr snapee, WayPtr middle, WayPtr snapTo) const
{
  snapee->replaceNode(middle->getNodeId(0), snapTo->getNodeId(0));
  snapee->replaceNode(middle->getLastNodeId(), snapTo->getLastNodeId());
}

void HighwaySnapMerger::_splitElement(const OsmMapPtr& map, const WaySublineCollection& s,
  const vector<bool>& reverse, vector<pair<ElementId, ElementId>>& replaced,
  const ConstElementPtr& splitee, ElementPtr& match, ElementPtr& scrap) const
{
  LOG_VART(splitee->getElementId());

  MultiLineStringSplitter(_markAddedMultilineStringRelations).split(map, s, reverse, match, scrap);

  LOG_VART(match->getElementId());

  vector<ConstWayPtr> waysV = WaysVisitor::extractWays(map, splitee);
  set<ConstWayPtr, WayPtrCompare> ways;
  ways.insert(waysV.begin(), waysV.end());

  // remove all the ways that are part of the subline. This leaves us with a list of ways that
  // aren't going to be modified.
  for (size_t i = 0; i < s.getSublines().size(); i++)
  {
    ways.erase(s.getSublines()[i].getWay());
  }

  // the subline string split should always result in a match section.
  assert(match);

  // if there are ways that aren't part of the way subline string
  if (ways.size() > 0)
  {
    // add the ways to the scrap relation
    RelationPtr r;
    if (!scrap || scrap->getElementType() == ElementType::Way)
    {
      r.reset(new Relation(splitee->getStatus(), map->createNextRelationId(),
                           splitee->getCircularError(), MetadataTags::RelationMultilineString()));
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
      map->addElement(r);
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

  match->setTags(splitee->getTags());
  match->setCircularError(splitee->getCircularError());
  match->setStatus(splitee->getStatus());
  LOG_VART(match);

  if (scrap)
  {
    LOG_VART(scrap);

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
        scrap->getTags().getInformationCount() > 0 &&
        scrap->getElementType() == ElementType::Way)
    {
      // create a new relation to contain this single way (footway relation)
      RelationPtr r(new Relation(splitee->getStatus(), map->createNextRelationId(),
        splitee->getCircularError(), MetadataTags::RelationMultilineString()));
      r->addElement("", scrap->getElementId());
      if (_markAddedMultilineStringRelations)
      {
        r->getTags().set(MetadataTags::HootMultilineString(), "yes");
      }
      scrap = r;
      LOG_TRACE("Created multilinestring relation for footway: " << r->getElementId());
      map->addElement(r);
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
      // make sure none of the child ways have tags.
      for (size_t i = 0; i < r->getMembers().size(); i++)
      {
        LOG_TRACE(
          "multilinestring: removing member tags from: " << r->getMembers()[i].getElementId() <<
          " belonging to : " << r->getElementId() << "...");
        map->getElement(r->getMembers()[i].getElementId())->getTags().clear();
      }
    }

    bool multiLineStringAdded = false;
    if (_markAddedMultilineStringRelations &&
        (match->getTags().contains(MetadataTags::HootMultilineString()) ||
        scrap->getTags().contains(MetadataTags::HootMultilineString())))
    {
      multiLineStringAdded = true;
    }

    // make sure the tags are still legit on the scrap.
    scrap->setTags(splitee->getTags());
    // With the merging switching between split ways and relations, it gets a little hard to keep
    // track of where this tags is needed, so one final check here to make sure it gets added
    // correctly.
    if (_markAddedMultilineStringRelations && multiLineStringAdded &&
        scrap->getElementType() == ElementType::Relation)
    {
      scrap->getTags().set(MetadataTags::HootMultilineString(), "yes");
    }
    LOG_VART(scrap);

    replaced.push_back(
      std::pair<ElementId, ElementId>(splitee->getElementId(), scrap->getElementId()));
  }
}

void HighwaySnapMerger::_updateScrapParent(const OsmMapPtr& map, long id, const ElementPtr& scrap)
{
  if (!scrap)
    return;
  if (scrap->getElementType() == ElementType::Way)
    std::dynamic_pointer_cast<Way>(scrap)->setPid(id);
  else if (scrap->getElementType() == ElementType::Relation)
  {
    RelationPtr relation = std::dynamic_pointer_cast<Relation>(scrap);
    const vector<RelationData::Entry>& members = relation->getMembers();
    //  Iterate all of the members and update the parent id recursively
    for (size_t i = 0; i < members.size(); ++i)
      _updateScrapParent(map, id, map->getElement(members[i].getElementId()));
  }
}

}
