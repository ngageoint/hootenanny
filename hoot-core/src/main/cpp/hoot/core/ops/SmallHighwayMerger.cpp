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

#include "SmallHighwayMerger.h"

// GEOS
#include <geos/geom/LineString.h>

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/algorithms/DirectionFinder.h>
#include <hoot/core/elements/NodeToWayMap.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/ops/RecursiveElementRemover.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/schema/TagMergerFactory.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/geometry/ElementToGeometryConverter.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/criterion/OneWayCriterion.h>
#include <hoot/core/schema/TagDifferencer.h>
#include <hoot/core/criterion/BridgeCriterion.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/criterion/HighwayCriterion.h>


// Tgs
#include <tgs/StreamUtils.h>

using namespace std;
using namespace Tgs;
using namespace geos::geom;

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, SmallHighwayMerger)

SmallHighwayMerger::SmallHighwayMerger(Meters threshold)
{
  ConfigOptions opts = ConfigOptions();
  if (threshold >= 0)
  {
    _threshold = threshold;
  }
  else
  {
    _threshold = opts.getSmallHighwayMergerThreshold();
  }
  _diff = Factory::getInstance().constructObject<TagDifferencer>(opts.getSmallHighwayMergerDiff());
  _taskStatusUpdateInterval = opts.getTaskStatusUpdateInterval();
}

void SmallHighwayMerger::apply(std::shared_ptr<OsmMap>& map)
{
  _map = map;
  _numProcessed = 0;

  // create a map from nodes to ways
  std::shared_ptr<NodeToWayMap> n2wp = _map->getIndex().getNodeToWayMap();
  _n2w = n2wp.get();

  // make a copy so we can make changes.
  WayMap wm = _map->getWays();
  // go through each way
  HighwayCriterion highwayCrit(_map);
  for (WayMap::const_iterator it = wm.begin(); it != wm.end(); ++it)
  {
    // if we haven't already merged the way
    if (_map->containsWay(it->first))
    {
      std::shared_ptr<Way> w = it->second;

      // if the way is smaller than the threshold, that isn't a `hoot:special` way
      if (w && highwayCrit.isSatisfied(w) &&
          !w->getTags().contains(MetadataTags::HootSpecial()))
      {
        std::shared_ptr<LineString> linestring = ElementToGeometryConverter(map).convertToLineString(w);
        if (linestring && linestring->getLength() <= _threshold)
          _mergeNeighbors(w);
      }
    }

    _numProcessed++;
    if (_numProcessed % (_taskStatusUpdateInterval * 10) == 0)
    {
      PROGRESS_INFO(
        "\tProcessed " << StringUtils::formatLargeNumber(_numProcessed) <<
        " highways for possible merging.");
    }
  }
}

void SmallHighwayMerger::_mergeNeighbors(const std::shared_ptr<Way>& w)
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

void SmallHighwayMerger::_mergeWays(const set<long>& ids)
{
  assert(ids.size() == 2);

  set<long>::iterator it = ids.begin();
  std::shared_ptr<Way> w1 = _map->getWay(*it);
  std::shared_ptr<Way> w2 = _map->getWay(*(++it));

  HighwayCriterion highwayCrit(_map);
  // if either way is not a highway
  // TODO: could we gain anything by opening this whole class up to other linear feature types?
  if (highwayCrit.isSatisfied(w1) == false || highwayCrit.isSatisfied(w2) == false)
  {
    return;
  }

  // if either one is a loop (because it's really an intersection, even though
  // "only" two ways intersect!)
  if (w1->isSimpleLoop() || w2->isSimpleLoop())
  {
    return;
  }

  // if they're from the same input sets & have effectively the same tags
  if (w1->getStatus() == w2->getStatus() && _diff->diff(_map, w1, w2) == 0.0)
  {
    // if both ways are one-way & the beginning of one isn't equal to the end
    // of the other
    OneWayCriterion oneWayCrit;
    if ((oneWayCrit.isSatisfied(w1) && oneWayCrit.isSatisfied(w2)) &&
        (w1->getNodeId(0) != w2->getLastNodeId() &&
         w2->getNodeId(0) != w1->getLastNodeId()))
    {
      // They aren't headed in a consistent direction. No need to merge.
      return;
    }

    BridgeCriterion bridgeCrit;
    const bool w1IsBridge = bridgeCrit.isSatisfied(w1);
    const bool w2IsBridge = bridgeCrit.isSatisfied(w2);
    if ((w1IsBridge && !w2IsBridge) || (w2IsBridge && !w1IsBridge))
    {
      // One is a bridge and the other isn't. Don't merge.
      return;
    }

    // Line the ways up so they're end to head and assign them to first and next.
    std::shared_ptr<Way> first, next;
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
      //throw HootException("The ends of the ways don't touch. "
                          //"Did you run the intersection splitter first?");
      // So far, have only seen this happen due to the UnconnectedWaySplitter connecting two roads
      // pointing toward each other. That may need to be addressed, but regardless, don't think an
      // exception is needed here.
      LOG_TRACE(
        "The ends of the ways don't touch. Skipping merge of: " << w1->getElementId() << " and " <<
        w2->getElementId());
      return;
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
      LOG_TRACE("Merging " << next->getElementId() << " into " << first->getElementId() << "...");
      LOG_VART(first->getElementId());
      LOG_VART(first->getStatus());
      LOG_VART(next->getElementId());
      LOG_VART(next->getStatus());

      // add next's nodes onto first's list.
      for (size_t i = 1; i < next->getNodeCount(); ++i)
      {
        first->addNode(next->getNodeIds()[i]);
      }

      Tags tags = TagMergerFactory::mergeTags(first->getTags(), next->getTags(),
        first->getElementType());
      first->setTags(tags);
      first->setPid(Way::getPid(first, next));

      // just in case we can't delete it, clear the tags.
      next->getTags().clear();
      RecursiveElementRemover(next->getElementId()).apply(_map);

      LOG_VART(_map->containsElement(next->getElementId()));

      _numAffected++;
    }
  }
}

void SmallHighwayMerger::mergeWays(std::shared_ptr<OsmMap> map, Meters threshold)
{
  SmallHighwayMerger a(threshold);
  a.apply(map);
}

QStringList SmallHighwayMerger::getCriteria() const
{
  return QStringList(HighwayCriterion::className());
}

}
