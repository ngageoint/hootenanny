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

#ifndef SHARED_WAY_NODE_FINDER_H
#define SHARED_WAY_NODE_FINDER_H

// Hoot
#include <hoot/core/elements/Way.h>

// Qt
#include <QSet>

namespace hoot
{

/**
 * TODO
 */
class SharedWayNodeFinder
{

public:

  /**
   * TODO
   *
   * @param way1
   * @param way2
   * @return
   */
  static QSet<long> getSharedWayNodes(const ConstWayPtr& way1, const ConstWayPtr& way2);

  /**
   * TODO
   *
   * @param way1
   * @param way2
   * @return
   */
  static bool waysShareNode(const ConstWayPtr& way1, const ConstWayPtr& way2);

  /**
   * TODO
   *
   * @param way1
   * @param way2
   * @param sameDirection
   * @return
   */
  static bool waysShareEndNode(
    const ConstWayPtr& way1, const ConstWayPtr& way2, const bool sameDirection = true);
  
};

}

#endif // SHARED_WAY_NODE_FINDER_H
