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
 * @copyright Copyright (C) 2015, 2016, 2017, 2019, 2021, 2022 Maxar (http://www.maxar.com/)
 */

// Standard Includes
#include <cassert>
#include <cmath>

// CPP Unit Includes
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

#include "../PluginFactory.h"
#include <tgs/RStarTree/DistanceIterator.h>
#include <tgs/RStarTree/MemoryPageStore.h>
#include <tgs/RStarTree/RStarTree.h>
#include <tgs/RStarTree/RTreeNode.h>
#include <tgs/Statistics/Random.h>

using namespace Tgs;

class DistanceIteratorTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(DistanceIteratorTest);
  CPPUNIT_TEST(calculateDistanceTest);
  // these are both benchmarks
  //CPPUNIT_TEST(test1);
  //CPPUNIT_TEST(benchmark);
  CPPUNIT_TEST_SUITE_END();
public:


//   void benchmark()
//   {
//     for (int pageSize = 512; pageSize <= 8192; pageSize *= 2)
//     {
//       for (int size = 1000; size <= 2e6; size *= 2)
//       {
//         for (int dimensions = 1; dimensions < 4; dimensions++)
//         {
//           benchmark(dimensions, size, 1000, pageSize);
//         }
//       }
//     }
//   }

//   void benchmark(int dimensions, int size, int iterations, int pageSize)
//   {
//    std::shared_ptr<MemoryPageStore> mps(new MemoryPageStore(pageSize));
//     RStarTree uut(mps, dimensions);
//
//     Box b(dimensions);
//     Timer t;
//     t.start();
//     std::vector<std::vector<double>> statFodder;
//     std::vector<Box> boxes;
//     std::vector<bool> found;
//
//     found.resize(size);
//
//     statFodder.resize(dimensions);
//     for (int i = 0; i < size; i++)
//     {
//       for (int j = 0; j < dimensions; j++)
//       {
//         double x = generateRandomNormal();
//         b.setBounds(j, x, x);
//         statFodder[j].push_back(x);
//         boxes.push_back(b);
//       }
//       uut.insert(b, i);
//     }
//     double createTime = t.elapsed();
//
//     Stats stats;
//     stats.calculate(statFodder[0]);
//
//     double lambda = stats.percentile75th - stats.percentile25th;
//     double bw = 0.79 * lambda * pow((double)stats.size, -1./5.);
//
//     //printDetails(uut);
//
//     t.restart();
//     std::vector<double> point;
//     point.resize(dimensions);
//     int totalCount = 0;
//     double treeTime = 0.0;
//     double bruteForceTime = 0.0;
//     double nodeHits = 0.0;
//     double distCalcs = 0.0;
//     for (int i = 0; i < iterations; i++)
//     {
//       for (int j = 0; j < dimensions; j++)
//       {
//         point[j] = generateRandomNormal();
//       }
//
//       t.restart();
//       for (int j = 0; j < size; j++)
//       {
//         double d = calculateDistance(point, boxes[j]);
//         found[j] = d <= bw;
//       }
//       bruteForceTime += t.elapsed();
//
//       t.restart();
//       DistanceIterator it(&uut, point, bw);
//       while (it.next())
//       {
//         totalCount++;
//       }
//       treeTime += t.elapsed();
//       nodeHits += it.nodeHits;
//       distCalcs += it.distCalcs;
//     }
//
//     qDebug("%d\t%d\t%d\t%d\t%f\t%f\t%f\t%f\t%.2f",
//       dimensions, size, pageSize, iterations, createTime, (double)totalCount / (double)iterations,
//       treeTime, bruteForceTime, distCalcs / (double)iterations);
// //     qDebug("  nodeHits: %f distCalcs: %f", nodeHits / (double)iterations,
// //       distCalcs / (double)iterations);
//   }

  double calculateDistance(const std::vector<double>& point, const Box& box)
  {
    // calculate a simple euclidean distance in n dimensional space. Probably not what is desired.
    assert((int)point.size() == box.getDimensions());
    double c = 0;
    for (size_t i = 0; i < point.size(); i++)
    {
      double a = 0;
      if (point[i] > box.getUpperBound(i))
      {
        a = point[i] - box.getUpperBound(i);
      }
      else if (point[i] < box.getLowerBound(i))
      {
        a = point[i] - box.getLowerBound(i);
      }
      c += a * a;
    }
    return sqrt(c);
  }

  void calculateDistanceTest()
  {
    Box b(2);
    b.setBounds(0, 1, 2);
    b.setBounds(1, 3, 4);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(1, calculateDistance(makePoint(0, 3.5), b), 1e-6);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1, calculateDistance(makePoint(3, 3.5), b), 1e-6);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1, calculateDistance(makePoint(1.5, 2), b), 1e-6);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1, calculateDistance(makePoint(1.5, 5), b), 1e-6);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(sqrt(2.0), calculateDistance(makePoint(0, 2), b), 1e-6);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(sqrt(2.0), calculateDistance(makePoint(3, 2), b), 1e-6);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(sqrt(2.0), calculateDistance(makePoint(3, 5), b), 1e-6);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(sqrt(2.0), calculateDistance(makePoint(0, 5), b), 1e-6);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0, calculateDistance(makePoint(1.5, 3.5), b), 1e-6);
  }

  /**
   * Generates a pseudo random pseudo normally distributed number
   */
  double generateRandomNormal()
  {
    double result = 0.0;
    for (int i = 0; i < 10; i++)
    {
      double r = Tgs::Random::instance()->generateUniform();
      r = r * 2.0 - 1.0;
      result += r;
    }
    return result;
  }

  std::vector<double> makePoint(double x, double y)
  {
    std::vector<double> result;
    result.resize(2);
    result[0] = x;
    result[1] = y;
    return result;
  }

