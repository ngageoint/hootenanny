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
 * @copyright Copyright (C) 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */

#include "ReplacementSnappedWayJoiner.h"

// Hoot
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/util/Factory.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(WayJoiner, ReplacementSnappedWayJoiner)

ReplacementSnappedWayJoiner::ReplacementSnappedWayJoiner()
  : WayJoinerAdvanced::WayJoinerAdvanced()
{
  _leavePid = true;
  _callingClass = className();
}

ReplacementSnappedWayJoiner::ReplacementSnappedWayJoiner(const QMap<ElementId, long>& refIdToVersionMappings)
  : WayJoinerAdvanced::WayJoinerAdvanced(),
    _refIdToVersionMappings(refIdToVersionMappings)
{
  _leavePid = true;
  _callingClass = className();
}

bool ReplacementSnappedWayJoiner::_hasPid(const ConstWayPtr& way) const
{
  return way->hasPid() || way->getTags().contains(MetadataTags::HootSplitParentId());
}

long ReplacementSnappedWayJoiner::_getPid(const ConstWayPtr& way) const
{
  long pid = WayData::PID_EMPTY;
  if (way->hasPid())
    pid = way->getPid();
  else if (way->getTags().contains(MetadataTags::HootSplitParentId()))
    pid = way->getTags()[MetadataTags::HootSplitParentId()].toLong();
  return pid;
}

void ReplacementSnappedWayJoiner::join(const OsmMapPtr& map)
{
  LOG_DEBUG("Joining ways...");

  _leavePid = true;

  WayJoinerAdvanced::join(map);

  if (_refIdToVersionMappings.isEmpty())
  {
    LOG_WARN("No version mappings exist for ref ways.");
  }

  // If anything left has a PID on it, let's make that PID its ID. This doesn't seem like a great
  // way to do it and problems could be encountered in the future where more than one split way
  // inside of the bounds had the same parent...haven't seen that happen yet, so will go with this
  // for now.
  const WayMap& ways = _map->getWays();
  for (auto it = ways.begin(); it != ways.end(); ++it)
  {
    WayPtr way = it->second;
    LOG_VART(way->getElementId());
    const long pid = _getPid(way);
    LOG_VART(pid);
  }
  OsmMapWriterFactory::writeDebugMap(map, className(), "after-pid-set");
}

}
