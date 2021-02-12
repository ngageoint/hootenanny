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
 * @copyright Copyright (C) 2018, 2019, 2020 Maxar (http://www.maxar.com/)
 */
#include "AddBboxVisitor.h"

// GEOS
#include <geos/geom/Geometry.h>

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/geometry/ElementToGeometryConverter.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, AddBboxVisitor)

void AddBboxVisitor::visit(const std::shared_ptr<Element>& e)
{
  if (e->getTags().getNonDebugCount() > 0)
  {
    // Skip nodes
    if (e->getElementType() != ElementType::Node)
    {
      std::shared_ptr<geos::geom::Envelope> bounds(e->getEnvelope(_map->shared_from_this()));

      if (bounds)
      {
        e->getTags()["hoot:bbox"] =
          QString("%1,%2,%3,%4").arg(QString::number(bounds->getMinX(), 'g', 10))
            .arg(QString::number(bounds->getMinY(), 'g', 10))
            .arg(QString::number(bounds->getMaxX(), 'g', 10))
            .arg(QString::number(bounds->getMaxY(), 'g', 10));
      }
    }
  }
}

}
