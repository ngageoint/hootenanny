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

#include "PolygonCriterion.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/elements/Relation.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/elements/Way.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementCriterion, PolygonCriterion)

PolygonCriterion::PolygonCriterion()
{
}

bool PolygonCriterion::isSatisfied(const ConstElementPtr& e) const
{
  LOG_VART(e->getElementId());

  bool result = false;

  if (e->getElementType() == ElementType::Node)
  {
    return false;
  }
  else if (e->getElementType() == ElementType::Relation)
  {
    ConstRelationPtr r = std::dynamic_pointer_cast<const Relation>(e);
    result |= r->getType() == MetadataTags::RelationBuilding();
    result |= r->getType() == MetadataTags::RelationMultiPolygon();
  }
  else if (e->getElementType() == ElementType::Way)
  {
    ConstWayPtr way = std::dynamic_pointer_cast<const Way>(e);
//    if (!way->isClosedArea())
//    {
//      LOG_TRACE("Way not closed area; crit unsatisfied.");
//      return false;
//    }
    if (way->isValidPolygon() && way->isClosedArea())
    {
      LOG_TRACE("Way is valid closed area; crit satisfied.");
      return true;
    }
  }

//  const Tags& t = e->getTags();
//  for (Tags::const_iterator it = t.constBegin(); it != t.constEnd(); ++it)
//  {
//    const QString kvp = OsmSchema::getInstance().toKvp(it.key(), it.value());
//    const SchemaVertex& tv = OsmSchema::getInstance().getTagVertex(kvp);
//    uint16_t g = tv.geometries;
//    if (g & (OsmGeometries::ClosedWay | OsmGeometries::Area))
//    {
//      result = true;
//      LOG_TRACE("Found closed way or area geometry from kvp: " << kvp << "; crit satisfied.");
//      break;
//    }
//  }

  LOG_VART(result);
  return result;
}

}

