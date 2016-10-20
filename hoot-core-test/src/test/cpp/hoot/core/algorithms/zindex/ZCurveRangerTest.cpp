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
 * This will properly maintain the copyright information. DigitalGlobe
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2014 DigitalGlobe (http://www.digitalglobe.com/)
 */

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// geos
#include <geos/geom/Coordinate.h>

// Hoot
#include <hoot/core/algorithms/zindex/ZCurveRanger.h>

#include "../../TestUtils.h"

namespace hoot
{

class ZCurveRangerTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(ZCurveRangerTest);
  CPPUNIT_TEST(testMaxBitColumn);
  CPPUNIT_TEST(testGetSplitValue);
  CPPUNIT_TEST(testBreakBox);
  CPPUNIT_TEST(testDecomposeBox);
  CPPUNIT_TEST(test2D);
  CPPUNIT_TEST_SUITE_END();

public:

  void testMaxBitColumn()
  {
    vector<double> minV;
    minV.push_back(0.0);
    minV.push_back(1.0);
    vector<double> maxV;
    maxV.push_back(0.0);
    maxV.push_back(1.0);
    ZValue zv(2, 4, minV, maxV);

    ZCurveRanger ranger(zv);
    CPPUNIT_ASSERT_EQUAL(7, ranger.getMaxBitColumn(0xFF));
    CPPUNIT_ASSERT_EQUAL(0, ranger.getMaxBitColumn(0x1));
    CPPUNIT_ASSERT_EQUAL(1, ranger.getMaxBitColumn(0x2));
  }

  void testGetSplitValue()
  {
    vector<double> minV;
    minV.push_back(0.0);
    minV.push_back(1.0);
    vector<double> maxV;
    maxV.push_back(0.0);
    maxV.push_back(1.0);
    ZValue zv(2, 4, minV, maxV);
    ZCurveRanger ranger(zv);
    CPPUNIT_ASSERT_EQUAL((long int)16, ranger.getSplitValue(0x0, 0x10));
    CPPUNIT_ASSERT_EQUAL((long int)272, ranger.getSplitValue(0x100, 0x113));
    CPPUNIT_ASSERT_EQUAL((long int)4, ranger.getSplitValue(0x3, 0x5));
  }

  void testBreakBox()
  {
    vector<double> minV;
    minV.push_back(0.0);
    minV.push_back(1.0);
    vector<double> maxV;
    maxV.push_back(0.0);
    maxV.push_back(1.0);
    ZValue zv(2, 4, minV, maxV);
    ZCurveRanger ranger(zv);

    vector<long int> minL;
    minL.push_back(2);
    minL.push_back(0);
    vector<long int> maxL;
    maxL.push_back(5);
    maxL.push_back(3);
    shared_ptr<LongBox> in = shared_ptr<LongBox>(new LongBox(minL, maxL));
    vector<shared_ptr<LongBox> > lb = ranger.breakBox(in);
    HOOT_STR_EQUALS("{ ( 2 : 3) ( 0 : 3) }", lb[0]->toString());
    HOOT_STR_EQUALS("{ ( 4 : 5) ( 0 : 3) }", lb[1]->toString());

    vector<long int> minL1;
    minL1.push_back(2);
    minL1.push_back(3);
    vector<long int> maxL1;
    maxL1.push_back(5);
    maxL1.push_back(4);
    shared_ptr<LongBox> in1 = shared_ptr<LongBox>(new LongBox(minL1, maxL1));
    vector<shared_ptr<LongBox> > lb1 = ranger.breakBox(in1);
    HOOT_STR_EQUALS("{ ( 2 : 5) ( 3 : 3) }", lb1[0]->toString());
    HOOT_STR_EQUALS("{ ( 2 : 5) ( 4 : 4) }", lb1[1]->toString());

    vector<long int> minL2;
    minL2.push_back(2);
    minL2.push_back(2);
    vector<long int> maxL2;
    maxL2.push_back(4);
    maxL2.push_back(7);
    shared_ptr<LongBox> in2 = shared_ptr<LongBox>(new LongBox(minL2, maxL2));
    vector<shared_ptr<LongBox> > lb2 = ranger.breakBox(in2);
    HOOT_STR_EQUALS("{ ( 2 : 4) ( 2 : 3) }", lb2[0]->toString());
    HOOT_STR_EQUALS("{ ( 2 : 4) ( 4 : 7) }", lb2[1]->toString());

    vector<long int> minL3;
    minL3.push_back(4);
    minL3.push_back(4);
    vector<long int> maxL3;
    maxL3.push_back(7);
    maxL3.push_back(5);
    shared_ptr<LongBox> in3 = shared_ptr<LongBox>(new LongBox(minL3, maxL3));
    vector<shared_ptr<LongBox> > lb3 = ranger.breakBox(in3);
    HOOT_STR_EQUALS("{ ( 4 : 5) ( 4 : 5) }", lb3[0]->toString());
    HOOT_STR_EQUALS("{ ( 6 : 7) ( 4 : 5) }", lb3[1]->toString());
  }

  void testDecomposeBox()
  {
    vector<double> minV;
    minV.push_back(0.0);
    minV.push_back(1.0);
    vector<double> maxV;
    maxV.push_back(0.0);
    maxV.push_back(1.0);
    ZValue zv(2, 4, minV, maxV);
    ZCurveRanger ranger(zv);

    vector<long int> minL;
    minL.push_back(2);
    minL.push_back(0);
    vector<long int> maxL;
    maxL.push_back(5);
    maxL.push_back(3);
    LongBox in(minL, maxL);

    vector<Range> r = ranger.decomposeRange(in, 0);
    QStringList result;
    for (uint i = 0; i < r.size(); i++)
    {
      result << r[i].toString();
    }
    HOOT_STR_EQUALS("[4 : 27]", result.join(", "));

    vector<long int> minL1;
    minL1.push_back(1);
    minL1.push_back(0);
    vector<long int> maxL1;
    maxL1.push_back(2);
    maxL1.push_back(7);
    LongBox in1(minL1, maxL1);

    vector<Range> r1 = ranger.decomposeRange(in1, 1);
    QStringList result1;
    for (uint i = 0; i < r1.size(); i++)
    {
      result1 << r1[i].toString();
    }
    HOOT_STR_EQUALS("[1 : 14], [33 : 46]", result1.join(", "));

    ranger.setSlop(1);
    vector<Range> r2  = ranger.decomposeRange(in1, 1);
    QStringList result2;
    for (uint i = 0; i < r2.size(); i++)
    {
      result2 << r2[i].toString();
    }
    HOOT_STR_EQUALS("[1 : 6], [9 : 14], [33 : 38], [41 : 46]", result2.join(", "));
  }

  void test2D()
  {
    vector<double> minV;
    minV.push_back(-180.0);
    minV.push_back(-90.0);
    vector<double> maxV;
    maxV.push_back(180.0);
    maxV.push_back(90.0);
    ZValue zv(2, 10, minV, maxV);
    ZCurveRanger ranger(zv);

    vector<double> minB;
    minB.push_back(-105.0);
    minB.push_back(38.0);
    vector<double> maxB;
    maxB.push_back(-104.9);
    maxB.push_back(38.1);

    BBox b(minB, maxB);
    vector<Range> r = ranger.decomposeRange(b, 1);
    QStringList result;
    for (uint i = 0; i < r.size(); i++)
    {
      result << r[i].toString();
    }
    HOOT_STR_EQUALS("[586555 : 586555], [586641 : 586641]", result.join(", "));
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ZCurveRangerTest, "quick");

}

