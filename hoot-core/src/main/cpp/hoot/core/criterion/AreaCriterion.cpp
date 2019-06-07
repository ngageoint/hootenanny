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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "AreaCriterion.h"

// hoot
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/criterion/BuildingCriterion.h>
#include <hoot/core/schema/MetadataTags.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementCriterion, AreaCriterion)

AreaCriterion::AreaCriterion()
{
}

bool AreaCriterion::isSatisfied(const ConstElementPtr& e) const
{
  return isSatisfied(e->getTags(), e->getElementType());
}

bool AreaCriterion::isSatisfied(const Tags& tags, const ElementType& elementType) const
{
  bool result = false;

  // don't process if a node
  if (elementType == ElementType::Node)
  {
    return false;
  }

  result |= BuildingCriterion().isSatisfied(tags, elementType);
  LOG_VART(result);
  result |= tags.isTrue(MetadataTags::BuildingPart());
  LOG_VART(result);
  result |= tags.isTrue("area");
  LOG_VART(result);

  // if at least one of the tags is marked as an area, but not a linestring tag then we consider
  // this to be an area feature.
  for (Tags::const_iterator it = tags.constBegin(); it != tags.constEnd(); ++it)
  {
    const SchemaVertex& tv = OsmSchema::getInstance().getTagVertex(it.key() + "=" + it.value());
    uint16_t g = tv.geometries;
    LOG_VART(g);
    if (g & OsmGeometries::Area && !(g & (OsmGeometries::LineString | OsmGeometries::ClosedWay)))
    {
      //LOG_TRACE("Area: " << it.key() << "=" << it.value());
      result = true;
      LOG_VART(result);
      break;
    }
  }

  return result;
}

}
