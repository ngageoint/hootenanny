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

#ifndef NODETOWAYMAP_H
#define NODETOWAYMAP_H

// Boost
#include <boost/shared_ptr.hpp>

// Standard
#include <map>
#include <set>
#include <string>

namespace hoot
{
  using namespace boost;
  using namespace std;

  class OsmMap;
  class Way;

/**
 * Traverses an OsmMap and creates a map from node id to a set of way IDs. This is handy when
 * trying to find intersections.
 */
class NodeToWayMap : public map<long, set<long> >
{
public:

  static std::string className() { return "hoot::NodeToWayMap"; }

  static unsigned int logWarnCount;

  NodeToWayMap(const OsmMap& map);

  void addWay(boost::shared_ptr<const Way> w);

  /**
   * Returns all the ways that use the given node ID. If the given node ID isn't found an empty
   * set is returned.
   */
  const set<long>& getWaysByNode(long nid) const;

  /**
   * This function assumes that the nodes in the way haven't changed since it was last added.
   */
  void removeWay(boost::shared_ptr<const Way> w);

  bool validate(const OsmMap& map);

private:

  set<long> _emptySet;
};

}

#endif // NODETOWAYMAP_H
