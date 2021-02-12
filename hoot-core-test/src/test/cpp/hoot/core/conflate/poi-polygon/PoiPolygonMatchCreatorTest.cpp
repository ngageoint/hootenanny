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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/conflate/matching/Match.h>
#include <hoot/core/conflate/matching/MatchThreshold.h>
#include <hoot/core/conflate/poi-polygon/PoiPolygonMatchCreator.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/visitors/ElementIdsVisitor.h>
#include <hoot/core/util/ConfigOptions.h>

using namespace geos::geom;
using namespace std;

namespace hoot
{

class PoiPolygonMatchCreatorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(PoiPolygonMatchCreatorTest);
  CPPUNIT_TEST(basicTest);
  CPPUNIT_TEST(runIsCandidateTest);
  CPPUNIT_TEST_SUITE_END();

private:

  OsmMapPtr getTestMap1()
  {
    OsmMapPtr map(new OsmMap());

    Coordinate c1[] = { Coordinate(0.0, 0.0), Coordinate(20.0, 0.0),
                        Coordinate(20.0, 20.0), Coordinate(0.0, 20.0),
                        Coordinate(0.0, 0.0),
                        Coordinate::getNull() };
    WayPtr w1 = TestUtils::createWay(map, Status::Unknown1, c1, 5, "w1");
    w1->getTags().set("building", true);
    w1->getTags().set("name", "foo");
    WayPtr w2 = TestUtils::createWay(map, Status::Unknown1, c1, 5, "w2");
    w2->getTags().set("area", true);

    NodePtr n1(new Node(Status::Unknown1, 1, 10, 10, 5));
    n1->getTags().set("poi", true);
    n1->getTags().set("name", "foo");
    map->addNode(n1);
    NodePtr n2(new Node(Status::Unknown2, 2, 5, 10, 5));
    n2->getTags().set("poi", true);
    n2->getTags().set("name", "bar");
    map->addNode(n2);

    return map;
  }

public:

  PoiPolygonMatchCreatorTest()
  {
    setResetType(ResetAll);
  }

  void basicTest()
  {
    // Poi/poly test output is based on this config for now, despite it being different than the
    // default config.
    conf().set(ConfigOptions::getPoiPolygonAddressMatchEnabledKey(), "false");

    OsmMapPtr map = getTestMap1();

    {
      PoiPolygonMatchCreator uut;
      vector<ConstMatchPtr> matches;
      std::shared_ptr<const MatchThreshold> threshold(new MatchThreshold(0.5, 0.5, 0.5));
      uut.createMatches(map, matches, threshold);
      HOOT_STR_EQUALS(2, matches.size());
      HOOT_STR_EQUALS(
        "PoiPolygonMatch: POI: Node(1), Poly: Way(-1), P: match: 1 miss: 0 review: 0, distance: 0, close match: 1, type score: 0, name score: 1, address score: -1",
        matches[0]->toString());
      HOOT_STR_EQUALS(
        "PoiPolygonMatch: POI: Node(2), Poly: Way(-1), P: match: 0 miss: 0 review: 1, distance: 0, close match: 1, type score: 0, name score: 0, address score: 0",
        matches[1]->toString());
    }
  }

  void runIsCandidateTest()
  {
    // see corresponding note in basicTest
    conf().set(ConfigOptions::getPoiPolygonAddressMatchEnabledKey(), "false");

    PoiPolygonMatchCreator uut;

    OsmMapPtr map = getTestMap1();
    CPPUNIT_ASSERT(
      uut.isMatchCandidate(
        map->getNode(
          ElementIdsVisitor::findElementsByTag(map, ElementType::Node, "name", "foo")[0]), map));
    CPPUNIT_ASSERT(
      uut.isMatchCandidate(
        map->getWay(ElementIdsVisitor::findElementsByTag(map, ElementType::Way, "name", "foo")[0]), map));

    OsmXmlReader reader;
    map.reset(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.read("test-files/ToyTestA.osm", map);
    MapProjector::projectToPlanar(map);
    CPPUNIT_ASSERT(
      !uut.isMatchCandidate(
        map->getWay(ElementIdsVisitor::findElementsByTag(map, ElementType::Way, "note", "1")[0]), map));
  }
};

//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(PoiPolygonMatchCreatorTest, "current");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(PoiPolygonMatchCreatorTest, "quick");

}
