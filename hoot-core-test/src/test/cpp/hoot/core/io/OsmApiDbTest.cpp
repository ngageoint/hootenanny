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
 * @copyright Copyright (C) 2013, 2014, 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Hoot
#include <hoot/core/io/OsmApiDb.h>
#include <hoot/core/util/ConfigOptions.h>

#include "../TestUtils.h"
#include "ServicesDbTestUtils.h"

namespace hoot
{

class OsmApiDbTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(OsmApiDbTest);

  // osm apidb tests
  CPPUNIT_TEST(runOpenTest);
  CPPUNIT_TEST(runSelectElementsTest);

  CPPUNIT_TEST_SUITE_END();

public:

  static QString userEmail() { return "OsmApiDbTest@hoottestcpp.org"; }

  long mapId;

  void deleteUser(QString email)
  {
    OsmApiDb db;
    db.open(getOsmApiDbUrl());

    long userId = db.getUserId(email, false);
    if (userId != -1)
    {
      db.transaction();
      db.deleteUser(userId);
      db.commit();
    }
  }

  QUrl getOsmApiDbUrl()
  {
    return ServicesDbTestUtils::getOsmApiDbUrl();
  }

  /***********************************************************************************************
   * Purpose: Quick test to open the Osm ApiDb database
   * To see the output from this test, type the following:
   *   bin/HootTest --debug --single hoot::ServicesDbTest::runOpenOsmApiTest
   * *********************************************************************************************
   */
  void runOpenTest()
  {
    OsmApiDb db;

    db.open(getOsmApiDbUrl());
    CPPUNIT_ASSERT_EQUAL(true, db.getDB().isOpen());
    db.close();
    CPPUNIT_ASSERT_EQUAL(false, db.getDB().isOpen());
  }

