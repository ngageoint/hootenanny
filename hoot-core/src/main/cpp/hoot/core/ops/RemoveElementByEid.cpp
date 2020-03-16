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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "RemoveElementByEid.h"

// hoot
#include <hoot/core/ops/RemoveNodeByEid.h>
#include <hoot/core/ops/RemoveRelationByEid.h>
#include <hoot/core/ops/RemoveWayByEid.h>

namespace hoot
{

RemoveElementByEid::RemoveElementByEid(bool doCheck) :
_doCheck(doCheck),
_removeNodeFully(true),
_removeOnlyUnusedNodes(false)
{
}

RemoveElementByEid::RemoveElementByEid(ElementId eId, bool doCheck) :
_eIdToRemove(eId),
_doCheck(doCheck),
_removeNodeFully(true),
_removeOnlyUnusedNodes(false)
{
}

void RemoveElementByEid::apply(OsmMapPtr& map)
{
  if (ElementType::Node == _eIdToRemove.getType().getEnum())
  {
    // Remove node fully (Removes node from relations & ways, then removes node from map)
    RemoveNodeByEid removeNode(
      _eIdToRemove.getId(), _doCheck, _removeNodeFully, _removeOnlyUnusedNodes);
    removeNode.apply(map);
  }
  else if (ElementType::Way == _eIdToRemove.getType().getEnum())
  {
    RemoveWayByEid removeWay(_eIdToRemove.getId(), _doCheck);
    removeWay.apply(map);
  }
  else if (ElementType::Relation == _eIdToRemove.getType().getEnum())
  {
    RemoveRelationByEid removeRelation(_eIdToRemove.getId());
    removeRelation.apply(map);
  }
  else
  {
    throw HootException(QString("Unexpected element type: %1").arg(_eIdToRemove.toString()));
  }
}

void RemoveElementByEid::removeElement(OsmMapPtr map, ElementId eId)
{
  RemoveElementByEid elementRemover(eId);
  elementRemover.apply(map);
}

void RemoveElementByEid::removeElementNoCheck(OsmMapPtr map, ElementId eId)
{
  RemoveElementByEid elementRemover(eId, false);
  elementRemover.apply(map);
}

void RemoveElementByEid::removeUnusedElementsOnly(OsmMapPtr map, ElementId eId)
{
  RemoveElementByEid elementRemover(eId, true);
  elementRemover.setRemoveNodeFully(false);
  elementRemover.setRemoveOnlyUnusedNodes(true);
  elementRemover.apply(map);
}

}

