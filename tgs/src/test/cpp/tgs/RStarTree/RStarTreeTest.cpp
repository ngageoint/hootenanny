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
#include <stdlib.h>
#include <iostream>

// CPP Unit Includes
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

#ifdef WIN32
// Qt Includes
#include <QFile>
#endif

#include "../PluginFactory.h"
#include <tgs/RStarTree/FilePageStore.h>
#include <tgs/RStarTree/MemoryPageStore.h>
#include <tgs/RStarTree/RStarTree.h>
#include <tgs/RStarTree/KnnIterator.h>
#include <tgs/Statistics/Random.h>

namespace Tgs
{

class RStarTreeTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(RStarTreeTest);
  CPPUNIT_TEST(test1);
  CPPUNIT_TEST(test2);
#ifdef WIN32
  CPPUNIT_TEST(fileTest);
#endif
  // benchmarking test
  //CPPUNIT_TEST(timingTest);
  CPPUNIT_TEST_SUITE_END();
public:

   void test1()
   {
     RStarTree uut(std::make_shared<MemoryPageStore>(2048), 2);
     int maxChildCount = uut.getRoot()->getMaxChildCount();

     CPPUNIT_ASSERT_EQUAL(0, uut.getRoot()->getChildCount());

     Box all(2);

     int cc = 0;
     for (int i = 0; i < maxChildCount; i++)
     {
       Box b(2);
       b.setBounds(0, cc, cc + 1);
       b.setBounds(1, cc + 2, cc + 3);
       all.expand(b);
       uut.insert(b, cc);
       CPPUNIT_ASSERT_EQUAL(true, uut.getRoot()->isLeafNode());
       CPPUNIT_ASSERT_EQUAL(cc + 1, uut.getRoot()->getChildCount());
       CPPUNIT_ASSERT_EQUAL(cc, uut.getRoot()->getChildUserId(i));
       CPPUNIT_ASSERT(b == uut.getRoot()->getChildEnvelope(cc).toBox());
       cc++;
     }

     validateTreeEntries(uut, cc);

     for (int i = 0; i < maxChildCount - 2; i++)
     {
       Box b(2);
       b.setBounds(0, cc, cc + 1);
       b.setBounds(1, cc + 2, cc + 3);
       all.expand(b);
       uut.insert(b, cc);
       CPPUNIT_ASSERT_EQUAL(false, uut.getRoot()->isLeafNode());
       CPPUNIT_ASSERT(all == uut.getRoot()->calculateEnvelope());
       cc++;
       validateTreeEntries(uut, cc);
     }

     for (int i = 0; i < 1000; i++)
     {
       Box b(2);
       b.setBounds(0, cc, cc + 1);
       b.setBounds(1, cc + 2, cc + 3);
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
     RStarTree uut(std::make_shared<MemoryPageStore>(256), 2);

     Box b(2);
     for (int i = 0; i < 1000; i++)
     {
       double l1 = Tgs::Random::instance()->generateInt(10000);
       double u1 = l1 + Tgs::Random::instance()->generateInt(100);
       double l2 = Tgs::Random::instance()->generateInt(10000);
       double u2 = l2 + Tgs::Random::instance()->generateInt(100);
       b.setBounds(0, l1, u1);
       b.setBounds(1, l2, u2);
       uut.insert(b, i);
     }
     validateTreeBounds(uut);
   }

#ifdef WIN32
   void fileTest()
   {
     {
       if (QFile::exists("fileTest.dat"))
       {
         QFile::remove("fileTest.dat");
       }

       RStarTree uut(std::make_shared<FilePageStore>(256, "fileTest.dat"), 2);

       Box b(2);
       for (int i = 0; i < 3000; i++)
       {
         double l1 = Tgs::Random::instance()->generate(10000);
         double u1 = l1 + Tgs::Random::instance()->generate(100);
         double l2 = Tgs::Random::instance()->generate(10000);
         double u2 = l2 + Tgs::Random::instance()->generate(100);
         b.setBounds(0, l1, u1);
         b.setBounds(1, l2, u2);
         uut.insert(b, i);
       }
       validateTreeBounds(uut);
     }
     {
       RStarTree uut(std::make_shared<FilePageStore>(256, "fileTest.dat"), 2);

       validateTreeBounds(uut);
     }
   }
#endif

   void timingTest()
   {
    std::shared_ptr<MemoryPageStore> mps(new MemoryPageStore(256));
    std::shared_ptr<RStarTree> rst(new RStarTree(mps, 2));
     RStarTree& uut = *rst;

//      Timer t;
     Box b(2);
//      t.start();
     for (int i = 0; i < 20000; i++)
     {
       double l1 = Tgs::Random::instance()->generateInt(10000);
       double u1 = l1 + Tgs::Random::instance()->generateInt(100);
       double l2 = Tgs::Random::instance()->generateInt(10000);
       double u2 = l2 + Tgs::Random::instance()->generateInt(100);
       b.setBounds(0, l1, u1);
       b.setBounds(1, l2, u2);
       uut.insert(b, i);
     }
//      std::cout << t.elapsed() << "sec" << std::endl;

     std::cout << "Searching..." << std::endl;
//      t.restart();

     for (int i = 0; i < 2000; i++)
     {
       double x = Tgs::Random::instance()->generateInt(20000) - 5000;
       double y = Tgs::Random::instance()->generateInt(20000) - 5000;
       KnnIterator it(rst.get(), x, y);
       it.next();
//        if (i % 1000 == 0)
//        {
//          std::cout << t.elapsed() << "sec" << std::endl;
//        }
     }
//      std::cout << t.elapsed() << "sec" << std::endl;
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
     //qDebug("*** Begin Validation ***");
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
         //qDebug("Leaf Node %d contains: (parent %d)", n->getId(), n->getParentId());
         //int lastId = -100;
         for (int i = 0; i < n->getChildCount(); i++)
         {
           int id = n->getChildUserId(i);

           //if (i == 0)
           //{
           //  qDebug("  %d", id);
           //}
           //else if (id != lastId + 1)
           //{
           //  qDebug("  -> %d", lastId);
           //  qDebug("  %d", id);
           //}
           //else if (i == n->getChildCount() - 1)
           //{
           //  qDebug("  -> %d", id);
           //}
           //lastId = id;

           CPPUNIT_ASSERT(id < (int)found.size());
           CPPUNIT_ASSERT(found[id] == false);
           found[id] = true;
           Box b(2);
           b.setBounds(0, id, id + 1);
           b.setBounds(1, id + 2, id + 3);
           bounds.expand(b);
           Box tmp = n->getChildEnvelope(i).toBox();
           CPPUNIT_ASSERT(b == tmp);
         }
         CPPUNIT_ASSERT(bounds == n->calculateEnvelope());
       }
       else
       {
         //qDebug("Node %d contains: (parent %d)", n->getId(), n->getParentId());
         for (int i = 0; i < n->getChildCount(); i++)
         {
           int id = n->getChildNodeId(i);
           //qDebug("  %d", id);
           nodes.push_back(uut.getNode(id));
           bounds.expand(uut.getNode(id)->calculateEnvelope());
         }
         CPPUNIT_ASSERT(bounds == n->calculateEnvelope());
       }
     }

     for (unsigned int i = 0; i < found.size(); i++)
     {
       //if (found[i] != true)
       //{
       //  qDebug("Didn't find %d", i);
       //}
       CPPUNIT_ASSERT(found[i] == true);
     }
   }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(RStarTreeTest, PluginFactory::testName());

}
