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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef LARGEWAYSPLITTER_H
#define LARGEWAYSPLITTER_H

// Std
#include <string>

namespace hoot
{

class OsmMap;
class Way;

/**
 *
 */
/**
 * @brief The LargeWaySplitter class is designed for splitting ways that are too large into smaller
 * ways.
 *
 * If the way length is greater than threshold, it is split into an number of equal sized pieces
 * that are smaller than threshold.
 */
class LargeWaySplitter
{
public:

  static QString className() { return "hoot::LargeWaySplitter"; }

  static int logWarnCount;

  LargeWaySplitter(double threshold);

  void apply(const std::shared_ptr<OsmMap>& map);

  /**
   * @brief splitWays splist large ways into smaller ways.
   * @param threshold the threshold length.
   */
  static void splitWays(const std::shared_ptr<OsmMap>& map, double threshold);

private:

  std::shared_ptr<OsmMap> _map;
  double _threshold;

  void _divideWay(const std::shared_ptr<Way>& way, int numPieces) const;
};

}

#endif // LARGEWAYSPLITTER_H
