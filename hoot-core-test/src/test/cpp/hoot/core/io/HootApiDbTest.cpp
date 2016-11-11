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
#include <hoot/core/io/HootApiDb.h>
#include <hoot/core/util/ConfigOptions.h>

#include "../TestUtils.h"
#include "ServicesDbTestUtils.h"

namespace hoot
{

class HootApiDbTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(HootApiDbTest);

  // standard hoot services tests
  CPPUNIT_TEST(runDbVersionTest);
  CPPUNIT_TEST(runOpenTest);
  CPPUNIT_TEST(runDropMapTest);
  CPPUNIT_TEST(runInsertTest);
  CPPUNIT_TEST(runMapExistsTest);
  CPPUNIT_TEST(runChangesetExistsTest);
  CPPUNIT_TEST(runNumElementsTest);
  CPPUNIT_TEST(runSelectAllElementsTest);
  CPPUNIT_TEST(runSelectElementsTest);
  CPPUNIT_TEST(runSelectElementsCustomTagsTest);
  CPPUNIT_TEST(runSelectNodeIdsForWayTest);
  CPPUNIT_TEST(runSelectMembersForRelationTest);
  CPPUNIT_TEST(runUpdateNodeTest);
  CPPUNIT_TEST(runUnescapeTags);
  CPPUNIT_TEST_SUITE_END();

public:

  static QString userEmail() { return "HootApiDbTest@hoottestcpp.org"; }

  long mapId;

  void deleteUser(QString email)
  {
    HootApiDb db;
    db.open(getDbUrl());

    long userId = db.getUserId(email, false);

    if (userId != -1)
    {
      db.transaction();
      db.deleteUser(userId);
      db.commit();
    }
  }

  QUrl getDbUrl()
  {
    return ServicesDbTestUtils::getDbModifyUrl();
  }

  void runOpenTest()
  {
    HootApiDb db;
    db.open(getDbUrl());
    CPPUNIT_ASSERT_EQUAL(true, db.getDB().isOpen());
    db.close();
    CPPUNIT_ASSERT_EQUAL(false, db.getDB().isOpen());
  }

  /***********************************************************************************************
   * Purpose: Print the current Services DB version
   * To see the version from this test, type the following:
   *   bin/HootTest --debug --single hoot::ServicesDbTest::runDbVersionTest
   * *********************************************************************************************
   */
  void runDbVersionTest()
  {
    HootApiDb db;
    db.open(getDbUrl());
    QString version = db.getDbVersion();
    LOG_DEBUG("The version = " << version << ".");
    CPPUNIT_ASSERT_EQUAL(ApiDb::expectedDbVersion().toStdString(), version.toStdString());
  }

  void runDropMapTest()
  {
    HootApiDb database;
    database.open(getDbUrl());

    const shared_ptr<QList<long> > ids = insertTestMap1(database);

    mapId = ids->at(0);

    //LOG_WARN("Map ID out of ITM1: " << mapId);

    CPPUNIT_ASSERT(database.mapExists(mapId));

    HOOT_STR_EQUALS(true, database.hasTable("current_nodes" + database.getMapIdString(mapId)));
    HOOT_STR_EQUALS(true, database.hasTable("current_relation_members" +
      database.getMapIdString(mapId)));
    HOOT_STR_EQUALS(true, database.hasTable("current_relations" +
      database.getMapIdString(mapId)));
    HOOT_STR_EQUALS(true, database.hasTable("current_way_nodes" +
      database.getMapIdString(mapId)));
    HOOT_STR_EQUALS(true, database.hasTable("current_ways" + database.getMapIdString(mapId)));

    database.deleteMap(mapId);

    HOOT_STR_EQUALS(false, database.hasTable("current_nodes" + database.getMapIdString(mapId)));
    HOOT_STR_EQUALS(false, database.hasTable("current_relation_members" +
      database.getMapIdString(mapId)));
    HOOT_STR_EQUALS(false, database.hasTable("current_relations" +
      database.getMapIdString(mapId)));
    HOOT_STR_EQUALS(false, database.hasTable("current_way_nodes" +
      database.getMapIdString(mapId)));
    HOOT_STR_EQUALS(false, database.hasTable("current_ways" + database.getMapIdString(mapId)));
  }

