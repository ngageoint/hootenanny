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
 * @copyright Copyright (C) 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "BuildingOnlyOp.h"

// hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/criterion/ArbitraryCriterion.h>
#include <hoot/core/ops/RemoveRelationByEid.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/visitors/MultiVisitor.h>
#include <hoot/core/visitors/RemoveElementsVisitor.h>
#include <hoot/core/visitors/RemoveTagsVisitor.h>
#include <hoot/core/visitors/ReplaceTagVisitor.h>

// Standard
#include <functional>

using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, BuildingOnlyOp)

void BuildingOnlyOp::apply(std::shared_ptr<OsmMap>& map)
{
  _map = map;

  // Setup a visitor to remove superfluous tags
  QStringList tagsToRemove;
  tagsToRemove.append(MetadataTags::ErrorCircular());
  tagsToRemove.append("OBJECTID");
  tagsToRemove.append("PAGENUMBER");
  tagsToRemove.append("SHAPE_AREA");
  tagsToRemove.append("SHAPE_LENG");
  tagsToRemove.append("hoot:layername");
  RemoveTagsVisitor removeTagVtor(tagsToRemove);

  // Setup a visitor to change uppercase "BUILDING" tag to lower
  ReplaceTagVisitor replaceTagVtor("BUILDING", "yes", "building", "yes");

  // Setup a visitor to remove unwanted relations
  std::function<bool (ConstElementPtr e)> f =
    std::bind(&BuildingOnlyOp::_isBuildingRelation, this, placeholders::_1);
  std::shared_ptr<ArbitraryCriterion> pBuildingCrit(new ArbitraryCriterion(f));
  RemoveElementsVisitor removeEVisitor;
  removeEVisitor.addCriterion(pBuildingCrit);

  // Visit the map once, execute all visitors on each element
  MultiVisitor multiVtor;
  multiVtor.addVisitor(&removeTagVtor);
  multiVtor.addVisitor(reinterpret_cast<ElementOsmMapVisitor*>(&replaceTagVtor));
  multiVtor.addVisitor(reinterpret_cast<ElementOsmMapVisitor*>(&removeEVisitor));
  map->visitRw(multiVtor);
}

bool BuildingOnlyOp::_isBuildingRelation(ConstElementPtr e)
{
  // Is it a building relation?
  if (e->getElementType() == ElementType::Relation)
  {
    const Relation *r = dynamic_cast<const Relation*>(e.get());
    if (r->getType() == MetadataTags::RelationBuilding())
      return true;
  }

  return false;
}

}
