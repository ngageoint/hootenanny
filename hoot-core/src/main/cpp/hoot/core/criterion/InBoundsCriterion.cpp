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
 * @copyright Copyright (C) 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "InBoundsCriterion.h"

// GEOS
#include <geos/geom/GeometryFactory.h>

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/elements/Element.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/GeometryUtils.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementCriterion, InBoundsCriterion)

InBoundsCriterion::InBoundsCriterion() :
_mustCompletelyContain(true)
{
  setConfiguration(conf());
}

InBoundsCriterion::InBoundsCriterion(const bool mustCompletelyContain) :
_mustCompletelyContain(mustCompletelyContain)
{
}

void InBoundsCriterion::setConfiguration(const Settings& conf)
{
  ConfigOptions config = ConfigOptions(conf);
  _mustCompletelyContain = config.getInBoundsCriterionStrict();
  LOG_VART(_mustCompletelyContain);
  const geos::geom::Envelope bounds =
    GeometryUtils::envelopeFromConfigString(config.getInBoundsCriterionBounds());
  if (!bounds.isNull())
  {
    setBounds(bounds);
  }
}

void InBoundsCriterion::setBounds(const geos::geom::Envelope& bounds)
{
  LOG_VART(bounds);
  _boundsGeom.reset(geos::geom::GeometryFactory::getDefaultInstance()->toGeometry(&bounds));
  LOG_VART(_boundsGeom->toString());
}

void InBoundsCriterion::setOsmMap(const OsmMap* map)
{
  _map = map->shared_from_this();
  _elementConverter.reset(new ElementConverter(_map));
}

bool InBoundsCriterion::isSatisfied(const ConstElementPtr& e) const
{
  if (!_boundsGeom)
  {
    throw IllegalArgumentException("No bounds passed to InBoundsCriterion.");
  }
  if (!_elementConverter)
  {
    throw IllegalArgumentException("No map set on InBoundsCriterion.");
  }

  LOG_VART(e->getElementId());
  std::shared_ptr<geos::geom::Geometry> geom = _elementConverter->convertToGeometry(e);
  LOG_VART(geom->toString());
  if (_mustCompletelyContain)
  {
    LOG_VART(_boundsGeom->contains(geom.get()));
    return _boundsGeom->contains(geom.get());
  }
  else
  {
    LOG_VART(_boundsGeom->intersects(geom.get()));
    return _boundsGeom->intersects(geom.get());
  }
}

}
