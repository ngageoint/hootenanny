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
#include <hoot/core/criterion/BridgeCriterion.h>
#include <hoot/core/conflate/highway/HighwaySnapMerger.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/elements/OsmUtils.h>

namespace hoot
{

HighwayTagOnlyMerger::HighwayTagOnlyMerger(const std::set<std::pair<ElementId, ElementId>>& pairs) :
HighwaySnapMerger(pairs, boost::shared_ptr<SublineStringMatcher>()),
_performBridgeGeometryMerging(false)
{
}

HighwayTagOnlyMerger::HighwayTagOnlyMerger(const std::set<std::pair<ElementId, ElementId>>& pairs,
                                     const boost::shared_ptr<SublineStringMatcher>& sublineMatcher) :
HighwaySnapMerger(pairs, sublineMatcher),
_performBridgeGeometryMerging(true)
{
  // Merging geometries for bridges is governed both by a config option and whether a subline
  // matcher gets passed in, since not all calling merger creators have a subline matcher available
  // to pass in at this point.
  if (!ConfigOptions().getAttributeConflationAllowRefGeometryChangesForBridges())
  {
    _performBridgeGeometryMerging = false;
  }
}

void HighwayTagOnlyMerger::_determineKeeperFeature(ElementPtr element1, ElementPtr element2,
                                                   ElementPtr& keeper, ElementPtr& toRemove,
                                                   bool& removeSecondaryElement)
{
  removeSecondaryElement = true;
  if (element1->getStatus() == Status::Conflated && element2->getStatus() == Status::Conflated)
  {
    keeper = element1;
    toRemove = element2;
    if (toRemove->getElementType() == ElementType::Way)
    {
      WayPtr wayWithTagsToRemove = boost::dynamic_pointer_cast<Way>(toRemove);
      wayWithTagsToRemove->setPid(element1->getElementId().getId());
      removeSecondaryElement = false;
    }
  }
  else if (element1->getStatus() == Status::Unknown1 || element1->getStatus() == Status::Conflated)
  {
    keeper = element1;
    toRemove = element2;
  }
  else if (element1->getStatus() == Status::Unknown2 || element2->getStatus() == Status::Conflated)
  {
    keeper = element2;
    toRemove = element1;
  }
  LOG_VARD(keeper->getElementId());
  LOG_VARD(toRemove->getElementId());
}

bool HighwayTagOnlyMerger::_mergePair(const OsmMapPtr& map, ElementId eid1, ElementId eid2,
  std::vector<std::pair<ElementId, ElementId>>& replaced)
{
  ElementPtr e1 = map->getElement(eid1);
  ElementPtr e2 = map->getElement(eid2);

  if (!e1 || !e2)
  {
    return HighwayMergerAbstract::_mergePair(map, eid1, eid2, replaced);
  }

  if (e1 && e2)
  {
    // If just one of the features is a bridge, we want the bridge feature to separate from the road
    // feature its being merged with.  So, use the normal geometry AND tag merger.
    std::vector<ConstElementPtr> elements;
    elements.push_back(e1);
    elements.push_back(e2);
    const bool onlyOneIsABridge = OsmUtils::isSatisfied<BridgeCriterion>(elements, 1, true);
    if (onlyOneIsABridge)
    {
      if (!_performBridgeGeometryMerging)
      {
        LOG_DEBUG(
          "Unable to perform geometric bridge merging due to invalid subline string matcher.  << "
          "Performing tag only merge...");
      }
      else
      {
        LOG_DEBUG("Using tag and geometry merger, since just one of the features is a bridge...");
        const bool needsReview = HighwaySnapMerger::_mergePair(map, eid1, eid2, replaced);
        if (needsReview)
        {
          LOG_DEBUG("HighwaySnapMerger returned review.");
        }
        return needsReview;
//        if (!HighwaySnapMerger::_mergePair(map, eid1, eid2, replaced))
//        {
//          return false;
//        }
      }
    }

    OsmUtils::logElementDetail(e1, map);
    OsmUtils::logElementDetail(e2, map);

    // handle relations coming from HighwaySnapMerger's previous handling of bridges
//    if (e1->getElementType() == ElementType::Relation ||
//        e2->getElementType() == ElementType::Relation)
//    {

//    }

    // Otherwise, proceed with tag only merging.

    ElementPtr elementWithTagsToKeep;
    ElementPtr elementWithTagsToRemove;
    bool removeSecondaryElement;
    _determineKeeperFeature(
      e1, e2, elementWithTagsToKeep, elementWithTagsToRemove, removeSecondaryElement);

    if (elementWithTagsToKeep->getTags().hasName() &&
        elementWithTagsToRemove->getTags().hasName() &&
        !Tags::haveMatchingName(
          elementWithTagsToKeep->getTags(), elementWithTagsToRemove->getTags()))
    {
      LOG_DEBUG("Conflicting name tags.  Skipping merge.");
      return false;
    }

    //LOG_VARD(elementWithTagsToKeep->getElementId());
    //LOG_VARD(elementWithTagsToRemove->getElementId());
    OsmUtils::logElementDetail(elementWithTagsToKeep, map);
    OsmUtils::logElementDetail(elementWithTagsToRemove, map);

    OneWayCriterion isAOneWayStreet;

    // don't try to merge streets with conflicting one way info
    // TODO: use Tags::isFalse here instead
    const bool keepElementExplicitlyNotAOneWayStreet =
      elementWithTagsToKeep->getTags().get("oneway") == "no";
    const bool removeElementExplicitlyNotAOneWayStreet =
      elementWithTagsToRemove->getTags().get("oneway") == "no";
    if ((isAOneWayStreet.isSatisfied(elementWithTagsToKeep) &&
         removeElementExplicitlyNotAOneWayStreet) ||
        (isAOneWayStreet.isSatisfied(elementWithTagsToRemove) &&
         keepElementExplicitlyNotAOneWayStreet))
    {
      LOG_DEBUG("Conflicting one way street tags.  Skipping merge.");
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
        LOG_DEBUG("Reversing " << wayWithTagsToKeep->getElementId() << "...");
        wayWithTagsToKeep->reverseOrder();
      }
    }

    // There actually could be a relation in here, but the default tag merging doesn't use that
    // element type anyway, so not worrying about it for now.
    elementWithTagsToKeep->setTags(
      TagMergerFactory::mergeTags(
        elementWithTagsToKeep->getTags(), elementWithTagsToRemove->getTags(), ElementType::Way));
    elementWithTagsToKeep->setStatus(Status::Conflated);
    LOG_DEBUG("Keeping element: " << elementWithTagsToKeep);

    if (removeSecondaryElement)
    {
      LOG_DEBUG("Marking " << elementWithTagsToRemove->getElementId() << " for replacement...");
      replaced.push_back(
        std::pair<ElementId, ElementId>(
          elementWithTagsToRemove->getElementId(), elementWithTagsToKeep->getElementId()));
    }
  }

  return false;
}

}
