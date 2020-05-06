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
 * @copyright Copyright (C) 2013, 2014, 2015, 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/io/HootApiDb.h>
#include <hoot/core/io/HootApiDbWriter.h>
#include <hoot/core/io/ServicesDbTestUtils.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/util/DateTimeUtils.h>
#include <hoot/core/util/Settings.h>

// Standard
#include <functional>

// TGS
#include <tgs/BigContainers/BigMap.h>
#include <tgs/System/Time.h>

using namespace std;

namespace hoot
{

class ServiceHootApiDbWriterTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(ServiceHootApiDbWriterTest);
  CPPUNIT_TEST(runEscapeTest);
  CPPUNIT_TEST(runInsertTest);
  CPPUNIT_TEST(runRemapInsertTest);
  CPPUNIT_TEST(writeTwoMapsSameNameDifferentUsers);
  CPPUNIT_TEST(twoMapsSameNameSameUserOverwriteDisabledTest);
  CPPUNIT_TEST(twoMapsSameNameSameUserOverwriteEnabledTest);
  CPPUNIT_TEST(jobIdTest);
  CPPUNIT_TEST(preserveVersionOnInsertTest);
  CPPUNIT_TEST_SUITE_END();

public:

  QString userEmail()
  { return QString("%1.ServiceHootApiDbWriterTest@hoottestcpp.org").arg(_testName); }
  QString userName()  { return QString("%1.ServiceHootApiDbWriterTest").arg(_testName); }

  ServiceHootApiDbWriterTest()
  {
    setResetType(ResetBasic);
  }

  void setUpTest(const QString& testName)
  {
    _mapId = -1;
    _testName = testName;
    ServicesDbTestUtils::deleteUser(userEmail());
    HootApiDb database;

    database.open(ServicesDbTestUtils::getDbModifyUrl());
    database.getOrCreateUser(userEmail(), userName());
    database.close();
  }

  virtual void tearDown()
  {
    ServicesDbTestUtils::deleteUser(userEmail());

    if (_mapId != -1)
    {
      HootApiDb database;
      database.open(ServicesDbTestUtils::getDbModifyUrl());
      database.deleteMap(_mapId);
      database.close();
    }
  }

  void runEscapeTest()
  {
    setUpTest("runEscapeTest");
    // populate the database.
    HootApiDbWriter writer;
    writer.setUserEmail(userEmail());
    writer.setIncludeDebug(true);
    writer.setPreserveVersionOnInsert(false);
    writer.open(ServicesDbTestUtils::getDbModifyUrl(_testName).toString());

    OsmMapPtr map(new OsmMap());

    NodePtr n1(new Node(Status::Unknown1, -1, 0.0, 0.0, 10.0));
    n1->setTag("note", "n1',\n");
    NodePtr n2(new Node(Status::Unknown2, -2, 0.1, 0.0, 11.0));
    n2->setTag("note", "n2\"");
    NodePtr n3(new Node(Status::Conflated, -3, 0.2, 0.0, 12.0));
    n3->setTag("note", "n3\\");
    map->addNode(n1);
    map->addNode(n2);
    map->addNode(n3);

    writer.write(map);

    _mapId = writer.getMapId();

    ServicesDbTestUtils::compareRecords( "SELECT tags FROM " + HootApiDb::getCurrentNodesTableName(_mapId) +
                                         " ORDER BY longitude",
                                         "\"note\"=>\"n1',\", \"" + MetadataTags::HootId() + "\"=>\"-1\", \"" + MetadataTags::HootStatus() + "\"=>\"1\", \"" + MetadataTags::ErrorCircular() + "\"=>\"10\"\n"
                                         "\"note\"=>\"n2\\\\\\\"\", \"" + MetadataTags::HootId() + "\"=>\"-2\", \"" + MetadataTags::HootStatus() + "\"=>\"2\", \"" + MetadataTags::ErrorCircular() + "\"=>\"11\"\n"
                                         "\"note\"=>\"n3\\\\\\\\\", \"" + MetadataTags::HootId() + "\"=>\"-3\", \"" + MetadataTags::HootStatus() + "\"=>\"3\", \"" + MetadataTags::ErrorCircular() + "\"=>\"12\"",
                                         _testName,
                                         (qlonglong)_mapId);
  }

