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
 * @copyright Copyright (C) 2015, 2017, 2019, 2021, 2022 Maxar (http://www.maxar.com/)
 */

// CPP Unit Includes
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

#include "../PluginFactory.h"
#include <tgs/RStarTree/RTreeNodeStore.h>
#include <tgs/RStarTree/MemoryPageStore.h>
#include <tgs/RStarTree/Page.h>
#include <tgs/RStarTree/RTreeNode.h>

namespace Tgs
{
  class RTreeNodeTest : public CppUnit::TestFixture
  {
    CPPUNIT_TEST_SUITE(RTreeNodeTest);
    CPPUNIT_TEST(test1);
    CPPUNIT_TEST(test2);
    CPPUNIT_TEST_SUITE_END();

  public:

    void test1()
    {
      std::shared_ptr<PageStore> mps = std::make_shared<MemoryPageStore>(100);
      std::shared_ptr<RTreeNodeStore> store = std::make_shared<RTreeNodeStore>(2, mps);

      {
        Tgs::RTreeNode rtn(2, mps->createPage());
        rtn.clear();

        // check init values.
        CPPUNIT_ASSERT(rtn.calculateEnvelope().isValid() == false);
        CPPUNIT_ASSERT_EQUAL(0, rtn.getChildCount());
        CPPUNIT_ASSERT_EQUAL(2, rtn.getDimensions());
        CPPUNIT_ASSERT_EQUAL(0, rtn.getId());
        CPPUNIT_ASSERT_EQUAL(2, rtn.getMaxChildCount());
        CPPUNIT_ASSERT_EQUAL(true, rtn.isLeafNode());
      }

      // load the node again from the same page and make sure it is still ok w/o a clear()
      RTreeNode rtn(2, mps->getPage(0));
      CPPUNIT_ASSERT(rtn.calculateEnvelope().isValid() == false);
      CPPUNIT_ASSERT_EQUAL(0, rtn.getChildCount());
      CPPUNIT_ASSERT_EQUAL(2, rtn.getDimensions());
      CPPUNIT_ASSERT_EQUAL(0, rtn.getId());
      CPPUNIT_ASSERT_EQUAL(2, rtn.getMaxChildCount());
      CPPUNIT_ASSERT_EQUAL(true, rtn.isLeafNode());
    }

    void test2()
    {
      std::shared_ptr<PageStore> mps = std::make_shared<MemoryPageStore>(368);
      std::shared_ptr<RTreeNodeStore> store = std::make_shared<RTreeNodeStore>(2, mps);

      RTreeNode rtn(2, mps->createPage());
      rtn.clear();
      CPPUNIT_ASSERT_EQUAL(10, rtn.getMaxChildCount());

      rtn.setParentId(3);
      CPPUNIT_ASSERT_EQUAL(3, rtn.getParentId());

      // check for basic adding and removing of children.

      Box b1(2);
      b1.setBounds(0, 0, 1);
      b1.setBounds(1, 1, 2);
      rtn.addUserChild(b1, 21);
      CPPUNIT_ASSERT_EQUAL(1, rtn.getChildCount());
      CPPUNIT_ASSERT_EQUAL(21, rtn.getChildUserId(0));
      CPPUNIT_ASSERT(rtn.calculateEnvelope() == b1);
      CPPUNIT_ASSERT(rtn.getChildEnvelope(0).toBox() == b1);
      CPPUNIT_ASSERT_EQUAL(true, rtn.isLeafNode());

      b1.setBounds(0, 3, 4);
      b1.setBounds(1, 5, 6);
      rtn.updateChildEnvelope(0, b1);
      CPPUNIT_ASSERT(rtn.calculateEnvelope() == b1);
      CPPUNIT_ASSERT(rtn.getChildEnvelope(0).toBox() == b1);

      b1.setBounds(0, 10, 11);
      b1.setBounds(1, 1, 1);
      rtn.addUserChild(b1, 22);
      Box b2(2);
      b2.setBounds(0, 3, 11);
      b2.setBounds(1, 1, 6);
      CPPUNIT_ASSERT_EQUAL(2, rtn.getChildCount());
      CPPUNIT_ASSERT_EQUAL(22, rtn.getChildUserId(1));
      CPPUNIT_ASSERT(rtn.calculateEnvelope() == b2);
      CPPUNIT_ASSERT(rtn.getChildEnvelope(1).toBox() == b1);
      CPPUNIT_ASSERT_EQUAL(true, rtn.isLeafNode());

      rtn.removeChild(0);
      CPPUNIT_ASSERT_EQUAL(1, rtn.getChildCount());
      CPPUNIT_ASSERT_EQUAL(22, rtn.getChildUserId(0));
      CPPUNIT_ASSERT(rtn.calculateEnvelope() == b1);
      CPPUNIT_ASSERT(rtn.getChildEnvelope(0).toBox() == b1);
      CPPUNIT_ASSERT_EQUAL(true, rtn.isLeafNode());

      rtn.removeChild(0);
      CPPUNIT_ASSERT_EQUAL(0, rtn.getChildCount());
      CPPUNIT_ASSERT_EQUAL(false, rtn.calculateEnvelope().isValid());
      CPPUNIT_ASSERT_EQUAL(true, rtn.isLeafNode());

      for (int i = 20; i < 30; i++)
      {
        rtn.addUserChild(b1, i);
        CPPUNIT_ASSERT_EQUAL(i - 19, rtn.getChildCount());
      }

      for (int i = 0; i < 10; i++)
      {
        CPPUNIT_ASSERT_EQUAL(i + 20, rtn.getChildUserId(i));
        CPPUNIT_ASSERT(rtn.getChildEnvelope(i).toBox() == b1);
      }

      std::vector<int> ids;
      ids.push_back(2);
      ids.push_back(5);
      ids.push_back(7);
      ids.push_back(1);
      rtn.removeChildren(ids);
      CPPUNIT_ASSERT_EQUAL(6, rtn.getChildCount());
      CPPUNIT_ASSERT_EQUAL(20, rtn.getChildUserId(0));
      CPPUNIT_ASSERT_EQUAL(23, rtn.getChildUserId(1));
      CPPUNIT_ASSERT_EQUAL(24, rtn.getChildUserId(2));
      CPPUNIT_ASSERT_EQUAL(26, rtn.getChildUserId(3));
      CPPUNIT_ASSERT_EQUAL(28, rtn.getChildUserId(4));
      CPPUNIT_ASSERT_EQUAL(29, rtn.getChildUserId(5));

      RTreeNode rtn2(2, mps->getPage(0));
      CPPUNIT_ASSERT_EQUAL(3, rtn.getParentId());
      CPPUNIT_ASSERT_EQUAL(6, rtn.getChildCount());
      CPPUNIT_ASSERT_EQUAL(20, rtn.getChildUserId(0));
      CPPUNIT_ASSERT_EQUAL(23, rtn.getChildUserId(1));
      CPPUNIT_ASSERT_EQUAL(24, rtn.getChildUserId(2));
      CPPUNIT_ASSERT_EQUAL(26, rtn.getChildUserId(3));
      CPPUNIT_ASSERT_EQUAL(28, rtn.getChildUserId(4));
      CPPUNIT_ASSERT_EQUAL(29, rtn.getChildUserId(5));
    }
  };
}

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(Tgs::RTreeNodeTest, PluginFactory::testName());
