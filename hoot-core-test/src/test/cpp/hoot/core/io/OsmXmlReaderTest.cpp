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
 * @copyright Copyright (C) 2012, 2013, 2014, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/geometry/GeometryUtils.h>

namespace hoot
{

class OsmXmlReaderTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(OsmXmlReaderTest);
  CPPUNIT_TEST(runTest);
  CPPUNIT_TEST(runUseIdTest);
  CPPUNIT_TEST(runUseStatusTest);
  CPPUNIT_TEST(runUncompressTest);
  CPPUNIT_TEST(runDecodeCharsTest);
  CPPUNIT_TEST(runBoundsTest);
  CPPUNIT_TEST(runBoundsLeaveConnectedOobWaysTest);
  CPPUNIT_TEST(runIgnoreDuplicateMergeTest);
  CPPUNIT_TEST_SUITE_END();

public:

  OsmXmlReaderTest() :
  HootTestFixture("test-files/io/OsmXmlReaderTest/", "test-output/io/OsmXmlReaderTest/")
  {
    setResetType(ResetEnvironment);
  }

  void runTest()
  {
    OsmXmlReader uut;

    OsmMapPtr map = std::make_shared<OsmMap>();
    uut.read("test-files/ToyTestA.osm", map);

    CPPUNIT_ASSERT_EQUAL(36, (int)map->getNodes().size());
    CPPUNIT_ASSERT_EQUAL(4, (int)map->getWays().size());
  }

  void runUseIdTest()
  {
    OsmXmlReader uut;

    OsmMapPtr map = std::make_shared<OsmMap>();
    uut.setUseDataSourceIds(true);
    uut.read("test-files/ToyTestA.osm", map);

    CPPUNIT_ASSERT_EQUAL(36,(int)map->getNodes().size());
    CPPUNIT_ASSERT_EQUAL(4, (int)map->getWays().size());

    long min = 1e9;
    long max = -1e9;
    for (NodeMap::const_iterator it = map->getNodes().begin();
         it != map->getNodes().end(); ++it)
    {
      const ConstNodePtr& n = it->second;
      min = std::min(min, n->getId());
      max = std::max(max, n->getId());
    }

    CPPUNIT_ASSERT_EQUAL(-1669793l, min);
    CPPUNIT_ASSERT_EQUAL(-1669723l, max);

    CPPUNIT_ASSERT(map->containsWay(-1669801));
    CPPUNIT_ASSERT(map->containsWay(-1669799));
    CPPUNIT_ASSERT(map->containsWay(-1669797));
    CPPUNIT_ASSERT(map->containsWay(-1669795));
  }

  void runUseStatusTest()
  {
    OsmXmlReader uut;

    OsmMapPtr map = std::make_shared<OsmMap>();
    uut.setUseDataSourceIds(true);
    uut.setUseFileStatus(true);
    uut.setDefaultStatus(Status::Invalid);
    uut.read(_inputPath + "OsmXmlReaderUseStatusTest.osm", map);

    CPPUNIT_ASSERT_EQUAL(104, (int)map->getNodes().size());
    CPPUNIT_ASSERT_EQUAL(17, (int)map->getWays().size());

    HOOT_STR_EQUALS(Status::Unknown1, map->getWay(-12)->getStatus().getEnum());
    HOOT_STR_EQUALS(Status::Conflated, map->getWay(-13)->getStatus().getEnum());
    HOOT_STR_EQUALS(Status::Unknown2, map->getWay(-51)->getStatus().getEnum());
    HOOT_STR_EQUALS(Status::EnumEnd + 1, map->getWay(-14)->getStatus().getEnum());
    HOOT_STR_EQUALS(Status::EnumEnd + 2, map->getWay(-15)->getStatus().getEnum());

    HOOT_STR_EQUALS(0, map->getWay(-12)->getStatus().getInput());
    HOOT_STR_EQUALS(1, map->getWay(-51)->getStatus().getInput());
    HOOT_STR_EQUALS(2, map->getWay(-14)->getStatus().getInput());
    HOOT_STR_EQUALS(3, map->getWay(-15)->getStatus().getInput());

    HOOT_STR_EQUALS("Unknown1", map->getWay(-12)->getStatus().toString());
    HOOT_STR_EQUALS("Unknown2", map->getWay(-51)->getStatus().toString());
    HOOT_STR_EQUALS("Input003", map->getWay(-14)->getStatus().toString());
    HOOT_STR_EQUALS("Input004", map->getWay(-15)->getStatus().toString());
  }

