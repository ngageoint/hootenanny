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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#include "WaySplitterOp.h"

// hoot
#include <hoot/core/algorithms/splitter/WaySplitter.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Settings.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, WaySplitterOp)

WaySplitterOp::WaySplitterOp()
{
  setConfiguration(conf());
}

void WaySplitterOp::apply(std::shared_ptr<OsmMap>& map)
{
  _numAffected = 0;

  LOG_INFO("splitting with max length: " << _maxLength);
  MapProjector::projectToPlanar(map);

  // use a copy of the map since we'll be making changes
  const WayMap ways = map->getWays();
  for (auto it = ways.begin(); it != ways.end(); ++it)
  {
    WaySplitter::split(map, map->getWay(it->first), _maxLength);
    _numAffected++;
  }
}

void WaySplitterOp::setConfiguration(const Settings& conf)
{
  _maxLength = ConfigOptions(conf).getWaySplitterMaxLength();
  LOG_VARD(_maxLength);
}

}
