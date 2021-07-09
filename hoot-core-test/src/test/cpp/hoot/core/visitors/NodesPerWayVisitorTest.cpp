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
 * @copyright Copyright (C) 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */

// hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/visitors/NodesPerWayVisitor.h>
#include <hoot/core/criterion/BuildingCriterion.h>

namespace hoot
{

class NodesPerWayVisitorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(NodesPerWayVisitorTest);
  CPPUNIT_TEST(runBasicTest);
  CPPUNIT_TEST(runCritTest);
  CPPUNIT_TEST_SUITE_END();

public:

  NodesPerWayVisitorTest()
  {
    setResetType(ResetBasic);
  }

  void runBasicTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    OsmMapReaderFactory::read(map, "test-files/ToyTestA.osm", false, Status::Unknown1);

    NodesPerWayVisitor uut;
    // rw here b/c MultipleCriterionConsumerVisitor inherits from ElementVisitor and not
    // ConstElementVisitor
    map->visitRw(uut);

    CPPUNIT_ASSERT_EQUAL(40, (int)uut.getStat());
    CPPUNIT_ASSERT_EQUAL(3, (int)uut.getMin());
    CPPUNIT_ASSERT_EQUAL(30, (int)uut.getMax());
    CPPUNIT_ASSERT_EQUAL(10.0, uut.getAverage());
  }

  void runCritTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    OsmMapReaderFactory::read(
      map, "test-files/conflate/unified/AllDataTypesA.osm", false, Status::Unknown1);

    NodesPerWayVisitor uut;
    uut.addCriterion(std::shared_ptr<BuildingCriterion>(new BuildingCriterion()));
    map->visitRw(uut);

    CPPUNIT_ASSERT_EQUAL(81, (int)uut.getStat());
    CPPUNIT_ASSERT_EQUAL(5, (int)uut.getMin());
    CPPUNIT_ASSERT_EQUAL(33, (int)uut.getMax());
    CPPUNIT_ASSERT_EQUAL(8.0, uut.getAverage());
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(NodesPerWayVisitorTest, "quick");

}


