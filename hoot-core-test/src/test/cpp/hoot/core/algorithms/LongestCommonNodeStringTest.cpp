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
 * @copyright Copyright (C) 2012, 2013, 2014 DigitalGlobe (http://www.digitalglobe.com/)
 */

// GEOS
#include <geos/geom/LineString.h>

// Hoot
#include <hoot/core/algorithms/LongestCommonNodeString.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/visitors/FindWaysVisitor.h>
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

// Standard
#include <sstream>
using namespace std;

class LongestCommonNodeStringTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(LongestCommonNodeStringTest);
  CPPUNIT_TEST(runTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runTest()
  {
    OsmXmlReader reader;

    boost::shared_ptr<OsmMap> map(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.read("test-files/algorithms/LongestCommonNodeStringTest.osm", map);

    {
      boost::shared_ptr<Way> w1 = map->getWay(FindWaysVisitor::findWaysByTag(map, "note", "1")[0]);
      boost::shared_ptr<Way> w2 = map->getWay(FindWaysVisitor::findWaysByTag(map, "note", "2")[0]);

      LongestCommonNodeString uut(w1, w2);
      CPPUNIT_ASSERT_EQUAL(2, uut.apply());
      CPPUNIT_ASSERT_EQUAL(1, uut.getW1Index());
      CPPUNIT_ASSERT_EQUAL(0, uut.getW2Index());
    }

    {
      boost::shared_ptr<Way> w1 = map->getWay(FindWaysVisitor::findWaysByTag(map, "note", "3")[0]);
      boost::shared_ptr<Way> w2 = map->getWay(FindWaysVisitor::findWaysByTag(map, "note", "4")[0]);

      LongestCommonNodeString uut(w1, w2);
      CPPUNIT_ASSERT_EQUAL(4, uut.apply());
      CPPUNIT_ASSERT_EQUAL(0, uut.getW1Index());
      CPPUNIT_ASSERT_EQUAL(2, uut.getW2Index());
    }
  }
};

CPPUNIT_TEST_SUITE_REGISTRATION(LongestCommonNodeStringTest);

