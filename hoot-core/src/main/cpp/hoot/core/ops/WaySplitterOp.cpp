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
#include "WaySplitterOp.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/algorithms/WaySplitter.h>
#include <hoot/core/util/Settings.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, WaySplitterOp)

WaySplitterOp::WaySplitterOp()
{
  setConfiguration(conf());
}

void WaySplitterOp::apply(boost::shared_ptr<OsmMap>& map)
{
  LOG_INFO("splitting with max length: " << _maxLength);
  MapProjector::projectToPlanar(map);

  // use a copy of the map since we'll be making changes
  const WayMap ways = map->getWays();
  for (WayMap::const_iterator it = ways.begin(); it != ways.end(); ++it)
  {
    WaySplitter::split(map, map->getWay(it->first), _maxLength);
  }
}

void WaySplitterOp::readObject(QDataStream& is)
{
  is >> _maxLength;
}

void WaySplitterOp::setConfiguration(const Settings& conf)
{
  _maxLength = ConfigOptions(conf).getWaySplitterMaxLength();
  LOG_INFO("default max length: " << _maxLength);
}

void WaySplitterOp::writeObject(QDataStream& os) const
{
  os << _maxLength;
}

}
