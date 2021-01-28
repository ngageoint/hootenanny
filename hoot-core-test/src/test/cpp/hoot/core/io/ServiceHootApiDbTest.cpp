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
 * @copyright Copyright (C) 2013, 2014, 2015, 2016, 2017, 2018, 2019, 2020, 2021 DigitalGlobe (http://www.digitalglobe.com/)
 */

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/io/HootApiDb.h>
#include <hoot/core/io/ServicesDbTestUtils.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/schema/MetadataTags.h>

//  Qt
#include <QTextCodec>

using namespace std;

namespace hoot
{

class ServiceHootApiDbTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(ServiceHootApiDbTest);
  CPPUNIT_TEST(runDbVersionTest);
  CPPUNIT_TEST(runOpenTest);
  CPPUNIT_TEST(runDropMapTest);
  CPPUNIT_TEST(runInsertTest);
  CPPUNIT_TEST(runMapExistsTest);
  CPPUNIT_TEST(runMapUrlWithNameTest);
  CPPUNIT_TEST(runChangesetExistsTest);
  CPPUNIT_TEST(runNumElementsTest);
  CPPUNIT_TEST(runSelectAllElementsTest);
  CPPUNIT_TEST(runSelectElementsTest);
  CPPUNIT_TEST(runSelectElementsCustomTagsTest);
  CPPUNIT_TEST(runSelectNodeIdsForWayTest);
  CPPUNIT_TEST(runSelectMembersForRelationTest);
  CPPUNIT_TEST(runUpdateNodeTest);
  CPPUNIT_TEST(runUnescapeTags);
  CPPUNIT_TEST(runAvailableMapNamesTest);
  CPPUNIT_TEST_SUITE_END();

public:

  QString userEmail() { return QString("%1.ServiceHootApiDbTest@hoottestcpp.org").arg(testName); }
  QString userName()  { return QString("%1.ServiceHootApiDbTest").arg(testName); }

  long mapId;
  QString testName;

  void setUpTest(const QString& test_name)
  {
    mapId = -1;
    testName = test_name;
    ServicesDbTestUtils::deleteUser(userEmail());
    HootApiDb database;

    database.open(ServicesDbTestUtils::getDbModifyUrl());
    database.getOrCreateUser(userEmail(), QString("%1.ServiceHootApiDbReaderTest").arg(testName));
    database.close();
  }

  virtual void tearDown()
  {
    ServicesDbTestUtils::deleteUser(userEmail());

    if (mapId != -1)
    {
      HootApiDb database;
      database.open(ServicesDbTestUtils::getDbModifyUrl());
      database.deleteMap(mapId);
      database.close();
    }
  }

  void runOpenTest()
  {
    setUpTest("runOpenTest");
    HootApiDb db;
    db.open(ServicesDbTestUtils::getDbModifyUrl());
    CPPUNIT_ASSERT_EQUAL(true, db.getDB().isOpen());
    db.close();
    CPPUNIT_ASSERT_EQUAL(false, db.getDB().isOpen());
  }

  void runDbVersionTest()
  {
    setUpTest("runDbVersionTest");
    HootApiDb db;
    db.open(ServicesDbTestUtils::getDbModifyUrl());
    QString version = db.getHootDbVersion();
    LOG_DEBUG("The version = " << version << ".");
    CPPUNIT_ASSERT_EQUAL(ApiDb::expectedHootDbVersion().toStdString(), version.toStdString());
  }

  void runDropMapTest()
  {
    setUpTest("runDropMapTest");
    HootApiDb database;
    database.open(ServicesDbTestUtils::getDbModifyUrl(testName));

    const std::shared_ptr<QList<long>> ids = insertTestMap1(database);

    mapId = ids->at(0);

    CPPUNIT_ASSERT(database.mapExists(mapId));

    HOOT_STR_EQUALS(true, database.hasTable(HootApiDb::getCurrentNodesTableName(mapId)));
    HOOT_STR_EQUALS(true, database.hasTable(HootApiDb::getCurrentRelationMembersTableName(mapId)));
    HOOT_STR_EQUALS(true, database.hasTable(HootApiDb::getCurrentRelationsTableName(mapId)));
    HOOT_STR_EQUALS(true, database.hasTable(HootApiDb::getCurrentWayNodesTableName(mapId)));
    HOOT_STR_EQUALS(true, database.hasTable(HootApiDb::getCurrentWaysTableName(mapId)));

    database.deleteMap(mapId);

    HOOT_STR_EQUALS(false, database.hasTable(HootApiDb::getCurrentNodesTableName(mapId)));
    HOOT_STR_EQUALS(false, database.hasTable(HootApiDb::getCurrentRelationMembersTableName(mapId)));
    HOOT_STR_EQUALS(false, database.hasTable(HootApiDb::getCurrentRelationsTableName(mapId)));
    HOOT_STR_EQUALS(false, database.hasTable(HootApiDb::getCurrentWayNodesTableName(mapId)));
    HOOT_STR_EQUALS(false, database.hasTable(HootApiDb::getCurrentWaysTableName(mapId)));
  }

  void runInsertTest()
  {
    setUpTest("runInsertTest");
    HootApiDb db;
    db.open(ServicesDbTestUtils::getDbModifyUrl(testName));

    insertTestMap1(db);
  }

  void runMapExistsTest()
  {
    setUpTest("runMapExistsTest");
    HootApiDb database;
    database.open(ServicesDbTestUtils::getDbModifyUrl(testName));
    const std::shared_ptr<QList<long>> ids = insertTestMap1(database);

    mapId = ids->at(0);
    CPPUNIT_ASSERT(database.mapExists(mapId));
  }

  void runMapUrlWithNameTest()
  {
    setUpTest("runMapUrlWithNameTest");
    HootApiDb database;
    QUrl dbModifyUrl = ServicesDbTestUtils::getDbModifyUrl(testName);
    database.open(dbModifyUrl);
    const std::shared_ptr<QList<long>> ids = insertTestMap1(database);

    // Should recognize that the URL refers to a db layer by name and not by numerical ID.
    const long requestedMapId = database.getMapIdFromUrl(dbModifyUrl);
    CPPUNIT_ASSERT_EQUAL(ids->at(0), requestedMapId);
  }

  void runChangesetExistsTest()
  {
    setUpTest("runChangesetExistsTest");
    HootApiDb database;
    database.open(ServicesDbTestUtils::getDbModifyUrl(testName));
    const std::shared_ptr<QList<long>> ids = insertTestMap1(database);

    const long changesetId = ids->at(1);
    CPPUNIT_ASSERT(database.changesetExists(changesetId));
  }

  void runNumElementsTest()
  {
    setUpTest("runNumElementsTest");
    HootApiDb database;
    database.open(ServicesDbTestUtils::getDbModifyUrl(testName));

    const std::shared_ptr<QList<long>> ids = insertTestMap1(database);

    mapId = ids->at(0);
    CPPUNIT_ASSERT_EQUAL((long)1, database.numElements(ElementType::Node));
  }

  void runSelectNodeIdsForWayTest()
  {
    setUpTest("runSelectNodeIdsForWayTest");
    HootApiDb database;
    database.open(ServicesDbTestUtils::getDbModifyUrl(testName));

    const std::shared_ptr<QList<long>> ids = insertTestMap1(database);
    CPPUNIT_ASSERT_EQUAL(5, ids->size());
    mapId = ids->at(0);
    const long wayId = ids->at(3);

    HOOT_STR_EQUALS("[1]{1}", database.selectNodeIdsForWay(wayId));
  }

  void runSelectMembersForRelationTest()
  {
    setUpTest("runSelectMembersForRelationTest");
    HootApiDb database;
    database.open(ServicesDbTestUtils::getDbModifyUrl(testName));

    const std::shared_ptr<QList<long>> ids = insertTestMap1(database);
    CPPUNIT_ASSERT_EQUAL(5, ids->size());
    mapId = ids->at(0);
    const long relationId = ids->at(4);

    vector<RelationData::Entry> members = database.selectMembersForRelation(relationId);
    HOOT_STR_EQUALS("[2]{Entry: role: wayrole, eid: Way(1), Entry: role: noderole, eid: Node(1)}",
                    members);
  }

  void runSelectAllElementsTest()
  {
    setUpTest("runSelectAllElementsTest");
    HootApiDb database;
    database.open(ServicesDbTestUtils::getDbModifyUrl(testName));

    const std::shared_ptr<QList<long>> ids = insertTestMap1(database);
    CPPUNIT_ASSERT_EQUAL(5, ids->size());
    mapId = ids->at(0);
    const long nodeId = ids->at(2);
    const long wayId = ids->at(3);
    const long relationId = ids->at(4);

    std::shared_ptr<QSqlQuery> nodeResultIterator = database.selectElements(ElementType::Node);
    int ctr = 0;
    while (nodeResultIterator->next())
    {
      for (int j = 0; j < 10; j++) { LOG_TRACE("VALUE = " + nodeResultIterator->value(j).toString()); }

      HOOT_STR_EQUALS(nodeId, nodeResultIterator->value(0).toLongLong());
      HOOT_STR_EQUALS(38.0, nodeResultIterator->value(1).toDouble());
      HOOT_STR_EQUALS(-104.0, nodeResultIterator->value(2).toDouble());
      stringstream s;
      s << ApiDb::unescapeTags(nodeResultIterator->value(8));
      LOG_TRACE("unescapeTag :"+s.str());
      HOOT_STR_EQUALS("foo = bar\n", ApiDb::unescapeTags(nodeResultIterator->value(8)));

      ctr++;
    }
    CPPUNIT_ASSERT_EQUAL(1, ctr);

    std::shared_ptr<QSqlQuery> wayResultIterator = database.selectElements(ElementType::Way);
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

    std::shared_ptr<QSqlQuery> relationResultIterator =
      database.selectElements(ElementType::Relation);
    ctr = 0;
    while (relationResultIterator->next())
    {
      HOOT_STR_EQUALS(relationId, relationResultIterator->value(0).toLongLong());
      vector<RelationData::Entry> members = database.selectMembersForRelation(relationId);
      HOOT_STR_EQUALS("[2]{Entry: role: wayrole, eid: Way(1), Entry: role: noderole, eid: Node(1)}",
                      members);
      HOOT_STR_EQUALS("type = multistuff\n",
                      ApiDb::unescapeTags(relationResultIterator->value(5)));
      ctr++;
    }

    CPPUNIT_ASSERT_EQUAL(1, ctr);
  }

  void runSelectElementsTest()
  {
    setUpTest("runSelectElementsTest");
    HootApiDb database;
    database.open(ServicesDbTestUtils::getDbModifyUrl(testName));

    const std::shared_ptr<QList<long>> ids = insertTestMap2(database);
    CPPUNIT_ASSERT_EQUAL(8, ids->size());
    mapId = ids->at(0);
    const long nodeId1 = ids->at(2);
    const long nodeId2 = ids->at(3);

    std::shared_ptr<QSqlQuery> nodeResultIterator =
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
    setUpTest("runSelectElementsCustomTagsTest");
    HootApiDb database;
    database.open(ServicesDbTestUtils::getDbModifyUrl(testName));

    const std::shared_ptr<QList<long>> ids = insertTestMapWithCustomTags(database);
    CPPUNIT_ASSERT_EQUAL(4, ids->size());
    mapId = ids->at(0);
    const long nodeId1 = ids->at(1);
    const long nodeId2 = ids->at(2);
    const long nodeId3 = ids->at(3);

    std::shared_ptr<QSqlQuery> nodeResultIterator = database.selectElements(ElementType::Node);
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
          HOOT_STR_EQUALS(MetadataTags::HootStatus() + " = " + MetadataTags::Unknown1() + "\n" +
                          MetadataTags::Accuracy() + " = 20.0\n",
                          ApiDb::unescapeTags(nodeResultIterator->value(8)));
        }
        break;
      case 1:
        {
          HOOT_STR_EQUALS(nodeId2, nodeResultIterator->value(0).toLongLong());
          HOOT_STR_EQUALS(38.0, nodeResultIterator->value(ApiDb::NODES_LATITUDE).toDouble());
          HOOT_STR_EQUALS(-105.0, nodeResultIterator->value(ApiDb::NODES_LONGITUDE).toDouble());
          HOOT_STR_EQUALS(MetadataTags::ErrorCircular() + " = 20.0\n",
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
    setUpTest("runUpdateNodeTest");
    HootApiDb database;
    database.open(ServicesDbTestUtils::getDbModifyUrl(testName));

    const std::shared_ptr<QList<long>> ids = insertTestMap1(database);
    CPPUNIT_ASSERT_EQUAL(5, ids->size());
    mapId = ids->at(0);
    const long nodeId = ids->at(2);

    ServicesDbTestUtils::compareRecords(
          "SELECT latitude, longitude, visible, tile, version FROM " +
          HootApiDb::getCurrentNodesTableName(mapId) +
          " WHERE id=:id "
          "ORDER BY longitude",
          "38;-104;true;1329332431;1",
          NULL,
          (qlonglong)nodeId);


    database.setUserId(database.getOrCreateUser(userEmail(), userName()));
    database.beginChangeset();
    database.updateNode(nodeId, 3.1415, 2.71828, 1, Tags());

    ServicesDbTestUtils::compareRecords(
          "SELECT latitude, longitude, visible, tile, version FROM " +
          HootApiDb::getCurrentNodesTableName(mapId) +
          " WHERE id=:id "
          "ORDER BY longitude",
          "3.1415;2.71828;true;3222453693;1",
          NULL,
          (qlonglong)nodeId);
  }

  void runUnescapeTags()
  {
    //  Simple key/value pair
    HOOT_STR_EQUALS("key = value\n",
      HootApiDb::unescapeTags("\"key\"=>\"value\""));
    //  A couple of key/value pairs separated by a comma
    HOOT_STR_EQUALS("key1 = value1\nkey2 = value2\n",
      HootApiDb::unescapeTags("\"key1\"=>\"value1\", \"key2\"=>\"value2\""));
    //  More complicated case where the value contains double quotes
    HOOT_STR_EQUALS("fixme = check: building or just a \"paved\" place\n",
      HootApiDb::unescapeTags("\"fixme\"=>\"check: building or just a \"paved\" place\""));
    //  Complicated case from a user where the value contains the key/value separator '=>',
    //  this example isn't a Latin string but a UTF-8 string, change the codec temporarily
    QTextCodec* oldCodec = QTextCodec::codecForLocale();
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    HOOT_STR_EQUALS("type = route"
                    "\nto = München Hbf"
                    "\n" + MetadataTags::HootStatus() + " = 1"
                    "\nfrom = Berlin Ostbahnhof"
                    "\nservice = high_speed"
                    "\nroute = train"
                     "\nname = ICE 11: Berlin => Frankfurt => München"
                    "\npublic_transport:version = 2"
                    "\nref = ICE11"
                    "\nalt_name = ICE11 München"
                    "\nvia = Braunschweig Hbf;Frankfurt (Main) Hbf;Mannheim;Stuttgart"
                    "\nnetwork = DB"
                    "\noperator = DB Fernverkehr AG"
                    "\ninterval = 120"
                    "\n" + MetadataTags::ErrorCircular() + " = 15"
                    "\n",
                    HootApiDb::unescapeTags("\"to\"=>\"München Hbf\", \"ref\"=>\"ICE11\", \"via\"=>\"Braunschweig Hbf;Frankfurt (Main) Hbf;Mannheim;Stuttgart\", \"from\"=>\"Berlin Ostbahnhof\", \"name\"=>\"ICE 11: Berlin => Frankfurt => München\", \"type\"=>\"route\", \"route\"=>\"train\", \"network\"=>\"DB\", \"service\"=>\"high_speed\", \"alt_name\"=>\"ICE11 München\", \"interval\"=>\"120\", \"operator\"=>\"DB Fernverkehr AG\", \"" + MetadataTags::HootStatus() + "\"=>\"1\", \"" + MetadataTags::ErrorCircular() + "\"=>\"15\", \"public_transport:version\"=>\"2\""));

    QTextCodec::setCodecForLocale(oldCodec);
  }

  void runAvailableMapNamesTest()
  {
    QString test_name = "runAvailableMapNamesTest";
    setUpTest(test_name);
    const QString differentUserEmail = userEmail().replace(testName, testName + "-different-user");
    ServicesDbTestUtils::deleteUser(differentUserEmail);

    HootApiDb database;
    database.open(QUrl(HootApiDb::getBaseUrl().toString()));

    // Note that determination of a map being public is based of its folder public setting and not
    // the value of the public attribute in the maps table. Therefore, we'll put all of these test
    // maps into a folder.

    // insert a couple of private maps owned by this user; these should be returned by
    // selectMapNamesAvailableToCurrentUser

    const long currentUserId = database.getOrCreateUser(userEmail(), userName());
    LOG_VART(currentUserId);
    database.setUserId(currentUserId);
    QString currentUser = testName + "-current-user";
    const long currentUserPrivateFolderId =
      database.insertFolder(currentUser, nullptr, currentUserId, false);
    long testMapId = database.insertMap("runAvailableMapNamesTest-1");
    database.insertFolderMapMapping(testMapId, currentUserPrivateFolderId);
    testMapId = database.insertMap("runAvailableMapNamesTest-2");
    database.insertFolderMapMapping(testMapId, currentUserPrivateFolderId);

    // insert a couple of public maps owned by a different user; these should be returned by
    // selectMapNamesAvailableToCurrentUser
    QString differentUser = testName + "-different-user";
    const long differentUserId =
      database.getOrCreateUser(differentUserEmail, differentUser);
    LOG_VART(differentUserId);
    database.setUserId(differentUserId);
    QString differentUser1 = testName + "-different-user-1";
    const long differentUserPublicFolderId =
      database.insertFolder(differentUser1, nullptr, differentUserId, true);
    testMapId = database.insertMap("runAvailableMapNamesTest-3");
    database.insertFolderMapMapping(testMapId, differentUserPublicFolderId);
    testMapId = database.insertMap("runAvailableMapNamesTest-4");
    database.insertFolderMapMapping(testMapId, differentUserPublicFolderId);

    // insert a couple of private maps owned by a different user; these should *not* be returned by
    // selectMapNamesAvailableToCurrentUser
    QString differentUser2 = testName + "-different-user-2";
    const long differentUserPrivateFolderId =
      database.insertFolder(differentUser2, nullptr, differentUserId, false);
    testMapId = database.insertMap("runAvailableMapNamesTest-5");
    database.insertFolderMapMapping(testMapId, differentUserPrivateFolderId);
    testMapId = database.insertMap("runAvailableMapNamesTest-6");
    database.insertFolderMapMapping(testMapId, differentUserPrivateFolderId);

    // set the current user ID back to the original user
    database.setUserId(currentUserId);
    const QStringList mapNames = database.selectMapNamesAvailableToCurrentUser();
    LOG_VART(mapNames);

    database.close();
    ServicesDbTestUtils::deleteUser(differentUserEmail);

    CPPUNIT_ASSERT_EQUAL(4, mapNames.size());
    HOOT_STR_EQUALS("runAvailableMapNamesTest-1", mapNames[0]);
    HOOT_STR_EQUALS("runAvailableMapNamesTest-2", mapNames[1]);
    HOOT_STR_EQUALS("runAvailableMapNamesTest-3", mapNames[2]);
    HOOT_STR_EQUALS("runAvailableMapNamesTest-4", mapNames[3]);
  }

  const std::shared_ptr<QList<long>> insertTestMap1(HootApiDb& database)
  {
    database.transaction();

    QList<long> ids;
    database.setUserId(database.getOrCreateUser(userEmail(), userName()));

    mapId = database.insertMap(testName);
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

    return std::shared_ptr<QList<long>>(new QList<long>(ids));
  }

  const std::shared_ptr<QList<long>> insertTestMap2(HootApiDb& database)
  {
    database.transaction();

    QList<long> ids;
    database.setUserId(database.getOrCreateUser(userEmail(), userName()));
    mapId = database.insertMap(testName);
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

    return std::shared_ptr<QList<long>>(new QList<long>(ids));
  }

  const std::shared_ptr<QList<long>> insertTestMapWithCustomTags(HootApiDb& database)
  {
    QList<long> ids;
    database.transaction();
    database.setUserId(database.getOrCreateUser(userEmail(), userName()));
    mapId = database.insertMap(testName);
    ids.append(mapId);
    database.setMapId(mapId);
    database.beginChangeset();

    Tags t;
    t[MetadataTags::HootStatus()] = MetadataTags::Unknown1();
    t[MetadataTags::Accuracy()] = "20.0";
    long nodeId;
    database.insertNode(38, -104, t, nodeId);
    ids.append(nodeId);

    t.clear();
    t[MetadataTags::ErrorCircular()] = "20.0";
    database.insertNode(38, -105, t, nodeId);
    ids.append(nodeId);

    t.clear();
    database.insertNode(38, -106, t, nodeId);
    ids.append(nodeId);

    database.commit();

    return std::shared_ptr<QList<long>>(new QList<long>(ids));
  }

};

#ifdef HOOT_HAVE_SERVICES
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ServiceHootApiDbTest, "slow");
#endif  // HOOT_HAVE_SERVICES

}
