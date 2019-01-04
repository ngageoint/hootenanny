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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/criterion/TagCriterion2.h>

namespace hoot
{

class TagCriterion2Test : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(TagCriterion2Test);
  CPPUNIT_TEST(runBasicTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runBasicTest()
  {
//    QStringList kvps;
//    kvps.append("amenity=school");
//    kvps.append("poi=yes");

//    TagCriterion uut;
//    uut.setKvps(kvps);

//    NodePtr node1(new Node(Status::Unknown1, -1, Coordinate(0.0, 0.0), 15.0));
//    node1->getTags().set("amenity", "school");
//    node1->getTags().set("poi", "yes");
//    CPPUNIT_ASSERT(uut.isSatisfied(node1));

//    NodePtr node2(new Node(Status::Unknown1, -1, Coordinate(0.0, 0.0), 15.0));
//    node2->getTags().set("amenity", "school");
//    CPPUNIT_ASSERT(uut.isSatisfied(node2));

//    NodePtr node3(new Node(Status::Unknown1, -1, Coordinate(0.0, 0.0), 15.0));
//    node3->getTags().set("highway", "road");
//    CPPUNIT_ASSERT(!uut.isSatisfied(node3));
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(TagCriterion2Test, "quick");

}
