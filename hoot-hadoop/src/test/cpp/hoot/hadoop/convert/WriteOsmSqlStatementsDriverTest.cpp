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
#include <hoot/core/io/OsmApiDb.h>
#include <hoot/core/io/ServicesDbTestUtils.h>
#include <hoot/core/util/DbUtils.h>

#include "../MapReduceTestFixture.h"

namespace hoot
{

class WriteOsmSqlStatementsDriverTest : public MapReduceTestFixture
{
  CPPUNIT_TEST_SUITE(WriteOsmSqlStatementsDriverTest);
  CPPUNIT_TEST(testSqlFileOutput);
  CPPUNIT_TEST_SUITE_END();

public:

  void verifyDatabaseOutput()
  {
    OsmApiDbReader reader;
    OsmMapPtr map(new OsmMap());
    reader.open(ServicesDbTestUtils::getOsmApiDbUrl().toString());
    reader.read(map);

    //we're already validating the sql output file, so just doing minimal db validation here

    //verify current elements
    CPPUNIT_ASSERT_EQUAL((size_t)117, map->getNodes().size());
    CPPUNIT_ASSERT_EQUAL((size_t)14, map->getWays().size());
    CPPUNIT_ASSERT_EQUAL((size_t)0, map->getRelations().size());

    reader.close();
  }

  void testSqlFileOutput()
  {
    const string outDir = "test-output/hadoop/convert/WriteOsmSqlStatementsDriverTest";
    const QString outFile = QString::fromStdString(outDir) + "/output.sql";
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
    ServicesDbTestUtils::execOsmApiDbSqlTestScript("users.sql");

    WriteOsmSqlStatementsDriver driver;
    driver.setWriteBufferSize(10);
    driver.write(QString::fromStdString(outDir) + "/input.osm.pbf", outFile);

    TestUtils::verifyStdMatchesOutputIgnoreDate(
      "test-files/hadoop/convert/WriteOsmSqlStatementsDriverTest/output.sql", outFile);

    //even though only sql file output was specified, we're still going to try to write this
    //to a db to see that its valid for now

    OsmApiDb database;
    database.open(ServicesDbTestUtils::getOsmApiDbUrl().toString());
    //We have to turn off constraints before writing the sql file to the db, since the table
    //copy commands are out of order and will violate ref integrity.
    database.disableConstraints();

    //write the sql file
    ApiDb::execSqlFile(ServicesDbTestUtils::getOsmApiDbUrl().toString(), outFile);

    //now re-enable the constraints to make sure the db is valid before reading from it
    database.enableConstraints();
    database.close();

    ServicesDbTestUtils::verifyTestDatabaseEmpty();
  }
};

}

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(hoot::WriteOsmSqlStatementsDriverTest, "glacial");

