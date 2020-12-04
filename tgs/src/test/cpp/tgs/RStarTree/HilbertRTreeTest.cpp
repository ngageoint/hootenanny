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
 * @copyright Copyright (C) 2015, 2016, 2017, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Standard Includes
#include <stdlib.h>
#include <iostream>
using namespace std;

// CPP Unit Includes
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Tgs
#include "../PluginFactory.h"
#include <tgs/RStarTree/MemoryPageStore.h>
#include <tgs/RStarTree/HilbertRTree.h>
#include <tgs/RStarTree/KnnIterator.h>
#include <tgs/Statistics/Random.h>

namespace Tgs
{

class Tmp
{
public:
  double d;
  int id;

  Tmp() {}

  Tmp(double d, int id)
  {
    this->d = d;
    this->id = id;
  }

  bool operator<(const Tmp& t) const
  {
    return (d < t.d) || (d == t.d && id < t.id);
  }
};

class HilbertRTreeTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(HilbertRTreeTest);
  CPPUNIT_TEST(test1);
  CPPUNIT_TEST(test2);
  CPPUNIT_TEST(test3);
  CPPUNIT_TEST(test4);
  CPPUNIT_TEST(testBulkLoadDistance);
  CPPUNIT_TEST(testStandardLoadDistance);
  CPPUNIT_TEST(testRStarDistance);
  CPPUNIT_TEST(testGreedyShuffle);
  CPPUNIT_TEST(testMixLoadDistance);
  // this is used for benchmarking, not testing.
  //CPPUNIT_TEST(benchmark);
  CPPUNIT_TEST_SUITE_END();
public:

  void _createOrderedTestData(int testSize, std::vector<Box>& boxes, std::vector<int>& fids)
  {
    boxes.resize(0);
    fids.resize(0);
    boxes.reserve(testSize);
    fids.reserve(testSize);
    Box b(2);
    int cc = 0;
    for (int i = 0; i < testSize; i++)
    {
      b.setBounds(0, cc, cc + 1);
      b.setBounds(1, cc + 2, cc + 1 - 2 * (cc % 2));
      boxes.push_back(b);
      fids.push_back(cc);
      cc++;
    }
  }

  void _createRandomTestData(int testSize, std::vector<Box>& boxes, std::vector<int>& fids)
  {
    boxes.resize(0);
    fids.resize(0);
    boxes.reserve(testSize);
    fids.reserve(testSize);
    Box b(2);
    Tgs::Random::instance()->seed(0);
    for (int i = 0; i < testSize; i++)
    {
      double l1 = Tgs::Random::instance()->generateInt(10000);
      double u1 = l1 + Tgs::Random::instance()->generateInt(100) - 50;
      double l2 = Tgs::Random::instance()->generateInt(10000);
      double u2 = l2 + Tgs::Random::instance()->generateInt(100) - 50;
      b.setBounds(0, l1, u1);
      b.setBounds(1, l2, u2);
      boxes.push_back(b);
      fids.push_back(i);
    }
  }

  void test1()
  {
    std::shared_ptr<MemoryPageStore> mps(new MemoryPageStore(2048));
    RStarTree uut(mps, 2);
    int maxChildCount = uut.getRoot()->getMaxChildCount();

    CPPUNIT_ASSERT_EQUAL(0, uut.getRoot()->getChildCount());

    Box all(2);

    int cc = 0;
    for (int i = 0; i < maxChildCount; i++)
    {
      Box b(2);
      b.setBounds(0, cc, cc + 1);
      b.setBounds(1, cc + 2, cc + 1 - 2 * (cc % 2));
      all.expand(b);
      uut.insert(b, cc);
      CPPUNIT_ASSERT_EQUAL(true, uut.getRoot()->isLeafNode());
      CPPUNIT_ASSERT_EQUAL(cc + 1, uut.getRoot()->getChildCount());
      CPPUNIT_ASSERT_EQUAL(cc, uut.getRoot()->getChildUserId(i));
      CPPUNIT_ASSERT(b == uut.getRoot()->getChildEnvelope(cc).toBox());
      cc++;
      validateTreeEntries(uut, cc);
    }

    for (int i = 0; i < maxChildCount - 2; i++)
    {
      Box b(2);
      b.setBounds(0, cc, cc + 1);
      b.setBounds(1, cc + 2, cc + 1 - 2 * (cc % 2));
      all.expand(b);
      uut.insert(b, cc);
      validateTreeBounds(uut);
      CPPUNIT_ASSERT_EQUAL(false, uut.getRoot()->isLeafNode());
      CPPUNIT_ASSERT(all == uut.getRoot()->calculateEnvelope());
      cc++;
      validateTreeEntries(uut, cc);
    }

    for (int i = 0; i < 1000; i++)
    {
      Box b(2);
      b.setBounds(0, cc, cc + 1);
      b.setBounds(1, cc + 2, cc + 1 - 2 * (cc % 2));
      all.expand(b);
      uut.insert(b, cc);
      CPPUNIT_ASSERT_EQUAL(false, uut.getRoot()->isLeafNode());
      Box env = uut.getRoot()->calculateEnvelope();
      CPPUNIT_ASSERT(all == env);
      cc++;
      if (i % 100 == 0)
      {
        validateTreeEntries(uut, cc);
      }
    }
  }

