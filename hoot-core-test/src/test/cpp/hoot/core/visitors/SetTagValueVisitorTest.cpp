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
 * @copyright Copyright (C) 2014, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/visitors/SetTagValueVisitor.h>

namespace hoot
{

class SetTagValueVisitorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(SetTagValueVisitorTest);
  CPPUNIT_TEST(runAddNewTest);
  CPPUNIT_TEST(runOverwriteExistingTest);
  CPPUNIT_TEST(runAppendValueTest);
  CPPUNIT_TEST(runFilterTest);
  CPPUNIT_TEST(runOverwriteDisabledTest);
  CPPUNIT_TEST(runNegatedFilterTest);
  CPPUNIT_TEST_SUITE_END();

public:

  SetTagValueVisitorTest()
  {
    setResetType(ResetBasic);
    TestUtils::mkpath("test-output/visitors");
  }

  void runAddNewTest()
  {
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map, "test-files/visitors/SetTagValueVisitorTest.osm", false, Status::Unknown1);

    SetTagValueVisitor visitor("key3", "value3");
    map->visitRw(visitor);

    OsmMapWriterFactory::write(map,
      "test-output/visitors/SetTagValueVisitorTest-runAddNewTest.osm");

    HOOT_FILE_EQUALS(
      "test-files/visitors/SetTagValueVisitorTest-runAddNewTest.osm",
      "test-output/visitors/SetTagValueVisitorTest-runAddNewTest.osm");
  }

  void runOverwriteExistingTest()
  {
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map, "test-files/visitors/SetTagValueVisitorTest.osm", false, Status::Unknown1);

    SetTagValueVisitor visitor("key2", "value2");
    map->visitRw(visitor);

    OsmMapWriterFactory::write(map,
      "test-output/visitors/SetTagValueVisitorTest-runOverwriteExistingTest.osm");

    HOOT_FILE_EQUALS(
      "test-files/visitors/SetTagValueVisitorTest-runOverwriteExistingTest.osm",
      "test-output/visitors/SetTagValueVisitorTest-runOverwriteExistingTest.osm");
  }

  void runAppendValueTest()
  {
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map, "test-files/visitors/SetTagValueVisitorTest.osm", false, Status::Unknown1);

    SetTagValueVisitor visitor("key1", "value1b", true);
    map->visitRw(visitor);

    OsmMapWriterFactory::write(map,
      "test-output/visitors/SetTagValueVisitorTest-runAppendValueTest.osm");

    HOOT_FILE_EQUALS(
      "test-files/visitors/SetTagValueVisitorTest-runAppendValueTest.osm",
      "test-output/visitors/SetTagValueVisitorTest-runAppendValueTest.osm");
  }

  void runFilterTest()
  {
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map, "test-files/visitors/SetTagValueVisitorTest.osm", false, Status::Unknown1);

    SetTagValueVisitor visitor("key3", "value3", false, "hoot::NodeCriterion");
    map->visitRw(visitor);

    OsmMapWriterFactory::write(map,
      "test-output/visitors/SetTagValueVisitorTest-runFilterTest.osm");

    HOOT_FILE_EQUALS(
      "test-files/visitors/SetTagValueVisitorTest-runFilterTest.osm",
      "test-output/visitors/SetTagValueVisitorTest-runFilterTest.osm");
  }

  void runOverwriteDisabledTest()
  {
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map, "test-files/visitors/SetTagValueVisitorTest.osm", false, Status::Unknown1);

    //We've disabled overwriting existing tags, so the tag with key="key2" should not be updated
    //on any element that already has a tag with the key.  It should only be added to elements
    //that don't have a tag with that key.
    SetTagValueVisitor visitor("key2", "updatedValue", false, "", false);
    map->visitRw(visitor);

    OsmMapWriterFactory::write(map,
      "test-output/visitors/SetTagValueVisitorTest-runOverwriteDisabledTest.osm");

    HOOT_FILE_EQUALS(
      "test-files/visitors/SetTagValueVisitorTest-runOverwriteDisabledTest.osm",
      "test-output/visitors/SetTagValueVisitorTest-runOverwriteDisabledTest.osm");
  }

  void runNegatedFilterTest()
  {
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map, "test-files/visitors/SetTagValueVisitorTest.osm", false, Status::Unknown1);

    SetTagValueVisitor visitor("key3", "value3", false, "hoot::NodeCriterion", false, true);
    map->visitRw(visitor);

    OsmMapWriterFactory::write(map,
      "test-output/visitors/SetTagValueVisitorTest-runNegatedFilterTest.osm");

    HOOT_FILE_EQUALS(
      "test-files/visitors/SetTagValueVisitorTest-runNegatedFilterTest.osm",
      "test-output/visitors/SetTagValueVisitorTest-runNegatedFilterTest.osm");
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(SetTagValueVisitorTest, "quick");

}


