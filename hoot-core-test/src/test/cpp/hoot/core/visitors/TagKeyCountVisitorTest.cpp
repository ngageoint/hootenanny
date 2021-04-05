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
 * @copyright Copyright (C) 2014, 2015, 2017, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
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
#include <hoot/core/visitors/TagKeyCountVisitor.h>

namespace hoot
{

class TagKeyCountVisitorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(TagKeyCountVisitorTest);
  CPPUNIT_TEST(runBasicTest);
  CPPUNIT_TEST(runConfigureTest);
  CPPUNIT_TEST_SUITE_END();

public:

  TagKeyCountVisitorTest() :
  HootTestFixture("test-files/visitors/", UNUSED_PATH)
  {
  }

  void runBasicTest()
  {
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map, _inputPath + "TagKeyCountVisitorTest.osm", false, Status::Unknown1);

    TagKeyCountVisitor uut("source");
    map->visitRo(uut);

    CPPUNIT_ASSERT_EQUAL((long)6, (long)uut.getStat());
  }

  void runConfigureTest()
  {
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map, _inputPath + "TagKeyCountVisitorTest.osm", false, Status::Unknown1);

    Settings settings;
    settings.set(ConfigOptions::getTagsVisitorKeysKey(), QStringList("source"));
    TagKeyCountVisitor uut;
    uut.setConfiguration(settings);
    map->visitRo(uut);

    CPPUNIT_ASSERT_EQUAL((long)6, (long)uut.getStat());
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(TagKeyCountVisitorTest, "quick");

}


