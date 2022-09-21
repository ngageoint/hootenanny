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
 * @copyright Copyright (C) 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#include "OneWayRoadStandardizer.h"

// hoot
#include <hoot/core/criterion/HighwayCriterion.h>
#include <hoot/core/criterion/ReversedRoadCriterion.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/util/Factory.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, OneWayRoadStandardizer)

void OneWayRoadStandardizer::visit(const std::shared_ptr<Element>& e)
{
  if (!e)
    return;

  if (ReversedRoadCriterion(_map->shared_from_this()).isSatisfied(e))
  {
    // must be a way or relation if ReversedRoadCriterion was satisfied
    if (e->getElementType() == ElementType::Way)
    {
      WayPtr road = std::dynamic_pointer_cast<Way>(e);
      road->reverseOrder();
      road->getTags().set("oneway", "yes");
      _numAffected++;
    }
    else
    {
      RelationPtr roadRelation = std::dynamic_pointer_cast<Relation>(e);
      for (const auto& m : roadRelation->getMembers())
      {
        ElementPtr member = _map->getElement(m.getElementId());
        if (member->getElementType() == ElementType::Way)
        {
          // assuming the oneway tag isn't also present on the member
          WayPtr road = std::dynamic_pointer_cast<Way>(member);
          road->reverseOrder();
          _numAffected++;
        }
      }
      roadRelation->getTags().set("oneway", "yes");
    }
  }
}

QStringList OneWayRoadStandardizer::getCriteria() const
{
  return QStringList(HighwayCriterion::className());
}

}
