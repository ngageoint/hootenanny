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
 * @copyright Copyright (C) 2013, 2014, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/elements/ElementAttributeType.h>
#include <hoot/core/io/ApiDb.h>
#include <hoot/core/io/OsmApiDb.h>
#include <hoot/core/io/OsmApiDbReader.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/io/ServicesDbTestUtils.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/io/OsmApiDbBulkInserter.h>
#include <hoot/core/visitors/RemoveAttributesVisitor.h>

using namespace std;

namespace hoot
{

class ServiceOsmApiDbReaderTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(ServiceOsmApiDbReaderTest);
  CPPUNIT_TEST(runReadOsmApiTest);
  CPPUNIT_TEST(runReadByBoundsTest);
  CPPUNIT_TEST(runPartialReadTest);
  CPPUNIT_TEST(readByBoundsLeaveConnectedOobWaysTest);
  CPPUNIT_TEST_SUITE_END();

public:

  static QString userEmail() { return "ServiceOsmApiDbReaderTest@hoottestcpp.org"; }

  const QString _scriptDir = "test-files/servicesdb/";

  ServiceOsmApiDbReaderTest()
    : HootTestFixture("test-files/io/ServiceOsmApiDbReaderTest/",
                      "test-output/io/ServiceOsmApiDbReaderTest/")
  {
  }

  virtual void setUp()
  {
    ServicesDbTestUtils::deleteDataFromOsmApiTestDatabase();
  }

  void insertData()
  {
    ////////////////////////////////////////
    // insert simple test data
    ////////////////////////////////////////
    ApiDb::execSqlFile(ServicesDbTestUtils::getOsmApiDbUrl().toString(), _scriptDir + "users.sql");
    ApiDb::execSqlFile(ServicesDbTestUtils::getOsmApiDbUrl().toString(), _scriptDir + "changesets.sql");
    ApiDb::execSqlFile(ServicesDbTestUtils::getOsmApiDbUrl().toString(), _scriptDir + "nodes.sql");
    ApiDb::execSqlFile(ServicesDbTestUtils::getOsmApiDbUrl().toString(), _scriptDir + "ways.sql");
    ApiDb::execSqlFile(ServicesDbTestUtils::getOsmApiDbUrl().toString(), _scriptDir + "relations.sql");
  }

  void insertDataForBoundTest()
  {
    ApiDb::execSqlFile(ServicesDbTestUtils::getOsmApiDbUrl().toString(), _scriptDir + "users.sql");
    ApiDb::execSqlFile(
      ServicesDbTestUtils::getOsmApiDbUrl().toString(),
      _scriptDir + "postgresql_forbounding_test.sql");
  }

