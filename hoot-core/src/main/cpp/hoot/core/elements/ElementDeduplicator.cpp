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
 * @copyright Copyright (C) 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "ElementDeduplicator.h"

// Hoot
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/visitors/CalculateHashVisitor.h>
#include <hoot/core/elements/WayUtils.h>
#include <hoot/core/ops/RemoveElementByEid.h>

namespace hoot
{

ElementDeduplicator::ElementDeduplicator() :
_dedupeIntraMap(true),
_dedupeNodes(true),
_dedupeWays(true),
_dedupeRelations(true),
_favorMoreConnectedWays(false)
{
}

void ElementDeduplicator::_validateInputs()
{
  if (!_dedupeNodes && !_dedupeWays && !_dedupeRelations)
  {
    throw IllegalArgumentException("All element types set to be skipped for de-duplication.");
  }
}

void ElementDeduplicator::_calcElementHashes(
  OsmMapPtr map, QMap<QString, ElementId>& hashes,
  QSet<std::pair<ElementId, ElementId>>& duplicates)
{
  CalculateHashVisitor hashVis;
  hashVis.setWriteHashes(false);
  hashVis.setCollectHashes(true);

  LOG_DEBUG("Calculating " << map->getName() << " element hashes...");
  hashVis.setOsmMap(map.get());
  map->visitRw(hashVis);
  hashes = hashVis.getHashes();
  duplicates = hashVis.getDuplicates();
  LOG_VARD(duplicates.size());
}

void ElementDeduplicator::dedupe(OsmMapPtr map)
{
  _validateInputs();

  LOG_STATUS("De-duping intra-map: " << map->getName() << "...");
  LOG_DEBUG(map->getName() << " size before de-duping: " << map->size());

  QMap<QString, ElementId> mapHashes;
  QSet<std::pair<ElementId, ElementId>> duplicates;
  _calcElementHashes(map, mapHashes, duplicates);
  QSet<QString> mapHashesSet = mapHashes.keys().toSet();
  LOG_VARD(mapHashesSet.size());

  QMap<ElementType::Type, QSet<ElementId>> elementsToRemove;

  LOG_DEBUG("Recording " << map->getName() << " duplicates...");
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

  if (_dedupeRelations)
  {
    LOG_DEBUG("Removing duplicate relations from " << map->getName() << "...");
    const QSet<ElementId> relationsToRemove = elementsToRemove[ElementType::Relation];
    LOG_VARD(relationsToRemove.size());
    for (QSet<ElementId>::const_iterator itr = relationsToRemove.begin();
         itr != relationsToRemove.end(); ++itr)
    {
      LOG_TRACE("Removing " << *itr << "...");
      RemoveElementByEid::removeElementNoCheck(map, *itr);
    }
  }

  if (_dedupeWays)
  {
    LOG_DEBUG("Removing duplicate ways...");
    const QSet<ElementId> waysToRemove = elementsToRemove[ElementType::Way];
    LOG_VARD(waysToRemove.size());
    for (QSet<ElementId>::const_iterator itr = waysToRemove.begin();
         itr != waysToRemove.end(); ++itr)
    {
      LOG_TRACE("Removing " << *itr << "...");
      RemoveElementByEid::removeElementNoCheck(map, *itr);
    }
  }

  if (_dedupeNodes)
  {
    LOG_DEBUG("Removing duplicate nodes from " << map->getName() << "...");
    const QSet<ElementId> nodesToRemove = elementsToRemove[ElementType::Node];
    LOG_VARD(nodesToRemove.size());
    for (QSet<ElementId>::const_iterator itr = nodesToRemove.begin();
         itr != nodesToRemove.end(); ++itr)
    {
      LOG_TRACE("Removing " << *itr << "...");
      RemoveElementByEid::removeElementNoCheck(map, *itr);
    }
  }

  LOG_DEBUG(map->getName() << " size after de-duping: " << map->size());
  // TODO: stats
}

void ElementDeduplicator::dedupe(OsmMapPtr map1, OsmMapPtr map2)
{
  _validateInputs();

  LOG_STATUS("De-duping map: " << map1->getName() << " and " << map2->getName() << "...");
  LOG_DEBUG(map1->getName() << " size before de-duping: " << map1->size());
  LOG_DEBUG(map2->getName() << " size before de-duping: " << map2->size());

  QMap<QString, ElementId> map1Hashes;
  QSet<std::pair<ElementId, ElementId>> duplicates1;
  _calcElementHashes(map1, map1Hashes, duplicates1);
  QSet<QString> map1HashesSet = map1Hashes.keys().toSet();
  LOG_VARD(map1HashesSet.size());

  QMap<QString, ElementId> map2Hashes;
  QSet<std::pair<ElementId, ElementId>> duplicates2;
  _calcElementHashes(map2, map2Hashes, duplicates2);
  QSet<QString> map2HashesSet = map2Hashes.keys().toSet();
  LOG_VARD(map2HashesSet.size());

  QMap<ElementType::Type, QSet<ElementId>> elementsToRemove;
  QMap<ElementId, QString> elementIdsToRemoveFromMap;

  if (_dedupeIntraMap)
  {
    LOG_DEBUG("Recording " << map1->getName() << " duplicates...");
    for (QSet<std::pair<ElementId, ElementId>>::const_iterator itr = duplicates1.begin();
         itr != duplicates1.end(); ++itr)
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
          elementIdsToRemoveFromMap[dupe1] = "1";
          elementsToRemove[elementType.getEnum()].insert(dupe1);
        }
        else
        {
          elementIdsToRemoveFromMap[dupe2] = "2";
          elementsToRemove[elementType.getEnum()].insert(dupe2);
        }
      }
      else
      {
        elementsToRemove[elementType.getEnum()].insert(dupe1);
      }
    }

    LOG_DEBUG("Recording " << map2->getName() << " duplicates...");
    for (QSet<std::pair<ElementId, ElementId>>::const_iterator itr = duplicates2.begin();
         itr != duplicates2.end(); ++itr)
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
          elementIdsToRemoveFromMap[dupe1] = "1";
          elementsToRemove[elementType.getEnum()].insert(dupe1);
        }
        else
        {
          elementIdsToRemoveFromMap[dupe2] = "2";
          elementsToRemove[elementType.getEnum()].insert(dupe2);
        }
      }
      else
      {
        elementsToRemove[elementType.getEnum()].insert(dupe1);
      }
    }
  }

  LOG_DEBUG(
    "Calculating duplicates between " << map1->getName() << " and " << map2->getName() << "...");
  const QSet<QString> sharedHashes = map1HashesSet.intersect(map2HashesSet);
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
        elementIdsToRemoveFromMap[toRemove1] = "1";
        elementsToRemove[elementType.getEnum()].insert(toRemove1);
      }
      else
      {
        elementIdsToRemoveFromMap[toRemove2] = "2";
        elementsToRemove[elementType.getEnum()].insert(toRemove2);
      }
    }
    else
    {
      elementsToRemove[elementType.getEnum()].insert(toRemove1);
    }
  }

  if (_dedupeRelations)
  {
    LOG_DEBUG("Removing duplicate relations from " << map2->getName() << "...");
    const QSet<ElementId> relationsToRemove = elementsToRemove[ElementType::Relation];
    LOG_VARD(relationsToRemove.size());
    for (QSet<ElementId>::const_iterator itr = relationsToRemove.begin();
         itr != relationsToRemove.end(); ++itr)
    {
      LOG_TRACE("Removing " << *itr << "...");
      RemoveElementByEid::removeElementNoCheck(map2, *itr);
    }
  }

  if (_dedupeWays)
  {
    LOG_DEBUG("Removing duplicate ways...");
    const QSet<ElementId> waysToRemove = elementsToRemove[ElementType::Way];
    LOG_VARD(waysToRemove.size());
    for (QSet<ElementId>::const_iterator itr = waysToRemove.begin();
         itr != waysToRemove.end(); ++itr)
    {
      const ElementId id = *itr;
      OsmMapPtr mapToRemoveFrom;
      if (elementIdsToRemoveFromMap[id] == "1" && _favorMoreConnectedWays)
      {
        mapToRemoveFrom = map1;
      }
      else
      {
        mapToRemoveFrom = map2;
      }
      LOG_TRACE("Removing " << id << " from " << mapToRemoveFrom->getName() << "...");
      RemoveElementByEid::removeElementNoCheck(mapToRemoveFrom, id);
    }
  }

  if (_dedupeNodes)
  {
    LOG_DEBUG("Removing duplicate nodes from " << map2->getName() << "...");
    const QSet<ElementId> nodesToRemove = elementsToRemove[ElementType::Node];
    LOG_VARD(nodesToRemove.size());
    for (QSet<ElementId>::const_iterator itr = nodesToRemove.begin();
         itr != nodesToRemove.end(); ++itr)
    {
      LOG_TRACE("Removing " << *itr << "...");
      RemoveElementByEid::removeElementNoCheck(map2, *itr);
    }
  }

  LOG_DEBUG(map1->getName() << " size after de-duping: " << map1->size());
  LOG_DEBUG(map2->getName() << " size after de-duping: " << map2->size());
  // TODO: stats
}

}
