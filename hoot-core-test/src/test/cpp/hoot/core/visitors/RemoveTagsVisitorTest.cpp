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
 * @copyright Copyright (C) 2018, 2019, 2020 Maxar (http://www.maxar.com/)
 */

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/criterion/ElementTypeCriterion.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/visitors/RemoveTagsVisitor.h>

namespace hoot
{

class RemoveTagsVisitorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(RemoveTagsVisitorTest);
  CPPUNIT_TEST(runRemoveTest);
  CPPUNIT_TEST(runFilterTest);
  CPPUNIT_TEST(runNegatedFilterTest);
  CPPUNIT_TEST(runWildcardTest);
  CPPUNIT_TEST_SUITE_END();

public:

  RemoveTagsVisitorTest() :
  HootTestFixture("test-files/visitors/RemoveTagsVisitorTest/",
                  "test-output/visitors/RemoveTagsVisitorTest/")
  {
    setResetType(ResetBasic);
  }

  void runRemoveTest()
  {
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map, _inputPath + "RemoveTagsVisitorTest.osm", false, Status::Unknown1);

    QStringList keys;
    keys.append("key1");
    keys.append("key2");
    RemoveTagsVisitor visitor(keys);
    visitor.setNegateCriterion(false);
    map->visitRw(visitor);

    OsmMapWriterFactory::write(map, _outputPath + "RunRemoveTest.osm");

    HOOT_FILE_EQUALS(_inputPath + "RunRemoveTest.osm",
                     _outputPath + "RunRemoveTest.osm");
  }

  void runFilterTest()
  {
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map, _inputPath + "RemoveTagsVisitorTest.osm", false, Status::Unknown1);

    QStringList keys;
    keys.append("key1");
    keys.append("key2");
    RemoveTagsVisitor visitor(keys);
    visitor.setNegateCriterion(false);
    visitor.addCriterion(std::shared_ptr<NodeCriterion>(new NodeCriterion()));
    map->visitRw(visitor);

    OsmMapWriterFactory::write(map, _outputPath + "RunFilterTest.osm");

    HOOT_FILE_EQUALS(_inputPath + "RunFilterTest.osm",
                     _outputPath + "RunFilterTest.osm");
  }

  void runNegatedFilterTest()
  {
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map, _inputPath + "RemoveTagsVisitorTest.osm", false, Status::Unknown1);

    QStringList keys;
    keys.append("key1");
    keys.append("key2");
    RemoveTagsVisitor visitor(keys);
    visitor.setNegateCriterion(true);
    visitor.addCriterion(std::shared_ptr<NodeCriterion>(new NodeCriterion()));
    map->visitRw(visitor);

    OsmMapWriterFactory::write(map, _outputPath + "RunNegatedFilterTest.osm");

    HOOT_FILE_EQUALS(_inputPath + "RunNegatedFilterTest.osm",
                     _outputPath + "RunNegatedFilterTest.osm");
  }

  void runWildcardTest()
  {
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map, _inputPath + "RemoveTagsVisitorTest.osm", false, Status::Unknown1);

    QStringList keys;
    keys.append("key*");
    keys.append("mykey");
    RemoveTagsVisitor visitor(keys);
    visitor.setNegateCriterion(false);
    map->visitRw(visitor);

    OsmMapWriterFactory::write(map, _outputPath + "RunWildcardTest.osm");

    HOOT_FILE_EQUALS(_inputPath + "RunWildcardTest.osm",
                     _outputPath + "RunWildcardTest.osm");
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(RemoveTagsVisitorTest, "quick");

}


