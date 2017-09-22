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
 * @copyright Copyright (C) 2013 DigitalGlobe (http://www.digitalglobe.com/)
 */

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// GEOS
#include <geos/geom/LineString.h>

// Hoot
#include <hoot/core/algorithms/MeanWordSetDistance.h>
#include <hoot/core/algorithms/ExactStringDistance.h>
#include <hoot/core/algorithms/LevenshteinDistance.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/Settings.h>
using namespace hoot;

// Qt
#include <QString>

// Standard
#include <string>

#include "../TestUtils.h"

namespace hoot
{

class MeanWordSetDistanceTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(MeanWordSetDistanceTest);
  CPPUNIT_TEST(runTest);
  CPPUNIT_TEST(runLevenshteinTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runTest()
  {
    MeanWordSetDistance uut(new ExactStringDistance());

    CPPUNIT_ASSERT_DOUBLES_EQUAL(1, uut.compare("Hello world", "hello World"), 0.01);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1, uut.compare("world", "hello World"), 0.01);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5, uut.compare("world hallo", "hello World"), 0.01);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1, uut.compare("world hello", "hello World fish"), 0.01);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5, uut.compare("embassy of hungary", "embassy swiss"), 0.01);
  }

  void runLevenshteinTest()
  {
    MeanWordSetDistance uut(new LevenshteinDistance(1.5));

    Settings s;
    s.set("token.separator", "[\\s-,';]+");
    uut.setConfiguration(s);

    HOOT_STR_EQUALS(1, uut.compare("Sheraton hotel", "Sheraton Sana'a Hotel"));
  }

};

//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(MeanWordSetDistanceTest, "current");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(MeanWordSetDistanceTest, "quick");

}
