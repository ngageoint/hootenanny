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
#include <hoot/core/io/ServicesDb.h>
#include <hoot/core/util/ConfigOptions.h>

#include "../TestUtils.h"
#include "ServicesDbTestUtils.h"

namespace hoot
{

class ServicesDbTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(ServicesDbTest);
  CPPUNIT_TEST(runDbVersionTest);
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
  CPPUNIT_TEST_SUITE_END();

public:

  static QString userEmail() { return "ServicesDbTest@hoottestcpp.org"; }

  long mapId;

  void deleteUser(QString email)
  {
    ServicesDb db;
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

  void runDbVersionTest()
  {
    ServicesDb db;
    db.open(getDbUrl());
    QString version = db.getDbVersion();

    CPPUNIT_ASSERT_EQUAL(ServicesDb::expectedDbVersion().toStdString(), version.toStdString());
  }

  void runDropMapTest()
  {
    ServicesDb database;
    database.open(getDbUrl());
    const shared_ptr<QList<long> > ids = insertTestMap1(database);

    mapId = ids->at(0);
    CPPUNIT_ASSERT(database.mapExists(mapId));

    HOOT_STR_EQUALS(true, database._hasTable("current_nodes" + database._getMapIdString(mapId)));
    HOOT_STR_EQUALS(true, database._hasTable("current_relation_members" +
      database._getMapIdString(mapId)));
    HOOT_STR_EQUALS(true, database._hasTable("current_relations" +
      database._getMapIdString(mapId)));
    HOOT_STR_EQUALS(true, database._hasTable("current_way_nodes" +
      database._getMapIdString(mapId)));
    HOOT_STR_EQUALS(true, database._hasTable("current_ways" + database._getMapIdString(mapId)));

    database.deleteMap(mapId);

    HOOT_STR_EQUALS(false, database._hasTable("current_nodes" + database._getMapIdString(mapId)));
    HOOT_STR_EQUALS(false, database._hasTable("current_relation_members" +
      database._getMapIdString(mapId)));
    HOOT_STR_EQUALS(false, database._hasTable("current_relations" +
      database._getMapIdString(mapId)));
    HOOT_STR_EQUALS(false, database._hasTable("current_way_nodes" +
      database._getMapIdString(mapId)));
    HOOT_STR_EQUALS(false, database._hasTable("current_ways" + database._getMapIdString(mapId)));
  }

  const shared_ptr<QList<long> > insertTestMap1(ServicesDb& database)
  {
    database.transaction();

    QList<long> ids;
    long userId = database.getOrCreateUser(userEmail(), "ServicesDbTest");
    long mapId = database.insertMap("foo", userId, true);
    ids.append(mapId);
    long changeSetId = database.insertChangeSet(mapId, userId, Tags());
    ids.append(changeSetId);

    Tags t;
    t["foo"] = "bar";
    long nodeId = database.insertNode(mapId, -1, 38, -104, changeSetId, t, true);
    ids.append(nodeId);

    Tags t2;
    t2["highway"] = "primary";
    long wayId = database.insertWay(mapId, -1, changeSetId, t2, true);
    ids.append(wayId);
    vector<long> nodeIds;
    nodeIds.push_back(nodeId);
    database.insertWayNodes(mapId, wayId, nodeIds);

    Tags rt;
    rt["type"] = "multistuff";
    long relationId = database.insertRelation(mapId, -1, changeSetId, rt, true);
    ids.append(relationId);
    database.insertRelationMembers(mapId, relationId, ElementType::Way, wayId, "wayrole", 0);
    database.insertRelationMembers(mapId, relationId, ElementType::Node, nodeId, "noderole", 1);

    database.commit();

    return shared_ptr<QList<long> >(new QList<long>(ids));
  }

  const shared_ptr<QList<long> > insertTestMap2(ServicesDb& database)
  {
    database.transaction();

    QList<long> ids;
    long userId = database.getOrCreateUser(userEmail(), "ServicesDbTest");
    long mapId = database.insertMap("foo", userId, true);
    ids.append(mapId);
    long changeSetId = database.insertChangeSet(mapId, userId);

    Tags t;
    long nodeId = database.insertNode(mapId, -1, 38.0, -104, changeSetId, t, true);
    ids.append(nodeId);
    t["foo"] = "bar";
    nodeId = database.insertNode(mapId, -1, 37.9, -105, changeSetId, t, true);
    ids.append(nodeId);
    t.clear();
    t["foo2"] = "bar2";
    nodeId = database.insertNode(mapId, -1, 38.1, -106, changeSetId, t, true);
    ids.append(nodeId);

    Tags wt;
    wt["highway"] = "primary";
    long wayId = database.insertWay(mapId, -1, changeSetId, wt, true);
    ids.append(wayId);
    vector<long> nodeIds;
    nodeIds.push_back(nodeId);
    database.insertWayNodes(mapId, wayId, nodeIds);
    wt.clear();
    wt["highway2"] = "primary2";
    wayId = database.insertWay(mapId, -1, changeSetId, wt, true);
    ids.append(wayId);
    database.insertWayNodes(mapId, wayId, nodeIds);

    t.clear();
    t["type"] = "multistuff";
    long relationId = database.insertRelation(mapId, -1, changeSetId, t, true);
    ids.append(relationId);
    database.insertRelationMembers(mapId, relationId, ElementType::Way, wayId, "wayrole", 0);
    database.insertRelationMembers(mapId, relationId, ElementType::Node, nodeId, "noderole", 1);
    t.clear();
    t["type2"] = "multistuff2";
    relationId = database.insertRelation(mapId, -1, changeSetId, t, true);
    ids.append(relationId);
    database.insertRelationMembers(mapId, relationId, ElementType::Way, wayId, "wayrole", 0);
    database.insertRelationMembers(mapId, relationId, ElementType::Node, nodeId, "noderole", 1);

    database.commit();

    return shared_ptr<QList<long> >(new QList<long>(ids));
  }

  const shared_ptr<QList<long> > insertTestMapWithCustomTags(ServicesDb& database)
  {
    QList<long> ids;
    database.transaction();
    long userId = database.getOrCreateUser(userEmail(), "ServicesDbTest");
    long mapId = database.insertMap("foo", userId, true);
    ids.append(mapId);
    long changeSetId = database.insertChangeSet(mapId, userId);

    Tags t;
    t["hoot:status"] = "Unknown1";
    t["accuracy"] = "20.0";
    long nodeId = database.insertNode(mapId, -1, 38, -104, changeSetId, t, true);
    ids.append(nodeId);

    t.clear();
    t["error:circular"] = "20.0";
    nodeId = database.insertNode(mapId, -1, 38, -105, changeSetId, t, true);
    ids.append(nodeId);

    t.clear();
    nodeId = database.insertNode(mapId, -1, 38, -106, changeSetId, t, true);
    ids.append(nodeId);

    database.commit();

    return shared_ptr<QList<long> >(new QList<long>(ids));
  }

  void runInsertTest()
  {
    ServicesDb db;
    db.open(getDbUrl());

    insertTestMap1(db);
  }

  void runMapExistsTest()
  {
    ServicesDb database;
    database.open(getDbUrl());
    const shared_ptr<QList<long> > ids = insertTestMap1(database);

    mapId = ids->at(0);
    CPPUNIT_ASSERT(database.mapExists(mapId));
  }

  void runChangesetExistsTest()
  {
    ServicesDb database;
    database.open(getDbUrl());
    const shared_ptr<QList<long> > ids = insertTestMap1(database);

    const long changesetId = ids->at(1);
    CPPUNIT_ASSERT(database.changesetExists(ids->at(0), changesetId));
  }

  void runNumElementsTest()
  {
    ServicesDb database;
    database.open(getDbUrl());

    const shared_ptr<QList<long> > ids = insertTestMap1(database);

    mapId = ids->at(0);
    CPPUNIT_ASSERT_EQUAL((long)1, database.numElements(mapId, ElementType::Node));
  }

  void runSelectNodeIdsForWayTest()
  {
    ServicesDb database;
    database.open(getDbUrl());

    const shared_ptr<QList<long> > ids = insertTestMap1(database);
    CPPUNIT_ASSERT_EQUAL(5, ids->size());
    mapId = ids->at(0);
    const long wayId = ids->at(3);

    HOOT_STR_EQUALS("[1]{1}", database.selectNodeIdsForWay(mapId, wayId));
  }

  void runSelectMembersForRelationTest()
  {
    ServicesDb database;
    database.open(getDbUrl());

    const shared_ptr<QList<long> > ids = insertTestMap1(database);
    CPPUNIT_ASSERT_EQUAL(5, ids->size());
    mapId = ids->at(0);
    const long relationId = ids->at(4);

    vector<RelationData::Entry> members = database.selectMembersForRelation(mapId, relationId);
    HOOT_STR_EQUALS("[2]{Entry: role: wayrole, eid: Way:1, Entry: role: noderole, eid: Node:1}",
                    members);
  }

  void runSelectAllElementsTest()
  {
    ServicesDb database;
    database.open(getDbUrl());

    shared_ptr<OsmMap> map(new OsmMap());

    const shared_ptr<QList<long> > ids = insertTestMap1(database);
    CPPUNIT_ASSERT_EQUAL(5, ids->size());
    mapId = ids->at(0);
    const long nodeId = ids->at(2);
    const long wayId = ids->at(3);
    const long relationId = ids->at(4);

    shared_ptr<QSqlQuery> nodeResultIterator = database.selectAllElements(mapId, ElementType::Node);
    int ctr = 0;
    while (nodeResultIterator->next())
    {
      HOOT_STR_EQUALS(nodeId, nodeResultIterator->value(0).toLongLong());
      HOOT_STR_EQUALS(38.0, nodeResultIterator->value(1).toLongLong() /
        (double)ServicesDb::COORDINATE_SCALE);
      HOOT_STR_EQUALS(-104.0, nodeResultIterator->value(2).toLongLong() /
        (double)ServicesDb::COORDINATE_SCALE);
      HOOT_STR_EQUALS("foo = bar\n", ServicesDb::unescapeTags(nodeResultIterator->value(8)));

      ctr++;
    }
    CPPUNIT_ASSERT_EQUAL(1, ctr);

    shared_ptr<QSqlQuery> wayResultIterator = database.selectAllElements(mapId, ElementType::Way);
    ctr = 0;
    while (wayResultIterator->next())
    {
      HOOT_STR_EQUALS(wayId, wayResultIterator->value(0).toLongLong());
      vector<long> v = database.selectNodeIdsForWay(mapId, wayId);
      HOOT_STR_EQUALS(1, v.size());
      CPPUNIT_ASSERT_EQUAL(nodeId, v[0]);

      HOOT_STR_EQUALS("highway = primary\n", ServicesDb::unescapeTags(wayResultIterator->value(5)));
      ctr++;
    }
    CPPUNIT_ASSERT_EQUAL(1, ctr);

    shared_ptr<QSqlQuery> relationResultIterator =
      database.selectAllElements(mapId, ElementType::Relation);
    ctr = 0;
    while (relationResultIterator->next())
    {
      HOOT_STR_EQUALS(relationId, relationResultIterator->value(0).toLongLong());
      vector<RelationData::Entry> members = database.selectMembersForRelation(mapId, relationId);
      HOOT_STR_EQUALS("[2]{Entry: role: wayrole, eid: Way:1, Entry: role: noderole, eid: Node:1}",
                      members);
      HOOT_STR_EQUALS("type = multistuff\n",
                      ServicesDb::unescapeTags(relationResultIterator->value(5)));
      ctr++;
    }

    CPPUNIT_ASSERT_EQUAL(1, ctr);
  }

  void runSelectElementsTest()
  {
    ServicesDb database;
    database.open(getDbUrl());

    shared_ptr<OsmMap> map(new OsmMap());

    const shared_ptr<QList<long> > ids = insertTestMap2(database);
    CPPUNIT_ASSERT_EQUAL(8, ids->size());
    mapId = ids->at(0);
    const long nodeId1 = ids->at(2);
    const long nodeId2 = ids->at(3);

    shared_ptr<QSqlQuery> nodeResultIterator =
      database.selectElements(mapId, -1, ElementType::Node, 2, 1);
    int ctr = 0;
    while (nodeResultIterator->next())
    {
      switch (ctr)
      {
        case 0:
        {
          HOOT_STR_EQUALS(nodeId1, nodeResultIterator->value(0).toLongLong());
          HOOT_STR_EQUALS(37.9, nodeResultIterator->value(1).toLongLong() /
            (double)ServicesDb::COORDINATE_SCALE);
          HOOT_STR_EQUALS(-105.0, nodeResultIterator->value(2).toLongLong() /
            (double)ServicesDb::COORDINATE_SCALE);
          HOOT_STR_EQUALS("foo = bar\n", ServicesDb::unescapeTags(nodeResultIterator->value(8)));
        }
        break;

        case 1:
        {
          HOOT_STR_EQUALS(nodeId2, nodeResultIterator->value(0).toLongLong());
          HOOT_STR_EQUALS(38.1, nodeResultIterator->value(1).toLongLong() /
            (double)ServicesDb::COORDINATE_SCALE);
          HOOT_STR_EQUALS(-106.0, nodeResultIterator->value(2).toLongLong() /
            (double)ServicesDb::COORDINATE_SCALE);
          HOOT_STR_EQUALS("foo2 = bar2\n", ServicesDb::unescapeTags(nodeResultIterator->value(8)));
        }
        break;

        default:

          const QString errorMessage = "Invalid number of results: " + QString::number(ctr);
          CPPUNIT_FAIL(errorMessage.toStdString());
      }
      ctr++;
    }
    CPPUNIT_ASSERT_EQUAL(2, ctr);
  }

  void runSelectElementsCustomTagsTest()
  {
    ServicesDb database;
    database.open(getDbUrl());

    shared_ptr<OsmMap> map(new OsmMap());

    const shared_ptr<QList<long> > ids = insertTestMapWithCustomTags(database);
    CPPUNIT_ASSERT_EQUAL(4, ids->size());
    mapId = ids->at(0);
    const long nodeId1 = ids->at(1);
    const long nodeId2 = ids->at(2);
    const long nodeId3 = ids->at(3);

    shared_ptr<QSqlQuery> nodeResultIterator = database.selectAllElements(mapId, ElementType::Node);
    int ctr = 0;
    while (nodeResultIterator->next())
    {
      switch (ctr)
      {
        case 0:
        {
          HOOT_STR_EQUALS(nodeId1, nodeResultIterator->value(0).toLongLong());
          HOOT_STR_EQUALS(38.0, nodeResultIterator->value(ServicesDb::NODES_LATITUDE).toLongLong() /
            (double)ServicesDb::COORDINATE_SCALE);
          HOOT_STR_EQUALS(-104.0, nodeResultIterator->value(
            ServicesDb::NODES_LONGITUDE).toLongLong() / (double)ServicesDb::COORDINATE_SCALE);
          HOOT_STR_EQUALS("accuracy = 20.0\n"
                          "hoot:status = Unknown1\n",
                          ServicesDb::unescapeTags(nodeResultIterator->value(8)));
        }
        break;

        case 1:
        {
          HOOT_STR_EQUALS(nodeId2, nodeResultIterator->value(0).toLongLong());
          HOOT_STR_EQUALS(38.0, nodeResultIterator->value(ServicesDb::NODES_LATITUDE).toLongLong() /
            (double)ServicesDb::COORDINATE_SCALE);
          HOOT_STR_EQUALS(-105.0, nodeResultIterator->value(
            ServicesDb::NODES_LONGITUDE).toLongLong() / (double)ServicesDb::COORDINATE_SCALE);
          HOOT_STR_EQUALS("error:circular = 20.0\n",
                          ServicesDb::unescapeTags(nodeResultIterator->value(8)));
        }
        break;

        case 2:
        {
          HOOT_STR_EQUALS(nodeId3, nodeResultIterator->value(0).toLongLong());
          HOOT_STR_EQUALS(38.0, nodeResultIterator->value(ServicesDb::NODES_LATITUDE).toLongLong() /
            (double)ServicesDb::COORDINATE_SCALE);
          HOOT_STR_EQUALS(-106.0, nodeResultIterator->value(
            ServicesDb::NODES_LONGITUDE).toLongLong() / (double)ServicesDb::COORDINATE_SCALE);
          HOOT_STR_EQUALS("",
                          ServicesDb::unescapeTags(nodeResultIterator->value(8)));
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
    ServicesDb database;
    database.open(getDbUrl());

    const shared_ptr<QList<long> > ids = insertTestMap1(database);
    CPPUNIT_ASSERT_EQUAL(5, ids->size());
    mapId = ids->at(0);
    const long nodeId = ids->at(2);

    ServicesDbTestUtils::compareRecords(
          "SELECT latitude, longitude, visible, tile, version FROM " +
          ServicesDb::_getNodesTableName(mapId) +
          " WHERE id=:id "
          "ORDER BY longitude",
          "380000000;-1040000000;true;1329332431;1",
          (qlonglong)nodeId);


    long userId = database.getOrCreateUser(userEmail(), "ServicesDbTest");
    long changeSetId = database.insertChangeSet(mapId, userId);
    database.updateNode(mapId, nodeId, 3.1415, 2.71828, changeSetId, Tags());

    ServicesDbTestUtils::compareRecords(
          "SELECT latitude, longitude, visible, tile, version FROM " +
          ServicesDb::_getNodesTableName(mapId) +
          " WHERE id=:id "
          "ORDER BY longitude",
          "31415000;27182800;true;3222453693;1",
          (qlonglong)nodeId);
  }

  void setUp()
  {
    mapId = -1;

    deleteUser(userEmail());
  }

  void tearDown()
  {
    deleteUser(userEmail());

    if (mapId > 0)
    {
      ServicesDb database;
      database.open(getDbUrl());
      if (database.mapExists(mapId))
      {
        database.deleteMap(mapId);
      }
    }
    mapId = -1;
  }

};

//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ServicesDbTest, "current");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ServicesDbTest, "quick");

}
