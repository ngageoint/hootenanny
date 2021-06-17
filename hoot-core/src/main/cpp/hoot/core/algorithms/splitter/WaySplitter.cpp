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

#include "WaySplitter.h"

// GEOS
#include <geos/geom/LineString.h>

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/algorithms/linearreference/WayLocation.h>
#include <hoot/core/algorithms/linearreference/WaySubline.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/ops/ReplaceElementOp.h>
#include <hoot/core/ops/RemoveWayByEid.h>
#include <hoot/core/geometry/ElementToGeometryConverter.h>
#include <hoot/core/algorithms/FindNodesInWayFactory.h>

using namespace geos::geom;
using namespace std;

namespace hoot
{

WaySplitter::WaySplitter(const OsmMapPtr& map, WayPtr way) :
  _map(map)
{
  _way = way;
  _nf.reset(new FindNodesInWayFactory(_way));
}

vector<WayPtr> WaySplitter::createSplits(const vector<WayLocation>& wl) const
{
  vector<WayPtr> result;
  WayLocation last = WayLocation(_map, _way, 0, 0.0);

  result.resize(wl.size() + 1);

  for (size_t i = 0; i < wl.size(); i++)
  {
    assert(wl[i].getWay() == _way);
    WayLocation curr = wl[i];

    if (last.compareTo(curr) != 0)
    {
      result[i] = WaySubline(last, curr).toWay(_map, _nf.get());
      result[i]->setPid(_way->getId());
      if (result[i]->getNodeCount() == 0)
      {
        result[i].reset();
      }
    }
    last = curr;
  }

  WayLocation end(_map, _way, _way->getNodeCount() - 1, 0.0);
  if (last.compareTo(end) != 0)
  {
    WayPtr w = WaySubline(last, end).toWay(_map, _nf.get());
    w->setPid(_way->getId());
    result[result.size() - 1] = w;
  }

  return result;
}

WayPtr WaySplitter::createSubline(const WaySubline& subline, vector<WayPtr>& scraps) const
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

vector<WayPtr> WaySplitter::split(const OsmMapPtr& map, WayPtr way, WayLocation& splitPoint)
{
  WaySplitter s(map, way);
  return s.split(splitPoint);
}

void WaySplitter::split(const OsmMapPtr& map, const WayPtr& w, double maxSize)
{
  std::shared_ptr<LineString> ls = ElementToGeometryConverter(map).convertToLineString(w);

  double l = ls->getLength();

  if (l > maxSize)
  {
    WayLocation wl (map, w, l / 2.0);
    vector<WayPtr> children = WaySplitter(map, w).split(wl);

    for (size_t i = 0; i < children.size(); i++)
    {
      split(map, children[i], maxSize);
    }
  }
}

vector<WayPtr> WaySplitter::split(WayLocation& splitPoint) const
{
  vector<WayPtr> result;

  if (splitPoint.isFirst() || splitPoint.isLast())
  {
    result.push_back(_way);
  }
  else
  {
    WayLocation first(_map, _way, 0, 0.0);
    WayLocation last(_map, _way, _way->getNodeCount() - 1, 0.0);

    result.push_back(WaySubline(first, splitPoint).toWay(_map, _nf.get(), true));
    result.push_back(WaySubline(splitPoint, last).toWay(_map, _nf.get(), false));

    //  Record the parent id for the way joiner
    result[1]->setPid(_way->getId());

    RemoveWayByEid::removeWay(_map, _way->getId());
    _map->addWay(result[0]);
    _map->addWay(result[1]);
  }

  return result;
}

}
