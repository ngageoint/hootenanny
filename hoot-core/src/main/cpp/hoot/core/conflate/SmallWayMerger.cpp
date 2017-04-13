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
 * @copyright Copyright (C) 2015, 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "SmallWayMerger.h"

// GEOS
#include <geos/geom/LineString.h>

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/algorithms/DirectionFinder.h>
#include <hoot/core/conflate/NodeToWayMap.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/ops/RecursiveElementRemover.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/schema/ExactTagDifferencer.h>
#include <hoot/core/schema/TagMergerFactory.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/ElementConverter.h>
#include <hoot/core/util/Log.h>

// Tgs
#include <tgs/StreamUtils.h>

using namespace Tgs;

namespace hoot
{
  using namespace std;

HOOT_FACTORY_REGISTER(OsmMapOperation, SmallWayMerger)

SmallWayMerger::SmallWayMerger(Meters threshold)
{
  if (threshold >= 0)
  {
    _threshold = threshold;
  }
  else
  {
    _threshold = ConfigOptions().getSmallWayMergerThreshold();
  }
  _diff.reset(Factory::getInstance().constructObject<TagDifferencer>(
    ConfigOptions().getSmallWayMergerDiff()));
}

void SmallWayMerger::apply(boost::shared_ptr<OsmMap>& map)
{
  _map = map;

  // create a map from nodes to ways
  boost::shared_ptr<NodeToWayMap> n2wp = _map->getIndex().getNodeToWayMap();
  _n2w = n2wp.get();

  // make a copy so we can make changes.
  WayMap wm = _map->getWays();
  // go through each way
  for (WayMap::const_iterator it = wm.begin(); it != wm.end(); it++)
  {
    // if we haven't already merged the way
    if (_map->containsWay(it->first))
    {
      boost::shared_ptr<Way> w = it->second;

      // if the way is smaller than the threshold
      if (OsmSchema::getInstance().isLinearHighway(w->getTags(), w->getElementType()) &&
        ElementConverter(map).convertToLineString(w)->getLength() <= _threshold)
      {
        _mergeNeighbors(w);
      }
    }
  }
}

void SmallWayMerger::_mergeNeighbors(boost::shared_ptr<Way> w)
{
  NodeToWayMap& n2w = *_n2w;

  // is there a way on the front end that isn't an intersection
  const set<long>& front = n2w[w->getNodeId(0)];
  const set<long>& back = n2w[w->getLastNodeId()];
  if (front.size() == 2)
  {
    // merge those two ways
    _mergeWays(front);
  }
  // is there a way on the back end that isn't an intersection
  else if (back.size() == 2)
  {
    // merge those two ways
    _mergeWays(back);
  }
}

void SmallWayMerger::_mergeWays(const set<long>& ids)
{
  assert(ids.size() == 2);

  set<long>::iterator it = ids.begin();
  boost::shared_ptr<Way> w1 = _map->getWay(*it);
  boost::shared_ptr<Way> w2 = _map->getWay(*(++it));

  // if either way is not a highway
  if (OsmSchema::getInstance().isLinearHighway(w1->getTags(), w1->getElementType()) == false ||
      OsmSchema::getInstance().isLinearHighway(w2->getTags(), w2->getElementType()) == false)
  {
    return;
  }

  // if they're from the same input sets & have effectively the same tags
  if (w1->getStatus() == w2->getStatus() &&
      _diff->diff(_map, w1, w2) == 0.0)
  {
    // if both ways are one way
    if (OsmSchema::getInstance().isOneWay(*w1) &&
        OsmSchema::getInstance().isOneWay(*w2))
    {
      // if they the beginning of one isn't equal to the end of the other
      if (w1->getNodeId(0) != w2->getLastNodeId() &&
          w2->getNodeId(0) != w1->getLastNodeId())
      {
        // They aren't headed in a consistent direction. No need to merge.
        return;
      }
    }

    // Line the ways up so they're end to head and assign them to first and next.
    boost::shared_ptr<Way> first, next;
    if (w1->getLastNodeId() == w2->getNodeId(0))
    {
      first = w1;
      next = w2;
    }
    else if (w1->getLastNodeId() == w2->getLastNodeId())
    {
      w2->reverseOrder();
      first = w1;
      next = w2;
    }
    else if (w1->getNodeId(0) == w2->getLastNodeId())
    {
      first = w2;
      next = w1;
    }
    else if (w1->getNodeId(0) == w2->getNodeId(0))
    {
      w1->reverseOrder();
      first = w1;
      next = w2;
    }
    else
    {
      LOG_TRACE("w1: " << w1->toString());
      LOG_TRACE("w2: " << w2->toString());
      throw HootException("The ends of the ways don't touch. "
                    "Did you run the intersection splitter first?");
    }

    // if the ways share both ends (circle) then this causes bad weird things to happen so
    // we aren't going to allow it. Most notably you may end up with an intersection in the middle
    // of a way when we're done.
    if (first->getNodeId(0) == next->getLastNodeId())
    {
      // pass
    }
    else
    {
      // add next's nodes onto first's list.
      for (size_t i = 1; i < next->getNodeCount(); ++i)
      {
        first->addNode(next->getNodeIds()[i]);
      }

      Tags tags = TagMergerFactory::mergeTags(first->getTags(), next->getTags(),
        first->getElementType());
      first->setTags(tags);

      // just in case we can't delete it, clear the tags.
      next->getTags().clear();
      RecursiveElementRemover(next->getElementId()).apply(_map);
    }
  }
}

void SmallWayMerger::mergeWays(boost::shared_ptr<OsmMap> map, Meters threshold)
{
  SmallWayMerger a(threshold);
  a.apply(map);
}

}
