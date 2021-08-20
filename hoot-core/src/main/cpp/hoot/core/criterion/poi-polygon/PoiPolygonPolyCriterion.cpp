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
#include "PoiPolygonPolyCriterion.h"

// hoot
#include <hoot/core/criterion/AreaCriterion.h>
#include <hoot/core/criterion/poi-polygon/PoiPolygonPolyWayNodeCriterion.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>

using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementCriterion, PoiPolygonPolyCriterion)

PoiPolygonPolyCriterion::PoiPolygonPolyCriterion() :
_tagIgnoreList(ConfigOptions().getPoiPolygonPolyIgnoreTags())
{
  LOG_VART(_tagIgnoreList);
}

bool PoiPolygonPolyCriterion::isSatisfied(const ConstElementPtr& e) const
{
  LOG_VART(e->getElementId());

  if (e->getElementType() == ElementType::Node)
  {
    return false;
  }

  const Tags& tags = e->getTags();

  // Using this looser definition b/c isLinearHighway will return false if any tag is in an area
  // category and not a linestring category, which still gives us some features we don't want to
  // conflate with poi/poly. Do an empty string check here, b/c oddly at some point in the
  // conflation chain empty highway tags get added (among others) to features and then are removed
  // at a later point...probably something that needs to be looked into.
  if (!tags.get("highway").trimmed().isEmpty())
  {
    LOG_TRACE("tags contain highway");
    return false;
  }

  // types we don't care about at all - see #1172 as to why this can't be handled in the schema
  // files
  if (OsmSchema::getInstance().containsTagFromList(tags, _tagIgnoreList))
  {
    LOG_TRACE("Contains tag from tag ignore list");
    return false;
  }

  // TODO: should "use" be added as a category here?
  const bool inABuildingOrPoiCategory =
    OsmSchema::getInstance().getCategories(tags)
      .intersects(OsmSchemaCategory::building() | OsmSchemaCategory::poi());
  LOG_VART(inABuildingOrPoiCategory);
  LOG_VART(tags.getNames().size());
  LOG_VART(AreaCriterion().isSatisfied(e));
  // isArea includes building too
  const bool isPoly =
    AreaCriterion().isSatisfied(e) && (inABuildingOrPoiCategory || !tags.getNames().empty());

  LOG_VART(isPoly);
  return isPoly;
}

QStringList PoiPolygonPolyCriterion::getChildCriteria() const
{
  return QStringList(PoiPolygonPolyWayNodeCriterion::className());
}

}
