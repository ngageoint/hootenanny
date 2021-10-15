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
 * @copyright Copyright (C) 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/util/LogColor.h>

#include <iostream>

namespace hoot
{

class LogColorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(LogColorTest);
  CPPUNIT_TEST(logColorRegexTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void logColorRegexTest()
  {
    QString string1 = "Error text.";
    QString string2 = "Regular text.";
    QString string3 = "Trace text.";

    std::ostringstream oss;
    oss << LogColor::Error() << string1 << LogColor::None()
        << string2
        << LogColor::Trace() << string3 << LogColor::None();

    QString removed(oss.str().c_str());

    CPPUNIT_ASSERT(removed.contains(LogColor::Error()));
    CPPUNIT_ASSERT(removed.contains(LogColor::Trace()));
    CPPUNIT_ASSERT(removed.contains(LogColor::None()));

    removed.replace(LogColor::ColorRegex, "");

    CPPUNIT_ASSERT(!removed.contains(LogColor::Error()));
    CPPUNIT_ASSERT(!removed.contains(LogColor::Trace()));
    CPPUNIT_ASSERT(!removed.contains(LogColor::None()));

    HOOT_STR_EQUALS(string1 + string2 + string3, removed);
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(LogColorTest, "quick");

}

