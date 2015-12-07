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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "ParentFirstVisitorOp.h"

// hoot
#include <hoot/core/OsmMapConsumer.h>
#include <hoot/core/elements/ElementVisitor.h>
#include <hoot/core/index/OsmMapIndex.h>

namespace hoot
{

ParentFirstVisitorOp::ParentFirstVisitorOp(shared_ptr<ElementVisitor> v) : _visitor(v)
{
}

void ParentFirstVisitorOp::apply(shared_ptr<OsmMap> &map)
{
  _clearCache();

  OsmMapConsumer* consumer = dynamic_cast<OsmMapConsumer*>(_visitor.get());
  if (consumer != 0)
  {
    consumer->setOsmMap(map.get());
  }

  const RelationMap rm = map->getRelationMap();

  for (RelationMap::const_iterator it = rm.begin(); it != rm.end(); ++it)
  {
    if (map->containsRelation(it->first))
    {
      _visit(it->second);
    }
  }

  const WayMap wm = map->getWays();
  for (WayMap::const_iterator it = wm.begin(); it != wm.end(); ++it)
  {
    if (map->containsWay(it->first))
    {
      _visitor->visit(it->second);
    }
  }

  const NodeMap nm = map->getNodeMap();
  for (NodeMap::const_iterator it = nm.begin(); it != nm.end(); ++it)
  {
    if (map->containsNode(it->first))
    {
      _visitor->visit(it->second);
    }
  }

  _clearCache();
}

void ParentFirstVisitorOp::_clearCache()
{
  _map.reset();
  _visited.clear();
}

void ParentFirstVisitorOp::_visit(shared_ptr<Element> e)
{
  _visited.insert(e->getElementId());
  set<ElementId> parents = _map->getIndex().getParents(e->getElementId());

  for (set<ElementId>::const_iterator it = parents.begin(); it != parents.end(); ++it)
  {
    if (_visited.count(*it) == 0)
    {
      _visit(_map->getElement(*it));
    }
  }

  _visitor->visit(e);
}

}
