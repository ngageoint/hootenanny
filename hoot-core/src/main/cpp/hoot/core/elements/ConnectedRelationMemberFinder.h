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
 * @copyright Copyright (C) 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef CONNECTED_RELATION_MEMBER_FINDER_H
#define CONNECTED_RELATION_MEMBER_FINDER_H

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/elements/ConstOsmMapConsumer.h>

namespace hoot
{

/**
 * Finds instances where way members across different relations are connected
 */
class ConnectedRelationMemberFinder : public ConstOsmMapConsumer
{

public:

  ConnectedRelationMemberFinder() = default;
  virtual ~ConnectedRelationMemberFinder() = default;

  /**
   * Determines if any way from one relation is connected to a way in another relation
   *
   * @param relation1 first relation to examine
   * @param relation2 second relation to examine
   * @return true if connected ways are found; false otherwise
   */
  bool haveConnectedWayMembers(
    const ConstRelationPtr& relation1, const ConstRelationPtr& relation2) const;

  /**
   * @see ConstOsmMapConsumer
   */
  virtual void setOsmMap(const OsmMap* map) { _map = map->shared_from_this(); }

private:

  ConstOsmMapPtr _map;
};

}

#endif // CONNECTED_RELATION_MEMBER_FINDER_H
