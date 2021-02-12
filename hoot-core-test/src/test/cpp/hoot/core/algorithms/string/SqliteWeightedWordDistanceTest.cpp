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
 * @copyright Copyright (C) 2013, 2014, 2015, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/algorithms/string/SqliteWordWeightDictionary.h>
#include <hoot/core/util/Log.h>

// Tgs
#include <tgs/System/SystemInfo.h>

namespace hoot
{

class SqliteWordWeightDictionaryTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(SqliteWordWeightDictionaryTest);
  CPPUNIT_TEST(runTest);
  CPPUNIT_TEST_SUITE_END();

public:

  SqliteWordWeightDictionaryTest()
    : HootTestFixture("test-files/algorithms/string/",
                      UNUSED_PATH)
  {
  }

  void runTest()
  {
    SqliteWordWeightDictionary uut(_inputPath + "WordWeight.sqlite");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(6, uut.getWeight("starbucks"), 0.0001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(6, uut.getWeight("starbucks"), 0.0001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(4, uut.getWeight("bad"), 0.0001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1, uut.getWeight("casual"), 0.0001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1, uut.getWeight("example"), 0.0001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0, uut.getWeight("none"), 0.0001);
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(SqliteWordWeightDictionaryTest, "quick");

}

