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
 * @copyright Copyright (C) 2014, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/visitors/SetTagVisitor.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>

#include "../TestUtils.h"

// Qt
#include <QDir>

namespace hoot
{

class SetTagVisitorTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(SetTagVisitorTest);
  CPPUNIT_TEST(runAddNewTest);
  CPPUNIT_TEST(runOverwriteExistingTest);
  CPPUNIT_TEST(runAppendValueTest);
  CPPUNIT_TEST(runElementFilterTest);
  CPPUNIT_TEST(runBadElementTypeTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runAddNewTest()
  {
    OsmMapPtr map(new OsmMap());
    OsmMap::resetCounters();
    OsmMapReaderFactory::read(
      map, "test-files/visitors/SetTagVisitorTest.osm", false, Status::Unknown1);

    SetTagVisitor visitor("key3", "value3");
    map->visitRw(visitor);

    QDir().mkpath("test-output/visitors");
    OsmMapWriterFactory::getInstance().write(map,
      "test-output/visitors/SetTagVisitorTest-runAddNewTest.osm");

    HOOT_FILE_EQUALS(
      "test-files/visitors/SetTagVisitorTest-runAddNewTest.osm",
      "test-output/visitors/SetTagVisitorTest-runAddNewTest.osm");
  }

  void runOverwriteExistingTest()
  {
    OsmMapPtr map(new OsmMap());
    OsmMap::resetCounters();
    OsmMapReaderFactory::read(
      map, "test-files/visitors/SetTagVisitorTest.osm", false, Status::Unknown1);

    SetTagVisitor visitor("key2", "value2");
    map->visitRw(visitor);

    QDir().mkpath("test-output/visitors");
    OsmMapWriterFactory::getInstance().write(map,
      "test-output/visitors/SetTagVisitorTest-runOverwriteExistingTest.osm");

    HOOT_FILE_EQUALS(
      "test-files/visitors/SetTagVisitorTest-runOverwriteExistingTest.osm",
      "test-output/visitors/SetTagVisitorTest-runOverwriteExistingTest.osm");
  }

  void runAppendValueTest()
  {
    OsmMapPtr map(new OsmMap());
    OsmMap::resetCounters();
    OsmMapReaderFactory::read(
      map, "test-files/visitors/SetTagVisitorTest.osm", false, Status::Unknown1);

    SetTagVisitor visitor("key1", "value1b", true);
    map->visitRw(visitor);

    QDir().mkpath("test-output/visitors");
    OsmMapWriterFactory::getInstance().write(map,
      "test-output/visitors/SetTagVisitorTest-runAppendValueTest.osm");

    HOOT_FILE_EQUALS(
      "test-files/visitors/SetTagVisitorTest-runAppendValueTest.osm",
      "test-output/visitors/SetTagVisitorTest-runAppendValueTest.osm");
  }

  void runElementFilterTest()
  {
    OsmMapPtr map(new OsmMap());
    OsmMap::resetCounters();
    OsmMapReaderFactory::read(
      map, "test-files/visitors/SetTagVisitorTest.osm", false, Status::Unknown1);

    SetTagVisitor visitor("key3", "value3", false, ElementType::fromString("node"));
    map->visitRw(visitor);

    QDir().mkpath("test-output/visitors");
    OsmMapWriterFactory::getInstance().write(map,
      "test-output/visitors/SetTagVisitorTest-runElementFilterTest.osm");

    HOOT_FILE_EQUALS(
      "test-files/visitors/SetTagVisitorTest-runElementFilterTest.osm",
      "test-output/visitors/SetTagVisitorTest-runElementFilterTest.osm");
  }

  void runBadElementTypeTest()
  {
    QString exceptionMsg("");
    try
    {
      SetTagVisitor visitor("key1", "value1", false, ElementType::fromString("blah"));
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }

    CPPUNIT_ASSERT(exceptionMsg.contains("Invalid element type string"));
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(SetTagVisitorTest, "quick");

}