  void test2()
  {
    int testSize = 1000;

    std::shared_ptr<MemoryPageStore> mps(new MemoryPageStore(256));
    HilbertRTree uut(mps, 2);

    std::vector<Box> boxes;
    std::vector<int> fids;
    _createOrderedTestData(testSize, boxes, fids);
    uut.bulkInsert(boxes, fids);

    validateTreeEntries(uut, testSize);
    validateTreeBounds(uut);
  }

  void test3()
  {
    int testSize = 1000;

    std::shared_ptr<MemoryPageStore> mps(new MemoryPageStore(256));
    HilbertRTree uut(mps, 2);
    
    std::vector<Box> boxes;
    std::vector<int> fids;
    _createOrderedTestData(testSize, boxes, fids);
    for (int i = 0; i < testSize; i++)
    {
      uut.insert(boxes[i], fids[i]);
      uut.sanityCheck();
    }

    validateTreeBounds(uut);
    validateTreeEntries(uut, testSize);
  }

  void test4()
  {
    int testSize = 1000;

    std::shared_ptr<MemoryPageStore> mps(new MemoryPageStore(256));
    HilbertRTree uut(mps, 2);

    std::vector<Box> boxes;
    std::vector<int> fids;
    _createRandomTestData(testSize, boxes, fids);
    uut.bulkInsert(boxes, fids);

    validateTreeBounds(uut);
  }

  void testStandardLoadDistance()
  {
    int testSize = 300;
    int sampleSize = 40;

    std::shared_ptr<MemoryPageStore> mps(new MemoryPageStore(256));
    std::shared_ptr<HilbertRTree> uut(new HilbertRTree(mps, 2));
    _testTreeDistance(uut, testSize, sampleSize, false);
  }

  void testBulkLoadDistance()
  {
    int testSize = 500;
    int sampleSize = 50;

    std::shared_ptr<MemoryPageStore> mps(new MemoryPageStore(256));
    std::shared_ptr<HilbertRTree> uut(new HilbertRTree(mps, 2));
    _testTreeDistance(uut, testSize, sampleSize, true);
  }

  void testMixLoadDistance()
  {
    int testSize = 500;
    int sampleSize = 50;
    std::vector<Box> boxes;
    std::vector<int> fids;
    _createRandomTestData(testSize, boxes, fids);

    std::shared_ptr<MemoryPageStore> mps(new MemoryPageStore(256));
    std::shared_ptr<HilbertRTree> uut(new HilbertRTree(mps, 2));
    std::vector<Box> firstBoxes = boxes;
    std::vector<int> firstFids = fids;
    firstBoxes.resize(testSize / 2);
    firstFids.resize(testSize / 2);

    uut->bulkInsert(firstBoxes, firstFids);
    for (int i = testSize / 2; i < testSize; i++)
    {
      uut->insert(boxes[i], fids[i]);
    }
    _testTreeDistance(uut, testSize, sampleSize);
  }

  void testGreedyShuffle()
  {
    int testSize = 1000;

    std::shared_ptr<MemoryPageStore> mps(new MemoryPageStore(128));
    HilbertRTree uut(mps, 2);

    std::vector<Box> boxes;
    std::vector<int> fids;
    _createRandomTestData(testSize, boxes, fids);
    uut.bulkInsert(boxes, fids);
    validateTreeBounds(uut);

    int sampleSize = 100;

    Tgs::Random::instance()->seed(0);
//     t.restart();
    double x, y;
    for (int i = 0; i < sampleSize; i++)
    {
      x = Tgs::Random::instance()->generateInt() - 3000;
      y = Tgs::Random::instance()->generateInt() - 3000;

      KnnIterator it(&uut, x, y);
      CPPUNIT_ASSERT(it.next());
    }
//    double searchTime = t.elapsed();
//     printf("%.2f\t", searchTime);

    uut.greedyShuffle();
    validateTreeBounds(uut);

    Tgs::Random::instance()->seed(0);
//    t.restart();
    for (int i = 0; i < sampleSize; i++)
    {
      x = Tgs::Random::instance()->generateInt() - 3000;
      y = Tgs::Random::instance()->generateInt() - 3000;

      KnnIterator it(&uut, x, y);
      CPPUNIT_ASSERT(it.next());
    }
//    searchTime = t.elapsed();
//     printf("%.2f\t", searchTime);
  }

