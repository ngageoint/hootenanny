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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#include "PoiPolygonPoiCriterion.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/criterion/AreaCriterion.h>
#include <hoot/core/elements/Node.h>
#include <hoot/core/conflate/poi-polygon/PoiPolygonTagIgnoreListReader.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementCriterion, PoiPolygonPoiCriterion)

PoiPolygonPoiCriterion::PoiPolygonPoiCriterion() :
_tagIgnoreList(PoiPolygonTagIgnoreListReader::getInstance().getPoiTagIgnoreList())
{
  LOG_VART(_tagIgnoreList);
}

bool PoiPolygonPoiCriterion::isSatisfied(const ConstElementPtr& e) const
{
  const Tags& tags = e->getTags();

  const bool isNode = e->getElementType() == ElementType::Node;
  if (!isNode)
  {
    return false;
  }

  //see note in PoiPolygonPolyCriterion::isSatisified
  if (OsmSchema::getInstance().containsTagFromList(tags, _tagIgnoreList))
  {
    LOG_TRACE("Contains tag from tag ignore list");
    return false;
  }
  LOG_TRACE("Does not contain tag from tag ignore list");

  LOG_VART(tags.getNames());
  bool isPoi = false;
  if (tags.getNames().size() > 0)
  {
    isPoi = true;
  }
  else
  {
    // TODO: should "use" be added as a category here?
    const bool inABuildingOrPoiCategory =
      OsmSchema::getInstance().getCategories(tags)
        .intersects(OsmSchemaCategory::building() | OsmSchemaCategory::poi());
    isPoi = inABuildingOrPoiCategory;
    LOG_VART(inABuildingOrPoiCategory);
  }
  LOG_VART(isPoi);

  if (!isPoi && ConfigOptions().getPoiPolygonPromotePointsWithAddressesToPois())
  {
    if (_addressParser.hasAddress(*std::dynamic_pointer_cast<const Node>(e)))
    {
      isPoi = true;
    }
  }

  //LOG_VART(e);
  //LOG_VART(isPoi);
  return isPoi;
}

}
