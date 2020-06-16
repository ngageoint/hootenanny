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
 * @copyright Copyright (C) 2015, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef BOUNDABLE_H
#define BOUNDABLE_H

// geos
#include <geos/geom/Envelope.h>

namespace hoot
{

/**
 * An interface defining a class that may have the bounds set. The exact meaning of setting the
 * bounds is context dependant, but in the case of OsmMapOperations it limits the scope of the
 * operation.
 */
class Boundable
{
public:

  Boundable() = default;
  virtual ~Boundable() = default;

  /**
   * Sets the bounds in WGS84. It is important to note that even if the rest of the operation is
   * not in WGS84 the bounds will still be in WGS84.
   */
  virtual void setBounds(const geos::geom::Envelope& bounds) = 0;
};

}

#endif // BOUNDABLE_H
