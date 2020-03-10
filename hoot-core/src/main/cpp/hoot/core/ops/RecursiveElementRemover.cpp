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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "RecursiveElementRemover.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/ops/RemoveWayByEid.h>
#include <hoot/core/ops/RemoveNodeByEid.h>
#include <hoot/core/ops/RemoveRelationByEid.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/visitors/UniqueElementIdVisitor.h>

using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, RecursiveElementRemover)

RecursiveElementRemover::RecursiveElementRemover(ElementId eid, const ElementCriterion* criterion) :
_eid(eid),
_criterion(criterion)
{
}

void RecursiveElementRemover::apply(const std::shared_ptr<OsmMap>& map)
{
  _numAffected = 0;

  assert(_eid.isNull() == false);
  if (map->containsElement(_eid) == false)
  {
    return;
  }

  const ConstElementPtr& e = map->getElement(_eid);
  UniqueElementIdVisitor sv;
  e->visitRo(*map, sv);

  // find all potential candidates for erasure. We'll whittle away any invalid candidates.
  set<ElementId> toErase = sv.getElementSet();

  bool foundOne = true;
  // Keep looping through until we stop removing children. There may be times when the ordering of
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

  if (_criterion)
  {
    // go through all remaining delete candidates
    for (set<ElementId>::iterator it = toErase.begin(); it != toErase.end();)
    {
      ConstElementPtr child = map->getElement(*it);
      if (_criterion->isSatisfied(child))
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

void RecursiveElementRemover::_remove(const std::shared_ptr<OsmMap>& map, ElementId eid,
  const set<ElementId>& removeSet)
{
  // if this element isn't being removed
  if (removeSet.find(eid) == removeSet.end() || map->containsElement(eid) == false)
  {
    return;
  }

  LOG_TRACE("Removing: " << eid << "...");

  if (eid.getType() == ElementType::Relation)
  {
    const RelationPtr& r = map->getRelation(eid.getId());

    // make a copy so we can traverse it after this element is cleared.
    vector<RelationData::Entry> e = r->getMembers();
    r->clear();
    for (size_t i = 0; i < e.size(); i++)
    {
      _remove(map, e[i].getElementId(), removeSet);
    }

    RemoveRelationByEid::removeRelation(map, eid.getId());
    _numAffected++;
  }
  else if (eid.getType() == ElementType::Way)
  {
    const WayPtr& w = map->getWay(eid.getId());

    std::vector<long> nodes = w->getNodeIds();
    LOG_VART(nodes);
    w->clear();
    for (size_t i = 0; i < nodes.size(); i++)
    {
      _remove(map, ElementId::node(nodes[i]), removeSet);
    }

    RemoveWayByEid::removeWay(map, w->getId());
    _numAffected++;
  }
  else if (eid.getType() == ElementType::Node)
  {
    RemoveNodeByEid::removeNodeNoCheck(map, eid.getId());
    _numAffected++;
  }
  else
  {
    throw HootException("Unexpected element type.");
  }
}

}
