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
 * @copyright Copyright (C) 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef ROUNDABOUTHANDLER_H
#define ROUNDABOUTHANDLER_H

// Hoot
#include <hoot/core/ops/OsmMapOperation.h>
#include <hoot/core/conflate/Roundabout.h>

// Qt
#include <QMultiHash>
#include <QSet>
#include <QMap>
#include <vector>

namespace hoot
{

class OsmMap;
class Way;

/**
 * This class is being developed to handle roundabouts during conflation.
 * The initial approach is to remove roundabouts, and replace them with simple
 * intersections. Post conflation, if the roundabout is in the reference data,
 * it will be put back.
 *
 */
class RoundaboutHandler : public OsmMapOperation
{
public:

  static std::string className() { return "hoot::RoundaboutHandler"; }

  RoundaboutHandler();

  RoundaboutHandler(boost::shared_ptr<OsmMap> pMap);

  void apply(boost::shared_ptr<OsmMap>& pMap);

  // Convenience function
  static void removeRoundabouts(boost::shared_ptr<OsmMap> pMap,
                                std::vector<Roundabout> &removed);

  static void addRoundabouts(boost::shared_ptr<OsmMap> pMap);

  // Remove roundabouts, store them, replace them with simple intersections
  void removeRoundabouts(std::vector<Roundabout> &removed);

  // Go through our list of roundabouts and try to put them back.
  void addRoundabouts();

private:
  boost::shared_ptr<OsmMap> _pMap;
  std::vector<long> _todoWays;
};

}

#endif // ROUNDABOUTHANDLER_H
