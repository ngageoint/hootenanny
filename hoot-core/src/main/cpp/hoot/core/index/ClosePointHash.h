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
 * @copyright Copyright (C) 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef CLOSEPOINTHASH_H
#define CLOSEPOINTHASH_H

// Standard
#include <stdint.h>
#include <vector>

// TGS
#include <tgs/HashMap.h>

namespace hoot
{

/**
 * Creates a hash with the explicit purpose of finding that are within a predefined distance. This
 * is re-entrant, but not thread safe.
 */
class ClosePointHash
{
public:
  /**
   * @param distance - The maximimum distance that is still considered a match between two points.
   */
  ClosePointHash(double distance);

  /**
   * Location of the point to add. Adding the same ID results in undefined behavior.
   */
  void addPoint(double x, double y, long id);

  /**
   * Returns the ids of the current match. This may have more ids than actually match.
   */
  const std::vector<long>& getMatch();

  /**
   * Returns true if there is another match in the hash. A match may be returned multiple times
   * and the order is arbitrary.
   */
  bool next();

  /**
   * Resets the iterator to the beginning of the hash.
   */
  void resetIterator();

private:

  struct Point
  {
    double x, y;

    Point(double x, double y)
    {
      this->x = x;
      this->y = y;
    }
  };

  double _binSize;
  double _distance;
  std::vector<long> _match;

  HashMap< int64_t, std::vector<long> > _bins;
  HashMap< int64_t, std::vector<long> >::const_iterator _it;

  int64_t _toBin(double x, double y);
};

}

#endif // CLOSEPOINTHASH_H
