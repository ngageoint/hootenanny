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

// Standard Includes
#include <iostream>
#include <math.h>

// CPP Unit Includes
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

#include <tgs/RStarTree/Box.h>
#include <tgs/RStarTree/HilbertRTree.h>
#include <tgs/RStarTree/KnnIteratorNd.h>
#include <tgs/RStarTree/MemoryPageStore.h>
#include <tgs/RStarTree/Page.h>
#include <tgs/Statistics/Random.h>

#include "../PluginFactory.h"

namespace Tgs
{

class KnnIteratorNdTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(KnnIteratorNdTest);
  CPPUNIT_TEST(test1);
  CPPUNIT_TEST_SUITE_END();
public:
  std::vector<Box> testData;
  std::vector<int> testId;

  class Distance
  {
  public:
    double distance;
    int id;

    bool operator<(const Distance& other) const
    {
      return distance < other.distance;
    }
  };

  /**
   * Calculates the distance from a point to a box. We assume the box has a volume of zero.
   * (lowerBound == upperBound in all dimensions)
   */
  double _calculateDistance(const std::vector<double>& pt, const Box& b)
  {
    double sum = 0.0;
    for (unsigned int i = 0; i < pt.size(); i++)
    {
      double diff = pt[i] - b.getLowerBoundRaw(i);
      sum += diff * diff;
    }
    return sqrt(sum);
  }

  /**
   * uses brute force to calculate the nearest neighbors
   */
  void calculateNearestNeighbors(const std::vector<double>& pt, std::vector<Distance>& r)
  {
    r.resize(testData.size());
    for (unsigned int i = 0; i < testData.size(); i++)
    {
      Distance d;
      d.distance = _calculateDistance(pt, testData[i]);
      d.id = testId[i];
      r[i] = d;
    }
    sort(r.begin(), r.end());
  }

  void createTestData(int dimensions, int count)
  {
    testData.resize(count);
    testId.resize(count);

    for (int i = 0; i < count; i++)
    {
      testData[i].setDimensions(dimensions);
      testId[i] = i;
      for (int j = 0; j < dimensions; j++)
      {
        double v = Tgs::Random::instance()->generateInt();
        testData[i].setBounds(j, v, v);
      }
    }
  }


  void test1()
  {
    std::shared_ptr<HilbertRTree> tree =
      std::make_shared<HilbertRTree>(std::make_shared<MemoryPageStore>(256), 3);

    createTestData(3, 4);
    tree->bulkInsert(testData, testId);
    std::vector<double> pt;
    pt.resize(3);
    pt[0] = Tgs::Random::instance()->generateInt();
    pt[1] = Tgs::Random::instance()->generateInt();
    pt[2] = Tgs::Random::instance()->generateInt();
    KnnIteratorNd it(tree.get(), pt);
    std::vector<Distance> verficationData;
    for (unsigned int i = 0; i < 10; i++)
    {
      calculateNearestNeighbors(pt, verficationData);

      int idx = 0;
      while (it.next())
      {
        CPPUNIT_ASSERT(idx < (int)verficationData.size());
        CPPUNIT_ASSERT_DOUBLES_EQUAL(it.getDistance(), verficationData[idx].distance, 1e-6);
        CPPUNIT_ASSERT_EQUAL(it.getId(), verficationData[idx].id);
        idx++;
      }
      CPPUNIT_ASSERT_EQUAL((int)verficationData.size(), idx);

      pt[0] = Tgs::Random::instance()->generateInt();
      pt[1] = Tgs::Random::instance()->generateInt();
      pt[2] = Tgs::Random::instance()->generateInt();
      it.reset(pt);
    }
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(KnnIteratorNdTest, PluginFactory::testName());

}
