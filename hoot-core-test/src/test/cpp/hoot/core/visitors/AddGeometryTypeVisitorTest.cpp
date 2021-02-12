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
 * @copyright Copyright (C) 2014, 2016, 2017, 2018, 2019, 2020 Maxar (http://www.maxar.com/)
 */

// hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/criterion/HighwayCriterion.h>
#include <hoot/core/criterion/PoiCriterion.h>
#include <hoot/core/io/OsmJsonWriter.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/visitors/AddGeometryTypeVisitor.h>

using namespace std;

namespace hoot
{

class AddGeometryTypeVisitorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(AddGeometryTypeVisitorTest);
  CPPUNIT_TEST(runBasicTest);
  CPPUNIT_TEST_SUITE_END();

public:

  AddGeometryTypeVisitorTest()
  {
    setResetType(ResetBasic);
  }

  void runBasicTest()
  {
    OsmMapPtr map(new OsmMap());
    NodePtr n1(new Node(Status::Unknown1, map->createNextNodeId(), 0, 0, 10));
    n1->getTags()["name"] = "strange test";
    map->addNode(n1);

    WayPtr w1(new Way(Status::Unknown1, map->createNextWayId(), 13.0));
    w1->setTag("highway", "road");
    map->addWay(w1);

    AddGeometryTypeVisitor v;
    map->visitRw(v);

    QString ss1 = n1->getTags().toString().replace("\n", " ");
    CPPUNIT_ASSERT_EQUAL(string("name = strange test geometry_type = Point "), ss1.toStdString());

    QString ss2 = w1->getTags().toString().replace("\n", " ");
    CPPUNIT_ASSERT_EQUAL(string("highway = road geometry_type = LineString "), ss2.toStdString());
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(AddGeometryTypeVisitorTest, "quick");

}


