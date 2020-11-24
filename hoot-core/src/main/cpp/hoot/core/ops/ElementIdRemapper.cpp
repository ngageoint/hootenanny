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

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, ElementIdRemapper)

ElementIdRemapper::ElementIdRemapper() :
_restoredIds(0)
{
}

ElementIdRemapper::ElementIdRemapper(const ElementCriterionPtr& remapFilter) :
_remapFilter(remapFilter),
_restoreFilter(remapFilter),
_restoredIds(0)
{
}

ElementIdRemapper::ElementIdRemapper(
  const ElementCriterionPtr& remapFilter, const ElementCriterionPtr& restoreFilter) :
_remapFilter(remapFilter),
_restoreFilter(restoreFilter),
_restoredIds(0)
{
}

void ElementIdRemapper::apply(OsmMapPtr& map)
{
  LOG_INFO("Remapping element IDs for: " << map->getName() << "...");

  _numAffected = 0;
  _restoredIds = 0;
  _originalToRemappedElementIds.clear();

  // TODO: make this more generic, if possible; use ElementIterator?

  const NodeMap nodes = map->getNodes();
  for (NodeMap::const_iterator it = nodes.begin(); it != nodes.end(); ++it)
  {
    ElementPtr currentElement = it->second;
    if (!currentElement || (_remapFilter && !_remapFilter->isSatisfied(currentElement)))
    {
      continue;
    }
    ElementPtr newElement(currentElement->clone());
    const ElementId newElementId =
      ElementId(ElementType::Node, IdGenerator::getInstance()->createNodeId());
    newElement->setId(newElementId.getId());
    LOG_TRACE(
      "Remapping " << currentElement->getElementId() << " to " << newElement->getElementId() <<
      "...");
    _originalToRemappedElementIds[newElementId] = currentElement->getElementId();
    map->addElement(newElement);
    map->replace(currentElement, newElement);
    _numAffected++;
  }

  const WayMap ways = map->getWays();
  for (WayMap::const_iterator it = ways.begin(); it != ways.end(); ++it)
  {
    ElementPtr currentElement = it->second;
    if (!currentElement || (_remapFilter && !_remapFilter->isSatisfied(currentElement)))
    {
      continue;
    }
    ElementPtr newElement(currentElement->clone());
    const ElementId newElementId =
      ElementId(ElementType::Way, IdGenerator::getInstance()->createWayId());
    newElement->setId(newElementId.getId());
    LOG_TRACE(
      "Remapping " << currentElement->getElementId() << " to " << newElement->getElementId() <<
      "...");
    _originalToRemappedElementIds[newElementId] = currentElement->getElementId();
    map->addElement(newElement);
    map->replace(currentElement, newElement);
    _numAffected++;
  }

  const RelationMap relations = map->getRelations();
  for (RelationMap::const_iterator it = relations.begin(); it != relations.end(); ++it)
  {
    ElementPtr currentElement = it->second;
    if (!currentElement || (_remapFilter && !_remapFilter->isSatisfied(currentElement)))
    {
      continue;
    }
    ElementPtr newElement(currentElement->clone());
    const ElementId newElementId =
      ElementId(ElementType::Relation, IdGenerator::getInstance()->createRelationId());
    newElement->setId(newElementId.getId());
    LOG_TRACE(
      "Remapping " << currentElement->getElementId() << " to " << newElement->getElementId() <<
      "...");
    _originalToRemappedElementIds[newElementId] = currentElement->getElementId();
    map->addElement(newElement);
    map->replace(currentElement, newElement);
    _numAffected++;
  }
}

void ElementIdRemapper::restore(OsmMapPtr& map)
{
  LOG_INFO("Restoring original element IDs for: " << map->getName() << "...");

  _restoredIds = 0;
  if (!_restoreFilter)
  {
    _restoreFilter = _remapFilter;
  }

  // TODO: make this more generic, if possible; use ElementIterator?

  const NodeMap nodes = map->getNodes();
  for (NodeMap::const_iterator it = nodes.begin(); it != nodes.end(); ++it)
  {
    ElementPtr currentElement = it->second;
    if (!currentElement || (_restoreFilter && !_restoreFilter->isSatisfied(currentElement)))
    {
      continue;
    }
    const ElementId originalElementId =
      _originalToRemappedElementIds[currentElement->getElementId()];
    if (!originalElementId.isNull() && !map->containsElement(originalElementId))
    {
      ElementPtr originalElement(currentElement->clone());
      originalElement->setId(originalElementId.getId());
      LOG_TRACE(
        "Restoring " << currentElement->getElementId() << " to " <<
        originalElement->getElementId() << "...");
      map->addElement(originalElement);
      map->replace(currentElement, originalElement);
      _restoredIds++;
    }
  }

  const WayMap ways = map->getWays();
  for (WayMap::const_iterator it = ways.begin(); it != ways.end(); ++it)
  {
    ElementPtr currentElement = it->second;
    if (!currentElement || (_restoreFilter && !_restoreFilter->isSatisfied(currentElement)))
    {
      continue;
    }
    const ElementId originalElementId =
      _originalToRemappedElementIds[currentElement->getElementId()];
    if (!originalElementId.isNull() && !map->containsElement(originalElementId))
    {
      ElementPtr originalElement(currentElement->clone());
      originalElement->setId(originalElementId.getId());
      LOG_TRACE(
        "Restoring " << currentElement->getElementId() << " to " <<
        originalElement->getElementId() << "...");
      map->addElement(originalElement);
      map->replace(currentElement, originalElement);
      _restoredIds++;
    }
  }

  const RelationMap relations = map->getRelations();
  for (RelationMap::const_iterator it = relations.begin(); it != relations.end(); ++it)
  {
    ElementPtr currentElement = it->second;
    if (!currentElement || (_restoreFilter && !_restoreFilter->isSatisfied(currentElement)))
    {
      continue;
    }
    const ElementId originalElementId =
      _originalToRemappedElementIds[currentElement->getElementId()];
    if (!originalElementId.isNull() && !map->containsElement(originalElementId))
    {
      ElementPtr originalElement(currentElement->clone());
      originalElement->setId(originalElementId.getId());
      LOG_TRACE(
        "Restoring " << currentElement->getElementId() << " to " <<
        originalElement->getElementId() << "...");
      map->addElement(originalElement);;
      map->replace(currentElement, originalElement);
      _restoredIds++;
    }
  }
}

}
