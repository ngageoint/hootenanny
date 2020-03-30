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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "CollectionRelationCriterion.h"

// hoot
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/elements/Relation.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementCriterion, CollectionRelationCriterion)

CollectionRelationCriterion::CollectionRelationCriterion()
{
}

bool CollectionRelationCriterion::isSatisfied(const ConstElementPtr& e) const
{
  bool result = false;

  if (e->getElementType() == ElementType::Relation)
  {
    ConstRelationPtr r = std::dynamic_pointer_cast<const Relation>(e);

    // This list could get HUGE.
    if (r->getType() == MetadataTags::RelationWaterway() ||
        r->getType() == MetadataTags::RelationNetwork() ||
        r->getType() == MetadataTags::RelationRouteMaster() ||
        r->getType() == MetadataTags::RelationSuperRoute() ||
        r->getType() == MetadataTags::RelationRoute() ||
        r->getType() == MetadataTags::RelationBoundary() ||
        r->getType() == MetadataTags::RelationRestriction() ||
        // not sure about this one yet
        r->getType() == MetadataTags::RelationMultiPolygon())
    {
      result = true;
    }
  }

  return result;
}

}
