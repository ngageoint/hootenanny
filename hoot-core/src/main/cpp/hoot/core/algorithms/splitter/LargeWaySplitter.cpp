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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#include "LargeWaySplitter.h"

// GEOS
#include <geos/geom/LineString.h>

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/algorithms/splitter/WaySplitter.h>
#include <hoot/core/algorithms/linearreference/WayLocation.h>
#include <hoot/core/elements/NodeToWayMap.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/geometry/ElementToGeometryConverter.h>
#include <hoot/core/util/Log.h>

using namespace geos::geom;
using namespace std;

namespace hoot
{

int LargeWaySplitter::logWarnCount = 0;

LargeWaySplitter::LargeWaySplitter(double threshold)
{
  _threshold = threshold;
}

void LargeWaySplitter::apply(const std::shared_ptr<OsmMap>& map)
{
  _map = map;

  // make a copy so we can make changes.
  WayMap wm = _map->getWays();
  // go through each way
  for (WayMap::const_iterator it = wm.begin(); it != wm.end(); ++it)
  {
    std::shared_ptr<Way> w = it->second;
    std::shared_ptr<LineString> ls = ElementToGeometryConverter(map).convertToLineString(w);
    double len = ls->getLength();
    // if the way is larger than the threshold
    if (len > _threshold)
    {
      int pieces = (int)(ceil(len / _threshold) + 0.1);
      _divideWay(w, pieces);
    }
  }
}

void LargeWaySplitter::_divideWay(const std::shared_ptr<Way>& way, int numPieces) const
{
  double startLength = ElementToGeometryConverter(_map).convertToLineString(way)->getLength();
  double pieceLength = startLength / (double)numPieces;

  // iteratively carve off pieceLength sized ways from the beginning
  std::shared_ptr<Way> tmp = way;
  for (int i = 0; i < numPieces - 1; i++)
  {
    WayLocation wl(_map, tmp, pieceLength);
    if (wl.getSegmentIndex() == 0 && wl.getSegmentFraction() == 0.0)
    {
      WayLocation wl2(_map, tmp, pieceLength);
      if (logWarnCount < Log::getWarnMessageLimit())
      {
        LOG_WARN("Invalid line piece. Odd: " << wl.toString());
      }
      else if (logWarnCount == Log::getWarnMessageLimit())
      {
        LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
      }
      logWarnCount++;
    }
    else
    {
      vector<std::shared_ptr<Way>> pieces = WaySplitter::split(_map, tmp, wl);
      assert(pieces.size() == 2);
      tmp = pieces[1];
    }
  }
}

void LargeWaySplitter::splitWays(const std::shared_ptr<OsmMap>& map, double threshold)
{
  LargeWaySplitter a(threshold);
  a.apply(map);
}

}
