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

// Boost
#include <boost/random/uniform_int.hpp>

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/io/GmlWriter.h>
#include <hoot/core/io/OgrReader.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/io/OsmPbfReader.h>
#include <hoot/core/io/OsmPbfWriter.h>
#include <hoot/core/io/ShapefileWriter.h>
#include <hoot/core/util/GeometryUtils.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/Progress.h>
#include <hoot/core/visitors/CalculateMapBoundsVisitor.h>

// Qt
#include <QDir>
#include <QFile>

// Tgs
#include <tgs/Statistics/Random.h>

namespace hoot
{

class EvalMoveCmd : public BaseCommand
{
public:

  static string className() { return "hoot::EvalMoveCmd"; }

  EvalMoveCmd() { }

  virtual QString getName() const { return "eval-move"; }

  struct Comparison
  {
    int errorCount;
    int syncMoveError;
    double meanError;
    double sigmaError;
    double errorSum;
    double errorSquareSum;
    double maxError;

    void print()
    {
      Meters deg = GeometryUtils::haversine(Coordinate(0.0, 0.0), Coordinate(0.0, 1.0));

      cout << errorCount << "\t" << syncMoveError << "\t" << meanError << "\t" << maxError <<
              "\t" << sigmaError << "\t" << meanError * deg * 1e6 << "\tum\t" <<
              maxError * deg * 1e6 << "\tum\t" << sigmaError * deg * 1e6 << "\tum";
    }
  };

  boost::shared_ptr<const Node> getNode(const ConstOsmMapPtr& a, QString k, QString v)
  {
    for (NodeMap::const_iterator it = a->getNodes().begin();
      it != a->getNodes().end(); ++it)
    {
      const boost::shared_ptr<const Node>& n = it->second;
      if (n->getTags().contains(k) && n->getTags()[k] == v)
      {
        return n;
      }
    }
    throw HootException("The expected node was not found.");
  }

  Comparison compareMaps(const ConstOsmMapPtr& ref, const ConstOsmMapPtr& test, int pointCount)
  {
    Comparison result;
    result.errorCount = 0;
    result.syncMoveError = 0;
    result.maxError = 0;
    result.errorSum = 0.0;
    result.errorSquareSum = 0.0;


    for (int i = 0; i < pointCount; i++)
    {
      boost::shared_ptr<const Node> ref1 = getNode(ref, "note1", QString::number(i));
      boost::shared_ptr<const Node> test1 = getNode(test, "note1", QString::number(i));
      boost::shared_ptr<const Node> test2 = getNode(test, "note2", QString::number(i));

      if (ref1->toCoordinate() != test1->toCoordinate())
      {
        result.errorCount++;
//        double d = GeometryUtils::haversine(ref1->toCoordinate(), test1->toCoordinate());
//        if (isnan(d))
//        {
//          cout << d << endl;
//          cout << ref1->toCoordinate() << " " << test1->toCoordinate() << endl;
//        }
        double d = ref1->toCoordinate().distance(test1->toCoordinate());
        result.errorSum += d;
        result.errorSquareSum += d * d;
        result.maxError = max(result.maxError, d);
      }
      if (test1->toCoordinate() != test2->toCoordinate())
      {
        result.syncMoveError++;
      }
    }

    result.meanError = result.errorSum / (double)pointCount;
    result.sigmaError = sqrt(result.errorSquareSum / pointCount -
        (result.meanError * result.meanError));

    return result;
  }

  boost::shared_ptr<OsmMap> osmTransform(boost::shared_ptr<OsmMap> map, QString workingDir)
  {
    OsmXmlWriter writer;
    writer.write(map, workingDir + "/EvalMove.osm");
    OsmXmlReader reader;
    boost::shared_ptr<OsmMap> result(new OsmMap());
    reader.read(workingDir + "/EvalMove.osm", result);
    return result;
  }

  boost::shared_ptr<OsmMap> osmPbfTransform(boost::shared_ptr<OsmMap> map, QString workingDir, int granularity)
  {
    boost::shared_ptr<OsmMap> result(new OsmMap());

    QString fn = QString("/EvalMove%1.osm.pbf").arg(granularity);

    OsmPbfWriter writer;
    writer.setGranularity(granularity);
    writer.write(map, workingDir + fn);
    OsmPbfReader reader(false);
    reader.read(workingDir + fn, result);
    return result;
  }

