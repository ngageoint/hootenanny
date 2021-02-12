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
 * @copyright Copyright (C) 2016, 2017, 2018, 2020 Maxar (http://www.maxar.com/)
 */
#include "MultiUseBuildingCriterion.h"

// hoot
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementCriterion, MultiUseBuildingCriterion)

bool MultiUseBuildingCriterion::isSatisfied(const ConstElementPtr& e) const
{
  const OsmSchema& osmSchema = OsmSchema::getInstance();
  const ElementType elementType = e->getElementType();
  LOG_VART(elementType);
  const Tags& tags = e->getTags();
  LOG_VART(tags);
  LOG_VART(osmSchema.getCategories(e->getTags()).intersects(OsmSchemaCategory::building()));
  LOG_VART(tags.contains("amenity"));
  LOG_VART(osmSchema.getCategories(tags).intersects(OsmSchemaCategory::multiUse()));

  //(element is in a building category OR is an amenity) AND
  //(element has a multi-purpose building tag OR is in a multi-use category)
  return
    (osmSchema.getCategories(e->getTags()).intersects(OsmSchemaCategory::building()) ||
      tags.contains("amenity")) &&
    (tags.get("building:use") == "multipurpose" ||
     osmSchema.getCategories(tags).intersects(OsmSchemaCategory::multiUse()));
}

}
