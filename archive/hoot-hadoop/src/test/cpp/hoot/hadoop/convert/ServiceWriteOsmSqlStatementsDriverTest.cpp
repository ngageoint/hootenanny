/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

// Pretty Pipes
#include <pp/Hdfs.h>
#include <pp/mapreduce/Job.h>
using namespace pp;

// Qt
#include <QDir>

#include <hoot/core/TestUtils.h>
#include <hoot/hadoop/convert/WriteOsmSqlStatementsDriver.h>
#include <hoot/core/io/OsmApiDbReader.h>
#include <hoot/core/io/OsmApiDbBulkInserter.h>
#include <hoot/core/io/ServicesDbTestUtils.h>
#include <hoot/core/util/DbUtils.h>

#include "../MapReduceTestFixture.h"

using namespace std;

namespace hoot
{

class ServiceWriteOsmSqlStatementsDriverTest : public MapReduceTestFixture
{
  CPPUNIT_TEST_SUITE(ServiceWriteOsmSqlStatementsDriverTest);
  CPPUNIT_TEST(testSqlFileOutputNoBuffering);
  CPPUNIT_TEST(testDatabaseOutputNoBuffering);
  CPPUNIT_TEST(testSqlFileOutputWithBuffering);
  CPPUNIT_TEST(testDatabaseOutputWithBuffering);
  CPPUNIT_TEST(testDatabaseOutputExecSqlWithMapreduce);
  CPPUNIT_TEST_SUITE_END();

public:

  void verifyDatabaseOutput()
  {
    OsmApiDbReader reader;
    OsmMapPtr map(new OsmMap());
    reader.open(ServicesDbTestUtils::getOsmApiDbUrl().toString());
    reader.read(map);

    //we're already validating the sql output file, so just basic db row count validation here

    //verify current elements
    CPPUNIT_ASSERT_EQUAL((size_t)117, map->getNodes().size());
    CPPUNIT_ASSERT_EQUAL((size_t)14, map->getWays().size());
    CPPUNIT_ASSERT_EQUAL((size_t)0, map->getRelations().size());

    CPPUNIT_ASSERT_EQUAL(
      (long)38,
      DbUtils::getRowCount(reader._getDatabase()->getDB(), ApiDb::getCurrentNodeTagsTableName()));
    CPPUNIT_ASSERT_EQUAL(
      (long)39,
      DbUtils::getRowCount(reader._getDatabase()->getDB(), ApiDb::getCurrentWayTagsTableName()));
    CPPUNIT_ASSERT_EQUAL(
      (long)121,
      DbUtils::getRowCount(reader._getDatabase()->getDB(), ApiDb::getCurrentWayNodesTableName()));
    CPPUNIT_ASSERT_EQUAL(
      (long)0,
      DbUtils::getRowCount(reader._getDatabase()->getDB(), ApiDb::getCurrentRelationTagsTableName()));
    CPPUNIT_ASSERT_EQUAL(
      (long)0,
      DbUtils::getRowCount(reader._getDatabase()->getDB(),
      ApiDb::getCurrentRelationMembersTableName()));

    //verify historical element table sizes
    CPPUNIT_ASSERT_EQUAL(
      (long)117,
      DbUtils::getRowCount(reader._getDatabase()->getDB(), ApiDb::getNodesTableName()));
    CPPUNIT_ASSERT_EQUAL(
      (long)38,
      DbUtils::getRowCount(reader._getDatabase()->getDB(), ApiDb::getNodeTagsTableName()));
    CPPUNIT_ASSERT_EQUAL(
      (long)14,
      DbUtils::getRowCount(reader._getDatabase()->getDB(), ApiDb::getWaysTableName()));
    CPPUNIT_ASSERT_EQUAL(
      (long)39,
      DbUtils::getRowCount(reader._getDatabase()->getDB(), ApiDb::getWayTagsTableName()));
    CPPUNIT_ASSERT_EQUAL(
      (long)121,
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
      (long)1,
      DbUtils::getRowCount(reader._getDatabase()->getDB(), ApiDb::getChangesetsTableName()));

    //verify sequences - sequences can't be updated b/c of a chicken egg situation with nextval; sql
    //file validation will have to be good enough
//    boost::shared_ptr<OsmApiDb> osmApiDb = boost::dynamic_pointer_cast<OsmApiDb>(reader._getDatabase());
//    CPPUNIT_ASSERT_EQUAL((long)118, osmApiDb->getNextId(ElementType::Node));
//    CPPUNIT_ASSERT_EQUAL((long)15, osmApiDb->getNextId(ElementType::Way));
//    CPPUNIT_ASSERT_EQUAL((long)1, osmApiDb->getNextId(ElementType::Relation));
//    CPPUNIT_ASSERT_EQUAL((long)2, osmApiDb->getNextId(ApiDb::getChangesetsTableName()));

    reader.close();
  }

