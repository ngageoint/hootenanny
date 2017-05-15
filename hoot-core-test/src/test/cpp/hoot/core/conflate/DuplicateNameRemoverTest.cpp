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
 * @copyright Copyright (C) 2014 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/conflate/DuplicateNameRemover.h>
#include <hoot/core/OsmMap.h>
using namespace hoot;


// Boost
using namespace boost;

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Qt
#include <QDebug>

#include "../TestUtils.h"

using namespace geos::geom;

namespace hoot
{

class DuplicateNameRemoverTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(DuplicateNameRemoverTest);
  CPPUNIT_TEST(runCaseInsensitiveTest);
  CPPUNIT_TEST(runCaseSensitiveTest);
  CPPUNIT_TEST(runExtraNamesTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runCaseInsensitiveTest()
  {
    OsmMap::resetCounters();
    OsmMapPtr map(new OsmMap());
    Coordinate coords[] = { Coordinate(0.0, 0.0), Coordinate(100.0, 0.0),
                            Coordinate(100.0, 10.0), Coordinate(0.0, 10.0),
                            Coordinate::getNull() };
    WayPtr way = TestUtils::createWay(map, Status::Unknown1, coords, 15);
    way->getTags().appendValue("name", "test");
    way->getTags().appendValue("name", "TEST");

    DuplicateNameRemover dupeNameRemover;
    dupeNameRemover.setCaseSensitive(false);
    dupeNameRemover.apply(map);

    CPPUNIT_ASSERT_EQUAL(1, (int)map->getWays().size());
    const Tags& tags = map->getWay(way->getElementId())->getTags();
    CPPUNIT_ASSERT_EQUAL(1, tags.size());
    CPPUNIT_ASSERT_EQUAL(QString("test"), tags.get("name"));
  }

  void runCaseSensitiveTest()
  {
    OsmMap::resetCounters();
    OsmMapPtr map(new OsmMap());
    Coordinate coords[] = { Coordinate(0.0, 0.0), Coordinate(100.0, 0.0),
                            Coordinate(100.0, 10.0), Coordinate(0.0, 10.0),
                            Coordinate::getNull() };
    WayPtr way = TestUtils::createWay(map, Status::Unknown1, coords, 15);
    way->getTags().appendValue("name", "test");
    way->getTags().appendValue("name", "TEST");

    DuplicateNameRemover dupeNameRemover;
    dupeNameRemover.setCaseSensitive(true);
    dupeNameRemover.apply(map);

    CPPUNIT_ASSERT_EQUAL(1, (int)map->getWays().size());
    const Tags& tags = map->getWay(way->getElementId())->getTags();
    CPPUNIT_ASSERT_EQUAL(1, tags.size());
    CPPUNIT_ASSERT_EQUAL(QString("test;TEST"), tags.get("name"));
  }

  void runExtraNamesTest()
  {
    OsmMap::resetCounters();
    OsmMapPtr map(new OsmMap());
    Coordinate coords[] = { Coordinate(0.0, 0.0), Coordinate(100.0, 0.0),
                            Coordinate(100.0, 10.0), Coordinate(0.0, 10.0),
                            Coordinate::getNull() };
    WayPtr way = TestUtils::createWay(map, Status::Unknown1, coords, 15);
    way->getTags().appendValue("name", "test");
    way->getTags().appendValue("name", "TEST");
    way->getTags().appendValue("name", "blah");

    DuplicateNameRemover dupeNameRemover;
    dupeNameRemover.setCaseSensitive(false);
    dupeNameRemover.apply(map);

    CPPUNIT_ASSERT_EQUAL(1, (int)map->getWays().size());
    const Tags& tags = map->getWay(way->getElementId())->getTags();
    CPPUNIT_ASSERT_EQUAL(2, tags.size());
    CPPUNIT_ASSERT_EQUAL(QString("test"), tags.get("name"));
    CPPUNIT_ASSERT_EQUAL(QString("blah"), tags.get("alt_name"));
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(DuplicateNameRemoverTest, "quick");
//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(DuplicateNameRemoverTest, "current");

}

