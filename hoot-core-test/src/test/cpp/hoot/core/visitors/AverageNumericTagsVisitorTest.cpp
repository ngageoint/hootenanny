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
 * @copyright Copyright (C) 2018, 2019 Maxar (http://www.maxar.com/)
 */

// hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/visitors/AverageNumericTagsVisitor.h>

namespace hoot
{

class AverageNumericTagsVisitorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(AverageNumericTagsVisitorTest);
  CPPUNIT_TEST(runBasicTest);
  CPPUNIT_TEST_SUITE_END();

public:

  AverageNumericTagsVisitorTest()
    : HootTestFixture("test-files/visitors/",
                      UNUSED_PATH)
  {
  }

  void runBasicTest()
  {
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map,
      _inputPath + "SumNumericTagsVisitorTest.osm",
      false,
      Status::Unknown1);

    AverageNumericTagsVisitor uut(QStringList("test1"));
    map->visitRo(uut);
    const double avg = uut.getStat();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.667, avg, 1e-3);
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(AverageNumericTagsVisitorTest, "quick");

}


