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
#include "RemoveEmptyAreasVisitor.h"

// geos
#include <geos/geom/Geometry.h>

// hoot
#include <hoot/core/ops/RecursiveElementRemover.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/ElementConverter.h>
#include <hoot/core/util/Factory.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, RemoveEmptyAreasVisitor)

RemoveEmptyAreasVisitor::RemoveEmptyAreasVisitor()
{
}

void RemoveEmptyAreasVisitor::visit(const ConstElementPtr& e)
{
  // no need to visit nodes.
  if (e->getElementType() != ElementType::Node)
  {
    shared_ptr<Element> ee = _map->getElement(e->getElementId());
    visit(ee);
  }
}

void RemoveEmptyAreasVisitor::visit(const shared_ptr<Element>& e)
{
  if (!_ec.get())
  {
    _ec.reset(new ElementConverter(_map->shared_from_this()));
  }

  if (OsmSchema::getInstance().isArea(e->getTags(), e->getElementType()))
  {
    shared_ptr<Geometry> g = _ec->convertToGeometry(e);

    if (g->getArea() == 0.0)
    {
      RecursiveElementRemover(e->getElementId()).apply(_map->shared_from_this());
    }
  }
}


}
