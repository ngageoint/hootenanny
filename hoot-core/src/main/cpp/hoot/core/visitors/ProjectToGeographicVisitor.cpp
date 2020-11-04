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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "ProjectToGeographicVisitor.h"

// GEOS
#include <geos/geom/Geometry.h>

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/geometry/ElementToGeometryConverter.h>
#include <hoot/core/elements/MapProjector.h>

using namespace geos::geom;

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, ProjectToGeographicVisitor)

ProjectToGeographicVisitor::ProjectToGeographicVisitor()
  : _transform(0)
{
}

ProjectToGeographicVisitor::~ProjectToGeographicVisitor()
{
  if (_transform)
    OGRCoordinateTransformation::DestroyCT(_transform);
}

void ProjectToGeographicVisitor::initialize(std::shared_ptr<OGRSpatialReference>& projection)
{
  _transform = OGRCreateCoordinateTransformation(projection.get(), MapProjector::createWgs84Projection().get());
  _rcf = std::shared_ptr<ReprojectCoordinateFilter>(new ReprojectCoordinateFilter(_transform));
}

void ProjectToGeographicVisitor::visit(const std::shared_ptr<Element>& e)
{
  if (e->getElementType().getEnum() == ElementType::Node)
  {
    NodePtr node = std::dynamic_pointer_cast<Node>(e);
    Coordinate coord = node->toCoordinate();
    try
    {
      _rcf->project(&coord);
    }
    catch(const IllegalArgumentException&)
    {
      LOG_ERROR("Failure projecting node: " << node->toString());
      throw;
    }

    node->setX(coord.x);
    node->setY(coord.y);
  }
}

}

