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

#ifndef LARGEWAYSPLITTER_H
#define LARGEWAYSPLITTER_H

// Boost
#include <boost/shared_ptr.hpp>

// Std
#include <string>

namespace hoot
{

using namespace boost;

class OsmMap;
class Way;

/**
 * This class is designed for splitting ways that are too large into smaller ways. If the
 * way length is greater than threshold into an number of equal sized pieces that are smaller than
 * threshold.
 */
class LargeWaySplitter
{
public:

  static std::string className() { return "hoot::LargeWaySplitter"; }

  static unsigned int logWarnCount;

  LargeWaySplitter(double threshold);

  void apply(boost::shared_ptr<OsmMap> map);

  /**
   * Split large ways into smaller ways.
   * @param threshold - The threshold length. This projection units.
   */
  static void splitWays(boost::shared_ptr<OsmMap> map, double threshold);

protected:

  boost::shared_ptr<OsmMap> _map;

  double _threshold;

  void _divideWay(boost::shared_ptr<Way> way, int numPieces);
};

}

#endif // LARGEWAYSPLITTER_H