  void runUncompressTest()
  {
    const std::string cmd(
      "gzip -c test-files/ToyTestA.osm > " + _outputPath.toStdString() + "ToyTestA_compressed.osm.gz");
    LOG_DEBUG("Running compress command: " << cmd);

    int retVal;
    if ((retVal = std::system(cmd.c_str())) != 0)
    {
      QString error =
        QString("Error %1 returned from compress command: %2")
          .arg(retVal)
          .arg(QString::fromStdString(cmd));
      throw HootException(error);
    }

    OsmXmlReader uut;
    OsmMapPtr map = std::make_shared<OsmMap>();
    uut.setUseDataSourceIds(true);

    // Excercise the code
    uut.read(_outputPath + "ToyTestA_compressed.osm.gz", map);

    // Check a few things
    CPPUNIT_ASSERT_EQUAL(36,(int)map->getNodes().size());
    CPPUNIT_ASSERT_EQUAL(4, (int)map->getWays().size());

    QFile f(_outputPath + "ToyTestA_compressed.osm.gz");
    CPPUNIT_ASSERT(f.exists());
    CPPUNIT_ASSERT(f.remove());
  }

  void runDecodeCharsTest()
  {
    //This test ensures that characters not allowed in well-formed XML get decoded as read in.
    //Qt's XML reading does this for us automatically.

    OsmXmlReader uut;

    OsmMapPtr map = std::make_shared<OsmMap>();
    uut.read(_inputPath + "runDecodeCharsTest.osm", map);

    int wayCtr = 0;
    for (WayMap::const_iterator it = map->getWays().begin(); it != map->getWays().end(); ++it)
    {
      const ConstWayPtr& w = it->second;
      if (w->getTags().get("note2") == "1")
      {
        HOOT_STR_EQUALS("1 & 2", w->getTags().get("note"));
        wayCtr++;
      }
      else if (w->getTags().get("note2") == "2")
      {
        HOOT_STR_EQUALS("\"3\"", w->getTags().get("note"));
        wayCtr++;
      }
      else if (w->getTags().get("note2") == "3")
      {
        HOOT_STR_EQUALS("0", w->getTags().get("note"));
        wayCtr++;
      }
      else if (w->getTags().get("note2") == "4")
      {
        HOOT_STR_EQUALS("<2>", w->getTags().get("note"));
        wayCtr++;
      }
    }
    CPPUNIT_ASSERT_EQUAL(4, wayCtr);
  }

  void runBoundsTest()
  {   
    // See related note in ServiceOsmApiDbReaderTest::runReadByBoundsTest.

    OsmXmlReader uut;
    uut.setBounds(geos::geom::Envelope(-104.8996,-104.8976,38.8531,38.8552));
    OsmMapPtr map = std::make_shared<OsmMap>();
    uut.read("test-files/ToyTestA.osm", map);
    uut.close();

    CPPUNIT_ASSERT_EQUAL(32, (int)map->getNodes().size());
    CPPUNIT_ASSERT_EQUAL(2, (int)map->getWays().size());
  }

  void runBoundsLeaveConnectedOobWaysTest()
  {
    // This will leave any ways in the output which are outside of the bounds but are directly
    // connected to ways which cross the bounds.

    const QString testFileName = "runBoundsLeaveConnectedOobWaysTest.osm";

    OsmXmlReader uut;
    uut.setBounds(geos::geom::Envelope(38.91362, 38.915478, 15.37365, 15.37506));
    uut.setKeepImmediatelyConnectedWaysOutsideBounds(true);

    // set cropping up for strict bounds handling
    conf().set(ConfigOptions::getBoundsKeepEntireFeaturesCrossingBoundsKey(), false);
    conf().set(ConfigOptions::getBoundsKeepOnlyFeaturesInsideBoundsKey(), true);

    OsmMapPtr map = std::make_shared<OsmMap>();
    uut.read("test-files/ops/ImmediatelyConnectedOutOfBoundsWayTagger/in.osm", map);
    uut.close();
    OsmMapWriterFactory::write(map, _outputPath + testFileName, false, true);

    HOOT_FILE_EQUALS(_inputPath + testFileName, _outputPath + testFileName);
  }

  void runIgnoreDuplicateMergeTest()
  {
    //  This test opens two versions of ToyTestA (with positive IDs) that contain duplicate nodes
    //  and ways that should result in merging them both into ToyTestA
    const QString testFileName = "IgnoreDuplicateMergeTest.osm";

    //  Set the merge ignore duplicate IDs flag
    conf().set(ConfigOptions::getMapMergeIgnoreDuplicateIdsKey(), true);

    OsmXmlReader uut;
    uut.setIgnoreDuplicates(true);
    uut.setUseDataSourceIds(true);
    OsmMapPtr map = std::make_shared<OsmMap>();
    uut.read(_inputPath + "IgnoreDuplicateMergeTest-1.osm", map);
    uut.read(_inputPath + "IgnoreDuplicateMergeTest-2.osm", map);
    uut.close();

    conf().set(ConfigOptions::getWriterIncludeCircularErrorTagsKey(), false);

    OsmMapWriterFactory::write(map, _outputPath + testFileName, false, false);

    //  Since HOOT_FILE_EQUALS uses a reader factory, turn off the flag because it breaks it
    conf().set(ConfigOptions::getMapMergeIgnoreDuplicateIdsKey(), false);

    HOOT_FILE_EQUALS(_inputPath + testFileName, _outputPath + testFileName);
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(OsmXmlReaderTest, "quick");

}
