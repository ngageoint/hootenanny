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
 * @copyright Copyright (C) 2015, 2021, 2022 Maxar (http://www.maxar.com/)
 */

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Standard
#include <iostream>
#include <math.h>
using namespace std;

// TGS
#include <tgs/TgsException.h>
#include <tgs/Optimization/NelderMead.h>

namespace Tgs
{

class NelderMeadTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(NelderMeadTest);
  CPPUNIT_TEST(simpleTest);
  CPPUNIT_TEST_SUITE_END();

public:

  // taken from https://github.com/blinry/nelder-mead-optimizer/blob/master/t/test.cpp
  void simpleTest()
  {
    class FunctionLookup : public NelderMead::Function
    {
    public:
      virtual double f(Vector v)
      {
        callOrder.push_back(v);
        return vMap[v];
      }

      map<Vector, double> vMap;
      vector<Vector> callOrder;
    };

    Vector a(0,0);
    Vector b(1,0);
    Vector c(0,1);
    Vector d(1,1);
    Vector e(1.5,1.5);
    Vector f(0,2);
    Vector g(0.75,0.5);
    Vector h(0.5,1);

    std::shared_ptr<FunctionLookup> fl = std::make_shared<FunctionLookup>();
    fl->vMap[a] = 0;
    fl->vMap[b] = 0.1;
    fl->vMap[c] = 0.5;
    fl->vMap[d] = 1;
    fl->vMap[e] = 0;
    fl->vMap[f] = 0;
    fl->vMap[g] = 0;
    fl->vMap[h] = 0;

    NelderMead o(2, fl);

    o.step(b,0.1);
    o.step(a,0);
    Vector result;
    result.prepare(2);
    o.step(c,0.5);
    result = o.step(c,0.5);
    CPPUNIT_ASSERT(fl->callOrder[0] == d);
    CPPUNIT_ASSERT(fl->callOrder[1] == e);
    CPPUNIT_ASSERT(fl->callOrder[2] == f);
    CPPUNIT_ASSERT(fl->callOrder[3] == g);
    CPPUNIT_ASSERT(fl->callOrder[4] == h);
    double dx = result[0] - 1;
    double dy = result[1] - 1;
    double distance = sqrt(dx * dx + dy * dy);
    CPPUNIT_ASSERT(distance < 0.001);
  }
};

}

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(Tgs::NelderMeadTest, "quick");