  void runInsertTest()
  {
    setUpTest("runInsertTest");
    // populate the database.
    HootApiDbWriter writer;
    writer.setRemap(false);
    writer.setUserEmail(userEmail());
    writer.setIncludeDebug(true);
    writer.setPreserveVersionOnInsert(false);
    writer.open(ServicesDbTestUtils::getDbModifyUrl(_testName).toString());

    OsmMapPtr map(new OsmMap());

    NodePtr n1(new Node(Status::Unknown1, 1, 0.0, 0.0, 10.0));
    n1->setTag("note", "n1");
    NodePtr n2(new Node(Status::Unknown2, 2, 0.1, 0.0, 11.0));
    n2->setTag("note", "n2");
    NodePtr n3(new Node(Status::Conflated, 3, 0.2, 0.0, 12.0));
    n3->setTag("note", "n3");
    map->addNode(n1);
    map->addNode(n2);
    map->addNode(n3);

    WayPtr w1(new Way(Status::Unknown1, 1, 13.0));
    w1->addNode(1);
    w1->addNode(2);
    w1->setTag("note", "w1");
    WayPtr w2(new Way(Status::Unknown2, 2, 14.0));
    w2->addNode(2);
    w2->addNode(3);
    w2->setTag("note", "w2");
    map->addWay(w1);
    map->addWay(w2);

    RelationPtr r1(new Relation(Status::Unknown1, 1, 15.0, MetadataTags::RelationCollection()));
    r1->addElement("n1", n1->getElementId());
    r1->addElement("w1", w1->getElementId());
    r1->setTag("note", "r1");
    map->addRelation(r1);

    writer.write(map);

    _mapId = writer.getMapId();

    ServicesDbTestUtils::compareRecords( "SELECT email, display_name FROM " + ApiDb::getUsersTableName() +
                                         " WHERE email LIKE :email",
                                         QString("%1;%2").arg(userEmail()).arg(userName()),
                                         _testName,
                                         userEmail());

    ServicesDbTestUtils::compareRecords( "SELECT latitude, longitude, visible, tile, version, tags FROM " +
                                         HootApiDb::getCurrentNodesTableName(_mapId) +
                                         " ORDER BY longitude",
                                         "0;0;true;3221225472;1;\"note\"=>\"n1\", \"" + MetadataTags::HootId() + "\"=>\"1\", \"" + MetadataTags::HootStatus() + "\"=>\"1\", \"" + MetadataTags::ErrorCircular() + "\"=>\"10\"\n"
                                         "0;0.1;true;3221225992;1;\"note\"=>\"n2\", \"" + MetadataTags::HootId() + "\"=>\"2\", \"" + MetadataTags::HootStatus() + "\"=>\"2\", \"" + MetadataTags::ErrorCircular() + "\"=>\"11\"\n"
                                         "0;0.2;true;3221227552;1;\"note\"=>\"n3\", \"" + MetadataTags::HootId() + "\"=>\"3\", \"" + MetadataTags::HootStatus() + "\"=>\"3\", \"" + MetadataTags::ErrorCircular() + "\"=>\"12\"",
                                         _testName,
                                         (qlonglong)_mapId);

    ServicesDbTestUtils::compareRecords( "SELECT id, visible, version, tags FROM " +
                                         HootApiDb::getCurrentWaysTableName(_mapId) +
                                         " ORDER BY id",
                                         "1;true;1;\"note\"=>\"w1\", \"" + MetadataTags::HootId() + "\"=>\"1\", \"" + MetadataTags::HootStatus() + "\"=>\"1\", \"" + MetadataTags::ErrorCircular() + "\"=>\"13\"\n"
                                         "2;true;1;\"note\"=>\"w2\", \"" + MetadataTags::HootId() + "\"=>\"2\", \"" + MetadataTags::HootStatus() + "\"=>\"2\", \"" + MetadataTags::ErrorCircular() + "\"=>\"14\"",
                                         _testName,
                                         (qlonglong)_mapId);

    ServicesDbTestUtils::compareRecords( "SELECT way_id, node_id, sequence_id FROM " +
                                         HootApiDb::getCurrentWayNodesTableName(_mapId) +
                                         " ORDER BY way_id, node_id, sequence_id",
                                         "1;1;0\n"
                                         "1;2;1\n"
                                         "2;2;0\n"
                                         "2;3;1",
                                         _testName,
                                         (qlonglong)_mapId);

    ServicesDbTestUtils::compareRecords( "SELECT id, visible, version, tags FROM " +
                                         HootApiDb::getCurrentRelationsTableName(_mapId),
                                         "1;true;1;\"note\"=>\"r1\", \"type\"=>\"collection\", \"" + MetadataTags::HootId() + "\"=>\"1\", \"" + MetadataTags::HootStatus() + "\"=>\"1\", \"" + MetadataTags::ErrorCircular() + "\"=>\"15\"",
                                         _testName,
                                         (qlonglong)_mapId);

    ServicesDbTestUtils::compareRecords( "SELECT relation_id, member_type, member_id, member_role, sequence_id "
                                         "FROM " +
                                         HootApiDb::getCurrentRelationMembersTableName(_mapId) +
                                         " ORDER BY relation_id, member_type",
                                         "1;node;1;n1;0\n"
                                         "1;way;1;w1;1",
                                         _testName,
                                         (qlonglong)_mapId);

    HootApiDb db;
    db.open(ServicesDbTestUtils::getDbModifyUrl(_testName));

    QStringList tableNames;
    tableNames.append(HootApiDb::getCurrentNodesTableName(_mapId));
    tableNames.append(HootApiDb::getCurrentWaysTableName(_mapId));
    tableNames.append(HootApiDb::getCurrentRelationsTableName(_mapId));

    for (int i = 0; i < tableNames.length(); i++)
    {
      QStringList results =
        db.execToString("SELECT timestamp FROM " + tableNames[i],
                         (qlonglong)_mapId).split("\n");
      for (int j = 0; j < results.length(); j++)
      {
        CPPUNIT_ASSERT(DateTimeUtils::fromTimeString(results[j]) != ElementData::TIMESTAMP_EMPTY);
      }
    }

    db.close();
  }

