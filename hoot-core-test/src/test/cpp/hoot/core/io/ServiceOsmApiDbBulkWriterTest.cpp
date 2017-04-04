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
#include <hoot/core/io/OsmApiDbBulkWriter.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/io/OsmApiDbReader.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/util/DbUtils.h>

// Qt
#include <QDir>
#include <QFileInfo>

#include "../TestUtils.h"
#include "ServicesDbTestUtils.h"

namespace hoot
{

/*
 * TODO: need tests for:
 *
 * - unresolved way node members
 * - unresolved relation members
 * - config options error handling
 */
class ServiceOsmApiDbBulkWriterTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(ServiceOsmApiDbBulkWriterTest);
  CPPUNIT_TEST(runPsqlDbOfflineTest);
  CPPUNIT_TEST(runPsqlDbOfflineValidateOffTest);
  CPPUNIT_TEST(runPsqlDbOfflineStxxlTest);
  CPPUNIT_TEST(runPsqlDbOnlineTest);
  CPPUNIT_TEST(runPsqlCustomStartingIdsDbOfflineTest);
  CPPUNIT_TEST(runSqlFileOutputTest);
  CPPUNIT_TEST_SUITE_END();

public:

  shared_ptr<OsmMap> _map;

  shared_ptr<Node> createNode(double x, double y)
  {
    shared_ptr<Node> n(new Node(Status::Unknown1, _map->createNextNodeId(), x, y, 10.0));
    _map->addNode(n);
    return n;
  }

  shared_ptr<OsmMap> createTestMap()
  {
    shared_ptr<OsmMap> map(new OsmMap());
    _map = map;

    shared_ptr<Node> n1 = createNode(-77.0, 38.0);
    n1->setTag("building", "yes");
    n1->setTag("name", "n1");

    shared_ptr<Way> w1(new Way(Status::Unknown1, map->createNextWayId(), 13.0));
    w1->setTag("area", "yes");
    w1->setTag("building", "yes");
    w1->setTag("name", "w1");
    w1->addNode(createNode(-77.1, 38.0)->getId());
    w1->addNode(createNode(-77.2, 38.0)->getId());
    w1->addNode(createNode(-77.2, 38.1)->getId());
    w1->addNode(w1->getNodeId(0));
    map->addWay(w1);

    shared_ptr<Way> w2(new Way(Status::Unknown1, map->createNextWayId(), 13.0));
    w2->setTag("highway", "track");
    w2->setTag("name", "w2");
    w2->addNode(createNode(-77.3, 38.0)->getId());
    w2->addNode(createNode(-77.3, 38.1)->getId());
    map->addWay(w2);

    shared_ptr<Way> w3(new Way(Status::Unknown1, map->createNextWayId(), 13.0));
    w3->setTag("highway", "road");
    w3->setTag("name", "w3");
    w3->addNode(createNode(-77.4, 38.0)->getId());
    w3->addNode(createNode(-77.4, 38.1)->getId());
    map->addWay(w3);

    shared_ptr<Way> w4(new Way(Status::Unknown1, map->createNextWayId(), 13.0));
    w4->addNode(createNode(-77.5, 38.0)->getId());
    w4->addNode(createNode(-77.7, 38.0)->getId());
    w4->addNode(createNode(-77.6, 38.1)->getId());
    w4->addNode(w4->getNodeId(0));
    map->addWay(w4);

    shared_ptr<Way> w5(new Way(Status::Unknown1, map->createNextWayId(), 13.0));
    w5->addNode(createNode(-77.55, 38.01)->getId());
    w5->addNode(createNode(-77.65, 38.01)->getId());
    w5->addNode(createNode(-77.6, 38.05)->getId());
    w5->addNode(w5->getNodeId(0));
    map->addWay(w5);

    shared_ptr<Relation> r1(new Relation(Status::Unknown1, 1, 15.0, "multipolygon"));
    r1->setTag("building", "yes");
    r1->setTag("name", "r1");
    r1->addElement("outer", w4->getElementId());
    r1->addElement("inner", w5->getElementId());
    map->addRelation(r1);

    return map;
  }

