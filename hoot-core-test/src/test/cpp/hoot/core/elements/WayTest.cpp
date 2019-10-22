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
 * @copyright Copyright (C) 2013, 2014, 2015, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/util/Log.h>

namespace hoot
{

class WayTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(WayTest);
  CPPUNIT_TEST(runRemoveTest);
  // TODO: update for 3495 added logic
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
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(WayTest, "quick");

}