  void runRemapInsertTest()
  {
    setUpTest("runRemapInsertTest");
    // populate the database.
    HootApiDbWriter writer;
    writer.setUserEmail(userEmail());
    writer.setIncludeDebug(true);
    writer.setPreserveVersionOnInsert(false);
    writer.open(ServicesDbTestUtils::getDbModifyUrl(_testName).toString());

    OsmMapPtr map(new OsmMap());

    NodePtr n1(new Node(Status::Unknown1, -1, 0.0, 0.0, 10.0));
    n1->setTag("note", "n1");
    NodePtr n2(new Node(Status::Unknown2, -2, 0.1, 0.0, 11.0));
    n2->setTag("note", "n2");
    NodePtr n3(new Node(Status::Conflated, -3, 0.2, 0.0, 12.0));
    n3->setTag("note", "n3");
    map->addNode(n1);
    map->addNode(n2);
    map->addNode(n3);

    WayPtr w1(new Way(Status::Unknown1, -1, 13.0));
    w1->addNode(-1);
    w1->addNode(-2);
    w1->setTag("note", "w1");
    WayPtr w2(new Way(Status::Unknown2, -2, 14.0));
    w2->addNode(-2);
    w2->addNode(-3);
    w2->setTag("note", "w2");
    map->addWay(w1);
    map->addWay(w2);

    RelationPtr r1(new Relation(Status::Unknown1, -1, 15.0, MetadataTags::RelationCollection()));
    r1->addElement("n1", n1->getElementId());
    r1->addElement("w1", w1->getElementId());
    r1->setTag("note", "r1");
    map->addRelation(r1);

    RelationPtr r2(new Relation(Status::Unknown1, -2, 15.0, MetadataTags::RelationCollection()));
    r2->addElement("r1", r1->getElementId());
    r2->setTag("note", "r2");
    map->addRelation(r2);

    writer.write(map);
    _mapId = writer.getMapId();

    ServicesDbTestUtils::compareRecords( "SELECT email, display_name FROM " + ApiDb::getUsersTableName() +
                                         " WHERE email LIKE :email",
                                         QString("%1;%2").arg(userEmail()).arg(userName()),
                                         _testName,
                                         userEmail());

    ServicesDbTestUtils::compareRecords( "SELECT latitude, longitude, visible, tile, version, tags FROM " +
                                         HootApiDb::getCurrentNodesTableName(_mapId) +
                                         " ORDER BY longitude",
                                         "0;0;true;3221225472;1;\"note\"=>\"n1\", \"" + MetadataTags::HootId() + "\"=>\"-1\", \"" + MetadataTags::HootStatus() + "\"=>\"1\", \"" + MetadataTags::ErrorCircular() + "\"=>\"10\"\n"
                                         "0;0.1;true;3221225992;1;\"note\"=>\"n2\", \"" + MetadataTags::HootId() + "\"=>\"-2\", \"" + MetadataTags::HootStatus() + "\"=>\"2\", \"" + MetadataTags::ErrorCircular() + "\"=>\"11\"\n"
                                         "0;0.2;true;3221227552;1;\"note\"=>\"n3\", \"" + MetadataTags::HootId() + "\"=>\"-3\", \"" + MetadataTags::HootStatus() + "\"=>\"3\", \"" + MetadataTags::ErrorCircular() + "\"=>\"12\"",
                                         _testName,
                                         (qlonglong)_mapId);

    ServicesDbTestUtils::compareRecords( "SELECT visible, version, tags FROM " +
                                         HootApiDb::getCurrentWaysTableName(_mapId) +
                                         " ORDER BY id",
                                         "true;1;\"note\"=>\"w2\", \"" + MetadataTags::HootId() + "\"=>\"-2\", \"" + MetadataTags::HootStatus() + "\"=>\"2\", \"" + MetadataTags::ErrorCircular() + "\"=>\"14\"\n"
                                         "true;1;\"note\"=>\"w1\", \"" + MetadataTags::HootId() + "\"=>\"-1\", \"" + MetadataTags::HootStatus() + "\"=>\"1\", \"" + MetadataTags::ErrorCircular() + "\"=>\"13\"",
                                         _testName,
                                         (qlonglong)_mapId);

    ServicesDbTestUtils::compareRecords( "SELECT sequence_id FROM " +
                                         HootApiDb::getCurrentWayNodesTableName(_mapId) +
                                         " ORDER BY way_id, node_id, sequence_id",
                                         "1\n"
                                         "0\n"
                                         "1\n"
                                         "0",
                                         _testName,
                                         (qlonglong)_mapId);

    ServicesDbTestUtils::compareRecords( "SELECT visible, version, tags FROM " +
                                         HootApiDb::getCurrentRelationsTableName(_mapId) +
                                         " ORDER BY id",
                                         "true;1;\"note\"=>\"r2\", \"type\"=>\"collection\", \"" + MetadataTags::HootId() + "\"=>\"-2\", \"" + MetadataTags::HootStatus() + "\"=>\"1\", \"" + MetadataTags::ErrorCircular() + "\"=>\"15\"\n"
                                         "true;1;\"note\"=>\"r1\", \"type\"=>\"collection\", \"" + MetadataTags::HootId() + "\"=>\"-1\", \"" + MetadataTags::HootStatus() + "\"=>\"1\", \"" + MetadataTags::ErrorCircular() + "\"=>\"15\"",
                                         _testName,
                                         (qlonglong)_mapId);

    ServicesDbTestUtils::compareRecords( "SELECT member_type, member_role, sequence_id "
                                         "FROM " +
                                         HootApiDb::getCurrentRelationMembersTableName(_mapId) +
                                         " ORDER BY relation_id, member_type",
                                         "relation;r1;0\n"
                                         "node;n1;0\n"
                                         "way;w1;1",
                                         _testName,
                                         (qlonglong)_mapId);
  }