  void init(const string& outDir, const QString outFile)
  {
    if (QFile::exists(outFile))
    {
      QFile::remove(outFile);
    }
    Hdfs fs;
    if (fs.exists(outDir))
    {
      fs.deletePath(outDir, true);
    }
    QDir().mkpath(QString::fromStdString(outDir));
    fs.copyFromLocal(
      /*"test-files/DcGisRoads.pbf"*/
      "test-files/conflate/unified/AllDataTypesA.osm.pbf",
      outDir + "/input.osm.pbf");

    //init db
    ServicesDbTestUtils::deleteDataFromOsmApiTestDatabase();
    const QString scriptDir = "test-files/servicesdb";
    ApiDb::execSqlFile(ServicesDbTestUtils::getOsmApiDbUrl().toString(), scriptDir + "/users.sql");
  }

  void writeAdditionalNewRecords()
  {
    OsmApiDbBulkInserter writer;
    writer.setReserveRecordIdsBeforeWritingData(false);
    writer.setChangesetUserId(1);
    writer.setReserveRecordIdsBeforeWritingData(true);
    writer.setValidateData(true);
    writer.open(ServicesDbTestUtils::getOsmApiDbUrl().toString());
    writer.write(ServicesDbTestUtils::createTestMap1());
    writer.close();
  }

  void testSqlFileOutputNoBuffering()
  {
    const string outDir =
      "test-output/hadoop/convert/WriteOsmSqlStatementsDriverTest-testSqlFileOutput-no-buffering";
    const QString outFile = QString::fromStdString(outDir) + "/output.sql";
    init(outDir, outFile);

    WriteOsmSqlStatementsDriver driver;
    //set this much higher than the total element size of the map to avoid buffering
    driver.setFileOutputElementBufferSize(10000);
    driver.setChangesetUserId(1);
    driver.open(outFile);
    driver.write(QString::fromStdString(outDir) + "/input.osm.pbf");
    driver.close();

    TestUtils::verifyStdMatchesOutputIgnoreDate(
      "test-files/hadoop/convert/WriteOsmSqlStatementsDriverTest/output-no-buffering.sql", outFile);
    ServicesDbTestUtils::verifyTestDatabaseEmpty();
  }

  void testDatabaseOutputNoBuffering()
  {
    const string outDir =
      "test-output/hadoop/convert/WriteOsmSqlStatementsDriverTest-testDatabaseOutput-no-buffering";
    const QString outFile = QString::fromStdString(outDir) + "/output.sql";
    init(outDir, outFile);

    WriteOsmSqlStatementsDriver driver;
    //set this much higher than the total element size of the map to avoid buffering
    driver.setFileOutputElementBufferSize(10000);
    driver.setChangesetUserId(1);
    driver.setOutputFilesCopyLocation(outFile);
    driver.setExecSqlWithMapreduce(false);
    driver.open(ServicesDbTestUtils::getOsmApiDbUrl().toString());
    driver.write(QString::fromStdString(outDir) + "/input.osm.pbf");
    driver.close();

    TestUtils::verifyStdMatchesOutputIgnoreDate(
      "test-files/hadoop/convert/WriteOsmSqlStatementsDriverTest/output-no-buffering.sql", outFile);
    verifyDatabaseOutput();
    //let's write some new records just to make sure we didn't mess the constraints up; only
    //checking for errors here and not reading back this data after its written
    writeAdditionalNewRecords();
  }

