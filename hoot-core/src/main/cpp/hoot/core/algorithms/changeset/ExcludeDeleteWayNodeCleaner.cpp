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
 * @copyright Copyright (C) 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "ExcludeDeleteWayNodeCleaner.h"

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/criterion/WayNodeCriterion.h>
#include <hoot/core/elements/WayUtils.h>
#include <hoot/core/schema/MetadataTags.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, ExcludeDeleteWayNodeCleaner)

ExcludeDeleteWayNodeCleaner::ExcludeDeleteWayNodeCleaner()
{
}

void ExcludeDeleteWayNodeCleaner::setOsmMap(OsmMap* map)
{
  _map = map;
  _wayNodeCrit.setOsmMap(_map);
}

void ExcludeDeleteWayNodeCleaner::visit(const ElementPtr& e)
{
  // if its a way node
  if (_wayNodeCrit.isSatisfied(e))
  {
    // find all the ways that contain it
    const std::set<long> containingWayIds =
      WayUtils::getContainingWayIdsByNodeId(e->getId(), _map->shared_from_this());
    bool anyWayHasExcludeDelete = false;
    for (std::set<long>::const_iterator wayIdItr = containingWayIds.begin();
         wayIdItr != containingWayIds.end(); ++wayIdItr)
    {
      ConstWayPtr way = _map->getWay(*wayIdItr);
      if (way && way->getTags().contains(MetadataTags::HootChangeExcludeDelete()))
      {
        anyWayHasExcludeDelete = true;
        break;
      }
    }

    // If none of the containing ways had the exclude delete tag, then the way node shouldn't have
    // it anymore either.
    if (!anyWayHasExcludeDelete)
    {
      e->getTags().remove(MetadataTags::HootChangeExcludeDelete());
      _numAffected++;
    }
  }
  _numProcessed++;
}

}
