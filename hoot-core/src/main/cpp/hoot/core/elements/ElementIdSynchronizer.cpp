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
 * @copyright Copyright (C) 2020 Maxar (http://www.maxar.com/)
 */

#include "ElementIdSynchronizer.h"

// Hoot
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/visitors/ElementHashVisitor.h>
#include <hoot/core/util/CollectionUtils.h>
#include <hoot/core/elements/WayUtils.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/elements/RelationMemberUtils.h>
#include <hoot/core/criterion/TagCriterion.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/ops/ElementHashOp.h>

namespace hoot
{

ElementIdSynchronizer::ElementIdSynchronizer() :
_useNodeTagsForHash(true),
_coordinateComparisonSensitivity(ConfigOptions().getNodeComparisonCoordinateSensitivity()),
_updatedNodeCtr(0),
_updatedWayCtr(0),
_updatedRelationCtr(0)
{
}

void ElementIdSynchronizer::clear()
{
  _map1HashesToElementIds.clear();
  _map1ElementIdsToHashes.clear();
  _map2HashesToElementIds.clear();
  _map2ElementIdsToHashes.clear();
  _syncedElementIds.clear();
}

void ElementIdSynchronizer::synchronize(const OsmMapPtr& map1, const OsmMapPtr& map2,
                                        const ElementType& elementType)
{
  _updatedNodeCtr = 0;
  _updatedWayCtr = 0;
  _updatedRelationCtr = 0;
  _map1 = map1;
  _map2 = map2;

  QString msg = "Synchronizing IDs for identical elements";
  if (!_map1->getName().trimmed().isEmpty() && !_map2->getName().trimmed().isEmpty())
  {
    msg += " between " + _map1->getName() + " and " + _map2->getName();
  }
  msg += "...";
  LOG_INFO(msg);

  // Calc element hashes associated with element IDs.
  _calcElementHashes(_map1, _map1HashesToElementIds, _map1ElementIdsToHashes);
  LOG_VARD(_map1HashesToElementIds.size());
  _calcElementHashes(_map2, _map2HashesToElementIds, _map2ElementIdsToHashes);
  LOG_VARD(_map2HashesToElementIds.size());

  // Obtain the hashes for the elements that are identical between the two maps.
  QSet<QString> map1HashesSet;
  QSet<QString> map2HashesSet;
  if (elementType == ElementType::Unknown)
  {
    map1HashesSet = _map1HashesToElementIds.keys().toSet();
    map2HashesSet = _map2HashesToElementIds.keys().toSet();
  }
  else
  {
    map1HashesSet = _getHashesByElementType(_map1ElementIdsToHashes, elementType);
    map2HashesSet = _getHashesByElementType(_map2ElementIdsToHashes, elementType);
  }
  const QSet<QString> identicalHashes = map1HashesSet.intersect(map2HashesSet);
  LOG_VARD(identicalHashes.size());

  // Overwrite map2 IDs with the IDs from map1 for the features that are identical.
  _syncElementIds(identicalHashes);

  LOG_DEBUG(
    "Updated IDs on " << StringUtils::formatLargeNumber(getNumTotalFeatureIdsSynchronized()) <<
    " identical elements in second map.");
}

QSet<QString> ElementIdSynchronizer::_getHashesByElementType(
  const QMap<ElementId, QString>& hashesByElementId, const ElementType& elementType) const
{
  QSet<QString> filteredHashes;
  for (QMap<ElementId, QString>::const_iterator itr = hashesByElementId.begin();
       itr != hashesByElementId.end(); ++itr)
  {
    if (itr.key().getType() == elementType)
    {
      filteredHashes.insert(itr.value());
    }
  }
  return filteredHashes;
}

void ElementIdSynchronizer::_syncElementIds(const QSet<QString>& identicalHashes)
{
  for (QSet<QString>::const_iterator itr = identicalHashes.begin(); itr != identicalHashes.end();
       ++itr)
  {
    const QString identicalHash = *itr;
    LOG_VART(identicalHash);

    // Get the element with matching hash from the ref map.
    ElementPtr map1IdenticalElement = _map1->getElement(_map1HashesToElementIds[identicalHash]);
    if (map1IdenticalElement)
    {
      LOG_VART(map1IdenticalElement->getElementId());

      // Make sure the map being updated doesn't already have an element with this ID and that
      // we haven't already synced an element to this ID.
      if (!_map2->containsElement(map1IdenticalElement->getElementId()) &&
          !_syncedElementIds.contains(map1IdenticalElement->getElementId()))
      {
        // Copy it to be safe.
        ElementPtr map1IdenticalElementCopy(map1IdenticalElement->clone());
        LOG_VART(map1IdenticalElementCopy->getElementId());
        // Get the element with matching hash from the sec map.
        ElementPtr map2IdenticalElement = _map2->getElement(_map2HashesToElementIds[identicalHash]);
        if (map2IdenticalElement)
        {
          LOG_VART(map2IdenticalElement->getElementId());

          // Here, we're verifying both that two way nodes don't belong to ways of very dissimilar
          // types and that they share at least one way in common before syncing their IDs. Using
          // either one of these checks separately causes issue with the output. This is probably
          // too brittle and not a good long term solution...a work in progress.
          if (_areWayNodesWithoutAWayInCommon(map1IdenticalElement, map2IdenticalElement) &&
              _areWayNodesInWaysOfMismatchedType(map1IdenticalElement, map2IdenticalElement))
          {
            LOG_TRACE(
              map1IdenticalElement->getElementId() << " and " <<
              map2IdenticalElement->getElementId() <<
              " are both way nodes that are in ways without matching types and have no way " <<
              "in common. Skipping ID sync...");
            continue;
          }

          LOG_TRACE(
            "Updating map 2 element: " << map2IdenticalElement->getElementId() << " to " <<
            map1IdenticalElementCopy->getElementId() << "...");

          // Add a custom metadata tag for debugging purposes.
          map1IdenticalElementCopy->getTags().set(MetadataTags::HootIdSynchronized(), "yes");
          // Add the element from the ref map.
          _map2->addElement(map1IdenticalElementCopy);
          // Replace the element from the sec map with the newly added element, which removes the
          // old element.
          _map2->replace(map2IdenticalElement, map1IdenticalElementCopy);
          _syncedElementIds.insert(map1IdenticalElementCopy->getElementId());

          switch (map1IdenticalElementCopy->getElementType().getEnum())
          {
            case ElementType::Node:
              _updatedNodeCtr++;
              break;
            case ElementType::Way:
              _updatedWayCtr++;
              break;
            case ElementType::Relation:
              _updatedRelationCtr++;
              break;
            default:
              throw IllegalArgumentException("Invalid element type.");
          }

          // expensive; leave disabled by default
//          OsmMapWriterFactory::writeDebugMap(
//            _map2,
//            "after-id-sync-" + map2IdenticalElement->getElementId().toString() + "-to-" +
//            map1IdenticalElementCopy->getElementId().toString());
        }
      }
    }
  }
}

bool ElementIdSynchronizer::_areWayNodesInWaysOfMismatchedType(
  ElementPtr element1, ElementPtr element2)
{
  // This method is similar to ElementDeduplicator::_areWayNodesInWaysOfMismatchedType.

  LOG_VART(element1->getElementId());
  LOG_VART(element2->getElementId());

  _wayNodeCrit.setOsmMap(_map1.get());
  LOG_VART(_wayNodeCrit.isSatisfied(element1));
  const bool element1IdWayNode = _wayNodeCrit.isSatisfied(element1);
  _wayNodeCrit.setOsmMap(_map2.get());
  LOG_VART(_wayNodeCrit.isSatisfied(element2));
  const bool element2IdWayNode = _wayNodeCrit.isSatisfied(element2);
  // If they are both way nodes,
  if (!element1IdWayNode ||!element2IdWayNode)
  {
    return false;
  }

  // get the ways that contain each.
  const std::vector<ConstWayPtr> containingWays1 =
    WayUtils::getContainingWaysByNodeId(element1->getId(), _map1);
  LOG_VART(containingWays1.size());
  const std::vector<ConstWayPtr> containingWays2 =
    WayUtils::getContainingWaysByNodeId(element2->getId(), _map2);
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
            _map1, way1->getElementId(), adminBoundsCrit))
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
                _map2, way2->getElementId(), adminBoundsCrit))
          {
            return false;
          }

          // We keep our type comparison score low here to reflect that fact that you don't need
          // strict type matches to allow for snapping (the ID sync allows for snapping to occur in
          // some cases). This score may need some tweaking.
          if (schema.explicitTypeMismatch(way1->getTags(), way2->getTags(), 0.3))
          // This doesn't work. Don't know if it could be a better long term solution or not.
          //if (schema.typeMismatch(way1->getTags(), way2->getTags(), 0.3))
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

