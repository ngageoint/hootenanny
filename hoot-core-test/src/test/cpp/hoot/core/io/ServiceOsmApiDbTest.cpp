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

// Qt
#include <QFile>
#include <QTextStream>
#include <QDir>

#include "../TestUtils.h"
#include "ServicesDbTestUtils.h"

namespace hoot
{

class ServiceOsmApiDbTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(ServiceOsmApiDbTest);
  CPPUNIT_TEST(runOpenTest);
  CPPUNIT_TEST(runSelectElementsTest);
  CPPUNIT_TEST(runToOsmApiCoordConvertTest);
  CPPUNIT_TEST_SUITE_END();

public:

  static QString userEmail() { return "ServiceOsmApiDbTest@hoottestcpp.org"; }

  long mapId;

  void setUp()
  {
    deleteUser(userEmail());
  }

  void tearDown()
  {
    deleteUser(userEmail());

    OsmApiDb database;
    database.open(ServicesDbTestUtils::getOsmApiDbUrl());
    database.deleteData();
    database.close();
  }

  void deleteUser(QString email)
  {
    OsmApiDb db;
    db.open(ServicesDbTestUtils::getOsmApiDbUrl());

    long userId = db.getUserId(email, false);
    if (userId != -1)
    {
      db.transaction();
      db.deleteUser(userId);
      db.commit();
    }
  }

  void runOpenTest()
  {
    OsmApiDb db;

    db.open(ServicesDbTestUtils::getOsmApiDbUrl());
    CPPUNIT_ASSERT_EQUAL(true, db.getDB().isOpen());
    db.close();
    CPPUNIT_ASSERT_EQUAL(false, db.getDB().isOpen());
  }

