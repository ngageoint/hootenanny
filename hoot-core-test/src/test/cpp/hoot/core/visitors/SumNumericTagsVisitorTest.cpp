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
 * @copyright Copyright (C) 2014, 2017, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */

// hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/visitors/SumNumericTagsVisitor.h>

namespace hoot
{

class SumNumericTagsVisitorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(SumNumericTagsVisitorTest);
  CPPUNIT_TEST(runBasicTest);
  CPPUNIT_TEST(runConfigureTest);
  CPPUNIT_TEST_SUITE_END();

public:

  SumNumericTagsVisitorTest() :
  HootTestFixture("test-files/visitors/", UNUSED_PATH)
  {
  }

  void runBasicTest()
  {
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map, _inputPath + "SumNumericTagsVisitorTest.osm", false, Status::Unknown1);

    SumNumericTagsVisitor uut(QStringList("test1"));
    map->visitRo(uut);
    const int sum = (int)uut.getStat();
    CPPUNIT_ASSERT_EQUAL(11, sum);
  }

  void runConfigureTest()
  {
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map, _inputPath + "SumNumericTagsVisitorTest.osm", false, Status::Unknown1);

    Settings settings;
    settings.set(ConfigOptions::getTagsVisitorKeysKey(), QStringList("test1"));
    SumNumericTagsVisitor uut;
    uut.setConfiguration(settings);
    map->visitRo(uut);
    const int sum = (int)uut.getStat();
    CPPUNIT_ASSERT_EQUAL(11, sum);
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(SumNumericTagsVisitorTest, "quick");

}


