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
 * @copyright Copyright (C) 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "LargeWaySplitter.h"

// GEOS
#include <geos/geom/LineString.h>

// Hoot
#include <hoot/core/algorithms/WaySplitter.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/conflate/NodeToWayMap.h>
#include <hoot/core/util/ElementConverter.h>
#include <hoot/core/algorithms/linearreference/WayLocation.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/util/Log.h>

namespace hoot
{

unsigned int LargeWaySplitter::logWarnCount = 0;

using namespace geos::geom;

LargeWaySplitter::LargeWaySplitter(double threshold)
{
  _threshold = threshold;
}

void LargeWaySplitter::apply(boost::shared_ptr<OsmMap> map)
{
  _map = map;

  // make a copy so we can make changes.
  WayMap wm = _map->getWays();
  // go through each way
  for (WayMap::const_iterator it = wm.begin(); it != wm.end(); it++)
  {
    boost::shared_ptr<Way> w = it->second;
    boost::shared_ptr<LineString> ls = ElementConverter(map).convertToLineString(w);
    double len = ls->getLength();
    // if the way is larger than the threshold
    if (len > _threshold)
    {
      int pieces = (int)(ceil(len / _threshold) + 0.1);
      _divideWay(w, pieces);
    }
  }
}

void LargeWaySplitter::_divideWay(boost::shared_ptr<Way> way, int numPieces)
{
  double startLength = ElementConverter(_map).convertToLineString(way)->getLength();
  double pieceLength = startLength / (double)numPieces;

  // iteratively carve off pieceLength sized ways from the beginning
  boost::shared_ptr<Way> tmp = way;
  for (int i = 0; i < numPieces; i++)
  {
    WayLocation wl(_map, tmp, pieceLength);
    if (wl.getSegmentIndex() == 0 && wl.getSegmentFraction() == 0.0)
    {
      WayLocation wl2(_map, tmp, pieceLength);
      if (logWarnCount < ConfigOptions().getLogWarnMessageLimit())
      {
        LOG_WARN("Invalid line piece. Odd: " << wl.toString());
      }
      else if (logWarnCount == ConfigOptions().getLogWarnMessageLimit())
      {
        LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
      }
      logWarnCount++;
    }
    else
    {
      vector< boost::shared_ptr<Way> > pieces = WaySplitter::split(_map, tmp, wl);
      assert(pieces.size() == 1 || pieces.size() == 2);
      pieces[0]->setTags(tmp->getTags());
      if (pieces.size() > 1)
      {
        pieces[1]->setTags(tmp->getTags());
        tmp = pieces[1];
      }
      else
      {
        tmp = pieces[0];
      }
    }
  }
}

void LargeWaySplitter::splitWays(boost::shared_ptr<OsmMap> map, double threshold)
{
  LargeWaySplitter a(threshold);
  a.apply(map);
}

}
