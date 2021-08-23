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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/conflate/matching/Match.h>
#include <hoot/core/conflate/matching/MatchThreshold.h>
#include <hoot/core/visitors/poi-polygon/PoiPolygonMatchVisitor.h>
#include <hoot/core/io/OsmXmlReader.h>

#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/visitors/ElementIdsVisitor.h>

using namespace geos::geom;
using namespace std;

namespace hoot
{

class PoiPolygonMatchVisitorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(PoiPolygonMatchVisitorTest);
  CPPUNIT_TEST(runIsCandidateTest);
  CPPUNIT_TEST_SUITE_END();

private:

  OsmMapPtr getTestMap1()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();

    Coordinate c1[] = { Coordinate(0.0, 0.0), Coordinate(20.0, 0.0),
                        Coordinate(20.0, 20.0), Coordinate(0.0, 20.0),
                        Coordinate(0.0, 0.0),
                        Coordinate::getNull() };
    WayPtr w1 = TestUtils::createWay(map, c1, "w1", Status::Unknown1, 5);
    w1->getTags().set("building", true);
    w1->getTags().set("name", "foo");
    WayPtr w2 = TestUtils::createWay(map, c1, "w2", Status::Unknown1, 5);
    w2->getTags().set("area", true);

    NodePtr n1 = std::make_shared<Node>(Status::Unknown1, 1, 10, 10, 5);
    n1->getTags().set("poi", true);
    n1->getTags().set("name", "foo");
    map->addNode(n1);
    NodePtr n2 = std::make_shared<Node>(Status::Unknown2, 2, 5, 10, 5);
    n2->getTags().set("poi", true);
    n2->getTags().set("name", "bar");
    map->addNode(n2);

    return map;
  }

public:

  PoiPolygonMatchVisitorTest() : HootTestFixture("test-files/", UNUSED_PATH)
  {
    setResetType(ResetBasic);
  }

  void runIsCandidateTest()
  {
    std::vector<ConstMatchPtr> result;

    OsmMapPtr map = getTestMap1();
    CPPUNIT_ASSERT(
      PoiPolygonMatchVisitor(map, result).isMatchCandidate(
        map->getNode(
          ElementIdsVisitor::findElementsByTag(map, ElementType::Node, "name", "foo")[0])));
    CPPUNIT_ASSERT(
      !PoiPolygonMatchVisitor(map, result).isMatchCandidate(
        map->getWay(ElementIdsVisitor::findElementsByTag(map, ElementType::Way, "name", "foo")[0])));

    OsmXmlReader reader;
    map = std::make_shared<OsmMap>();
    reader.setDefaultStatus(Status::Unknown1);
    reader.read(_inputPath + "ToyTestA.osm", map);
    MapProjector::projectToPlanar(map);
    CPPUNIT_ASSERT(
      !PoiPolygonMatchVisitor(map, result).isMatchCandidate(
        map->getWay(ElementIdsVisitor::findElementsByTag(map, ElementType::Way, "note", "1")[0])));
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(PoiPolygonMatchVisitorTest, "quick");

}
