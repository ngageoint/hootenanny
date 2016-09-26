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

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// geos
#include <geos/geom/Coordinate.h>

// Hoot
#include <hoot/core/algorithms/zindex/BBox.h>

#include "../../TestUtils.h"

namespace hoot
{

class BBoxTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(BBoxTest);
  CPPUNIT_TEST(testBasics);
  CPPUNIT_TEST_SUITE_END();

public:

  void testBasics()
  {
    vector<double> min;
    min.push_back(-1.0);
    min.push_back(-1.0);
    vector<double> max;
    max.push_back(1.0);
    max.push_back(1.0);
    BBox box(min, max);

    //point is inside of box
    vector<double> p;
    p.push_back(0.0);
    p.push_back(0.0);
    CPPUNIT_ASSERT_EQUAL(true, box.in(p));

    //point is outside of box
    p[0] = 1.1;
    p[1] = 1.1;
    CPPUNIT_ASSERT_EQUAL(false, box.in(p));

    //two boxes intersect
    Coordinate p1(0.0, 0.0);
    Coordinate p2(2.0, 2.0);
    Envelope env(p1, p2);
    BBox box1(env);
    CPPUNIT_ASSERT_EQUAL(true, box.intersects(box1));

    //two boxes don't intersect
    Coordinate p3(2.0, 2.0);
    Coordinate p4(4.0, 4.0);
    Envelope env1(p3, p4);
    BBox box2(env1);
    CPPUNIT_ASSERT_EQUAL(false, box.intersects(box2));

    CPPUNIT_ASSERT_EQUAL(2.0, box.getWidth(0));

  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(BBoxTest, "quick");

}

