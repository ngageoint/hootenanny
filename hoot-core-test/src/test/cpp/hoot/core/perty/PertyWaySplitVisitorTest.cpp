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

// Hoot
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/perty/PertyWaySplitVisitor.h>

// Qt
#include <QString>
#include <QDir>

#include "../TestUtils.h"

using namespace std;

namespace hoot
{

class PertyWaySplitVisitorTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(PertyWaySplitVisitorTest);
  CPPUNIT_TEST(runWaySplitTest);
  CPPUNIT_TEST(runMultiLineStringSplitTest);
  CPPUNIT_TEST(runInvalidProbabilityTest);
  CPPUNIT_TEST(runInvalidMinNodeSpacingTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void setUp()
  {
    TestUtils::resetEnvironment();
  }

  void runWaySplitTest()
  {
    //Log::WarningLevel levelBefore = Log::getInstance().getLevel();
    //Log::getInstance().setLevel(Log::Debug);

    OsmMap::resetCounters();
    OsmXmlReader reader;
    boost::shared_ptr<OsmMap> map(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.setUseDataSourceIds(true);
    reader.read("test-files/perty/PertyWaySplitVisitorTest/PertyWaySplitVisitorTest-in-1.osm", map);
    const int numNodesBeforeSplitting = map->getNodes().size();
    LOG_VARD(numNodesBeforeSplitting);
    const int numWaysBeforeSplitting = map->getWays().size();
    LOG_VARD(numWaysBeforeSplitting)

    MapProjector::projectToPlanar(map);
    PertyWaySplitVisitor waySplitVisitor;
    boost::minstd_rand rng;
    rng.seed(1);
    waySplitVisitor.setRng(rng);
    waySplitVisitor.setWaySplitProbability(0.5);
    waySplitVisitor.setMinNodeSpacing(1.0);
    map->visitRw(waySplitVisitor);
    MapProjector::projectToWgs84(map);
    const int numNewNodesCreatedBySpliting = map->getNodes().size() - numNodesBeforeSplitting;
    LOG_VARD(numNewNodesCreatedBySpliting);
    const int numNewWaysCreatedBySpliting = map->getWays().size() - numWaysBeforeSplitting;
    LOG_VARD(numNewWaysCreatedBySpliting);

    const QString outDir = "test-output/perty/PertyWaySplitVisitorTest/";
    QDir().mkpath(outDir);
    OsmXmlWriter writer;
    writer.setIncludeHootInfo(true);
    const QString outFile = outDir + "/PertyWaySplitVisitorTest-out-1.osm";
    writer.write(map, outFile);

    HOOT_FILE_EQUALS(
      "test-files/perty/PertyWaySplitVisitorTest/PertyWaySplitVisitorTest-out-1.osm", outFile);

    //Log::getInstance().setLevel(levelBefore);
  }

  void runMultiLineStringSplitTest()
  {
    //Log::WarningLevel levelBefore = Log::getInstance().getLevel();
    //Log::getInstance().setLevel(Log::Debug);

    OsmMap::resetCounters();
    OsmXmlReader reader;
    boost::shared_ptr<OsmMap> map(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.setUseDataSourceIds(true);
    reader.read("test-files/perty/PertyWaySplitVisitorTest/PertyWaySplitVisitorTest-in-2.osm", map);
    const int numNodesBeforeSplitting = map->getNodes().size();
    LOG_VARD(numNodesBeforeSplitting);
    const int numWaysBeforeSplitting = map->getWays().size();
    LOG_VARD(numWaysBeforeSplitting)

    MapProjector::projectToPlanar(map);
    PertyWaySplitVisitor waySplitVisitor;
    boost::minstd_rand rng;
    rng.seed(1);
    waySplitVisitor.setRng(rng);
    waySplitVisitor.setWaySplitProbability(0.5);
    waySplitVisitor.setMinNodeSpacing(1.0);
    map->visitRw(waySplitVisitor);
    MapProjector::projectToWgs84(map);
    const int numNewNodesCreatedBySplitting = map->getNodes().size() - numNodesBeforeSplitting;
    LOG_VARD(numNewNodesCreatedBySplitting);
    const int numNewWaysCreatedBySplitting = map->getWays().size() - numWaysBeforeSplitting;
    LOG_VARD(numNewWaysCreatedBySplitting);

    const QString outDir = "test-output/perty/PertyWaySplitVisitorTest/";
    QDir().mkpath(outDir);
    OsmXmlWriter writer;
    writer.setIncludeHootInfo(true);
    const QString outFile = outDir + "/PertyWaySplitVisitorTest-out-2.osm";
    writer.write(map, outFile);

    HOOT_FILE_EQUALS(
      "test-files/perty/PertyWaySplitVisitorTest/PertyWaySplitVisitorTest-out-2.osm", outFile);

    //Log::getInstance().setLevel(levelBefore);
  }

  void runInvalidProbabilityTest()
  {
    QString exceptionMsg;
    try
    {
      PertyWaySplitVisitor waySplitVisitor;
      waySplitVisitor.setWaySplitProbability(-0.1);
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT_EQUAL(
      QString("Invalid split probability: -0.1").toStdString(),
      exceptionMsg.toStdString());
  }

  void runInvalidMinNodeSpacingTest()
  {
    QString exceptionMsg;
    try
    {
      PertyWaySplitVisitor waySplitVisitor;
      waySplitVisitor.setMinNodeSpacing(-0.1);
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT_EQUAL(
      QString("Invalid minimum node spacing: -0.1").toStdString(),
      exceptionMsg.toStdString());
  }
};

//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(PertyDuplicatePoiOpTest, "current");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(PertyWaySplitVisitorTest, "quick");

}
