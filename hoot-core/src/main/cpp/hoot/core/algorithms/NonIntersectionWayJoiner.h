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
 * @copyright Copyright (C) 2021 Maxar (http://www.maxar.com/)
 */

#ifndef NON_INTERSECTION_WAY_JOINER_H
#define NON_INTERSECTION_WAY_JOINER_H

// Hoot
#include <hoot/core/algorithms/WayJoiner.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/elements/Way.h>

namespace hoot
{

/**
 * @brief The NonIntersectionWayJoiner class is a way joiner to join continuous ways but not at way
 * intersections where more than two ways meet.
 */
class NonIntersectionWayJoiner : public WayJoiner
{
public:

  static QString className() { return "hoot::NonIntersectionWayJoiner"; }

  NonIntersectionWayJoiner() = default;
  ~NonIntersectionWayJoiner() = default;

  /**
   * @brief joinWays is a static method to join all joinable ways using NonIntersectionWayJoiner.
   */
  static void joinWays(const OsmMapPtr& map);

  QString getDescription() const override
  { return "Joins ways split between actual intersections."; }
  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }

protected:

  /**
   * @brief joinAtNode Joins
   */
  void _joinAtNode() override;

  /**
   *  Overridding these functions to make them do nothing when they are called
   */
  void _joinParentChild() override { }
  void _joinSiblings() override { }
  void _rejoinSiblings(std::deque<long>& /*way_ids*/) override { }
};

}

#endif  //  WAYJOINER_H
