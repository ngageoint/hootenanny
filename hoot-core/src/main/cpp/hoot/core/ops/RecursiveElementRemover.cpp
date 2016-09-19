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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "RecursiveElementRemover.h"

// hoot
#include <hoot/core/Factory.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/ops/RemoveWayOp.h>
#include <hoot/core/ops/RemoveNodeOp.h>
#include <hoot/core/ops/RemoveRelationOp.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/visitors/SetVisitor.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, RecursiveElementRemover)

RecursiveElementRemover::RecursiveElementRemover(ElementId eid, const ElementCriterion* filter) :
  _eid(eid),
  _filter(filter)
{
}

void RecursiveElementRemover::apply(const shared_ptr<OsmMap> &map)
{
  assert(_eid.isNull() == false);
  if (map->containsElement(_eid) == false)
  {
    return;
  }
  const ConstElementPtr& e = map->getElement(_eid);
  SetVisitor sv;
  e->visitRo(*map, sv);

  // find all potential candidates for erasure. We'll whittle away any invalid candidates.
  set<ElementId> toErase = sv.getElementSet();

  bool foundOne = true;
  // keep looping through until we stop removing children. There may be times when the ordering of
  // the removal matters.
  while (foundOne)
  {
    foundOne = false;
    // go through all
    for (set<ElementId>::iterator it = toErase.begin(); it != toErase.end();)
    {
      bool erased = false;
      set<ElementId> parents = map->getIndex().getParents(*it);

      // go through each of the child's direct parents
      for (set<ElementId>::const_iterator jt = parents.begin(); jt != parents.end(); ++jt)
      {
        if (toErase.find(*jt) == toErase.end())
        {
          // remove the child b/c it is owned by an element outside _eid.
          toErase.erase(it++);
          erased = true;
          foundOne = true;
          break;
        }
      }

      // if we didn't erase the element then move the iterator forward
      if (!erased)
      {
        ++it;
      }
    }
  }

  if (_filter)
  {
    // go through all remaining delete candidates
    for (set<ElementId>::iterator it = toErase.begin(); it != toErase.end();)
    {
      ConstElementPtr child = map->getElement(*it);
      if (_filter->isSatisfied(child))
      {
        // remove the child
        toErase.erase(it++);
      }
      else
      {
        ++it;
      }
    }
  }

  // remove the relations from most general to most specific
  _remove(map, _eid, toErase);
}

void RecursiveElementRemover::_remove(const shared_ptr<OsmMap>& map, ElementId eid,
  const set<ElementId>& removeSet)
{
  // if this element isn't being removed
  if (removeSet.find(eid) == removeSet.end() || map->containsElement(eid) == false)
  {
    return;
  }

  if (eid.getType() == ElementType::Relation)
  {
    const shared_ptr<Relation>& r = map->getRelation(eid.getId());

    // make a copy so we can traverse it after this element is cleared.
    vector<RelationData::Entry> e = r->getMembers();
    r->clear();
    for (size_t i = 0; i < e.size(); i++)
    {
      _remove(map, e[i].getElementId(), removeSet);
    }

    RemoveRelationOp::removeRelation(map, eid.getId());
  }
  else if (eid.getType() == ElementType::Way)
  {
    const shared_ptr<Way>& w = map->getWay(eid.getId());

    std::vector<long> nodes = w->getNodeIds();
    w->clear();
    for (size_t i = 0; i < nodes.size(); i++)
    {
      _remove(map, ElementId::node(nodes[i]), removeSet);
    }

    RemoveWayOp::removeWay(map, w->getId());
  }
  else if (eid.getType() == ElementType::Node)
  {
    RemoveNodeOp::removeNodeNoCheck(map, eid.getId());
  }
  else
  {
    throw HootException("Unexpected element type.");
  }
}

}
