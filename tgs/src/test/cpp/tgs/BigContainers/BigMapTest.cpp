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
 * @copyright Copyright (C) 2015, 2016, 2017, 2019, 2021 Maxar (http://www.maxar.com/)
 */

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Standard
#include <stdlib.h>
#include <iostream>
#include <limits>
using namespace std;

// TGS
#include <tgs/BigContainers/BigMap.h>
#include <tgs/Statistics/Random.h>
#include <tgs/System/Time.h>

namespace Tgs
{

class BigMapTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(BigMapTest);
  CPPUNIT_TEST(simpleTest);
  CPPUNIT_TEST_SUITE_END();
public:

  void simpleTest()
  {
    BigMap<long, long> t;

    map<long, long> m;

    for (size_t i = 0; i < 100000; i++)
    {
      long k = Tgs::Random::instance()->generateInt();
      long v = Tgs::Random::instance()->generateInt();
      m[k] = v;
      t[k] = v;
    }

    for (auto it = m.begin(); it != m.end(); ++it)
    {
      CPPUNIT_ASSERT_EQUAL(it->second, t[it->first]);
    }

    BigMap<long, long>& ct = t;
    const map<long, long>& cm = m;
    for (size_t i = 0; i < 100000; i++)
    {
      long k = Tgs::Random::instance()->generateInt();
      CPPUNIT_ASSERT_EQUAL(cm.count(k), ct.count(k));
    }
  }

  struct CompareLess
  {
    bool operator() (const long& a, const long& b) const
    {
      return a < b;
    }

    static long max_value() { return std::numeric_limits<long>::max(); }
  };

};

}

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(Tgs::BigMapTest, "quick");

