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
#include "ElementIdRemapper.h"

// hoot
#include <hoot/core/util/Factory.h>
//#include <hoot/core/ops/IdSwapOp.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, ElementIdRemapper)

ElementIdRemapper::ElementIdRemapper() :
_remappedIds(0)
{
}

void ElementIdRemapper::apply(OsmMapPtr& map)
{
  LOG_INFO("Remapping element IDs for: " << map->getName() << "...");

  _numAffected = 0;
  _remappedIds = 0;
  _originalToRemappedElementIds.clear();

  // TODO: make this more generic, if possible; use ElementIterator?

  const NodeMap nodes = map->getNodes();
  for (NodeMap::const_iterator it = nodes.begin(); it != nodes.end(); ++it)
  {
    ElementPtr currentElement = it->second;
    if (!currentElement)
    {
      continue;
    }
    LOG_VARD(currentElement->getElementId());
    ElementPtr newElement(currentElement->clone());
    const ElementId newElementId =
      ElementId(ElementType::Node, IdGenerator::getInstance()->createNodeId());
    newElement->setId(newElementId.getId());
    LOG_VARD(newElement->getElementId());
    _originalToRemappedElementIds[newElementId] = currentElement->getElementId();
    LOG_DEBUG("Adding element: " << newElement->getElementId() << "...");
    map->addElement(newElement);
    LOG_DEBUG(
      "Replacing element " << currentElement->getElementId() << " with " <<
      newElement->getElementId() << "...");
    map->replace(currentElement, newElement);
    //IdSwapOp idSwapper(currentElement->getElementId(), newElement->getElementId());
    //idSwapper.apply(map);
    _numAffected++;
  }

  const WayMap ways = map->getWays();
  for (WayMap::const_iterator it = ways.begin(); it != ways.end(); ++it)
  {
    ElementPtr currentElement = it->second;
    if (!currentElement)
    {
      continue;
    }
    LOG_VARD(currentElement->getElementId());
    ElementPtr newElement(currentElement->clone());
    const ElementId newElementId =
      ElementId(ElementType::Way, IdGenerator::getInstance()->createWayId());
    newElement->setId(newElementId.getId());
    LOG_VARD(newElement->getElementId());
    _originalToRemappedElementIds[newElementId] = currentElement->getElementId();
    LOG_DEBUG("Adding element: " << newElement->getElementId() << "...");
    map->addElement(newElement);
    LOG_DEBUG(
      "Replacing element " << currentElement->getElementId() << " with " <<
      newElement->getElementId() << "...");
    map->replace(currentElement, newElement);
    //IdSwapOp idSwapper(currentElement->getElementId(), newElement->getElementId());
    //idSwapper.apply(map);
    _numAffected++;
  }

  const RelationMap relations = map->getRelations();
  for (RelationMap::const_iterator it = relations.begin(); it != relations.end(); ++it)
  {
    ElementPtr currentElement = it->second;
    if (!currentElement)
    {
      continue;
    }
    // TODO: change to trace
    LOG_VARD(currentElement->getElementId());
    ElementPtr newElement(currentElement->clone());
    const ElementId newElementId =
      ElementId(ElementType::Relation, IdGenerator::getInstance()->createRelationId());
    newElement->setId(newElementId.getId());
    LOG_VARD(newElement->getElementId());
    _originalToRemappedElementIds[newElementId] = currentElement->getElementId();
    LOG_DEBUG("Adding element: " << newElement->getElementId() << "...");
    map->addElement(newElement);
    LOG_DEBUG(
      "Replacing element " << currentElement->getElementId() << " with " <<
      newElement->getElementId() << "...");
    map->replace(currentElement, newElement);
    //IdSwapOp idSwapper(currentElement->getElementId(), newElement->getElementId());
   // idSwapper.apply(map);
    _numAffected++;
  }
}

void ElementIdRemapper::restore(OsmMapPtr& map)
{
  LOG_INFO("Restoring original element IDs for: " << map->getName() << "...");

  _remappedIds = 0;

  // TODO: make this more generic, if possible; use ElementIterator?

  const NodeMap nodes = map->getNodes();
  for (NodeMap::const_iterator it = nodes.begin(); it != nodes.end(); ++it)
  {
    ElementPtr currentElement = it->second;
    if (!currentElement)
    {
      continue;
    }
    const ElementId originalElementId =
      _originalToRemappedElementIds[currentElement->getElementId()];
    if (!originalElementId.isNull())
    {
      ElementPtr originalElement(currentElement->clone());
      originalElement->setId(originalElementId.getId());
      LOG_DEBUG("Adding element: " << originalElement->getElementId() << "...");
      map->addElement(originalElement);
      LOG_DEBUG(
        "Replacing element " << currentElement->getElementId() << " with " <<
        originalElement->getElementId() << "...");
      map->replace(currentElement, originalElement);
      //IdSwapOp idSwapper(currentElement->getElementId(), originalElement->getElementId());
      //idSwapper.apply(map);
      _remappedIds++;
    }
  }

  const WayMap ways = map->getWays();
  for (WayMap::const_iterator it = ways.begin(); it != ways.end(); ++it)
  {
    ElementPtr currentElement = it->second;
    if (!currentElement)
    {
      continue;
    }
    const ElementId originalElementId =
      _originalToRemappedElementIds[currentElement->getElementId()];
    if (!originalElementId.isNull())
    {
      ElementPtr originalElement(currentElement->clone());
      originalElement->setId(originalElementId.getId());
      LOG_DEBUG("Adding element: " << originalElement->getElementId() << "...");
      map->addElement(originalElement);
      LOG_DEBUG(
        "Replacing element " << currentElement->getElementId() << " with " <<
        originalElement->getElementId() << "...");
      map->replace(currentElement, originalElement);
      //IdSwapOp idSwapper(currentElement->getElementId(), originalElement->getElementId());
      //idSwapper.apply(map);
      _remappedIds++;
    }
  }

  const RelationMap relations = map->getRelations();
  for (RelationMap::const_iterator it = relations.begin(); it != relations.end(); ++it)
  {
    ElementPtr currentElement = it->second;
    if (!currentElement)
    {
      continue;
    }
    LOG_VARD(currentElement->getElementId());
    const ElementId originalElementId =
      _originalToRemappedElementIds[currentElement->getElementId()];
    if (!originalElementId.isNull())
    {
      ElementPtr originalElement(currentElement->clone());
      originalElement->setId(originalElementId.getId());
      LOG_DEBUG("Adding element: " << originalElement->getElementId() << "...");
      map->addElement(originalElement);
      LOG_DEBUG(
        "Replacing element " << currentElement->getElementId() << " with " <<
        originalElement->getElementId() << "...");
      map->replace(currentElement, originalElement);
      //IdSwapOp idSwapper(currentElement->getElementId(), originalElement->getElementId());
      //idSwapper.apply(map);
      _remappedIds++;
    }
  }
}

}
