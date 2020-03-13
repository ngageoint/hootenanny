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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "RemoveReviewsByEidOp.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/conflate/review/ReviewMarker.h>
#include <hoot/core/ops/RecursiveElementRemover.h>
#include <hoot/core/ops/RemoveElementByEid.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/StringUtils.h>

using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, RemoveReviewsByEidOp)

RemoveReviewsByEidOp::RemoveReviewsByEidOp(ElementId eid, bool clearAndRemoveElement) :
  _eid(eid),
  _clearAndRemove(clearAndRemoveElement)
{
}

void RemoveReviewsByEidOp::addElement(const ConstElementPtr& e)
{
  if (_eid.isNull())
  {
    _eid = e->getElementId();
  }
  else
  {
    throw IllegalArgumentException("Error adding element. Only one element can be added.");
  }
}

void RemoveReviewsByEidOp::apply(const OsmMapPtr &map)
{
  _numAffected = 0;

  if (_eid.isNull())
  {
    throw IllegalArgumentException("You must specify a valid element ID.");
  }

  // if from isn't in the map, there is nothing to do.
  if (map->containsElement(_eid) == false)
  {
    LOG_TRACE(_eid << " doesn't exist in map.");
    return;
  }

  LOG_TRACE("Removing reviews for " << _eid << " from map...");
  ElementPtr from = map->getElement(_eid);
  set<ReviewMarker::ReviewUid> reviews = ReviewMarker().getReviewUids(map, from);
  for (set<ReviewMarker::ReviewUid>::const_iterator it = reviews.begin(); it != reviews.end();
    ++it)
  {
    // just remove the review, not its children.
    RemoveElementByEid::removeElement(map, *it);
    _numAffected++;
  }
  LOG_TRACE(
    "Removed " << StringUtils::formatLargeNumber(_numAffected) << " reviews for " << _eid << ".");

  if (_clearAndRemove)
  {
    // just in case it is still part of an element (e.g. part of another relation)
    LOG_TRACE("Removing " << _eid << "...");
    from->getTags().clear();
    RecursiveElementRemover(_eid).apply(map);
  }
}

}
