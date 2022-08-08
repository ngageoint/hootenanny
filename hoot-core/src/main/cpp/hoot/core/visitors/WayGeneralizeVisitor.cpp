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
#include "WayGeneralizeVisitor.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/algorithms/RdpWayGeneralizer.h>
#include <hoot/core/elements/MapProjector.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, WayGeneralizeVisitor)

WayGeneralizeVisitor::WayGeneralizeVisitor() :
_epsilon(1.0),
_removeNodesSharedByWays(false),
_totalNodesRemoved(0)
{
}

void WayGeneralizeVisitor::setConfiguration(const Settings& conf)
{
  ConfigOptions configOptions(conf);
  setEpsilon(configOptions.getWayGeneralizerEpsilon());
  _removeNodesSharedByWays = configOptions.getWayGeneralizerRemoveNodesSharedByWays();
  const QString critClass = configOptions.getWayGeneralizerCriterion().trimmed();
  if (!critClass.isEmpty())
  {
    addCriterion(Factory::getInstance().constructObject<ElementCriterion>(critClass));
  }
}

void WayGeneralizeVisitor::setOsmMap(OsmMap* map)
{
  _map = map;
  MapProjector::projectToPlanar(_map->shared_from_this());

  assert(_epsilon != -1.0);
  _generalizer = std::make_shared<RdpWayGeneralizer>(_epsilon);
  _generalizer->setOsmMap(_map);
  _generalizer->setRemoveNodesSharedByWays(_removeNodesSharedByWays);
}

void WayGeneralizeVisitor::addCriterion(const ElementCriterionPtr& crit)
{
  _crit = crit;
}

void WayGeneralizeVisitor::visit(const std::shared_ptr<Element>& element)
{
  if (element->getElementType() == ElementType::Way && (!_crit || _crit->isSatisfied(element)))
  {
    if (!_map)
    {
      throw IllegalArgumentException("No map passed to way generalizer.");
    }
    else if (_map->getProjection()->IsGeographic())
    {
      throw IllegalArgumentException("Input map must be projected to planar.");
    }

    const int numNodesRemoved = _generalizer->generalize(std::dynamic_pointer_cast<Way>(element));
    if (numNodesRemoved > 0)
    {
      _totalNodesRemoved += numNodesRemoved;
      _numAffected++;
    }
  }
  _numProcessed++;
}

}
