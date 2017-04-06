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
 * @copyright Copyright (C) 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef WAYSNAPMERGEMANIPULATION_H
#define WAYSNAPMERGEMANIPULATION_H

#include "WayMergeManipulation.h"

namespace hoot
{

/**
 * This manipulation will "snap" the Unknown2 way to the Unknown1 way. The Unknown1 way will not be
 * changed in any way (even its status). The Unknown2 way will be broken into smaller pieces as
 * needed and snapped to the Unknown1 way as appropriate.
 */
class WaySnapMergeManipulation : public WayMergeManipulation
{
public:

  WaySnapMergeManipulation(long leftId, long rightId, boost::shared_ptr<const OsmMap> map,
    Meters minSplitSize);

  virtual void applyManipulation(boost::shared_ptr<OsmMap> wm, set<ElementId>& impactedElements,
    set<ElementId>& newElements) const;

  virtual const set<long>& getImpactedWayIds(boost::shared_ptr<const OsmMap> map) const;

protected:

  /**
   * Snap the ends of snapee that match with either end point of middle to snapTo's end points.
   */
  void _snapEnds(WayPtr snapee, WayPtr middle, WayPtr snapTo) const;
};

}

#endif // WAYSNAPMERGEMANIPULATION_H
