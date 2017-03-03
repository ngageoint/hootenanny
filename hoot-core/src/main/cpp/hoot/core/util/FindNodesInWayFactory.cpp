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

#include "FindNodesInWayFactory.h"

#include <hoot/core/util/Log.h>

namespace hoot
{

FindNodesInWayFactory::FindNodesInWayFactory(const ConstWayPtr& w)
{
  addWay(w);
}

void FindNodesInWayFactory::addWay(const ConstWayPtr& w)
{
  const std::vector<long>& nids = w->getNodeIds();
  _nodesToSearch.insert(nids.begin(), nids.end());
}

shared_ptr<Node> FindNodesInWayFactory::createNode(const shared_ptr<OsmMap>& map,
                                                   const Coordinate& c, Status s,
                                                   double circularError)
{
  long result = std::numeric_limits<long>::max();

  for (set<long>::const_iterator it = _nodesToSearch.begin(); it != _nodesToSearch.end(); ++it)
  {
    long nid = *it;
    shared_ptr<const Node> n = map->getNode(nid);
    if (n->toCoordinate() == c)
    {
      // if there are multiple corresponding nodes, throw an exception.
      if (result != std::numeric_limits<long>::max() && result != nid)
      {
        LOG_ERROR("" << "Internal Error: Two nodes were found with the same coordinate.");
        LOG_VART(n);
      }
      result = nid;
    }
  }

  if (result == std::numeric_limits<long>::max())
  {
    shared_ptr<Node> n = shared_ptr<Node>(new Node(s, map->createNextNodeId(), c,
      circularError));
    map->addNode(n);
    result = n->getId();
    _nodesToSearch.insert(n->getId());
  }

  return map->getNode(result);
}

}
