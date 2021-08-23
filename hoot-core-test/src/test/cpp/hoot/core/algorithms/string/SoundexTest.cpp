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
 * @copyright Copyright (C) 2013, 2015, 2018, 2021 Maxar (http://www.maxar.com/)
 */

// GEOS
#include <geos/geom/LineString.h>

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/algorithms/string/Soundex.h>
#include <hoot/core/util/Log.h>

namespace hoot
{

class SoundexTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(SoundexTest);
  CPPUNIT_TEST(runTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runTest()
  {
    HOOT_STR_EQUALS(QString("R163"), Soundex::encode("Robert"));
    HOOT_STR_EQUALS(QString("R163"), Soundex::encode("Rupert"));
    HOOT_STR_EQUALS(QString("R150"), Soundex::encode("Rubin"));
    HOOT_STR_EQUALS(QString("A261"), Soundex::encode("Ashcraft"));
    HOOT_STR_EQUALS(QString("A261"), Soundex::encode("Ashcroft"));
    HOOT_STR_EQUALS(QString("T522"), Soundex::encode("Tymczak"));
    HOOT_STR_EQUALS(QString("P236"), Soundex::encode("Pfister"));

    // check boundary conditions. Soundex shouldn't take two words as input, but we should handle
    // it.
    HOOT_STR_EQUALS(QString("H643"), Soundex::encode("hi world"));
    // short words should have zeros appended
    HOOT_STR_EQUALS(QString("H000"), Soundex::encode("hi"));
    HOOT_STR_EQUALS(QString("L200"), Soundex::encode("LoOk"));

    CPPUNIT_ASSERT_EQUAL(4, Soundex::compareSoundex("Washington", "Washingtin"));
    CPPUNIT_ASSERT_EQUAL(1, Soundex::compareSoundex("fish", "swimmer"));
  }

};

//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(SoundexTest, "current");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(SoundexTest, "quick");

}
