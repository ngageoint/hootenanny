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
 * @copyright Copyright (C) 2005 VividSolutions (http://www.vividsolutions.com/)
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#include "MultiLineStringLocation.h"

// Hoot
#include <hoot/core/algorithms/linearreference/WaySubline.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/elements/Relation.h>
#include <hoot/core/elements/Way.h>

using namespace std;

namespace hoot
{

MultiLineStringLocation::MultiLineStringLocation(ConstOsmMapPtr map, ConstRelationPtr relation,
                                                 const int wayIndex, const WayLocation& wayLocation)
{
  if (relation->getMembers().empty())
    throw HootException("Feature splitting for multi-line string relations requires that the relation has way members.");
  if (relation->getType() != MetadataTags::RelationMultilineString())
    throw HootException("Invalid relation type: " + relation->getType() + " expected multiline string.");
  if (wayIndex < 0 || wayIndex > (int)relation->getMemberCount() - 1)
    throw HootException("Invalid way index: " + QString::number(wayIndex));
  if (relation->getMembers().at(wayIndex).getElementId() != wayLocation.getWay()->getElementId())
  {
    throw HootException("Feature at way index: " + QString::number(wayIndex) + " does not match way assigned to way location: " +
      wayLocation.getWay()->getElementId().toString());
  }

  _wayLocation = wayLocation;

  const vector<RelationData::Entry>& members = relation->getMembers();
  //add sublines for each way beginning with the first way up to the way just before the way
  //containing the way location
  for (size_t i = 0; i < (size_t)wayIndex; i++)
  {
    RelationData::Entry member = members[i];
    ConstElementPtr element = map->getElement(member.getElementId());
    //there could be a relation here, but we don't handle it for now
    if (element->getElementType() != ElementType::Way)
      throw HootException("Feature splitting for multi-line string relations may only occur on relations which contain only ways.");
    ConstWayPtr way = std::dynamic_pointer_cast<const Way>(element);
    _waySublineString.addSubline(
      WaySubline(
        WayLocation(map, way, 0, 0.0),
        WayLocation(map, way, static_cast<int>(way->getNodeCount()) - 1, 0.0)));
  }

  //add line from start of way where way location resides up to the split point
  _waySublineString.addSubline(WaySubline(WayLocation(map, _wayLocation.getWay(), 0, 0.0), _wayLocation));
  LOG_VART(_waySublineString.getSublines().size());
}

}
