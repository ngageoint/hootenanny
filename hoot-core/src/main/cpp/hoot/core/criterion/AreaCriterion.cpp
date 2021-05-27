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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#include "AreaCriterion.h"

// hoot
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/criterion/BuildingCriterion.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/criterion/AreaWayNodeCriterion.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementCriterion, AreaCriterion)

bool AreaCriterion::isSatisfied(const ConstElementPtr& e) const
{
  LOG_VART(e->getElementId());
  _currentElementId = e->getElementId();
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

  LOG_VART(BuildingCriterion(_map).isSatisfied(tags, elementType));
  LOG_VART(tags.isTrue(MetadataTags::BuildingPart()));
  LOG_VART(tags.isTrue("area"));

  result |= BuildingCriterion(_map).isSatisfied(tags, elementType);
  result |= tags.isTrue(MetadataTags::BuildingPart());
  result |= tags.isTrue("area");

  // If at least one of the tags is marked as an area, but not a linestring tag then we consider
  // this to be an area feature.
  for (Tags::const_iterator it = tags.constBegin(); it != tags.constEnd(); ++it)
  {
    const QString kvp = OsmSchema::toKvp(it.key(), it.value());
    const SchemaVertex& tv = OsmSchema::getInstance().getTagVertex(kvp);
    uint16_t g = tv.geometries;

    LOG_VART(tv.toString());
    LOG_VART(g);
    LOG_VART(g & OsmGeometries::Area);
    LOG_VART(g & (OsmGeometries::LineString | OsmGeometries::ClosedWay));
    LOG_VART(g & OsmGeometries::LineString);
    LOG_VART(g & OsmGeometries::ClosedWay);

    if (g & OsmGeometries::Area && !(g & (OsmGeometries::LineString | OsmGeometries::ClosedWay)))
    {
      QString msg =
        "Found area geometry (non-linestring or closed way) from kvp: " + kvp + "; crit satisfied.";
      if (!_currentElementId.isNull())
      {
        msg += "; ID: " + _currentElementId.toString();
      }
      LOG_TRACE(msg);
      result = true;
      break;
    }
  }

  LOG_VART(result);
  return result;
}

QStringList AreaCriterion::getChildCriteria() const
{
  return QStringList(AreaWayNodeCriterion::className());
}

}
