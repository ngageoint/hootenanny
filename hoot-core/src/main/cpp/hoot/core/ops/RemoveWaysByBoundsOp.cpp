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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "RemoveWaysByBoundsOp.h"

// GEOS
#include <geos/geom/LineString.h>

// Hoot
#include <hoot/core/elements/Way.h>
#include <hoot/core/ops/RemoveWayOp.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/GeometryUtils.h>


using namespace geos::geom;

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, RemoveWaysByBoundsOp)

RemoveWaysByBoundsOp::RemoveWaysByBoundsOp() :
_inverse(false)
{
}

RemoveWaysByBoundsOp::RemoveWaysByBoundsOp(const Envelope& e, const bool inverse) :
_envelope(e),
_inverse(inverse)
{
}

void RemoveWaysByBoundsOp::setConfiguration(const Settings& conf)
{
  ConfigOptions opts(conf);
  _inverse = opts.getWayRemoverInvertBounds();
  _envelope = GeometryUtils::envelopeFromConfigString(opts.getWayRemoverBounds());
}

void RemoveWaysByBoundsOp::apply(boost::shared_ptr<OsmMap>& map)
{
  _numAffected = 0;
  _map = map;

  _removeWays();
}

void RemoveWaysByBoundsOp::_removeWays()
{
  const WayMap ways = _map->getWays();
  for (WayMap::const_iterator it = ways.begin(); it != ways.end(); ++it)
  {
    const ConstWayPtr& w = it->second;
    Envelope e = w->getEnvelopeInternal(_map);

    if (_inverse == false)
    {
      // if the way envelope doesn't intersect the given envelope.
      if (_envelope.intersects(e) == false)
      {
        RemoveWayOp::removeWay(_map, w->getId());
        _numAffected++;
      }
    }
    else
    {
      // if the way envelope intersects the given envelope.
      if (_envelope.intersects(e) == true)
      {
        RemoveWayOp::removeWay(_map, w->getId());
        _numAffected++;
      }
    }
  }
}

QString RemoveWaysByBoundsOp::getInitStatusMessage() const
{
  return "Removing ways in relation to the given bounds...";
}

QString RemoveWaysByBoundsOp::getCompletedStatusMessage() const
{
  QString msg = "Removed " + QString::number(_numAffected) + " ways ";
  if (_inverse)
  {
    msg += "outside";
  }
  else
  {
    msg += "inside";
  }
  msg += " bounds: " + QString::fromStdString(_envelope.toString());
  return msg;
}

}