  void runSelectElementsTest()
  {
    OsmApiDb database;
    database.open(ServicesDbTestUtils::getOsmApiDbUrl());
    database.deleteData();
    ServicesDbTestUtils::execOsmApiDbSqlTestScript("users.sql");
    ServicesDbTestUtils::execOsmApiDbSqlTestScript("changesets.sql");

    ServicesDbTestUtils::execOsmApiDbSqlTestScript("nodes.sql");

    /////////////////////////////////////
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

    // list of insertions

    QList<long> ids;
    const long nodeId1 = 1;
    const long nodeId2 = 2;
    ids.append(nodeId1);
    ids.append(nodeId2);
    QList<QString> keys = QList<QString>() << "highway" << "accuracy1" << "foo";
    QList<QString> values = QList<QString>() << "road" << "5" << "bar";
    QList<float> lats = QList<float>() << 38.4 << 38;
    QList<float> lons = QList<float>() << -106.5 << -104;

    boost::shared_ptr<QSqlQuery> nodeResultIterator = database.selectElements(ElementType::Node);

    // check if db active or not
    assert(nodeResultIterator->isActive());

    const int numNodeFields = 10;
    long long lastId = LLONG_MIN;

    // read through the elements until the number inserted for this test is reached
    // - the number inserted is determined by ids.size()
    int elementCtr = ids.size()-1;
    LOG_VART(elementCtr);
    CPPUNIT_ASSERT(elementCtr >= 0);
    int tagIndx = -1;
    int ctr = 0;
    while( nodeResultIterator->next() )
    {
      long long id = nodeResultIterator->value(0).toLongLong();
      LOG_VART(id);
      if( lastId != id )
      {
        if (elementCtr < 0) break;

        // perform the comparison tests
        LOG_TRACE(QString("Processing element ")+QString::number(elementCtr+1));
        // test the first line's data which is the current_nodes (main data): id, lat, lon, tag1
        HOOT_STR_EQUALS(ids[elementCtr], id);
        HOOT_STR_EQUALS(lats[elementCtr], nodeResultIterator->value(1).toLongLong() /
          (double)ApiDb::COORDINATE_SCALE);
        HOOT_STR_EQUALS(lons[elementCtr], nodeResultIterator->value(2).toLongLong() /
          (double)ApiDb::COORDINATE_SCALE);
        lastId = id;
        elementCtr--;
        ctr++;
      }

      // verify the values written to the DB upon their read-back
      for (int j = 0; j < numNodeFields; j++)
      {
        LOG_TRACE("VALUE = " + nodeResultIterator->value(j).toString());
      }

      // read the tag for as many rows as there are tags
      QString key = nodeResultIterator->value(8).toString();
      LOG_VART(key);
      LOG_TRACE(QString("Processing tag ") + key);
      tagIndx = ServicesDbTestUtils::findIndex(keys, key);
      HOOT_STR_EQUALS(QString(keys[tagIndx]+" = "+values[tagIndx]+"\n").toStdString().c_str(),
        ApiDb::unescapeTags(database.extractTagFromRow(nodeResultIterator, ElementType::Node)));
    }
    LOG_VART(ctr);
    CPPUNIT_ASSERT_EQUAL(ids.size(), ctr);

    ids.clear();
    Tags t2;
    t2["highway"] = "primary";
    long insertedWayId = 1;
    ids.append(insertedWayId);
    vector<long> nodeIds;
    nodeIds.push_back(nodeId1);
    nodeIds.push_back(nodeId2);

    ServicesDbTestUtils::execOsmApiDbSqlTestScript("ways.sql");

    boost::shared_ptr<QSqlQuery> wayResultIterator = database.selectElements(ElementType::Way);

    // check again if db active or not
    assert(wayResultIterator->isActive());

    const int numWayFields = 7;
    lastId = LLONG_MIN;

    // read through the elements until the number inserted for this test is reached
    // - the number inserted is determined by ids.size()
    elementCtr = ids.size()-1;
    LOG_VART(elementCtr);
    CPPUNIT_ASSERT(elementCtr >= 0);
    tagIndx = -1;
    ctr = 0;
    while( wayResultIterator->next() )
    {
      long long wayId = wayResultIterator->value(0).toLongLong();
      if( lastId != wayId )
      {
        if(elementCtr < 0) break;

        // perform the comparison tests
        LOG_TRACE(QString("Processing element ")+QString::number(elementCtr+1));
        // test the first line's data which is the current_ways (main data): id
        LOG_TRACE("ids = "+QString::number(ids[elementCtr]));
        LOG_TRACE("wayId = "+QString::number(wayId));

        HOOT_STR_EQUALS(ids[elementCtr], wayId);

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
        LOG_TRACE("VALUE = " + wayResultIterator->value(j).toString());
      }

      // read the tag for as many rows as there are tags
      QString key = wayResultIterator->value(ApiDb::WAYS_TAGS).toString();
      LOG_TRACE(QString("Processing tag ")+key);
      HOOT_STR_EQUALS("highway = primary\n", ApiDb::unescapeTags(
        database.extractTagFromRow(wayResultIterator, ElementType::Way)));
    }
    LOG_VART(ctr);
    CPPUNIT_ASSERT_EQUAL(ids.size(), ctr);

    const long nodeId3 = nodeIds.at(0);
    const long wayId1 = ids.at(0);
    ids.clear();
    Tags rt;
    rt["type"] = "multistuff";
    long relationId = 1;
    ids.append(relationId);

    ServicesDbTestUtils::execOsmApiDbSqlTestScript("relations.sql");

    boost::shared_ptr<QSqlQuery> relationResultIterator = database.selectElements(ElementType::Relation);

    // check again if db active or not
    assert(relationResultIterator->isActive());

    const int numRelationFields = 7;
    lastId = LLONG_MIN;

    // read through the elements until the number inserted for this test is reached
    // - the number inserted is determined by ids.size()
    elementCtr = ids.size()-1;
    LOG_VART(elementCtr);
    CPPUNIT_ASSERT(elementCtr >= 0);
    tagIndx = -1;
    ctr = 0;
    while ( relationResultIterator->next() )
    {
      long long relId = relationResultIterator->value(0).toLongLong();
      if( lastId != relId )
      {
        if(elementCtr < 0) break;

        // perform the comparison tests
        LOG_TRACE(QString("Processing element ") + QString::number(elementCtr+1));
        // test the first line's data which is the current_nodes (main data): id
        LOG_TRACE("ids = "+ QString::number(ids[elementCtr]));
        LOG_TRACE("relId = "+ QString::number(relId));
        HOOT_STR_EQUALS(ids[elementCtr], relId);

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
        LOG_TRACE("VALUE = " + relationResultIterator->value(j).toString());
      }

      // read the tag for as many rows as there are tags
      QString key = relationResultIterator->value(ApiDb::WAYS_TAGS).toString();
      LOG_TRACE(QString("Processing tag ")+key);
      HOOT_STR_EQUALS("type = multistuff\n", ApiDb::unescapeTags(
        database.extractTagFromRow(relationResultIterator, ElementType::Relation)));
    }
    LOG_VART(ctr);
    CPPUNIT_ASSERT_EQUAL(ids.size(), ctr);
  }

  void runToOsmApiCoordConvertTest()
  {
    //convert precision from 0.01 nanodegrees to 100 nanodegrees

    double hootApiCoordValue = 38.85433192782;
    long osmApiDbCoordValue = OsmApiDb::toOsmApiDbCoord(hootApiCoordValue);
    CPPUNIT_ASSERT_EQUAL(osmApiDbCoordValue, (long)388543319);

    hootApiCoordValue = 38.85433199782;
    osmApiDbCoordValue = OsmApiDb::toOsmApiDbCoord(hootApiCoordValue);
    CPPUNIT_ASSERT_EQUAL(osmApiDbCoordValue, (long)388543320);
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ServiceOsmApiDbTest, "slow");

}
