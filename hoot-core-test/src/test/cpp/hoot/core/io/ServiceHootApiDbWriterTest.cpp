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
#include <hoot/core/io/HootApiDbWriter.h>
#include <hoot/core/util/MetadataTags.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/util/OsmUtils.h>

// Standard
#include <functional>

// TGS
#include <tgs/BigContainers/BigMap.h>
#include <tgs/System/Time.h>

#include "../TestUtils.h"
#include "ServicesDbTestUtils.h"

namespace hoot
{

class ServiceHootApiDbWriterTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(ServiceHootApiDbWriterTest);
  CPPUNIT_TEST(runEscapeTest);
  CPPUNIT_TEST(runInsertTest);
  CPPUNIT_TEST(runRemapInsertTest);
  CPPUNIT_TEST_SUITE_END();

public:

  static QString userEmail() { return "ServiceHootApiDbWriterTest@hoottestcpp.org"; }

  void compareRecords(QString sql, QString expected, QVariant v1 = QVariant())
  {
    HootApiDb db;
    db.open(ServicesDbTestUtils::getDbModifyUrl());
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

  void runEscapeTest()
  {
    // populate the database.
    HootApiDbWriter writer;
    writer.setUserEmail(userEmail());
    writer.setIncludeDebug(true);
    writer.open(ServicesDbTestUtils::getDbModifyUrl().toString());

    boost::shared_ptr<OsmMap> map(new OsmMap());

    boost::shared_ptr<Node> n1(new Node(Status::Unknown1, -1, 0.0, 0.0, 10.0));
    n1->setTag("note", "n1',\n");
    boost::shared_ptr<Node> n2(new Node(Status::Unknown2, -2, 0.1, 0.0, 11.0));
    n2->setTag("note", "n2\"");
    boost::shared_ptr<Node> n3(new Node(Status::Conflated, -3, 0.2, 0.0, 12.0));
    n3->setTag("note", "n3\\");
    map->addNode(n1);
    map->addNode(n2);
    map->addNode(n3);

    writer.write(map);

    long mapId = writer.getMapId();

    compareRecords("SELECT tags FROM " + HootApiDb::getCurrentNodesTableName(mapId) +
                   " ORDER BY longitude",
                   "\"note\"=>\"n1',\n\", \"" + MetadataTags::HootId() + "\"=>\"-1\", \"" + MetadataTags::HootStatus() + "\"=>\"1\", \"" + MetadataTags::ErrorCircular() + "\"=>\"10\"\n"
                   "\"note\"=>\"n2\\\\\\\"\", \"" + MetadataTags::HootId() + "\"=>\"-2\", \"" + MetadataTags::HootStatus() + "\"=>\"2\", \"" + MetadataTags::ErrorCircular() + "\"=>\"11\"\n"
                   "\"note\"=>\"n3\\\\\\\\\", \"" + MetadataTags::HootId() + "\"=>\"-3\", \"" + MetadataTags::HootStatus() + "\"=>\"3\", \"" + MetadataTags::ErrorCircular() + "\"=>\"12\"",
                   (qlonglong)mapId);
  }

  void runInsertTest()
  {
    // populate the database.
    HootApiDbWriter writer;
    writer.setRemap(false);
    writer.setUserEmail(userEmail());
    writer.setIncludeDebug(true);
    writer.open(ServicesDbTestUtils::getDbModifyUrl().toString());

    boost::shared_ptr<OsmMap> map(new OsmMap());

    boost::shared_ptr<Node> n1(new Node(Status::Unknown1, 1, 0.0, 0.0, 10.0));
    n1->setTag("note", "n1");
    boost::shared_ptr<Node> n2(new Node(Status::Unknown2, 2, 0.1, 0.0, 11.0));
    n2->setTag("note", "n2");
    boost::shared_ptr<Node> n3(new Node(Status::Conflated, 3, 0.2, 0.0, 12.0));
    n3->setTag("note", "n3");
    map->addNode(n1);
    map->addNode(n2);
    map->addNode(n3);

    boost::shared_ptr<Way> w1(new Way(Status::Unknown1, 1, 13.0));
    w1->addNode(1);
    w1->addNode(2);
    w1->setTag("note", "w1");
    boost::shared_ptr<Way> w2(new Way(Status::Unknown2, 2, 14.0));
    w2->addNode(2);
    w2->addNode(3);
    w2->setTag("note", "w2");
    map->addWay(w1);
    map->addWay(w2);

    boost::shared_ptr<Relation> r1(new Relation(Status::Unknown1, 1, 15.0, "collection"));
    r1->addElement("n1", n1->getElementId());
    r1->addElement("w1", w1->getElementId());
    r1->setTag("note", "r1");
    map->addRelation(r1);

    writer.write(map);

    long mapId = writer.getMapId();

    compareRecords("SELECT email, display_name FROM " + ApiDb::getUsersTableName() +
                   " WHERE email LIKE :email",
                   "ServiceHootApiDbWriterTest@hoottestcpp.org;ServiceHootApiDbWriterTest",
                   userEmail());

    compareRecords("SELECT latitude, longitude, visible, tile, version, tags FROM " +
                   HootApiDb::getCurrentNodesTableName(mapId) +
                   " ORDER BY longitude",
                   "0;0;true;3221225472;1;\"note\"=>\"n1\", \"" + MetadataTags::HootId() + "\"=>\"1\", \"" + MetadataTags::HootStatus() + "\"=>\"1\", \"" + MetadataTags::ErrorCircular() + "\"=>\"10\"\n"
                   "0;0.1;true;3221225992;1;\"note\"=>\"n2\", \"" + MetadataTags::HootId() + "\"=>\"2\", \"" + MetadataTags::HootStatus() + "\"=>\"2\", \"" + MetadataTags::ErrorCircular() + "\"=>\"11\"\n"
                   "0;0.2;true;3221227552;1;\"note\"=>\"n3\", \"" + MetadataTags::HootId() + "\"=>\"3\", \"" + MetadataTags::HootStatus() + "\"=>\"3\", \"" + MetadataTags::ErrorCircular() + "\"=>\"12\"",
                   (qlonglong)mapId);

    compareRecords("SELECT id, visible, version, tags FROM " +
                   HootApiDb::getCurrentWaysTableName(mapId) +
                   " ORDER BY id",
                   "1;true;1;\"note\"=>\"w1\", \"" + MetadataTags::HootId() + "\"=>\"1\", \"" + MetadataTags::HootStatus() + "\"=>\"1\", \"" + MetadataTags::ErrorCircular() + "\"=>\"13\"\n"
                   "2;true;1;\"note\"=>\"w2\", \"" + MetadataTags::HootId() + "\"=>\"2\", \"" + MetadataTags::HootStatus() + "\"=>\"2\", \"" + MetadataTags::ErrorCircular() + "\"=>\"14\"",
                   (qlonglong)mapId);

    compareRecords("SELECT way_id, node_id, sequence_id FROM " +
                   HootApiDb::getCurrentWayNodesTableName(mapId) +
                   " ORDER BY way_id, node_id, sequence_id",
                   "1;1;0\n"
                   "1;2;1\n"
                   "2;2;0\n"
                   "2;3;1",
                   (qlonglong)mapId);

    compareRecords("SELECT id, visible, version, tags FROM " +
                   HootApiDb::getCurrentRelationsTableName(mapId),
                   "1;true;1;\"note\"=>\"r1\", \"type\"=>\"collection\", \"" + MetadataTags::HootId() + "\"=>\"1\", \"" + MetadataTags::HootStatus() + "\"=>\"1\", \"" + MetadataTags::ErrorCircular() + "\"=>\"15\"",
                   (qlonglong)mapId);

    compareRecords("SELECT relation_id, member_type, member_id, member_role, sequence_id "
                   "FROM " +
                   HootApiDb::getCurrentRelationMembersTableName(mapId) +
                   " ORDER BY relation_id, member_type",
                   "1;node;1;n1;0\n"
                   "1;way;1;w1;1",
                   (qlonglong)mapId);

    HootApiDb db;
    db.open(ServicesDbTestUtils::getDbModifyUrl());

    QStringList tableNames;
    tableNames.append(HootApiDb::getCurrentNodesTableName(mapId));
    tableNames.append(HootApiDb::getCurrentWaysTableName(mapId));
    tableNames.append(HootApiDb::getCurrentRelationsTableName(mapId));

    for (int i = 0; i < tableNames.length(); i++)
    {
      QStringList results =
        db.execToString("SELECT timestamp FROM " + tableNames[i],
                         (qlonglong)mapId).split("\n");
      for (int j = 0; j < results.length(); j++)
      {
        CPPUNIT_ASSERT(OsmUtils::fromTimeString(results[j]) != ElementData::TIMESTAMP_EMPTY);
      }
    }

    db.close();
  }

  void runRemapInsertTest()
  {
    // populate the database.
    HootApiDbWriter writer;
    writer.setUserEmail(userEmail());
    writer.setIncludeDebug(true);
    writer.open(ServicesDbTestUtils::getDbModifyUrl().toString());

    boost::shared_ptr<OsmMap> map(new OsmMap());

    boost::shared_ptr<Node> n1(new Node(Status::Unknown1, -1, 0.0, 0.0, 10.0));
    n1->setTag("note", "n1");
    boost::shared_ptr<Node> n2(new Node(Status::Unknown2, -2, 0.1, 0.0, 11.0));
    n2->setTag("note", "n2");
    boost::shared_ptr<Node> n3(new Node(Status::Conflated, -3, 0.2, 0.0, 12.0));
    n3->setTag("note", "n3");
    map->addNode(n1);
    map->addNode(n2);
    map->addNode(n3);

    boost::shared_ptr<Way> w1(new Way(Status::Unknown1, -1, 13.0));
    w1->addNode(-1);
    w1->addNode(-2);
    w1->setTag("note", "w1");
    boost::shared_ptr<Way> w2(new Way(Status::Unknown2, -2, 14.0));
    w2->addNode(-2);
    w2->addNode(-3);
    w2->setTag("note", "w2");
    map->addWay(w1);
    map->addWay(w2);

    boost::shared_ptr<Relation> r1(new Relation(Status::Unknown1, -1, 15.0, "collection"));
    r1->addElement("n1", n1->getElementId());
    r1->addElement("w1", w1->getElementId());
    r1->setTag("note", "r1");
    map->addRelation(r1);

    boost::shared_ptr<Relation> r2(new Relation(Status::Unknown1, -2, 15.0, "collection"));
    r2->addElement("r1", r1->getElementId());
    r2->setTag("note", "r2");
    map->addRelation(r2);

    writer.write(map);
    long mapId = writer.getMapId();

    compareRecords("SELECT email, display_name FROM " + ApiDb::getUsersTableName() +
                   " WHERE email LIKE :email",
                   "ServiceHootApiDbWriterTest@hoottestcpp.org;ServiceHootApiDbWriterTest",
                   userEmail());

    compareRecords("SELECT latitude, longitude, visible, tile, version, tags FROM " +
                   HootApiDb::getCurrentNodesTableName(mapId) +
                   " ORDER BY longitude",
                   "0;0;true;3221225472;1;\"note\"=>\"n1\", \"" + MetadataTags::HootId() + "\"=>\"-1\", \"" + MetadataTags::HootStatus() + "\"=>\"1\", \"" + MetadataTags::ErrorCircular() + "\"=>\"10\"\n"
                   "0;0.1;true;3221225992;1;\"note\"=>\"n2\", \"" + MetadataTags::HootId() + "\"=>\"-2\", \"" + MetadataTags::HootStatus() + "\"=>\"2\", \"" + MetadataTags::ErrorCircular() + "\"=>\"11\"\n"
                   "0;0.2;true;3221227552;1;\"note\"=>\"n3\", \"" + MetadataTags::HootId() + "\"=>\"-3\", \"" + MetadataTags::HootStatus() + "\"=>\"3\", \"" + MetadataTags::ErrorCircular() + "\"=>\"12\"",
                   (qlonglong)mapId);

    compareRecords("SELECT visible, version, tags FROM " +
                   HootApiDb::getCurrentWaysTableName(mapId) +
                   " ORDER BY id",
                   "true;1;\"note\"=>\"w2\", \"" + MetadataTags::HootId() + "\"=>\"-2\", \"" + MetadataTags::HootStatus() + "\"=>\"2\", \"" + MetadataTags::ErrorCircular() + "\"=>\"14\"\n"
                   "true;1;\"note\"=>\"w1\", \"" + MetadataTags::HootId() + "\"=>\"-1\", \"" + MetadataTags::HootStatus() + "\"=>\"1\", \"" + MetadataTags::ErrorCircular() + "\"=>\"13\"",
                   (qlonglong)mapId);

    compareRecords("SELECT sequence_id FROM " +
                   HootApiDb::getCurrentWayNodesTableName(mapId) +
                   " ORDER BY way_id, node_id, sequence_id",
                   "1\n"
                   "0\n"
                   "1\n"
                   "0",
                   (qlonglong)mapId);

    compareRecords("SELECT visible, version, tags FROM " +
                   HootApiDb::getCurrentRelationsTableName(mapId) +
                   " ORDER BY id",
                   "true;1;\"note\"=>\"r2\", \"type\"=>\"collection\", \"" + MetadataTags::HootId() + "\"=>\"-2\", \"" + MetadataTags::HootStatus() + "\"=>\"1\", \"" + MetadataTags::ErrorCircular() + "\"=>\"15\"\n"
                   "true;1;\"note\"=>\"r1\", \"type\"=>\"collection\", \"" + MetadataTags::HootId() + "\"=>\"-1\", \"" + MetadataTags::HootStatus() + "\"=>\"1\", \"" + MetadataTags::ErrorCircular() + "\"=>\"15\"",
                   (qlonglong)mapId);

    compareRecords("SELECT member_type, member_role, sequence_id "
                   "FROM " +
                   HootApiDb::getCurrentRelationMembersTableName(mapId) +
                   " ORDER BY relation_id, member_type",
                   "relation;r1;0\n"
                   "node;n1;0\n"
                   "way;w1;1",
                   (qlonglong)mapId);
  }

  struct CompareLess
  {
    bool operator() (const long& a, const long& b) const
    {
      return a < b;
    }

    static long max_value() { return std::numeric_limits<long>::max(); }
  };

  void setUp()
  {
    ServicesDbTestUtils::deleteUser(userEmail());
    HootApiDb db;
    db.open(ServicesDbTestUtils::getDbModifyUrl());
    db.getOrCreateUser(userEmail(), "ServiceHootApiDbWriterTest");
  }

  void tearDown()
  {
    ServicesDbTestUtils::deleteUser(userEmail());
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ServiceHootApiDbWriterTest, "slow");

}
