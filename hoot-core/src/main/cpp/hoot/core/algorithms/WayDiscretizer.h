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
#ifndef __WAY_DISCRETIZER_H__
#define __WAY_DISCRETIZER_H__

// Boost
#include <tgs/SharedPtr.h>

// GEOS
namespace geos {
  namespace geom {
    class Coordinate;
  }
}

// Hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/util/Units.h>
namespace hoot {
  class Way;
  class WayLocation;
}

// Standard
#include <vector>

// Tgs
#include <tgs/HashMap.h>

namespace hoot
{
using namespace boost;
using namespace std;
using namespace geos::geom;

class WayDiscretizer
{
public:

  WayDiscretizer(const ConstOsmMapPtr& map, boost::shared_ptr<const hoot::Way> way);

  /**
   * Given an input way, discretize the way out into discrete coordinates. The first and last nodes
   * are guaranteed to be included.
   *
   * @param spacing - The maximum distance between points
   */
  void discretize(double spacing, vector<Coordinate>& result);

  /**
   * The above function is much more efficient.
   *
   * Split a way up into a number of equally spaced way locations. As long as the way length > 0
   * you are guaranteed to get at least two points, one for the beginning and one for the end.
   *
   * @optimize make this more efficient by using the _lengthNodes array.
   */
  void discretize(double spacing, vector<WayLocation>& result);

  /**
   * Interpolates the coordinate at the given distance d.
   */
  geos::geom::Coordinate interpolate(double d);

protected:
  ConstOsmMapPtr _map;
  boost::shared_ptr<const hoot::Way> _way;
  // The distance from the beginning of the way to each node that makes up the way.
  vector<double> _lengthNodes;
};

}

#endif // __WAY_DISCRETIZER_H_