  const shared_ptr<QList<long> > insertTestMap1(HootApiDb& database)
  {
    database.transaction();

    QList<long> ids;
    database.setUserId(database.getOrCreateUser(userEmail(), "HootApiDbTest"));

    long mapId = database.insertMap("foo", true);
    ids.append(mapId);

    database.setMapId(mapId);

    database.beginChangeset(Tags());
    ids.append(database.getChangesetId());

    Tags t;
    t["foo"] = "bar";
    long nodeId;
    database.insertNode(38, -104, t, nodeId);
    ids.append(nodeId);

    Tags t2;
    t2["highway"] = "primary";
    long wayId;
    database.insertWay(t2, wayId);
    ids.append(wayId);
    vector<long> nodeIds;
    nodeIds.push_back(nodeId);
    database.insertWayNodes(wayId, nodeIds);

    Tags rt;
    rt["type"] = "multistuff";
    long relationId;
    database.insertRelation(rt, relationId);
    ids.append(relationId);
    database.insertRelationMember(relationId, ElementType::Way, wayId, "wayrole", 0);
    database.insertRelationMember(relationId, ElementType::Node, nodeId, "noderole", 1);

    database.commit();

    return shared_ptr<QList<long> >(new QList<long>(ids));
  }

  const shared_ptr<QList<long> > insertTestMap2(HootApiDb& database)
  {
    database.transaction();

    QList<long> ids;
    database.setUserId(database.getOrCreateUser(userEmail(), "HootApiDbTest"));
    long mapId = database.insertMap("foo", true);
    ids.append(mapId);
    database.setMapId(mapId);
    database.beginChangeset();

    Tags t;
    long nodeId;
    database.insertNode(38.0, -104, t, nodeId);
    ids.append(nodeId);
    t["foo"] = "bar";
    database.insertNode(37.9, -105, t, nodeId);
    ids.append(nodeId);
    t.clear();
    t["foo2"] = "bar2";
    database.insertNode(38.1, -106, t, nodeId);
    ids.append(nodeId);

    Tags wt;
    wt["highway"] = "primary";
    long wayId;
    database.insertWay(wt, wayId);
    ids.append(wayId);
    vector<long> nodeIds;
    nodeIds.push_back(nodeId);
    database.insertWayNodes(wayId, nodeIds);
    wt.clear();
    wt["highway2"] = "primary2";
    database.insertWay(wt, wayId);
    ids.append(wayId);
    database.insertWayNodes(wayId, nodeIds);

    t.clear();
    t["type"] = "multistuff";
    long relationId;
    database.insertRelation(t, relationId);
    ids.append(relationId);
    database.insertRelationMember(relationId, ElementType::Way, wayId, "wayrole", 0);
    database.insertRelationMember(relationId, ElementType::Node, nodeId, "noderole", 1);
    t.clear();
    t["type2"] = "multistuff2";
    database.insertRelation(t, relationId);
    ids.append(relationId);
    database.insertRelationMember(relationId, ElementType::Way, wayId, "wayrole", 0);
    database.insertRelationMember(relationId, ElementType::Node, nodeId, "noderole", 1);

    database.commit();

    return shared_ptr<QList<long> >(new QList<long>(ids));
  }

  const shared_ptr<QList<long> > insertTestMapWithCustomTags(HootApiDb& database)
  {
    QList<long> ids;
    database.transaction();
    database.setUserId(database.getOrCreateUser(userEmail(), "HootApiDbTest"));
    long mapId = database.insertMap("foo", true);
    ids.append(mapId);
    database.setMapId(mapId);
    database.beginChangeset();

    Tags t;
    t["hoot:status"] = "Unknown1";
    t["accuracy"] = "20.0";
    long nodeId;
    database.insertNode(38, -104, t, nodeId);
    ids.append(nodeId);

    t.clear();
    t["error:circular"] = "20.0";
    database.insertNode(38, -105, t, nodeId);
    ids.append(nodeId);

    t.clear();
    database.insertNode(38, -106, t, nodeId);
    ids.append(nodeId);

    database.commit();

    return shared_ptr<QList<long> >(new QList<long>(ids));
  }

