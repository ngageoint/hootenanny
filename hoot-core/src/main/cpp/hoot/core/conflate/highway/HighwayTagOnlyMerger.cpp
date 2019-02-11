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
#include "HighwayTagOnlyMerger.h"

// hoot
#include <hoot/core/algorithms/DirectionFinder.h>
#include <hoot/core/criterion/OneWayCriterion.h>
#include <hoot/core/schema/TagMergerFactory.h>
#include <hoot/core/util/Log.h>

namespace hoot
{

HighwayTagOnlyMerger::HighwayTagOnlyMerger(const std::set<std::pair<ElementId, ElementId>>& pairs)
{
  _pairs = pairs;
  LOG_VART(hoot::toString(_pairs));
}

void HighwayTagOnlyMerger::apply(const OsmMapPtr& map,
                                 std::vector<std::pair<ElementId, ElementId>>& replaced)
{
  LOG_VART(hoot::toString(_pairs));
  LOG_VART(hoot::toString(replaced));

  std::vector<std::pair<ElementId, ElementId>> pairs;
  pairs.reserve(_pairs.size());

  for (std::set<std::pair<ElementId, ElementId>>::const_iterator it = _pairs.begin();
       it != _pairs.end(); ++it)
  {
    ElementId eid1 = it->first;
    ElementId eid2 = it->second;
    LOG_VART(eid1);
    LOG_VART(eid2);

    if (map->containsElement(eid1) && map->containsElement(eid2))
    {
      pairs.push_back(std::pair<ElementId, ElementId>(eid1, eid2));
    }
    else
    {
      LOG_TRACE(
        "Map doesn't contain one or more of the following elements: " << eid1 << ", " << eid2);
    }
  }
  LOG_VART(hoot::toString(pairs));

  ShortestFirstComparator shortestFirst;
  shortestFirst.map = map;
  sort(pairs.begin(), pairs.end(), shortestFirst);
  for (std::vector<std::pair<ElementId, ElementId>>::const_iterator it = pairs.begin();
       it != pairs.end(); ++it)
  {
    ElementId eid1 = it->first;
    ElementId eid2 = it->second;
    LOG_TRACE("eid1 before replacement check: " << eid1);
    LOG_TRACE("eid2 before replacement check: " << eid2);

    for (size_t i = 0; i < replaced.size(); i++)
    {
      LOG_VART(eid1);
      LOG_VART(eid2);
      LOG_VART(replaced[i].first);
      LOG_VART(replaced[i].second);
      if (eid1 == replaced[i].first)
      {
        LOG_TRACE("Changing " << eid1 << " to " << replaced[i].second << "...");
        eid1 = replaced[i].second;
      }
      if (eid2 == replaced[i].first)
      {
        LOG_TRACE("Changing " << eid2 << " to " << replaced[i].second << "...");
        eid2 = replaced[i].second;
      }
    }

    LOG_TRACE("eid1 after replacement check: " << eid1);
    LOG_TRACE("eid2 after replacement check: " << eid2);

    _mergePair(map, eid1, eid2, replaced);
  }
}

bool HighwayTagOnlyMerger::_mergePair(const OsmMapPtr& map, ElementId eid1, ElementId eid2,
  std::vector<std::pair<ElementId, ElementId>>& replaced)
{
  if (HighwayMergerAbstract::_mergePair(map, eid1, eid2, replaced))
  {
    return true;
  }

  ElementPtr e1 = map->getElement(eid1);
  ElementPtr e2 = map->getElement(eid2);
  if (!e1)
  {
    LOG_TRACE(eid1 << " null.");
  }
  if (!e2)
  {
    LOG_TRACE(eid2 << " null.");
  }

  if (e1 && e2)
  {
    ElementPtr elementWithTagsToKeep;
    ElementPtr elementWithTagsToRemove;
    bool removeSecondaryElement = true;
    if (e1->getStatus() == Status::Conflated && e2->getStatus() == Status::Conflated)
    {
      elementWithTagsToKeep = e1;
      elementWithTagsToRemove = e2;
      if (elementWithTagsToRemove->getElementType() == ElementType::Way)
      {
        WayPtr wayWithTagsToRemove = boost::dynamic_pointer_cast<Way>(elementWithTagsToRemove);
        wayWithTagsToRemove->setPid(e1->getElementId().getId());
        removeSecondaryElement = false;
      }
    }
    else if (e1->getStatus() == Status::Unknown1 || e1->getStatus() == Status::Conflated)
    {
      elementWithTagsToKeep = e1;
      elementWithTagsToRemove = e2;
    }
    else if (e1->getStatus() == Status::Unknown2 || e2->getStatus() == Status::Conflated)
    {
      elementWithTagsToKeep = e2;
      elementWithTagsToRemove = e1;
    }
    //LOG_VART(elementWithTagsToKeep->getElementId());
    //LOG_VART(elementWithTagsToRemove->getElementId());
    LOG_VART(elementWithTagsToKeep);
    LOG_VART(elementWithTagsToRemove);

    // don't try to merge if there are explicitly conflicting names; fix for #2888; not sure what
    // implications this has outside of the single test dataset I've tested on so far
    if (elementWithTagsToKeep->getTags().hasName() &&
        elementWithTagsToRemove->getTags().hasName() &&
        !Tags::haveMatchingName(
           elementWithTagsToKeep->getTags(), elementWithTagsToRemove->getTags()))
    {
      LOG_TRACE("Conflicting name tags.  Skipping merge.");
      return false;
    }

    OneWayCriterion isAOneWayStreet;

    // don't try to merge streets with conflicting one way info
    const bool keepElementExplicitlyNotAOneWayStreet =
      elementWithTagsToKeep->getTags().get("oneway") == "no";
    const bool removeElementExplicitlyNotAOneWayStreet =
      elementWithTagsToRemove->getTags().get("oneway") == "no";
    if ((isAOneWayStreet.isSatisfied(elementWithTagsToKeep) &&
         removeElementExplicitlyNotAOneWayStreet) ||
        (isAOneWayStreet.isSatisfied(elementWithTagsToRemove) &&
         keepElementExplicitlyNotAOneWayStreet))
    {
      LOG_TRACE("Conflicting one way street tags.  Skipping merge.");
      return false;
    }

    // Reverse the way if way to remove is one way and the two ways aren't in similar directions
    if (elementWithTagsToKeep->getElementType() == ElementType::Way &&
        elementWithTagsToRemove->getElementType() == ElementType::Way)
    {
      WayPtr wayWithTagsToKeep = boost::dynamic_pointer_cast<Way>(elementWithTagsToKeep);
      WayPtr wayWithTagsToRemove = boost::dynamic_pointer_cast<Way>(elementWithTagsToRemove);

      if (isAOneWayStreet.isSatisfied(wayWithTagsToRemove) &&
          // note the use of an alternative isSimilarDirection method
          !DirectionFinder::isSimilarDirection2(
             map->shared_from_this(), wayWithTagsToKeep, wayWithTagsToRemove))
      {
        LOG_TRACE("Reversing " << wayWithTagsToKeep->getElementId());
        wayWithTagsToKeep->reverseOrder();
      }
    }

    // There actually could be a relation in here, but the default tag merging doesn't use that
    // element type anyway, so not worrying about it for now.
    elementWithTagsToKeep->setTags(
      TagMergerFactory::mergeTags(
        elementWithTagsToKeep->getTags(), elementWithTagsToRemove->getTags(), ElementType::Way));
    elementWithTagsToKeep->setStatus(Status::Conflated);
    LOG_VART(elementWithTagsToKeep);

    if (removeSecondaryElement)
    {
      LOG_TRACE("Removing " << elementWithTagsToRemove->getElementId() << "...");
      replaced.push_back(
        std::pair<ElementId, ElementId>(
          elementWithTagsToRemove->getElementId(), elementWithTagsToKeep->getElementId()));
    }
  }

  return false;
}

}
