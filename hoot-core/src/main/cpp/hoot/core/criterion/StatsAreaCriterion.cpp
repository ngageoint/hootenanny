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
 * @copyright Copyright (C) 2018, 2021 Maxar (http://www.maxar.com/)
 */

#include "StatsAreaCriterion.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/criterion/BuildingCriterion.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementCriterion, StatsAreaCriterion)

bool StatsAreaCriterion::isSatisfied(const ConstElementPtr& e) const
{
  bool result = false;

  const Tags& t = e->getTags();

  // don't process if a node
  if (e->getElementType() == ElementType::Node)
  {
    return false;
  }

  // don't process if a building part
  if (t.isTrue(MetadataTags::BuildingPart()))
  {
    return false;
  }

  // don't process if a relation
  if (e->getElementType() == ElementType::Relation)
  {
    return false;
  }

  result |= BuildingCriterion().isSatisfied(e);
  result |= t.isTrue("area");

  // if at least one of the tags is marked as an area, but not a linestring tag then we consider
  // this to be an area feature.
  for (Tags::const_iterator it = t.constBegin(); it != t.constEnd(); ++it)
  {
    const SchemaVertex& tv = OsmSchema::getInstance().getTagVertex(it.key() + "=" + it.value());
    uint16_t g = tv.getGeometries();
    if (g & OsmGeometries::Area && !(g & (OsmGeometries::LineString | OsmGeometries::ClosedWay)))
    {
      result = true;
      break;
    }
  }

  return result;
}

}