bool ElementIdSynchronizer::_areWayNodesWithoutAWayInCommon(
  ElementPtr element1, ElementPtr element2)
{
  LOG_VART(element1->getElementId());
  LOG_VART(element2->getElementId());

  _wayNodeCrit.setOsmMap(_map1.get());
  LOG_VART(_wayNodeCrit.isSatisfied(element1));
  const bool element1IdWayNode = _wayNodeCrit.isSatisfied(element1);
  _wayNodeCrit.setOsmMap(_map2.get());
  LOG_VART(_wayNodeCrit.isSatisfied(element2));
  const bool element2IdWayNode = _wayNodeCrit.isSatisfied(element2);
  // If they are both way nodes,
  if (!element1IdWayNode ||!element2IdWayNode)
  {
    return false;
  }

  // get the ways that contain each.
  const QSet<long> containingWayIds1 =
    CollectionUtils::stdSetToQSet(
      WayUtils::getContainingWayIdsByNodeId(element1->getId(), _map1));
  LOG_VART(containingWayIds1);
  const QSet<long> containingWayIds2 =
    CollectionUtils::stdSetToQSet(
      WayUtils::getContainingWayIdsByNodeId(element2->getId(), _map2));
  LOG_VART(containingWayIds2);

  for (QSet<long>::const_iterator containingWays1Itr = containingWayIds1.begin();
       containingWays1Itr != containingWayIds1.end(); ++containingWays1Itr)
  {
    const QString way1Hash =
      _map1ElementIdsToHashes[ElementId(ElementType::Way, *containingWays1Itr)];
    LOG_VART(way1Hash);
    if (!way1Hash.trimmed().isEmpty())
    {
      for (QSet<long>::const_iterator containingWays2Itr = containingWayIds2.begin();
           containingWays2Itr != containingWayIds2.end(); ++containingWays2Itr)
      {
        const QString way2Hash =
          _map2ElementIdsToHashes[ElementId(ElementType::Way, *containingWays2Itr)];
        LOG_VART(way2Hash);
        if (!way2Hash.trimmed().isEmpty())
        {
          // If any of the ways between the two are identical, then they share a parent way.
          if (way1Hash == way2Hash)
          {
            LOG_TRACE(
              "Found common way node for " << element1->getElementId() << " and " <<
              element2->getElementId() << ".");
            return false;
          }
        }
      }
    }
  }

  return true;
}

void ElementIdSynchronizer::_calcElementHashes(
  const OsmMapPtr& map, QMap<QString, ElementId>& hashesToElementIds,
  QMap<ElementId, QString>& elementIdsToHashes)
{
  LOG_DEBUG("Calculating " << map->getName() << " element hashes...");

  ElementHashVisitor hashVis;
  // This exists as an option to use, since its capable of comparing way nodes better. However, it
  // ends up preventing some id syncs against real world data.
  //ElementHashOp hashVis;

  hashVis.setWriteHashes(false);
  hashVis.setCollectHashes(true);
  hashVis.setUseNodeTags(_useNodeTagsForHash);
  hashVis.setCoordinateComparisonSensitivity(_coordinateComparisonSensitivity);

  hashVis.setOsmMap(map.get());
  map->visitRw(hashVis);
  //hashVis.setAddParentToWayNodes(true);
  //hashVis.apply(map);

  hashesToElementIds = hashVis.getHashesToElementIds();
  elementIdsToHashes = hashVis.getElementIdsToHashes();
}

}
