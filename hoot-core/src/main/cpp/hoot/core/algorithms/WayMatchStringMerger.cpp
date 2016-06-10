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
#include "WayMatchStringMerger.h"

// hoot
#include <hoot/core/algorithms/WaySplitter.h>
#include <hoot/core/algorithms/linearreference/WayString.h>
#include <hoot/core/ops/RecursiveElementRemover.h>
#include <hoot/core/util/ElementConverter.h>

namespace hoot
{

WayMatchStringMerger::WayMatchStringMerger(const OsmMapPtr& map,
  WayMatchStringMappingPtr mapping, vector<pair<ElementId, ElementId> > &replaced) :
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
  _sublineMapping.insert(_map->getWay(sm->start.getWay()->getElementId()), sm);
  _sublineMappingOrder.append(sm);
}

WaySublineMatchStringPtr WayMatchStringMerger::createMatchString() const
{
  vector<WaySublineMatch> matches;

  for (int i = 0; i < _sublineMappingOrder.size(); ++i)
  {
    SublineMappingPtr sm = _sublineMappingOrder[i];
    WaySubline ws1(sm->start, sm->end);
    WaySubline ws2(_mapping->map1To2(sm->start), _mapping->map1To2(sm->end));
    // they're reversed if they aren't both in the same orientation.
    bool reversed = ws1.isBackwards() != ws2.isBackwards();

    ws1.ensureForwards();
    ws2.ensureForwards();

    matches.push_back(WaySublineMatch(ws1, ws2, reversed));
  }

  LOG_VAR(matches);

  WaySublineMatchStringPtr result(new WaySublineMatchString(matches));
  return result;
}

void WayMatchStringMerger::_createWayMappings(WayLocation split1, WayLocation split2,
  WaySubline subline2)
{
  ConstWayPtr way2 = subline2.getWay();
  WayStringPtr ws1 = _mapping->getWayString1();

  SublineMappingPtr lastSm(new SublineMapping());
  lastSm->start = split1;
  lastSm->way2 = way2;
  lastSm->subline2 = subline2;

  int i = 0;
  // go through the list till we find the starting way
  while (i < ws1->getSize() && ws1->at(i).getWay() != split1.getWay())
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
      lastSm->start = ws1->at(i).getStart();
      lastSm->way2 = way2;
      lastSm->subline2 = subline2;
    }

    if (ws1->at(i).getStart().getWay() != lastSm->start.getWay())
    {
      throw InternalErrorException(QString("Not a way match %1, %2").arg(i).arg(
        hoot::toString(ws1->at(i).getStart().getWay()->getElementId())));
    }
    // if we found the end of the match
    if (ws1->at(i).getEnd().getWay() == split2.getWay())
    {
      lastSm->end = split2;
      _addSublineMapping(lastSm);
      foundEnd = true;
    }
    // if we aren't at the end yet
    else
    {
      // add the rest of the subline to the mapping
      lastSm->end = ws1->at(i).getEnd();
      _addSublineMapping(lastSm);
    }
    lastSm.reset();
  }
}

