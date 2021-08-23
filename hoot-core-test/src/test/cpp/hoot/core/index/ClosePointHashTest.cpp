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
 * @copyright Copyright (C) 2012, 2013, 2014, 2015, 2016, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/index/ClosePointHash.h>

// Qt
#include <QDebug>
#include <QTime>

// Standard

#include <math.h>
#include <sstream>
using namespace std;

// TGS
#include <tgs/Statistics/Random.h>
#include <tgs/StreamUtils.h>
using namespace Tgs;

namespace hoot
{

class ClosePointHashTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(ClosePointHashTest);
  CPPUNIT_TEST(runBasicTest);
  CPPUNIT_TEST(runRandomTest);
  CPPUNIT_TEST_SUITE_END();

public:

  ClosePointHashTest() = default;

  void runBasicTest()
  {
    ClosePointHash cph(1.0);

    cph.addPoint(0.0, 0.0, 0);
    cph.addPoint(0.0, 0.0, 1);
    cph.addPoint(0.0, 0.0, 2);

    int count = 0;
    cph.resetIterator();
    while (cph.next())
    {
      const vector<long>& v = cph.getMatch();
      count++;
      CPPUNIT_ASSERT_EQUAL((size_t)3, v.size());
    }
    CPPUNIT_ASSERT_EQUAL(false, cph.next());
    CPPUNIT_ASSERT_EQUAL(4, count);
  }

  void runRandomTest()
  {
    double neighborDistance = 1.0;
    ClosePointHash cph(neighborDistance);

    vector<double> x, y;
    vector<set<int>> neighbors1, neighbors2;
    const size_t SIZE = 100;
    const double range = 10;
    neighbors1.resize(SIZE);
    neighbors2.resize(SIZE);

    for (size_t i = 0; i < SIZE; i++)
    {
      x.push_back(Random::instance()->generateUniform() * range * 2 - range);
      y.push_back(Random::instance()->generateUniform() * range * 2 - range);
      cph.addPoint(x[i], y[i], i);
    }

    for (size_t i = 0; i < SIZE; i++)
    {
      for (size_t j = 0; j < SIZE; j++)
      {
        double dx = x[i] - x[j];
        double dy = y[i] - y[j];
        double d = sqrt(dx * dx + dy * dy);
        if (i != j && d <= neighborDistance)
        {
          neighbors1[i].insert(j);
        }
      }
    }

    QTime t;

    t.start();

    int count = 0;
    cph.resetIterator();
    while (cph.next())
    {
      const vector<long>& v = cph.getMatch();

      for (size_t i = 0; i < v.size(); i++)
      {
        for (size_t j = 0; j < v.size(); j++)
        {
          double dx = x[v[i]] - x[v[j]];
          double dy = y[v[i]] - y[v[j]];
          double d = sqrt(dx * dx + dy * dy);
          if (v[i] != v[j] && d <= neighborDistance)
          {
            neighbors2[v[i]].insert(v[j]);
            count++;
          }
        }
      }
    }

    for (size_t i = 0; i < SIZE; i++)
    {
      CPPUNIT_ASSERT_EQUAL(neighbors1[i].size(), neighbors2[i].size());
      neighbors1[i].insert(neighbors2[i].begin(), neighbors2[i].end());
      CPPUNIT_ASSERT_EQUAL(neighbors1[i].size(), neighbors2[i].size());
    }
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ClosePointHashTest, "quick");

}