  void testSqlFileOutputWithBuffering()
  {
    const string outDir =
      "test-output/hadoop/convert/WriteOsmSqlStatementsDriverTest-testSqlFileOutput-with-buffering";
    const QString outFile = QString::fromStdString(outDir) + "/output.sql";
    init(outDir, outFile);

    WriteOsmSqlStatementsDriver driver;
    driver.setFileOutputElementBufferSize(10);
    driver.setChangesetUserId(1);
    driver.setExecSqlWithMapreduce(false);
    driver.open(outFile);
    driver.write(QString::fromStdString(outDir) + "/input.osm.pbf");
    driver.close();

    TestUtils::verifyStdMatchesOutputIgnoreDate(
      "test-files/hadoop/convert/WriteOsmSqlStatementsDriverTest/output-with-buffering.sql",
      outFile);
    ServicesDbTestUtils::verifyTestDatabaseEmpty();
  }

  void testDatabaseOutputWithBuffering()
  {
    const string outDir =
      "test-output/hadoop/convert/WriteOsmSqlStatementsDriverTest-testDatabaseOutput-with-buffering";
    const QString outFile = QString::fromStdString(outDir) + "/output.sql";
    init(outDir, outFile);

    WriteOsmSqlStatementsDriver driver;
    driver.setFileOutputElementBufferSize(10);
    driver.setChangesetUserId(1);
    driver.setOutputFilesCopyLocation(outFile);
    driver.setExecSqlWithMapreduce(false);
    driver.open(ServicesDbTestUtils::getOsmApiDbUrl().toString());
    driver.write(QString::fromStdString(outDir) + "/input.osm.pbf");
    driver.close();

    TestUtils::verifyStdMatchesOutputIgnoreDate(
      "test-files/hadoop/convert/WriteOsmSqlStatementsDriverTest/output-with-buffering.sql",
      outFile);
    verifyDatabaseOutput();
    //let's write some new records just to make sure we didn't mess the constraints up; only
    //checking for errors here and not reading back this data after its written
    writeAdditionalNewRecords();
  }

  void testDatabaseOutputExecSqlWithMapreduce()
  {
    const string outDir =
      "test-output/hadoop/convert/WriteOsmSqlStatementsDriverTest-testDatabaseOutput-exec-sql-with-mapreduce";
    const QString outFile = QString::fromStdString(outDir) + "/output.sql";
    init(outDir, outFile);

    WriteOsmSqlStatementsDriver driver;
    driver.setFileOutputElementBufferSize(10);
    driver.setChangesetUserId(1);
    driver.setOutputFilesCopyLocation(outFile);
    driver.setExecSqlWithMapreduce(true);
    driver.open(ServicesDbTestUtils::getOsmApiDbUrl().toString());
    driver.write(QString::fromStdString(outDir) + "/input.osm.pbf");
    driver.close();

    TestUtils::verifyStdMatchesOutputIgnoreDate(
      "test-files/hadoop/convert/WriteOsmSqlStatementsDriverTest/output-with-buffering.sql",
      outFile);
    verifyDatabaseOutput();
    //let's write some new records just to make sure we didn't mess the constraints up; only
    //checking for errors here and not reading back this data after its written
    writeAdditionalNewRecords();
  }
};

}

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(hoot::ServiceWriteOsmSqlStatementsDriverTest, "glacial");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(hoot::ServiceWriteOsmSqlStatementsDriverTest, "serial");