  void writeTwoMapsSameNameDifferentUsers()
  {
    setUpTest("ServiceHootApiDbWriterTest-writeTwoMapsSameNameDifferentUsers");

    // write a map
    LOG_DEBUG("Writing map...");
    HootApiDbWriter writer;
    writer.setRemap(false);
    writer.setUserEmail(userEmail());
    writer.setIncludeDebug(true);
    writer.setPreserveVersionOnInsert(false);
    writer.open(ServicesDbTestUtils::getDbModifyUrl(_testName).toString());
    OsmMapPtr map(new OsmMap());
    NodePtr n1(new Node(Status::Unknown1, 1, 0.0, 0.0, 10.0));
    n1->setTag("note", "n1");
    map->addNode(n1);
    writer.write(map);
    const long firstMapId = writer.getMapId();
    LOG_VARD(firstMapId);
    writer.close();

    // Create a different user
    LOG_DEBUG("Creating second user...");
    HootApiDb db;
    db.open(ServicesDbTestUtils::getDbModifyUrl(_testName).toString());
    const QString differentUserEmail =
      "ServiceHootApiDbWriterTest-writeTwoMapsSameNameDifferentUsers2";
    const long differentUserId = db.insertUser(differentUserEmail, differentUserEmail);
    LOG_VARD(differentUserId);
    db.close();

    // Configure the writer with the second user
    HootApiDbWriter writer2;
    writer2.setRemap(false);
    writer2.setIncludeDebug(true);
    writer2.setUserEmail(differentUserEmail);
    writer2.setPreserveVersionOnInsert(false);
    LOG_DEBUG("Attempting to open db for writing second map...");
    writer2.open(ServicesDbTestUtils::getDbModifyUrl(_testName).toString());
    // This should not fail, since we allow different users to write maps with the same name (just
    // checking that open succeeds here...not the actual write).
    writer2.close();

    LOG_DEBUG("Deleting second user...");
    db.open(ServicesDbTestUtils::getDbModifyUrl(_testName).toString());
    db.deleteUser(differentUserId);
  }

