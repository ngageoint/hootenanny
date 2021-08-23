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
 * @copyright Copyright (C) 2013, 2014, 2015, 2017, 2018, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/schema/TagMergerFactory.h>


namespace hoot
{

class TagMergerFactoryTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(TagMergerFactoryTest);
  CPPUNIT_TEST(averageTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void averageTest()
  {
    {
      Tags t1;
      t1["source"] = "ufd";
      t1["status"] = "construction";
      t1["uuid"] = "foo";

      Tags t2;
      t2["source"] = "mgcp";
      t2["status"] = "functional";
      t2["uuid"] = "bar";

      Tags expected;
      expected["source"] = "ufd;mgcp";
      expected["status"] = "construction";
      expected["uuid"] = "foo;bar";

      Tags avg = TagMergerFactory::mergeTags(t1, t2, ElementType::Node);
      compareTags(expected, avg);
    }
  }

  void compareTags(const Tags& t1, const Tags& t2)
  {
    CPPUNIT_ASSERT_EQUAL(t1.size(), t2.size());

    for (Tags::const_iterator it = t1.begin(); it != t1.end(); ++it)
    {
      CPPUNIT_ASSERT_EQUAL(t1[it.key()].toStdString(), t2[it.key()].toStdString());
    }
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(TagMergerFactoryTest, "quick");

}
