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

// hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/visitors/LengthOfWaysVisitor.h>

namespace hoot
{

static const QString input = "";

class LengthOfWaysVisitorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(LengthOfWaysVisitorTest);
  CPPUNIT_TEST(runBasicTest);
  CPPUNIT_TEST_SUITE_END();

public:

  LengthOfWaysVisitorTest()
  {
    setResetType(ResetBasic);
  }

  void runBasicTest()
  {
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(map, input, false, Status::Unknown1);

    LengthOfWaysVisitor uut;
    map->visitRo(uut);

    CPPUNIT_ASSERT_EQUAL(0L, uut.numWithStat());
    CPPUNIT_ASSERT_EQUAL(0.0, uut.getStat());
    CPPUNIT_ASSERT_EQUAL(0.0, uut.getMin());
    CPPUNIT_ASSERT_EQUAL(0.0, uut.getMax());
    CPPUNIT_ASSERT_EQUAL(0.0, uut.getAverage());
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(LengthOfWaysVisitorTest, "quick");

}