  void populatePartialMap()
  {
    OsmMapPtr map = ServicesDbTestUtils::createServiceTestMap();

    //It isn't the best thing in the world for this test to have a dependency on
    //OsmApiDbBulkInserter, but it will do for now.
    ApiDb::execSqlFile(ServicesDbTestUtils::getOsmApiDbUrl().toString(), _scriptDir + "users.sql");
    OsmApiDbBulkInserter writer;
    writer.setChangesetUserId(1);
    writer.open(ServicesDbTestUtils::getOsmApiDbUrl().toString());
    writer.write(map);
    writer.close();
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
    OsmMapPtr map = std::make_shared<OsmMap>();

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
    OsmMapPtr map = std::make_shared<OsmMap>();

    // The default behavior is return both features entirely within the bounds and those that
    // cross the bounds. This uses MapCropper internally, so other variations on the cropping are
    // tested in MapCropperTest.

    reader.setBoundingBox("-88.1,28.89,-88.0,28.91");
    reader.read(map);

    //quick check to see if the element counts are off...consult the test output for more detail

    //All but two of the seven nodes should be returned.  There are five nodes outside of the
    //requested bounds, one of them is referenced by a way within the bounds and the other three by a
    //relation within the bounds.  The nodes not returned is outside of the requested bounds and not
    //reference by any other element.
    CPPUNIT_ASSERT_EQUAL(5, (int)map->getNodes().size());
    //Two of the five ways should be returned.  The ways not returned contain nodes
    //that are out of bounds.
    CPPUNIT_ASSERT_EQUAL(2, (int)map->getWays().size());
    //Two of the six relations should be returned.  The relations not returned contain all
    //members that are out of bounds.
    CPPUNIT_ASSERT_EQUAL(2, (int)map->getRelations().size());

    // Verify timestamps look OK
    WayPtr pWay = map->getWays().begin()->second;
    CPPUNIT_ASSERT(0 != pWay->getTimestamp());

    //Need to remove timestamps, otherwise they cause issues with the compare
    QList<ElementAttributeType> types;
    types.append(ElementAttributeType(ElementAttributeType::Timestamp));
    RemoveAttributesVisitor attrVis(types);
    map->visitRw(attrVis);

    MapProjector::projectToWgs84(map);
    OsmMapWriterFactory::write(map, _outputPath + "runReadByBoundsTest.osm");
    HOOT_FILE_EQUALS(_inputPath + "runReadByBoundsTest.osm",
                     _outputPath + "runReadByBoundsTest.osm");

    //just want to make sure I can read against the same data twice in a row w/o crashing and also
    //make sure I don't get the same result again for a different bounds
    reader.setBoundingBox("-1,-1,1,1");
    map = std::make_shared<OsmMap>();
    reader.read(map);

    CPPUNIT_ASSERT_EQUAL(0, (int)map->getNodes().size());
    CPPUNIT_ASSERT_EQUAL(0, (int)map->getWays().size());
    CPPUNIT_ASSERT_EQUAL(0, (int)map->getRelations().size());

    reader.close();
  }