  void runInsertTest()
  {
    HootApiDb db;
    db.open(getDbUrl());

    insertTestMap1(db);
  }

  void runMapExistsTest()
  {
    HootApiDb database;
    database.open(getDbUrl());
    const shared_ptr<QList<long> > ids = insertTestMap1(database);

    mapId = ids->at(0);
    CPPUNIT_ASSERT(database.mapExists(mapId));
  }

  void runChangesetExistsTest()
  {
    HootApiDb database;
    database.open(getDbUrl());
    const shared_ptr<QList<long> > ids = insertTestMap1(database);

    const long changesetId = ids->at(1);
    CPPUNIT_ASSERT(database.changesetExists(changesetId));
  }

  void runNumElementsTest()
  {
    HootApiDb database;
    database.open(getDbUrl());

    const shared_ptr<QList<long> > ids = insertTestMap1(database);

    mapId = ids->at(0);
    CPPUNIT_ASSERT_EQUAL((long)1, database.numElements(ElementType::Node));
  }

  void runSelectNodeIdsForWayTest()
  {
    HootApiDb database;
    database.open(getDbUrl());

    const shared_ptr<QList<long> > ids = insertTestMap1(database);
    CPPUNIT_ASSERT_EQUAL(5, ids->size());
    mapId = ids->at(0);
    const long wayId = ids->at(3);

    HOOT_STR_EQUALS("[1]{1}", database.selectNodeIdsForWay(wayId));
  }

  void runSelectMembersForRelationTest()
  {
    HootApiDb database;
    database.open(getDbUrl());

    const shared_ptr<QList<long> > ids = insertTestMap1(database);
    CPPUNIT_ASSERT_EQUAL(5, ids->size());
    mapId = ids->at(0);
    const long relationId = ids->at(4);

    vector<RelationData::Entry> members = database.selectMembersForRelation(relationId);
    HOOT_STR_EQUALS("[2]{Entry: role: wayrole, eid: Way:1, Entry: role: noderole, eid: Node:1}",
                    members);
  }

  void runSelectAllElementsTest()
  {
    HootApiDb database;
    database.open(getDbUrl());

    shared_ptr<OsmMap> map(new OsmMap());

    const shared_ptr<QList<long> > ids = insertTestMap1(database);
    CPPUNIT_ASSERT_EQUAL(5, ids->size());
    mapId = ids->at(0);
    const long nodeId = ids->at(2);
    const long wayId = ids->at(3);
    const long relationId = ids->at(4);

    shared_ptr<QSqlQuery> nodeResultIterator = database.selectElements(ElementType::Node);
    int ctr = 0;
    while (nodeResultIterator->next())
    {
      for(int j=0;j<10;j++) { LOG_DEBUG("VALUE = "+nodeResultIterator->value(j).toString()); }

      HOOT_STR_EQUALS(nodeId, nodeResultIterator->value(0).toLongLong());
      HOOT_STR_EQUALS(38.0, nodeResultIterator->value(1).toDouble());
      HOOT_STR_EQUALS(-104.0, nodeResultIterator->value(2).toDouble());
      stringstream s;
      s << ApiDb::unescapeTags(nodeResultIterator->value(8));
      LOG_DEBUG("unescapeTag :"+s.str());
      HOOT_STR_EQUALS("foo = bar\n", ApiDb::unescapeTags(nodeResultIterator->value(8)));

      ctr++;
    }
    CPPUNIT_ASSERT_EQUAL(1, ctr);

    shared_ptr<QSqlQuery> wayResultIterator = database.selectElements(ElementType::Way);
    ctr = 0;
    while (wayResultIterator->next())
    {
      HOOT_STR_EQUALS(wayId, wayResultIterator->value(0).toLongLong());
      vector<long> v = database.selectNodeIdsForWay(wayId);
      HOOT_STR_EQUALS(1, v.size());
      CPPUNIT_ASSERT_EQUAL(nodeId, v[0]);

      HOOT_STR_EQUALS("highway = primary\n", ApiDb::unescapeTags(wayResultIterator->value(5)));
      ctr++;
    }
    CPPUNIT_ASSERT_EQUAL(1, ctr);

    shared_ptr<QSqlQuery> relationResultIterator =
      database.selectElements(ElementType::Relation);
    ctr = 0;
    while (relationResultIterator->next())
    {
      HOOT_STR_EQUALS(relationId, relationResultIterator->value(0).toLongLong());
      vector<RelationData::Entry> members = database.selectMembersForRelation(relationId);
      HOOT_STR_EQUALS("[2]{Entry: role: wayrole, eid: Way:1, Entry: role: noderole, eid: Node:1}",
                      members);
      HOOT_STR_EQUALS("type = multistuff\n",
                      ApiDb::unescapeTags(relationResultIterator->value(5)));
      ctr++;
    }

    CPPUNIT_ASSERT_EQUAL(1, ctr);
  }

