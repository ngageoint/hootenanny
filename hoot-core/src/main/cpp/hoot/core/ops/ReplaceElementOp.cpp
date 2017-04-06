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
#include "ReplaceElementOp.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/ops/RecursiveElementRemover.h>
#include <hoot/core/util/Log.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, ReplaceElementOp)

ReplaceElementOp::ReplaceElementOp(ElementId from, ElementId to, bool clearAndRemove) :
  _from(from),
  _to(to),
  _clearAndRemove(clearAndRemove)
{
}

void ReplaceElementOp::addElement(const ConstElementPtr& e)
{
  if (_from.isNull())
  {
    _from = e->getElementId();
  }
  else if (_to.isNull())
  {
    _to = e->getElementId();
  }
  else
  {
    throw IllegalArgumentException("Error adding element. Only two elements can be added 'from' "
      "and 'to'.");
  }
}

void ReplaceElementOp::apply(const boost::shared_ptr<OsmMap> &map)
{
  if (_from.isNull() || _to.isNull())
  {
    throw IllegalArgumentException("You must specify a valid 'from' and 'to' element ID.");
  }

  // if from isn't in the map, there is nothing to do.
  if (map->containsElement(_from) == false)
  {
    return;
  }

  const ElementPtr& from = map->getElement(_from);
  const ConstElementPtr& to = map->getElement(_to);
  assert(from);
  assert(to);

  set<ElementId> parents = map->getParents(_from);

  for (set<ElementId>::const_iterator it = parents.begin(); it != parents.end(); ++it)
  {
    ElementId pid = *it;

    switch (pid.getType().getEnum())
    {
    case ElementType::Relation:
      {
        RelationPtr r = map->getRelation(pid.getId());
        r->replaceElement(from, to);
        break;
      }
    case ElementType::Way:
      {
        // this should never happen.
        if (from->getElementType() != ElementType::Node)
        {
          LOG_VARE(from);
          LOG_VARE(to);
          map->validate();
          throw InternalErrorException("Internal Error: Somehow a non-node is being reported as "
            "part of a way.");
        }
        else if (to->getElementType() == ElementType::Node)
        {
          WayPtr w = map->getWay(pid.getId());
          w->replaceNode(_from.getId(), _to.getId());
        }
        break;
      }
    default:
      LOG_VARE(pid);
      throw InternalErrorException("Internal Error: Unexpected element reported as a parent.");
    }
  }

  if (_clearAndRemove)
  {
    // just in case it is still part of an element (e.g. part of another relation)
    from->getTags().clear();
    RecursiveElementRemover(_from).apply(map);
  }
}

}
