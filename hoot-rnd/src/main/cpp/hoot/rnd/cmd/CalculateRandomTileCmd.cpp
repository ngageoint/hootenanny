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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/rnd/cmd/BaseCalculateTilesCmd.h>

namespace hoot
{

class CalculateRandomTileCmd : public BaseCalculateTilesCmd
{

public:

  static std::string className() { return "hoot::CalculateRandomTileCmd"; }

  CalculateRandomTileCmd() { }

  virtual QString getName() const { return "tiles-calculate-random"; }

  virtual QString getDescription() const
  { return "(experimental) Calculates bounding boxes based on node density and selects one at random"; }

  virtual int runSimple(QStringList args)
  {
    if (args.size() != 2 && args.size() != 3 && args.size() != 5)
    {
      std::cout << getHelp() << std::endl << std::endl;
      throw HootException(
        QString("%1 takes either two, three, or five parameters.").arg(getName()));
    }

    QStringList inputs;
    const QString input = args[0];
    LOG_VARD(input);
    if (!input.contains(";"))
    {
      inputs.append(input);
    }
    else
    {
      //multiple inputs
      inputs = input.split(";");
    }
    LOG_VARD(inputs);

    const QString output = args[1];
    if (!output.toLower().endsWith(".geojson"))
    {
      throw HootException(
        "Invalid output file format: " + output + ".  Only the GeoJSON output format is " +
        "supported.");
    }
    LOG_VARD(output);

    int randomSeed = -1;
    if (args.size() > 2)
    {
      bool parseSuccess = false;
      randomSeed = args[2].toInt(&parseSuccess);
      if (!parseSuccess || randomSeed < -1)
      {
        throw HootException("Invalid random seed value: " + args[2]);
      }
    }
    LOG_VARD(randomSeed);

    //if either of max nodes per tile or pixel size is specified, then both must be specified

    long maxNodesPerTile = 1000;
    if (args.size() > 3)
    {
      bool parseSuccess = false;
      maxNodesPerTile = args[3].toLong(&parseSuccess);
      if (!parseSuccess || maxNodesPerTile < 1)
      {
        throw HootException("Invalid maximum nodes per tile value: " + args[3]);
      }
    }
    LOG_VARD(maxNodesPerTile);

    double pixelSize = 0.001; //.1km?
    if (args.size() > 3)
    {
      bool parseSuccess = false;
      pixelSize = args[4].toDouble(&parseSuccess);
      if (!parseSuccess || pixelSize <= 0.0)
      {
        throw HootException("Invalid pixel size value: " + args[4]);
      }
    }
    LOG_VARD(pixelSize);

    conf().set(ConfigOptions().getIdGeneratorKey(), "hoot::PositiveIdGenerator");

    OsmMapPtr inputMap = _readInputs(inputs);
    const std::vector< std::vector<geos::geom::Envelope> > tiles =
      _calculateTiles(maxNodesPerTile, pixelSize, inputMap);
    _writeOutputAsGeoJson(tiles, output, true, randomSeed);

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, CalculateRandomTileCmd)

}
