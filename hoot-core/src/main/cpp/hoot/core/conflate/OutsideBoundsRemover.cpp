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

#include "OutsideBoundsRemover.h"

// GEOS
#include <geos/geom/LineString.h>

// Hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/ops/RemoveWayOp.h>

namespace hoot
{

OutsideBoundsRemover::OutsideBoundsRemover(shared_ptr<OsmMap> map, const Envelope& e, bool inverse)
{
  _inputMap = map;
  _envelope = e;
  _inverse = inverse;
}


void OutsideBoundsRemover::removeWays(shared_ptr<OsmMap> map, const Envelope& e, bool inverse)
{
  OutsideBoundsRemover obr(map, e, inverse);
  return obr.removeWays();
}

void OutsideBoundsRemover::removeWays()
{
  LOG_INFO("Removing ways outside bounds...");

  const WayMap ways = _inputMap->getWays();
  for (WayMap::const_iterator it = ways.begin(); it != ways.end(); ++it)
  {
    const shared_ptr<const Way>& w = it->second;
    Envelope e = w->getEnvelopeInternal(_inputMap);

    if (_inverse == false)
    {
      // if the way envelope doesn't intersect the given envelope.
      if (_envelope.intersects(e) == false)
      {
        RemoveWayOp::removeWay(_inputMap, w->getId());
      }
    }
    else
    {
      // if the way envelope intersects the given envelope.
      if (_envelope.intersects(e) == true)
      {
        RemoveWayOp::removeWay(_inputMap, w->getId());
      }
    }
  }

}

}
