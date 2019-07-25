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
 * TODO
 */
class ReplacementSnappedWayJoiner : public WayJoinerAdvanced
{
public:

  static std::string className() { return "hoot::ReplacementSnappedWayJoiner"; }

  // TODO: can probably get rid of the default constructor at some point
  ReplacementSnappedWayJoiner();
  ReplacementSnappedWayJoiner(const QMap<ElementId, long>& refIdToVersionMappings);

  /**
   * @see WayJoiner
   */
  virtual void join(const OsmMapPtr& map) override;

protected:

  /**
   * TODO
   *
   * @param w1
   * @param w2
   * @return
   */
  virtual bool _areJoinable(const WayPtr& w1, const WayPtr& w2) const override;

  /**
   * TODO
   *
   * @param parent
   * @param child
   * @param keeper
   * @param toRemove
   */
  virtual void _determineKeeperFeatureForTags(WayPtr parent, WayPtr child, WayPtr& keeper,
                                              WayPtr& toRemove) const override;

  /**
   * TODO
   *
   * @param parent
   * @param child
   * @param keeper
   * @param toRemove
   */
  virtual void _determineKeeperFeatureForId(WayPtr parent, WayPtr child, WayPtr& keeper,
                                            WayPtr& toRemove) const override;

protected:

  virtual bool _hasPid(const ConstWayPtr& way) const;
  virtual long _getPid(const ConstWayPtr& way) const;

private:

  QMap<ElementId, long> _refIdToVersionMappings;
};

}

#endif  //  REPLACEMENT_SNAPPED_WAY_JOINER_H
