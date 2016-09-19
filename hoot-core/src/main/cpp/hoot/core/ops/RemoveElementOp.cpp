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
 * @copyright Copyright (C) 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "RemoveElementOp.h"
#include "RemoveNodeOp.h"
#include "RemoveWayOp.h"
#include "RemoveRelationOp.h"

// hoot
#include <hoot/core/Factory.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, RemoveElementOp)

RemoveElementOp::RemoveElementOp(bool doCheck):
  _doCheck(doCheck)
{
}

RemoveElementOp::RemoveElementOp(ElementId eId, bool doCheck):
  _eIdToRemove(eId),
  _doCheck(doCheck)
{
}

void RemoveElementOp::apply(shared_ptr<OsmMap>& map)
{
  if (ElementType::Node == _eIdToRemove.getType().getEnum())
  {
    // Remove node fully (Removes node from relations & ways, then removes node from map)
    RemoveNodeOp removeNode(_eIdToRemove.getId(), _doCheck, true);
    removeNode.apply(map);
  }
  else if (ElementType::Way == _eIdToRemove.getType().getEnum())
  {
    RemoveWayOp removeWay(_eIdToRemove.getId(), _doCheck);
    removeWay.apply(map);
  }
  else if (ElementType::Relation == _eIdToRemove.getType().getEnum())
  {
    RemoveRelationOp removeRelation(_eIdToRemove.getId());
    removeRelation.apply(map);
  }
  else
  {
    throw HootException(QString("Unexpected element type: %1").arg(_eIdToRemove.toString()));
  }
}

} // end namespace hoot