  void runSelectElementsTest()
  {
    OsmApiDb database;
    database.open(getOsmApiDbUrl());

    // parse out the osm api dbname, dbuser, and dbpassword
    //example: postgresql://hoot:hoottest@localhost:5432/osmapi_test
    QUrl dbUrl = getOsmApiDbUrl();
    QString dbUrlString = dbUrl.toString();
    QStringList dbUrlParts = dbUrlString.split("/");
    QString dbName = dbUrlParts[dbUrlParts.size()-1];
    QStringList userParts = dbUrlParts[dbUrlParts.size()-2].split(":");
    QString dbUser = userParts[0];
    QString dbPassword = userParts[1].split("@")[0];
    QString dbHost = userParts[1].split("@")[1];
    QString dbPort = userParts[2];
    LOG_DEBUG("Name="+dbName+", user="+dbUser+", pass="+dbPassword+", port="+dbPort+", host="+dbHost);

    QString auth = "-h "+dbHost+" -p "+dbPort+" -U "+dbUser;

    /////////////////////////////////////
    // INSERT NODES INTO DB
    /////////////////////////////////////

    // list of insertions
    const long nodeId1 = 1;
    const long nodeId2 = 2;
    QList<long> ids;
    ids.append(nodeId1);
    ids.append(nodeId2);
    QList<QString> keys = QList<QString>() << "highway" << "accuracy1" << "foo";
    QList<QString> values = QList<QString>() << "road" << "5" << "bar";
    QList<float> lats = QList<float>() << 38.4 << 38;
    QList<float> lons = QList<float>() << -106.5 << -104;

    // Insert nodes
    QString cmd = "export PGPASSWORD="+dbPassword+"; export PGUSER="+dbUser+"; export PGDATABASE="+dbName+";\
      psql "+auth+" -f ${HOOT_HOME}/test-files/servicesdb/users.sql > /dev/null 2>&1; \
      psql "+auth+" -f ${HOOT_HOME}/test-files/servicesdb/changesets.sql > /dev/null 2>&1; \
      psql "+auth+" -f ${HOOT_HOME}/test-files/servicesdb/nodes.sql > /dev/null 2>&1";
    if( std::system(cmd.toStdString().c_str()) != 0 )
    {
      throw HootException("Failed postgres command.  Exiting test.");
    }

    /////////////////////////////////////
    // SELECT THE NODES USING SELECT_ALL
    // Need to get the data in the format exactly like the return of the Services Db now so we don't need to
    // change the front-end reader code
    //
    // Current format returned from apidb is in multiple rows with a new row for each tag.  Not great, but opened
    // a ticket for that to be optimized later when we can figure it out.
    // THe format:
    // row 1: id, latitude, longitude, changeset_id, visible, timestamp, tile, version, k{1}, v{1}
    // row 2: "     "         "              "          "         "        "      "     k{2}, v{2}
    // etc.
    //
    // Goal with these processing steps is to get this in the proper return format of the Services DB selectElements
    // The proper format:
    // id, latitude, longitude, changeset_id, visible, timestamp, tile, version, "k{1}"=>"v{1}", "k{2}"=>"v{2}", etc.
    //
    // Note: Again, ideally this gets done in the DB, faster there and less data to pass overhead.
    // Note2: I think this is important to do this processing here, so the front-end code that calls upon selectAll
    //   doesn't have to change, so it works for both ServicesDb and ApiDb without change.
    /////////////////////////////////////

    shared_ptr<QSqlQuery> nodeResultIterator = database.selectElements(ElementType::Node);

    // check if db active or not
    assert(nodeResultIterator->isActive());
    LOG_VARD(nodeResultIterator->size());

    const int numNodeFields = 10;
    long long lastId = LLONG_MIN;

    // read through the elements until the number inserted for this test is reached
    // - the number inserted is determined by ids.size()
    int elementCtr = ids.size()-1;
    LOG_VARD(elementCtr);
    CPPUNIT_ASSERT(elementCtr >= 0);
    int tagIndx = -1;
    int ctr = 0;
    while( nodeResultIterator->next() )
    {
      long long id = nodeResultIterator->value(0).toLongLong();
      LOG_VARD(id);
      if( lastId != id )
      {
        if(elementCtr < 0) break;

        // perform the comparison tests
        LOG_DEBUG(QString("Processing element ")+QString::number(elementCtr+1));
        // test the first line's data which is the current_nodes (main data): id, lat, lon, tag1
        HOOT_STR_EQUALS(nodeIds[elementCtr], nodeId);
        HOOT_STR_EQUALS(lats[elementCtr], nodeResultIterator->value(1).toLongLong() /
          (double)ApiDb::COORDINATE_SCALE);
        HOOT_STR_EQUALS(lons[elementCtr], nodeResultIterator->value(2).toLongLong() /
          (double)ApiDb::COORDINATE_SCALE);
        lastId = nodeId;
        elementCtr--;
        ctr++;
      }

      // verify the values written to the DB upon their read-back
      for (int j = 0; j < numNodeFields; j++)
      {
        LOG_DEBUG("VALUE = " + nodeResultIterator->value(j).toString());
      }

      // read the tag for as many rows as there are tags
      QString key = nodeResultIterator->value(8).toString();
      LOG_VARD(key);
      LOG_DEBUG(QString("Processing tag ") + key);
      tagIndx = ServicesDbTestUtils::findIndex(keys, key);
      HOOT_STR_EQUALS(QString(keys[tagIndx]+" = "+values[tagIndx]+"\n").toStdString().c_str(),
        ApiDb::unescapeTags(database.extractTagFromRow(nodeResultIterator, ElementType::Node)));
    }
    LOG_VARD(ctr);
    CPPUNIT_ASSERT_EQUAL(ids.size(), ctr);

    ///////////////////////////////////////////////
    /// Insert a way into the Osm Api DB
    ///////////////////////////////////////////////

    ids.clear();
    Tags t2;
    t2["highway"] = "primary";
    long insertedWayId = 1;
    wayIds.append(insertedWayId);

    cmd = "export PGPASSWORD="+dbPassword+"; export PGUSER="+dbUser+"; export PGDATABASE="+dbName+";\
      psql "+auth+" -f ${HOOT_HOME}/test-files/servicesdb/ways.sql > /dev/null 2>&1";
    if( std::system(cmd.toStdString().c_str()) != 0 )
    {
      throw HootException("Failed postgres command.  Exiting test.");
    }

    ///////////////////////////////////////////////
    /// Reads the ways from the Osm Api DB
    ///////////////////////////////////////////////

    shared_ptr<QSqlQuery> wayResultIterator = database.selectElements(ElementType::Way);

    // check again if db active or not
    assert(wayResultIterator->isActive());

    const int numWayFields = 7;
    lastId = LLONG_MIN;

    // read through the elements until the number inserted for this test is reached
    // - the number inserted is determined by ids.size()
    elementCtr = ids.size()-1;
    LOG_VARD(elementCtr);
    CPPUNIT_ASSERT(elementCtr >= 0);
    tagIndx = -1;
    ctr = 0;
    while( wayResultIterator->next() )
    {
      long long wayId = wayResultIterator->value(0).toLongLong();
      LOG_VARD(wayId);
      if( lastId != wayId )
      {
        if(elementCtr < 0) break;

        // perform the comparison tests
        LOG_DEBUG(QString("Processing element ")+QString::number(elementCtr+1));
        // test the first line's data which is the current_ways (main data): id
        LOG_DEBUG("ids = "+QString::number(wayIds[elementCtr]));
        LOG_DEBUG("wayId = "+QString::number(wayId));

        HOOT_STR_EQUALS(wayIds[elementCtr], wayId);

        // get the way nodes and do some minimal testing
        vector<long> v = database.selectNodeIdsForWay(wayId);
        HOOT_STR_EQUALS(2, v.size());
        CPPUNIT_ASSERT_EQUAL(nodeId2, v[1]);
        CPPUNIT_ASSERT_EQUAL(nodeId1, v[0]);

        // mark this way id processed
        lastId = wayId;
        elementCtr--;
        ctr++;
      }

      // verify the values written to the DB upon their read-back
      for (int j = 0; j < numWayFields; j++)
      {
        LOG_DEBUG("VALUE = " + wayResultIterator->value(j).toString());
      }

      // read the tag for as many rows as there are tags
      QString key = wayResultIterator->value(ApiDb::WAYS_TAGS).toString();
      LOG_DEBUG(QString("Processing tag ")+key);
      HOOT_STR_EQUALS("highway = primary\n", ApiDb::unescapeTags(
        database.extractTagFromRow(wayResultIterator, ElementType::Way)));
    }
    LOG_VARD(ctr);
    CPPUNIT_ASSERT_EQUAL(ids.size(), ctr);

    ///////////////////////////////////////////////
    /// Insert a relation into the Osm Api DB
    ///////////////////////////////////////////////

    QList<long> relIds;
    const long nodeId3 = nodeIds.at(0);
    const long wayId1 = wayIds.at(0);
    Tags rt;
    rt["type"] = "multistuff";
    long relationId = 1;
    relIds.append(relationId);

    cmd = "export PGPASSWORD="+dbPassword+"; export PGUSER="+dbUser+"; export PGDATABASE="+dbName+";\
      psql "+auth+" -f ${HOOT_HOME}/test-files/servicesdb/relations.sql > /dev/null 2>&1";
    if( std::system(cmd.toStdString().c_str()) != 0 )
    {
      throw HootException("Failed postgres command.  Exiting test.");
    }

    ///////////////////////////////////////////////
    /// Reads the relations from the Osm Api DB
    ///////////////////////////////////////////////

    shared_ptr<QSqlQuery> relationResultIterator = database.selectElements(ElementType::Relation);

    // check again if db active or not
    assert(relationResultIterator->isActive());

    const int numRelationFields = 7;
    lastId = LLONG_MIN;

    // read through the elements until the number inserted for this test is reached
    // - the number inserted is determined by ids.size()
    elementCtr = ids.size()-1;
    LOG_VARD(elementCtr);
    CPPUNIT_ASSERT(elementCtr >= 0);
    tagIndx = -1;
    ctr = 0;
    while ( relationResultIterator->next() )
    {
      long long relId = relationResultIterator->value(0).toLongLong();
      LOG_VARD(relId);
      if( lastId != relId )
      {
        if(elementCtr < 0) break;

        // perform the comparison tests
        LOG_DEBUG(QString("Processing element ") + QString::number(elementCtr+1));
        // test the first line's data which is the current_nodes (main data): id
        LOG_DEBUG("ids = "+ QString::number(relIds[elementCtr]));
        LOG_DEBUG("relId = "+ QString::number(relId));
        HOOT_STR_EQUALS(relIds[elementCtr], relId);

        // get the relation nodes and do some minimal testing
        vector<RelationData::Entry> members = database.selectMembersForRelation(relId);
        HOOT_STR_EQUALS(2, members.size());
        QString expected = "[2]{Entry: role: wayrole, eid: Way:"+QString::number(wayId1)+
                ", Entry: role: noderole, eid: Node:"+QString::number(nodeId3)+"}";
        HOOT_STR_EQUALS(expected, members);

        // mark this way id processed
        lastId = relId;
        elementCtr--;
        ctr++;
      }

      // verify the values written to the DB upon their read-back
      for(int j = 0; j < numRelationFields; j++)
      {
        LOG_DEBUG("VALUE = " + relationResultIterator->value(j).toString());
      }

      // read the tag for as many rows as there are tags
      QString key = relationResultIterator->value(ApiDb::WAYS_TAGS).toString();
      LOG_DEBUG(QString("Processing tag ")+key);
      HOOT_STR_EQUALS("type = multistuff\n", ApiDb::unescapeTags(
        database.extractTagFromRow(relationResultIterator, ElementType::Relation)));
    }
    LOG_VARD(ctr);
    CPPUNIT_ASSERT_EQUAL(ids.size(), ctr);
  }

  void setUp()
  {
    deleteUser(userEmail());
  }

  void tearDown()
  {
    deleteUser(userEmail());

    // tear down the ServicesDB
    OsmApiDb database;

    // tear down the osm api db
    database.open(getOsmApiDbUrl());
    //database.deleteData();
    database.close();
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(OsmApiDbTest, "slow");

}
