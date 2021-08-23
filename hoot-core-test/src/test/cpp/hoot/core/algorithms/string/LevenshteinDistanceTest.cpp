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
 * @copyright Copyright (C) 2012, 2013, 2014, 2015, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */

// GEOS
#include <geos/geom/LineString.h>

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/algorithms/string/LevenshteinDistance.h>
#include <hoot/core/language/ToEnglishDictionaryTranslator.h>
#include <hoot/core/util/Log.h>

// Qt
#include <QString>

// Standard
#include <string>

namespace hoot
{

class LevenshteinDistanceTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(LevenshteinDistanceTest);
  CPPUNIT_TEST(runTest);
  CPPUNIT_TEST(scriptTest);
  CPPUNIT_TEST_SUITE_END();

public:

  QString te(const char* s)
  {
    QString result = ToEnglishDictionaryTranslator().translateStreet(QString::fromUtf8(s));
    return result;
  }

  void scriptTest()
  {
    // hello world in various scripts.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.909, LevenshteinDistance::score(te("хелло ворлд"),
      te("hello world")), 0.001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.636, LevenshteinDistance::score(te("هالو وورلد"), te("hello world")),
                                 0.001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.727, LevenshteinDistance::score(te("הללו וורלד"), te("hello world")),
                                 0.001);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.778, LevenshteinDistance::score(te("الواثق"), te("Al Wathiq")),
                                 0.001);
    // this transliteration doesn't work so well, but it is still a valid test.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.190,
      LevenshteinDistance::score(te("الطريق السريع الدوراح"), te("Ad Dawrah Expressway")), 0.001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.375, LevenshteinDistance::score(te("טבריה"), te("Tiberias")),
                                 0.001);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.909, LevenshteinDistance::score(te("улица Симоновский Вал"),
      te("Simonovsky Val Street")), 0.001);
  }

  void runTest()
  {

    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, LevenshteinDistance::score("abc", "abc"), 0.001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, LevenshteinDistance::score("", "abc"), 0.001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.667, LevenshteinDistance::score("abb", "abc"), 0.001);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.857, LevenshteinDistance::score("Trikora", "TRIKOYA"), 0.001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.269,
      LevenshteinDistance::score(te("JALAN TOL JAKARTA-CIKAMPEK"), te("JAKARTA CIKAMPEK TOLLROAD")),
                                 0.001);
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(LevenshteinDistanceTest, "quick");

}
