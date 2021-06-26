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
 * @copyright Copyright (C) 2015, 2017, 2019, 2021 Maxar (http://www.maxar.com/)
 */


// CPP Unit Includes
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

#include "../PluginFactory.h"
#include <tgs/RStarTree/MemoryPageStore.h>
#include <tgs/RStarTree/RTreeNodeStore.h>

namespace Tgs
{

class RTreeNodeStoreTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(RTreeNodeStoreTest);
  CPPUNIT_TEST(test1);
  CPPUNIT_TEST_SUITE_END();

public:

  void test1()
  {
    std::shared_ptr<MemoryPageStore> mps(new MemoryPageStore(100));
    RTreeNodeStore uut(2, mps);

    RTreeNode* n1 = uut.createNode();
    CPPUNIT_ASSERT_EQUAL(0, n1->getChildCount());
    RTreeNode* n2 = uut.createNode();
    CPPUNIT_ASSERT_EQUAL(0, n2->getChildCount());

    RTreeNode* n1t = uut.getNode(n1->getId());
    RTreeNode* n2t = uut.getNode(n2->getId());

    CPPUNIT_ASSERT_EQUAL(n1, n1t);
    CPPUNIT_ASSERT_EQUAL(n2, n2t);
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(RTreeNodeStoreTest, PluginFactory::testName());

}
