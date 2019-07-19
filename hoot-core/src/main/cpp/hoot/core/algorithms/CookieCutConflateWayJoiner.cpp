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
 * @copyright Copyright (C) 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "CookieCutConflateWayJoiner.h"

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/ops/ReplaceElementOp.h>
#include <hoot/core/io/OsmMapWriterFactory.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(WayJoiner, CookieCutConflateWayJoiner)

CookieCutConflateWayJoiner::CookieCutConflateWayJoiner() :
WayJoinerAdvanced::WayJoinerAdvanced()
{
  _leavePid = true;
}

void CookieCutConflateWayJoiner::joinWays(const OsmMapPtr& map)
{
  CookieCutConflateWayJoiner().join(map);
}

bool CookieCutConflateWayJoiner::_areJoinable(const WayPtr& w1, const WayPtr& w2) const
{
  // We'll join up anything, as long as its not invalid. TODO: is this right?
  return w1->getStatus() != Status::Invalid && w2->getStatus() != Status::Invalid;
  //return (w1->getStatus() == Status::Unknown1 && w2->getStatus() == Status::Unknown2) ||
         //(w2->getStatus() == Status::Unknown1 && w1->getStatus() == Status::Unknown2);
}

void CookieCutConflateWayJoiner::_determineKeeperFeatureForTags(
  WayPtr parent, WayPtr child, WayPtr& keeper, WayPtr& toRemove) const
{
  // We always want to keep unknown2, which is the dough ways being joined up with the cookie cut
  // replacement ways. TODO: is this right?
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

void CookieCutConflateWayJoiner::_determineKeeperFeatureForId(
  WayPtr parent, WayPtr child, WayPtr& keeper, WayPtr& toRemove) const
{
  _determineKeeperFeatureForTags(parent, child, keeper, toRemove);
}

long CookieCutConflateWayJoiner::_getPid(const ConstWayPtr& way) const
{
  LOG_VART(way->hasPid());
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

void CookieCutConflateWayJoiner::join(const OsmMapPtr& map)
{
  WayJoinerAdvanced::join(map);

  // If anything left has a PID on it, let's make that PID its ID. TODO: This doesn't seem right...
  WayMap ways = _map->getWays();
  QSet<long> pidsUsed;
  for (WayMap::const_iterator it = ways.begin(); it != ways.end(); ++it)
  {
    WayPtr way = it->second;
    const long pid = _getPid(way);
    if (pid != WayData::PID_EMPTY && pid > 0 && !pidsUsed.contains(pid))
    {
      LOG_TRACE("Setting id from pid: " << pid << " on: " << way->getElementId());
      ElementPtr newWay(way->clone());
      newWay->setId(pid);
      _map->replace(way, newWay);
      pidsUsed.insert(pid);
    }
  }
  OsmMapWriterFactory::writeDebugMap(map, "after-cookie-cut-way-joiner-pid-set");
}

}
