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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef FINDNODESINWAYFACTORY_H
#define FINDNODESINWAYFACTORY_H

#include <hoot/core/geometry/GeometryToElementConverter.h>

namespace hoot
{

/**
 * @brief The FindNodesInWayFactory class is used by the GeometryUtils class to create a new way
 * using only nodes from the old way, or by creating new nodes.
 *
 * If new nodes are created then they are also used before creating another node.
 */
class FindNodesInWayFactory : public GeometryToElementConverter::NodeFactory
{
public:

  static QString className() { return "hoot::FindNodesInWayFactory"; }

  FindNodesInWayFactory() = default;
  FindNodesInWayFactory(const ConstWayPtr& w);
  ~FindNodesInWayFactory() = default;

  void addWay(const ConstWayPtr& w);

  NodePtr createNode(
    const OsmMapPtr& map, const geos::geom::Coordinate& c, Status s, double circularError) override;

private:

  static int logWarnCount;

  const ConstWayPtr _way;
  std::set<long> _nodesToSearch;
};

}

#endif // FINDNODESINWAYFACTORY_H
