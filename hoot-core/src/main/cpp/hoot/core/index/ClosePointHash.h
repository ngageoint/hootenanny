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
 * @copyright Copyright (C) 2015, 2017, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */

#ifndef CLOSEPOINTHASH_H
#define CLOSEPOINTHASH_H

// Standard
#include <stdint.h>

// TGS
#include <tgs/HashMap.h>

namespace hoot
{

/**
 * Creates a hash with the explicit purpose of finding points which are within a predefined
 * distance.
 *
 * This is re-entrant, but not thread safe.
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
   * New location of the point to add. Removes the old point hashes and adds new hashes.
   */
  void updatePoint(double x, double y, long id);

  /**
   * Returns the ids of the current match during iteration. This may have more ids than actually
   * match.
   */
  const std::vector<long>& getMatch() const;

  // Returns all potential matches for a specific id, works outside of the iteration method.
  std::vector<long> getMatchesFor(long id);

  /**
   * Returns true if there is another match in the hash. A match may be returned multiple times
   * and the order is arbitrary.
   */
  bool next();

  /**
   * Resets the iterator to the beginning of the hash.
   */
  void resetIterator();

  size_t size() const { return _bins.size(); }

  /**
   * Empty out the ClosePointHash object
   */
  void reset();

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
  std::vector<long>& _match;

  using HashToIds = HashMap<int64_t, std::vector<long>>;
  using IdToHashes = HashMap<long, std::vector<int64_t>>;

  HashToIds _bins;
  HashToIds::const_iterator _it;

  IdToHashes _idTobin;

  int64_t _toBin(double x, double y) const;
};

}

#endif // CLOSEPOINTHASH_H
