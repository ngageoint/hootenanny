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

#ifndef WAYJOINER1_H
#define WAYJOINER1_H

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/algorithms/WayJoiner.h>

namespace hoot
{

/**
 * Joins ways back together that were split during pre-conflation cleaning or during matching.
 */
class WayJoiner1 : public WayJoiner
{
public:

  static std::string className() { return "hoot::WayJoiner1"; }

  WayJoiner1();

  /**
   * @see WayJoiner
   */
  static void joinWays(const OsmMapPtr& map);

  /**
   * @brief join Runs all joining algorithms
   */
  virtual void join(const OsmMapPtr& map) override;

protected:

  /**
   * @brief joinParentChild Simplest joining algorithm that joins a way with a parent id to that parent
   */
  virtual void _joinParentChild() override;

  /**
   * @brief joinSiblings Joining algorithm that searches for all ways that have the same parent id,
   *    attempts to order them into adjoining way order, then joins them
   */
  virtual void _joinSiblings() override;

  /**
   * @brief joinAtNode Joining algorithm that searches all ways that have a parent id and tries
   *    to join them with adjacent ways that have the same tags
   */
  virtual void _joinAtNode() override;

  /**
   * @brief areJoinable Check the status of the ways to see if they are compatible when joining at a node
   *    essentially UNKNOWN1 and UNKNOWN2 ways aren't joinable together
   */
  virtual bool _areJoinable(const WayPtr& w1, const WayPtr& w2) override;

  /**
   * @brief resetParents Resets parent id for all ways after joining operation has completed
   *    does nothing if _leavePid is true
   */
  virtual void _resetParents() override;

  /**
   * @brief rejoinSiblings Function that rejoins ways that all have the same parent id
   *    but that parent way doesn't exist
   * @param way_ids Deque of sorted ways to join
   */
  virtual void _rejoinSiblings(std::deque<long>& way_ids) override;

  /**
   * @brief joinWays Function to rejoin two ways
   * @param parent Way that is modified to include the child way
   * @param child Way that will be merged into the parent and then deleted
   */
  virtual bool _joinWays(const WayPtr& parent, const WayPtr& child) override;

};

}

#endif  //  WAYJOINER1_H
