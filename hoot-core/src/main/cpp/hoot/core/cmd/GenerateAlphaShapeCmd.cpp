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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

// GEOS
#include <geos/geom/GeometryFactory.h>

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/io/ShapefileWriter.h>
#include <hoot/core/algorithms/alpha-shape/AlphaShapeGenerator.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/io/IoUtils.h>

namespace hoot
{
using namespace std;

class GenerateAlphaShapeCmd : public BaseCommand
{
public:

  static string className() { return "hoot::GenerateAlphaShapeCmd"; }

  GenerateAlphaShapeCmd() {}

  virtual QString getName() const override { return "generate-alpha-shape"; }

  virtual QString getDescription() const override
  { return "Generates a shape that covers a set of features in a map"; }

  virtual int runSimple(QStringList& args) override
  {
    if (args.size() != 4)
    {
      cout << getHelp() << endl << endl;
      throw HootException(QString("%1 takes four parameters.").arg(getName()));
    }
    int i = 0;
    QString pointsPath = args[i++];
    double alpha = args[i++].toDouble();
    double buffer = args[i++].toDouble();
    QString outputPath = args[i++];

    OsmMapPtr pointsMap(new OsmMap());
    IoUtils::loadMap(pointsMap, pointsPath, false, Status::Unknown1);

    OsmMapPtr result = AlphaShapeGenerator(alpha, buffer).generateMap(pointsMap);

    // reproject back into lat/lng
    MapProjector::projectToWgs84(result);

    // save out the result.
    if (outputPath.toLower().endsWith(".shp"))
    {
      ShapefileWriter writer;
      writer.writePolygons(result, outputPath);
    }
    else
    {
      IoUtils::saveMap(result, outputPath);
    }

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, GenerateAlphaShapeCmd)

}
