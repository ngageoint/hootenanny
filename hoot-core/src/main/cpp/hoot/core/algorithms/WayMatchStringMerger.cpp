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
#include "WayMatchStringMerger.h"

// hoot
#include <hoot/core/algorithms/linearreference/WayString.h>
#include <hoot/core/algorithms/splitter/WaySplitter.h>
#include <hoot/core/ops/RecursiveElementRemover.h>
#include <hoot/core/util/Log.h>

using namespace geos::geom;
using namespace std;

namespace hoot
{

int WayMatchStringMerger::logWarnCount = 0;

QString WayMatchStringMerger::SublineMapping::toString() const
{
  return QString("{start: %1, end: %2, newWay1: %3, way2: %4, subline2: %5, newWay2: %6}")
    .arg(hoot::toString(_start))
    .arg(hoot::toString(_end))
    .arg(hoot::toString(_newWay1 ? _newWay1->getElementId() : ElementId()))
    .arg(hoot::toString(_way2 ? _way2->getElementId() : ElementId()))
    .arg(hoot::toString(_subline2))
    .arg(_newWay2 ? hoot::toString(_newWay2->getElementId()) : "<empty>");
}

WayMatchStringMerger::WayMatchStringMerger(const OsmMapPtr& map,
  WayMatchStringMappingPtr mapping, vector<pair<ElementId, ElementId>>& replaced) :
  _map(map),
  _mapping(mapping),
  _replaced(replaced)
{
  // split the primary way string into chunks that can match n:1 with the secondary way. This will
  // make merging tags simple.
  _splitPrimary();
}

void WayMatchStringMerger::_addSublineMapping(SublineMappingPtr sm)
{
  _sublineMappingOrder.append(sm);
}

WaySublineMatchStringPtr WayMatchStringMerger::createMatchString() const
{
  LOG_TRACE("Creating match string...");

  vector<WaySublineMatch> matches;

  foreach (SublineMappingPtr sm, _sublineMappingOrder)
  {
    WaySubline ws1(sm->getStart1(), sm->getEnd1());
    WaySubline ws2(_mapping->map1To2(sm->getStart1()), _mapping->map1To2(sm->getEnd1()));
    // They're reversed if they aren't both in the same orientation.
    bool reversed = ws1.isBackwards() != ws2.isBackwards();

    ws1.ensureForwards();
    ws2.ensureForwards();

    matches.emplace_back(ws1, ws2, reversed);
  }

  LOG_VAR(matches);

  WaySublineMatchStringPtr result(new WaySublineMatchString(matches));
  return result;
}

void WayMatchStringMerger::_createWayMappings(const WayLocation& splitStart,
                                              const WayLocation& splitEnd,
                                              const WaySubline& subline2)
{
  LOG_TRACE("Creating way mappings...");

  // splitStart and splitEnd represent the locations that subline2 matches on the primary way
  // string. For this reason splitStart and splitEnd may not be on the same way. We need to go
  // through all the ways in the primary in this function and find the sublines that match up to
  // subline2, then place them into the appropriate data structures for later use.

  ConstWayPtr way2 = subline2.getWay();
  WayStringPtr ws1 = _mapping->getWayString1();

  SublineMappingPtr lastSm(new SublineMapping());
  lastSm->setStart1(splitStart);
  lastSm->setWay2(way2);

  int i = 0;
  // go through the list till we find the starting way
  while (i < ws1->getSize() && ws1->at(i).getWay() != splitStart.getWay())
  {
    ++i;
  }

  // do this till we find the end
  bool foundEnd = false;
  for (; i < ws1->getSize() && foundEnd == false; ++i)
  {
    // if there isn't a last mapping then create one.
    if (!lastSm)
    {
      lastSm.reset(new SublineMapping());
      lastSm->setStart1(ws1->at(i).getStart());
      lastSm->setWay2(way2);
    }

    // check our assumptions.
    if (ws1->at(i).getStart().getWay() != lastSm->getStart1().getWay())
    {
      throw InternalErrorException(QString("Not a way match %1, %2").arg(i).arg(
        hoot::toString(ws1->at(i).getStart().getWay()->getElementId())));
    }
    // if we found the end of the match
    if (ws1->at(i).getEnd().getWay() == splitEnd.getWay())
    {
      lastSm->setEnd1(splitEnd);
      lastSm->setSubline2(WaySubline(
        _snapToEnd(_mapping->map1To2(lastSm->getStart1(), way2->getElementId())),
        _snapToEnd(_mapping->map1To2(lastSm->getEnd1(), way2->getElementId()))));
      _addSublineMapping(lastSm);
      foundEnd = true;
    }
    // if we aren't at the end yet
    else
    {
      // add the rest of the subline to the mapping
      lastSm->setEnd1(ws1->at(i).getEnd());
      lastSm->setSubline2(WaySubline(
        _snapToEnd(_mapping->map1To2(lastSm->getStart1(), way2->getElementId())),
        _snapToEnd(_mapping->map1To2(lastSm->getEnd1(), way2->getElementId()))));
      _addSublineMapping(lastSm);
    }
    lastSm.reset();
  }
}

WayLocation WayMatchStringMerger::_findNodeLocation2(WayStringPtr ws, ElementId nodeId)
{
  LOG_TRACE("Finding node location...");

  assert(nodeId.getType() == ElementType::Node);

  // if we haven't created the index, create it.
  if (_nodeToWayLocation2.empty())
  {
    // go through the way string and look at each subline.
    for (int i = 0; i < ws->getSize(); ++i)
    {
      WaySubline subline = ws->at(i);
      ConstWayPtr w = subline.getWay();
      // go through all the segments that are part of this subline
      for (int j = subline.getFormer().getSegmentIndex();
           j <= subline.getLatter().getSegmentIndex(); ++j)
      {
        WayLocation wl(_map, w, j, 0.0);
        if (subline.contains(wl))
        {
          // only record the first instance.
          if (_nodeToWayLocation2.contains(ElementId::node(w->getNodeId(j))) == false)
          {
            assert(wl.isNode(WayLocation::SLOPPY_EPSILON) &&
              wl.getNode(WayLocation::SLOPPY_EPSILON)->getId() == w->getNodeId(j));
            _nodeToWayLocation2[ElementId::node(w->getNodeId(j))] = wl;
          }
        }
      }
    }
  }

  if (_nodeToWayLocation2.contains(nodeId) == false)
  {
    throw IllegalArgumentException("Unable to find node in way string 2.");
  }

  return _nodeToWayLocation2[nodeId];
}

void WayMatchStringMerger::mergeIntersection(ElementId scrapNodeId)
{
  LOG_TRACE("Merging intersection for scrap: " << scrapNodeId << "...");

  ConstNodePtr scrapNode = _map->getNode(scrapNodeId);
  LOG_VART(scrapNode->getElementId());
  LOG_VART(scrapNode->getX());
  LOG_VART(scrapNode->getY());

  // find the first instance of scrapNode in way 2
  WayLocation wl2 = _findNodeLocation2(_mapping->getWayString2(), scrapNodeId);

  // map the WayLocation of scrapNode to way string 1
  WayLocation wl1 = _mapping->map2To1(wl2);

  if (wl1.isExtreme(WayLocation::SLOPPY_EPSILON) == false)
  {
    if (logWarnCount < Log::getWarnMessageLimit())
    {
      // Possibly change this back to an exception as part of the work to be done in #1311.
      //throw IllegalArgumentException("scrapNode should line up with the beginning or end of a way.");

      //Note: this situation appears to arise when there is a failure to
      // split a way at a t-intersection, or when previous mergers produce
      // an intersection that is not split.

      LOG_DEBUG("scrapNode should line up with the beginning or end of a way.");
      LOG_VART(_mapping->getWayString1());
      LOG_VART(_mapping->getWayString2());
      LOG_VART(scrapNodeId);
      LOG_VART(wl2);
      LOG_VART(wl1.getWay());
      LOG_VART(wl1);
    }
    else if (logWarnCount == Log::getWarnMessageLimit())
    {
      LOG_DEBUG(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
    }
    logWarnCount++;
  }

  // move scrapNode to the WayLocation in way string 1
  _moveNode(scrapNodeId, wl1);

  // notify _sublineMapping that way string 1 changed.
  _rebuildWayString(WayNumber::Way1);
}

void WayMatchStringMerger::mergeNode(ElementId scrapNode)
{
  LOG_TRACE("Merging node for scrap: " << scrapNode << "...");

  LOG_VART(_map->getNode(scrapNode));
  // find the first instance of scrapNode in way 2
  WayLocation wl2 = _findNodeLocation2(_mapping->getWayString2(), scrapNode);

  // map the WayLocation of scrapNode to way string 1
  WayLocation wl1 = _mapping->map2To1(wl2);

  // move scrapNode to the WayLocation in way string 1
  _moveNode(scrapNode, wl1);

  // notify _sublineMapping that way string 1 changed.
  _rebuildWayString(WayNumber::Way1);
}

void WayMatchStringMerger::mergeTags()
{
  LOG_TRACE("Merging tags...");

  if (!_tagMerger)
  {
    throw IllegalArgumentException("You must specify the tag merger first.");
  }

  // go through all the way mappings
  foreach (SublineMappingPtr sm, _sublineMappingOrder)
  {
    // merge the tags (order matters)
    Tags mergedTags =
      _tagMerger->mergeTags(
        sm->getNewWay1()->getTags(), sm->getNewWay2()->getTags(), sm->getNewWay1()->getElementType());
    // set the new tags.
    sm->getNewWay1()->setTags(mergedTags);
  }
}

void WayMatchStringMerger::_moveNode(ElementId scrapNodeId, const WayLocation& wl1)
{
  LOG_TRACE("Moving node...");

  if (!_tagMerger)
  {
    throw IllegalArgumentException("You must specify the tag merger first.");
  }

  NodePtr scrapNode = _map->getNode(scrapNodeId);

  // create a new placeholder node so we don't move any scrap ways.
  NodePtr placeholder(new Node(scrapNode->getStatus(), _map->createNextNodeId(),
    scrapNode->getX(), scrapNode->getY(), scrapNode->getCircularError()));
  _map->addNode(placeholder);

  LOG_VART(_sublineMappingOrder);
  foreach (SublineMappingPtr sm, _sublineMappingOrder)
  {
    LOG_VART(sm->getNewWay2()->getId());
    LOG_VART(_map->containsElement(sm->getNewWay2()->getElementId()));
    WayPtr w = _map->getWay(sm->getNewWay2()->getId());
    w->replaceNode(scrapNodeId.getId(), placeholder->getId());
  }

  // move the scrapNode to the appropriate spot on the way.
  Coordinate c = wl1.getCoordinate();
  scrapNode->setX(c.x);
  scrapNode->setY(c.y);

  // if we're merging a node onto a node
  if (wl1.isNode(WayLocation::SLOPPY_EPSILON))
  {
    NodePtr n1 = _map->getNode(wl1.getNode(WayLocation::SLOPPY_EPSILON)->getElementId());
    NodePtr n2 = scrapNode;
    Tags t = _tagMerger->mergeTags(n1->getTags(), n2->getTags(), ElementType::Node);

    _map->replaceNode(n2->getId(), n1->getId());

    n1->setTags(t);

    _replaced.emplace_back(scrapNodeId, n1->getElementId());
  }
  else
  {
    // get a non-const version of the way so we can modify it.
    WayPtr w = _map->getWay(wl1.getWay()->getElementId());
    // grab the previous nodes and insert the new one.
    vector<long> nids = w->getNodeIds();
    nids.insert(nids.begin() + wl1.getSegmentIndex() + 1, scrapNodeId.getId());
    w->setNodes(nids);
  }
}

void WayMatchStringMerger::_rebuildWayString(WayNumber way)
{
  WayStringPtr ws(new WayString());

  for (int i = 0; i < _sublineMappingOrder.size(); ++i)
  {
    WayPtr w = _sublineMappingOrder[i]->getNewWay(way);
    if (_sublineMappingOrder[i]->getStart(way) <= _sublineMappingOrder[i]->getEnd(way))
    {
      ws->append(WaySubline(WayLocation(_map, w, 0.0), WayLocation::createAtEndOfWay(_map, w)));
    }
    else
    {
      ws->append(WaySubline(WayLocation::createAtEndOfWay(_map, w), WayLocation(_map, w, 0.0)));
    }
  }

  _mapping->setWayString(way, ws);
}

void WayMatchStringMerger::replaceScraps()
{
  LOG_TRACE("Replacing scraps...");

  // Determine which bits in secondary will be replaced by bits in the primary. Once we have that
  // we can replace them all at once, delete them, and update the _replaced structure.

  QMap<WayPtr, QList<ElementPtr>> w2ToW1;

  foreach (SublineMappingPtr sm, _sublineMappingOrder)
  {
    ElementPtr w1 = _map->getElement(sm->getNewWay1()->getElementId());
    LOG_VART(w1->getElementId());
    LOG_VART(w1->getStatus());
    WayPtr w2 = _map->getWay(sm->getNewWay2()->getId());
    LOG_VART(w2->getElementId());
    LOG_VART(w2->getStatus());

    // w1 should only occur once.
    assert(w2ToW1[w2].contains(w1) == false);

    w2ToW1[w2].append(w1);
  }

  for (QMap<WayPtr, QList<ElementPtr>>::iterator it = w2ToW1.begin(); it != w2ToW1.end(); ++it)
  {
    LOG_TRACE("Replacing: " << it.key() << " with value: " << it.value());
    _map->replace(it.key(), it.value());

    // clean up any bits that might be left over.
    it.key()->getTags().clear();
    RecursiveElementRemover(it.key()->getElementId()).apply(_map);

    for (int i = 0; i < it.value().size(); ++i)
    {
      _replaced.emplace_back(it.key()->getElementId(), it.value()[i]->getElementId());
    }
  }
}

void WayMatchStringMerger::setKeeperStatus(Status s)
{
  // go through all the way mappings
  foreach (SublineMappingPtr sm, _sublineMappingOrder)
  {
    // set the new status.
    sm->getNewWay1()->setStatus(s);
  }
}

void WayMatchStringMerger::addKeeperTags(const Tags& tags)
{
  // go through all the way mappings
  foreach (SublineMappingPtr sm, _sublineMappingOrder)
  {
    // set the new status.
    sm->getNewWay1()->addTags(tags);
  }
}

WayLocation WayMatchStringMerger::_snapToEnd(const WayLocation& wl) const
{
  LOG_TRACE("Snapping to end...");

  WayLocation result;

  if (wl.getSegmentIndex() == 0 && wl.getSegmentFraction() <= WayLocation::SLOPPY_EPSILON)
  {
    result = WayLocation(wl.getMap(), wl.getWay(), 0, 0.0);
  }
  else if ((size_t)wl.getSegmentIndex() == wl.getWay()->getNodeCount() - 1 &&
    wl.getSegmentFraction() >= 1 - WayLocation::SLOPPY_EPSILON)
  {
    result = WayLocation(wl.getMap(), wl.getWay(), wl.getWay()->getNodeCount() - 1, 1.0);
  }
  else
  {
    result = wl;
  }

  return result;
}

void WayMatchStringMerger::_splitPrimary()
{
  LOG_TRACE("Splitting primary...");

  WayStringPtr ws2 = _mapping->getWayString2();

  QList<ConstWayPtr> order2;

  // Create mappings from the sublines in s2 to sublines in s1
  for (int i = 0; i < ws2->getSize(); ++i)
  {
    WaySubline& s2 = ws2->at(i);
    LOG_VART(s2);
    WayLocation splitStart = _mapping->map2To1(s2.getStart());
    WayLocation splitEnd = _mapping->map2To1(s2.getEnd());
    _createWayMappings(splitStart, splitEnd, s2);

    if (order2.contains(s2.getWay()))
    {
      throw NotImplementedException("A single way inserted multiple times in one way is not"
        " yet supported.");
    }

    order2.append(s2.getWay());
  }
}

}
