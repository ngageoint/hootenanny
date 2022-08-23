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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */

#include <hoot/core/geometry/GeometryUtils.h>
#include <hoot/core/info/SingleStatistic.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/visitors/CalculateMapBoundsVisitor.h>

using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, CalculateMapBoundsVisitor)

void CalculateMapBoundsVisitor::visit(const std::shared_ptr<const Element>& e)
{
  // TRICKY: We will be in trouble if our element is NOT a node
  if (e->getElementType() != ElementType::Node)
    throw HootException("CalculateMapBoundsVisitor attempted to visit element that is not a node!");

  // Merge node. OGREnvelope takes care of initializing & merging logic
  std::shared_ptr<const Node> pNode = dynamic_pointer_cast<const Node>(e);
  _envelope.Merge(pNode->getX(), pNode->getY());
}

// Convenient way to get bounds
OGREnvelope CalculateMapBoundsVisitor::getBounds(const OsmMapPtr& map)
{
  CalculateMapBoundsVisitor v;
  map->visitNodesRo(v);
  return v.getBounds();
}

OGREnvelope CalculateMapBoundsVisitor::getBounds(const ConstOsmMapPtr& map)
{
  CalculateMapBoundsVisitor v;
  map->visitNodesRo(v);
  return v.getBounds();
}

geos::geom::Envelope CalculateMapBoundsVisitor::getGeosBounds(const OsmMapPtr& map)
{
  OGREnvelope envelope = getBounds(map);
  std::shared_ptr<geos::geom::Envelope> e(GeometryUtils::toEnvelope(envelope));
  return *e;
}

geos::geom::Envelope CalculateMapBoundsVisitor::getGeosBounds(const ConstOsmMapPtr& map)
{
  OGREnvelope envelope = getBounds(map);
  std::shared_ptr<geos::geom::Envelope> e(GeometryUtils::toEnvelope(envelope));
  return *e;
}

}