  void testRStarDistance()
  {
    int testSize = 300;
    int sampleSize = 40;

    std::shared_ptr<MemoryPageStore> mps(new MemoryPageStore(256));
    std::shared_ptr<RStarTree> uut(new RStarTree(mps, 2));
    _testTreeDistance(uut, testSize, sampleSize, false);
  }

  void _testTreeDistance(const std::shared_ptr<RStarTree>& uut, int testSize, int sampleSize, bool bulkLoad)
  {
    std::vector<Box> boxes;
    std::vector<int> fids;
    _createRandomTestData(testSize, boxes, fids);
    if (bulkLoad)
    {
      std::shared_ptr<HilbertRTree> hrt = std::dynamic_pointer_cast<HilbertRTree>(uut);
      hrt->bulkInsert(boxes, fids);
    }
    else
    {
      for (int i = 0; i < testSize; i++)
      {
        uut->insert(boxes[i], fids[i]);
      }
    }
    _testTreeDistance(uut, testSize, sampleSize);
  }

  void _testTreeDistance(const std::shared_ptr<RStarTree>& uut, int testSize, int sampleSize)
  {
    std::vector<Box> boxes;
    std::vector<int> fids;
    _createRandomTestData(testSize, boxes, fids);
    validateTreeBounds(*uut);

    std::vector<Tmp> bruteForceResults;
    bruteForceResults.resize(testSize);
    for (int i = 0; i < sampleSize; i++)
    {
      double x = Tgs::Random::instance()->generateInt() - 3000;
      double y = Tgs::Random::instance()->generateInt() - 3000;

      for (int j = 0; j < testSize; j++)
      {
        double d = KnnIterator::distPtToLine(x, y, 
          boxes[j].getLowerBoundRaw(0), boxes[j].getLowerBoundRaw(1),
          boxes[j].getUpperBoundRaw(0), boxes[j].getUpperBoundRaw(1));
        d = sqrt(d);
        bruteForceResults[j].d = d;
        //printf("%f, %f : %f\n", x, y, d);
        bruteForceResults[j].id = fids[j];
      }
      sort(bruteForceResults.begin(), bruteForceResults.end());

      KnnIterator it(uut.get(), x, y);
      int j = 0;
      while (it.next())
      {
        if (bruteForceResults[j].d != it.getDistance() || 
          bruteForceResults[j].id != it.getId())
        {
          std::cout << bruteForceResults[j].d << ":" << it.getDistance() << "\t" <<
            bruteForceResults[j].id << ":" << it.getId() << std::endl;
        }
        j++;
      }
    }
  }

  void benchmarkTree(const std::shared_ptr<RStarTree>& rst, int testSize, int sampleSize, bool bulkLoad)
  {
    std::vector<Box> boxes;
    std::vector<int> fids;
    _createRandomTestData(testSize, boxes, fids);
    
    if (bulkLoad == true)
    {
      std::shared_ptr<HilbertRTree> hrt = std::dynamic_pointer_cast<HilbertRTree>(rst);
      hrt->bulkInsert(boxes, fids);
    }
    else
    {
      for (unsigned int i = 0; i < boxes.size(); i++)
      {
        rst->insert(boxes[i], fids[i]);
      }
    }

    Tgs::Random::instance()->seed(0);
    for (int i = 0; i < sampleSize; i++)
    {
      double x = Tgs::Random::instance()->generateInt() - 3000;
      double y = Tgs::Random::instance()->generateInt() - 3000;

      KnnIterator it(rst.get(), x, y);
      CPPUNIT_ASSERT(it.next());
    }
  }

  void benchmark()
  {
    int sampleSize = 40000;
    printf("\nTestSz\tPageSz\tChldrn\tLoadH\tSrchH\tLoadHB\tSrchHB\tLoadR*\tSrchR*\n");
    int testSize = 30000;
    for (int pageSize = 128; pageSize <= 4096; pageSize = (int)(1.2 * pageSize))
    {
      printf("%d\t%d\t", testSize, pageSize);
      {
        std::shared_ptr<MemoryPageStore> mps(new MemoryPageStore(pageSize));
        std::shared_ptr<HilbertRTree> hrt(new HilbertRTree(mps, 2));
        printf("%d\t", hrt->getRoot()->getMaxChildCount());
        benchmarkTree(hrt, testSize, sampleSize, false);
      }

      {
        std::shared_ptr<MemoryPageStore> mps(new MemoryPageStore(pageSize));
        std::shared_ptr<HilbertRTree> hrt(new HilbertRTree(mps, 2));
        benchmarkTree(hrt, testSize, sampleSize, true);
      }

      {
        std::shared_ptr<MemoryPageStore> mps(new MemoryPageStore(pageSize));
        std::shared_ptr<RStarTree> rst(new RStarTree(mps, 2));
        benchmarkTree(rst, testSize, sampleSize, false);
      }
      printf("\n");
    }
  }

