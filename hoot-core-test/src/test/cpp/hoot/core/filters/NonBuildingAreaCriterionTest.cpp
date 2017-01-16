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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/filters/NonBuildingAreaCriterion.h>

// Qt
#include <QDir>

#include "../TestUtils.h"

namespace hoot
{

class NonBuildingAreaCriterionTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(NonBuildingAreaCriterionTest);
  CPPUNIT_TEST(runBasicTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runBasicTest()
  {
    NonBuildingAreaCriterion uut;

    WayPtr way1(new Way(Status::Unknown1, -1, 15.0));
    way1->getTags().set("building", "yes");
    way1->getTags().set("area", "yes");
    CPPUNIT_ASSERT(!uut.isSatisfied(way1));

    WayPtr way2(new Way(Status::Unknown1, -1, 15.0));
    way2->getTags().set("area", "yes");
    CPPUNIT_ASSERT(uut.isSatisfied(way2));
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(NonBuildingAreaCriterionTest, "quick");

}
