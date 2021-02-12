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
 * @copyright Copyright (C) 2013, 2014, 2015, 2018 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/algorithms/string/StringTokenizer.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Log.h>

namespace hoot
{

class StringTokenizerTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(StringTokenizerTest);
  CPPUNIT_TEST(runTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runTest()
  {
    StringTokenizer uut;

    Settings s;
    s.set(ConfigOptions::getTokenKeepNonWordsKey(), false);
    s.set(ConfigOptions::getTokenMinSizeKey(), 2);
    uut.setConfiguration(s);

    HOOT_STR_EQUALS("[2]{Hello, World.}", uut.tokenize("Hello World."));
    HOOT_STR_EQUALS("[1]{Dime}", uut.tokenize("5 & Dime"));
    HOOT_STR_EQUALS("[2]{Foo, Bar}", uut.tokenize("Foo ---- Bar"));

    s.set(ConfigOptions::getTokenKeepNonWordsKey(), true);
    s.set(ConfigOptions::getTokenMinSizeKey(), 1);
    uut.setConfiguration(s);

    HOOT_STR_EQUALS("[2]{Hello, World.}", uut.tokenize("Hello World."));
    HOOT_STR_EQUALS("[3]{5, &, Dime}", uut.tokenize("5 & Dime"));
    HOOT_STR_EQUALS("[3]{Foo, ----, Bar}", uut.tokenize("Foo ---- Bar"));

    s.set(ConfigOptions::getTokenKeepNonWordsKey(), true);
    s.set(ConfigOptions::getTokenMinSizeKey(), 3);
    uut.setConfiguration(s);

    HOOT_STR_EQUALS("[2]{Hello, World.}", uut.tokenize("Hello World."));
    HOOT_STR_EQUALS("[1]{Dime}", uut.tokenize("5 & Dime"));
    HOOT_STR_EQUALS("[3]{Foo, ----, Bar}", uut.tokenize("Foo ---- Bar"));
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(StringTokenizerTest, "quick");

}

