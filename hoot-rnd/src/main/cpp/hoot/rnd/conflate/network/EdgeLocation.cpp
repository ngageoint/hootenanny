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
 * @copyright Copyright (C) 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "EdgeLocation.h"

namespace hoot
{

const double EdgeLocation::SLOPPY_EPSILON = WayLocation::SLOPPY_EPSILON;

ConstNetworkVertexPtr EdgeLocation::getVertex(double epsilon) const
{
  if (isExtreme(epsilon) == false)
  {
    throw IllegalArgumentException("Attempted to get a vertex on an edge location that isn't on "
     "a vertex.");
  }

  if (_portion < 0.5)
  {
    return _e->getFrom();
  }
  else
  {
    return _e->getTo();
  }
}

QString EdgeLocation::toString() const
{
  return QString("{ _e: %1, _portion: %2 }").arg(hoot::toString(_e)).arg(_portion);
}

}
