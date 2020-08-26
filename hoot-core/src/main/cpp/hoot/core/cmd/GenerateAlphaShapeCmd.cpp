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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

// GEOS
#include <geos/geom/GeometryFactory.h>

// Hoot
#include <hoot/core/algorithms/alpha-shape/AlphaShapeGenerator.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/io/IoUtils.h>
#include <hoot/core/io/OsmGeoJsonWriter.h>
#include <hoot/core/io/ShapefileWriter.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/util/StringUtils.h>

// Qt
#include <QElapsedTimer>

namespace hoot
{
using namespace std;

class GenerateAlphaShapeCmd : public BaseCommand
{
public:

  static string className() { return "hoot::GenerateAlphaShapeCmd"; }

  GenerateAlphaShapeCmd() = default;

  virtual QString getName() const override { return "generate-alpha-shape"; }

  virtual QString getDescription() const override
  { return "Generates a shape that covers a set of features in a map"; }

  virtual int runSimple(QStringList& args) override
  {
    QElapsedTimer timer;
    timer.start();

    switch (args.size())
    {
    default:
      cout << getHelp() << endl << endl;
      throw HootException(QString("%1 takes two, four, or six parameters.").arg(getName()));
      break;
    case 2:   //  Input/output filenames only
    case 4:   //  Input/output filenames and one of '--alpha' or '--buffer'
    case 6:   //  Input/output filenames and both of '--alpha' and '--buffer'
      break;
    }
    //  Parse and remove the alpha value from the arguments, if present
    double alpha = -1.0;
    if (args.contains("--alpha"))
    {
      int index = args.indexOf("--alpha");
      if (index == -1 || index >= args.size() - 1 || args.at(index + 1).startsWith("--"))
        throw HootException(QString("%1 takes --alpha followed by a positive number.").arg(getName()));

      bool ok = false;
      alpha = args.at(index + 1).toDouble(&ok);
      if (!ok || alpha <= 0.0)
        throw HootException(QString("%1 takes --alpha followed by a positive number.").arg(getName()));

      args.removeAt(index + 1);
      args.removeAll("--alpha");
    }
    //  Parse and remove the buffer value from the arguments, if present
    double buffer = 0.0;
    if (args.contains("--buffer"))
    {
      int index = args.indexOf("--buffer");
      if (index == -1 || index >= args.size() - 1 || args.at(index + 1).startsWith("--"))
        throw HootException(QString("%1 takes --buffer followed by a non-negative number.").arg(getName()));

      bool ok = false;
      buffer = args.at(index + 1).toDouble(&ok);
      if (!ok)
        throw HootException(QString("%1 takes --buffer followed by a number.").arg(getName()));

      args.removeAt(index + 1);
      args.removeAll("--buffer");
    }
    //  Get the input and output paths from the two remaining arguments
    QString pointsPath = args[0];
    QString outputPath = args[1];

    OsmMapPtr pointsMap(new OsmMap());
    IoUtils::loadMap(pointsMap, pointsPath, false, Status::Unknown1);

    AlphaShapeGenerator generator(alpha, buffer);
    //generator.setManuallyCoverSmallPointClusters(false);
    OsmMapPtr result = generator.generateMap(pointsMap);

    // reproject back into lat/lng
    MapProjector::projectToWgs84(result);

    // save out the result.
    if (outputPath.toLower().endsWith(".shp"))
    {
      ShapefileWriter writer;
      writer.writePolygons(result, outputPath);
    }
    else if (outputPath.toLower().endsWith(".geojson"))
    {
      OsmGeoJsonWriter writer;
      Settings s;
      //  Output the source tags in the geojson writer
      s.set(ConfigOptions::getJsonOutputTaskingManagerAoiKey(), true);
      writer.setConfiguration(s);
      writer.open(outputPath);
      writer.write(result);
    }
    else
    {
      IoUtils::saveMap(result, outputPath);
    }

    LOG_STATUS(
      "Alpha shape written to " << outputPath << " in " <<
       StringUtils::millisecondsToDhms(timer.elapsed()) << " total.");

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, GenerateAlphaShapeCmd)

}