//   void test1()
//   {
//     int dimensions = 2;
//     int pointCount = 3000;
//    std::shared_ptr<MemoryPageStore> mps(new MemoryPageStore(256));
//     RStarTree uut(mps, dimensions);
//
//     std::vector<Box> boxes;
//     std::vector<int> ids;
//     std::vector<bool> found;
//     found.resize(pointCount);
//
// //     Timer t;
// //     t.start();
//     Box b(dimensions);
//     std::vector<std::vector<double>> statFodder;
//     statFodder.resize(dimensions);
//     for (int i = 0; i < pointCount; i++)
//     {
//       for (int j = 0; j < dimensions; j++)
//       {
//         double x = generateRandomNormal();
//         b.setBounds(j, x, x);
//         statFodder[j].push_back(x);
//       }
//       boxes.push_back(b);
//       ids.push_back(i);
//       uut.insert(b, i);
//     }
// //    double populateTime = t.elapsed();
// //    qDebug("Populate time: %f", populateTime);
//
//     Stats stats;
//     stats.calculate(statFodder[0]);
//
//     double lambda = stats.percentile75th - stats.percentile25th;
//     double bw = 0.79 * lambda * pow((double)stats.size, -1./5.);
//
//     double queryTime = 0.0;
//     double bruteForceTime = 0.0;
//
// //    qDebug("bw: %f", bw);
//
//     double distance = bw * 4.0;
//     std::vector<double> point;
//     point.resize(dimensions);
//     int totalCount = 0;
//     int testCount = 100;
//     for (int i = 0; i < testCount; i++)
//     {
//       for (int j = 0; j < dimensions; j++)
//       {
//         point[j] = generateRandomNormal();
//       }
//
//       t.restart();
//       for (int j = 0; j < pointCount; j++)
//       {
//         double d = calculateDistance(point, boxes[j]);
//         found[j] = d <= distance;
//       }
//       bruteForceTime += t.elapsed();
//
//       t.restart();
//       DistanceIterator it(&uut, point, distance);
//       while (it.next())
//       {
//         //qDebug("%d", it.getId());
//         totalCount++;
//         if (found[it.getId()] == false)
//         {
//           qDebug("Whoops found one twice, or incorrectly. %d", it.getId());
//         }
//         found[it.getId()] = false;
//       }
//       queryTime += t.elapsed();
//
//       for (int j = 0; j < pointCount; j++)
//       {
//         if (found[j] == true)
//         {
//           Box b = boxes[j];
//           double d = calculateDistance(point, boxes[j]);
//           qDebug("Didn't find %d (%f)", j, d);
//           printDetails(uut, uut.getRoot(), point, j);
//         }
//       }
//     }
//
// //     qDebug("Query time: %f", queryTime);
// //     qDebug("Brute force time: %f", bruteForceTime);
// //     qDebug("Average count %f", (double)totalCount / (double)testCount);
//   }

//   bool printDetails(RStarTree& tree, const RTreeNode* n, const std::vector<double>& point, int id)
//   {
//     if (n->isLeafNode())
//     {
//       for (int i = 0; i < n->getChildCount(); i++)
//       {
//         if (n->getChildUserId(i) == id)
//         {
//           qDebug("ID %d details:", id);
//           qDebug(" Leaf node %d[%d] %f %s", n->getId(), i,
//             calculateDistance(point, n->getChildEnvelope(i).toBox()),
//             n->getChildEnvelope(i).toBox().toString().c_str());
//           qDebug("   Envelope: %f %s",
//             calculateDistance(point, n->calculateEnvelope()),
//             n->calculateEnvelope().toString().c_str());
//           return true;
//         }
//       }
//     }
//     else
//     {
//       for (int i = 0; i < n->getChildCount(); i++)
//       {
//         if (printDetails(tree, tree.getNode(n->getChildNodeId(i)), point, id))
//         {
//           qDebug(" Node %d[%d] %f %s", n->getId(), i,
//             calculateDistance(point, n->getChildEnvelope(i).toBox()),
//             n->getChildEnvelope(i).toBox().toString().c_str());
//           qDebug("   Envelope: %f %s",
//             calculateDistance(point, n->calculateEnvelope()),
//             n->calculateEnvelope().toString().c_str());
//           return true;
//         }
//       }
//     }
//     return false;
//   }
//
//   void printDetails(RStarTree& tree)
//   {
//     const RTreeNode* n = tree.getRoot();
//     for (int i = 0; i < n->getChildCount(); i++)
//     {
//       qDebug(" Node %d[%d] %s", n->getId(), i,
//         n->getChildEnvelope(i).toBox().toString().c_str());
//     }
//     qDebug("   Envelope: %s", n->calculateEnvelope().toString().c_str());
//   }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(DistanceIteratorTest, PluginFactory::testName());
