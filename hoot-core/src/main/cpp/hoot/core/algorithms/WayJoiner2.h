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

#ifndef WAYJOINER2_H
#define WAYJOINER2_H

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/algorithms/WayJoiner.h>

namespace hoot
{

/**
 * An experimental way joiner implemented to work with Attribute Conflation and a possible
 * predecessor to WayJoiner1.  If we end up keeping WayJoiner1 and WayJoiner2 separate, then
 * possibly a shared base class could be made to reduce some of the redundant code between them.
 */
class WayJoiner2 : public WayJoiner
{
public:

  static std::string className() { return "hoot::WayJoiner2"; }

  typedef enum
  {
    ParentFirst,
    ParentLast,
    ShareFirstNode
  } JoinAtNodeMergeType;

  WayJoiner2();

  /**
   * @see WayJoiner
   */
  virtual void join(const OsmMapPtr& map);

  /**
   * @see WayJoiner
   */
  virtual void setLeavePid(bool leavePid) { _leavePid = leavePid; }
  virtual bool getLeavePid() const { return _leavePid; }

  virtual int getNumJoined() const { return _numJoined; }

private:

  /** Debugging flag to leave parent IDs intact for output */
  bool _leavePid;
  /** Pointer to the map to work on */
  OsmMapPtr _map;
  int _numJoined;

  /**
   * @brief joinParentChild Simplest joining algorithm that joins a way with a parent id to that
   * parent
   */
  void _joinParentChild();

  /**
   * @brief joinSiblings Joining algorithm that searches for all ways that have the same parent id,
   *    attempts to order them into adjoining way order, then joins them
   */
  void _joinSiblings();

  /**
   * @brief joinAtNode Joining algorithm that searches all ways that have a parent id and tries
   *    to join them with adjacent ways that have the same tags
   */
  void _joinAtNode();

  /**
   * @brief areJoinable Check the status of the ways to see if they are compatible when joining at
   * a node; essentially UNKNOWN1 and UNKNOWN2 ways aren't joinable together
   */
  bool _areJoinable(const WayPtr& w1, const WayPtr& w2);

  /**
   * @brief resetParents Resets parent id for all ways after joining operation has completed
   *    does nothing if _leavePid is true
   */
  void _resetParents();

  /**
   * @brief rejoinSiblings Function that rejoins ways that all have the same parent id
   *    but that parent way doesn't exist
   * @param way_ids Deque of sorted ways to join
   */
  void _rejoinSiblings(std::deque<long>& way_ids);

  /**
   * @brief joinWays Function to rejoin two ways
   * @param parent Way that is modified to include the child way
   * @param child Way that will be merged into the parent and then deleted
   * @return true if the two ways were joined; false otherwise
   */
  bool _joinWays(const WayPtr& parent, const WayPtr& child);

  /*
   * A more aggressive joining approach that tries to join up any unnamed generic roads in the
   * output with a non-generic named road.  Best to be run after other joining routines and will
   * likely result in some undesirable joins.
   */
  void _joinUnsplitWaysAtNode();

  void _determineKeeperFeature(WayPtr parent, WayPtr child, WayPtr& keeper, WayPtr& toRemove);

  bool _handleOneWayStreetReversal(WayPtr wayWithTagsToKeep, ConstWayPtr wayWithTagsToLose);

  double _getTotalLengthFromTags(const Tags& tags1, const Tags& tags2) const;
};

}

#endif  //  WAYJOINER2_H
