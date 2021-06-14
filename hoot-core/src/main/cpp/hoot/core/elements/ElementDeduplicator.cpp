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
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2020, 2021 Maxar (http://www.maxar.com/)
 */

#include "ElementDeduplicator.h"

// Hoot
#include <hoot/core/criterion/TagCriterion.h>
#include <hoot/core/criterion/WayNodeCriterion.h>
#include <hoot/core/elements/RelationMemberUtils.h>
#include <hoot/core/elements/WayUtils.h>
#include <hoot/core/ops/RemoveElementByEid.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/visitors/ElementHashVisitor.h>

namespace hoot
{

ElementDeduplicator::ElementDeduplicator() :
_favorMoreConnectedWays(false),
_map1DuplicateNodesRemoved(0),
_map1DuplicateWaysRemoved(0),
_map1DuplicateRelationsRemoved(0),
_map2DuplicateNodesRemoved(0),
_map2DuplicateWaysRemoved(0),
_map2DuplicateRelationsRemoved(0)
{
}

void ElementDeduplicator::dedupe(OsmMapPtr map)
{
  _map1DuplicateNodesRemoved = 0;
  _map1DuplicateWaysRemoved = 0;
  _map1DuplicateRelationsRemoved = 0;
  _map2DuplicateNodesRemoved = 0;
  _map2DuplicateWaysRemoved = 0;
  _map2DuplicateRelationsRemoved = 0;

  LOG_INFO("De-duping intra-map: " << map->getName() << "...");
  LOG_DEBUG(map->getName() << " size before de-duping: " << map->size());

  const long nodesBefore = map->getNodeCount();
  const long waysBefore = map->getWayCount();
  const long relationsBefore = map->getRelationCount();

  // calculate our unique hashes per element and get a list of duplicate pairs within the map
  QMap<QString, ElementId> mapHashes;
  QSet<std::pair<ElementId, ElementId>> duplicates;
  calculateDuplicateElements(map, mapHashes, duplicates);
  QSet<QString> mapHashesSet = mapHashes.keys().toSet();
  LOG_VARD(mapHashesSet.size());

  // convert the duplicate pairs to the IDs of the element we actually want to remove, sorted by
  // element type and then remove them
  const QMap<ElementType::Type, QSet<ElementId>> elementsToRemove = _dupesToElementIds(duplicates);
  _removeElements(elementsToRemove[ElementType::Relation], map);
  _removeElements(elementsToRemove[ElementType::Way], map);
  _removeElements(elementsToRemove[ElementType::Node], map);

  _map1DuplicateNodesRemoved = (int)(nodesBefore - map->getNodeCount());
  _map1DuplicateWaysRemoved = (int)(waysBefore - map->getWayCount());
  _map1DuplicateRelationsRemoved = (int)(relationsBefore - map->getRelationCount());

  LOG_DEBUG(map->getName() << " size after de-duping: " << map->size());
  LOG_DEBUG("Removed " << _map1DuplicateNodesRemoved << " duplicate nodes from " << map->getName());
  LOG_DEBUG("Removed " << _map1DuplicateWaysRemoved << " duplicate ways from " << map->getName());
  LOG_DEBUG(
    "Removed " << _map1DuplicateRelationsRemoved << " duplicate relations from " << map->getName());
}

void ElementDeduplicator::dedupe(OsmMapPtr map1, OsmMapPtr map2)
{
  _map1DuplicateNodesRemoved = 0;
  _map1DuplicateWaysRemoved = 0;
  _map1DuplicateRelationsRemoved = 0;
  _map2DuplicateNodesRemoved = 0;
  _map2DuplicateWaysRemoved = 0;
  _map2DuplicateRelationsRemoved = 0;

  LOG_INFO("De-duping map: " << map1->getName() << " and " << map2->getName() << "...");
  LOG_DEBUG(map1->getName() << " size before de-duping: " << map1->size());
  LOG_DEBUG(map2->getName() << " size before de-duping: " << map2->size());
  LOG_VARD(_favorMoreConnectedWays);

  const int map1NodesBefore = map1->getNodeCount();
  const int map1WaysBefore = map1->getWayCount();
  const int map1RelationsBefore = map1->getRelationCount();
  const int map2NodesBefore = map2->getNodeCount();
  const int map2WaysBefore = map2->getWayCount();
  const int map2RelationsBefore = map2->getRelationCount();

  // Calculate our unique hashes per element for each map and get a list of duplicate pairs within
  // each map.

  QMap<QString, ElementId> map1Hashes;
  QSet<std::pair<ElementId, ElementId>> duplicates1;
  calculateDuplicateElements(map1, map1Hashes, duplicates1);
  QSet<QString> map1HashesSet = map1Hashes.keys().toSet();
  LOG_VARD(map1HashesSet.size());

  QMap<QString, ElementId> map2Hashes;
  QSet<std::pair<ElementId, ElementId>> duplicates2;
  calculateDuplicateElements(map2, map2Hashes, duplicates2);
  QSet<QString> map2HashesSet = map2Hashes.keys().toSet();
  LOG_VARD(map2HashesSet.size());

  QMap<ElementType::Type, QSet<ElementId>> elementsToRemove;
  QMap<ElementId, QString> elementIdsToRemoveFromMap;

  // remove the dupes within each map

  LOG_DEBUG("Recording " << map1->getName() << " duplicates...");
  _dupesToElementIdsCheckMap(duplicates1, map1, map2, elementsToRemove, elementIdsToRemoveFromMap);

  LOG_DEBUG("Recording " << map2->getName() << " duplicates...");
  _dupesToElementIdsCheckMap(duplicates2, map1, map2, elementsToRemove, elementIdsToRemoveFromMap);

  // Now, calculate the duplicates when comparing the maps between each other. We'll arbitrarily
  // remove features from the second map except where _favorMoreConnectedWays has influence, if it
  // has been enabled.
  _dupeHashesToElementIdsCheckMap(
    map1HashesSet.intersect(map2HashesSet), map1, map2, map1Hashes, map2Hashes, elementsToRemove,
    elementIdsToRemoveFromMap);

  _removeElements(elementsToRemove[ElementType::Relation], map2);
  LOG_DEBUG("Removing duplicate ways...");
  if (!_favorMoreConnectedWays)
  {
    _removeElements(elementsToRemove[ElementType::Way], map2);
  }
  else
  {
    _removeWaysCheckMap(elementsToRemove[ElementType::Way], map1, map2, elementIdsToRemoveFromMap);
  }
  _removeElements(elementsToRemove[ElementType::Node], map2);

  _map1DuplicateNodesRemoved = map1NodesBefore - map1->getNodeCount();
  _map1DuplicateWaysRemoved = map1WaysBefore - map1->getWayCount();
  _map1DuplicateRelationsRemoved = map1RelationsBefore - map1->getRelationCount();
  _map2DuplicateNodesRemoved = map2NodesBefore - map2->getNodeCount();
  _map2DuplicateWaysRemoved = map2WaysBefore - map2->getWayCount();
  _map2DuplicateRelationsRemoved = map2RelationsBefore - map2->getRelationCount();

  LOG_DEBUG(map1->getName() << " size after de-duping: " << map1->size());
  LOG_DEBUG(map2->getName() << " size after de-duping: " << map2->size());
  LOG_DEBUG(
    "Removed " << _map1DuplicateNodesRemoved << " duplicate nodes from " << map1->getName());
  LOG_DEBUG(
    "Removed " << _map1DuplicateWaysRemoved << " duplicate ways from " << map1->getName());
  LOG_DEBUG(
    "Removed " << _map1DuplicateRelationsRemoved << " duplicate relations from " <<
    map1->getName());
  LOG_DEBUG(
    "Removed " << _map2DuplicateNodesRemoved << " duplicate nodes from " << map2->getName());
  LOG_DEBUG("Removed " << _map2DuplicateWaysRemoved << " duplicate ways from " << map2->getName());
  LOG_DEBUG(
    "Removed " << _map2DuplicateRelationsRemoved << " duplicate relations from " <<
    map2->getName());
}

void ElementDeduplicator::calculateDuplicateElements(
  OsmMapPtr map, QMap<QString, ElementId>& hashes,
  QSet<std::pair<ElementId, ElementId>>& duplicates, const int coordinateComparisonSensitivity)
{
  LOG_VARD(coordinateComparisonSensitivity);
  ElementHashVisitor hashVis;
  hashVis.setWriteHashes(false);
  hashVis.setCollectHashes(true);
  hashVis.setCoordinateComparisonSensitivity(coordinateComparisonSensitivity);
  hashVis.setOsmMap(map.get());

  LOG_DEBUG("Calculating " << map->getName() << " element hashes...");
  map->visitRw(hashVis);
  hashes = hashVis.getHashesToElementIds();
  LOG_VARD(hashes.size());
  duplicates = hashVis.getDuplicates();
  LOG_VARD(duplicates.size());
  // Here, we're verifying that two way nodes don't belong to ways of very dissimilar types
  // before declaring them duplicates.
  duplicates = _filterOutNonDupeWayNodes(duplicates, map);
  LOG_VARD(duplicates.size());
}

QSet<std::pair<ElementId, ElementId>> ElementDeduplicator::_filterOutNonDupeWayNodes(
  const QSet<std::pair<ElementId, ElementId>>& dupes, OsmMapPtr map)
{
  QSet<std::pair<ElementId, ElementId>> filteredDupes;
  for (QSet<std::pair<ElementId, ElementId>>::const_iterator dupesItr = dupes.begin();
       dupesItr != dupes.end(); ++dupesItr)
  {
    std::pair<ElementId, ElementId> dupePair = *dupesItr;
    ElementPtr element1 = map->getElement(dupePair.first);
    ElementPtr element2 = map->getElement(dupePair.second);
    if (element1 && element2 && !_areWayNodesInWaysOfMismatchedType(element1, element2, map))
    {
      filteredDupes.insert(dupePair);
    }
    else if (element1 && element2)
    {
      LOG_TRACE(
        element1->getElementId() << " and " <<
        element2->getElementId() <<
        " are both way nodes that are in ways without matching types. Not marking as duplicates...");
    }
    else
    {
      LOG_TRACE(
        "Either " << dupePair.first << " or " << dupePair.second <<
        " not found in map. Not marking as duplicates...");
    }
  }
  return filteredDupes;
}

bool ElementDeduplicator::_areWayNodesInWaysOfMismatchedType(
  ElementPtr element1, ElementPtr element2, OsmMapPtr map)
{
  // This method is similar to ElementIdSynchronizer::_areWayNodesInWaysOfMismatchedType.

  LOG_VART(element1->getElementId());
  LOG_VART(element2->getElementId());

  WayNodeCriterion wayNodeCrit;
  wayNodeCrit.setOsmMap(map.get());
  LOG_VART(wayNodeCrit.isSatisfied(element1));
  const bool element1IdWayNode = wayNodeCrit.isSatisfied(element1);
  LOG_VART(wayNodeCrit.isSatisfied(element2));
  const bool element2IdWayNode = wayNodeCrit.isSatisfied(element2);
  // If they are both way nodes,
  if (!element1IdWayNode ||!element2IdWayNode)
  {
    return false;
  }

  // get the ways that contain each.
  const std::vector<ConstWayPtr> containingWays1 =
    WayUtils::getContainingWaysConst(element1->getId(), map);
  LOG_VART(containingWays1.size());
  const std::vector<ConstWayPtr> containingWays2 =
    WayUtils::getContainingWaysConst(element2->getId(), map);
  LOG_VART(containingWays2.size())

  // See if any of the ways between the two have a matching type.
  OsmSchema& schema = OsmSchema::getInstance();
  TagCriterion adminBoundsCrit("boundary", "administrative");
  for (std::vector<ConstWayPtr>::const_iterator containingWays1Itr = containingWays1.begin();
       containingWays1Itr != containingWays1.end(); ++containingWays1Itr)
  {
    ConstWayPtr way1 = *containingWays1Itr;
    if (way1)
    {
      LOG_VART(way1->getElementId());

      // If either of our containing ways is a administrative boundary, we're going to bail on the
      // type comparison, since many different types of ways could be part of an admin boundary.
      // This may not end up being the best way to deal with this.
      if (RelationMemberUtils::isMemberOfRelationSatisfyingCriterion(
            way1->getElementId(), adminBoundsCrit, map))
      {
        return false;
      }

      for (std::vector<ConstWayPtr>::const_iterator containingWays2Itr = containingWays2.begin();
           containingWays2Itr != containingWays2.end(); ++containingWays2Itr)
      {
        ConstWayPtr way2 = *containingWays2Itr;;
        if (way2)
        {
          LOG_VART(way2->getElementId());

          if (RelationMemberUtils::isMemberOfRelationSatisfyingCriterion(
                way2->getElementId(), adminBoundsCrit, map))
          {
            return false;
          }

          // Here we check the types. TODO: The use of 1.0 score for exact match differs from the
          // lower value used in ElementIdSynchronizer::_areWayNodesInWaysOfMismatchedType. That
          // logic needs to be rationalized or pass the score in externally.
          if (schema.explicitTypeMismatch(way1->getTags(), way2->getTags(), 1.0))
          {
            LOG_TRACE(
              "Found mismatching way parent type for way nodes " << element1->getElementId() <<
              " and " << element2->getElementId() << ".");
            return true;
          }
        }
      }
    }
  }

  return false;
}

void ElementDeduplicator::_removeElements(const QSet<ElementId>& elementsToRemove, OsmMapPtr map) const
{
  if (elementsToRemove.empty())
  {
    return;
  }

  // This works b/c we're sending in all ids of the same element type.
  const ElementId id = *elementsToRemove.begin();
  LOG_TRACE(
    "Removing duplicate " << id.getType().toString() << "s from " << map->getName() << "...");

  for (QSet<ElementId>::const_iterator itr = elementsToRemove.begin();
       itr != elementsToRemove.end(); ++itr)
  {
    const ElementId elementId = *itr;
    ConstElementPtr element = map->getElement(elementId);
    if (element)
    {
      LOG_TRACE("Removing " << elementId << "...");
      RemoveElementByEid::removeElementNoCheck(map, elementId);
    }
  }
}

void ElementDeduplicator::_removeWaysCheckMap(
  const QSet<ElementId>& waysToRemove, OsmMapPtr map1, OsmMapPtr map2,
  const QMap<ElementId, QString>& elementIdsToRemoveFromMap) const
{
  LOG_VARD(waysToRemove.size());

  int removedCtr = 0;
  for (QSet<ElementId>::const_iterator itr = waysToRemove.begin();
       itr != waysToRemove.end(); ++itr)
  {
    const ElementId elementId = *itr;
    assert(elementId.getType() == ElementType::Way);

    OsmMapPtr mapToRemoveFrom;
    LOG_VART(elementIdsToRemoveFromMap[elementId]);
    if (elementIdsToRemoveFromMap[elementId] == "1")
    {
      mapToRemoveFrom = map1;
    }
    else
    {
      mapToRemoveFrom = map2;
    }

    ConstElementPtr element = mapToRemoveFrom->getElement(elementId);
    LOG_VART(element.get());
    if (element)
    {
      LOG_TRACE("Removing " << elementId << " from " << mapToRemoveFrom->getName() << "...");
      RemoveElementByEid::removeElementNoCheck(mapToRemoveFrom, elementId);
      removedCtr++;
    }
  }
  LOG_DEBUG("Removed " << removedCtr << " ways.");
}

QMap<ElementType::Type, QSet<ElementId>> ElementDeduplicator::_dupesToElementIds(
  const QSet<std::pair<ElementId, ElementId>>& duplicates) const
{
  QMap<ElementType::Type, QSet<ElementId>> elementsToRemove;

  for (QSet<std::pair<ElementId, ElementId>>::const_iterator itr = duplicates.begin();
       itr != duplicates.end(); ++itr)
  {
    const std::pair<ElementId, ElementId> dupes = *itr;
    const ElementId dupe1 = dupes.first;
    LOG_VART(dupe1);
    const ElementId dupe2 = dupes.second;
    LOG_VART(dupe2);
    const ElementType elementType = dupe1.getType();
    LOG_VART(elementType);
    assert(elementType == dupe2.getType());

    elementsToRemove[elementType.getEnum()].insert(dupe1);
  }

  return elementsToRemove;
}

void ElementDeduplicator::_dupesToElementIdsCheckMap(
  const QSet<std::pair<ElementId, ElementId>>& duplicates, OsmMapPtr map1, OsmMapPtr map2,
  QMap<ElementType::Type, QSet<ElementId>>& elementsToRemove,
  QMap<ElementId, QString>& elementIdsToRemoveFromMap) const
{
  for (QSet<std::pair<ElementId, ElementId>>::const_iterator itr = duplicates.begin();
         itr != duplicates.end(); ++itr)
  {
    const std::pair<ElementId, ElementId> dupes = *itr;
    const ElementId dupe1 = dupes.first;
    LOG_VART(dupe1);
    const ElementId dupe2 = dupes.second;
    LOG_VART(dupe2);
    const ElementType elementType = dupe1.getType();
    LOG_VART(elementType);
    assert(elementType == dupe2.getType());

    if (elementType == ElementType::Way && _favorMoreConnectedWays)
    {
      const int numConnectedTo1 = WayUtils::getNumberOfConnectedWays(dupe1.getId(), map1);
      LOG_VART(numConnectedTo1);
      const int numConnectedTo2 = WayUtils::getNumberOfConnectedWays(dupe2.getId(), map2);
      LOG_VART(numConnectedTo2);
      if (numConnectedTo1 >= numConnectedTo2)
      {
        elementIdsToRemoveFromMap[dupe1] = "2";
        elementsToRemove[elementType.getEnum()].insert(dupe2);
      }
      else
      {
        elementIdsToRemoveFromMap[dupe2] = "1";
        elementsToRemove[elementType.getEnum()].insert(dupe1);
      }
    }
    else
    {
      // We always favor the first map when the number of connected ways aren't a factor, so record
      // the second map's element as the one to be removed.
      elementsToRemove[elementType.getEnum()].insert(dupe2);
    }
    LOG_VART(elementIdsToRemoveFromMap[dupe1]);
    LOG_VART(elementIdsToRemoveFromMap[dupe2]);
  }
}

void ElementDeduplicator::_dupeHashesToElementIdsCheckMap(
  const QSet<QString>& sharedHashes, OsmMapPtr map1, OsmMapPtr map2,
  const QMap<QString, ElementId>& map1Hashes, const QMap<QString, ElementId>& map2Hashes,
  QMap<ElementType::Type, QSet<ElementId>>& elementsToRemove,
  QMap<ElementId, QString>& elementIdsToRemoveFromMap) const
{
  LOG_DEBUG(
    "Calculating duplicates between " << map1->getName() << " and " << map2->getName() << "...");
  LOG_VARD(sharedHashes.size());
  for (QSet<QString>::const_iterator itr = sharedHashes.begin(); itr != sharedHashes.end(); ++itr)
  {
    const QString sharedHash = *itr;
    const ElementId toRemove1 = map1Hashes[sharedHash];
    LOG_VART(toRemove1);
    const ElementId toRemove2 = map2Hashes[sharedHash];
    LOG_VART(toRemove2);
    const ElementType elementType = toRemove1.getType();
    LOG_VART(elementType);
    assert(elementType == toRemove2.getType());

    if (elementType == ElementType::Way && _favorMoreConnectedWays)
    {
      const int numConnectedTo1 = WayUtils::getNumberOfConnectedWays(toRemove1.getId(), map1);
      LOG_VART(numConnectedTo1);
      const int numConnectedTo2 = WayUtils::getNumberOfConnectedWays(toRemove2.getId(), map2);
      LOG_VART(numConnectedTo2);
      if (numConnectedTo1 >= numConnectedTo2)
      {
        elementIdsToRemoveFromMap[toRemove1] = "2";
        elementsToRemove[elementType.getEnum()].insert(toRemove2);
      }
      else
      {
        elementIdsToRemoveFromMap[toRemove2] = "1";
        elementsToRemove[elementType.getEnum()].insert(toRemove1);
      }
    }
    else
    {
      // see related note in _dupesToElementIdsCheckMap
      elementsToRemove[elementType.getEnum()].insert(toRemove2);
    }
    LOG_VART(elementIdsToRemoveFromMap[toRemove1]);
    LOG_VART(elementIdsToRemoveFromMap[toRemove2]);
  }
}

}
