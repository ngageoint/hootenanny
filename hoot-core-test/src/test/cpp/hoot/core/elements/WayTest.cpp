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
 * @copyright Copyright (C) 2013, 2014, 2015, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/elements/Way.h>


namespace hoot
{

class WayTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(WayTest);
  CPPUNIT_TEST(runRemoveTest);
  CPPUNIT_TEST(runReplaceTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runRemoveTest()
  {
    Way w(Status::Unknown1, -1, 10);

    w.addNode(1);
    w.addNode(2);
    w.addNode(2);
    w.addNode(3);
    w.addNode(2);
    w.addNode(4);
    w.addNode(5);

    w.removeNode(4);
    HOOT_STR_EQUALS("[6]{1, 2, 2, 3, 2, 5}", w.getNodeIds());
    w.removeNode(2);
    HOOT_STR_EQUALS("[3]{1, 3, 5}", w.getNodeIds());
  }

  void runReplaceTest()
  {
    Way w(Status::Unknown1, -1, 10);

    w.clear();
    w.addNode(1);
    w.addNode(2);
    w.addNode(5);
    w.replaceNode(2, 4);
    HOOT_STR_EQUALS("[3]{1, 4, 5}", w.getNodeIds());

    // closed ways
    w.clear();
    w.addNode(1);
    w.addNode(2);
    w.addNode(1);
    w.replaceNode(2, 4);
    HOOT_STR_EQUALS("[3]{1, 4, 1}", w.getNodeIds());

    // replacement node already exists - Note that Way::replaceNode does allow duplicated way nodes
    // to be introduced. Its currently the responsibility of the caller to clean up after the
    // replacement. See notes in Way::replaceNode. If we decide we want to change that behavior,
    // the the correct output here becomes: [4]{1, 4, 3, 1}
    w.clear();
    w.addNode(1);
    w.addNode(2);
    w.addNode(3);
    w.addNode(4);
    w.addNode(1);
    w.replaceNode(2, 4);
    HOOT_STR_EQUALS("[5]{1, 4, 3, 4, 1}", w.getNodeIds());

    // start/end node replaced
    w.clear();
    w.addNode(1);
    w.addNode(2);
    w.addNode(1);
    w.replaceNode(1, 3);
    HOOT_STR_EQUALS("[3]{3, 2, 3}", w.getNodeIds());

    // This is non-sensical, and mostly here for documentation purposes. If we didn't allow dupe
    // nodes in Way::replaceNode, we would end up with two nodes instead of three. No way should
    // ever have duplicate nodes, but we don't check that at runtime outside of debugging due to
    // the performance hit.
    w.clear();
    w.addNode(1);
    w.addNode(2);
    w.addNode(1);
    w.replaceNode(1, 2);
    HOOT_STR_EQUALS("[3]{2, 2, 2}", w.getNodeIds());

    // Replace a middle node with an end node that matches with the ID of what we're replacing with
    // We end up with two consecutive nodes with the same ID due to dupes being allowed.
    w.clear();
    w.addNode(1);
    w.addNode(2);
    w.addNode(3);
    w.replaceNode(2, 3);
    HOOT_STR_EQUALS("[3]{1, 3, 3}", w.getNodeIds());

    // Replace an end node with the ID from the end node on the other side. We should end up with
    // the way having identical end nodes that both have the replacement ID.
    w.clear();
    w.addNode(1);
    w.addNode(2);
    w.addNode(3);
    w.replaceNode(1, 3);
    HOOT_STR_EQUALS("[3]{3, 2, 3}", w.getNodeIds());
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(WayTest, "quick");

}
