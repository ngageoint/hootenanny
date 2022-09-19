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
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2014, 2015, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

// boost
#include <boost/random/uniform_real.hpp>
#include <boost/random/linear_congruential.hpp>

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/algorithms/RdpWayGeneralizer.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/io/OsmXmlWriter.h>

using namespace geos::geom;

namespace hoot
{

class RdpWayGeneralizerTest : public HootTestFixture
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

  QMap<QString, QList<ConstNodePtr>> _inputPointsCache;
  QMap<QString, QList<Coordinate>> _inputCoordsCache;

  RdpWayGeneralizerTest()
    : HootTestFixture("test-files/algorithms/RdpWayGeneralizerTest/",
                      "test-output/algorithms/RdpWayGeneralizerTest/")
  {
  }

  QList<ConstNodePtr> readPoints(const QString& filePath)
  {
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
    QList<ConstNodePtr> points;
    OsmMapPtr map = std::make_shared<OsmMap>();
    while (!textStream.atEnd())
    {
      QStringList pointParts = textStream.readLine().split(",");
      ConstNodePtr node =
        std::make_shared<const Node>(
          Status::Unknown1,
          map->createNextNodeId(),
          pointParts.at(0).toDouble(),
          pointParts.at(1).toDouble(),
          15.0);
      points.append(node);
    }
    file.close();

    _inputPointsCache[filePath] = points;
    return points;
  }

  QList<Coordinate> readCoords(const QString& filePath)
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

  void writeMap(OsmMapPtr map, const QString& outFileName)
  {
    OsmXmlWriter writer;
    writer.setIncludeHootInfo(true);
    writer.write(map, outFileName);
  }

  void writePointOutput(const QList<ConstNodePtr>& points, const QString& outFileName)
  {
    OsmMapPtr map = std::make_shared<OsmMap>();

    // Points will be empty for the generalize calls with way inputs instead of points.
    for (QList<ConstNodePtr>::const_iterator it = points.constBegin();
         it != points.constEnd(); ++it)
    {
      NodePtr nodeCopy = std::make_shared<Node>(*(*it).get());
      map->addNode(nodeCopy);
    }

    writeMap(map, outFileName);
  }

