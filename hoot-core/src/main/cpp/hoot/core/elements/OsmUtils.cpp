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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#include "OsmUtils.h"

// Hoot
#include <hoot/core/util/Log.h>
#include <hoot/core/elements/RelationMemberUtils.h>
#include <hoot/core/elements/WayUtils.h>

using namespace geos::geom;
using namespace std;

namespace hoot
{

QString OsmUtils::getRelationDetailString(const ConstRelationPtr& relation,
                                          const ConstOsmMapPtr& map)
{
  return
    relation->toString() + RelationMemberUtils::getRelationMembersDetailedString(relation, map);
}

QString OsmUtils::getElementDetailString(const ConstElementPtr& element, const ConstOsmMapPtr& map)
{
  QString str;
  str += element->toString() + "\n";
  if (element->getElementType() == ElementType::Way)
  {
    ConstWayPtr way = std::dynamic_pointer_cast<const Way>(element);
    str += WayUtils::getWayNodesDetailedString(way, map) + "\n";
  }
  else if (element->getElementType() == ElementType::Relation)
  {
    ConstRelationPtr relation = std::dynamic_pointer_cast<const Relation>(element);
    str += RelationMemberUtils::getRelationMembersDetailedString(relation, map) + "\n";
  }
  return str;
}

QString OsmUtils::getElementsDetailString(const std::vector<ElementPtr>& elements,
                                          const ConstOsmMapPtr& map)
{
  QString str;
  for (std::vector<ElementPtr>::const_iterator it = elements.begin(); it != elements.end(); ++it)
  {
    str += getElementDetailString(*it, map);
  }
  return str;
}

bool OsmUtils::isChild(const ElementId& elementId, const ConstOsmMapPtr& map)
{
  if (RelationMemberUtils::elementContainedByAnyRelation(elementId, map))
  {
    return true;
  }
  if (elementId.getType() == ElementType::Node &&
      WayUtils::nodeContainedByAnyWay(elementId.getId(), map))
  {
    return true;
  }
  return false;
}

}
