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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#ifndef __WAY_DISCRETIZER_H__
#define __WAY_DISCRETIZER_H__

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/util/Units.h>

// Tgs
#include <tgs/HashMap.h>

// GEOS
namespace geos
{
  namespace geom
  {
    class Coordinate;
  }
}

namespace hoot
{

class WayLocation;

class WayDiscretizer
{
public:

  WayDiscretizer(const ConstOsmMapPtr& map, const std::shared_ptr<const Way>& way);

  /**
   * @brief discretize given an input way, discretize the way out into discrete coordinates. The
   * first and last nodes are guaranteed to be included.
   * @param spacing the maximum distance between points
   * @param result discretized coordinates
   * @return true if the operation was successful; false otherwise
   */
  bool discretize(double spacing, std::vector<geos::geom::Coordinate>& result);
  /**
   * @brief discretize splits a way up into a number of equally spaced way locations. As long as the
   * way length > 0 you are guaranteed to get at least two points, one for the beginning and one for
   * the end. The above function is much more efficient.
   * @param spacing the maximum distance between points
   * @param result discretized coordinates
   * @return true if the operation was successful; false otherwise
   * @optimize make this more efficient by using the _lengthNodes array.
   */
  bool discretize(double spacing, std::vector<WayLocation>& result) const;

  /**
   * @brief interpolate interpolates the coordinate at the given distance d.
   */
  geos::geom::Coordinate interpolate(double d);

private:

  ConstOsmMapPtr _map;
  std::shared_ptr<const Way> _way;
  // The distance from the beginning of the way to each node that makes up the way.
  std::vector<double> _lengthNodes;
};

}

#endif // __WAY_DISCRETIZER_H_
