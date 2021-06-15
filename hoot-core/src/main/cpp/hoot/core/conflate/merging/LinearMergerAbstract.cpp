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
 * @copyright Copyright (C) 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#include "LinearMergerAbstract.h"

// geos
#include <geos/geom/CoordinateSequence.h>
#include <geos/geom/CoordinateSequenceFactory.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/LineString.h>

// hoot
#include <hoot/core/conflate/highway/HighwayMatch.h>
#include <hoot/core/elements/NodeToWayMap.h>
#include <hoot/core/geometry/ElementToGeometryConverter.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/ops/RecursiveElementRemover.h>
#include <hoot/core/util/Log.h>

using namespace geos::geom;
using namespace std;

namespace hoot
{

int LinearMergerAbstract::logWarnCount = 0;

LinearMergerAbstract::LinearMergerAbstract(
  const std::set<std::pair<ElementId, ElementId>>& pairs,
  const std::shared_ptr<SublineStringMatcher>& sublineMatcher) :
MergerBase(pairs),
_sublineMatcher(sublineMatcher)
{
}

void LinearMergerAbstract::apply(const OsmMapPtr& map, vector<pair<ElementId, ElementId>>& replaced)
{
  LOG_TRACE("Applying linear merger...");
  LOG_VART(_pairs);
  _map = map;

  vector<pair<ElementId, ElementId>> pairs;
  pairs.reserve(_pairs.size());
  for (set<pair<ElementId, ElementId>>::const_iterator it = _pairs.begin(); it != _pairs.end();
       ++it)
  {
    ElementId eid1 = it->first;
    ElementId eid2 = it->second;

    if (_map->containsElement(eid1) && _map->containsElement(eid2))
    {
      pairs.push_back(pair<ElementId, ElementId>(eid1, eid2));
    }
    else
    {
      LOG_TRACE(
        "Map doesn't contain one or more of the following elements: " << eid1 << ", " << eid2);
    }
  }

  _mergeShortestPairsFirst(pairs, replaced);
}

void LinearMergerAbstract::_mergeShortestPairsFirst(
  std::vector<std::pair<ElementId, ElementId>>& pairs,
  std::vector<std::pair<ElementId, ElementId>>& replaced)
{
    ShortestFirstComparator shortestFirst;
    shortestFirst.map = _map;
    sort(pairs.begin(), pairs.end(), shortestFirst);
    LOG_VART(pairs);
    for (vector<pair<ElementId, ElementId>>::const_iterator it = pairs.begin(); it != pairs.end();
         ++it)
    {
      ElementId eid1 = it->first;
      ElementId eid2 = it->second;

      for (size_t i = 0; i < replaced.size(); i++)
      {
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
      }

      _eidLogString = "-" + eid1.toString() + "-" + eid2.toString();
      _mergePair(eid1, eid2, replaced);
    }
}

void LinearMergerAbstract::_markNeedsReview(
  ElementPtr e1, ElementPtr e2, QString note, QString reviewType)
{
  if (!e1 && !e2)
  {
    if (logWarnCount < Log::getWarnMessageLimit())
    {
      LOG_WARN("Unable to mark element as needing review. Neither element exists. " << note);
    }
    else if (logWarnCount == Log::getWarnMessageLimit())
    {
      LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
    }
    logWarnCount++;
  }
  else if (e1 && e2)
  {
    _reviewMarker.mark(_map, e1, e2, note, reviewType);
  }
  else if (e1)
  {
    _reviewMarker.mark(_map, e1, note, reviewType);
  }
  else if (e2)
  {
    _reviewMarker.mark(_map, e2, note, reviewType);
  }
}

bool LinearMergerAbstract::_mergePair(
  ElementId eid1, ElementId eid2, vector<pair<ElementId, ElementId>>& /*replaced*/)
{
  ElementPtr e1 = _map->getElement(eid1);
  ElementPtr e2 = _map->getElement(eid2);

  // if the element is no longer part of the map. This can happen in rare cases where a match may
  // not conflict with any one match in the set, but may conflict with multiple matches in the
  // set. E.g.
  // A x-------x
  // B    x--------x
  // C         x--------x
  //
  // In the above example no two matches conflict, but B conflicts with (A U C). We don't determine
  // this in the conflict code at this time, so we'll ignore the merge.
  if (!e1 || !e2)
  {
    _markNeedsReview(e1, e2, "Missing match pair", HighwayMatch::getHighwayMatchName());
    return true;
  }
  return false;
}

void LinearMergerAbstract::_removeSpans(const ElementPtr& e1, const ElementPtr& e2) const
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

