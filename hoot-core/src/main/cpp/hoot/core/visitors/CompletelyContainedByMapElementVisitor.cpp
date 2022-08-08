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
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#include "CompletelyContainedByMapElementVisitor.h"

// geos
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/LineString.h>

// hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/util/Factory.h>

using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, CompletelyContainedByMapElementVisitor)

CompletelyContainedByMapElementVisitor::CompletelyContainedByMapElementVisitor()
{
  _complete = true;
}

bool CompletelyContainedByMapElementVisitor::isComplete(const OsmMap* map, ElementId eid)
{
  bool result;
  CompletelyContainedByMapElementVisitor v;
  v.setOsmMap(map);
  if (map->containsElement(eid))
  {
    map->getElement(eid)->visitRo(*map, v);
    result = v.isComplete();
  }
  else
  {
    result = false;
  }

  return result;
}

void CompletelyContainedByMapElementVisitor::visit(const ConstElementPtr& e)
{
  if (!_complete)
  {
    return;
  }

  ElementType type = e->getElementType();
  long id = e->getId();

  if (_map->containsElement(type, id) == false)
  {
    _complete = false;
    return;
  }

  if (type == ElementType::Way)
  {
    std::shared_ptr<const Way> w = _map->getWay(id);
    _visit(w);
  }
  else if (type == ElementType::Relation)
  {
    ConstRelationPtr r = _map->getRelation(id);
    _visit(r);
  }
  else if (type != ElementType::Node)
  {
    assert(false);
  }
}

void CompletelyContainedByMapElementVisitor::_visit(const std::shared_ptr<const Way>& w)
{
  const std::vector<long>& nids = w->getNodeIds();
  for (size_t i = 0; i < nids.size(); i++)
  {
    if (_map->containsNode(nids[i]) == false)
    {
      _complete = false;
      return;
    }
  }
}

void CompletelyContainedByMapElementVisitor::_visit(const std::shared_ptr<const Relation>& r)
{
  const vector<RelationData::Entry>& m = r->getMembers();

  for (size_t i = 0; i < m.size(); i++)
  {
    if (_map->containsElement(m[i].getElementId()) == false)
    {
      _complete = false;
      return;
    }
  }
}

}
