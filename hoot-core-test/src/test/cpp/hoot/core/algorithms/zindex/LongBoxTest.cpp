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
 * @copyright Copyright (C) 2014, 2016, 2017, 2018, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/algorithms/zindex/LongBox.h>

using namespace std;

namespace hoot
{

class LongBoxTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(LongBoxTest);
  CPPUNIT_TEST(testBasics);

  CPPUNIT_TEST_SUITE_END();

public:

  void testBasics()
  {
    vector<long int> min;
    min.push_back(-1);
    min.push_back(-1);
    vector<long int> max;
    max.push_back(1);
    max.push_back(1);
    LongBox box(min, max);

    vector<long int> min1;
    min1.push_back(1);
    min1.push_back(1);
    vector<long int> max1;
    max1.push_back(3);
    max1.push_back(3);
    LongBox box1(min1, max1);
    CPPUNIT_ASSERT_EQUAL(true, box.edgeOverlaps(box1));

    //point is inside of box
    vector<long int> p;
    p.push_back(0);
    p.push_back(0);
    CPPUNIT_ASSERT_EQUAL(true, box.in(p));

    //point is outside of box
    p[0] = 2;
    p[1] = 2;
    CPPUNIT_ASSERT_EQUAL(false, box.in(p));

    //expand
    LongBox newBox = box.expand(2);
    HOOT_STR_EQUALS("{ ( -3 : 3) ( -3 : 3) }", newBox.toString());
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(LongBoxTest, "quick");

}