  void runSelectElementsTest()
  {
    HootApiDb database;
    database.open(getDbUrl());

    shared_ptr<OsmMap> map(new OsmMap());

    const shared_ptr<QList<long> > ids = insertTestMap2(database);
    CPPUNIT_ASSERT_EQUAL(8, ids->size());
    mapId = ids->at(0);
    const long nodeId1 = ids->at(2);
    const long nodeId2 = ids->at(3);

    shared_ptr<QSqlQuery> nodeResultIterator =
      database.selectElements(ElementType::Node);
    int ctr = 0;
    while (nodeResultIterator->next())
    {
     if (ctr == 1)
     {
        HOOT_STR_EQUALS(nodeId1, nodeResultIterator->value(0).toLongLong());
        HOOT_STR_EQUALS(37.9, nodeResultIterator->value(1).toDouble());
        HOOT_STR_EQUALS(-105.0, nodeResultIterator->value(2).toDouble());
        HOOT_STR_EQUALS("foo = bar\n", ApiDb::unescapeTags(nodeResultIterator->value(8)));
      }
      else if (ctr == 2)
     {
        HOOT_STR_EQUALS(nodeId2, nodeResultIterator->value(0).toLongLong());
        HOOT_STR_EQUALS(38.1, nodeResultIterator->value(1).toDouble());
        HOOT_STR_EQUALS(-106.0, nodeResultIterator->value(2).toDouble());
        HOOT_STR_EQUALS("foo2 = bar2\n", ApiDb::unescapeTags(nodeResultIterator->value(8)));
      }
      ctr++;
    }
    CPPUNIT_ASSERT_EQUAL(3, ctr);
  }

