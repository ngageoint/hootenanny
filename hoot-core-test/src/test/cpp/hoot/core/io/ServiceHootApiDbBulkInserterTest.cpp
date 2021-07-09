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
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2013, 2014, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
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
  CPPUNIT_TEST_SUITE_END();

public:

  QString userEmail()
  { return QString("%1.ServiceHootApiDbBulkInserterTest@hoottestcpp.org").arg(_testName); }
  QString userName()  { return QString("%1.ServiceHootApiDbBulkInserterTest").arg(_testName); }

  ServiceHootApiDbBulkInserterTest()
    : HootTestFixture("test-files/io/ServiceHootApiDbBulkInserterTest/",
                      "test-output/io/ServiceHootApiDbBulkInserterTest/")
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
      database.open(ServicesDbTestUtils::getDbModifyUrl().toString());
      database.deleteMap(_mapId);
      database.close();
    }
  }

  void runPsqlDbOfflineTest()
  {
    setUpTest("runPsqlDbOfflineTest");

    HootApiDbBulkInserter writer;
    const QString outFile = _outputPath + "psql-offline-out.sql";
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
    OsmMapPtr actualMap = std::make_shared<OsmMap>();
    reader.setUserEmail(userEmail());
    reader.open(ServicesDbTestUtils::getDbModifyUrl(_testName).toString());
    reader.read(actualMap);
    reader.close();
    const QString actualOutputFile = _outputPath + "psqlOffline-out.osm";
    std::shared_ptr<OsmMapWriter> actualMapWriter =
      OsmMapWriterFactory::createWriter(actualOutputFile);
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

    // TODO: This needs to be enabled (or some check similar to it), but doing so will require
    //ignoring the map and changeset ID's in the file that will change with each test run.
//    TestUtils::verifyStdMatchesOutputIgnoreDate(
//      "test-files/io/ServiceHootApiDbBulkInserterTest/psql-offline.sql",
//      outFile);
    HOOT_FILE_EQUALS(_inputPath + "psqlOffline.osm", actualOutputFile);
  }

private:

  long _mapId;
  QString _testName;
};

#ifdef HOOT_HAVE_SERVICES
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ServiceHootApiDbBulkInserterTest, "slow");
#endif  // HOOT_HAVE_SERVICES

}
