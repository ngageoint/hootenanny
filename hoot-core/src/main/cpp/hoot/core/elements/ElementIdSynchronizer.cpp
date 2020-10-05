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

#include "ElementIdSynchronizer.h"

// Hoot
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/visitors/ElementHashVisitor.h>
#include <hoot/core/util/CollectionUtils.h>
#include <hoot/core/elements/WayUtils.h>

namespace hoot
{

ElementIdSynchronizer::ElementIdSynchronizer() :
_useNodeTagsForHash(true),
_updatedNodeCtr(0),
_updatedWayCtr(0),
_updatedRelationCtr(0)
{
}

void ElementIdSynchronizer::synchronize(const OsmMapPtr& map1, const OsmMapPtr& map2)
{
  _updatedNodeCtr = 0;
  _updatedWayCtr = 0;
  _updatedRelationCtr = 0;

  QString msg = "Synchronizing IDs for identical elements";
  if (!map1->getName().trimmed().isEmpty() && !map2->getName().trimmed().isEmpty())
  {
    msg += " between " + map1->getName() + " and " + map2->getName();
  }
  msg += "...";
  LOG_INFO(msg);

  // Calc element hashes associated with element IDs.
  const QMap<QString, ElementId> map1Hashes = _calcElementHashes(map1);
  LOG_VARD(map1Hashes.size());
  QSet<QString> map1HashesSet = map1Hashes.keys().toSet();
  const QMap<QString, ElementId> map2Hashes = _calcElementHashes(map2);
  LOG_VARD(map2Hashes.size());
  QSet<QString> map2HashesSet = map2Hashes.keys().toSet();

  // Obtain the hashes for the elements that are identical between the two maps.
  const QSet<QString> identicalHashes = map1HashesSet.intersect(map2HashesSet);
  LOG_VARD(identicalHashes.size());

  for (QSet<QString>::const_iterator itr = identicalHashes.begin(); itr != identicalHashes.end();
       ++itr)
  {
    const QString identicalHash = *itr;
    LOG_VART(identicalHash);

    // Get the element with matching hash from the ref map.
    ConstElementPtr map1IdenticalElement = map1->getElement(map1Hashes[identicalHash]);
    if (map1IdenticalElement)
    {
      LOG_VART(map1IdenticalElement->getElementId());

      if (!map2->containsElement(map1IdenticalElement->getElementId()))
      {
        // Copy it to be safe.
        ElementPtr map1IdenticalElementCopy(map1IdenticalElement->clone());
        LOG_VART(map1IdenticalElementCopy->getElementId());
        // Get the element with matching hash from the sec map.
        ElementPtr map2IdenticalElement = map2->getElement(map2Hashes[identicalHash]);
        if (map2IdenticalElement)
        {
          LOG_VART(map2IdenticalElement->getElementId());

          // This breaks out of spec relations test output.
//          bool element1IsWayNode = false;
//          _wayNodeCrit.setOsmMap(map1.get());
//          element1IsWayNode = _wayNodeCrit.isSatisfied(map1IdenticalElement);
//          bool element2IsWayNode = false;
//          _wayNodeCrit.setOsmMap(map2.get());
//          element2IsWayNode = _wayNodeCrit.isSatisfied(map2IdenticalElement);
//          if (element1IsWayNode && element2IsWayNode)
//          {
//            // find all ways each node belong to
//            QSet<long> containingWayIds1 =
//              CollectionUtils::stdSetToQSet(
//                WayUtils::getContainingWayIdsByNodeId(map1IdenticalElement->getId(), map1));
//            QSet<long> containingWayIds2 =
//              CollectionUtils::stdSetToQSet(
//                WayUtils::getContainingWayIdsByNodeId(map2IdenticalElement->getId(), map2));
//            if (containingWayIds1.intersect(containingWayIds2).size() == 0)
//            {
//              continue;
//            }
//          }

          // Make sure the map being updated doesn't already have an element with this ID (this check
          // may not be necessary).
          LOG_TRACE(
            "Updating map 2 element: " << map2IdenticalElement->getElementId() << " to " <<
            map1IdenticalElementCopy->getElementId() << "...");

          // Add a custom metadata tag for debugging purposes.
          map1IdenticalElementCopy->getTags().set(MetadataTags::HootIdSynchronized(), "yes");
          // Add the element from the ref map.
          map2->addElement(map1IdenticalElementCopy);
          // Replace the element from the sec map with the newly added element, which removes the
          // old element.
          map2->replace(map2IdenticalElement, map1IdenticalElementCopy);

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
        }
      }
    }
  }

  LOG_DEBUG(
    "Updated IDs on " << getNumTotalFeatureIdsSynchronized() <<
    " identical elements in second map.");
}

QMap<QString, ElementId> ElementIdSynchronizer::_calcElementHashes(
  const OsmMapPtr& map, const int coordinateComparisonSensitivity)
{
  LOG_DEBUG("Calculating " << map->getName() << " element hashes...");
  ElementHashVisitor hashVis;
  hashVis.setWriteHashes(false);
  hashVis.setCollectHashes(true);
  hashVis.setUseNodeTags(_useNodeTagsForHash);
  hashVis.setCoordinateComparisonSensitivity(coordinateComparisonSensitivity);
  hashVis.setOsmMap(map.get());
  map->visitRw(hashVis);
  return hashVis.getHashes();
}

}
