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
 * @copyright Copyright (C) 2013, 2014 DigitalGlobe (http://www.digitalglobe.com/)
 */

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/io/ApiDb.h>
#include <hoot/core/io/OsmApiDb.h>
#include <hoot/core/io/OsmApiDbReader.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/MapProjector.h>

// Qt
#include <QDir>

#include "../TestUtils.h"
#include "ServicesDbTestUtils.h"

using namespace std;

namespace hoot
{

class ServiceOsmApiDbReaderTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(ServiceOsmApiDbReaderTest);
  CPPUNIT_TEST(runReadOsmApiTest);
  CPPUNIT_TEST(runReadByBoundsTest);
  CPPUNIT_TEST_SUITE_END();

public:

  static QString userEmail() { return "ServiceOsmApiDbReaderTest@hoottestcpp.org"; }

  void setUp()
  {
    ServicesDbTestUtils::deleteDataFromOsmApiTestDatabase();
  }

  void insertData()
  {
    ////////////////////////////////////////
    // insert simple test data
    ////////////////////////////////////////
    const QString scriptDir = "test-files/servicesdb";
    ApiDb::execSqlFile(ServicesDbTestUtils::getOsmApiDbUrl().toString(), scriptDir + "/users.sql");
    ApiDb::execSqlFile(
      ServicesDbTestUtils::getOsmApiDbUrl().toString(), scriptDir + "/changesets.sql");
    ApiDb::execSqlFile(ServicesDbTestUtils::getOsmApiDbUrl().toString(), scriptDir + "/nodes.sql");
    ApiDb::execSqlFile(ServicesDbTestUtils::getOsmApiDbUrl().toString(), scriptDir + "/ways.sql");
    ApiDb::execSqlFile(
      ServicesDbTestUtils::getOsmApiDbUrl().toString(), scriptDir + "/relations.sql");
  }

  void insertDataForBoundTest()
  {
    const QString scriptDir = "test-files/servicesdb";
    ApiDb::execSqlFile(ServicesDbTestUtils::getOsmApiDbUrl().toString(), scriptDir + "/users.sql");
    ApiDb::execSqlFile(
      ServicesDbTestUtils::getOsmApiDbUrl().toString(),
      scriptDir + "/postgresql_forbounding_test.sql");
  }

  void verifyFullReadOutput(OsmMapPtr map)
  {
    //nodes
    CPPUNIT_ASSERT_EQUAL(2, (int)map->getNodes().size());
    HOOT_STR_EQUALS(true, map->containsNode(1));
    NodePtr node = map->getNode(1);
    CPPUNIT_ASSERT_EQUAL((long)1, node->getId());
    CPPUNIT_ASSERT_EQUAL(38.4, node->getY());
    CPPUNIT_ASSERT_EQUAL(-106.5, node->getX());
    CPPUNIT_ASSERT_EQUAL(15.0, node->getCircularError());
    CPPUNIT_ASSERT_EQUAL(2, node->getTags().size());

    NodePtr node1 = map->getNode(2);
    CPPUNIT_ASSERT_EQUAL((long)2, node1->getId());
    CPPUNIT_ASSERT_EQUAL(38.0, node1->getY());
    CPPUNIT_ASSERT_EQUAL(-104.0, node1->getX());

    //ways
    HOOT_STR_EQUALS(true, map->containsWay(1));
    WayPtr way = map->getWay(1);
    CPPUNIT_ASSERT_EQUAL((long)1, way->getId());
    CPPUNIT_ASSERT_EQUAL(2, (int)way->getNodeCount());
    CPPUNIT_ASSERT_EQUAL((long)1, way->getNodeId(0));
    CPPUNIT_ASSERT_EQUAL((long)2, way->getNodeId(1));
    CPPUNIT_ASSERT_EQUAL(15.0, way->getCircularError());
    CPPUNIT_ASSERT_EQUAL(1, way->getTags().size());

    //relations
    HOOT_STR_EQUALS(true, map->containsRelation(1));
    RelationPtr relation = map->getRelation(1);
    CPPUNIT_ASSERT_EQUAL((long)1, relation->getId());
    vector<RelationData::Entry> relationData = relation->getMembers();
    CPPUNIT_ASSERT_EQUAL(2, (int)relation->getMembers().size());
    HOOT_STR_EQUALS("wayrole", relationData[0].getRole());
    HOOT_STR_EQUALS("noderole",relationData[1].getRole());
    CPPUNIT_ASSERT_EQUAL(15.0, relation->getCircularError());
  }

  void runReadOsmApiTest()
  {
    OsmApiDbReader reader;
    OsmMapPtr map(new OsmMap());

    insertData();

    OsmApiDb database;
    database.open(ServicesDbTestUtils::getOsmApiDbUrl());

    reader.open(ServicesDbTestUtils::getOsmApiDbUrl().toString());
    reader.read(map);

    verifyFullReadOutput(map);
    reader.close();
  }

  void runReadByBoundsTest()
  {
    insertDataForBoundTest();

    OsmApiDb database;
    database.open(ServicesDbTestUtils::getOsmApiDbUrl());
    OsmApiDbReader reader;
    reader.open(ServicesDbTestUtils::getOsmApiDbUrl().toString());
    OsmMapPtr map(new OsmMap());

    reader.setBoundingBox(
      "-78.02265434416296,38.90089748801109,-77.9224564416296,39.00085678801109");
    reader.read(map);

    //quick check to see if the element counts are off...consult the test output for more detail

    //All but one of the seven nodes should be returned.  There are two nodes outside of the
    //requested bounds, but one of them is referenced by a way within the bounds and the other by a
    //relation within the bounds.  The node not returned is outside of the requested bounds and not
    //reference by any other element.
    CPPUNIT_ASSERT_EQUAL(6, (int)map->getNodes().size());
    //All but one of the five ways should be returned.  The way not returned contains all nodes
    //that are out of bounds.
    CPPUNIT_ASSERT_EQUAL(4, (int)map->getWays().size());
    //All but one of the six relations should be returned.  The relation not returned contains all
    //members that are out of bounds.
    CPPUNIT_ASSERT_EQUAL(5, (int)map->getRelations().size());

    QDir().mkpath("test-output/io/ServiceOsmApiDbReaderTest");
    MapProjector::projectToWgs84(map);
    OsmMapWriterFactory::getInstance().write(map,
      "test-output/io/ServiceOsmApiDbReaderTest/runReadByBoundsTest.osm");
    HOOT_STR_EQUALS(
      TestUtils::readFile("test-files/io/ServiceOsmApiDbReaderTest/runReadByBoundsTest.osm"),
      TestUtils::readFile("test-output/io/ServiceOsmApiDbReaderTest/runReadByBoundsTest.osm"));

    //just want to make sure I can read against the same data twice in a row w/o crashing and also
    //make sure I don't get the same result again for a different bounds
    reader.setBoundingBox("-1,-1,1,1");
    map.reset(new OsmMap());
    reader.read(map);

    CPPUNIT_ASSERT_EQUAL(0, (int)map->getNodes().size());
    CPPUNIT_ASSERT_EQUAL(0, (int)map->getWays().size());
    CPPUNIT_ASSERT_EQUAL(0, (int)map->getRelations().size());

    reader.close();
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ServiceOsmApiDbReaderTest, "slow");

}