  void runSelectElementsCustomTagsTest()
  {
    HootApiDb database;
    database.open(getDbUrl());

    shared_ptr<OsmMap> map(new OsmMap());

    const shared_ptr<QList<long> > ids = insertTestMapWithCustomTags(database);
    CPPUNIT_ASSERT_EQUAL(4, ids->size());
    mapId = ids->at(0);
    const long nodeId1 = ids->at(1);
    const long nodeId2 = ids->at(2);
    const long nodeId3 = ids->at(3);

    shared_ptr<QSqlQuery> nodeResultIterator = database.selectElements(ElementType::Node);
    int ctr = 0;
    while (nodeResultIterator->next())
    {
      switch (ctr)
      {
        case 0:
        {
          HOOT_STR_EQUALS(nodeId1, nodeResultIterator->value(0).toLongLong());
          HOOT_STR_EQUALS(38.0, nodeResultIterator->value(ApiDb::NODES_LATITUDE).toDouble());
          HOOT_STR_EQUALS(-104.0, nodeResultIterator->value(ApiDb::NODES_LONGITUDE).toDouble());
          HOOT_STR_EQUALS("accuracy = 20.0\n"
                          "hoot:status = Unknown1\n",
                          ApiDb::unescapeTags(nodeResultIterator->value(8)));
        }
        break;

        case 1:
        {
          HOOT_STR_EQUALS(nodeId2, nodeResultIterator->value(0).toLongLong());
          HOOT_STR_EQUALS(38.0, nodeResultIterator->value(ApiDb::NODES_LATITUDE).toDouble());
          HOOT_STR_EQUALS(-105.0, nodeResultIterator->value(ApiDb::NODES_LONGITUDE).toDouble());
          HOOT_STR_EQUALS("error:circular = 20.0\n",
                          ApiDb::unescapeTags(nodeResultIterator->value(8)));
        }
        break;

        case 2:
        {
          HOOT_STR_EQUALS(nodeId3, nodeResultIterator->value(0).toLongLong());
          HOOT_STR_EQUALS(38.0, nodeResultIterator->value(ApiDb::NODES_LATITUDE).toDouble());
          HOOT_STR_EQUALS(-106.0, nodeResultIterator->value(ApiDb::NODES_LONGITUDE).toDouble());
          HOOT_STR_EQUALS("",
                          ApiDb::unescapeTags(nodeResultIterator->value(8)));
        }
        break;

        default:

          const QString errorMessage = "Invalid number of results: " + QString::number(ctr);
          CPPUNIT_FAIL(errorMessage.toStdString());
      }

      ctr++;
    }
    CPPUNIT_ASSERT_EQUAL(3, ctr);
  }

  void runUpdateNodeTest()
  {
    HootApiDb database;
    database.open(getDbUrl());

    const shared_ptr<QList<long> > ids = insertTestMap1(database);
    CPPUNIT_ASSERT_EQUAL(5, ids->size());
    mapId = ids->at(0);
    const long nodeId = ids->at(2);

    ServicesDbTestUtils::compareRecords(
          "SELECT latitude, longitude, visible, tile, version FROM " +
          HootApiDb::getNodesTableName(mapId) +
          " WHERE id=:id "
          "ORDER BY longitude",
          "38;-104;true;1329332431;1",
          (qlonglong)nodeId);


    database.setUserId(database.getOrCreateUser(userEmail(), "HootApiDbTest"));
    database.beginChangeset();
    database.updateNode(nodeId, 3.1415, 2.71828, 1, Tags());

    ServicesDbTestUtils::compareRecords(
          "SELECT latitude, longitude, visible, tile, version FROM " +
          HootApiDb::getNodesTableName(mapId) +
          " WHERE id=:id "
          "ORDER BY longitude",
          "3.1415;2.71828;true;3222453693;1",
          (qlonglong)nodeId);
  }

  void runUnescapeTags()
  {
    HOOT_STR_EQUALS("key = value\n", HootApiDb::unescapeTags("\"key\"=>\"value\""));
    HOOT_STR_EQUALS("key1 = value1\nkey2 = value2\n", HootApiDb::unescapeTags("\"key1\"=>\"value1\", \"key2\"=>\"value2\""));
    HOOT_STR_EQUALS("fixme = check: building or just a \"paved\" place\n", HootApiDb::unescapeTags("\"fixme\"=>\"check: building or just a \"paved\" place\""));
  }

  void setUp()
  {
    mapId = -1;

    deleteUser(userEmail());
  }

  void tearDown()
  {
    deleteUser(userEmail());

    // tear down the HootApiDb
    HootApiDb database;
    if (mapId > 0)
    {
      database.open(getDbUrl());
      if (database.mapExists(mapId))
      {
        database.deleteMap(mapId);
      }
      database.close();
    }
    mapId = -1;
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(HootApiDbTest, "slow");

}