    _removeSpans(w1, w2);
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
      _removeSpans(_map->getWay(m1), _map->getWay(m2));
    }
  }

  if (ConfigOptions().getDebugMapsWriteDetailed())
  {
    OsmMapWriterFactory::writeDebugMap(_map, className(), "after-remove-spans" + _eidLogString);
  }
}

void LinearMergerAbstract::_removeSpans(const WayPtr& w1, const WayPtr& w2) const
{
  LOG_TRACE("Removing spans...");

  std::shared_ptr<NodeToWayMap> n2w = _map->getIndex().getNodeToWayMap();

  // find all the ways that connect to the beginning or end of w1. There shouldn't be any that
  // connect in the middle.
  set<long> wids = n2w->getWaysByNode(w1->getNodeIds()[0]);
  const set<long>& wids2 = n2w->getWaysByNode(w1->getLastNodeId());
  wids.insert(wids2.begin(), wids2.end());

  for (set<long>::const_iterator it = wids.begin(); it != wids.end(); ++it)
  {
    if (*it != w1->getId() && *it != w2->getId())
    {
      const WayPtr& w = _map->getWay(*it);
      // if this isn't one of the ways we're evaluating for a connection between and it is part of
      // the data set we're conflating in
      if (w->getElementId() != w1->getElementId() &&
          w->getElementId() != w2->getElementId() &&
          w->getStatus() == Status::Unknown2)
      {
        // if this way connects w1 to w2 at the beginning or the end
        if ((_doesWayConnect(w1->getNodeId(0), w2->getNodeId(0), w) ||
             _doesWayConnect(w1->getLastNodeId(), w2->getLastNodeId(), w)) &&
            // if the connection is more or less a straight shot. Don't want to worry about round
            // about connections (e.g. lollipop style).
            _directConnect(w))
        {
          // This should likely remove the way even if it is part of another relation - #2938
          RecursiveElementRemover(w->getElementId()).apply(_map);
        }
      }
    }
  }
}

bool LinearMergerAbstract::_directConnect(WayPtr w) const
{
  std::shared_ptr<LineString> ls = ElementToGeometryConverter(_map).convertToLineString(w);
  if (!ls)
  {
    return false;
  }

  std::unique_ptr<CoordinateSequence> cs =
    GeometryFactory::getDefaultInstance()->getCoordinateSequenceFactory()->create(2, 2);

  cs->setAt(_map->getNode(w->getNodeId(0))->toCoordinate(), 0);
  cs->setAt(_map->getNode(w->getLastNodeId())->toCoordinate(), 1);

  // create a straight line and buffer it
  std::shared_ptr<LineString> straight(GeometryFactory::getDefaultInstance()->createLineString(*cs));
  std::shared_ptr<Geometry> g(straight->buffer(w->getCircularError()));

  // is the way in question completely contained in the buffer?
  return g->contains(ls.get());
}

bool LinearMergerAbstract::_doesWayConnect(long node1, long node2, const ConstWayPtr& w) const
{
  return
    (w->getNodeId(0) == node1 && w->getLastNodeId() == node2) ||
    (w->getNodeId(0) == node2 && w->getLastNodeId() == node1);
}

QString LinearMergerAbstract::toString() const
{
  return QString("LinearMerger, pairs: ") + hoot::toString(_pairs);
}

}