  void twoMapsSameNameSameUserOverwriteDisabledTest()
  {
    setUpTest("ServiceHootApiDbWriterTest-twoMapsSameNameSameUserOverwriteDisabledTest");

    // create a map
    OsmMapPtr map1(new OsmMap());
    NodePtr n1(new Node(Status::Unknown1, 1, 0.0, 0.0, 10.0));
    n1->setTag("note", "n1");
    map1->addNode(n1);

    // write the first map
    HootApiDbWriter writer;
    writer.setRemap(false);
    writer.setUserEmail(userEmail());
    writer.setIncludeDebug(true);
    writer.setPreserveVersionOnInsert(false);
    writer.open(ServicesDbTestUtils::getDbModifyUrl(_testName).toString());
    writer.write(map1);
    const long mapId = writer.getMapId();
    writer.close();

    //create a second map
    OsmMapPtr map2(new OsmMap());
    NodePtr n2(new Node(Status::Unknown1, 2, 0.0, 0.0, 10.0));
    n2->setTag("note", "n2");
    map2->addNode(n2);

    // try to write the second map to the first map with with overwrite disabled
    HootApiDbWriter writer2;
    writer2.setRemap(false);
    writer2.setIncludeDebug(true);
    writer2.setUserEmail(userEmail());
    writer2.setOverwriteMap(false);
    writer2.setPreserveVersionOnInsert(false);
    writer2.open(ServicesDbTestUtils::getDbModifyUrl(_testName).toString());
    writer2.write(map2);
    writer2.close();

    //the second map should get appended to the first
    ServicesDbTestUtils::compareRecords("SELECT latitude, longitude, visible, tile, version, tags FROM " +
                                        HootApiDb::getCurrentNodesTableName(mapId) +
                                        " ORDER BY longitude",
                                        "0;0;true;3221225472;1;\"note\"=>\"n1\", \"" + MetadataTags::HootId() + "\"=>\"1\", \"" + MetadataTags::HootStatus() + "\"=>\"1\", \"" + MetadataTags::ErrorCircular() + "\"=>\"10\"\n"
                                        "0;0;true;3221225472;1;\"note\"=>\"n2\", \"" + MetadataTags::HootId() + "\"=>\"2\", \"" + MetadataTags::HootStatus() + "\"=>\"1\", \"" + MetadataTags::ErrorCircular() + "\"=>\"10\"",
                                        _testName,
                                        (qlonglong)mapId);
  }