  void runPartialReadTest()
  {
    //The differences in tag counts here when compared to
    //ServiceHootApiDbReaderTest::runPartialReadTest are due to differences between the way
    //HootApiDbWriter and OsmApiDbBulkInserter handle metadata tags, which is by design.

    populatePartialMap();

    OsmApiDbReader reader;
    const int chunkSize = 3;
    reader.setMaxElementsPerMap(chunkSize);
    reader.open(ServicesDbTestUtils::getOsmApiDbUrl().toString());
    reader.initializePartial();

    int ctr = 0;
    OsmMapPtr map = std::make_shared<OsmMap>();

    //3 nodes

    CPPUNIT_ASSERT(reader.hasMoreElements());
    reader.readPartial(map);
    CPPUNIT_ASSERT_EQUAL(3, (int)map->getNodes().size());
    CPPUNIT_ASSERT_EQUAL(0, (int)map->getWays().size());
    CPPUNIT_ASSERT_EQUAL(0, (int)map->getRelations().size());

    NodePtr node = map->getNode(1);
    CPPUNIT_ASSERT_EQUAL((long)1, node->getId());
    CPPUNIT_ASSERT_EQUAL(0.0, node->getX());
    CPPUNIT_ASSERT_EQUAL(0.0, node->getY());
    CPPUNIT_ASSERT_EQUAL(0, node->getTags().size());

    node = map->getNode(2);
    CPPUNIT_ASSERT_EQUAL((long)2, node->getId());
    CPPUNIT_ASSERT_EQUAL(0.1, node->getX());
    CPPUNIT_ASSERT_EQUAL(0.0, node->getY());
    CPPUNIT_ASSERT_EQUAL(1, node->getTags().size());
    HOOT_STR_EQUALS("n2b", node->getTags().get("noteb"));

    node = map->getNode(3);
    CPPUNIT_ASSERT_EQUAL((long)3, node->getId());
    CPPUNIT_ASSERT_EQUAL(0.2, node->getX());
    CPPUNIT_ASSERT_EQUAL(0.0, node->getY());
    CPPUNIT_ASSERT_EQUAL(1, node->getTags().size());
    HOOT_STR_EQUALS("n3", node->getTags().get("note"));

    ctr++;

    //2 nodes, 1 way

    map = std::make_shared<OsmMap>();
    CPPUNIT_ASSERT(reader.hasMoreElements());
    reader.readPartial(map);
    CPPUNIT_ASSERT_EQUAL(2, (int)map->getNodes().size());
    CPPUNIT_ASSERT_EQUAL(1, (int)map->getWays().size());
    CPPUNIT_ASSERT_EQUAL(0, (int)map->getRelations().size());

    node = map->getNode(4);
    CPPUNIT_ASSERT_EQUAL((long)4, node->getId());
    CPPUNIT_ASSERT_EQUAL(0.3, node->getX());
    CPPUNIT_ASSERT_EQUAL(0.0, node->getY());
    CPPUNIT_ASSERT_EQUAL(1, node->getTags().size());
    HOOT_STR_EQUALS("n4", node->getTags().get("note"));

    node = map->getNode(5);
    CPPUNIT_ASSERT_EQUAL((long)5, node->getId());
    CPPUNIT_ASSERT_EQUAL(0.4, node->getX());
    CPPUNIT_ASSERT_EQUAL(0.0, node->getY());
    CPPUNIT_ASSERT_EQUAL(0, node->getTags().size());

    WayPtr way = map->getWay(1);
    CPPUNIT_ASSERT_EQUAL((long)1, way->getId());
    CPPUNIT_ASSERT(way->hasNode(1));
    CPPUNIT_ASSERT(way->hasNode(2));
    CPPUNIT_ASSERT_EQUAL(1, way->getTags().size());
    HOOT_STR_EQUALS("w1b", way->getTags().get("noteb"));

    ctr++;

    //2 ways, 1 relation

    map = std::make_shared<OsmMap>();
    CPPUNIT_ASSERT(reader.hasMoreElements());
    reader.readPartial(map);
    CPPUNIT_ASSERT_EQUAL(0, (int)map->getNodes().size());
    CPPUNIT_ASSERT_EQUAL(2, (int)map->getWays().size());
    CPPUNIT_ASSERT_EQUAL(1, (int)map->getRelations().size());

    way = map->getWay(2);
    CPPUNIT_ASSERT_EQUAL((long)2, way->getId());
    CPPUNIT_ASSERT(way->hasNode(2));
    CPPUNIT_ASSERT(way->hasNode(3));
    CPPUNIT_ASSERT_EQUAL(1, way->getTags().size());
    HOOT_STR_EQUALS("w2", way->getTags().get("note"));

    way = map->getWay(3);
    CPPUNIT_ASSERT_EQUAL((long)3, way->getId());
    CPPUNIT_ASSERT(way->hasNode(2));
    CPPUNIT_ASSERT_EQUAL(0, way->getTags().size());

    RelationPtr relation = map->getRelation(1);
    CPPUNIT_ASSERT_EQUAL((long)1, relation->getId());
    CPPUNIT_ASSERT_EQUAL(size_t(2), relation->getMembers().size());
    CPPUNIT_ASSERT(relation->contains(ElementId::node(1)));
    CPPUNIT_ASSERT(relation->contains(ElementId::way(1)));
    RelationData::Entry member = relation->getMembers().at(0);
    HOOT_STR_EQUALS("n1", member.getRole());
    CPPUNIT_ASSERT_EQUAL((long)1, member.getElementId().getId());
    member = relation->getMembers().at(1);
    HOOT_STR_EQUALS("w1", member.getRole());
    CPPUNIT_ASSERT_EQUAL((long)1, member.getElementId().getId());
    CPPUNIT_ASSERT_EQUAL(1, relation->getTags().size());
    HOOT_STR_EQUALS("r1", relation->getTags().get("note"));

    ctr++;

    //1 relation

    map = std::make_shared<OsmMap>();
    CPPUNIT_ASSERT(reader.hasMoreElements());
    reader.readPartial(map);
    CPPUNIT_ASSERT_EQUAL(0, (int)map->getNodes().size());
    CPPUNIT_ASSERT_EQUAL(0, (int)map->getWays().size());
    CPPUNIT_ASSERT_EQUAL(1, (int)map->getRelations().size());

    relation = map->getRelation(2);
    CPPUNIT_ASSERT_EQUAL((long)2, relation->getId());
    HOOT_STR_EQUALS("", relation->getType());
    CPPUNIT_ASSERT(relation->contains(ElementId::node(2)));
    CPPUNIT_ASSERT_EQUAL(size_t(1), relation->getMembers().size());
    member = relation->getMembers().at(0);
    HOOT_STR_EQUALS("n2", member.getRole());
    CPPUNIT_ASSERT_EQUAL((long)2, member.getElementId().getId());
    CPPUNIT_ASSERT_EQUAL(0, relation->getTags().size());

    ctr++;

    CPPUNIT_ASSERT(!reader.hasMoreElements());
    reader.finalizePartial();

    CPPUNIT_ASSERT_EQUAL(4, ctr);
  }

