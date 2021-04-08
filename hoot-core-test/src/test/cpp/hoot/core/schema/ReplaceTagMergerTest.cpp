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
#include <hoot/core/schema/ReplaceTagMerger.h>

namespace hoot
{

class ReplaceTagMergerTest : public HootTestFixture
{
    CPPUNIT_TEST_SUITE(ReplaceTagMergerTest);
    CPPUNIT_TEST(runBasicTest);
    CPPUNIT_TEST(runSwapTest);
    CPPUNIT_TEST(runConfigurationTest);
    CPPUNIT_TEST_SUITE_END();

public:

  ReplaceTagMergerTest() = default;

  void runBasicTest()
  {
    ReplaceTagMerger uut;

    Tags t1;
    t1["foo"] = "bar";

    Tags t2;
    t2["bar"] = "baz";
    t2["foo"] = "baz";

    Tags expected;
    expected["foo"] = "bar";

    Tags merged = uut.mergeTags(t1, t2, ElementType::Way);
    CPPUNIT_ASSERT_EQUAL(expected, merged);
  }

  void runSwapTest()
  {
    ReplaceTagMerger uut(true);

    Tags t1;
    t1["foo"] = "bar";
    t1["bar"] = "foo";

    Tags t2;
    t2["bar"] = "baz";

    Tags expected;
    expected["bar"] = "baz";

    Tags merged = uut.mergeTags(t1, t2, ElementType::Way);
    CPPUNIT_ASSERT_EQUAL(expected, merged);
  }

  void runConfigurationTest()
  {
    Settings settings;
    settings.set(ConfigOptions::getTagMergerOverwriteExcludeKey(), QStringList());
    ReplaceTagMerger uut;
    uut.setConfiguration(settings);

    Tags t1;
    t1["foo"] = "bar";

    Tags t2;
    t2["bar"] = "baz";
    t2["foo"] = "baz";

    Tags expected;
    expected["foo"] = "bar";

    Tags merged = uut.mergeTags(t1, t2, ElementType::Way);
    CPPUNIT_ASSERT_EQUAL(expected, merged);
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ReplaceTagMergerTest, "quick");

}
