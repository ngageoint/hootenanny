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

  // standard hoot services tests
  CPPUNIT_TEST(runDbVersionTest);
  CPPUNIT_TEST(runOpenServicesTest);
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

  // osm apidb tests
  CPPUNIT_TEST(runOpenOsmApiTest);
  CPPUNIT_TEST(runSelectAllElementsOsmApiTest);

  CPPUNIT_TEST_SUITE_END();

public:

  static const long COORDINATE_SCALE = 1e11;

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

  QUrl getOsmApiDbUrl()
  {
    LOG_DEBUG(QString("Got URL for OSM API DB: ") + ServicesDbTestUtils::getOsmApiDbUrl().toString());
    return ServicesDbTestUtils::getOsmApiDbUrl();
  }

  void runOpenServicesTest()
  {
    ServicesDb db;
    CPPUNIT_ASSERT_EQUAL(ServicesDb::DBTYPE_UNSUPPORTED, db.getDatabaseType());
    db.open(getDbUrl());
    CPPUNIT_ASSERT_EQUAL(ServicesDb::DBTYPE_SERVICES, db.getDatabaseType());
    db.close();
    CPPUNIT_ASSERT_EQUAL(ServicesDb::DBTYPE_UNSUPPORTED, db.getDatabaseType());
  }

  /***********************************************************************************************
   * Purpose: Quick test to open the Osm ApiDb database
   * To see the output from this test, type the following:
   *   bin/HootTest --debug --single hoot::ServicesDbTest::runOpenOsmApiTest
   * *********************************************************************************************
   */
  void runOpenOsmApiTest()
  {
    Settings s = conf();
    ServicesDb db;
    CPPUNIT_ASSERT_EQUAL(ServicesDb::DBTYPE_UNSUPPORTED, db.getDatabaseType());
    db.open(QUrl(ConfigOptions(s).getServicesDbTestUrlOsmapi()));
    CPPUNIT_ASSERT_EQUAL(ServicesDb::DBTYPE_OSMAPI, db.getDatabaseType());
    db.close();
    CPPUNIT_ASSERT_EQUAL(ServicesDb::DBTYPE_UNSUPPORTED, db.getDatabaseType());

    // Reset this back to default value
    s.set(ConfigOptions(s).getServicesDbTestUrlOsmapiKey(), ConfigOptions(s).getServicesDbTestUrlOsmapiDefaultValue());
  }

  /***********************************************************************************************
   * Purpose: Print the current Services DB version
   * To see the version from this test, type the following:
   *   bin/HootTest --debug --single hoot::ServicesDbTest::runDbVersionTest
   * *********************************************************************************************
   */
  void runDbVersionTest()
  {
    ServicesDb db;
    db.open(getDbUrl());
    QString version = db.getDbVersion();
    LOG_DEBUG("The version = " << version << ".");
    CPPUNIT_ASSERT_EQUAL(ServicesDb::expectedDbVersion().toStdString(), version.toStdString());
  }

  void runDropMapTest()
  {
    ServicesDb database;
    database.open(getDbUrl());

    const shared_ptr<QList<long> > ids = insertTestMap1(database);

    mapId = ids->at(0);

    //LOG_WARN("Map ID out of ITM1: " << mapId);

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
    database.setUserId(database.getOrCreateUser(userEmail(), "ServicesDbTest"));

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

  const shared_ptr<QList<long> > insertTestMap2(ServicesDb& database)
  {
    database.transaction();

    QList<long> ids;
    database.setUserId(database.getOrCreateUser(userEmail(), "ServicesDbTest"));
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

  const shared_ptr<QList<long> > insertTestMapWithCustomTags(ServicesDb& database)
  {
    QList<long> ids;
    database.transaction();
    database.setUserId(database.getOrCreateUser(userEmail(), "ServicesDbTest"));
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
    CPPUNIT_ASSERT(database.changesetExists(changesetId));
  }

  void runNumElementsTest()
  {
    ServicesDb database;
    database.open(getDbUrl());

    const shared_ptr<QList<long> > ids = insertTestMap1(database);

    mapId = ids->at(0);
    CPPUNIT_ASSERT_EQUAL((long)1, database.numElements(ElementType::Node));
  }

  void runSelectNodeIdsForWayTest()
  {
    ServicesDb database;
    database.open(getDbUrl());

    const shared_ptr<QList<long> > ids = insertTestMap1(database);
    CPPUNIT_ASSERT_EQUAL(5, ids->size());
    mapId = ids->at(0);
    const long wayId = ids->at(3);

    HOOT_STR_EQUALS("[1]{1}", database.selectNodeIdsForWay(wayId));
  }

  void runSelectMembersForRelationTest()
  {
    ServicesDb database;
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
    ServicesDb database;
    database.open(getDbUrl());

    shared_ptr<OsmMap> map(new OsmMap());

    const shared_ptr<QList<long> > ids = insertTestMap1(database);
    CPPUNIT_ASSERT_EQUAL(5, ids->size());
    mapId = ids->at(0);
    const long nodeId = ids->at(2);
    const long wayId = ids->at(3);
    const long relationId = ids->at(4);

    shared_ptr<QSqlQuery> nodeResultIterator = database.selectAllElements(ElementType::Node);
    int ctr = 0;
    while (nodeResultIterator->next())
    {
      for(int j=0;j<10;j++) { LOG_DEBUG("VALUE = "+nodeResultIterator->value(j).toString()); }

      HOOT_STR_EQUALS(nodeId, nodeResultIterator->value(0).toLongLong());
      HOOT_STR_EQUALS(38.0, nodeResultIterator->value(1).toDouble());
      HOOT_STR_EQUALS(-104.0, nodeResultIterator->value(2).toDouble());
      stringstream s;
      s << ServicesDb::unescapeTags(nodeResultIterator->value(8));
      LOG_DEBUG("unescapeTag :"+s.str());
      HOOT_STR_EQUALS("foo = bar\n", ServicesDb::unescapeTags(nodeResultIterator->value(8)));

      ctr++;
    }
    CPPUNIT_ASSERT_EQUAL(1, ctr);

    shared_ptr<QSqlQuery> wayResultIterator = database.selectAllElements(ElementType::Way);
    ctr = 0;
    while (wayResultIterator->next())
    {
      HOOT_STR_EQUALS(wayId, wayResultIterator->value(0).toLongLong());
      vector<long> v = database.selectNodeIdsForWay(wayId);
      HOOT_STR_EQUALS(1, v.size());
      CPPUNIT_ASSERT_EQUAL(nodeId, v[0]);

      HOOT_STR_EQUALS("highway = primary\n", ServicesDb::unescapeTags(wayResultIterator->value(5)));
      ctr++;
    }
    CPPUNIT_ASSERT_EQUAL(1, ctr);

    shared_ptr<QSqlQuery> relationResultIterator =
      database.selectAllElements(ElementType::Relation);
    ctr = 0;
    while (relationResultIterator->next())
    {
      HOOT_STR_EQUALS(relationId, relationResultIterator->value(0).toLongLong());
      vector<RelationData::Entry> members = database.selectMembersForRelation(relationId);
      HOOT_STR_EQUALS("[2]{Entry: role: wayrole, eid: Way:1, Entry: role: noderole, eid: Node:1}",
                      members);
      HOOT_STR_EQUALS("type = multistuff\n",
                      ServicesDb::unescapeTags(relationResultIterator->value(5)));
      ctr++;
    }

    CPPUNIT_ASSERT_EQUAL(1, ctr);
  }

  void runSelectAllElementsOsmApiTest()
  {
    ServicesDb database;
    database.open(ServicesDbTestUtils::getOsmApiDbUrl());

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

    QString auth = "-h "+dbHost+" -p "+dbPort+" -U "+dbUser;

    /////////////////////////////////////
    // INSERT NODES INTO DB
    /////////////////////////////////////

    // list of insertions
    QList<long> ids;
    QList<QString> keys = QList<QString>() << "highway" << "accuracy" << "foo";
    QList<QString> values = QList<QString>() << "road" << "5" << "bar";
    QList<float> lats = QList<float>() << 38.4 << 38;
    QList<float> lons = QList<float>() << -106.5 << -104;

    // Insert nodes
    QString cmd = "export PGPASSWORD="+dbPassword+"; \
      psql "+auth+" -f ${HOOT_HOME}/test-files/servicesdb/users.sql > /dev/null 2>&1; \
      psql "+auth+" -f ${HOOT_HOME}/test-files/servicesdb/changesets.sql > /dev/null 2>&1; \
      psql "+auth+" -f ${HOOT_HOME}/test-files/servicesdb/nodes.sql > /dev/null 2>&1";

    if( std::system(cmd.toStdString().c_str()) != 0 )
    {
      LOG_WARN("Failed postgres command.  Exiting test.");
      return;
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

    shared_ptr<QSqlQuery> nodeResultIterator = database.selectAllElements(ElementType::Node);

    // check if db active or not
    assert(nodeResultIterator->isActive());

    const int numNodeFields = 10;
    long long lastId = LLONG_MIN;

    // read through the elements until the number inserted for this test is reached
    // - the number inserted is determined by ids.size()
    int elementCtr = ids.size()-1;
    int tagIndx = -1;
    while( nodeResultIterator->next() )
    {
      long long id = nodeResultIterator->value(0).toLongLong();
      if( lastId != id )
      {
        if(elementCtr < 0) break;

        // perform the comparison tests
        LOG_DEBUG(QString("Processing element ")+QString::number(elementCtr+1));
        // test the first line's data which is the current_nodes (main data): id, lat, lon, tag1
        HOOT_STR_EQUALS(ids[elementCtr], id);
        HOOT_STR_EQUALS(lats[elementCtr], nodeResultIterator->value(1).toLongLong() /
          (double)ServicesDb::COORDINATE_SCALE);
        HOOT_STR_EQUALS(lons[elementCtr], nodeResultIterator->value(2).toLongLong() /
          (double)ServicesDb::COORDINATE_SCALE);
        lastId = id;
        elementCtr--;
      }

      // verify the values written to the DB upon their read-back
      for(int j=0;j<numNodeFields;j++) { LOG_DEBUG("VALUE = "+nodeResultIterator->value(j).toString()); }

      // read the tag for as many rows as there are tags
      QString key = nodeResultIterator->value(8).toString();
      LOG_DEBUG(QString("Processing tag ")+key);
      tagIndx = ServicesDbTestUtils::findIndex(keys, key);
      HOOT_STR_EQUALS(QString(keys[tagIndx]+" = "+values[tagIndx]+"\n").toStdString().c_str(),
        ServicesDb::unescapeTags(database.extractTagFromRow_OsmApi(nodeResultIterator, ElementType::Node)));
    }

    ///////////////////////////////////////////////
    /// Insert a way into the Osm Api DB
    ///////////////////////////////////////////////

    const long nodeId1 = 1;
    const long nodeId2 = 2;
    ids.clear();
    Tags t2;
    t2["highway"] = "primary";
    long insertedWayId = 1;
    ids.append(insertedWayId);
    vector<long> nodeIds;
    nodeIds.push_back(nodeId1);
    nodeIds.push_back(nodeId2);

    cmd = "export PGPASSWORD="+dbPassword+";\
      psql "+auth+" -f ${HOOT_HOME}/test-files/servicesdb/ways.sql > /dev/null 2>&1";
    if( std::system(cmd.toStdString().c_str()) != 0 )
    {
      LOG_WARN("Failed postgres command.  Exiting test.");
      return;
    }

    ///////////////////////////////////////////////
    /// Reads the ways from the Osm Api DB
    ///////////////////////////////////////////////

    shared_ptr<QSqlQuery> wayResultIterator = database.selectAllElements(ElementType::Way);

    // check again if db active or not
    assert(wayResultIterator->isActive());

    const int numWayFields = 7;
    lastId = LLONG_MIN;

    // read through the elements until the number inserted for this test is reached
    // - the number inserted is determined by ids.size()
    elementCtr = ids.size()-1;
    tagIndx = -1;
    while( wayResultIterator->next() )
    {
      long long wayId = wayResultIterator->value(0).toLongLong();
      if( lastId != wayId )
      {
        if(elementCtr < 0) break;

        // perform the comparison tests
        LOG_DEBUG(QString("Processing element ")+QString::number(elementCtr+1));
        // test the first line's data which is the current_ways (main data): id
        LOG_DEBUG("ids = "+QString::number(ids[elementCtr]));
        LOG_DEBUG("wayId = "+QString::number(wayId));

        HOOT_STR_EQUALS(ids[elementCtr], wayId);

        // get the way nodes and do some minimal testing
        vector<long> v = database.selectNodeIdsForWay(wayId);
        HOOT_STR_EQUALS(2, v.size());
        CPPUNIT_ASSERT_EQUAL(nodeId2, v[1]);
        CPPUNIT_ASSERT_EQUAL(nodeId1, v[0]);

        // mark this way id processed
        lastId = wayId;
        elementCtr--;
      }

      // verify the values written to the DB upon their read-back
      for(int j=0;j<numWayFields;j++) { LOG_DEBUG("VALUE = "+wayResultIterator->value(j).toString()); }

      // read the tag for as many rows as there are tags
      QString key = wayResultIterator->value(ServicesDb::WAYS_TAGS).toString();
      LOG_DEBUG(QString("Processing tag ")+key);
      HOOT_STR_EQUALS("highway = primary\n", ServicesDb::unescapeTags(
        database.extractTagFromRow_OsmApi(wayResultIterator, ElementType::Way)));
    }

    ///////////////////////////////////////////////
    /// Insert a relation into the Osm Api DB
    ///////////////////////////////////////////////

    const long nodeId3 = nodeIds.at(0);
    const long wayId1 = ids.at(0);
    ids.clear();
    Tags rt;
    rt["type"] = "multistuff";
    long relationId = 1;
    ids.append(relationId);

    cmd = "export PGPASSWORD="+dbPassword+";\
      psql "+auth+" -f ${HOOT_HOME}/test-files/servicesdb/relations.sql > /dev/null 2>&1";
    if( std::system(cmd.toStdString().c_str()) != 0 )
    {
      LOG_WARN("Failed postgres command.  Exiting test.");
      return;
    }

    ///////////////////////////////////////////////
    /// Reads the relations from the Osm Api DB
    ///////////////////////////////////////////////

    shared_ptr<QSqlQuery> relationResultIterator =
      database.selectAllElements(ElementType::Relation);

    // check again if db active or not
    assert(relationResultIterator->isActive());

    const int numRelationFields = 7;
    lastId = LLONG_MIN;

    // read through the elements until the number inserted for this test is reached
    // - the number inserted is determined by ids.size()
    elementCtr = ids.size()-1;
    tagIndx = -1;
    while ( relationResultIterator->next() )
    {
      long long relId = relationResultIterator->value(0).toLongLong();
      if( lastId != relId )
      {
        if(elementCtr < 0) break;

        // perform the comparison tests
        LOG_DEBUG(QString("Processing element ")+QString::number(elementCtr+1));
        // test the first line's data which is the current_nodes (main data): id
        LOG_DEBUG("ids = "+QString::number(ids[elementCtr]));
        LOG_DEBUG("relId = "+QString::number(relId));
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
      }

      // verify the values written to the DB upon their read-back
      for(int j=0;j<numRelationFields;j++) { LOG_DEBUG("VALUE = "+relationResultIterator->value(j).toString()); }

      // read the tag for as many rows as there are tags
      QString key = relationResultIterator->value(ServicesDb::WAYS_TAGS).toString();
      LOG_DEBUG(QString("Processing tag ")+key);
      HOOT_STR_EQUALS("type = multistuff\n", ServicesDb::unescapeTags(
        database.extractTagFromRow_OsmApi(relationResultIterator, ElementType::Relation)));
    }

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
      database.selectElements(ElementType::Node);
    int ctr = 0;
    while (nodeResultIterator->next())
    {
     if (ctr == 1)
     {
        HOOT_STR_EQUALS(nodeId1, nodeResultIterator->value(0).toLongLong());
        HOOT_STR_EQUALS(37.9, nodeResultIterator->value(1).toDouble());
        HOOT_STR_EQUALS(-105.0, nodeResultIterator->value(2).toDouble());
        HOOT_STR_EQUALS("foo = bar\n", ServicesDb::unescapeTags(nodeResultIterator->value(8)));
      }
      else if (ctr == 2)
     {
        HOOT_STR_EQUALS(nodeId2, nodeResultIterator->value(0).toLongLong());
        HOOT_STR_EQUALS(38.1, nodeResultIterator->value(1).toDouble());
        HOOT_STR_EQUALS(-106.0, nodeResultIterator->value(2).toDouble());
        HOOT_STR_EQUALS("foo2 = bar2\n", ServicesDb::unescapeTags(nodeResultIterator->value(8)));
      }
      ctr++;
    }
    CPPUNIT_ASSERT_EQUAL(3, ctr);
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

    shared_ptr<QSqlQuery> nodeResultIterator = database.selectAllElements(ElementType::Node);
    int ctr = 0;
    while (nodeResultIterator->next())
    {
      switch (ctr)
      {
        case 0:
        {
          HOOT_STR_EQUALS(nodeId1, nodeResultIterator->value(0).toLongLong());
          HOOT_STR_EQUALS(38.0, nodeResultIterator->value(ServicesDb::NODES_LATITUDE).toDouble());
          HOOT_STR_EQUALS(-104.0, nodeResultIterator->value(ServicesDb::NODES_LONGITUDE).toDouble());
          HOOT_STR_EQUALS("accuracy = 20.0\n"
                          "hoot:status = Unknown1\n",
                          ServicesDb::unescapeTags(nodeResultIterator->value(8)));
        }
        break;

        case 1:
        {
          HOOT_STR_EQUALS(nodeId2, nodeResultIterator->value(0).toLongLong());
          HOOT_STR_EQUALS(38.0, nodeResultIterator->value(ServicesDb::NODES_LATITUDE).toDouble());
          HOOT_STR_EQUALS(-105.0, nodeResultIterator->value(ServicesDb::NODES_LONGITUDE).toDouble());
          HOOT_STR_EQUALS("error:circular = 20.0\n",
                          ServicesDb::unescapeTags(nodeResultIterator->value(8)));
        }
        break;

        case 2:
        {
          HOOT_STR_EQUALS(nodeId3, nodeResultIterator->value(0).toLongLong());
          HOOT_STR_EQUALS(38.0, nodeResultIterator->value(ServicesDb::NODES_LATITUDE).toDouble());
          HOOT_STR_EQUALS(-106.0, nodeResultIterator->value(ServicesDb::NODES_LONGITUDE).toDouble());
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
          "38;-104;true;1329332431;1",
          (qlonglong)nodeId);


    database.setUserId(database.getOrCreateUser(userEmail(), "ServicesDbTest"));
    database.beginChangeset();
    database.updateNode(nodeId, 3.1415, 2.71828, 1, Tags());

    ServicesDbTestUtils::compareRecords(
          "SELECT latitude, longitude, visible, tile, version FROM " +
          ServicesDb::_getNodesTableName(mapId) +
          " WHERE id=:id "
          "ORDER BY longitude",
          "3.1415;2.71828;true;3222453693;1",
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

    // tear down the ServicesDB
    ServicesDb database;
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

    // tear down the osm api db
    database.open(ServicesDbTestUtils::getOsmApiDbUrl());
    database.deleteData_OsmApi();
    database.close();
  }

};

//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ServicesDbTest, "current");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ServicesDbTest, "slow");

}
