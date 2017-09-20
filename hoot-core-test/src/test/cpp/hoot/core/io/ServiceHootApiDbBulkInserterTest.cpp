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
#include <hoot/core/io/HootApiDbBulkInserter.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/io/HootApiDbReader.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/util/DbUtils.h>

// Qt
#include <QDir>
#include <QFileInfo>

#include "../TestUtils.h"
#include "ServicesDbTestUtils.h"

namespace hoot
{

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

  void verifyDatabaseOutputOffline()
  {
    HootApiDbReader reader;
    OsmMapPtr map(new OsmMap());
    reader.setUserEmail("ServiceHootApiDbBulkInserterTest@hoot.org");
    reader.open(ServicesDbTestUtils::getDbModifyUrl().toString());
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

    //verify changeset table size
    //TODO: fix
    CPPUNIT_ASSERT_EQUAL(
      (long)4,
      DbUtils::getRowCount(reader._getDatabase()->getDB(), HootApiDb::getChangesetsTableName(mapId)));

    reader.close();
  }

  void runPsqlDbOfflineTest()
  {
    QDir().mkpath("test-output/io/ServiceHootApiDbBulkInserterTest/");

    HootApiDbBulkInserter writer;
    const QString outFile = "test-output/io/ServiceHootApiDbBulkInserterTest/psql-offline-out.sql";
    writer.setOutputFilesCopyLocation(outFile);
    writer.setStatusUpdateInterval(1);
    writer.setChangesetUserId(1);
    writer.setMaxChangesetSize(5);
    writer.setFileOutputElementBufferSize(3);
    writer.setValidateData(true);
    writer.setCreateUser(true);
    writer.setOverwriteMap(true);
    writer.setUserEmail("ServiceHootApiDbBulkInserterTest@hoot.org");
    writer.setFastBulkInsertActivated(true);

    writer.open(ServicesDbTestUtils::getDbModifyUrl().toString());
    writer.write(ServicesDbTestUtils::createTestMap1());
    writer.close();
    mapId = writer.getMapId();

    verifyDatabaseOutputOffline();
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ServiceHootApiDbBulkInserterTest, "slow");

}
