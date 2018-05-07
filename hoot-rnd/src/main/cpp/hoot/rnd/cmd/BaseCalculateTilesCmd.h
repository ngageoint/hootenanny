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

#ifndef BASE_CALCULATE_TILES_CMD_H
#define BASE_CALCULATE_TILES_CMD_H

// Hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/cmd/BaseCommand.h>

namespace hoot
{

class OsmMap;

class BaseCalculateTilesCmd : public BaseCommand
{
public:

  BaseCalculateTilesCmd();

  virtual ~BaseCalculateTilesCmd() {}

protected:

  OsmMapPtr _readInputs(const QStringList inputs);

  std::vector< std::vector<geos::geom::Envelope> > _calculateTiles(
    const long maxNodesPerTile, const double pixelSize, OsmMapPtr map);

  void _writeOutputAsGeoJson(
    const std::vector< std::vector<geos::geom::Envelope> >& tiles,
    const QString outputPath, const bool selectSingleRandomTile = false, int randomSeed = -1);

  void _writeOutputAsOsm(
    const std::vector< std::vector<geos::geom::Envelope> >& tiles, const QString outputPath,
    const bool selectSingleRandomTile = false, int randomSeed = -1);
};

}

#endif // BASE_CALCULATE_TILES_CMD_H
