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
 * @copyright Copyright (C) 2021, 2022 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/schema/GeneralizeTagMerger.h>

namespace hoot
{

class GeneralizeTagMergerTest : public HootTestFixture
{
    CPPUNIT_TEST_SUITE(GeneralizeTagMergerTest);
    CPPUNIT_TEST(runBasicTest);
    CPPUNIT_TEST_SUITE_END();

public:

  GeneralizeTagMergerTest() = default;

  void runBasicTest()
  {
    GeneralizeTagMerger uut;

    Tags t1;
    t1["name"] = "foo";
    t1["lala"] = "1;2";
    t1[MetadataTags::Building()] = "yes";
    t1["uid"] = "123";

    Tags t2;
    t2["name"] = "bar";
    t2["lala"] = "2;1";
    t2[MetadataTags::Building()] = "yes";
    t2["uid"] = "456";

    Tags expected;
    expected["name"] = "foo";
    expected["alt_name"] = "bar";
    expected["lala"] = "1;2";
    expected["uid"] = "123;456";
    expected[MetadataTags::Building()] = "yes";

    Tags merged = uut.mergeTags(t1, t2, ElementType::Way);
    _compareTags(expected, merged);
  }

private:

  void _compareTags(const Tags& t1, const Tags& t2)
  {
    if (t1.size() != t2.size())
    {
      LOG_WARN("t1: " << t1.toString());
      LOG_WARN("t2: " << t2.toString());
      CPPUNIT_ASSERT_EQUAL(t1.size(), t2.size());
    }

    for (auto it = t1.begin(); it != t1.end(); ++it)
    {
      if (t1[it.key()] != t2[it.key()])
      {
        LOG_WARN("t1: " << t1.toString());
        LOG_WARN("t2: " << t2.toString());
        CPPUNIT_ASSERT_EQUAL(t1[it.key()].toStdString(), t2[it.key()].toStdString());
      }
    }
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(GeneralizeTagMergerTest, "quick");

}
