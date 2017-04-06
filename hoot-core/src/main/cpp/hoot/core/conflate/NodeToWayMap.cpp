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

#include "NodeToWayMap.h"

// Hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/util/Log.h>

namespace hoot
{

unsigned int NodeToWayMap::logWarnCount = 0;

NodeToWayMap::NodeToWayMap(const OsmMap& map)
{
  const WayMap& ways = map.getWays();
  for (WayMap::const_iterator it = ways.begin(); it != ways.end(); ++it)
  {
    boost::shared_ptr<Way> w = it->second;
    addWay(w);
  }
}

void NodeToWayMap::addWay(boost::shared_ptr<const Way> w)
{
  const std::vector<long>& nodes = w->getNodeIds();
  for (size_t i = 0; i < nodes.size(); i++)
  {
    (*this)[nodes[i]].insert(w->getId());
  }
}

const set<long>& NodeToWayMap::getWaysByNode(long nid) const
{
  NodeToWayMap::const_iterator it = this->find(nid);
  if (it == this->end())
  {
    return _emptySet;
  }
  else
  {
    return it->second;
  }
}

void NodeToWayMap::removeWay(boost::shared_ptr<const Way> w)
{
  const std::vector<long>& nodes = w->getNodeIds();
  for (size_t i = 0; i < nodes.size(); i++)
  {
    set<long>& s = (*this)[nodes[i]];
    s.erase(w->getId());
    // if we just removed the last node, then remove the entry from the map.
    if (s.size() == 0)
    {
      erase(nodes[i]);
    }
  }
}

bool NodeToWayMap::validate(const OsmMap& map)
{
  bool result = true;
  // verify that the map contains everything that it should.
  const WayMap& ways = map.getWays();
  for (WayMap::const_iterator it = ways.begin(); it != ways.end(); ++it)
  {
    const boost::shared_ptr<Way>& w = it->second;
    assert(w->getId() != 0);
    const std::vector<long>& nodes = w->getNodeIds();
    for (size_t i = 0; i < nodes.size(); i++)
    {
#     ifndef NDEBUG
        const set<long>& s = (*this)[nodes[i]];
        assert(s.find(w->getId()) != s.end());
#     endif
    }
  }

  // verify that the map doesn't contain anything it shouldn't.
  for (const_iterator it = begin(); it != end(); it++)
  {
    long nid = it->first;
    const set<long>& s = it->second;

    // this assumption causes problems when a OsmMap is incomplete (think map reduce)
    //assert(map.containsNode(nid) || s.size() == 0);

    for (set<long>::const_iterator it = s.begin(); it != s.end(); it++)
    {
      assert(*it != 0);
      if (map.containsWay(*it) == false)
      {
        if (logWarnCount < ConfigOptions().getLogWarnMessageLimit())
        {
          LOG_WARN(QString("Map does not contain way: %1 ref by node: %2").arg(*it).arg(nid));
        }
        else if (logWarnCount == ConfigOptions().getLogWarnMessageLimit())
        {
          LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
        }
        logWarnCount++;
        result = false;
      }
    }
  }

  return result;
}

}
