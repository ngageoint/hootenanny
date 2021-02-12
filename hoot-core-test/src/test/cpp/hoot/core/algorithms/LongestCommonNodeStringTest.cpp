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
 * @copyright Copyright (C) 2012, 2013, 2014, 2015, 2016, 2017, 2018, 2019 Maxar (http://www.maxar.com/)
 */

// GEOS
#include <geos/geom/LineString.h>

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/algorithms/LongestCommonNodeString.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/visitors/ElementIdsVisitor.h>

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Qt
#include <QDebug>

// Standard
#include <sstream>
using namespace std;

namespace hoot
{

class LongestCommonNodeStringTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(LongestCommonNodeStringTest);
  CPPUNIT_TEST(runTest);
  CPPUNIT_TEST_SUITE_END();

public:

  LongestCommonNodeStringTest()
    : HootTestFixture("test-files/algorithms/",
                      UNUSED_PATH)
  {
  }

  void runTest()
  {
    OsmXmlReader reader;

    OsmMapPtr map(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.read(_inputPath + "LongestCommonNodeStringTest.osm", map);

    {
      WayPtr w1 =
        map->getWay(ElementIdsVisitor::findElementsByTag(map, ElementType::Way, "note", "1")[0]);
      WayPtr w2 =
        map->getWay(ElementIdsVisitor::findElementsByTag(map, ElementType::Way, "note", "2")[0]);

      LongestCommonNodeString uut(w1, w2);
      CPPUNIT_ASSERT_EQUAL(2, uut.apply());
      CPPUNIT_ASSERT_EQUAL(1, uut.getW1Index());
      CPPUNIT_ASSERT_EQUAL(0, uut.getW2Index());
    }

    {
      WayPtr w1 =
        map->getWay(ElementIdsVisitor::findElementsByTag(map, ElementType::Way, "note", "3")[0]);
      WayPtr w2 =
        map->getWay(ElementIdsVisitor::findElementsByTag(map, ElementType::Way, "note", "4")[0]);

      LongestCommonNodeString uut(w1, w2);
      CPPUNIT_ASSERT_EQUAL(4, uut.apply());
      CPPUNIT_ASSERT_EQUAL(0, uut.getW1Index());
      CPPUNIT_ASSERT_EQUAL(2, uut.getW2Index());
    }
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(LongestCommonNodeStringTest, "quick");

}
