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
 * @copyright Copyright (C) 2021 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "OsmChangesetFileWriter.h"

// hoot
#include <hoot/core/elements/RelationMemberUtils.h>
#include <hoot/core/util/ConfigUtils.h>

namespace hoot
{

OsmChangesetFileWriter::OsmChangesetFileWriter() :
_includeDebugTags(false),
_includeCircularErrorTags(false),
_changesetIgnoreBounds(false)
{
}

bool OsmChangesetFileWriter::_failsBoundsCheck(
  const ConstElementPtr& element, const ConstOsmMapPtr& map1, const ConstOsmMapPtr& map2) const
{
  if (!element || !map1 || !map2)
  {
    return false;
  }

  LOG_TRACE("Checking bounds requirement for " << element->getElementId() << "...");

  // Pick the map and bounds crit based on the status of the element involved in the current
  // change being processed.
  std::shared_ptr<InBoundsCriterion> boundsCrit;
  ConstOsmMapPtr map;
  if (element->getStatus() == Status::Unknown1 && map1->containsElement(element))
  {
    map = map1;
    boundsCrit = ConfigUtils::getBoundsFilter(map1);
  }
  else if ((element->getStatus() == Status::Unknown2 ||
            element->getStatus() == Status::Conflated) &&
           map2->containsElement(element))
  {
    map = map2;
    boundsCrit = ConfigUtils::getBoundsFilter(map2);
  }
  else
  {
    LOG_TRACE(element->getElementId() << " not found in map for bounds check.");
    return false;
  }

//  if (element->getElementType() == ElementType::Relation &&
//      (element->getStatus() == Status::Unknown2 || element->getStatus() == Status::Conflated))
//  {
//    // TODO: should be able to replace this with a call to the bounds crit
//    if (!RelationMemberUtils::relationHasAllMembersWithinBounds(
//          std::dynamic_pointer_cast<const Relation>(element), boundsCrit, map))
//    {
//      LOG_TRACE(
//        "Skipping change with relation containing out of bounds element: " <<
//        element->getElementId() << "...");
//      return true;
//    }
//  }
//  else
//  {
//    LOG_VART(boundsCrit->isSatisfied(element));
//    if (!boundsCrit->isSatisfied(element))
//    {
//      LOG_TRACE(
//        "Skipping change with out of bounds element: " << element->getElementId() << "...");
//      return true;
//    }
//  }

  if (element->getElementType() == ElementType::Relation &&
      (element->getStatus() == Status::Unknown2 || element->getStatus() == Status::Conflated))
  {
    // If we're dealing with a relation from the new data (secondary; status = 2 or
    // conflated), we're requiring that all of its members be within bounds for it to be used
    // in the changeset. If this isn't done, relations may end up incomplete with missing
    // members. We don't worry about this for ref data, as we're assuming our ref data store
    // has all of its relation member data intact.
    boundsCrit->setMustCompletelyContain(true);
  }
  else
  {
    boundsCrit->setMustCompletelyContain(ConfigOptions().getBoundsKeepOnlyFeaturesInsideBounds());
  }

  LOG_VART(boundsCrit->isSatisfied(element));
  if (!boundsCrit->isSatisfied(element))
  {
    LOG_TRACE(
      "Skipping change with out of bounds element: " << element->getElementId() << "...");
    return true;
  }

  return false;
}

}
