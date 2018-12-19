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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "WayTagMergeManipulation.h"

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/ops/RemoveWayOp.h>

using namespace std;

namespace hoot
{

WayTagMergeManipulation::WayTagMergeManipulation(long leftId, long rightId,
  ConstOsmMapPtr map, Meters minSplitSize) :
  WayMergeManipulation(leftId, rightId, map, minSplitSize)
{
}

void WayTagMergeManipulation::applyManipulation(OsmMapPtr wm,
  set<ElementId>& impactedElements, set<ElementId>& newElements) const
{
  impactedElements.clear();
  impactedElements.insert(ElementId::way(_left));
  impactedElements.insert(ElementId::way(_right));

  newElements.clear();

  WayPtr w1 = wm->getWay(_left);
  WayPtr w2 = wm->getWay(_right);

  // make sure w1 is the Unknown1
  if (w1->getStatus() == Status::Unknown2)
  {
    swap(w1, w2);
  }

  w1->getTags().addTags(w2->getTags());
  w1->setStatus(Status::Conflated);

  RemoveWayOp::removeWay(wm, w2->getId());
}

const set<long>& WayTagMergeManipulation::getImpactedWayIds(ConstOsmMapPtr /*map*/) const
{
  _impactedWays.clear();
  _impactedWays.insert(_left);
  _impactedWays.insert(_right);
  return _impactedWays;
}

}
