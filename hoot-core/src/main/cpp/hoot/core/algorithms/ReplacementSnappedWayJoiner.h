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
 * @copyright Copyright (C) 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef REPLACEMENT_SNAPPED_WAY_JOINER_H
#define REPLACEMENT_SNAPPED_WAY_JOINER_H

// Hoot
#include <hoot/core/algorithms/WayJoinerAdvanced.h>

namespace hoot
{

/**
 * A way joiner to be used in the changeset replacement generation workflow to join up ways
 * snapped with UnconnectedWaySnapper.
 */
class ReplacementSnappedWayJoiner : public WayJoinerAdvanced
{
public:

  static std::string className() { return "hoot::ReplacementSnappedWayJoiner"; }

  ReplacementSnappedWayJoiner();
  ReplacementSnappedWayJoiner(const QMap<ElementId, long>& refIdToVersionMappings);
  virtual ~ReplacementSnappedWayJoiner() = default;

  /**
   * @see WayJoiner
   */
  virtual void join(const OsmMapPtr& map) override;

  /**
   * @see ApiEntityInfo
   */
  virtual QString getDescription() const override
  { return "Way joiner for use after way snapping when generating replacement changesets."; }

protected:

  /*
   * @see WayJoiner
   */
  virtual bool _areJoinable(const WayPtr& w1, const WayPtr& w2) const override;

  /*
   * @see WayJoinerAdvanced
   */
  virtual void _determineKeeperFeatureForTags(WayPtr parent, WayPtr child, WayPtr& keeper,
                                              WayPtr& toRemove) const override;

  /*
   * @see WayJoinerAdvanced
   */
  virtual void _determineKeeperFeatureForId(WayPtr parent, WayPtr child, WayPtr& keeper,
                                            WayPtr& toRemove) const override;

  /*
   * @see WayJoinerAdvanced
   */
  virtual bool _hasPid(const ConstWayPtr& way) const override;
  /*
   * @see WayJoinerAdvanced
   */
  virtual long _getPid(const ConstWayPtr& way) const override;

private:

  // keeps track of element changeset versions
  QMap<ElementId, long> _refIdToVersionMappings;
};

}

#endif  //  REPLACEMENT_SNAPPED_WAY_JOINER_H
