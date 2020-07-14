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
 * @copyright Copyright (C) 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "LinearCriterion.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/elements/Relation.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/elements/Way.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementCriterion, LinearCriterion)

bool LinearCriterion::isSatisfied(const ConstElementPtr& e) const
{
  LOG_VART(e->getElementId());
  //LOG_VART(e);
  bool result = false;

  if (e->getElementType() == ElementType::Node)
  {
    return false;
  }
  else if (e->getElementType() == ElementType::Relation)
  {
    ConstRelationPtr relation = std::dynamic_pointer_cast<const Relation>(e);
    result = isLinearRelation(relation);
  }
  else if (e->getElementType() == ElementType::Way)
  {
    ConstWayPtr way = std::dynamic_pointer_cast<const Way>(e);
    if (way->isClosedArea())
    {
      LOG_TRACE("Way is a closed area, so fails LinearCriterion.");
      return false;
    }
  }

  const Tags& t = e->getTags();
  for (Tags::const_iterator it = t.constBegin(); it != t.constEnd(); ++it)
  {
    const SchemaVertex& tv = OsmSchema::getInstance().getTagVertex(it.key() + "=" + it.value());
    uint16_t g = tv.geometries;

    LOG_VART(g & OsmGeometries::LineString);
    LOG_VART(g & OsmGeometries::Area);

    // We don't want to fail here if the associated schema type supports both a line and a poly. We
    // only care by this point that it does support a line. The previous closed area check will take
    // care of weeding out any polys.
    if (g & OsmGeometries::LineString)
    {
      result = true;
      break;
    }
  }
  LOG_VART(result);

  return result;
}

bool LinearCriterion::isLinearRelation(const ConstRelationPtr& relation)
{
  return relation->getType() == MetadataTags::RelationMultilineString() ||
         relation->getType() == MetadataTags::RelationRoute() ||
         relation->getType() == MetadataTags::RelationBoundary() ||
         relation->getType() == MetadataTags::RelationRouteMaster() ||
         relation->getType() == MetadataTags::RelationSuperRoute() ||
         relation->getType() == MetadataTags::RelationRestriction();
}

}

