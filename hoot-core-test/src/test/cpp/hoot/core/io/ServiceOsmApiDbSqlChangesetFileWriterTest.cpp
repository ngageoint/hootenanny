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
 * @copyright Copyright (C) 2012, 2013, 2014, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/io/ServicesDbTestUtils.h>
#include <hoot/core/algorithms/changeset/TestOsmChangesetProvider.h>
#include <hoot/core/io/OsmApiDbSqlChangesetFileWriter.h>

namespace hoot
{

class ServiceOsmApiDbSqlChangesetFileWriterTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(ServiceOsmApiDbSqlChangesetFileWriterTest);
  CPPUNIT_TEST(runBasicTest);
  CPPUNIT_TEST_SUITE_END();

public:

  OsmApiDb database;

  const QString _scriptDir = "test-files/servicesdb/";

  ServiceOsmApiDbSqlChangesetFileWriterTest()
    : HootTestFixture("test-files/io/ServiceOsmApiDbSqlChangesetFileWriterTest/",
                      "test-output/io/ServiceOsmApiDbSqlChangesetFileWriterTest/")
  {
  }

  virtual void tearDown()
  {
    database.open(ServicesDbTestUtils::getOsmApiDbUrl());
    database.deleteData();
    database.close();
  }

  void runBasicTest()
  {
    std::shared_ptr<ChangesetProvider> changesetProvider(new TestOsmChangesetProvider(true));

    //clear out the db so we get consistent next id results
    database.open(ServicesDbTestUtils::getOsmApiDbUrl());
    database.deleteData();
    ApiDb::execSqlFile(ServicesDbTestUtils::getOsmApiDbUrl().toString(), _scriptDir + "users.sql");

    OsmApiDbSqlChangesetFileWriter writer(ServicesDbTestUtils::getOsmApiDbUrl());
    writer.setChangesetUserId(1);
    writer.write(_outputPath + "changeset.osc.sql", changesetProvider);
    HOOT_FILE_EQUALS(_inputPath + "changeset.osc.sql",
                    _outputPath + "changeset.osc.sql");
  }
};

#ifdef HOOT_HAVE_SERVICES
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ServiceOsmApiDbSqlChangesetFileWriterTest, "quick");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ServiceOsmApiDbSqlChangesetFileWriterTest, "serial");
#endif  // HOOT_HAVE_SERVICES

}