  QStringList tokenizeOutputFileWithoutDates(const QString filePath)
  {
    QStringList tokens;

    //parse all the string tokens, except those containing dates, which we can't directly compare
    const QRegExp reDate("[12][0-9][0-9][0-9]-[01][0-9]-[0-3][0-9]");
    const QRegExp reTime("[0-2][0-9]:[0-5][0-9]:[0-5][0-9].[0-9][0-9][0-9]");

    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly))
    {
      QTextStream in(&file);
      while (!in.atEnd())
      {
        QString line = in.readLine();
        line = line.remove(reDate);
        line = line.remove(reTime);
        tokens << line;
      }
      file.close();
    }
    else
    {
      throw HootException("Unable to open file " + filePath + ".");
    }

    return tokens;
  }

  void verifySqlOutput(const QString stdFilePath, const QString outFilePath)
  {
    LOG_VART(stdFilePath);
    LOG_VART(outFilePath);
    const QStringList stdSqlTokens = tokenizeOutputFileWithoutDates(stdFilePath);
    const QStringList outputSqlTokens = tokenizeOutputFileWithoutDates(outFilePath);
    CPPUNIT_ASSERT_EQUAL(stdSqlTokens.size(), outputSqlTokens.size());
    for (int i = 0; i < stdSqlTokens.size(); i++)
    {
      HOOT_STR_EQUALS(stdSqlTokens.at(i), outputSqlTokens.at(i));
    }
  }

  void verifyDatabaseOutputOffline()
  {
    OsmApiDbReader reader;
    OsmMapPtr map(new OsmMap());
    reader.open(ServicesDbTestUtils::getOsmApiDbUrl().toString());
    reader.read(map);

    //verify current elements

    CPPUNIT_ASSERT_EQUAL((size_t)14, map->getNodes().size());
    CPPUNIT_ASSERT_EQUAL((int)2, map->getNode(14)->getTags().size());

    CPPUNIT_ASSERT_EQUAL((size_t)5, map->getWays().size());
    CPPUNIT_ASSERT_EQUAL((int)2, map->getWay(3)->getTags().size());
    CPPUNIT_ASSERT_EQUAL((int)2, map->getWay(4)->getTags().size());
    CPPUNIT_ASSERT_EQUAL((int)3, map->getWay(5)->getTags().size());
    CPPUNIT_ASSERT_EQUAL((size_t)4, map->getWay(1)->getNodeCount());
    CPPUNIT_ASSERT_EQUAL((size_t)4, map->getWay(2)->getNodeCount());
    CPPUNIT_ASSERT_EQUAL((size_t)2, map->getWay(3)->getNodeCount());
    CPPUNIT_ASSERT_EQUAL((size_t)2, map->getWay(4)->getNodeCount());
    CPPUNIT_ASSERT_EQUAL((size_t)4, map->getWay(5)->getNodeCount());

    CPPUNIT_ASSERT_EQUAL((size_t)1, map->getRelations().size());
    CPPUNIT_ASSERT_EQUAL((int)2, map->getRelation(1)->getTags().size());
    CPPUNIT_ASSERT_EQUAL((size_t)2, map->getRelation(1)->getMembers().size());

    //verify historical element table sizes
    CPPUNIT_ASSERT_EQUAL(
      (long)14,
      DbUtils::getRowCount(reader._getDatabase()->getDB(), ApiDb::getNodesTableName()));
    CPPUNIT_ASSERT_EQUAL(
      (long)2,
      DbUtils::getRowCount(reader._getDatabase()->getDB(), ApiDb::getNodeTagsTableName()));
    CPPUNIT_ASSERT_EQUAL(
      (long)5,
      DbUtils::getRowCount(reader._getDatabase()->getDB(), ApiDb::getWaysTableName()));
    CPPUNIT_ASSERT_EQUAL(
      (long)7,
      DbUtils::getRowCount(reader._getDatabase()->getDB(), ApiDb::getWayTagsTableName()));
    CPPUNIT_ASSERT_EQUAL(
      (long)16,
      DbUtils::getRowCount(reader._getDatabase()->getDB(), ApiDb::getWayNodesTableName()));
    CPPUNIT_ASSERT_EQUAL(
      (long)1,
      DbUtils::getRowCount(reader._getDatabase()->getDB(), ApiDb::getRelationsTableName()));
    CPPUNIT_ASSERT_EQUAL(
      (long)2,
      DbUtils::getRowCount(reader._getDatabase()->getDB(), ApiDb::getRelationTagsTableName()));
    CPPUNIT_ASSERT_EQUAL(
      (long)2,
      DbUtils::getRowCount(reader._getDatabase()->getDB(), ApiDb::getRelationMembersTableName()));

    //verify changeset table size
    CPPUNIT_ASSERT_EQUAL(
      (long)4,
      DbUtils::getRowCount(reader._getDatabase()->getDB(), ApiDb::getChangesetsTableName()));

    //verify sequences
//    shared_ptr<OsmApiDb> osmApiDb = dynamic_pointer_cast<OsmApiDb>(reader._getDatabase());
//    //TODO: fix?
//    CPPUNIT_ASSERT_EQUAL((long)15, osmApiDb->getNextId(ElementType::Node));
//    CPPUNIT_ASSERT_EQUAL((long)6, osmApiDb->getNextId(ElementType::Way));
//    CPPUNIT_ASSERT_EQUAL((long)2, osmApiDb->getNextId(ElementType::Relation));
//    CPPUNIT_ASSERT_EQUAL((long)5, osmApiDb->getNextId(ApiDb::getChangesetsTableName()));

    reader.close();
  }

  void verifyDatabaseOutputOfflineValidateOff()
  {
    OsmApiDbReader reader;
    OsmMapPtr map(new OsmMap());
    reader.open(ServicesDbTestUtils::getOsmApiDbUrl().toString());
    reader.read(map);

    //verify current elements

    CPPUNIT_ASSERT_EQUAL((size_t)14, map->getNodes().size());
    CPPUNIT_ASSERT_EQUAL((int)2, map->getNode(1)->getTags().size());

    CPPUNIT_ASSERT_EQUAL((size_t)5, map->getWays().size());
    CPPUNIT_ASSERT_EQUAL((int)2, map->getWay(3)->getTags().size());
    CPPUNIT_ASSERT_EQUAL((int)2, map->getWay(2)->getTags().size());
    CPPUNIT_ASSERT_EQUAL((int)3, map->getWay(1)->getTags().size());
    CPPUNIT_ASSERT_EQUAL((size_t)4, map->getWay(5)->getNodeCount());
    CPPUNIT_ASSERT_EQUAL((size_t)4, map->getWay(4)->getNodeCount());
    CPPUNIT_ASSERT_EQUAL((size_t)2, map->getWay(3)->getNodeCount());
    CPPUNIT_ASSERT_EQUAL((size_t)2, map->getWay(2)->getNodeCount());
    CPPUNIT_ASSERT_EQUAL((size_t)4, map->getWay(1)->getNodeCount());

    CPPUNIT_ASSERT_EQUAL((size_t)1, map->getRelations().size());
    CPPUNIT_ASSERT_EQUAL((int)2, map->getRelation(1)->getTags().size());
    CPPUNIT_ASSERT_EQUAL((size_t)2, map->getRelation(1)->getMembers().size());

    //verify historical element table sizes
    CPPUNIT_ASSERT_EQUAL(
      (long)14,
      DbUtils::getRowCount(reader._getDatabase()->getDB(), ApiDb::getNodesTableName()));
    CPPUNIT_ASSERT_EQUAL(
      (long)2,
      DbUtils::getRowCount(reader._getDatabase()->getDB(), ApiDb::getNodeTagsTableName()));
    CPPUNIT_ASSERT_EQUAL(
      (long)5,
      DbUtils::getRowCount(reader._getDatabase()->getDB(), ApiDb::getWaysTableName()));
    CPPUNIT_ASSERT_EQUAL(
      (long)7,
      DbUtils::getRowCount(reader._getDatabase()->getDB(), ApiDb::getWayTagsTableName()));
    CPPUNIT_ASSERT_EQUAL(
      (long)16,
      DbUtils::getRowCount(reader._getDatabase()->getDB(), ApiDb::getWayNodesTableName()));
    CPPUNIT_ASSERT_EQUAL(
      (long)1,
      DbUtils::getRowCount(reader._getDatabase()->getDB(), ApiDb::getRelationsTableName()));
    CPPUNIT_ASSERT_EQUAL(
      (long)2,
      DbUtils::getRowCount(reader._getDatabase()->getDB(), ApiDb::getRelationTagsTableName()));
    CPPUNIT_ASSERT_EQUAL(
      (long)2,
      DbUtils::getRowCount(reader._getDatabase()->getDB(), ApiDb::getRelationMembersTableName()));

    //verify changeset table size
    CPPUNIT_ASSERT_EQUAL(
      (long)4,
      DbUtils::getRowCount(reader._getDatabase()->getDB(), ApiDb::getChangesetsTableName()));

    //verify sequences
//    shared_ptr<OsmApiDb> osmApiDb = dynamic_pointer_cast<OsmApiDb>(reader._getDatabase());
//    //TODO: fix?
//    CPPUNIT_ASSERT_EQUAL((long)15, osmApiDb->getNextId(ElementType::Node));
//    CPPUNIT_ASSERT_EQUAL((long)6, osmApiDb->getNextId(ElementType::Way));
//    CPPUNIT_ASSERT_EQUAL((long)2, osmApiDb->getNextId(ElementType::Relation));
//    CPPUNIT_ASSERT_EQUAL((long)5, osmApiDb->getNextId(ApiDb::getChangesetsTableName()));

    reader.close();
  }

  void verifyDatabaseOutputOfflineWithCustomStartingIds()
  {
    OsmApiDbReader reader;
    OsmMapPtr map(new OsmMap());
    reader.open(ServicesDbTestUtils::getOsmApiDbUrl().toString());
    reader.read(map);

    //verify current elements

    CPPUNIT_ASSERT_EQUAL((size_t)14, map->getNodes().size());
    CPPUNIT_ASSERT_EQUAL((int)2, map->getNode(16)->getTags().size());

    CPPUNIT_ASSERT_EQUAL((size_t)5, map->getWays().size());
    CPPUNIT_ASSERT_EQUAL((int)2, map->getWay(6)->getTags().size());
    CPPUNIT_ASSERT_EQUAL((int)2, map->getWay(7)->getTags().size());
    CPPUNIT_ASSERT_EQUAL((int)3, map->getWay(8)->getTags().size());
    CPPUNIT_ASSERT_EQUAL((size_t)4, map->getWay(4)->getNodeCount());
    CPPUNIT_ASSERT_EQUAL((size_t)4, map->getWay(5)->getNodeCount());
    CPPUNIT_ASSERT_EQUAL((size_t)2, map->getWay(6)->getNodeCount());
    CPPUNIT_ASSERT_EQUAL((size_t)2, map->getWay(7)->getNodeCount());
    CPPUNIT_ASSERT_EQUAL((size_t)4, map->getWay(8)->getNodeCount());

    CPPUNIT_ASSERT_EQUAL((size_t)1, map->getRelations().size());
    CPPUNIT_ASSERT_EQUAL((int)2, map->getRelation(5)->getTags().size());
    CPPUNIT_ASSERT_EQUAL((size_t)2, map->getRelation(5)->getMembers().size());

    //verify historical element table sizes
    CPPUNIT_ASSERT_EQUAL(
      (long)14,
      DbUtils::getRowCount(reader._getDatabase()->getDB(), ApiDb::getNodesTableName()));
    CPPUNIT_ASSERT_EQUAL(
      (long)2,
      DbUtils::getRowCount(reader._getDatabase()->getDB(), ApiDb::getNodeTagsTableName()));
    CPPUNIT_ASSERT_EQUAL(
      (long)5,
      DbUtils::getRowCount(reader._getDatabase()->getDB(), ApiDb::getWaysTableName()));
    CPPUNIT_ASSERT_EQUAL(
      (long)7,
      DbUtils::getRowCount(reader._getDatabase()->getDB(), ApiDb::getWayTagsTableName()));
    CPPUNIT_ASSERT_EQUAL(
      (long)16,
      DbUtils::getRowCount(reader._getDatabase()->getDB(), ApiDb::getWayNodesTableName()));
    CPPUNIT_ASSERT_EQUAL(
      (long)1,
      DbUtils::getRowCount(reader._getDatabase()->getDB(), ApiDb::getRelationsTableName()));
    CPPUNIT_ASSERT_EQUAL(
      (long)2,
      DbUtils::getRowCount(reader._getDatabase()->getDB(), ApiDb::getRelationTagsTableName()));
    CPPUNIT_ASSERT_EQUAL(
      (long)2,
      DbUtils::getRowCount(reader._getDatabase()->getDB(), ApiDb::getRelationMembersTableName()));

    //verify changeset table size
    CPPUNIT_ASSERT_EQUAL(
      (long)4,
      DbUtils::getRowCount(reader._getDatabase()->getDB(), ApiDb::getChangesetsTableName()));

    //verify sequences
//    shared_ptr<OsmApiDb> osmApiDb = dynamic_pointer_cast<OsmApiDb>(reader._getDatabase());
//    //TODO: fix?
//    CPPUNIT_ASSERT_EQUAL((long)17, osmApiDb->getNextId(ElementType::Node));
//    CPPUNIT_ASSERT_EQUAL((long)9, osmApiDb->getNextId(ElementType::Way));
//    CPPUNIT_ASSERT_EQUAL((long)6, osmApiDb->getNextId(ElementType::Relation));
//    CPPUNIT_ASSERT_EQUAL((long)5, osmApiDb->getNextId(ApiDb::getChangesetsTableName()));

    reader.close();
  }

  void verifyDatabaseOutputOnline()
  {
    OsmApiDbReader reader;
    OsmMapPtr map(new OsmMap());
    reader.open(ServicesDbTestUtils::getOsmApiDbUrl().toString());
    reader.read(map);

    //verify current elements

    CPPUNIT_ASSERT_EQUAL((size_t)16, map->getNodes().size());
    CPPUNIT_ASSERT_EQUAL((int)2, map->getNode(17)->getTags().size());

    CPPUNIT_ASSERT_EQUAL((size_t)6, map->getWays().size());
    CPPUNIT_ASSERT_EQUAL((int)2, map->getWay(5)->getTags().size());
    CPPUNIT_ASSERT_EQUAL((int)2, map->getWay(6)->getTags().size());
    CPPUNIT_ASSERT_EQUAL((int)3, map->getWay(7)->getTags().size());
    CPPUNIT_ASSERT_EQUAL((size_t)4, map->getWay(3)->getNodeCount());
    CPPUNIT_ASSERT_EQUAL((size_t)4, map->getWay(4)->getNodeCount());
    CPPUNIT_ASSERT_EQUAL((size_t)2, map->getWay(5)->getNodeCount());
    CPPUNIT_ASSERT_EQUAL((size_t)2, map->getWay(6)->getNodeCount());
    CPPUNIT_ASSERT_EQUAL((size_t)4, map->getWay(7)->getNodeCount());

    CPPUNIT_ASSERT_EQUAL((size_t)2, map->getRelations().size());
    CPPUNIT_ASSERT_EQUAL((int)2, map->getRelation(3)->getTags().size());
    CPPUNIT_ASSERT_EQUAL((size_t)2, map->getRelation(3)->getMembers().size());

    //verify historical element table sizes
    CPPUNIT_ASSERT_EQUAL(
      (long)16,
      DbUtils::getRowCount(reader._getDatabase()->getDB(), ApiDb::getNodesTableName()));
    CPPUNIT_ASSERT_EQUAL(
      (long)5,
      DbUtils::getRowCount(reader._getDatabase()->getDB(), ApiDb::getNodeTagsTableName()));
    CPPUNIT_ASSERT_EQUAL(
      (long)6,
      DbUtils::getRowCount(reader._getDatabase()->getDB(), ApiDb::getWaysTableName()));
    CPPUNIT_ASSERT_EQUAL(
      (long)8,
      DbUtils::getRowCount(reader._getDatabase()->getDB(), ApiDb::getWayTagsTableName()));
    CPPUNIT_ASSERT_EQUAL(
      (long)18,
      DbUtils::getRowCount(reader._getDatabase()->getDB(), ApiDb::getWayNodesTableName()));
    CPPUNIT_ASSERT_EQUAL(
      (long)2,
      DbUtils::getRowCount(reader._getDatabase()->getDB(), ApiDb::getRelationsTableName()));
    CPPUNIT_ASSERT_EQUAL(
      (long)3,
      DbUtils::getRowCount(reader._getDatabase()->getDB(), ApiDb::getRelationTagsTableName()));
    CPPUNIT_ASSERT_EQUAL(
      (long)4,
      DbUtils::getRowCount(reader._getDatabase()->getDB(), ApiDb::getRelationMembersTableName()));

    //verify changeset table size
    CPPUNIT_ASSERT_EQUAL(
      (long)5,
      DbUtils::getRowCount(reader._getDatabase()->getDB(), ApiDb::getChangesetsTableName()));

    //verify sequences
    shared_ptr<OsmApiDb> osmApiDb = dynamic_pointer_cast<OsmApiDb>(reader._getDatabase());
    CPPUNIT_ASSERT_EQUAL((long)18, osmApiDb->getNextId(ElementType::Node));
    CPPUNIT_ASSERT_EQUAL((long)8, osmApiDb->getNextId(ElementType::Way));
    CPPUNIT_ASSERT_EQUAL((long)4, osmApiDb->getNextId(ElementType::Relation));
    CPPUNIT_ASSERT_EQUAL((long)7, osmApiDb->getNextId(ApiDb::getChangesetsTableName()));

    reader.close();
  }

  void verifyDatabaseEmpty()
  {
    OsmApiDbReader reader;
    OsmMapPtr map(new OsmMap());
    reader.open(ServicesDbTestUtils::getOsmApiDbUrl().toString());
    reader.read(map);

    //verify current elements
    CPPUNIT_ASSERT_EQUAL((size_t)0, map->getNodes().size());
    CPPUNIT_ASSERT_EQUAL((size_t)0, map->getWays().size());
    CPPUNIT_ASSERT_EQUAL((size_t)0, map->getRelations().size());

    //verify historical element table sizes
    CPPUNIT_ASSERT_EQUAL(
      (long)0,
      DbUtils::getRowCount(reader._getDatabase()->getDB(), ApiDb::getNodesTableName()));
    CPPUNIT_ASSERT_EQUAL(
      (long)0,
      DbUtils::getRowCount(reader._getDatabase()->getDB(), ApiDb::getNodeTagsTableName()));
    CPPUNIT_ASSERT_EQUAL(
      (long)0,
      DbUtils::getRowCount(reader._getDatabase()->getDB(), ApiDb::getWaysTableName()));
    CPPUNIT_ASSERT_EQUAL(
      (long)0,
      DbUtils::getRowCount(reader._getDatabase()->getDB(), ApiDb::getWayTagsTableName()));
    CPPUNIT_ASSERT_EQUAL(
      (long)0,
      DbUtils::getRowCount(reader._getDatabase()->getDB(), ApiDb::getWayNodesTableName()));
    CPPUNIT_ASSERT_EQUAL(
      (long)0,
      DbUtils::getRowCount(reader._getDatabase()->getDB(), ApiDb::getRelationsTableName()));
    CPPUNIT_ASSERT_EQUAL(
      (long)0,
      DbUtils::getRowCount(reader._getDatabase()->getDB(), ApiDb::getRelationTagsTableName()));
    CPPUNIT_ASSERT_EQUAL(
      (long)0,
      DbUtils::getRowCount(reader._getDatabase()->getDB(), ApiDb::getRelationMembersTableName()));

    //verify changeset table size
    CPPUNIT_ASSERT_EQUAL(
      (long)0,
      DbUtils::getRowCount(reader._getDatabase()->getDB(), ApiDb::getChangesetsTableName()));

    //verify sequences
    shared_ptr<OsmApiDb> osmApiDb = dynamic_pointer_cast<OsmApiDb>(reader._getDatabase());
    CPPUNIT_ASSERT_EQUAL((long)1, osmApiDb->getNextId(ElementType::Node));
    CPPUNIT_ASSERT_EQUAL((long)1, osmApiDb->getNextId(ElementType::Way));
    CPPUNIT_ASSERT_EQUAL((long)1, osmApiDb->getNextId(ElementType::Relation));
    CPPUNIT_ASSERT_EQUAL((long)1, osmApiDb->getNextId(ApiDb::getChangesetsTableName()));

    reader.close();
  }

  void runPsqlDbOfflineTest()
  {
    QDir().mkpath("test-output/io/OsmApiDbBulkWriterTest/");

    //init db
    ServicesDbTestUtils::deleteDataFromOsmApiTestDatabase();
    ServicesDbTestUtils::execOsmApiDbSqlTestScript("users.sql");

    OsmApiDbBulkWriter writer;
    const QString outFile =
      "test-output/io/OsmApiDbBulkWriterTest/OsmApiDbBulkWriter-psql-offline-out.sql";
    writer.setReserveRecordIdsBeforeWritingData(false);
    writer.setOutputFilesCopyLocation(outFile);
    writer.setStatusUpdateInterval(1);
    writer.setChangesetUserId(1);
    writer.setMaxChangesetSize(5);
    writer.setFileOutputElementBufferSize(3);

    writer.open(ServicesDbTestUtils::getOsmApiDbUrl().toString());
    writer.write(createTestMap());
    writer.close();

    verifySqlOutput(
      "test-files/io/OsmApiDbBulkWriterTest/OsmApiDbBulkWriter-psql-offline.sql", outFile);
    verifyDatabaseOutputOffline();
  }

  void runPsqlDbOfflineValidateOffTest()
  {
    QDir().mkpath("test-output/io/OsmApiDbBulkWriterTest/");
    OsmMap::resetCounters();

    //init db
    ServicesDbTestUtils::deleteDataFromOsmApiTestDatabase();
    ServicesDbTestUtils::execOsmApiDbSqlTestScript("users.sql");

    OsmApiDbBulkWriter writer;
    const QString outFile =
      "test-output/io/OsmApiDbBulkWriterTest/OsmApiDbBulkWriter-psql-offline-validate-off-out.sql";
    writer.setReserveRecordIdsBeforeWritingData(false);
    writer.setOutputFilesCopyLocation(outFile);
    writer.setStatusUpdateInterval(1);
    writer.setChangesetUserId(1);
    writer.setMaxChangesetSize(5);
    writer.setFileOutputElementBufferSize(3);
    writer.setValidateData(false);

    writer.open(ServicesDbTestUtils::getOsmApiDbUrl().toString());
    writer.write(createTestMap());
    writer.close();

    //the element IDs will be reversed in the sql output with validation off, due to the way the
    //osm data reading works
    verifySqlOutput(
      "test-files/io/OsmApiDbBulkWriterTest/OsmApiDbBulkWriter-psql-offline-validate-off.sql",
      outFile);
    verifyDatabaseOutputOfflineValidateOff();
  }

  void runPsqlDbOfflineStxxlTest()
  {
    QDir().mkpath("test-output/io/OsmApiDbBulkWriterTest/");

    //init db
    ServicesDbTestUtils::deleteDataFromOsmApiTestDatabase();
    ServicesDbTestUtils::execOsmApiDbSqlTestScript("users.sql");

    OsmApiDbBulkWriter writer;
    const QString outFile =
      "test-output/io/OsmApiDbBulkWriterTest/OsmApiDbBulkWriter-psql-offline-stxxl-out.sql";
    writer.setReserveRecordIdsBeforeWritingData(false);
    writer.setOutputFilesCopyLocation(outFile);
    writer.setStatusUpdateInterval(1);
    writer.setChangesetUserId(1);
    writer.setMaxChangesetSize(5);
    writer.setFileOutputElementBufferSize(3);
    writer.setStxxlMapMinSize(3);

    writer.open(ServicesDbTestUtils::getOsmApiDbUrl().toString());
    writer.write(createTestMap());
    writer.close();

    verifySqlOutput(
      "test-files/io/OsmApiDbBulkWriterTest/OsmApiDbBulkWriter-psql-offline.sql", outFile);
    verifyDatabaseOutputOffline();
  }

  void runPsqlDbOnlineTest()
  {
    QDir().mkpath("test-output/io/OsmApiDbBulkWriterTest/");

    //init db
    ServicesDbTestUtils::deleteDataFromOsmApiTestDatabase();
    ServicesDbTestUtils::execOsmApiDbSqlTestScript("users.sql");

    OsmApiDbBulkWriter writer;
    const QString outFile =
      "test-output/io/OsmApiDbBulkWriterTest/OsmApiDbBulkWriter-psql-online-out.sql";
    writer.setReserveRecordIdsBeforeWritingData(true);
    writer.setOutputFilesCopyLocation(outFile);
    writer.setStatusUpdateInterval(1);
    writer.setChangesetUserId(1);
    writer.setMaxChangesetSize(5);
    writer.setFileOutputElementBufferSize(3);

    writer.open(ServicesDbTestUtils::getOsmApiDbUrl().toString());

    //write some data from somewhere else while before our writer starts writing data
    ServicesDbTestUtils::execOsmApiDbSqlTestScript("changesets.sql"); //1 changeset
    ServicesDbTestUtils::execOsmApiDbSqlTestScript("nodes.sql"); //2 nodes
    ServicesDbTestUtils::execOsmApiDbSqlTestScript("ways.sql"); //1 way
    ServicesDbTestUtils::execOsmApiDbSqlTestScript("relations.sql"); //1 relation

    writer.write(createTestMap());
    writer.close();

    verifySqlOutput(
      "test-files/io/OsmApiDbBulkWriterTest/OsmApiDbBulkWriter-psql-online.sql", outFile);
    verifyDatabaseOutputOnline();
  }

  void runPsqlCustomStartingIdsDbOfflineTest()
  {
    QDir().mkpath("test-output/io/OsmApiDbBulkWriterTest/");

    //init db
    ServicesDbTestUtils::deleteDataFromOsmApiTestDatabase();
    ServicesDbTestUtils::execOsmApiDbSqlTestScript("users.sql");

    OsmApiDbBulkWriter writer;
    const QString outFile =
      "test-output/io/OsmApiDbBulkWriterTest/OsmApiDbBulkWriter-psql-offline-starting-ids-out.sql";
    writer.setReserveRecordIdsBeforeWritingData(false);
    writer.setOutputFilesCopyLocation(outFile);
    writer.setStatusUpdateInterval(1);
    writer.setChangesetUserId(1);
    writer.setMaxChangesetSize(5);
    writer.setFileOutputElementBufferSize(3);
    writer.setStartingNodeId(3);
    writer.setStartingWayId(4);
    writer.setStartingRelationId(5);

    writer.open(ServicesDbTestUtils::getOsmApiDbUrl().toString());
    writer.write(createTestMap());
    writer.close();

    verifySqlOutput(
      "test-files/io/OsmApiDbBulkWriterTest/OsmApiDbBulkWriter-psql-offline-starting-ids.sql",
      outFile);

    verifyDatabaseOutputOfflineWithCustomStartingIds();
  }

  void runSqlFileOutputTest()
  {
    QDir().mkpath("test-output/io/OsmApiDbBulkWriterTest/");

    //init db
    ServicesDbTestUtils::deleteDataFromOsmApiTestDatabase();
    ServicesDbTestUtils::execOsmApiDbSqlTestScript("users.sql");

    OsmApiDbBulkWriter writer;
    const QString outFile =
      "test-output/io/OsmApiDbBulkWriterTest/OsmApiDbBulkWriter-sql-file-out.sql";
    writer.setReserveRecordIdsBeforeWritingData(false);
    writer.setStatusUpdateInterval(1);
    writer.setChangesetUserId(1);
    writer.setMaxChangesetSize(5);
    writer.setFileOutputElementBufferSize(3);

    writer.open(outFile);
    writer.write(createTestMap());
    writer.close();

    verifySqlOutput(
      "test-files/io/OsmApiDbBulkWriterTest/OsmApiDbBulkWriter-psql-offline.sql", outFile);
    verifyDatabaseEmpty();
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ServiceOsmApiDbBulkWriterTest, "slow");

}
