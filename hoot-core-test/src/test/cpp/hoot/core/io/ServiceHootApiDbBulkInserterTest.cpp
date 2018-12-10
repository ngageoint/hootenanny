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
  CPPUNIT_TEST(writeTwoMapsSameNameDifferentUsers);
  CPPUNIT_TEST(twoMapsSameNameSameUserOverwriteDisabledTest);
  CPPUNIT_TEST(twoMapsSameNameSameUserOverwriteEnabledTest);
  CPPUNIT_TEST_SUITE_END();

public:

  QString userEmail()
  { return QString("%1.ServiceHootApiDbBulkInserterTest@hoottestcpp.org").arg(_testName); }
  QString userName()  { return QString("%1.ServiceHootApiDbBulkInserterTest").arg(_testName); }

  ServiceHootApiDbBulkInserterTest()
  {
    setResetType(ResetBasic);
    TestUtils::mkpath("test-output/io/ServiceHootApiDbBulkInserterTest");
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
      database.open(ServicesDbTestUtils::getDbModifyUrl().toString());
      database.deleteMap(_mapId);
      database.close();
    }
  }

  void runPsqlDbOfflineTest()
  {
    setUpTest("runPsqlDbOfflineTest");
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

    writer.open(ServicesDbTestUtils::getDbModifyUrl(_testName).toString());
    writer.write(ServicesDbTestUtils::createTestMap1());
    writer.close();
    _mapId = writer.getMapId();
    LOG_VARD(_mapId);

    HootApiDbReader reader;
    OsmMapPtr actualMap(new OsmMap());
    reader.setUserEmail(userEmail());
    reader.open(ServicesDbTestUtils::getDbModifyUrl(_testName).toString());
    reader.read(actualMap);
    reader.close();
    const QString actualOutputFile = outputDir + "/psqlOffline-out.osm";
    boost::shared_ptr<OsmMapWriter> actualMapWriter =
      OsmMapWriterFactory::getInstance().createWriter(actualOutputFile);
    actualMapWriter->open(actualOutputFile);
    actualMapWriter->write(actualMap);

    //Should be 4 changesets, but its 8.
    HootApiDb database;
    database.open(ServicesDbTestUtils::getDbModifyUrl(_testName).toString());
    database.setMapId(_mapId);
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

  void writeTwoMapsSameNameDifferentUsers()
  {
    setUpTest("ServiceHootApiDbBulkInserterTest-writeTwoMapsSameNameDifferentUsers");
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
    writer.open(ServicesDbTestUtils::getDbModifyUrl(_testName).toString());
    writer.write(ServicesDbTestUtils::createTestMap1());
    writer.close();
    _mapId = writer.getMapId();

    // Insert another user
    HootApiDb db;
    db.open(ServicesDbTestUtils::getDbModifyUrl(_testName).toString());
    const QString differentUserEmail =
      "ServiceHootApiDbBulkInserterTest-writeTwoMapsSameNameDifferentUsers2";
    const long differentUserId = db.insertUser(differentUserEmail, differentUserEmail);
    db.close();

    // Configure another writer with the other user
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
    writer2.open(ServicesDbTestUtils::getDbModifyUrl(_testName).toString());
    // This should not fail, since we allow different users to write maps with the same name (just
    // checking that open succeeds here...not the actual write).

    db.open(ServicesDbTestUtils::getDbModifyUrl(_testName).toString());
    db.deleteUser(differentUserId);
  }

  void twoMapsSameNameSameUserOverwriteDisabledTest()
  {
    setUpTest("ServiceHootApiDbBulkInserterTest-twoMapsSameNameSameUserOverwriteDisabledTest");
    const QString outputDir = "test-output/io/ServiceHootApiDbBulkInserterTest";

    OsmMapPtr map(new OsmMap());
    NodePtr n1(new Node(Status::Unknown1, 1, 0.0, 0.0, 10.0));
    n1->setTag("note", "n1");
    map->addNode(n1);

    // write a map
    HootApiDbBulkInserter writer;
    const QString outFile = outputDir + "/twoMapsSameNameSameUserOverwriteDisabledTest-out.sql";
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
    writer.open(ServicesDbTestUtils::getDbModifyUrl(_testName).toString());
    writer.write(ServicesDbTestUtils::createTestMap1());
    writer.close();

    // try to write another map with the same name for the same user with overwrite disabled
    HootApiDbBulkInserter writer2;
    writer2.setOutputFilesCopyLocation(
      outputDir + "/twoMapsSameNameSameUserOverwriteDisabledTest2-out.sql");
    writer2.setStatusUpdateInterval(1);
    writer2.setChangesetUserId(1);
    writer2.setMaxChangesetSize(5);
    writer2.setFileOutputElementBufferSize(3);
    writer2.setValidateData(false);
    writer2.setCreateUser(true);
    writer2.setOverwriteMap(false);
    writer2.setUserEmail(userEmail());
    writer2.setCopyBulkInsertActivated(true);

    // It should fail b/c we don't allow multiple map with the same name to be owned by the same
    // user.
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
    CPPUNIT_ASSERT(exceptionMsg.contains("already has map with name"));
  }

  void twoMapsSameNameSameUserOverwriteEnabledTest()
  {
    setUpTest("ServiceHootApiDbBulkInserterTest-twoMapsSameNameSameUserOverwriteEnabledTest");
    const QString outputDir = "test-output/io/ServiceHootApiDbBulkInserterTest";

    OsmMapPtr map(new OsmMap());
    NodePtr n1(new Node(Status::Unknown1, 1, 0.0, 0.0, 10.0));
    n1->setTag("note", "n1");
    map->addNode(n1);

    // write a map
    HootApiDbBulkInserter writer;
    const QString outFile = outputDir + "/twoMapsSameNameSameUserOverwriteEnabledTest-out.sql";
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
    writer.open(ServicesDbTestUtils::getDbModifyUrl(_testName).toString());
    writer.write(ServicesDbTestUtils::createTestMap1());
    writer.close();

    // try to write another map with the same name for the same user with overwrite enabled
    HootApiDbBulkInserter writer2;
    writer2.setOutputFilesCopyLocation(
      outputDir + "/twoMapsSameNameSameUserOverwriteEnabledTest2-out.sql");
    writer2.setStatusUpdateInterval(1);
    writer2.setChangesetUserId(1);
    writer2.setMaxChangesetSize(5);
    writer2.setFileOutputElementBufferSize(3);
    writer2.setValidateData(false);
    writer2.setCreateUser(true);
    writer2.setOverwriteMap(true);
    writer2.setUserEmail(userEmail());
    writer2.setCopyBulkInsertActivated(true);

    //open should succeed here (just checking open, not the actual write)
    writer2.open(ServicesDbTestUtils::getDbModifyUrl(_testName).toString());
  }

private:

  long _mapId;
  QString _testName;
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ServiceHootApiDbBulkInserterTest, "slow");

}