  void validateTreeBounds(RStarTree& uut)
  {
    int rootId = uut.getRoot()->getId();
    std::list<int> nodes;
    nodes.push_front(rootId);
    while (nodes.size() > 0)
    {
      const RTreeNode* n = uut.getNode(nodes.front());
      nodes.pop_front();

      // make sure that only the root node has a parent of -1
      CPPUNIT_ASSERT((n->getParentId() == -1 && n->getId() == rootId) ||
        (n->getParentId() != -1 && n->getId() != rootId));

      Box bounds(2);
      if (n->isLeafNode())
      {
        for (int i = 0; i < n->getChildCount(); i++)
        {
          Box tmp = n->getChildEnvelope(i).toBox();
          bounds.expand(tmp);
        }
        CPPUNIT_ASSERT(bounds == n->calculateEnvelope());
      }
      else
      {
        for (int i = 0; i < n->getChildCount(); i++)
        {
          int id = n->getChildNodeId(i);
          nodes.push_front(id);
          const RTreeNode* child = uut.getNode(id);
          Box childBounds = child->calculateEnvelope();
          CPPUNIT_ASSERT_EQUAL(n->getId(), child->getParentId());
          bounds.expand(childBounds);
          CPPUNIT_ASSERT(childBounds == n->getChildEnvelope(i).toBox());
        }
        CPPUNIT_ASSERT(bounds == n->calculateEnvelope());
      }
    }
  }

  void validateTreeEntries(RStarTree& uut, int cc)
  {
    //printf("*** Begin Validation ***\n");
    std::vector<bool> found;
    found.resize(cc);
    for (unsigned int i = 0; i < found.size(); i++)
    {
      found[i] = false;
    }

    int rootId = uut.getRoot()->getId();
    std::list<const RTreeNode*> nodes;
    nodes.push_back(uut.getRoot());
    while (nodes.size() > 0)
    {
      const RTreeNode* n = nodes.front();

      // make sure that only the root node has a parent of -1
      CPPUNIT_ASSERT((n->getParentId() == -1 && n->getId() == rootId) ||
        (n->getParentId() != -1 && n->getId() != rootId));
      
      nodes.pop_front();
      Box bounds(2);
      if (n->isLeafNode())
      {
        //printf("Leaf Node %d contains: (parent %d)\n", n->getId(), n->getParentId());
        int lastId = -100;
        for (int i = 0; i < n->getChildCount(); i++)
        {
          int id = n->getChildUserId(i);

          if (i == 0)
          {
            //printf("  %d\n", id);
          }
          else if (id != lastId + 1)
          {
            //printf("  -> %d\n", lastId);
            //printf("  %d\n", id);
          }
          else if (i == n->getChildCount() - 1)
          {
            //printf("  -> %d\n", id);
          }
          lastId = id;

          CPPUNIT_ASSERT(id < (int)found.size());
          CPPUNIT_ASSERT(found[id] == false);
          found[id] = true;
          Box b(2);
          b.setBounds(0, id, id + 1);
          b.setBounds(1, id + 2, id + 1 - 2 * (id % 2));
          bounds.expand(b);
          Box tmp = n->getChildEnvelope(i).toBox();
//           if ((b == tmp) == false)
//           {
//             std::cout << id << " " << b.toString() << std::endl;
//             std::cout << tmp.toString() << std::endl;
//           }
          CPPUNIT_ASSERT(b == tmp);
        }
        CPPUNIT_ASSERT(bounds == n->calculateEnvelope());
      }
      else
      {
        //printf("Node %d contains: (parent %d)\n", n->getId(), n->getParentId());
        for (int i = 0; i < n->getChildCount(); i++)
        {
          int id = n->getChildNodeId(i);
          CPPUNIT_ASSERT(id >= 0);
          //printf("  %d\n", id);
          nodes.push_back(uut.getNode(id));
          bounds.expand(uut.getNode(id)->calculateEnvelope());
        }
        CPPUNIT_ASSERT(bounds == n->calculateEnvelope());
      }
    }

    for (unsigned int i = 0; i < found.size(); i++)
    {
      if (found[i] != true)
      {
        printf("Didn't find %u\n", i);
      }
      CPPUNIT_ASSERT_EQUAL(true, (bool)found[i]);
    }
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(HilbertRTreeTest, PluginFactory::testName());

}
