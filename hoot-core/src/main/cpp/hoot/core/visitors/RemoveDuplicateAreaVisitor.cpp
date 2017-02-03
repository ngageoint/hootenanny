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
#include "RemoveDuplicateAreaVisitor.h"

// geos
#include <geos/geom/Geometry.h>
#include <geos/util/TopologyException.h>

// hoot
#include <hoot/core/Factory.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/ops/RecursiveElementRemover.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/schema/TagComparator.h>
#include <hoot/core/util/ElementConverter.h>
#include <hoot/core/util/GeometryUtils.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/visitors/ElementOsmMapVisitor.h>
#include <hoot/core/visitors/CompletelyContainedByMapElementVisitor.h>
#include <hoot/core/elements/ElementId.h>
#include <hoot/core/schema/TagDifferencer.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, RemoveDuplicateAreaVisitor)

RemoveDuplicateAreaVisitor::RemoveDuplicateAreaVisitor()
{
  _diff.reset(
    Factory::getInstance().constructObject<TagDifferencer>(
      ConfigOptions().getRemoveDuplicateAreasDiff()));
}

shared_ptr<Geometry> RemoveDuplicateAreaVisitor::_convertToGeometry(const shared_ptr<Element>& e1)
{
  QHash<ElementId, shared_ptr<Geometry> >::const_iterator it = _geoms.find(e1->getElementId());
  if (it != _geoms.end())
  {
    return it.value();
  }
  ElementConverter gc(_map->shared_from_this());
  shared_ptr<Geometry> g = gc.convertToGeometry(e1);
  _geoms[e1->getElementId()] = g;
  return g;
}

bool RemoveDuplicateAreaVisitor::_equals(const shared_ptr<Element>& e1,
  const shared_ptr<Element>& e2)
{
  if (e1->getStatus() != e2->getStatus())
  {
    return false;
  }

  // if e1 is a parent of e2 or vice versa, then don't consider them duplicates.
  if (_map->getIndex().getParents(e1->getElementId()).count(e2->getElementId()) == 1 ||
      _map->getIndex().getParents(e2->getElementId()).count(e1->getElementId()) == 1)
  {
    return false;
  }

  // if they effectively have different tags
  if (_diff->diff(_map->shared_from_this(), e1, e2) > 0.0)
  {
    return false;
  }

  // convert to geometry and cache as relevant.
  shared_ptr<Geometry> g1 = _convertToGeometry(e1);
  shared_ptr<Geometry> g2 = _convertToGeometry(e2);

  double a1 = g1->getArea();
  double a2 = g2->getArea();

  // if there is a significant difference in area, they aren't a match
  if (fabs(a1 - a2) >= (a1 * 1e-9) && a1 > 0.0 && a2 > 0.0)
  {
    return false;
  }

  // calculate the overlap of the areas
  shared_ptr<Geometry> overlap;
  try
  {
    overlap = shared_ptr<Geometry>(g1->intersection(g2.get()));
  }
  catch (geos::util::TopologyException& e)
  {
    g1.reset(GeometryUtils::validateGeometry(g1.get()));
    g2.reset(GeometryUtils::validateGeometry(g2.get()));
    overlap.reset(g1->intersection(g2.get()));
  }

  double ao = overlap->getArea();
  // if the overlap is outside double precision
  if (fabs(ao - a1) >= (a1 * 1e-9))
  {
    return false;
  }

  return true;
}

void RemoveDuplicateAreaVisitor::_removeOne(shared_ptr<Element> e1, shared_ptr<Element> e2)
{
  if (e1->getTags().size() > e2->getTags().size())
  {
    RecursiveElementRemover(e2->getElementId()).apply(_map->shared_from_this());
  }
  else if (e1->getTags().size() < e2->getTags().size())
  {
    RecursiveElementRemover(e1->getElementId()).apply(_map->shared_from_this());
  }
  else if (e1->getId() < e2->getId())
  {
    RecursiveElementRemover(e1->getElementId()).apply(_map->shared_from_this());
  }
  else
  {
    RecursiveElementRemover(e2->getElementId()).apply(_map->shared_from_this());
  }
}

void RemoveDuplicateAreaVisitor::visit(const ConstElementPtr& e)
{
  if(e->getElementType() != ElementType::Node)
  {
    shared_ptr<Element> ee = _map->getElement(e->getElementId());
    visit(ee);
  }
}

void RemoveDuplicateAreaVisitor::visit(const shared_ptr<Element>& e1)
{
  OsmSchema& schema = OsmSchema::getInstance();

  auto_ptr<Envelope> env(e1->getEnvelope(_map->shared_from_this()));
  // if the envelope is null or the element is incomplete.
  if (env->isNull() ||
      CompletelyContainedByMapElementVisitor::isComplete(_map, e1->getElementId()) == false ||
      schema.isArea(e1) == false)
  {
    return;
  }
  set<ElementId> neighbors = _map->getIndex().findWayRelations(*env);

  for (set<ElementId>::const_iterator it = neighbors.begin(); it != neighbors.end(); ++it)
  {
    ElementId eit = *it;
    if (e1->getElementId() < eit && eit.getType() != ElementType::Node)
    {
      shared_ptr<Element> e2 = _map->getElement(*it);

      // check to see if e2 is null, it is possible that we removed it w/ a previous call to remove
      // a parent.
      if (e2 != 0 &&
          schema.isArea(e2) &&
          _equals(e1, e2))
      {
        // remove the crummier one.
        _removeOne(e1, e2);
        // if we've deleted the element we're visiting.
        if (_map->containsElement(e1) == false)
        {
          return;
        }
      }
    }
  }
}

}
