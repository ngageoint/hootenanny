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
 * @copyright Copyright (C) 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef REPLACEROUNDABOUTS_H
#define REPLACEROUNDABOUTS_H

// Hoot
#include <hoot/core/ops/OsmMapOperation.h>
#include <hoot/core/conflate/highway/Roundabout.h>

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
 * @brief The ReplaceRoundabouts class is being developed to handle roundabouts
 * during conflation. The initial approach is to remove roundabouts, and
 * replace them with simple intersections. Post conflation, if the roundabout
 * is in the reference data, it will be put back.
 */
class ReplaceRoundabouts : public OsmMapOperation
{
public:

  /**
   * @brief className - Get classname string
   * @return "hoot::ReplaceRoundabouts"
   */
  static std::string className() { return "hoot::ReplaceRoundabouts"; }

  /**
   * @brief ReplaceRoundabouts - default constructor
   */
  ReplaceRoundabouts();

  /**
   * @brief apply - Apply the ReplaceRoundabouts Op to the map.
   * @param pMap - Map to operate on.
   */
  void apply(boost::shared_ptr<OsmMap>& pMap);

  /**
   * @brief replaceRoundabouts - Loops through all the roundabouts stored
   *                             in the map, and tries to put them back / merge
   *                             them back.
   * @param pMap - Map to operate on.
   */
  void replaceRoundabouts(boost::shared_ptr<OsmMap> pMap);

  virtual QString getDescription() const
  { return "Replaces roundabouts in road data with simple intersections"; }

private:

  boost::shared_ptr<OsmMap> _pMap;
};

}

#endif // ROUNDABOUTHANDLER_H
