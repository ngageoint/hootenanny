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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#include "NetworkEdge.h"


#include <hoot/core/elements/ElementGeometryUtils.h>

namespace hoot
{

NetworkEdge::NetworkEdge()
  : _directed(false)
{
}

NetworkEdge::NetworkEdge(ConstNetworkVertexPtr from, ConstNetworkVertexPtr to, bool directed,
                         ConstElementPtr member)
  : _from(from),
    _to(to),
    _directed(directed)
{
  if (member)
    addMember(member);
}

Meters NetworkEdge::calculateLength(const ConstElementProviderPtr& provider) const
{
  if (_members.size() != 1)
    throw NotImplementedException("Edges with multiple members are not yet supported.");

  return ElementGeometryUtils::calculateLength(_members[0], provider);
}

bool NetworkEdge::contains(const ConstNetworkVertexPtr& v) const
{
  return _from == v || _to == v;
}

QString NetworkEdge::toString() const
{
  QString result;

  QStringList memberIds;
  for (const auto& member : _members)
    memberIds << member->getElementId().toString();

  if (_directed)
    result = QString("%1 -- %2 --> %3").arg(hoot::toString(_from), memberIds.join(","), hoot::toString(_to));
  else
    result = QString("%1 -- %2 -- %3").arg(hoot::toString(_from), memberIds.join(","), hoot::toString(_to));

  return result;
}

}