  void twoMapsSameNameSameUserOverwriteEnabledTest()
  {
    setUpTest("ServiceHootApiDbWriterTest-twoMapsSameNameSameUserOverwriteEnabledTest");

    // create a map
    OsmMapPtr map(new OsmMap());
    NodePtr n1(new Node(Status::Unknown1, 1, 0.0, 0.0, 10.0));
    n1->setTag("note", "n1");
    map->addNode(n1);

    // write a map
    HootApiDbWriter writer;
    writer.setRemap(false);
    writer.setUserEmail(userEmail());
    writer.setIncludeDebug(true);
    writer.setPreserveVersionOnInsert(false);
    writer.open(ServicesDbTestUtils::getDbModifyUrl(_testName).toString());
    writer.write(map);
    const long firstMapId = writer.getMapId();
    writer.close();

    //create a second map
    OsmMapPtr map2(new OsmMap());
    NodePtr n2(new Node(Status::Unknown1, 2, 0.0, 0.0, 10.0));
    n2->setTag("note", "n2");
    map2->addNode(n2);

    // try to write another map with the same name for the same user with overwrite enabled
    HootApiDbWriter writer2;
    writer2.setOverwriteMap(true);
    writer2.setRemap(false);
    writer2.setIncludeDebug(true);
    writer2.setUserEmail(userEmail());
    writer2.setPreserveVersionOnInsert(false);
    writer2.open(ServicesDbTestUtils::getDbModifyUrl(_testName).toString());
    writer2.write(map2);
    const long secondMapId = writer2.getMapId();
    writer2.close();

    // the second map should replace the first
    ServicesDbTestUtils::compareRecords("SELECT latitude, longitude, visible, tile, version, tags FROM " +
                                        HootApiDb::getCurrentNodesTableName(secondMapId) +
                                        " ORDER BY longitude",
                                        "0;0;true;3221225472;1;\"note\"=>\"n2\", \"" + MetadataTags::HootId() + "\"=>\"2\", \"" + MetadataTags::HootStatus() + "\"=>\"1\", \"" + MetadataTags::ErrorCircular() + "\"=>\"10\"",
                                        _testName,
                                        (qlonglong)secondMapId);

    HootApiDb db;
    db.open(ServicesDbTestUtils::getDbModifyUrl(_testName).toString());
    const bool firstMapExists = db.mapExists(firstMapId);
    db.close();
    CPPUNIT_ASSERT(!firstMapExists);
  }

  void jobIdTest()
  {
    setUpTest("ServiceHootApiDbWriterTest-jobIdTest");

    // create a services job
    HootApiDb db;
    db.open(ServicesDbTestUtils::getDbModifyUrl(_testName).toString());
    const QString jobId = db.insertJob(_testName);
    LOG_VARD(jobId);
    db.close();

    // create a map
    OsmMapPtr map(new OsmMap());
    NodePtr n1(new Node(Status::Unknown1, 1, 0.0, 0.0, 10.0));
    n1->setTag("note", "n1");
    map->addNode(n1);

    // write the map and pass the job id into the writer
    HootApiDbWriter writer;
    writer.setJobId(jobId);
    writer.setRemap(false);
    writer.setUserEmail(userEmail());
    writer.setIncludeDebug(true);
    writer.setPreserveVersionOnInsert(false);
    writer.open(ServicesDbTestUtils::getDbModifyUrl(_testName).toString());
    writer.write(map);
    const long firstMapId = writer.getMapId();
    LOG_VARD(firstMapId);
    writer.close();

    // write another copy of the map w/o setting the job id on the writer
    HootApiDbWriter writer2;
    writer2.setJobId("");
    writer2.setRemap(false);
    writer2.setUserEmail(userEmail());
    writer2.setIncludeDebug(true);
    writer2.setPreserveVersionOnInsert(false);
    writer2.open(ServicesDbTestUtils::getDbModifyUrl(_testName + "2").toString());
    writer2.write(map);
    const long secondMapId = writer2.getMapId();
    LOG_VARD(secondMapId);
    writer2.close();

    // the first map's id should be written to the job status table resource_id col for the input
    // job id and the resource id shouldn't have changed when the second map was written
    db.open(ServicesDbTestUtils::getDbModifyUrl(_testName).toString());
    long resourceId = db.getJobStatusResourceId(jobId);
    LOG_VARD(resourceId);
    CPPUNIT_ASSERT(firstMapId != secondMapId);
    CPPUNIT_ASSERT_EQUAL(firstMapId, resourceId);
    db.close();

    db.open(ServicesDbTestUtils::getDbModifyUrl(_testName).toString());
    db.deleteMap(firstMapId);
    db.deleteMap(secondMapId);
    db._deleteJob(jobId);
    db.close();
  }

