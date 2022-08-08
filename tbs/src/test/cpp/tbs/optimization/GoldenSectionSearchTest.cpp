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
#include <stdexcept>

// TBS
#include <tbs/optimization/GoldenSectionSearch.h>

// boost
#include <boost/math/special_functions/digamma.hpp>

namespace tbs
{
using namespace std;

class GoldenSectionSearchTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(GoldenSectionSearchTest);
  CPPUNIT_TEST(runTest);
  CPPUNIT_TEST(runEqualTest);
  CPPUNIT_TEST(runFlatTest);
  CPPUNIT_TEST_SUITE_END();

public:

  class MyFunction : public LineSearch::Function
  {
  public:
    virtual double operator()(double x)
    {
      return (x - offset) * (x - offset);
    }

    double offset;
  };

  class FlatFunction : public LineSearch::Function
  {
  public:
    virtual double operator()(double /*x*/)
    {
      return 1;
    }
  };

  void runTest()
  {
    double epsilon = 1e-3;
    GoldenSectionSearch uut(epsilon);

    MyFunction f;

    for (int i = 0; i < 100; i++)
    {
      f.offset = drand48() * 200.0 - 100.0;
      double x = uut.argmin(f, -100, 100);
      CPPUNIT_ASSERT_DOUBLES_EQUAL(f.offset, x, epsilon);
    }
  }

  /**
   * See the "if (fx == fb)" comments in GoldenSectionSearch::_search for an explanation.
   */
  void runEqualTest()
  {
    double epsilon = 1e-3;
    GoldenSectionSearch uut(epsilon);

    MyFunction f;

    f.offset = -38.1966011250105097 / 2.0;
    double x = uut.argmin(f, -100, 100);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(f.offset, x, epsilon);
  }

  /**
   * See the "if (fx == fb)" comments in GoldenSectionSearch::_search for an explanation.
   */
  void runFlatTest()
  {
    double epsilon = 1e-3;
    GoldenSectionSearch uut(epsilon);

    FlatFunction f;

    CPPUNIT_ASSERT_THROW(uut.argmin(f, -100, 100), invalid_argument);
  }
};

//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(GoldenSectionSearchTest, "current");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(GoldenSectionSearchTest, "quick");

}
