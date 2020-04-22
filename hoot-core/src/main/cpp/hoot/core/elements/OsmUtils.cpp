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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "OsmUtils.h"

// Hoot
#include <hoot/core/util/Log.h>
#include <hoot/core/criterion/OneWayCriterion.h>
#include <hoot/core/elements/RelationMemberUtils.h>
#include <hoot/core/elements/WayNodeUtils.h>

using namespace geos::geom;
using namespace std;

namespace hoot
{

QString OsmUtils::getRelationDetailedString(const ConstRelationPtr& relation,
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
    str += WayNodeUtils::getWayNodesDetailedString(way, map) + "\n";
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

bool OsmUtils::oneWayConflictExists(const ConstElementPtr& element1,
                                    const ConstElementPtr& element2)
{
  // Technically, this should also take into account reverse one ways and check direction.  Since
  // we have a map pre-op standardizing all the ways to not be reversed, not worrying about it for
  // now.
  OneWayCriterion isAOneWayStreet;
  return
    (isAOneWayStreet.isSatisfied(element1) && explicitlyNotAOneWayStreet(element2)) ||
    (isAOneWayStreet.isSatisfied(element2) && explicitlyNotAOneWayStreet(element1));
}

bool OsmUtils::explicitlyNotAOneWayStreet(const ConstElementPtr& element)
{
  // TODO: use Tags::isFalse here instead
  return element->getTags().get("oneway") == "no";
}

bool OsmUtils::nameConflictExists(const ConstElementPtr& element1, const ConstElementPtr& element2)
{
  return
    element1->getTags().hasName() && element2->getTags().hasName() &&
      !Tags::haveMatchingName(element1->getTags(), element2->getTags());
}

bool OsmUtils::nonGenericHighwayConflictExists(const ConstElementPtr& element1,
                                               const ConstElementPtr& element2)
{
  const QString element1HighwayVal = element1->getTags().get("highway");
  const QString element2HighwayVal = element2->getTags().get("highway");
  return
    element1HighwayVal != "road" && element2HighwayVal != "road" &&
    element1HighwayVal != element2HighwayVal;
}

bool OsmUtils::isChild(const ElementId& elementId, const ConstOsmMapPtr& map)
{
  if (RelationMemberUtils::elementContainedByAnyRelation(elementId, map))
  {
    return true;
  }
  if (elementId.getType() == ElementType::Node &&
      WayNodeUtils::nodeContainedByAnyWay(elementId.getId(), map))
  {
    return true;
  }
  return false;
}

bool OsmUtils::containsMember(const ConstElementPtr& parent, const ElementId& memberId)
{
  if (!parent ||
      (parent->getElementType() != ElementType::Way &&
       parent->getElementType() != ElementType::Relation))
  {
    throw IllegalArgumentException("The parent element is null or of the wrong element type.");
  }
  if (parent->getElementType() != ElementType::Way && memberId.getType() != ElementType::Node)
  {
    throw IllegalArgumentException("The inputs are of the wrong element type.");
  }
  if (parent->getElementType() != ElementType::Relation &&
      memberId.getType() == ElementType::Unknown)
  {
    throw IllegalArgumentException("The inputs are of the wrong element type.");
  }

  bool containsMember = false;
  if (parent->getElementType() == ElementType::Way)
  {
    containsMember =
      (std::dynamic_pointer_cast<const Way>(parent))->containsNodeId(memberId.getId());
  }
  else
  {
    containsMember =
      (std::dynamic_pointer_cast<const Relation>(parent))->contains(memberId);
  }
  return containsMember;
}

}