  void preserveVersionOnInsertTest()
  {
    setUpTest("preserveVersionOnInsertTest");
    // populate the database.
    HootApiDbWriter writer;
    writer.setRemap(false);
    writer.setUserEmail(userEmail());
    writer.setIncludeDebug(true);
    writer.setPreserveVersionOnInsert(true);
    writer.open(ServicesDbTestUtils::getDbModifyUrl(_testName).toString());

    OsmMapPtr map(new OsmMap());

    NodePtr n1(new Node(Status::Unknown1, 1, 0.0, 0.0, 10.0));
    n1->setTag("note", "n1");
    NodePtr n2(new Node(Status::Unknown2, 2, 0.1, 0.0, 11.0));
    n2->setTag("note", "n2");
    n2->setVersion(2);
    NodePtr n3(new Node(Status::Conflated, 3, 0.2, 0.0, 12.0));
    n3->setTag("note", "n3");
    n3->setVersion(1);
    map->addNode(n1);
    map->addNode(n2);
    map->addNode(n3);

    WayPtr w1(new Way(Status::Unknown1, 1, 13.0));
    w1->addNode(1);
    w1->addNode(2);
    w1->setTag("note", "w1");
    w1->setVersion(3);
    WayPtr w2(new Way(Status::Unknown2, 2, 14.0));
    w2->addNode(2);
    w2->addNode(3);
    w2->setTag("note", "w2");
    map->addWay(w1);
    map->addWay(w2);

    RelationPtr r1(new Relation(Status::Unknown1, 1, 15.0, MetadataTags::RelationCollection()));
    r1->addElement("n1", n1->getElementId());
    r1->addElement("w1", w1->getElementId());
    r1->setTag("note", "r1");
    r1->setVersion(2);
    map->addRelation(r1);

    writer.write(map);

    _mapId = writer.getMapId();
    writer.close();

    ServicesDbTestUtils::compareRecords("SELECT latitude, longitude, visible, tile, version, tags FROM " +
                                        HootApiDb::getCurrentNodesTableName(_mapId) +
                                        " ORDER BY longitude",
                                        "0;0;true;3221225472;1;\"note\"=>\"n1\", \"" +
                                          MetadataTags::HootId() + "\"=>\"1\", \"" +
                                          MetadataTags::HootStatus() + "\"=>\"1\", \"" +
                                          MetadataTags::ErrorCircular() + "\"=>\"10\"\n"
                                        "0;0.1;true;3221225992;2;\"note\"=>\"n2\", \"" +
                                          MetadataTags::HootId() + "\"=>\"2\", \"" +
                                          MetadataTags::HootStatus() + "\"=>\"2\", \"" +
                                          MetadataTags::ErrorCircular() + "\"=>\"11\"\n"
                                        "0;0.2;true;3221227552;1;\"note\"=>\"n3\", \"" +
                                          MetadataTags::HootId() + "\"=>\"3\", \"" +
                                          MetadataTags::HootStatus() + "\"=>\"3\", \"" +
                                          MetadataTags::ErrorCircular() + "\"=>\"12\"",
                                        _testName,
                                        (qlonglong)_mapId);

    ServicesDbTestUtils::compareRecords("SELECT id, visible, version, tags FROM " +
                                        HootApiDb::getCurrentWaysTableName(_mapId) +
                                        " ORDER BY id",
                                        "1;true;3;\"note\"=>\"w1\", \"" + MetadataTags::HootId() +
                                          "\"=>\"1\", \"" + MetadataTags::HootStatus() +
                                          "\"=>\"1\", \"" + MetadataTags::ErrorCircular() +
                                          "\"=>\"13\"\n"
                                        "2;true;1;\"note\"=>\"w2\", \"" + MetadataTags::HootId() +
                                          "\"=>\"2\", \"" + MetadataTags::HootStatus() +
                                          "\"=>\"2\", \"" + MetadataTags::ErrorCircular() +
                                          "\"=>\"14\"",
                                        _testName,
                                        (qlonglong)_mapId);

    ServicesDbTestUtils::compareRecords("SELECT id, visible, version, tags FROM " +
                                        HootApiDb::getCurrentRelationsTableName(_mapId),
                                        "1;true;2;\"note\"=>\"r1\", \"type\"=>\"collection\", \"" +
                                          MetadataTags::HootId() + "\"=>\"1\", \"" +
                                          MetadataTags::HootStatus() + "\"=>\"1\", \"" +
                                          MetadataTags::ErrorCircular() + "\"=>\"15\"",
                                        _testName,
                                        (qlonglong)_mapId);
  }

private:

  long _mapId;
  QString _testName;
};

#ifdef HOOT_HAVE_SERVICES
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ServiceHootApiDbWriterTest, "slow");
#endif  // HOOT_HAVE_SERVICES

}
