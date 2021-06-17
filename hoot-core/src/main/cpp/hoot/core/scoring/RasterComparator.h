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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef RASTERCOMPARATOR_H
#define RASTERCOMPARATOR_H

#include <hoot/core/scoring/BaseComparator.h>

namespace hoot
{

class OsmMap;

/**
 * Compares two OSM maps for consistency of vector location and density. The resulting score is a
 * value from 0 to 1, where 1 is an exact match and 0 is completely different.
 *
 * We refer to this as the Shuey Method. Chad Shuey gave me the idea. -surratt
 */
class RasterComparator : public BaseComparator
{
public:

  /**
   * Takes two maps for comparison as input
   */
  RasterComparator(const std::shared_ptr<OsmMap>& map1, const std::shared_ptr<OsmMap>& map2);
  ~RasterComparator() = default;

  /**
   * @see BaseComparator
   */
  double compareMaps() override;

protected:

  cv::Mat _render1, _render2;
  Meters _wayLengthSum;

  void _dumpImage(cv::Mat& image) const;
  void _renderImage(const std::shared_ptr<OsmMap>& map, cv::Mat& result) const;
};

}

#endif // RASTERCOMPARATOR_H
