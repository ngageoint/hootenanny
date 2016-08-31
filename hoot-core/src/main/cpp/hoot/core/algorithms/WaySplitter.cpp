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

#include "WaySplitter.h"

// GEOS
#include <geos/geom/LineString.h>

// Hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/algorithms/linearreference/WaySubline.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/ops/ReplaceElementOp.h>
#include <hoot/core/ops/RemoveWayOp.h>
#include <hoot/core/util/ElementConverter.h>
#include <hoot/core/util/FindNodesInWayFactory.h>

namespace hoot
{

WaySplitter::WaySplitter(const OsmMapPtr& map, shared_ptr<Way> a) :
  _map(map)
{
  _a = a;
  _nf.reset(new FindNodesInWayFactory(a));
}

vector<WayPtr> WaySplitter::createSplits(const vector<WayLocation>& wl)
{
  vector<WayPtr> result;
  WayLocation last = WayLocation(_map, _a, 0, 0.0);

  result.resize(wl.size() + 1);

  for (size_t i = 0; i < wl.size(); i++)
  {
    assert(wl[i].getWay() == _a);
    WayLocation curr = wl[i];
    assert(last <= curr);

    if (last.compareTo(curr) != 0)
    {
      result[i] = WaySubline(last, curr).toWay(_map, _nf.get());
      if (result[i]->getNodeCount() == 0)
      {
        result[i].reset();
      }
    }
    last = curr;
  }

  WayLocation end(_map, _a, _a->getNodeCount() - 1, 0.0);
  if (last.compareTo(end) != 0)
  {
    result[result.size() - 1] = WaySubline(last, end).toWay(_map, _nf.get());
  }

  return result;
}

WayPtr WaySplitter::createSubline(const WaySubline& subline, vector<WayPtr>& scraps)
{
  vector<WayLocation> wls;
  wls.push_back(subline.getStart());
  wls.push_back(subline.getEnd());

  assert(subline.isValid() && subline.getStart() != subline.getEnd());

  vector<WayPtr> splits = createSplits(wls);

  assert(splits[1].get() != 0);

  if (splits[0].get())
  {
    scraps.push_back(splits[0]);
  }
  if (splits[2].get())
  {
    scraps.push_back(splits[2]);
  }

  return splits[1];
}

vector< shared_ptr<Way> > WaySplitter::split(const OsmMapPtr& map, shared_ptr<Way> a,
  WayLocation& splitPoint)
{
  WaySplitter s(map, a);
  return s.split(splitPoint);
}

void WaySplitter::split(const OsmMapPtr& map, const shared_ptr<Way>& w, double maxSize)
{
  shared_ptr<LineString> ls = ElementConverter(map).convertToLineString(w);

  double l = ls->getLength();

  if (l > maxSize)
  {
    WayLocation wl (map, w, l / 2.0);
    vector< shared_ptr<Way> > children = WaySplitter(map, w).split(wl);

    for (size_t i = 0; i < children.size(); i++)
    {
      split(map, children[i], maxSize);
    }
  }
}

vector< shared_ptr<Way> > WaySplitter::split(WayLocation& splitPoint)
{
  vector< shared_ptr<Way> > result;

  if (splitPoint.isFirst() || splitPoint.isLast())
  {
    result.push_back(_a);
  }
  else
  {
    WayLocation first(_map, _a, 0, 0.0);
    WayLocation last(_map, _a, _a->getNodeCount() - 1, 0.0);

    result.push_back(WaySubline(first, splitPoint).toWay(_map, _nf.get()));
    result.push_back(WaySubline(splitPoint, last).toWay(_map, _nf.get()));

    RemoveWayOp::removeWay(_map, _a->getId());
    _map->addWay(result[0]);
    _map->addWay(result[1]);
  }

  return result;
}


}