  void readByBoundsLeaveConnectedOobWaysTest()
  {
    // This will leave any ways in the output which are outside of the bounds but are directly
    // connected to ways which cross the bounds.

    insertDataForBoundTest();

    OsmApiDb database;
    database.open(ServicesDbTestUtils::getOsmApiDbUrl());
    OsmApiDbReader reader;
    reader.setKeepImmediatelyConnectedWaysOutsideBounds(true);
    reader.open(ServicesDbTestUtils::getOsmApiDbUrl().toString());
    OsmMapPtr map = std::make_shared<OsmMap>();

    // The default behavior is return both features entirely within the bounds and those that
    // cross the bounds. This uses MapCropper internally, so other variations on the cropping are
    // tested in MapCropperTest.

    reader.setBoundingBox("-88.1,28.89,-88.0,28.91");
    reader.read(map);

    //quick check to see if the element counts are off...consult the test output for more detail

    CPPUNIT_ASSERT_EQUAL(6, (int)map->getNodes().size());
    CPPUNIT_ASSERT_EQUAL(4, (int)map->getWays().size());
    CPPUNIT_ASSERT_EQUAL(3, (int)map->getRelations().size());

    // Verify timestamps look OK
    WayPtr pWay = map->getWays().begin()->second;
    CPPUNIT_ASSERT(0 != pWay->getTimestamp());

    //Need to remove timestamps, otherwise they cause issues with the compare
    QList<ElementAttributeType> types;
    types.append(ElementAttributeType(ElementAttributeType::Timestamp));
    RemoveAttributesVisitor attrVis(types);
    map->visitRw(attrVis);

    MapProjector::projectToWgs84(map);
    OsmMapWriterFactory::write(map, _outputPath + "readByBoundsLeaveConnectedOobWaysTest.osm");
    HOOT_FILE_EQUALS(_inputPath + "readByBoundsLeaveConnectedOobWaysTest.osm",
                     _outputPath + "readByBoundsLeaveConnectedOobWaysTest.osm");

    //just want to make sure I can read against the same data twice in a row w/o crashing and also
    //make sure I don't get the same result again for a different bounds
    reader.setBoundingBox("-1,-1,1,1");
    map = std::make_shared<OsmMap>();
    reader.read(map);

    CPPUNIT_ASSERT_EQUAL(0, (int)map->getNodes().size());
    CPPUNIT_ASSERT_EQUAL(0, (int)map->getWays().size());
    CPPUNIT_ASSERT_EQUAL(0, (int)map->getRelations().size());

    reader.close();
  }
};

#ifdef HOOT_HAVE_SERVICES
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ServiceOsmApiDbReaderTest, "slow");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ServiceOsmApiDbReaderTest, "serial");
#endif  // HOOT_HAVE_SERVICES

}
