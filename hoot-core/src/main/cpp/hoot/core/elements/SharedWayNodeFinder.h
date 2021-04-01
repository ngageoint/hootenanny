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
 * @copyright Copyright (C) 2020, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef SHARED_WAY_NODE_FINDER_H
#define SHARED_WAY_NODE_FINDER_H

// Hoot
#include <hoot/core/elements/Way.h>

// Qt
#include <QSet>

namespace hoot
{

/**
 * Finds shared nodes in ways
 */
class SharedWayNodeFinder
{

public:

  /**
   * Returns shared node IDs among two ways
   *
   * @param way1 first way to examine
   * @param way2 second way to examine
   * @return the IDs of all nodes shared by the input ways or an empty collection if no nodes are
   * shared
   */
  static QSet<long> getSharedWayNodes(const ConstWayPtr& way1, const ConstWayPtr& way2);

  /**
   * Determines if two ways share at least one node at their ends
   *
   * @param way1 first way to examine
   * @param way2 second way to examine
   * @param sameDirection if true, the input ways are assumed to point in the same direction; if
   * false they are assumed to point in opposite directions
   * @return
   */
  static bool waysShareEndNode(
    const ConstWayPtr& way1, const ConstWayPtr& way2, const bool sameDirection = true);
  
};

}

#endif // SHARED_WAY_NODE_FINDER_H
