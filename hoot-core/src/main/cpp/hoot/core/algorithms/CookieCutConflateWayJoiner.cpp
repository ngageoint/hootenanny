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

namespace hoot
{

HOOT_FACTORY_REGISTER(WayJoiner, CookieCutConflateWayJoiner)

CookieCutConflateWayJoiner::CookieCutConflateWayJoiner()
{
}

void CookieCutConflateWayJoiner::joinWays(const OsmMapPtr& map)
{
  CookieCutConflateWayJoiner wayJoiner;
  wayJoiner.join(map);
}

bool CookieCutConflateWayJoiner::_areJoinable(const WayPtr& w1, const WayPtr& w2) const
{
  // join anything up except invalid
  return w1->getStatus() != Status::Invalid && w2->getStatus() != Status::Invalid;
}

void CookieCutConflateWayJoiner::_determineKeeperFeatureForTags(WayPtr parent, WayPtr child,
                                                                WayPtr& keeper,
                                                                WayPtr& toRemove) const
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

void CookieCutConflateWayJoiner::_determineKeeperFeatureForId(WayPtr parent, WayPtr child,
                                                              WayPtr& keeper, WayPtr& toRemove) const
{
  _determineKeeperFeatureForTags(parent, child, keeper, toRemove);
}

}
