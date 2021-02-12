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
 * @copyright Copyright (C) 2013, 2014, 2015, 2016, 2017, 2018 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/algorithms/string/LevenshteinDistance.h>
#include <hoot/core/algorithms/string/MeanWordSetDistance.h>
#include <hoot/core/algorithms/string/TextFileWordWeightDictionary.h>
#include <hoot/core/algorithms/string/SqliteWordWeightDictionary.h>
#include <hoot/core/algorithms/string/WeightedWordDistance.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/ConfPath.h>
#include <hoot/core/util/Log.h>

// Tgs
#include <tgs/System/SystemInfo.h>

namespace hoot
{

class WeightedWordDistanceTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(WeightedWordDistanceTest);
  CPPUNIT_TEST(runTest);
  CPPUNIT_TEST(runRealWorldTest);
  CPPUNIT_TEST_SUITE_END();

public:

  WeightedWordDistance* _wwd;

  QString normalize(QString s)
  {
    QStringList l = s.split(" ");

    QRegExp nonWord;
    nonWord.setPattern("[^\\w]");

    for (int i = 0; i < l.size(); i++)
    {
      QString normalized = l[i].toLower().normalized(QString::NormalizationForm_C);
      normalized.replace(nonWord, "");
      l[i] = normalized;
    }

    return toString(l);
  }

  void test3(QString s1, QString s2)
  {
    LOG_INFO("* " << s1 << " (" << normalize(s1) << ")  vs. " << s2 << " (" <<
      normalize(s2) << ")");

    MeanWordSetDistance mwsd(StringDistancePtr(new LevenshteinDistance(1.5)));

    LevenshteinDistance ld(1.5);

    LOG_INFO("** weighted word score: " << _wwd->compare(s1, s2));
    LOG_INFO("** mean word set score: " << mwsd.compare(s1, s2));
    LOG_INFO("** levenshtein score: " << ld.compare(s1, s2));
  }

  void runTest()
  {
    WeightedWordDistance uut(new LevenshteinDistance(),
      new TextFileWordWeightDictionary("test-files/algorithms/string/WordWeight.tsv"));
    HOOT_STR_EQUALS(0, uut.compare("foo", "bar"));
    HOOT_STR_EQUALS(0.631018, uut.compare("foo street", "fou street"));
    HOOT_STR_EQUALS(0.987104, uut.compare("foo road", "foo street"));

    HOOT_STR_EQUALS(0.539453, uut.compare("Kafr Ibrahim al Aydi", "Kafr Ayyub"));
    HOOT_STR_EQUALS(1, uut.compare("Kafr Ayyub", "Kafr Ayyub"));

    HOOT_STR_EQUALS(0.359162, uut.compare("Food & More", "Cheap & Good"));
  }

  void runRealWorldTest()
  {
    SqliteWordWeightDictionary* dict = new SqliteWordWeightDictionary(
      ConfPath::search(ConfigOptions().getWeightedWordDistanceDictionary()));
    WeightedWordDistance uut(new LevenshteinDistance(1.5),
      dict);
    _wwd = &uut;
    LOG_VAR(dict->getWeight("kafr"));
    LOG_VAR(uut.compare("Kafr Ibrahim al Aydi", "Kafr Ayyub"));
    LOG_VAR(uut.compare("Kafr Ayyub", "Kafr Ayyub"));
    test3("Kafr Ibrahim al Aydi", "Kafr Ayyub");
    test3("Kaafr Ayyub", "Kafr Ayyub");
    test3("Ayyub Kafr", "Kafr Ayyub");
    // Kafr is quite common (10558)
    // suliman is relatively common (196)
    test3("Kafr Aiyub Suliman", "Kafr Ayyub");
    // london (125556)
    // england (16157)
    test3("London England", "London");
    test3("Lloyds of London", "Lloyds");
    test3("Foo Street", "Foo Road");
    test3("City of Denver", "Denver Colorado");
    test3("Paris Texas", "Paris France");
    test3("`Izbat al Usta", "Izbat al Sharaqwah");
    test3("`Izbat ash Sharaqwah", "Izbat al Sharaqwah");
    test3("Joe's Coffee", "Joe's Restaurant");
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(WeightedWordDistanceTest, "quick");

}

