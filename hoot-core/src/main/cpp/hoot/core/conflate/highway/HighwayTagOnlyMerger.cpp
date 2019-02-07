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
#include <hoot/core/ops/RecursiveElementRemover.h>
#include <hoot/core/ops/ReplaceElementOp.h>
#include <hoot/core/schema/TagMergerFactory.h>
#include <hoot/core/util/Log.h>

#include <hoot/core/visitors/LengthOfWaysVisitor.h>

namespace hoot
{

// TODO: remove
//class ShortestFirstComparator
//{
//public:

//  bool operator()(const std::pair<ElementId, ElementId>& p1, const std::pair<ElementId, ElementId>& p2)
//  {
//    return std::min(getLength(p1.first), getLength(p1.second)) <
//        std::min(getLength(p2.first), getLength(p2.second));
//  }

//  Meters getLength(const ElementId& eid)
//  {
//    Meters result;
//    if (_lengthMap.contains(eid))
//    {
//      result = _lengthMap[eid];
//    }
//    else
//    {
//      LengthOfWaysVisitor v;
//      v.setOsmMap(map.get());
//      map->getElement(eid)->visitRo(*map, v);
//      result = v.getLengthOfWays();
//      _lengthMap[eid] = result;
//    }
//    return result;
//  }

//  virtual QString getDescription() const { return ""; }

//  OsmMapPtr map;

//private:

//  QHash<ElementId, Meters> _lengthMap;
//};

HighwayTagOnlyMerger::HighwayTagOnlyMerger(const std::set<std::pair<ElementId, ElementId>>& pairs)
{
  _pairs = pairs;
  LOG_VARD(hoot::toString(_pairs));
}

void HighwayTagOnlyMerger::apply(const OsmMapPtr& map,
                                 std::vector<std::pair<ElementId, ElementId>>& replaced)
{
  LOG_VARD(hoot::toString(_pairs));
  LOG_VARD(hoot::toString(replaced));

  for (std::set<std::pair<ElementId, ElementId>>::const_iterator it = _pairs.begin();
       it != _pairs.end(); ++it)
  {
    ElementId eid1 = it->first;
    ElementId eid2 = it->second;
    LOG_DEBUG("eid1 before replacement check: " << eid1);
    LOG_DEBUG("eid2 before replacement check: " << eid2);

//    for (size_t i = 0; i < replaced.size(); i++)
//    {
//      if (eid1 == replaced[i].first)
//      {
//        eid1 = replaced[i].second;
//      }
//      if (eid2 == replaced[i].first)
//      {
//        eid2 = replaced[i].second;
//      }
//    }
//    LOG_DEBUG("eid1 after replacement check: " << eid1);
//    LOG_DEBUG("eid2 after replacement check: " << eid2);

    if (map->containsElement(eid1) && map->containsElement(eid2))
    {
      _mergePair(map, eid1, eid2, replaced);
    }
    else
    {
      LOG_DEBUG(
        "Map doesn't contain one or more of the following elements: " << eid1 << ", " << eid2);
    }
  }
}

//void HighwayTagOnlyMerger::apply(const OsmMapPtr& map,
//                                 std::vector<std::pair<ElementId, ElementId>>& replaced)
//{
//  LOG_VARD(hoot::toString(_pairs));
//  LOG_VARD(hoot::toString(replaced));

//  std::vector<std::pair<ElementId, ElementId>> pairs;
//  pairs.reserve(_pairs.size());

//  for (std::set<std::pair<ElementId, ElementId>>::const_iterator it = _pairs.begin();
//       it != _pairs.end();  ++it)
//  {
//    ElementId eid1 = it->first;
//    ElementId eid2 = it->second;

//    if (map->containsElement(eid1) && map->containsElement(eid2))
//    {
//      pairs.push_back(std::pair<ElementId, ElementId>(eid1, eid2));
//    }
//    else
//    {
//      LOG_DEBUG(
//        "Map doesn't contain one or more of the following elements: " << eid1 << ", " << eid2);
//    }
//  }

//  ShortestFirstComparator shortestFirst;
//  shortestFirst.map = map;
//  sort(pairs.begin(), pairs.end(), shortestFirst);
//  for (std::vector<std::pair<ElementId, ElementId>>::const_iterator it = pairs.begin();
//       it != pairs.end(); ++it)
//  {
//    ElementId eid1 = it->first;
//    ElementId eid2 = it->second;

//    for (size_t i = 0; i < replaced.size(); i++)
//    {
//      if (eid1 == replaced[i].first)
//      {
//        eid1 = replaced[i].second;
//      }
//      if (eid2 == replaced[i].first)
//      {
//        eid2 = replaced[i].second;
//      }
//    }

//    _mergePair(map, eid1, eid2, replaced);
//  }
//}

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
    LOG_DEBUG(eid1 << " null.");
  }
  if (!e2)
  {
    LOG_DEBUG(eid2 << " null.");
  }

  if (e1 && e2)
  {
    ElementPtr elementToKeep;
    ElementPtr elementToRemove;
    assert(!(e1->getStatus() == Status::Conflated && e2->getStatus() == Status::Conflated));
    if (e1->getStatus() == Status::Unknown1 || e1->getStatus() == Status::Conflated)
    {
      elementToKeep = e1;
      elementToRemove = e2;
    }
    else if (e1->getStatus() == Status::Unknown2 || e2->getStatus() == Status::Conflated)
    {
      elementToKeep = e2;
      elementToRemove = e1;
    }
    //LOG_VART(elementToKeep->getElementId());
    //LOG_VART(elementToRemove->getElementId());
    LOG_VARD(elementToKeep);
    LOG_VARD(elementToRemove);

    // don't try to join if there are explicitly conflicting names; fix for #2888; not sure what
    // implications this has outside of the single test dataset I've tested on so far
    if (elementToKeep->getTags().hasName() &&
        elementToRemove->getTags().hasName() &&
        !Tags::haveMatchingName(elementToKeep->getTags(), elementToRemove->getTags()))
    {
      LOG_DEBUG("Conflicting name tags.  Skipping merge.");
      return false;
    }

    // Reverse the way if way to remove is one way and the two ways aren't similar directions
    if (elementToKeep->getElementType() == ElementType::Way &&
        elementToRemove->getElementType() == ElementType::Way)
    {
      WayPtr wayToKeep = boost::dynamic_pointer_cast<Way>(elementToKeep);
      WayPtr wayToRemove = boost::dynamic_pointer_cast<Way>(elementToRemove);
      if (OneWayCriterion().isSatisfied(wayToRemove) &&
          !DirectionFinder::isSimilarDirection(map->shared_from_this(), wayToKeep, wayToRemove))
      {
        LOG_DEBUG("Reversing " << wayToKeep->getElementId());
        wayToKeep->reverseOrder();
      }
    }

    // There actually could be a relation in here, but the default tag merging doesn't use the
    // element type anyway, so not worrying about it for now.
    elementToKeep->setTags(
      TagMergerFactory::mergeTags(
        elementToKeep->getTags(), elementToRemove->getTags(), ElementType::Way));
    elementToKeep->setStatus(Status::Conflated);
    replaced.push_back(
      std::pair<ElementId, ElementId>(
        elementToRemove->getElementId(), elementToKeep->getElementId()));
    LOG_VARD(elementToKeep);
    // Is this necessary?
    RecursiveElementRemover(elementToRemove->getElementId()).apply(map);
  }

  return false;
}

}

