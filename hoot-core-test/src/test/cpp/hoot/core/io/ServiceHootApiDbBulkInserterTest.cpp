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
 * @copyright Copyright (C) 2013, 2014, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/io/HootApiDbBulkInserter.h>
#include <hoot/core/io/HootApiDbReader.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/io/ServicesDbTestUtils.h>
#include <hoot/core/util/FileUtils.h>

namespace hoot
{

/*
 * There are some unused and untested features here that HootApiDbBulkWriter inherited from
 * OsmApiDbBulkWriter.  Rather than add tests for the unneeded features, its a better idea to
 * rework the inheritance of the two classes as described in HootApiDbBulkWriter to get rid of the
 * unneeded functionality in HootApiDbBulkWriter.
 */
class ServiceHootApiDbBulkInserterTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(ServiceHootApiDbBulkInserterTest);
  CPPUNIT_TEST(runPsqlDbOfflineTest);
  CPPUNIT_TEST(overwriteDataWithDifferentUserTest);
  CPPUNIT_TEST_SUITE_END();

public:

  static QString userEmail() { return "ServiceHootApiDbBulkInserterTest@hoottestcpp.org"; }

  long mapId;

  ServiceHootApiDbBulkInserterTest()
  {
    setResetType(ResetBasic);
    TestUtils::mkpath("test-output/io/ServiceHootApiDbBulkInserterTest");
  }

  virtual void setUp()
  {
    HootTestFixture::setUp();
    mapId = -1;
    ServicesDbTestUtils::deleteUser(userEmail());

    HootApiDb database;
    database.open(ServicesDbTestUtils::getDbModifyUrl());
    database.getOrCreateUser(userEmail(), "ServiceHootApiDbBulkInserterTest");
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

  void runPsqlDbOfflineTest()
  {
    const QString outputDir = "test-output/io/ServiceHootApiDbBulkInserterTest";

    HootApiDbBulkInserter writer;
    const QString outFile = outputDir + "/psql-offline-out.sql";
    writer.setOutputFilesCopyLocation(outFile);
    writer.setStatusUpdateInterval(1);
    writer.setChangesetUserId(1);
    writer.setMaxChangesetSize(5);
    writer.setFileOutputElementBufferSize(3);
    writer.setValidateData(false);
    writer.setCreateUser(true);
    writer.setOverwriteMap(true);
    writer.setUserEmail(userEmail());
    writer.setCopyBulkInsertActivated(true);

    writer.open(ServicesDbTestUtils::getDbModifyUrl().toString());
    writer.write(ServicesDbTestUtils::createTestMap1());
    writer.close();
    mapId = writer.getMapId();
    LOG_VARD(mapId);

    HootApiDbReader reader;
    OsmMapPtr actualMap(new OsmMap());
    reader.setUserEmail(userEmail());
    reader.open(ServicesDbTestUtils::getDbModifyUrl().toString());
    reader.read(actualMap);
    reader.close();
    const QString actualOutputFile = outputDir + "/psqlOffline-out.osm";
    boost::shared_ptr<OsmMapWriter> actualMapWriter =
      OsmMapWriterFactory::getInstance().createWriter(actualOutputFile);
    actualMapWriter->open(actualOutputFile);
    actualMapWriter->write(actualMap);

    //Should be 4 changesets, but its 8.
    HootApiDb database;
    database.open(ServicesDbTestUtils::getDbModifyUrl().toString());
    database.setMapId(mapId);
    database.setCreateIndexesOnClose(false);
    database.setFlushOnClose(false);
    const long numChangesets = database.numChangesets();
    database.close();
    LOG_VARD(numChangesets);
    CPPUNIT_ASSERT_EQUAL(8L, numChangesets);

    //TODO: This needs to be enabled (or some check similar to it), but doing so will require
    //ignoring the map and changeset ID's in the file that will change with each test run.
//    TestUtils::verifyStdMatchesOutputIgnoreDate(
//      "test-files/io/ServiceHootApiDbBulkInserterTest/psql-offline.sql",
//      outFile);
    HOOT_FILE_EQUALS(
      "test-files/io/ServiceHootApiDbBulkInserterTest/psqlOffline.osm", actualOutputFile);
  }

  void overwriteDataWithDifferentUserTest()
  {
    const QString outputDir = "test-output/io/ServiceHootApiDbBulkInserterTest";

    // write a map
    HootApiDbBulkInserter writer;
    const QString outFile = outputDir + "/overwriteDataWithDifferentUserTest-out.sql";
    writer.setOutputFilesCopyLocation(outFile);
    writer.setStatusUpdateInterval(1);
    writer.setChangesetUserId(1);
    writer.setMaxChangesetSize(5);
    writer.setFileOutputElementBufferSize(3);
    writer.setValidateData(false);
    writer.setCreateUser(true);
    writer.setOverwriteMap(true);
    writer.setUserEmail(userEmail());
    writer.setCopyBulkInsertActivated(true);
    writer.open(ServicesDbTestUtils::getDbModifyUrl().toString());
    writer.write(ServicesDbTestUtils::createTestMap1());
    writer.close();
    mapId = writer.getMapId();

    // Insert another user
    HootApiDb db;
    db.open(ServicesDbTestUtils::getDbModifyUrl().toString());
    const QString differentUserEmail = "overwriteDataWithDifferentUserTest2";
    db.insertUser(differentUserEmail, differentUserEmail);
    db.close();

    // Configure another writer with the other user (had difficulties using the same writer for
    // both map inserts, but it should be possible)
    HootApiDbBulkInserter writer2;
    writer2.setOutputFilesCopyLocation(outputDir + "/overwriteDataWithDifferentUserTest2-out.sql");
    writer2.setStatusUpdateInterval(1);
    writer2.setChangesetUserId(1);
    writer2.setMaxChangesetSize(5);
    writer2.setFileOutputElementBufferSize(3);
    writer2.setValidateData(false);
    writer2.setCreateUser(true);
    writer2.setOverwriteMap(true);
    writer2.setUserEmail(differentUserEmail);
    writer2.setCopyBulkInsertActivated(true);

    // We shouldn't be able to overwrite the original user's data. (open overwrites if it finds
    // exisint maps with the same name
    QString exceptionMsg("");
    try
    {
      writer2.open(ServicesDbTestUtils::getDbModifyUrl().toString());
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
      writer2.close();
    }
    LOG_VARD(exceptionMsg);
    CPPUNIT_ASSERT(exceptionMsg.contains("does not have write access to map"));

    // Clean up the second user.
    db.open(ServicesDbTestUtils::getDbModifyUrl().toString());
    db.deleteUser(db.getUserId(differentUserEmail, true));
    db.close();
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ServiceHootApiDbBulkInserterTest, "slow");

}
