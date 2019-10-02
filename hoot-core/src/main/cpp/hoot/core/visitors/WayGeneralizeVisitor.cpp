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
#include "WayGeneralizeVisitor.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/algorithms/RdpWayGeneralizer.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/ops/SuperfluousNodeRemover.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, WayGeneralizeVisitor)

WayGeneralizeVisitor::WayGeneralizeVisitor() :
_epsilon(1.0)
{
}

void WayGeneralizeVisitor::setConfiguration(const Settings& conf)
{
  ConfigOptions configOptions(conf);
  setEpsilon(configOptions.getWayGeneralizerEpsilon());
}

void WayGeneralizeVisitor::setOsmMap(OsmMap* map)
{
  _map = map;

  assert(_epsilon != -1.0);
  _generalizer.reset(new RdpWayGeneralizer(_epsilon));
  _generalizer->setOsmMap(_map);
}

void WayGeneralizeVisitor::visit(const std::shared_ptr<Element>& element)
{
  MapProjector::projectToPlanar(_map->shared_from_this());

  if (element->getElementType() == ElementType::Way)
  {
    if (!_map)
    {
      throw IllegalArgumentException("No map passed to way generalizer.");
    }
    else if (_map->getProjection()->IsGeographic())
    {
      throw IllegalArgumentException("Input map must be projected to planar.");
    }

    _generalizer->generalize(std::dynamic_pointer_cast<Way>(element));
  }
}

}
