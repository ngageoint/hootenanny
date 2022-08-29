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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */
#include "EdgeLocation.h"

#include <hoot/core/algorithms/linearreference/WayLocation.h>

using namespace std;

namespace hoot
{

const double EdgeLocation::SLOPPY_EPSILON = WayLocation::SLOPPY_EPSILON;

ConstNetworkVertexPtr EdgeLocation::getVertex(double epsilon) const
{
  if (isExtreme(epsilon) == false)
    throw IllegalArgumentException("Attempted to get a vertex on an edge location that isn't on a vertex.");

  if (_portion < 0.5)
    return _e->getFrom();
  else
    return _e->getTo();
}


Meters EdgeLocation::getOffset(const ConstElementProviderPtr& provider) const
{
  return _portion * _e->calculateLength(provider);
}

std::shared_ptr<EdgeLocation> EdgeLocation::move(const ConstElementProviderPtr& provider, Meters distance) const
{
  Meters l = _e->calculateLength(provider);

  Meters offset = _portion * l + distance;
  Meters portion = min(1.0, max(offset / l, 0.0));

  return std::make_shared<EdgeLocation>(_e, portion);
}

QString EdgeLocation::toString() const
{
  return QString("{ _e: %1, _portion: %2 }").arg(hoot::toString(_e)).arg(_portion);
}

}
