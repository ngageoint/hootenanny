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
 * @copyright Copyright (C) 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */

#include "InBoundsCriterion.h"

// GEOS
#include <geos/geom/GeometryFactory.h>
#include <geos/util/TopologyException.h>

// hoot
#include <hoot/core/elements/Element.h>
#include <hoot/core/elements/WayUtils.h>
#include <hoot/core/geometry/GeometryUtils.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementCriterion, InBoundsCriterion)

InBoundsCriterion::InBoundsCriterion()
  : _mustCompletelyContain(true),
    _treatWayNodesAsPartOfWays(true)
{
  setConfiguration(conf());
}

InBoundsCriterion::InBoundsCriterion(const bool mustCompletelyContain)
  : _mustCompletelyContain(mustCompletelyContain),
    _treatWayNodesAsPartOfWays(true)
{
}

InBoundsCriterion::~InBoundsCriterion()
{
  _elementConverter.reset();
  _wayNodeCrit.reset();
}

void InBoundsCriterion::setConfiguration(const Settings& conf)
{
  ConfigOptions config = ConfigOptions(conf);
  _mustCompletelyContain = config.getInBoundsCriterionStrict();
  LOG_VART(_mustCompletelyContain);
  setBounds(GeometryUtils::boundsFromString(config.getInBoundsCriterionBounds()));
}

void InBoundsCriterion::setOsmMap(const OsmMap* map)
{
  _map = map->shared_from_this();
  _elementConverter = std::make_shared<ElementToGeometryConverter>(_map);
  _wayNodeCrit = std::make_shared<WayNodeCriterion>(_map);
}

bool InBoundsCriterion::isSatisfied(const ConstElementPtr& e) const
{
  if (!_map)
    throw IllegalArgumentException("No map passed to InBoundsCriterion.");
  if (!_bounds)
    throw IllegalArgumentException("No bounds passed to InBoundsCriterion.");
  if (!_elementConverter)
    throw IllegalArgumentException("No map set on InBoundsCriterion.");
  if (!_wayNodeCrit)
    throw IllegalArgumentException("No way node criterion set on InBoundsCriterion.");
  if (!e)
    return false;
  LOG_VART(e->getElementId());

  const bool isWayNode = _wayNodeCrit->isSatisfied(e);
  LOG_VART(isWayNode);
  if (!isWayNode || (isWayNode && !_treatWayNodesAsPartOfWays))
    return _nonWayNodeInBounds(e);
  else
  {
    std::vector<ConstWayPtr> containingWays = WayUtils::getContainingWaysConst(e->getElementId().getId(), _map);
    LOG_VART(containingWays.size());
    for (const auto& containingWay : containingWays)
    {
      if (containingWay && _nonWayNodeInBounds(containingWay))
          return true;
    }
    return false;
  }
}

bool InBoundsCriterion::_nonWayNodeInBounds(const ConstElementPtr& e) const
{
  std::shared_ptr<geos::geom::Geometry> geom = _elementConverter->convertToGeometry(e);
  if (!geom || geom->isEmpty())
    return false;

  if (_mustCompletelyContain)
  {
    try
    {
      return _bounds->contains(geom.get());
    }
    catch (geos::util::TopologyException&)
    {
      //  If the contains call fails, use the envelope
      return _bounds->contains(geom->getEnvelope().get());
    }
  }
  else
  {
    try
    {
      return _bounds->intersects(geom.get());
    }
    catch (geos::util::TopologyException&)
    {
      //  If the intersects call fails, use the envelope
      return _bounds->intersects(geom->getEnvelope().get());
    }
  }
}

}
