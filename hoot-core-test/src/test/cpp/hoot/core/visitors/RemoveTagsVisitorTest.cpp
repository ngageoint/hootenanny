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
 * @copyright Copyright (C) 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/criterion/NodeCriterion.h>
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
  CPPUNIT_TEST_SUITE_END();

public:

  RemoveTagsVisitorTest()
  {
    setResetType(ResetBasic);
    TestUtils::mkpath("test-output/visitors");
  }

  void runRemoveTest()
  {
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map, "test-files/visitors/RemoveTagsVisitorTest.osm", false, Status::Unknown1);

    RemoveTagsVisitor visitor("key1", "key2");
    visitor.setNegateCriterion(false);
    map->visitRw(visitor);

    OsmMapWriterFactory::getInstance().write(map,
      "test-output/visitors/RemoveTagsVisitorTest-runRemoveTest.osm");

    HOOT_FILE_EQUALS(
      "test-files/visitors/RemoveTagsVisitorTest-runRemoveTest.osm",
      "test-output/visitors/RemoveTagsVisitorTest-runRemoveTest.osm");
  }

  void runFilterTest()
  {
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map, "test-files/visitors/RemoveTagsVisitorTest.osm", false, Status::Unknown1);

    RemoveTagsVisitor visitor("key1", "key2");
    visitor.setNegateCriterion(false);
    visitor.addCriterion(boost::shared_ptr<NodeCriterion>(new NodeCriterion()));
    map->visitRw(visitor);

    OsmMapWriterFactory::getInstance().write(map,
      "test-output/visitors/RemoveTagsVisitorTest-runFilterTest.osm");

    HOOT_FILE_EQUALS(
      "test-files/visitors/RemoveTagsVisitorTest-runFilterTest.osm",
      "test-output/visitors/RemoveTagsVisitorTest-runFilterTest.osm");
  }

  void runNegatedFilterTest()
  {
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map, "test-files/visitors/RemoveTagsVisitorTest.osm", false, Status::Unknown1);

    RemoveTagsVisitor visitor("key1", "key2");
    visitor.setNegateCriterion(true);
    visitor.addCriterion(boost::shared_ptr<NodeCriterion>(new NodeCriterion()));
    map->visitRw(visitor);

    OsmMapWriterFactory::getInstance().write(map,
      "test-output/visitors/RemoveTagsVisitorTest-runNegatedFilterTest.osm");

    HOOT_FILE_EQUALS(
      "test-files/visitors/RemoveTagsVisitorTest-runNegatedFilterTest.osm",
      "test-output/visitors/RemoveTagsVisitorTest-runNegatedFilterTest.osm");
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(RemoveTagsVisitorTest, "quick");

}


