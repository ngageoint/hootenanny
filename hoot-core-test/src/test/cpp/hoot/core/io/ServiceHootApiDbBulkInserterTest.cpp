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
#include <hoot/core/io/HootApiDbBulkInserter.h>
#include <hoot/core/io/HootApiDbReader.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/util/FileUtils.h>

// Qt
#include <QDir>

#include "../TestUtils.h"
#include "ServicesDbTestUtils.h"

namespace hoot
{

/*
 * There are some unused and untested features here that HootApiDbBulkWriter inherited from
 * OsmApiDbBulkWriter.  Rather than add tests for the unneeded features, its a better idea to
 * rework the inheritance of the two classes as described in HootApiDbBulkWriter to get rid of the
 * unneeded functionality in HootApiDbBulkWriter.
 */
class ServiceHootApiDbBulkInserterTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(ServiceHootApiDbBulkInserterTest);
  CPPUNIT_TEST(runPsqlDbOfflineTest);
  CPPUNIT_TEST_SUITE_END();

public:

  static QString userEmail() { return "ServiceHootApiDbBulkInserterTest@hoottestcpp.org"; }

  long mapId;

  void setUp()
  {
    mapId = -1;
    ServicesDbTestUtils::deleteUser(userEmail());

    HootApiDb database;
    database.open(ServicesDbTestUtils::getDbModifyUrl());
    database.getOrCreateUser(userEmail(), "ServiceHootApiDbBulkInserterTest");
    database.close();

    TestUtils::mkpath("test-output/io/ServiceHootApiDbBulkInserterTest");
  }

  void tearDown()
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
    QString testName = "runPsqlDbOfflineTest";
    OsmMap::resetCounters();
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

    writer.open(ServicesDbTestUtils::getDbModifyUrl(testName).toString());
    writer.write(ServicesDbTestUtils::createTestMap1());
    writer.close();
    mapId = writer.getMapId();
    LOG_VARD(mapId);

    HootApiDbReader reader;
    OsmMapPtr actualMap(new OsmMap());
    reader.setUserEmail(userEmail());
    reader.open(ServicesDbTestUtils::getDbModifyUrl(testName).toString());
    reader.read(actualMap);
    reader.close();
    const QString actualOutputFile = outputDir + "/psqlOffline-out.osm";
    boost::shared_ptr<OsmMapWriter> actualMapWriter =
      OsmMapWriterFactory::getInstance().createWriter(actualOutputFile);
    actualMapWriter->open(actualOutputFile);
    actualMapWriter->write(actualMap);

    //Should be 4 changesets, but its 8.
    HootApiDb database;
    database.open(ServicesDbTestUtils::getDbModifyUrl(testName).toString());
    database.setMapId(mapId);
    database.setCreateIndexesOnClose(false);
    database.setFlushOnClose(false);
    const long numChangesets = database.numChangesets();
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
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ServiceHootApiDbBulkInserterTest, "slow");

}
