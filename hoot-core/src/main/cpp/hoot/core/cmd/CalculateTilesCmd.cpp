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
 * @copyright Copyright (C) 2015, 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/io/OsmMapReader.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/util/GeometryUtils.h>
#include <hoot/core/util/OpenCv.h>
#include <hoot/core/conflate/TileBoundsCalculator.h>
#include <hoot/core/util/FileUtils.h>

// Standard
#include <fstream>

namespace hoot
{

class CalculateTilesCmd : public BaseCommand
{

  public:

    static string className() { return "hoot::CalculateTilesCmd"; }

    CalculateTilesCmd() { }

    virtual QString getName() const { return "calculate-tiles"; }

    virtual int runSimple(QStringList args)
    {
      if (args.size() < 2 || args.size() > 3)
      {
        cout << getHelp() << endl << endl;
        throw HootException(QString("%1 takes two to three parameters.").arg(getName()));
      }

      QStringList inputs;
      const QString input = args[0];
      LOG_VAR(input);
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
      LOG_VARD(output);

      long maxNodesPerTile = 5000000; //not sure what a good default for this is
      if (args.size() > 2)
      {
        bool parseSuccess = false;
        maxNodesPerTile = args[2].toLong(&parseSuccess);
        if (!parseSuccess || maxNodesPerTile < 1)
        {
          throw HootException("Invalid maximum nodes per tile value: " + args[2]);
        }
      }
      LOG_VARD(maxNodesPerTile);

      OsmMapPtr map(new OsmMap());
      for (int i = 0; i < inputs.size(); i++)
      {
        boost::shared_ptr<OsmMapReader> reader =
          OsmMapReaderFactory::getInstance().createReader(inputs.at(i), true);
        reader->open(inputs.at(i));
        reader->read(map);
      }

      //1km; TODO: how to derive this...some percentage of the total area or let the user define it?
      TileBoundsCalculator tbc(0.01);
      tbc.setMaxNodesPerBox(maxNodesPerTile);
      //tbc.setSlop(0.1);
      cv::Mat r1, r2;
      tbc.renderImage(map, r1, r2);
      const std::vector< std::vector<geos::geom::Envelope> > tiles = tbc.calculateTiles();

      //return a semi-colon delimited string of bounds obj's
      QString outputTilesStr;
      LOG_VAR(tiles.size());
      for (size_t tx = 0; tx < tiles.size(); tx++)
      {
        LOG_VAR(tiles[tx].size());
        for (size_t ty = 0; ty < tiles[tx].size(); ty++)
        {
          outputTilesStr += QString::fromStdString(tiles[tx][ty].toString()) + ";";
        }
      }
      outputTilesStr.chop(1);
      LOG_VAR(outputTilesStr);
      FileUtils::writeFully(output, outputTilesStr);

      return 0;
    }
};

HOOT_FACTORY_REGISTER(Command, CalculateTilesCmd)

}
