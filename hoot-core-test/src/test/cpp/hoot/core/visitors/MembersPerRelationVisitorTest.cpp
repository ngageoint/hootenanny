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
#include <hoot/core/TestUtils.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/visitors/MembersPerRelationVisitor.h>

namespace hoot
{

static const QString input = "test-files/criterion/ComplexBuildings.osm";

class MembersPerRelationVisitorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(MembersPerRelationVisitorTest);
  CPPUNIT_TEST(runBasicTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runBasicTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    OsmMapReaderFactory::read(map, input, false, Status::Unknown1);

    MembersPerRelationVisitor uut;
    map->visitRo(uut);

    CPPUNIT_ASSERT_EQUAL(7, (int)uut.getStat());
    CPPUNIT_ASSERT_EQUAL(2, (int)uut.getMin());
    CPPUNIT_ASSERT_EQUAL(3, (int)uut.getMax());
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.33, uut.getAverage(), 2);
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(MembersPerRelationVisitorTest, "quick");

}


