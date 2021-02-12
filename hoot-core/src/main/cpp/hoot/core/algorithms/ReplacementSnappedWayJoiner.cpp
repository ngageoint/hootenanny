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
 * @copyright Copyright (C) 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#include "ReplacementSnappedWayJoiner.h"

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(WayJoiner, ReplacementSnappedWayJoiner)

ReplacementSnappedWayJoiner::ReplacementSnappedWayJoiner() :
WayJoinerAdvanced::WayJoinerAdvanced()
{
  _leavePid = true;
  _callingClass = className();
}

ReplacementSnappedWayJoiner::ReplacementSnappedWayJoiner(
  const QMap<ElementId, long>& refIdToVersionMappings) :
WayJoinerAdvanced::WayJoinerAdvanced(),
_refIdToVersionMappings(refIdToVersionMappings)
{
  _leavePid = true;
  _callingClass = className();
}

bool ReplacementSnappedWayJoiner::_areJoinable(const WayPtr& w1, const WayPtr& w2) const
{
  // We'll join up anything, as long as its not invalid.
  return w1->getStatus() != Status::Invalid && w2->getStatus() != Status::Invalid;
}

void ReplacementSnappedWayJoiner::_determineKeeperFeatureForTags(
  WayPtr parent, WayPtr child, WayPtr& keeper, WayPtr& toRemove) const
{
  // We always want to keep unknown2, which is the dough ways being joined up with the cookie cut
  // replacement ways.
  if (parent->getStatus() == Status::Unknown2)
  {
    keeper = parent;
    toRemove = child;
  }
  else if (child->getStatus() == Status::Unknown2)
  {
    keeper = child;
    toRemove = parent;
  }
  else
  {
    keeper = parent;
    toRemove = child;
  }
}

void ReplacementSnappedWayJoiner::_determineKeeperFeatureForId(
  WayPtr parent, WayPtr child, WayPtr& keeper, WayPtr& toRemove) const
{
  _determineKeeperFeatureForTags(parent, child, keeper, toRemove);
}

bool ReplacementSnappedWayJoiner::_hasPid(const ConstWayPtr& way) const
{
  return way->hasPid() || way->getTags().contains(MetadataTags::HootSplitParentId());
}

long ReplacementSnappedWayJoiner::_getPid(const ConstWayPtr& way) const
{
  long pid = WayData::PID_EMPTY;
  if (way->hasPid())
  {
    pid = way->getPid();
  }
  else if (way->getTags().contains(MetadataTags::HootSplitParentId()))
  {
    pid = way->getTags()[MetadataTags::HootSplitParentId()].toLong();
  }
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
  WayMap ways = _map->getWays();
  QSet<long> pidsUsed;
  for (WayMap::const_iterator it = ways.begin(); it != ways.end(); ++it)
  {
    WayPtr way = it->second;
    LOG_VART(way->getElementId());
    const long pid = _getPid(way);
    LOG_VART(pid);
    if (pid != WayData::PID_EMPTY && pid > 0 && !pidsUsed.contains(pid))
    {
      LOG_TRACE("Setting id from pid: " << pid << " on: " << way->getElementId());
      ElementPtr newWay(way->clone());
      newWay->setId(pid);
      const ElementId parentElementId = ElementId(ElementType::Way, pid);
      if (!_refIdToVersionMappings.contains(parentElementId))
      {
        LOG_WARN("No version mapping for ref way: " << parentElementId);
      }
      else
      {
        newWay->setVersion(_refIdToVersionMappings[parentElementId]);
      }
      LOG_VART(newWay->getVersion());
      LOG_VART(newWay->getStatus());
      _joinedWayIdMappings[way->getId()] = newWay->getId();
      _map->replace(way, newWay);
      pidsUsed.insert(pid);
    }
  }
  OsmMapWriterFactory::writeDebugMap(map, "after-replacement-snapped-way-joiner-pid-set");
}

}
