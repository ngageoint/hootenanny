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
#include <hoot/core/io/ApiDb.h>
#include <hoot/core/io/OsmApiDb.h>
#include <hoot/core/io/OsmApiDbReader.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/OsmMap.h>

#include "../TestUtils.h"
#include "ServicesDbTestUtils.h"


namespace hoot
{

class ServiceOsmApiDbReaderTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(ServiceOsmApiDbReaderTest);
  CPPUNIT_TEST(runReadOsmApiTest);
  CPPUNIT_TEST(runReadBoundingBoxTest);
  CPPUNIT_TEST_SUITE_END();

public:

  static QString userEmail() { return "ServiceOsmApiDbReaderTest@hoottestcpp.org"; }

  void tearDown()
  {
    // Osm Api DB
    OsmApiDb database;
    database.open(ServicesDbTestUtils::getOsmApiDbUrl().toString());
    database.deleteData();
    database.close();
  }

  void insertData()
  {
    // parse out the osm api dbname, dbuser, and dbpassword
    //example: postgresql://hoot:hoottest@localhost:5432/osmapi_test
    QUrl dbUrl = ServicesDbTestUtils::getOsmApiDbUrl();
    QString dbUrlString = dbUrl.toString();
    QStringList dbUrlParts = dbUrlString.split("/");
    QString dbName = dbUrlParts[dbUrlParts.size()-1];
    QStringList userParts = dbUrlParts[dbUrlParts.size()-2].split(":");
    QString dbUser = userParts[0];
    QString dbPassword = userParts[1].split("@")[0];
    QString dbHost = userParts[1].split("@")[1];
    QString dbPort = userParts[2];

    LOG_DEBUG("Name="+dbName+", user="+dbUser+", pass="+dbPassword+", port="+dbPort+", host="+dbHost);

    ////////////////////////////////////////
    // insert simple test data
    ////////////////////////////////////////
    QString auth = "-h "+dbHost+" -p "+dbPort+" -U "+dbUser;
    QString cmd = "export PGPASSWORD="+dbPassword+"; export PGUSER="+dbUser+"; export PGDATABASE="+dbName+";\
      psql "+auth+" -f ${HOOT_HOME}/test-files/servicesdb/users.sql > /dev/null 2>&1; \
      psql "+auth+" -f ${HOOT_HOME}/test-files/servicesdb/changesets.sql > /dev/null 2>&1; \
      psql "+auth+" -f ${HOOT_HOME}/test-files/servicesdb/nodes.sql > /dev/null 2>&1; \
      psql "+auth+" -f ${HOOT_HOME}/test-files/servicesdb/ways.sql > /dev/null 2>&1; \
      psql "+auth+" -f ${HOOT_HOME}/test-files/servicesdb/relations.sql > /dev/null 2>&1";

    if( std::system(cmd.toStdString().c_str()) != 0 )
    {
      throw HootException("Failed postgres command.  Exiting test.");
    }
  }

  void verifyFullReadOutput(shared_ptr<OsmMap> map)
  {
    //nodes
    CPPUNIT_ASSERT_EQUAL(2, (int)map->getNodeMap().size());
    HOOT_STR_EQUALS(true, map->containsNode(1));
    shared_ptr<Node> node = map->getNode(1);
    CPPUNIT_ASSERT_EQUAL((long)1, node->getId());
    CPPUNIT_ASSERT_EQUAL(38.4, node->getY());
    CPPUNIT_ASSERT_EQUAL(-106.5, node->getX());
    CPPUNIT_ASSERT_EQUAL(15.0, node->getCircularError());
    CPPUNIT_ASSERT_EQUAL(2, node->getTags().size());

    shared_ptr<Node> node1 = map->getNode(2);
    CPPUNIT_ASSERT_EQUAL((long)2, node1->getId());
    CPPUNIT_ASSERT_EQUAL(38.0, node1->getY());
    CPPUNIT_ASSERT_EQUAL(-104.0, node1->getX());

    //ways
    HOOT_STR_EQUALS(true, map->containsWay(1));
    shared_ptr<Way> way = map->getWay(1);
    CPPUNIT_ASSERT_EQUAL((long)1, way->getId());
    CPPUNIT_ASSERT_EQUAL(2, (int)way->getNodeCount());
    CPPUNIT_ASSERT_EQUAL((long)1, way->getNodeId(0));
    CPPUNIT_ASSERT_EQUAL((long)2, way->getNodeId(1));
    CPPUNIT_ASSERT_EQUAL(15.0, way->getCircularError());
    CPPUNIT_ASSERT_EQUAL(1, way->getTags().size());

    //relations
    HOOT_STR_EQUALS(true, map->containsRelation(1));
    shared_ptr<Relation> relation = map->getRelation(1);
    CPPUNIT_ASSERT_EQUAL((long)1, relation->getId());
    vector<RelationData::Entry> relationData = relation->getMembers();
    CPPUNIT_ASSERT_EQUAL(2, (int)relation->getMembers().size());
    HOOT_STR_EQUALS("wayrole", relationData[0].getRole());
    HOOT_STR_EQUALS("noderole",relationData[1].getRole());
    CPPUNIT_ASSERT_EQUAL(15.0, relation->getCircularError());
  }

