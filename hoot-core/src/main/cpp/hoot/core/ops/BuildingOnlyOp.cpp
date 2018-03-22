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
 * @copyright Copyright (C) 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "BuildingOnlyOp.h"

// hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/filters/ArbitraryCriterion.h>
#include <hoot/core/ops/RemoveRelationOp.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/visitors/MultiVisitor.h>
#include <hoot/core/visitors/RemoveElementsVisitor.h>
#include <hoot/core/visitors/RemoveTagVisitor.h>
#include <hoot/core/visitors/ReplaceTagVisitor.h>

// bost
#include <boost/function.hpp>

using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, BuildingOnlyOp)

BuildingOnlyOp::BuildingOnlyOp()
{
}

void BuildingOnlyOp::apply(boost::shared_ptr<OsmMap>& map)
{
  _map = map;

  // Setup a visitor to remove superfluous tags
  boost::shared_ptr<RemoveTagVisitor> pRemoveTagVtor(new RemoveTagVisitor());
  pRemoveTagVtor->addKey("error:circular");
  pRemoveTagVtor->addKey("OBJECTID");
  pRemoveTagVtor->addKey("PAGENUMBER");
  pRemoveTagVtor->addKey("SHAPE_AREA");
  pRemoveTagVtor->addKey("SHAPE_LENG");
  pRemoveTagVtor->addKey("hoot:layername");

  // Setup a visitor to change uppercase "BUILDING" tag to lower
  ElementOsmMapVisitorPtr pReplaceTagVtor(new ReplaceTagVisitor("BUILDING", "yes",
                                                                "building", "yes"));
  // Visit the map, execute both visitors on each element
  MultiVisitor multiVtor;
  multiVtor.addVisitor(pRemoveTagVtor);
  multiVtor.addVisitor(pReplaceTagVtor);
  map->visitRw(multiVtor);

  // Remove unwanted relations
  boost::function<bool (ConstElementPtr e)> f = boost::bind(&BuildingOnlyOp::_isBuildingRelation, this, _1);
  boost::shared_ptr<ArbitraryCriterion> pBuildingCrit(new ArbitraryCriterion(f));
  RemoveElementsVisitor removeEVisitor(pBuildingCrit);

  // Visit relations only
  map->visitRelationsRw(removeEVisitor);
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

} // namespace hoot
