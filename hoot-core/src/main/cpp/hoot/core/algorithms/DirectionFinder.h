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
 * @copyright Copyright (C) 2015, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef DIRECTIONFINDER_H
#define DIRECTIONFINDER_H

#include <hoot/core/elements/OsmMap.h>

namespace hoot
{

class DirectionFinder
{
public:

  /**
   * Returns true if these lines are generally pointed in a similar direction.
   */
  static bool isSimilarDirection(
    const ConstOsmMapPtr& map, const ConstWayPtr& w1, const ConstWayPtr& w2);

  /**
   * Returns true if these lines are generally pointed in a similar direction.
   *
   * This is an alternative implementation written for Attribute Conflation, which may possibly
   * replace the original at some point after extensive testing.
   *
   * @param map map containing the ways for which to compare direction
   * @param way1 the first way to compare direction for
   * @param way2 the second way to compare direction for
   * @return true if both ways are running in a similar direction; false otherwise
   * @see direction.finder.angle.threshold
   * @todo After recent fixes, we may be able to have Attribute Conflation switch back to the
   * original isSimilarDirection implementation.
   */
  static bool isSimilarDirection2(const ConstOsmMapPtr& map, ConstWayPtr way1, ConstWayPtr way2);

private:

  static double _getAngleDiff(const ConstOsmMapPtr& map, ConstWayPtr way1, ConstWayPtr way2);
};

}

#endif // DIRECTIONFINDER_H
