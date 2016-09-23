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
#include <hoot/core/algorithms/zindex/Range.h>

#include "../../TestUtils.h"

namespace hoot
{

class RangeTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(RangeTest);
  CPPUNIT_TEST(testBasics);
  CPPUNIT_TEST_SUITE_END();

public:

  void testBasics()
  {
    Range r(2, 10);
    Range r1(2, 10);
    Range r2(3, 8);

    CPPUNIT_ASSERT_EQUAL((long int)9, r.calculateSize());
    CPPUNIT_ASSERT_EQUAL(true, r=r1);
    CPPUNIT_ASSERT_EQUAL(false, r1>r2);
    CPPUNIT_ASSERT_EQUAL(true, r1<r2);
    CPPUNIT_ASSERT_EQUAL(true, r.in(5));
    CPPUNIT_ASSERT_EQUAL(false, r.in(11));

    CPPUNIT_ASSERT_EQUAL(true, r.isValid());
    r.setInvalid();
    CPPUNIT_ASSERT_EQUAL(false, r.isValid());
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(RangeTest, "quick");

}