WayLocation WayMatchStringMerger::_findNodeLocation2(WayStringPtr ws, ElementId nodeId)
{
  assert(nodeId.getType() == ElementType::Node);

  // if we haven't created the index, create it.
  if (_nodeToWayLocation2.size() == 0)
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
            assert(wl.isNode() && wl.getNode()->getId() == w->getNodeId(j));
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

void WayMatchStringMerger::mergeNode(ElementId scrapNode)
{
  LOG_VAR(scrapNode);
  LOG_VAR(_map->getNode(scrapNode));
  // find the first instance of scrapNode in way 2
  WayLocation wl2 = _findNodeLocation2(_mapping->getWayString2(), scrapNode);

  // map the WayLocation of scrapNode to way string 1
  WayLocation wl1 = _mapping->map2To1(wl2);

  // move scrapNode to the WayLocation in way string 1
  _moveNode(scrapNode, wl1);

  // notify _sublineMapping that way string 1 changed.
  _rebuildWayString1();
}

void WayMatchStringMerger::mergeTags()
{
  if (!_tagMerger)
  {
    throw IllegalArgumentException("You must specify the tag merger first.");
  }

  // go through all the way mappings
  for (QMultiMap<WayPtr, SublineMappingPtr>::iterator it = _sublineMapping.begin();
    it != _sublineMapping.end(); ++it)
  {
    LOG_VAR(it.value());
    // merge the tags (order matters)
    Tags mergedTags = _tagMerger->mergeTags(it.value()->newWay1->getTags(),
      it.value()->way2->getTags(), it.value()->newWay1->getElementType());
    // set the new tags.
    it.value()->newWay1->setTags(mergedTags);
  }
}

void WayMatchStringMerger::_moveNode(ElementId scrapNode, WayLocation wl1)
{
  if (!_tagMerger)
  {
    throw IllegalArgumentException("You must specify the tag merger first.");
  }

  // move the scrapNode to the appropriate spot on the way.
  Coordinate c = wl1.getCoordinate();
  _map->getNode(scrapNode)->setX(c.x);
  _map->getNode(scrapNode)->setY(c.y);

  // if we're merging a node onto a node
  if (wl1.isNode())
  {
    NodePtr n1 = _map->getNode(wl1.getNode()->getElementId());
    NodePtr n2 = _map->getNode(scrapNode);
    LOG_VAR(n1);
    LOG_VAR(n2);
    Tags t = _tagMerger->mergeTags(n1->getTags(), n2->getTags(), ElementType::Node);

    _map->replaceNode(n2->getId(), n1->getId());

    n1->setTags(t);

    _replaced.push_back(pair<ElementId, ElementId>(scrapNode, n1->getElementId()));
  }
  else
  {
    // get a non-const version of the way so we can modify it.
    WayPtr w = _map->getWay(wl1.getWay()->getElementId());
    // grab the previous nodes and insert the new one.
    vector<long> nids = w->getNodeIds();
    LOG_VAR(nids);
    LOG_VAR(wl1.getSegmentIndex());
    nids.insert(nids.begin() + wl1.getSegmentIndex() + 1, scrapNode.getId());
    LOG_VAR(nids);
    w->setNodes(nids);
  }
}

void WayMatchStringMerger::_rebuildWayString1()
{
  WayStringPtr ws1(new WayString());

  LOG_VAR(_sublineMappingOrder);

  for (int i = 0; i < _sublineMappingOrder.size(); ++i)
  {
    WayPtr w1 = _sublineMappingOrder[i]->newWay1;
    LOG_VAR(w1);
    if (_sublineMappingOrder[i]->start <= _sublineMappingOrder[i]->end)
    {
      ws1->append(WaySubline(WayLocation(_map, w1, 0.0), WayLocation::createAtEndOfWay(_map, w1)));
    }
    else
    {
      ws1->append(WaySubline(WayLocation::createAtEndOfWay(_map, w1), WayLocation(_map, w1, 0.0)));
    }
  }

  _mapping->setWayString1(ws1);
}

void WayMatchStringMerger::replaceScraps()
{
  QMap< WayPtr, QList<ElementPtr> > w2ToW1;

  for (int i = 0; i < _sublineMappingOrder.size(); ++i)
  {
    SublineMappingPtr sm = _sublineMappingOrder[i];
    ElementPtr w1 = _map->getElement(sm->newWay1->getElementId());
    WayPtr w2 = _map->getWay(sm->way2->getId());

    LOG_VAR(sm->subline2);

    // make sure the subline covers the full way -- partial matches will require more work.
    assert(sm->subline2.getFormer().isFirst() && sm->subline2.getLatter().isLast());

    // w1 should only occur once.
    assert(w2ToW1[w2].contains(w1) == false);

    w2ToW1[w2].append(w1);
  }

  for (QMap< WayPtr, QList<ElementPtr> >::iterator it = w2ToW1.begin();
    it != w2ToW1.end(); ++it)
  {
    _map->replace(it.key(), it.value());

    // clean up any bits that might be left over.
    it.key()->getTags().clear();
    RecursiveElementRemover(it.key()->getElementId()).apply(_map);

    for (int i = 0; i < it.value().size(); ++i)
    {
      _replaced.push_back(pair<ElementId, ElementId>(it.key()->getElementId(),
        it.value()[i]->getElementId()));
    }
  }
}

void WayMatchStringMerger::setKeeperStatus(Status s)
{
  // go through all the way mappings
  for (QMultiMap<WayPtr, SublineMappingPtr>::iterator it = _sublineMapping.begin();
    it != _sublineMapping.end(); ++it)
  {
    // set the new status.
    it.value()->newWay1->setStatus(s);
  }
}

void WayMatchStringMerger::_splitPrimary()
{
  WayStringPtr ws1 = _mapping->getWayString1();
  WayStringPtr ws2 = _mapping->getWayString2();

  QList<ConstWayPtr> order2;

  /*
   * Create mappings from the sublines in s2 to sublines in s1
   */
  for (int i = 0; i < ws2->getSize(); ++i)
  {
    WaySubline& s2 = ws2->at(i);
    LOG_VAR(s2);
    WayLocation split1 = _mapping->map2To1(s2.getStart());
    WayLocation split2 = _mapping->map2To1(s2.getEnd());
    _createWayMappings(split1, split2, s2);

    if (order2.contains(s2.getWay()))
    {
      throw NotImplementedException("A single way inserted multiple times in one way is not"
        " yet supported.");
    }

    order2.append(s2.getWay());
  }

  ElementConverter ec(_map);

  // split each way in the subline mapping.
  foreach (WayPtr w1, _sublineMapping.uniqueKeys())
  {
    QList<SublineMappingPtr> sm = _sublineMapping.values(w1);
    qSort(sm.begin(), sm.end(), SublineMappingLessThan());

    // if there is only one entry in sm and it spans the whole way, don't split anything.
    if (sm.size() == 1 && sm[0]->start.isExtreme() && sm[0]->end.isExtreme())
    {
      sm[0]->newWay1 = w1;
      continue;
    }

    vector<WayLocation> wls;

    // push them all on. This will inevitably create some empty ways, but it is predictable.
    for (int i = 0; i < sm.size(); ++i)
    {
      wls.push_back(sm.at(i)->start);
      wls.push_back(sm.at(i)->end);
    }

    LOG_VAR(w1.get());
    LOG_VAR(wls);
    vector<WayPtr> splits = WaySplitter(_map, w1).createSplits(wls);

    assert((int)splits.size() == sm.size() * 2 + 1);

    int c = 0;
    WayPtr w = splits[c++];
    // if this isn't empty
    if (w && ec.calculateLength(w) > 0.0)
    {
      _scraps1.append(w);
      _replaced.push_back(pair<ElementId, ElementId>(w1->getElementId(), w->getElementId()));
    }

    QList<ElementPtr> newWays;
    for (int i = 0; i < sm.size(); ++i)
    {
      WayPtr w;
      w = splits[c++];
      sm.at(i)->newWay1 = w;
      _replaced.push_back(pair<ElementId, ElementId>(w1->getElementId(), w->getElementId()));
      newWays.append(w);

      w = splits[c++];
      // if this isn't empty
      if (w && ec.calculateLength(w) > 0.0)
      {
        // only the last one should be non-empty
        assert(i == sm.size() - 1);
        newWays.append(w);
        _scraps1.append(w);
        _replaced.push_back(pair<ElementId, ElementId>(w1->getElementId(), w->getElementId()));
      }
    }

    _map->replace(w1, newWays);
  }

  _rebuildWayString1();
}

}
