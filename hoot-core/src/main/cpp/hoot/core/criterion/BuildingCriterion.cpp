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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "BuildingCriterion.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/criterion/BuildingCriterion.h>

using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementCriterion, BuildingCriterion)

bool BuildingCriterion::isParentABuilding(ElementId eid) const
{
  bool result = false;

  const std::shared_ptr<ElementToRelationMap>& e2r = _map->getIndex().getElementToRelationMap();
  const set<long>& parents = e2r->getRelationByElement(eid);
  LOG_VART(parents);
  for (set<long>::const_iterator it = parents.begin(); it != parents.end() && result == false;
    ++it)
  {
    ConstElementPtr e = _map->getRelation(*it);
    if (isSatisfied(e))
    {
      result = true;
      LOG_TRACE("isSatisfied=true: " << e->getElementId());
    }
    else
    {
      result = isParentABuilding(e->getElementId());
      LOG_TRACE("isParentABuilding result: " << result << ";" << e->getElementId());
    }
  }

  return result;
}

bool BuildingCriterion::isSatisfied(const ConstElementPtr& e) const
{
  bool result = false;

  LOG_VART(e->getElementId());

  // if it is a building

  LOG_VART(e->getElementType() == ElementType::Node);
  LOG_VART(OsmSchema::getInstance().hasCategory(e->getTags(), "building"));

  if ((e->getElementType() != ElementType::Node) &&
      (OsmSchema::getInstance().hasCategory(e->getTags(), "building") == true))
  {
    // If a map was set, then we assume the parent is to be checked as well. This is a little
    // messy but reflects how the logic worked before moving OsmSchema feature type method logic
    // out to criterion. We may still want to rethink allowing a null map. Another option could be
    // to make two separate criteria, one that checks the parent and one that doesn't.
    LOG_VART(_map.get());
    if (!_map || isParentABuilding(e->getElementId()) == false)
    {
      // see ticket #5952. If the building has a parent relation that is also a building then this
      // is really a building part, not a building.
      result = true;
      LOG_VART(result);
    }
  }

  return result;
}

bool BuildingCriterion::isSatisfied(const Tags& tags, const ElementType& elementType) const
{
  // There's no option to check the parent in this method, since doing so would require an element
  // ID and callers call this method, because they don't have it in certain circumstances.

  LOG_VART(elementType);
  LOG_VART(OsmSchema::getInstance().hasCategory(tags, "building"));

  return
    elementType != ElementType::Node && OsmSchema::getInstance().hasCategory(tags, "building");
}

}
