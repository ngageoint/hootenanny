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
 * @copyright Copyright (C) 2013, 2015, 2017, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */

// GEOS
#include <geos/geom/LineString.h>

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/algorithms/string/ExactStringDistance.h>
#include <hoot/core/algorithms/string/LevenshteinDistance.h>
#include <hoot/core/algorithms/string/MeanWordSetDistance.h>
#include <hoot/core/util/Settings.h>

namespace hoot
{

class MeanWordSetDistanceTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(MeanWordSetDistanceTest);
  CPPUNIT_TEST(runTest);
  CPPUNIT_TEST(runLevenshteinTest);
  CPPUNIT_TEST_SUITE_END();

public:

  MeanWordSetDistanceTest()
  {
    setResetType(ResetConfigs);
  }

  void runTest()
  {
    MeanWordSetDistance uut(std::make_shared<ExactStringDistance>());

    CPPUNIT_ASSERT_DOUBLES_EQUAL(1, uut.compare("Hello world", "hello World"), 0.01);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1, uut.compare("world", "hello World"), 0.01);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5, uut.compare("world hallo", "hello World"), 0.01);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1, uut.compare("world hello", "hello World fish"), 0.01);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5, uut.compare("embassy of hungary", "embassy swiss"), 0.01);
  }

  void runLevenshteinTest()
  {
    MeanWordSetDistance uut(std::make_shared<LevenshteinDistance>(1.5));

    Settings s;
    s.set(ConfigOptions::getTokenSeparatorKey(), "[\\s-,';]+");
    uut.setConfiguration(s);

    HOOT_STR_EQUALS(1, uut.compare("Sheraton hotel", "Sheraton Sana'a Hotel"));
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(MeanWordSetDistanceTest, "quick");

}
