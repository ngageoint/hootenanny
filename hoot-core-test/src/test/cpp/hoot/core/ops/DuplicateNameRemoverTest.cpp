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
 * @copyright Copyright (C) 2014, 2015, 2017, 2018, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/ops/DuplicateNameRemover.h>


using namespace geos::geom;

namespace hoot
{

class DuplicateNameRemoverTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(DuplicateNameRemoverTest);
  CPPUNIT_TEST(runCaseInsensitiveTest);
  CPPUNIT_TEST(runCaseSensitiveTest);
  CPPUNIT_TEST(runExtraNamesTest);
  CPPUNIT_TEST_SUITE_END();

public:

  DuplicateNameRemoverTest()
  {
    setResetType(ResetBasic);
  }

  void runCaseInsensitiveTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    Coordinate coords[] = { Coordinate(0.0, 0.0), Coordinate(100.0, 0.0),
                            Coordinate(100.0, 10.0), Coordinate(0.0, 10.0),
                            Coordinate::getNull() };
    WayPtr way = TestUtils::createWay(map, coords);
    way->getTags().appendValue("name", "test");
    way->getTags().appendValue("name", "TEST");

    DuplicateNameRemover dupeNameRemover;
    dupeNameRemover.setCaseSensitive(false);
    dupeNameRemover.apply(map);

    CPPUNIT_ASSERT_EQUAL(1, (int)map->getWays().size());
    const Tags& tags = map->getWay(way->getElementId())->getTags();
    CPPUNIT_ASSERT_EQUAL(1, tags.size());
    HOOT_STR_EQUALS(QString("test"), tags.get("name"));
  }

  void runCaseSensitiveTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    Coordinate coords[] = { Coordinate(0.0, 0.0), Coordinate(100.0, 0.0),
                            Coordinate(100.0, 10.0), Coordinate(0.0, 10.0),
                            Coordinate::getNull() };
    WayPtr way = TestUtils::createWay(map, coords);
    way->getTags().appendValue("name", "test");
    way->getTags().appendValue("name", "TEST");

    DuplicateNameRemover dupeNameRemover;
    dupeNameRemover.setCaseSensitive(true);
    dupeNameRemover.apply(map);

    CPPUNIT_ASSERT_EQUAL(1, (int)map->getWays().size());
    const Tags& tags = map->getWay(way->getElementId())->getTags();
    CPPUNIT_ASSERT_EQUAL(1, tags.size());
    HOOT_STR_EQUALS(QString("test;TEST"), tags.get("name"));
  }

  void runExtraNamesTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    Coordinate coords[] = { Coordinate(0.0, 0.0), Coordinate(100.0, 0.0),
                            Coordinate(100.0, 10.0), Coordinate(0.0, 10.0),
                            Coordinate::getNull() };
    WayPtr way = TestUtils::createWay(map, coords);
    way->getTags().appendValue("name", "test");
    way->getTags().appendValue("name", "TEST");
    way->getTags().appendValue("name", "blah");

    DuplicateNameRemover dupeNameRemover;
    dupeNameRemover.setCaseSensitive(false);
    dupeNameRemover.apply(map);

    CPPUNIT_ASSERT_EQUAL(1, (int)map->getWays().size());
    const Tags& tags = map->getWay(way->getElementId())->getTags();
    CPPUNIT_ASSERT_EQUAL(2, tags.size());
    HOOT_STR_EQUALS(QString("test"), tags.get("name"));
    HOOT_STR_EQUALS(QString("blah"), tags.get("alt_name"));
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(DuplicateNameRemoverTest, "quick");
//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(DuplicateNameRemoverTest, "current");

}

