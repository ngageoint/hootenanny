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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "CalculateFeatureDimensionsVisitor.h"

// geos
#include <geos/geom/LineString.h>

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/ElementConverter.h>

using namespace geos::geom;

namespace hoot
{

HOOT_FACTORY_REGISTER(ConstElementVisitor, CalculateFeatureDimensionsVisitor)

void CalculateFeatureDimensionsVisitor::visit(const ConstElementPtr& e)
{
  boost::shared_ptr<Geometry> g = ElementConverter(_map->shared_from_this()).convertToGeometry(e, true, true);
  boost::shared_ptr<Element> ee = _map->getElement(e->getElementId());

  if (OsmSchema::getInstance().isArea(e))
  {
    LOG_INFO("Is Area");
//    if (! e->getTags()["feature_area"])
    if (ee->getTags().get("feature_area").isEmpty())
    {
      ee->getTags()["feature_area"] = QString::number(g->getArea(),'f',2);
    }
    else
    {
      LOG_INFO("Has Area Tag: " + ee->getTags()["feature_area"]);
    }

  }
  else if (OsmSchema::getInstance().isLinear(*e))
  {
    LOG_INFO("Is Linear");
//    if (! e->getTags()["length"])
    if (ee->getTags().get("length").isEmpty())
    {
      ee->getTags()["length"] = QString::number(g->getLength(),'f',2);
    }
    else
    {
      LOG_INFO("Has Length Tag: " + ee->getTags()["length"]);
    }

  }
}

}