  void runPerpendicularDistanceTest()
  {
    const QList<ConstNodePtr>& inputPoints =
      readPoints(_inputPath + "RdpWayGeneralizerTestDataset1.txt");
    CPPUNIT_ASSERT_EQUAL(197, inputPoints.size());

    RdpWayGeneralizer generalizer(0.3);
    generalizer.setRemoveNodesSharedByWays(true);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      2.53015,
      generalizer._getPerpendicularDistanceBetweenSplitNodeAndImaginaryLine(
        inputPoints.at(1), inputPoints.at(0), inputPoints.at(inputPoints.size() - 1)),
      0.0001);
  }

  void runCalcPointsInput1aTest()
  {
    const QList<ConstNodePtr>& inputPoints =
      readPoints(_inputPath + "RdpWayGeneralizerTestDataset1.txt");
    CPPUNIT_ASSERT_EQUAL(197, inputPoints.size());

    RdpWayGeneralizer generalizer(0.1);
    generalizer.setRemoveNodesSharedByWays(true);
    const QList<ConstNodePtr>& outputPoints =
      generalizer._getGeneralizedPoints(inputPoints);
    CPPUNIT_ASSERT_EQUAL(148, outputPoints.size());
    QString outFile = _outputPath + "runCalcPointsInput1aTest-out.osm";
    writePointOutput(outputPoints, outFile);
    HOOT_FILE_EQUALS(_inputPath + "runCalcPointsInput1aTest-out.osm", outFile);
  }

  void runCalcPointsInput1bTest()
  {
    const QList<ConstNodePtr>& inputPoints =
      readPoints(_inputPath + "RdpWayGeneralizerTestDataset1.txt");
    CPPUNIT_ASSERT_EQUAL(197, inputPoints.size());

    RdpWayGeneralizer generalizer(5.9);
    generalizer.setRemoveNodesSharedByWays(true);
    const QList<ConstNodePtr>& outputPoints =
      generalizer._getGeneralizedPoints(inputPoints);
    CPPUNIT_ASSERT_EQUAL(22, outputPoints.size());
    QString outFile = _outputPath + "runCalcPointsInput1bTest-out.osm";
    writePointOutput(outputPoints, outFile);
    HOOT_FILE_EQUALS(_inputPath + "runCalcPointsInput1bTest-out.osm", outFile);
  }

  void runCalcPointsInput2aTest()
  {
    const QList<ConstNodePtr>& inputPoints =
      readPoints(_inputPath + "RdpWayGeneralizerTestDataset2.txt");
    CPPUNIT_ASSERT_EQUAL(77, inputPoints.size());

    RdpWayGeneralizer generalizer(0.1);
    generalizer.setRemoveNodesSharedByWays(true);
    const QList<ConstNodePtr>& outputPoints =
      generalizer._getGeneralizedPoints(inputPoints);
    CPPUNIT_ASSERT_EQUAL(48, outputPoints.size());
    QString outFile = _outputPath + "runCalcPointsInput2aTest-out.osm";
    writePointOutput(outputPoints, outFile);
    HOOT_FILE_EQUALS(_inputPath + "runCalcPointsInput2aTest-out.osm", outFile);
  }

  void runCalcPointsInput2bTest()
  {
    const QList<ConstNodePtr>& inputPoints =
      readPoints(_inputPath + "RdpWayGeneralizerTestDataset2.txt");
    CPPUNIT_ASSERT_EQUAL(77, inputPoints.size());

    RdpWayGeneralizer generalizer(5.9);
    generalizer.setRemoveNodesSharedByWays(true);
    const QList<ConstNodePtr>& outputPoints =
      generalizer._getGeneralizedPoints(inputPoints);
    CPPUNIT_ASSERT_EQUAL(4, outputPoints.size());
    QString outFile = _outputPath + "runCalcPointsInput2bTest-out.osm";
    writePointOutput(outputPoints, outFile);
    HOOT_FILE_EQUALS(_inputPath + "runCalcPointsInput2bTest-out.osm", outFile);
  }

  void runGeneralizeWayInput1NoInformationNodesTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    QList<Coordinate> inputCoords =
      readCoords(_inputPath + "RdpWayGeneralizerTestDataset1.txt");
    CPPUNIT_ASSERT_EQUAL(197, inputCoords.size());
    inputCoords.append(Coordinate::getNull());
    WayPtr way = TestUtils::createWay(map, inputCoords.toVector().data(), "", Status::Unknown1, 1);
    CPPUNIT_ASSERT_EQUAL((size_t)197, way->getNodeCount());

    RdpWayGeneralizer generalizer(0.1);
    generalizer.setRemoveNodesSharedByWays(true);
    generalizer.setOsmMap(map.get());
    generalizer.generalize(way);

    CPPUNIT_ASSERT_EQUAL((long)148, map->getNodeCount());
    CPPUNIT_ASSERT_EQUAL((size_t)148, way->getNodeCount());
    const QString outFile = _outputPath + "runGeneralizeWayInput1NoInformationNodesTest-out.osm";
    writeMap(map, outFile);
    HOOT_FILE_EQUALS(_inputPath + "runGeneralizeWayInput1NoInformationNodesTest-out.osm", outFile);
  }

  void runGeneralizeWayInput1WithInformationNodesTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    QList<Coordinate> inputCoords =
      readCoords(_inputPath + "RdpWayGeneralizerTestDataset1.txt");
    CPPUNIT_ASSERT_EQUAL(197, inputCoords.size());
    inputCoords.append(Coordinate::getNull());
    WayPtr way = TestUtils::createWay(map, inputCoords.toVector().data(), "", Status::Unknown1, 1);
    CPPUNIT_ASSERT_EQUAL((size_t)197, way->getNodeCount());

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

    RdpWayGeneralizer generalizer(0.1);
    generalizer.setRemoveNodesSharedByWays(true);
    generalizer.setOsmMap(map.get());
    generalizer.generalize(way);

    CPPUNIT_ASSERT_EQUAL((long)137, map->getNodeCount());
    CPPUNIT_ASSERT_EQUAL((size_t)137, way->getNodeCount());
    const QString outFile = _outputPath + "runGeneralizeWayInput1WithInformationNodesTest-out.osm";
    writeMap(map, outFile);
    HOOT_FILE_EQUALS(_inputPath + "runGeneralizeWayInput1WithInformationNodesTest-out.osm", outFile);
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
      QString("Invalid epsilon value: 0").toStdString(), exceptionMsg.toStdString());
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(RdpWayGeneralizerTest, "quick");

}
