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
 * @copyright Copyright (C) 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "UselessElementCriterion.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/elements/Tags.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/elements/Element.h>
#include <hoot/core/conflate/NodeToWayMap.h>

namespace hoot
{

bool UselessElementCriterion::isSatisfied(const shared_ptr<const Element> &e) const
{
  ElementId eid = e->getElementId();

  const shared_ptr<ElementToRelationMap> & e2r = _map->getIndex().getElementToRelationMap();

  // Is this element part of a relation? If so, it's not useless!
  const set<long>& parentRels = e2r->getRelationByElement(eid);
  if (parentRels.size() > 0)
    return false;

  if (ElementType::Node == eid.getType().getEnum())
  {
    // Check ways
    const set<long>& parentWays = _map->getIndex().getNodeToWayMap()->getWaysByNode(eid.getId());
    if (parentWays.size() > 0)
      return false;
  }
  else if (ElementType::Way == eid.getType().getEnum())
  {
    ConstWayPtr pWay = dynamic_pointer_cast<const Way>(e);

    // Check for kids
    if (pWay->getNodeCount() > 0)
      return false;
  }
  else if (ElementType::Relation == eid.getType().getEnum())
  {
    ConstRelationPtr pRel = dynamic_pointer_cast<const Relation>(e);
    if (pRel->getMembers().size() > 0)
      return false;
  }

  // No parents, no children, no siblings, no use!
  return true;
}

}
