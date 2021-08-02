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
#include <hoot/core/visitors/TagRenameKeyVisitor.h>
#include <hoot/core/visitors/TagKeyCountVisitor.h>

namespace hoot
{

class TagRenameKeyVisitorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(TagRenameKeyVisitorTest);
  CPPUNIT_TEST(runBasicTest);
  CPPUNIT_TEST(runConfigureTest);
  CPPUNIT_TEST_SUITE_END();

public:

  TagRenameKeyVisitorTest() :
  HootTestFixture("test-files/visitors/", UNUSED_PATH)
  {
  }

  void runBasicTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    OsmMapReaderFactory::read(
      map, _inputPath + "TagRenameKeyVisitorTest.osm", false, Status::Unknown1);

    TagRenameKeyVisitor uut("source", "source_modified");
    map->visitRw(uut);

    TagKeyCountVisitor keyCountVisitor1("source");
    map->visitRo(keyCountVisitor1);
    CPPUNIT_ASSERT_EQUAL((long)0, (long)keyCountVisitor1.getStat());

    TagKeyCountVisitor keyCountVisitor2("source_modified");
    map->visitRo(keyCountVisitor2);
    CPPUNIT_ASSERT_EQUAL((long)6, (long)keyCountVisitor2.getStat());
  }

  void runConfigureTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    OsmMapReaderFactory::read(
      map, _inputPath + "TagRenameKeyVisitorTest.osm", false, Status::Unknown1);

    Settings settings;
    settings.set(ConfigOptions::getTagRenameVisitorOldKeyKey(), "source");
    settings.set(ConfigOptions::getTagRenameVisitorNewKeyKey(), "source_modified");
    TagRenameKeyVisitor uut;
    uut.setConfiguration(settings);
    map->visitRw(uut);

    TagKeyCountVisitor keyCountVisitor1("source");
    map->visitRo(keyCountVisitor1);
    CPPUNIT_ASSERT_EQUAL((long)0, (long)keyCountVisitor1.getStat());

    TagKeyCountVisitor keyCountVisitor2("source_modified");
    map->visitRo(keyCountVisitor2);
    CPPUNIT_ASSERT_EQUAL((long)6, (long)keyCountVisitor2.getStat());
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(TagRenameKeyVisitorTest, "quick");

}


