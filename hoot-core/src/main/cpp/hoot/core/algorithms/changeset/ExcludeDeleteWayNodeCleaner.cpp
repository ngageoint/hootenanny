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
#include <hoot/core/elements/WayUtils.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/criterion/TagKeyCriterion.h>

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
  TagKeyCriterion excludeDeleteTagCrit(MetadataTags::HootChangeExcludeDelete());

  // if its a way node in the ref map and has the exclude delete tag
  if (_wayNodeCrit.isSatisfied(e) && excludeDeleteTagCrit.isSatisfied(e))
  {
    LOG_VART(e->getElementId());

    // find all the ways that contain it the ref map (its own map)
    const std::set<long> containingRefWayIds =
      WayUtils::getContainingWayIdsByNodeId(e->getId(), _map->shared_from_this());
    LOG_VART(containingRefWayIds.size());
    bool anyWayHasExcludeDelete = false;
    for (std::set<long>::const_iterator wayIdItr = containingRefWayIds.begin();
         wayIdItr != containingRefWayIds.end(); ++wayIdItr)
    {
      ConstWayPtr way = _map->getWay(*wayIdItr);
      LOG_VART(way->getElementId());
      if (way && excludeDeleteTagCrit.isSatisfied(way))
      {
        anyWayHasExcludeDelete = true;
        break;
      }
    }
    LOG_VART(anyWayHasExcludeDelete);

    // If none of the ref containing ways had the exclude delete tag, then the way node shouldn't
    // have it anymore either.
    if (!anyWayHasExcludeDelete)
    {
      LOG_TRACE(
        "Removing changeset exclude delete tag from: " << e->getElementId() <<
        " based on ref map evidence...");
      e->getTags().remove(MetadataTags::HootChangeExcludeDelete());
      _numAffected++;
    }
    // Otherwise, see if any ways contain it in the sec map.
    else if (containingRefWayIds.size() == 0 &&
             !WayUtils::nodeContainedByAnyWay(e->getId(), _comparisonMap))
    {
      // If not, its orphaned and shouldn't be excluded from deletion. The exclude delete tag will
      // never be on any ways in the comparison map, so no need to check for it.
      LOG_TRACE(
        "Removing changeset exclude delete tag from: " << e->getElementId() <<
        " based on sec map evidence...");
      e->getTags().remove(MetadataTags::HootChangeExcludeDelete());
      _numAffected++;
    }
  }

  _numProcessed++;
}

}
