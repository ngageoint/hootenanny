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
 * @copyright Copyright (C) 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef REMOVEROUNDABOUTS_H
#define REMOVEROUNDABOUTS_H

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
 * @brief The RemoveRoundabouts class is being developed to handle roundabouts
 * during conflation. The initial approach is to remove roundabouts, and
 * replace them with simple intersections. Post conflation, if the roundabout
 * is in the reference data it will be put back.
 *
 * Note that it is pretty important to run this operation before doing
 * some other things to the map, like splitting intersections!
 */
class RemoveRoundabouts : public OsmMapOperation
{
public:

  /**
   * @brief className - Get classname string
   * @return - "hoot::RemoveRoundabouts"
   */
  static std::string className() { return "hoot::RemoveRoundabouts"; }

  /**
   * @brief RemoveRoundabouts - Default constructor
   */
  RemoveRoundabouts();

  /**
   * @brief apply - Apply the RemoveRoundabouts op
   * @param pMap - Target map
   */
  void apply(boost::shared_ptr<OsmMap>& pMap) override;

  // Remove roundabouts, store them, replace them with simple intersections
  /**
   * @brief removeRoundabouts - Remove roundabouts, store the, replace them
   *                            with simple intersections
   * @param removed - Vector of the removed roundabouts
   */
  void removeRoundabouts(std::vector<RoundaboutPtr> &removed);

  virtual QString getDescription() const override { return "Removes roundabouts from roads"; }

private:

  boost::shared_ptr<OsmMap> _pMap;
  std::vector<long> _todoWays;
};

}

#endif // REMOVEROUNDABOUTS_H
