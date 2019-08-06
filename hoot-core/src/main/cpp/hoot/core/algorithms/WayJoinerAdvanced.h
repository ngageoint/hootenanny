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

#ifndef WAYJOINER_ADVANCED_H
#define WAYJOINER_ADVANCED_H

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/algorithms/WayJoiner.h>

namespace hoot
{

/**
 * An experimental way joiner implemented to work with Attribute Conflation and a possible
 * replacement for WayJoinerBasic.
 */
class WayJoinerAdvanced : public WayJoiner
{
public:

  static std::string className() { return "hoot::WayJoinerAdvanced"; }

  typedef enum
  {
    ParentFirst,
    ParentLast,
    ShareFirstNode
  } JoinAtNodeMergeType;

  WayJoinerAdvanced();

  /**
   * Static method to join all joinable ways using WayJoinerAdvanced
   */
  static void joinWays(const OsmMapPtr& map);

  /**
   * @see WayJoiner
   */
  virtual void join(const OsmMapPtr& map) override;

  /**
   * @see ApiEntityInfo
   */
  virtual QString getDescription() const override
  { return "Extends WayJoinerBasic with additional join pre-conditions."; }

protected:

  // identifies the way in the join pair whose ID was kep after the join
  WayPtr _wayKeptAfterJoin;
  // the name of the class implementation being executed
  QString _callingClass;

  virtual void _joinParentChild() override;
  virtual void _joinAtNode() override;
  virtual void _rejoinSiblings(std::deque<long>& way_ids) override;
  virtual bool _joinWays(const WayPtr& parent, const WayPtr& child) override;

  /*
   * Determines which feature's tags are kept during a way join
   */
  virtual void _determineKeeperFeatureForTags(WayPtr parent, WayPtr child, WayPtr& keeper,
                                              WayPtr& toRemove) const;

  /*
   * Determine which feature's ID is kept during a way join
   */
  virtual void _determineKeeperFeatureForId(WayPtr parent, WayPtr child, WayPtr& keeper,
                                            WayPtr& toRemove) const;

  /*
   * These accessors have been added to appease the ReplacementSnappedWayJoiner used by
   * ChangesetReplacementCreator. Without them there is difficulty in tracking provenance of
   * modified ways. I believe with some further work they eventually can go away (see related
   * method in UnconnectedWaySnapper as well.
   */
  virtual bool _hasPid(const ConstWayPtr& way) const;
  virtual long _getPid(const ConstWayPtr& way) const;

private:

  // name of the method calling into _jointAtNode
  QString _callingMethod;

  /**
   * A more aggressive joining approach that tries to join up any unnamed generic roads in the
   * output with a non-generic named road.  Best to be run after other joining routines and will
   * likely result in some undesirable joins.
   */
  void _joinUnsplitWaysAtNode();

  bool _handleOneWayStreetReversal(WayPtr wayWithTagsToKeep, ConstWayPtr wayWithTagsToLose);

  double _getTotalLengthFromTags(const Tags& tags1, const Tags& tags2) const;
};

}

#endif  //  WAYJOINER_ADVANCED_H