  void verifyReadBoundingBoxOutput(shared_ptr<OsmMap> map)
  {
    CPPUNIT_ASSERT_EQUAL(2, (int)map->getNodeMap().size());
    CPPUNIT_ASSERT_EQUAL(1, (int)map->getWays().size());
    CPPUNIT_ASSERT_EQUAL(1, (int)map->getRelationMap().size());

    //get the seconde node id. It changes during HootTest depends on how many tests
    NodeMap nm = map->getNodeMap();
    long secondNodeId = 0;
    int count = 1;
    for (NodeMap::iterator nodeIter = nm.begin(); nodeIter != nm.end(); ++nodeIter )
    {
      shared_ptr<Node> n = nodeIter->second;
      if (count == 2) //second node
      {
        secondNodeId = n->getId();
      }
      count++;
    }

    HOOT_STR_EQUALS(true, map->containsNode(1));
    shared_ptr<Node> node = map->getNode(1);
    CPPUNIT_ASSERT_EQUAL((long)1, node->getId());
    CPPUNIT_ASSERT_EQUAL(38.4, node->getY());
    CPPUNIT_ASSERT_EQUAL(-106.5, node->getX());
    CPPUNIT_ASSERT_EQUAL(15.0, node->getCircularError());
    CPPUNIT_ASSERT_EQUAL(2, node->getTags().size());

    //The original second node is outside the bounding box (38.0, -104.0)
    //so the new second node generated after cropping
    HOOT_STR_EQUALS(true, map->containsNode(secondNodeId));
    shared_ptr<Node> node1 = map->getNode(secondNodeId);
    CPPUNIT_ASSERT_EQUAL((long)secondNodeId, node1->getId());
    CPPUNIT_ASSERT_EQUAL(38.22048, node1->getY());
    CPPUNIT_ASSERT_EQUAL(-105.378, node1->getX());

    //ways
    WayMap wm = map->getWays();
    long wayId = 0;
    for (WayMap::iterator wayIter = wm.begin(); wayIter != wm.end(); ++wayIter )
    {
      shared_ptr<Way> w = wayIter->second;
      //we know there is only one way
      wayId = w->getId();
    }

    HOOT_STR_EQUALS(true, map->containsWay(wayId));
    shared_ptr<Way> way = map->getWay(wayId);
    CPPUNIT_ASSERT_EQUAL(wayId, way->getId());
    CPPUNIT_ASSERT_EQUAL(2, (int)way->getNodeCount());
    CPPUNIT_ASSERT_EQUAL((long)1, way->getNodeId(0));
    CPPUNIT_ASSERT_EQUAL((long)secondNodeId, way->getNodeId(1));
    CPPUNIT_ASSERT_EQUAL(15.0, way->getCircularError());
    CPPUNIT_ASSERT_EQUAL(1, way->getTags().size());

    //relations
    HOOT_STR_EQUALS(true, map->containsRelation(1));
    shared_ptr<Relation> relation = map->getRelation(1);
    CPPUNIT_ASSERT_EQUAL((long)1, relation->getId());
    vector<RelationData::Entry> relationData = relation->getMembers();
    CPPUNIT_ASSERT_EQUAL(2, (int)relation->getMembers().size());
    HOOT_STR_EQUALS("wayrole", relationData[0].getRole());
    HOOT_STR_EQUALS("noderole",relationData[1].getRole());
    CPPUNIT_ASSERT_EQUAL(15.0, relation->getCircularError());
  }

  void runReadOsmApiTest()
  {
    //test reader
    OsmApiDbReader reader;
    shared_ptr<OsmMap> map(new OsmMap());

    insertData();

    OsmApiDb database;
    database.open(ServicesDbTestUtils::getOsmApiDbUrl());

    Settings s = conf();
    reader.open(ServicesDbTestUtils::getOsmApiDbUrl().toString());
    reader.read(map);

    verifyFullReadOutput(map);
    reader.close();
  }

  void runReadBoundingBoxTest()
  {
    OsmApiDbReader reader;
    shared_ptr<OsmMap> map(new OsmMap());

    insertData();

    OsmApiDb database;
    database.open(ServicesDbTestUtils::getOsmApiDbUrl());

    Settings s = conf();
    reader.open(ServicesDbTestUtils::getOsmApiDbUrl().toString());

    QString bbox = "-106.51848,38.0445,-105.378,38.56";
    reader.setBoundingBox(bbox);

    reader.read(map);

    verifyReadBoundingBoxOutput(map);

    reader.close();
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ServiceOsmApiDbReaderTest, "slow");

}
