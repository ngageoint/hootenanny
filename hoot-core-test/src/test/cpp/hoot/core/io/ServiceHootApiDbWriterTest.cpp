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
 * @copyright Copyright (C) 2013, 2014, 2015, 2016, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
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
#include <hoot/core/util/OsmUtils.h>
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
  CPPUNIT_TEST(overwriteDataWithDifferentUserTest);
  CPPUNIT_TEST(deleteDataWithDifferentUserTest);
  CPPUNIT_TEST_SUITE_END();

public:

  QString userEmail()
  { return QString("%1.ServiceHootApiDbWriterTest@hoottestcpp.org").arg(_testName); }
  QString userName()  { return QString("%1.ServiceHootApiDbWriterTest").arg(_testName); }

  ServiceHootApiDbWriterTest()
  {
    setResetType(ResetBasic);
    TestUtils::mkpath("test-output/io/ServiceHootApiDbWriterTest");
  }

  void setUpTest(const QString testName)
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

    compareRecords("SELECT tags FROM " + HootApiDb::getCurrentNodesTableName(_mapId) +
                   " ORDER BY longitude",
                   "\"note\"=>\"n1',\", \"" + MetadataTags::HootId() + "\"=>\"-1\", \"" + MetadataTags::HootStatus() + "\"=>\"1\", \"" + MetadataTags::ErrorCircular() + "\"=>\"10\"\n"
                   "\"note\"=>\"n2\\\\\\\"\", \"" + MetadataTags::HootId() + "\"=>\"-2\", \"" + MetadataTags::HootStatus() + "\"=>\"2\", \"" + MetadataTags::ErrorCircular() + "\"=>\"11\"\n"
                   "\"note\"=>\"n3\\\\\\\\\", \"" + MetadataTags::HootId() + "\"=>\"-3\", \"" + MetadataTags::HootStatus() + "\"=>\"3\", \"" + MetadataTags::ErrorCircular() + "\"=>\"12\"",
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

    compareRecords("SELECT email, display_name FROM " + ApiDb::getUsersTableName() +
                   " WHERE email LIKE :email",
                   QString("%1;%2").arg(userEmail()).arg(userName()),
                   userEmail());

    compareRecords("SELECT latitude, longitude, visible, tile, version, tags FROM " +
                   HootApiDb::getCurrentNodesTableName(_mapId) +
                   " ORDER BY longitude",
                   "0;0;true;3221225472;1;\"note\"=>\"n1\", \"" + MetadataTags::HootId() + "\"=>\"1\", \"" + MetadataTags::HootStatus() + "\"=>\"1\", \"" + MetadataTags::ErrorCircular() + "\"=>\"10\"\n"
                   "0;0.1;true;3221225992;1;\"note\"=>\"n2\", \"" + MetadataTags::HootId() + "\"=>\"2\", \"" + MetadataTags::HootStatus() + "\"=>\"2\", \"" + MetadataTags::ErrorCircular() + "\"=>\"11\"\n"
                   "0;0.2;true;3221227552;1;\"note\"=>\"n3\", \"" + MetadataTags::HootId() + "\"=>\"3\", \"" + MetadataTags::HootStatus() + "\"=>\"3\", \"" + MetadataTags::ErrorCircular() + "\"=>\"12\"",
                   (qlonglong)_mapId);

    compareRecords("SELECT id, visible, version, tags FROM " +
                   HootApiDb::getCurrentWaysTableName(_mapId) +
                   " ORDER BY id",
                   "1;true;1;\"note\"=>\"w1\", \"" + MetadataTags::HootId() + "\"=>\"1\", \"" + MetadataTags::HootStatus() + "\"=>\"1\", \"" + MetadataTags::ErrorCircular() + "\"=>\"13\"\n"
                   "2;true;1;\"note\"=>\"w2\", \"" + MetadataTags::HootId() + "\"=>\"2\", \"" + MetadataTags::HootStatus() + "\"=>\"2\", \"" + MetadataTags::ErrorCircular() + "\"=>\"14\"",
                   (qlonglong)_mapId);

    compareRecords("SELECT way_id, node_id, sequence_id FROM " +
                   HootApiDb::getCurrentWayNodesTableName(_mapId) +
                   " ORDER BY way_id, node_id, sequence_id",
                   "1;1;0\n"
                   "1;2;1\n"
                   "2;2;0\n"
                   "2;3;1",
                   (qlonglong)_mapId);

    compareRecords("SELECT id, visible, version, tags FROM " +
                   HootApiDb::getCurrentRelationsTableName(_mapId),
                   "1;true;1;\"note\"=>\"r1\", \"type\"=>\"collection\", \"" + MetadataTags::HootId() + "\"=>\"1\", \"" + MetadataTags::HootStatus() + "\"=>\"1\", \"" + MetadataTags::ErrorCircular() + "\"=>\"15\"",
                   (qlonglong)_mapId);

    compareRecords("SELECT relation_id, member_type, member_id, member_role, sequence_id "
                   "FROM " +
                   HootApiDb::getCurrentRelationMembersTableName(_mapId) +
                   " ORDER BY relation_id, member_type",
                   "1;node;1;n1;0\n"
                   "1;way;1;w1;1",
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
        CPPUNIT_ASSERT(OsmUtils::fromTimeString(results[j]) != ElementData::TIMESTAMP_EMPTY);
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

    compareRecords("SELECT email, display_name FROM " + ApiDb::getUsersTableName() +
                   " WHERE email LIKE :email",
                   QString("%1;%2").arg(userEmail()).arg(userName()),
                   userEmail());

    compareRecords("SELECT latitude, longitude, visible, tile, version, tags FROM " +
                   HootApiDb::getCurrentNodesTableName(_mapId) +
                   " ORDER BY longitude",
                   "0;0;true;3221225472;1;\"note\"=>\"n1\", \"" + MetadataTags::HootId() + "\"=>\"-1\", \"" + MetadataTags::HootStatus() + "\"=>\"1\", \"" + MetadataTags::ErrorCircular() + "\"=>\"10\"\n"
                   "0;0.1;true;3221225992;1;\"note\"=>\"n2\", \"" + MetadataTags::HootId() + "\"=>\"-2\", \"" + MetadataTags::HootStatus() + "\"=>\"2\", \"" + MetadataTags::ErrorCircular() + "\"=>\"11\"\n"
                   "0;0.2;true;3221227552;1;\"note\"=>\"n3\", \"" + MetadataTags::HootId() + "\"=>\"-3\", \"" + MetadataTags::HootStatus() + "\"=>\"3\", \"" + MetadataTags::ErrorCircular() + "\"=>\"12\"",
                   (qlonglong)_mapId);

    compareRecords("SELECT visible, version, tags FROM " +
                   HootApiDb::getCurrentWaysTableName(_mapId) +
                   " ORDER BY id",
                   "true;1;\"note\"=>\"w2\", \"" + MetadataTags::HootId() + "\"=>\"-2\", \"" + MetadataTags::HootStatus() + "\"=>\"2\", \"" + MetadataTags::ErrorCircular() + "\"=>\"14\"\n"
                   "true;1;\"note\"=>\"w1\", \"" + MetadataTags::HootId() + "\"=>\"-1\", \"" + MetadataTags::HootStatus() + "\"=>\"1\", \"" + MetadataTags::ErrorCircular() + "\"=>\"13\"",
                   (qlonglong)_mapId);

    compareRecords("SELECT sequence_id FROM " +
                   HootApiDb::getCurrentWayNodesTableName(_mapId) +
                   " ORDER BY way_id, node_id, sequence_id",
                   "1\n"
                   "0\n"
                   "1\n"
                   "0",
                   (qlonglong)_mapId);

    compareRecords("SELECT visible, version, tags FROM " +
                   HootApiDb::getCurrentRelationsTableName(_mapId) +
                   " ORDER BY id",
                   "true;1;\"note\"=>\"r2\", \"type\"=>\"collection\", \"" + MetadataTags::HootId() + "\"=>\"-2\", \"" + MetadataTags::HootStatus() + "\"=>\"1\", \"" + MetadataTags::ErrorCircular() + "\"=>\"15\"\n"
                   "true;1;\"note\"=>\"r1\", \"type\"=>\"collection\", \"" + MetadataTags::HootId() + "\"=>\"-1\", \"" + MetadataTags::HootStatus() + "\"=>\"1\", \"" + MetadataTags::ErrorCircular() + "\"=>\"15\"",
                   (qlonglong)_mapId);

    compareRecords("SELECT member_type, member_role, sequence_id "
                   "FROM " +
                   HootApiDb::getCurrentRelationMembersTableName(_mapId) +
                   " ORDER BY relation_id, member_type",
                   "relation;r1;0\n"
                   "node;n1;0\n"
                   "way;w1;1",
                   (qlonglong)_mapId);
  }

  struct CompareLess
  {
    bool operator() (const long& a, const long& b) const
    {
      return a < b;
    }

    static long max_value() { return std::numeric_limits<long>::max(); }
  };

  void compareRecords(QString sql, QString expected, QVariant v1 = QVariant())
  {
    HootApiDb db;
    db.open(ServicesDbTestUtils::getDbModifyUrl(_testName));
    QString result = db.execToString(sql, v1);
    if (expected == "")
    {
      QStringList rows = result.split("\n");
      for (int i = 0; i < rows.size(); ++i)
      {
        cout << "\"" << rows[i];
        if (i == rows.size() - 1)
        {
          cout << "\"" << endl;
        }
        else
        {
          cout << "\\n\"" << endl;
        }
      }
    }
    else
    {
      if (expected != result)
      {
        LOG_INFO(TestUtils::toQuotedString(result));
      }
      CPPUNIT_ASSERT_EQUAL(expected.toStdString(), result.toStdString());
    }
  }

  void overwriteDataWithDifferentUserTest()
  {
    setUpTest("overwriteDataWithDifferentUserTest");

    // write a map
    HootApiDbWriter writer;
    writer.setRemap(false);
    writer.setUserEmail(userEmail());
    writer.setIncludeDebug(true);
    writer.open(ServicesDbTestUtils::getDbModifyUrl(_testName).toString());
    OsmMapPtr map(new OsmMap());
    NodePtr n1(new Node(Status::Unknown1, 1, 0.0, 0.0, 10.0));
    n1->setTag("note", "n1");
    map->addNode(n1);
    writer.write(map);
    writer.close();

    // Create a different user
    HootApiDb db;
    db.open(ServicesDbTestUtils::getDbModifyUrl(_testName).toString());
    const QString differentUserEmail = "overwriteDataWithDifferentUserTest2";
    db.insertUser(differentUserEmail, differentUserEmail);
    db.close();

    // Configure the writer with the second user
    HootApiDbWriter writer2;
    writer2.setRemap(false);
    writer2.setIncludeDebug(true);
    writer2.setUserEmail(differentUserEmail);

    // The second user shouldn't be able to overwrite the first user's data.
    QString exceptionMsg("");
    try
    {
      writer2.open(ServicesDbTestUtils::getDbModifyUrl(_testName).toString());
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
      writer2.close();
    }
    LOG_VARD(exceptionMsg);
    CPPUNIT_ASSERT(exceptionMsg.contains("does not have write access to map"));

    // Clean up the second user
    db.open(ServicesDbTestUtils::getDbModifyUrl(_testName).toString());
    db.deleteUser(db.getUserId(differentUserEmail, true));
    db.close();
  }

  void deleteDataWithDifferentUserTest()
  {
    setUpTest("deleteDataWithDifferentUserTest");

    // write a map

    HootApiDbWriter writer;
    writer.setRemap(false);
    writer.setUserEmail(userEmail());
    writer.setIncludeDebug(true);
    writer.open(ServicesDbTestUtils::getDbModifyUrl(_testName).toString());

    OsmMapPtr map(new OsmMap());
    NodePtr n1(new Node(Status::Unknown1, 1, 0.0, 0.0, 10.0));
    n1->setTag("note", "n1");
    map->addNode(n1);

    writer.write(map);
    writer.close();

    // Create a different user
    HootApiDb db;
    db.open(ServicesDbTestUtils::getDbModifyUrl(_testName).toString());
    const QString differentUserEmail = "deleteDataWithDifferentUserTest2";
    db.insertUser(differentUserEmail, differentUserEmail);

    // Configure the writer with the second user
    writer.setUserEmail(differentUserEmail);

    // The second user shouldn't be able to delete the first user's data.
    QString exceptionMsg("");
    try
    {
      writer.deleteMap(ServicesDbTestUtils::getDbModifyUrl(_testName).toString());
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    LOG_VARD(exceptionMsg);
    CPPUNIT_ASSERT(exceptionMsg.contains("does not have write access to map"));

    // Clean up the second user
    db.deleteUser(db.getUserId(differentUserEmail, true));
    db.close();
  }

private:

  long _mapId;
  QString _testName;
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ServiceHootApiDbWriterTest, "slow");

}
