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
#include <hoot/core/io/OsmApiDbBulkInserter.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/io/OsmApiDbReader.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/util/DbUtils.h>
#include <hoot/core/util/StringUtils.h>

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
class ServiceOsmApiDbBulkInserterTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(ServiceOsmApiDbBulkInserterTest);
  //CPPUNIT_TEST(maxDataTypeSizeTest);
  CPPUNIT_TEST(runPsqlDbOfflineTest);
  CPPUNIT_TEST(runPsqlDbOfflineValidateOffTest);
  CPPUNIT_TEST(runPsqlDbOfflineStxxlTest);
  CPPUNIT_TEST(runPsqlDbOnlineTest);
  CPPUNIT_TEST(runPsqlCustomStartingIdsDbOfflineTest);
  CPPUNIT_TEST(runSqlFileOutputTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void maxDataTypeSizeTest()
  {
    std::cout << "size of short: " << sizeof(short) << std::endl;
    std::cout << "max size of short: " <<
                 StringUtils::formatLargeNumber(std::numeric_limits<short>::max()) << std::endl;
    std::cout << "size of unsigned short: " << sizeof(unsigned short) << std::endl;
    std::cout << "max size of unsigned short: " <<
                 StringUtils::formatLargeNumber(std::numeric_limits<unsigned short>::max()) << std::endl;
    std::cout << "size of int: " << sizeof(int) << std::endl;
    std::cout << "max size of int : " <<
                 StringUtils::formatLargeNumber(std::numeric_limits<int>::max()) << std::endl;
    std::cout << "size of unsigned int: " << sizeof(unsigned int) << std::endl;
    std::cout << "max size of unsigned int : " <<
                 StringUtils::formatLargeNumber(std::numeric_limits<unsigned int>::max()) << std::endl;
    std::cout << "size of long: " << sizeof(long) << std::endl;
    std::cout << "max size of long : " <<
                 StringUtils::formatLargeNumber(std::numeric_limits<long>::max()) << std::endl;
    std::cout << "size of unsigned long: " << sizeof(unsigned long) << std::endl;
    std::cout << "max size of unsigned long : " <<
                 StringUtils::formatLargeNumber(std::numeric_limits<unsigned long>::max()) << std::endl;
    std::cout << "size of float: " << sizeof(float) << std::endl;
    std::cout << "max size of float : " <<
                 StringUtils::formatLargeNumber(std::numeric_limits<float>::max()) << std::endl;
    std::cout << "size of double: " << sizeof(double) << std::endl;
    std::cout << "max size of double : " <<
                 StringUtils::formatLargeNumber(std::numeric_limits<double>::max()) << std::endl;
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

    //verify sequences - sequences can't be updated b/c of a chicken egg situation with nextval; sql
    //file validation will have to be good enough
//    boost::shared_ptr<OsmApiDb> osmApiDb = boost::dynamic_pointer_cast<OsmApiDb>(reader._getDatabase());
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
//    boost::shared_ptr<OsmApiDb> osmApiDb = boost::dynamic_pointer_cast<OsmApiDb>(reader._getDatabase());
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
//    boost::shared_ptr<OsmApiDb> osmApiDb = boost::dynamic_pointer_cast<OsmApiDb>(reader._getDatabase());
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
    boost::shared_ptr<OsmApiDb> osmApiDb = boost::dynamic_pointer_cast<OsmApiDb>(reader._getDatabase());
    CPPUNIT_ASSERT_EQUAL((long)18, osmApiDb->getNextId(ElementType::Node));
    CPPUNIT_ASSERT_EQUAL((long)8, osmApiDb->getNextId(ElementType::Way));
    CPPUNIT_ASSERT_EQUAL((long)4, osmApiDb->getNextId(ElementType::Relation));
    CPPUNIT_ASSERT_EQUAL((long)7, osmApiDb->getNextId(ApiDb::getChangesetsTableName()));

    reader.close();
  }

  void runPsqlDbOfflineTest()
  {
    QDir().mkpath("test-output/io/ServiceOsmApiDbBulkInserterTest/");

    //init db
    ServicesDbTestUtils::deleteDataFromOsmApiTestDatabase();
    const QString scriptDir = "test-files/servicesdb";
    ApiDb::execSqlFile(ServicesDbTestUtils::getOsmApiDbUrl().toString(), scriptDir + "/users.sql");

    OsmApiDbBulkInserter writer;
    const QString outFile = "test-output/io/ServiceOsmApiDbBulkInserterTest/psql-offline-out.sql";
    writer.setReserveRecordIdsBeforeWritingData(false);
    writer.setOutputFilesCopyLocation(outFile);
    writer.setStatusUpdateInterval(1);
    writer.setChangesetUserId(1);
    writer.setMaxChangesetSize(5);
    writer.setFileOutputElementBufferSize(3);
    writer.setValidateData(true);
    writer.setDisableDatabaseConstraintsDuringWrite(true);
    writer.setDisableDatabaseIndexesDuringWrite(true);

    writer.open(ServicesDbTestUtils::getOsmApiDbUrl().toString());
    writer.write(ServicesDbTestUtils::createTestMap1());
    writer.close();

    TestUtils::verifyStdMatchesOutputIgnoreDate(
      "test-files/io/ServiceOsmApiDbBulkInserterTest/psql-offline.sql",
      outFile);
    verifyDatabaseOutputOffline();
  }

  void runPsqlDbOfflineValidateOffTest()
  {
    QDir().mkpath("test-output/io/ServiceOsmApiDbBulkInserterTest/");
    OsmMap::resetCounters();

    //init db
    ServicesDbTestUtils::deleteDataFromOsmApiTestDatabase();
    const QString scriptDir = "test-files/servicesdb";
    ApiDb::execSqlFile(ServicesDbTestUtils::getOsmApiDbUrl().toString(), scriptDir + "/users.sql");

    OsmApiDbBulkInserter writer;
    const QString outFile =
      "test-output/io/ServiceOsmApiDbBulkInserterTest/psql-offline-validate-off-out.sql";
    writer.setReserveRecordIdsBeforeWritingData(false);
    writer.setOutputFilesCopyLocation(outFile);
    writer.setStatusUpdateInterval(1);
    writer.setChangesetUserId(1);
    writer.setMaxChangesetSize(5);
    writer.setFileOutputElementBufferSize(3);
    writer.setValidateData(false);
    writer.setDisableDatabaseConstraintsDuringWrite(true);
    writer.setDisableDatabaseIndexesDuringWrite(true);

    writer.open(ServicesDbTestUtils::getOsmApiDbUrl().toString());
    writer.write(ServicesDbTestUtils::createTestMap1());
    writer.close();

    //the element IDs will be reversed in the sql output with validation off, due to the way the
    //osm data reading works
    TestUtils::verifyStdMatchesOutputIgnoreDate(
      "test-files/io/ServiceOsmApiDbBulkInserterTest/psql-offline-validate-off.sql",
      outFile);
    verifyDatabaseOutputOfflineValidateOff();
  }

  void runPsqlDbOfflineStxxlTest()
  {
    QDir().mkpath("test-output/io/ServiceOsmApiDbBulkInserterTest/");

    //init db
    ServicesDbTestUtils::deleteDataFromOsmApiTestDatabase();
    const QString scriptDir = "test-files/servicesdb";
    ApiDb::execSqlFile(ServicesDbTestUtils::getOsmApiDbUrl().toString(), scriptDir + "/users.sql");

    OsmApiDbBulkInserter writer;
    const QString outFile =
      "test-output/io/ServiceOsmApiDbBulkInserterTest/psql-offline-stxxl-out.sql";
    writer.setReserveRecordIdsBeforeWritingData(false);
    writer.setOutputFilesCopyLocation(outFile);
    writer.setStatusUpdateInterval(1);
    writer.setChangesetUserId(1);
    writer.setMaxChangesetSize(5);
    writer.setFileOutputElementBufferSize(3);
    writer.setStxxlMapMinSize(3);
    writer.setValidateData(true);
    writer.setDisableDatabaseConstraintsDuringWrite(true);
    writer.setDisableDatabaseIndexesDuringWrite(true);

    writer.open(ServicesDbTestUtils::getOsmApiDbUrl().toString());
    writer.write(ServicesDbTestUtils::createTestMap1());
    writer.close();

    TestUtils::verifyStdMatchesOutputIgnoreDate(
      "test-files/io/ServiceOsmApiDbBulkInserterTest/psql-offline.sql", outFile);
    verifyDatabaseOutputOffline();
  }

  void runPsqlDbOnlineTest()
  {
    QDir().mkpath("test-output/io/ServiceOsmApiDbBulkInserterTest/");

    //init db
    ServicesDbTestUtils::deleteDataFromOsmApiTestDatabase();
    const QString scriptDir = "test-files/servicesdb";
    ApiDb::execSqlFile(ServicesDbTestUtils::getOsmApiDbUrl().toString(), scriptDir + "/users.sql");

    OsmApiDbBulkInserter writer;
    const QString outFile = "test-output/io/ServiceOsmApiDbBulkInserterTest/psql-online-out.sql";
    writer.setReserveRecordIdsBeforeWritingData(true);
    writer.setOutputFilesCopyLocation(outFile);
    writer.setStatusUpdateInterval(1);
    writer.setChangesetUserId(1);
    writer.setMaxChangesetSize(5);
    writer.setFileOutputElementBufferSize(3);
    writer.setValidateData(true);
    writer.setDisableDatabaseConstraintsDuringWrite(false);
    writer.setDisableDatabaseIndexesDuringWrite(false);

    writer.open(ServicesDbTestUtils::getOsmApiDbUrl().toString());

    //write some data from somewhere else while before our writer starts writing data
    ApiDb::execSqlFile(
      ServicesDbTestUtils::getOsmApiDbUrl().toString(), scriptDir + "/changesets.sql"); //1 changeset
    ApiDb::execSqlFile(
      ServicesDbTestUtils::getOsmApiDbUrl().toString(), scriptDir + "/nodes.sql"); //2 nodes
    ApiDb::execSqlFile(
      ServicesDbTestUtils::getOsmApiDbUrl().toString(), scriptDir + "/ways.sql"); //1 way
    ApiDb::execSqlFile(
      ServicesDbTestUtils::getOsmApiDbUrl().toString(), scriptDir + "/relations.sql"); //1 relation

    writer.write(ServicesDbTestUtils::createTestMap1());
    writer.close();

    TestUtils::verifyStdMatchesOutputIgnoreDate(
      "test-files/io/ServiceOsmApiDbBulkInserterTest/psql-online.sql", outFile);
    verifyDatabaseOutputOnline();
  }

  void runPsqlCustomStartingIdsDbOfflineTest()
  {
    QDir().mkpath("test-output/io/ServiceOsmApiDbBulkInserterTest/");

    //init db
    ServicesDbTestUtils::deleteDataFromOsmApiTestDatabase();
    const QString scriptDir = "test-files/servicesdb";
    ApiDb::execSqlFile(ServicesDbTestUtils::getOsmApiDbUrl().toString(), scriptDir + "/users.sql");

    OsmApiDbBulkInserter writer;
    const QString outFile =
      "test-output/io/ServiceOsmApiDbBulkInserterTest/psql-offline-starting-ids-out.sql";
    writer.setReserveRecordIdsBeforeWritingData(false);
    writer.setOutputFilesCopyLocation(outFile);
    writer.setStatusUpdateInterval(1);
    writer.setChangesetUserId(1);
    writer.setMaxChangesetSize(5);
    writer.setFileOutputElementBufferSize(3);
    writer.setStartingNodeId(3);
    writer.setStartingWayId(4);
    writer.setStartingRelationId(5);
    writer.setValidateData(true);
    writer.setDisableDatabaseConstraintsDuringWrite(true);
    writer.setDisableDatabaseIndexesDuringWrite(true);

    writer.open(ServicesDbTestUtils::getOsmApiDbUrl().toString());
    writer.write(ServicesDbTestUtils::createTestMap1());
    writer.close();

    TestUtils::verifyStdMatchesOutputIgnoreDate(
      "test-files/io/ServiceOsmApiDbBulkInserterTest/psql-offline-starting-ids.sql",
      outFile);

    verifyDatabaseOutputOfflineWithCustomStartingIds();
  }

  void runSqlFileOutputTest()
  {
    QDir().mkpath("test-output/io/ServiceOsmApiDbBulkInserterTest/");

    //init db
    ServicesDbTestUtils::deleteDataFromOsmApiTestDatabase();
    const QString scriptDir = "test-files/servicesdb";
    ApiDb::execSqlFile(ServicesDbTestUtils::getOsmApiDbUrl().toString(), scriptDir + "/users.sql");

    OsmApiDbBulkInserter writer;
    const QString outFile =
      "test-output/io/ServiceOsmApiDbBulkInserterTest/sql-file-out.sql";
    writer.setReserveRecordIdsBeforeWritingData(false);
    writer.setStatusUpdateInterval(1);
    writer.setChangesetUserId(1);
    writer.setMaxChangesetSize(5);
    writer.setFileOutputElementBufferSize(3);
    writer.setValidateData(true);

    writer.open(outFile);
    writer.write(ServicesDbTestUtils::createTestMap1());
    writer.close();

    TestUtils::verifyStdMatchesOutputIgnoreDate(
      "test-files/io/ServiceOsmApiDbBulkInserterTest/psql-offline.sql", outFile);
    ServicesDbTestUtils::verifyTestDatabaseEmpty();
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ServiceOsmApiDbBulkInserterTest, "slow");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ServiceOsmApiDbBulkInserterTest, "serial");

}
