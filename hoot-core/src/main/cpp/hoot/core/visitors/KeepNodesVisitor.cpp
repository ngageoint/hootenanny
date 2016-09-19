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
#include "KeepNodesVisitor.h"

// hoot
#include <hoot/core/Factory.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/ops/RecursiveElementRemover.h>
#include <hoot/core/ops/RemoveRelationOp.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, KeepNodesVisitor)

class FindAllNodesVisitor : public ElementVisitor
{
public:
  FindAllNodesVisitor(OsmMapPtr map) : _map(map) {}

  virtual void visit(const ConstElementPtr& e)
  {
    ElementType type = e->getElementType();
    long id = e->getId();

    ElementPtr ee = _map->getElement(type, id);

    if (type == ElementType::Node && ee->getTags().getNonDebugCount() > 0)
    {
      _nodes.insert(ElementId(type, id));
    }
  }

  const set<ElementId>& getNodes() const { return _nodes; }

private:
  OsmMapPtr _map;

  set<ElementId> _nodes;
};

void KeepNodesVisitor::visit(const ConstElementPtr& e)
{
  //shared_ptr<Element> ee = _map->getElement(e->getElementId());

  if (e->getElementType() != ElementType::Node || e->getTags().getNonDebugCount() == 0)
  {
    // check to see if any of this element's children are Nodes.
    FindAllNodesVisitor v(_map->shared_from_this());
    e->visitRo(*_map, v);

    RelationPtr r;
    if (v.getNodes().size() > 0)
    {
      // create a relation to contain the POIs that were found. This will keep them from being
      // deleted by the recursive deleter.
      r.reset(new Relation(Status::Invalid, _map->createNextRelationId(), 0));
      _map->addRelation(r);
      for (set<ElementId>::const_iterator it = v.getNodes().begin(); it != v.getNodes().end(); ++it)
      {
        r->addElement("", *it);
      }
    }

    RecursiveElementRemover(e->getElementId()).apply(_map->shared_from_this());

    // if it was used, clean up the temporary parent relation.
    if (r)
    {
      RemoveRelationOp::removeRelation(_map->shared_from_this(), r->getId());
    }
  }
}

}
