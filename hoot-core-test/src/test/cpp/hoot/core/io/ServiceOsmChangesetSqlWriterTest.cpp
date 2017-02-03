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
 * @copyright Copyright (C) 2012, 2013, 2014 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include "TestOsmChangesetProvider.h"
#include "ServicesDbTestUtils.h"

#include <hoot/core/io/OsmChangesetSqlWriter.h>

// Boost
using namespace boost;

// Qt
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QDir>

#include "../TestUtils.h"

namespace hoot
{

class ServiceOsmApiDbChangesetSqlWriterTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(ServiceOsmApiDbChangesetSqlWriterTest);
    CPPUNIT_TEST(runBasicTest);
    CPPUNIT_TEST(runSplitTest);
    CPPUNIT_TEST_SUITE_END();

public:

  OsmApiDb database;

  void tearDown()
  {
    database.open(ServicesDbTestUtils::getOsmApiDbUrl());
    database.deleteData();
    database.close();
  }

  void runBasicTest()
  {
    QDir().mkpath("test-output/io/ServiceOsmApiDbChangesetSqlWriterTest");
    shared_ptr<ChangeSetProvider> changesetProvider(new TestOsmChangesetProvider(true));

    //clear out the db so we get consistent next id results
    database.open(ServicesDbTestUtils::getOsmApiDbUrl());
    database.deleteData();
    ServicesDbTestUtils::execOsmApiDbSqlTestScript("users.sql");

    OsmChangesetSqlWriter writer(ServicesDbTestUtils::getOsmApiDbUrl());
    writer
      .write(
        "test-output/io/ServiceOsmApiDbChangesetSqlWriterTest/changeset.osc.sql",
        changesetProvider);
    HOOT_STR_EQUALS(
      TestUtils::readFile("test-files/io/ServiceOsmApiDbChangesetSqlWriterTest/changeset.osc.sql"),
      TestUtils::readFile("test-output/io/ServiceOsmApiDbChangesetSqlWriterTest/changeset.osc.sql"));
  }

  void runSplitTest()
  {
    QDir().mkpath("test-output/io/ServiceOsmApiDbChangesetSqlWriterTest");
    shared_ptr<ChangeSetProvider> changesetProvider(new TestOsmChangesetProvider(true));

    //clear out the db so we get consistent next id results
    database.open(ServicesDbTestUtils::getOsmApiDbUrl());
    database.deleteData();
    ServicesDbTestUtils::execOsmApiDbSqlTestScript("users.sql");

    OsmChangesetSqlWriter writer(ServicesDbTestUtils::getOsmApiDbUrl());
    //  Set the changeset max size to 5 (half of the changes) for this test only
    Settings testSettings = conf();
    testSettings.set("changeset.max.size", "5");
    writer.setConfiguration(testSettings);
    writer
      .write(
        "test-output/io/ServiceOsmApiDbChangesetSqlWriterTest/changeset.split.osc.sql",
        changesetProvider);
    HOOT_STR_EQUALS(
      TestUtils::readFile("test-files/io/ServiceOsmApiDbChangesetSqlWriterTest/changeset.split.osc.sql"),
      TestUtils::readFile("test-output/io/ServiceOsmApiDbChangesetSqlWriterTest/changeset.split.osc.sql"));
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ServiceOsmApiDbChangesetSqlWriterTest, "quick");

}
