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
 * @copyright Copyright (C) 2014 DigitalGlobe (http://www.digitalglobe.com/)
 */

// hoot
#include <hoot/core/util/Log.h>
#include <hoot/core/visitors/CalculateHashVisitor.h>

#include "../TestUtils.h"

using namespace std;

namespace hoot
{
using namespace Tgs;

class CalculateHashVisitorTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(CalculateHashVisitorTest);
  CPPUNIT_TEST(runBasicTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runBasicTest()
  {
    NodePtr n1(new Node(Status::Unknown1, -1, -104.8852148123, 38.8467218123, 5));
    n1->getTags()["highway"] = "bus_stop";
    n1->getTags()["name"] = "Bus Stop 1";
    n1->getTags()["source"] = "imagery";

    HOOT_STR_EQUALS("{\"type\":\"Feature\",\"properties\":{\"type\":\"node\",\"tags\":{\"error:circular\":\"5\",\"highway\":\"bus_stop\",\"name\":\"Bus Stop 1\"}},\"geometry\":{\"type\":\"Point\",\"coordinates\":[-104.8852148,38.8467218]}}",
      CalculateHashVisitor::toJsonString(n1));

    // validated here: http://onlinemd5.com/
    // for some reason the commandline sha1sum gives a different result.
    HOOT_STR_EQUALS("746c440be4fc11f9631d62d26cbba6aa97f73e3c",
      QString::fromUtf8(CalculateHashVisitor::toHash(n1).toHex().data()));
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(CalculateHashVisitorTest, "quick");

}


