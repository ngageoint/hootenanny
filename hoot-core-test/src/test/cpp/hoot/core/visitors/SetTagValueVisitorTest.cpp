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
 * @copyright Copyright (C) 2014, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

// hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/elements/OsmMap.h>
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
    : HootTestFixture("test-files/visitors/SetTagValueVisitorTest/",
                      "test-output/visitors/SetTagValueVisitorTest/")
  {
  }

  void runAddNewTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    OsmMapReaderFactory::read(
      map, _inputPath + "SetTagValueVisitorTest.osm", false, Status::Unknown1);

    SetTagValueVisitor visitor("key3", "value3");
    map->visitRw(visitor);

    OsmMapWriterFactory::write(map, _outputPath + "RunAddNewTest.osm");

    HOOT_FILE_EQUALS( _inputPath + "RunAddNewTest.osm",
                     _outputPath + "RunAddNewTest.osm");
  }

  void runOverwriteExistingTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    OsmMapReaderFactory::read(
      map, _inputPath + "SetTagValueVisitorTest.osm", false, Status::Unknown1);

    SetTagValueVisitor visitor("key2", "value2");
    map->visitRw(visitor);

    OsmMapWriterFactory::write(map, _outputPath + "RunOverwriteExistingTest.osm");

    HOOT_FILE_EQUALS( _inputPath + "RunOverwriteExistingTest.osm",
                     _outputPath + "RunOverwriteExistingTest.osm");
  }

  void runAppendValueTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    OsmMapReaderFactory::read(
      map, _inputPath + "SetTagValueVisitorTest.osm", false, Status::Unknown1);

    SetTagValueVisitor visitor("key1", "value1b", true);
    map->visitRw(visitor);

    OsmMapWriterFactory::write(map, _outputPath + "RunAppendValueTest.osm");

    HOOT_FILE_EQUALS( _inputPath + "RunAppendValueTest.osm",
                     _outputPath + "RunAppendValueTest.osm");
  }

  void runFilterTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    OsmMapReaderFactory::read(
      map, _inputPath + "SetTagValueVisitorTest.osm", false, Status::Unknown1);

    SetTagValueVisitor visitor("key3", "value3", false, QStringList("NodeCriterion"));
    map->visitRw(visitor);

    OsmMapWriterFactory::write(map, _outputPath + "RunFilterTest.osm");

    HOOT_FILE_EQUALS( _inputPath + "RunFilterTest.osm",
                     _outputPath + "RunFilterTest.osm");
  }

  void runOverwriteDisabledTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    OsmMapReaderFactory::read(
      map, _inputPath + "SetTagValueVisitorTest.osm", false, Status::Unknown1);

    //We've disabled overwriting existing tags, so the tag with key="key2" should not be updated
    //on any element that already has a tag with the key.  It should only be added to elements
    //that don't have a tag with that key.
    SetTagValueVisitor visitor("key2", "updatedValue", false, QStringList(), false);
    map->visitRw(visitor);

    OsmMapWriterFactory::write(map, _outputPath + "RunOverwriteDisabledTest.osm");

    HOOT_FILE_EQUALS( _inputPath + "RunOverwriteDisabledTest.osm",
                     _outputPath + "RunOverwriteDisabledTest.osm");
  }

  void runNegatedFilterTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    OsmMapReaderFactory::read(
      map, _inputPath + "SetTagValueVisitorTest.osm", false, Status::Unknown1);

    SetTagValueVisitor visitor(
      "key3", "value3", false, QStringList("NodeCriterion"), false, true);
    map->visitRw(visitor);

    OsmMapWriterFactory::write(map, _outputPath + "RunNegatedFilterTest.osm");

    HOOT_FILE_EQUALS( _inputPath + "RunNegatedFilterTest.osm",
                     _outputPath + "RunNegatedFilterTest.osm");
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(SetTagValueVisitorTest, "quick");

}


