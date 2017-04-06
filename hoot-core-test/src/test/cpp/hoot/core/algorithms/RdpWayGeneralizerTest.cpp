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
 * @copyright Copyright (C) 2014 DigitalGlobe (http://www.digitalglobe.com/)
 */
// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// boost
#include <boost/random/uniform_real.hpp>
#include <boost/random/linear_congruential.hpp>

// Qt
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QDir>

// Hoot
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/algorithms/RdpWayGeneralizer.h>
#include <hoot/core/io/OsmXmlWriter.h>

#include "../TestUtils.h"

namespace hoot
{

class RdpWayGeneralizerTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(RdpWayGeneralizerTest);
  CPPUNIT_TEST(runPerpendicularDistanceTest);
  CPPUNIT_TEST(runCalcPointsInput1aTest);
  CPPUNIT_TEST(runCalcPointsInput1bTest);
  CPPUNIT_TEST(runCalcPointsInput2aTest);
  CPPUNIT_TEST(runCalcPointsInput2bTest);
  CPPUNIT_TEST(runGeneralizeWayInput1NoInformationNodesTest);
  CPPUNIT_TEST(runGeneralizeWayInput1WithInformationNodesTest);
  CPPUNIT_TEST(runInvalidEpsilonTest);
  CPPUNIT_TEST_SUITE_END();

public:

  QMap<QString, QList<boost::shared_ptr<const Node> > > _inputPointsCache;
  QMap<QString, QList<Coordinate> > _inputCoordsCache;

  QList<boost::shared_ptr<const Node> > readPoints(const QString filePath)
  {
    OsmMap::resetCounters();

    if (_inputPointsCache.contains(filePath))
    {
      return _inputPointsCache[filePath];
    }

    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly) == false)
    {
      throw HootException(QString("Error opening %1").arg(filePath));
    }

    QTextStream textStream(&file);
    QList<boost::shared_ptr<const Node> > points;
    boost::shared_ptr<OsmMap> map(new OsmMap());
    while (!textStream.atEnd())
    {
      QStringList pointParts = textStream.readLine().split(",");
      boost::shared_ptr<const Node> node(
        new Node(
          Status::Unknown1,
          map->createNextNodeId(),
          pointParts.at(0).toDouble(),
          pointParts.at(1).toDouble(),
          15.0));
      points.append(node);
    }
    file.close();

    _inputPointsCache[filePath] = points;
    return points;
  }

  QList<Coordinate> readCoords(const QString filePath)
  {
    if (_inputCoordsCache.contains(filePath))
    {
      return _inputCoordsCache[filePath];
    }

    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly) == false)
    {
      throw HootException(QString("Error opening %1 for writing").arg(filePath));
    }

    QTextStream textStream(&file);
    QList<Coordinate> coordinates;
    while (!textStream.atEnd())
    {
      QStringList pointParts = textStream.readLine().split(",");
      coordinates.append(Coordinate(pointParts.at(0).toDouble(), pointParts.at(1).toDouble()));
    }
    file.close();

    if (!_inputCoordsCache.contains(filePath))
    {
      _inputCoordsCache[filePath] = coordinates;
    }
    return coordinates;
  }

  QString writeMap(boost::shared_ptr<OsmMap> map, const QString outDir, const QString outFileName)
  {
    QDir().mkpath(outDir);
    OsmXmlWriter writer;
    writer.setIncludeHootInfo(true);
    const QString outFile = outDir + "/" + outFileName;
    writer.write(map, outFile);

    return outFile;
  }

  QString writePointOutput(const QList<boost::shared_ptr<const Node> >& points, const QString outDir,
                           const QString outFileName)
  {
    OsmMap::resetCounters();
    boost::shared_ptr<OsmMap> map(new OsmMap());

    //points will be empty for the generalize calls with way inputs instead of points
    for (QList<boost::shared_ptr<const Node> >::const_iterator it = points.constBegin();
         it != points.constEnd(); ++it)
    {
      NodePtr nodeCopy(new Node(*(*it).get()));
      map->addNode(nodeCopy);
    }

    return writeMap(map, outDir, outFileName);
  }

  void runPerpendicularDistanceTest()
  {
    const QList<boost::shared_ptr<const Node> >& inputPoints =
      readPoints("test-files/algorithms/RdpWayGeneralizerTest/RdpWayGeneralizerTestDataset1.txt");
    CPPUNIT_ASSERT_EQUAL(197, inputPoints.size());

    RdpWayGeneralizer generalizer(0.3);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      2.53015,
      generalizer._getPerpendicularDistanceBetweenSplitNodeAndImaginaryLine(
        inputPoints.at(1), inputPoints.at(0), inputPoints.at(inputPoints.size() - 1)),
      0.0001);
  }

  void runCalcPointsInput1aTest()
  {
    const QList<boost::shared_ptr<const Node> >& inputPoints =
      readPoints("test-files/algorithms/RdpWayGeneralizerTest/RdpWayGeneralizerTestDataset1.txt");
    CPPUNIT_ASSERT_EQUAL(197, inputPoints.size());

    RdpWayGeneralizer generalizer(0.1);
    const QList<boost::shared_ptr<const Node> >& outputPoints =
      generalizer.getGeneralizedPoints(inputPoints);
    CPPUNIT_ASSERT_EQUAL(148, outputPoints.size());
    QString outFile =
      writePointOutput(outputPoints, "test-output/algorithms/RdpWayGeneralizerTest",
        "runCalcPointsInput1aTest-out.osm");
    HOOT_FILE_EQUALS(
      "test-files/algorithms/RdpWayGeneralizerTest/runCalcPointsInput1aTest-out.osm",
      outFile);
  }

  void runCalcPointsInput1bTest()
  {
    const QList<boost::shared_ptr<const Node> >& inputPoints =
      readPoints("test-files/algorithms/RdpWayGeneralizerTest/RdpWayGeneralizerTestDataset1.txt");
    CPPUNIT_ASSERT_EQUAL(197, inputPoints.size());

    RdpWayGeneralizer generalizer(5.9);
    const QList<boost::shared_ptr<const Node> >& outputPoints =
      generalizer.getGeneralizedPoints(inputPoints);
    CPPUNIT_ASSERT_EQUAL(22, outputPoints.size());
    QString outFile =
      writePointOutput(outputPoints, "test-output/algorithms/RdpWayGeneralizerTest",
        "runCalcPointsInput1bTest-out.osm");
    HOOT_FILE_EQUALS(
      "test-files/algorithms/RdpWayGeneralizerTest/runCalcPointsInput1bTest-out.osm",
      outFile);
  }

  void runCalcPointsInput2aTest()
  {
    const QList<boost::shared_ptr<const Node> >& inputPoints =
      readPoints("test-files/algorithms/RdpWayGeneralizerTest/RdpWayGeneralizerTestDataset2.txt");
    CPPUNIT_ASSERT_EQUAL(77, inputPoints.size());

    RdpWayGeneralizer generalizer(0.1);
    const QList<boost::shared_ptr<const Node> >& outputPoints =
      generalizer.getGeneralizedPoints(inputPoints);
    CPPUNIT_ASSERT_EQUAL(48, outputPoints.size());
    QString outFile =
      writePointOutput(outputPoints, "test-output/algorithms/RdpWayGeneralizerTest",
        "runCalcPointsInput2aTest-out.osm");
    HOOT_FILE_EQUALS(
      "test-files/algorithms/RdpWayGeneralizerTest/runCalcPointsInput2aTest-out.osm",
      outFile);
  }

  void runCalcPointsInput2bTest()
  {
    const QList<boost::shared_ptr<const Node> >& inputPoints =
      readPoints("test-files/algorithms/RdpWayGeneralizerTest/RdpWayGeneralizerTestDataset2.txt");
    CPPUNIT_ASSERT_EQUAL(77, inputPoints.size());

    RdpWayGeneralizer generalizer(5.9);
    const QList<boost::shared_ptr<const Node> >& outputPoints =
      generalizer.getGeneralizedPoints(inputPoints);
    CPPUNIT_ASSERT_EQUAL(4, outputPoints.size());
    QString outFile =
      writePointOutput(outputPoints, "test-output/algorithms/RdpWayGeneralizerTest",
        "runCalcPointsInput2bTest-out.osm");
    HOOT_FILE_EQUALS(
      "test-files/algorithms/RdpWayGeneralizerTest/runCalcPointsInput2bTest-out.osm",
      outFile);
  }

  void runGeneralizeWayInput1NoInformationNodesTest()
  {
    OsmMap::resetCounters();
    boost::shared_ptr<OsmMap> map(new OsmMap());
    QList<Coordinate> inputCoords =
      readCoords("test-files/algorithms/RdpWayGeneralizerTest/RdpWayGeneralizerTestDataset1.txt");
    CPPUNIT_ASSERT_EQUAL(197, inputCoords.size());
    inputCoords.append(Coordinate::getNull());
    WayPtr way = TestUtils::createWay(map, Status::Unknown1, inputCoords.toVector().data(), 1, "");
    CPPUNIT_ASSERT_EQUAL((size_t)197, way->getNodeIds().size());

    RdpWayGeneralizer generalizer(map, 0.1);
    generalizer.generalize(way);

    CPPUNIT_ASSERT_EQUAL((size_t)197, map->getNodes().size());
    CPPUNIT_ASSERT_EQUAL((size_t)148, way->getNodeIds().size());
    const QString outFile =
      writeMap(map, "test-output/algorithms/RdpWayGeneralizerTest",
        "runGeneralizeWayInput1NoInformationNodesTest-out.osm");
    HOOT_FILE_EQUALS(
      "test-files/algorithms/RdpWayGeneralizerTest/runGeneralizeWayInput1NoInformationNodesTest-out.osm",
      outFile);
  }

  void runGeneralizeWayInput1WithInformationNodesTest()
  {
    OsmMap::resetCounters();
    boost::shared_ptr<OsmMap> map(new OsmMap());
    QList<Coordinate> inputCoords =
      readCoords("test-files/algorithms/RdpWayGeneralizerTest/RdpWayGeneralizerTestDataset1.txt");
    CPPUNIT_ASSERT_EQUAL(197, inputCoords.size());
    inputCoords.append(Coordinate::getNull());
    WayPtr way = TestUtils::createWay(map, Status::Unknown1, inputCoords.toVector().data(), 1, "");
    CPPUNIT_ASSERT_EQUAL((size_t)197, way->getNodeIds().size());

    //randomly add some information tags to nodes
    const double informationCountProbability = 0.1;
    boost::minstd_rand rng;
    rng.seed(1);
    boost::uniform_real<> randomDistribution(0.0, 1.0);
    for (int i = 0; i < (int)way->getNodeCount(); i++)
    {
      const double randomNum = randomDistribution(rng);
      if (randomNum <= informationCountProbability)
      {
        //key and value don't matter here...anything that's not debug or metadata will count as
        //an information tag
        map->getElement(map->getNode(way->getNodeId(i))->getElementId())->getTags().
            set("testKey", "testValue");
      }
    }

    RdpWayGeneralizer generalizer(map, 0.1);
    generalizer.generalize(way);

    CPPUNIT_ASSERT_EQUAL((size_t)197, map->getNodes().size());
    CPPUNIT_ASSERT_EQUAL((size_t)137, way->getNodeIds().size());
    const QString outFile =
      writeMap(map, "test-output/algorithms/RdpWayGeneralizerTest",
        "runGeneralizeWayInput1WithInformationNodesTest-out.osm");
    HOOT_FILE_EQUALS(
      "test-files/algorithms/RdpWayGeneralizerTest/runGeneralizeWayInput1WithInformationNodesTest-out.osm",
      outFile);
  }

  void runInvalidEpsilonTest()
  {
    QString exceptionMsg;
    try
    {
      RdpWayGeneralizer generalizer(0.0);
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT_EQUAL(
      QString("Invalid epsilon value: 0").toStdString(),
      exceptionMsg.toStdString());
  }

};

//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(MultiLineStringSplitterTest, "current");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(RdpWayGeneralizerTest, "quick");

}