  boost::shared_ptr<OsmMap> shpTransform(boost::shared_ptr<OsmMap> map, QString workingDir)
  {
    boost::shared_ptr<OsmMap> result(new OsmMap());

    ShapefileWriter writer;
    QStringList columns;
    columns << "note1";
    columns << "note2";
    writer.setColumns(columns);
    writer.writePoints(map, workingDir + "/EvalMove.shp");

    OgrReader reader;
    Progress progress(getName());
    reader.setTranslationFile("LowerCase");
    reader.read(workingDir + "/EvalMove.shp", "", result, progress);

    return result;
  }

  boost::shared_ptr<OsmMap> gmlTransform(boost::shared_ptr<OsmMap> map, QString workingDir)
  {
    boost::shared_ptr<OsmMap> result(new OsmMap());

    GmlWriter writer;
    QStringList columns;
    columns << "note1";
    columns << "note2";
    writer.setColumns(columns);
    writer.writePoints(map, workingDir + "/EvalMove.gml");

    OgrReader reader;
    Progress progress(getName());
    reader.setTranslationFile("LowerCase");
    reader.read(workingDir + "/EvalMove.gml", "", result, progress);

    return result;
  }

  virtual int runSimple(QStringList args)
  {
    if (args.size() != 3)
    {
      cout << getHelp() << endl << endl;
      throw HootException(QString("%1 takes three parameters.").arg(getName()));
    }

    bool ok;
    int pointCount = args[0].toInt(&ok);
    if (ok == false)
    {
      throw HootException("point-count was not a valid integer: " + args[0]);
    }
    Envelope bounds = parseEnvelope(args[1]);
    QString workingDir = args[2];
    QDir(".").mkpath(workingDir);

    boost::shared_ptr<OsmMap> map(new OsmMap());

    for (int i = 0; i < pointCount; i++)
    {
      double x = drand48() * bounds.getWidth() + bounds.getMinX();
      double y = drand48() * bounds.getHeight() + bounds.getMinY();

      boost::shared_ptr<Node> n1(new Node(Status::Unknown1, map->createNextNodeId(), x, y, 5));
      n1->setTag("note1", QString::number(i));
      boost::shared_ptr<Node> n2(new Node(Status::Unknown1, map->createNextNodeId(), x, y, 5));
      n2->setTag("note2", QString::number(i));

      map->addNode(n1);
      map->addNode(n2);
    }

    cout << "null transform\t";
    compareMaps(map, map, pointCount).print();
    cout << endl;

    cout << "Memory Copy\t";
    boost::shared_ptr<OsmMap> map2(new OsmMap(map));
    compareMaps(map, map2, pointCount).print();
    cout << endl;

    DisableLog disableLog(std::max(Log::Warn, Log::getInstance().getLevel()));
    boost::shared_ptr<OsmMap> mapReproject(new OsmMap(map));
    MapProjector::projectToPlanar(mapReproject);
    MapProjector::projectToWgs84(mapReproject);
    cout << "Reproject\t";
    compareMaps(map, mapReproject, pointCount).print();
    cout << endl;

    OGREnvelope e1 = CalculateMapBoundsVisitor::getBounds(map);
    e1.MinX = (e1.MinX + e1.MaxX) / 2.0;
    e1.MinY = (e1.MinY + e1.MaxY) / 2.0;
    OGREnvelope e2 = CalculateMapBoundsVisitor::getBounds(map);
    e2.MaxX = (e2.MinX + e2.MaxX) / 2.0;
    e2.MaxY = (e2.MinY + e2.MaxY) / 2.0;
    boost::shared_ptr<OsmMap> mapReproject1(new OsmMap(map));
    boost::shared_ptr<OsmMap> mapReproject2(new OsmMap(map));
    MapProjector::projectToPlanar(mapReproject1, e1);
    MapProjector::projectToPlanar(mapReproject2, e2);
    MapProjector::projectToWgs84(mapReproject1);
    MapProjector::projectToWgs84(mapReproject2);
    cout << "Different Reprojections\t";
    compareMaps(mapReproject1, mapReproject2, pointCount).print();
    cout << endl;

    cout << "Save to .osm\t";
    compareMaps(map, osmTransform(map, workingDir), pointCount).print();
    cout << endl;

    cout << "Save to .osm.pbf (100)\t";
    compareMaps(map, osmPbfTransform(map, workingDir, 100), pointCount).print();
    cout << endl;

    cout << "Save to .osm.pbf (1)\t";
    compareMaps(map, osmPbfTransform(map, workingDir, 1), pointCount).print();
    cout << endl;

    cout << "Save to .shp\t";
    compareMaps(map, shpTransform(map, workingDir), pointCount).print();
    cout << endl;

    cout << "Save to .gml\t";
    compareMaps(map, gmlTransform(map, workingDir), pointCount).print();
    cout << endl;

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, EvalMoveCmd)

}

