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
#ifndef FINDNODESINWAYFACTORY_H
#define FINDNODESINWAYFACTORY_H

#include "GeometryConverter.h"

namespace hoot
{

/**
 * Used by the GeometryUtils class to create a new way using only nodes from the old way, or by
 * creating new nodes. If new nodes are created then they are also used before creating another
 * node.
 */
class FindNodesInWayFactory : public GeometryConverter::NodeFactory
{
public:

  FindNodesInWayFactory() {}

  FindNodesInWayFactory(const ConstWayPtr& w);

  void addWay(const ConstWayPtr& w);

  virtual boost::shared_ptr<Node> createNode(const boost::shared_ptr<OsmMap>& map, const Coordinate& c,
    Status s, double circularError);

private:

  const ConstWayPtr _way;
  set<long> _nodesToSearch;
};

}

#endif // FINDNODESINWAYFACTORY_H
