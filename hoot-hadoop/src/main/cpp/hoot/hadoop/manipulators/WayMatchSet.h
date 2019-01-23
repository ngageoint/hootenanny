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
 * @copyright Copyright (C) 2015, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef WAYMATCHSET_H
#define WAYMATCHSET_H

// Standard
#include <map>
#include <vector>

// Tgs
#include <tgs/HashMap.h>

namespace hoot {

class WayMatch
{
public:

  /**
   * id1 is always the ID of the way with a status of Unknown1
   */
  WayMatch(long id1, long id2, double asd)
  {
    this->id1 = id1;
    this->id2 = id2;
    approximateScoreDelta = asd;
  }

  long id1;
  long id2;
  double approximateScoreDelta;
};

inline bool operator==(const WayMatch& wm1, const WayMatch& wm2)
{
  return wm1.id1 == wm2.id1 && wm1.id2 == wm2.id2;
}

}

namespace __gnu_cxx
{
  /**
   * Cantor pairing function:
   * http://en.wikipedia.org/wiki/Cantor_pairing_function#Cantor_pairing_function
   */
  template <>
  struct hash<hoot::WayMatch> {
    size_t operator() (const hoot::WayMatch& x) const {
      return hash< std::pair<long, long> >()(std::pair<long, long>(x.id1, x.id2));
    }
  };
}

namespace hoot
{

/**
 * Maintains a set of way matches and provides some simple queries.
 */
class WayMatchSet
{
public:
  WayMatchSet();

  /**
   * Get the number of matches that this id is part of.
   */
  int getMatchCount(long id) { return _matches[id].size(); }

  /**
   * @param id1 Unknown1 way ID
   * @param id1 Unknown2 way ID
   */
  void insertMatch(const WayMatch& match);

  /**
   * @param id way ID (can be either Unknown1 or Unknown2 status)
   */
  const std::vector<WayMatch>& getAllMatches(long id) { return _matches[id]; }

private:


  HashMap<long, std::vector<WayMatch> > _matches;
};

}

#endif // WAYMATCHSET_H
