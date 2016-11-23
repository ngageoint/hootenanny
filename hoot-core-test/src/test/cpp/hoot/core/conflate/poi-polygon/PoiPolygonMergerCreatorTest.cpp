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
#include "../../TestUtils.h"
#include <hoot/core/conflate/MarkForReviewMerger.h>
#include <hoot/core/conflate/MatchThreshold.h>
#include <hoot/core/conflate/polygon/BuildingMatchCreator.h>
#include <hoot/core/io/OsmJsonWriter.h>
#include <hoot/core/conflate/poi-polygon/PoiPolygonMatch.h>
#include <hoot/core/conflate/poi-polygon/PoiPolygonMerger.h>
#include <hoot/core/conflate/poi-polygon/PoiPolygonMergerCreator.h>
#include <hoot/core/util/ConfigOptions.h>

namespace hoot
{

class PoiPolygonMergerCreatorTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(PoiPolygonMergerCreatorTest);
  CPPUNIT_TEST(basicTest);
  CPPUNIT_TEST(reviewTest);
  CPPUNIT_TEST_SUITE_END();

public:

  /**
   * Creates a single match and should result in a PoiPolygonMerger
   */
  void basicTest()
  {
    OsmMap::resetCounters();
    OsmMapPtr map(new OsmMap());

    Coordinate c1[] = { Coordinate(0.0, 0.0), Coordinate(20.0, 0.0),
                        Coordinate(20.0, 20.0), Coordinate(0.0, 20.0),
                        Coordinate(0.0, 0.0),
                        Coordinate::getNull() };
    WayPtr w1 = TestUtils::createWay(map, Status::Unknown1, c1, 5, "w1");
    w1->getTags().set("area", true);
    w1->getTags()["name"] = "foo";
    w1->getTags()["amenity"] = "bar";
    NodePtr n1(new Node(Status::Unknown2, 1, 10, 10, 5));
    n1->getTags()["name"] = "bar";
    n1->getTags()["amenity"] = "cafe";
    map->addNode(n1);

    PoiPolygonMatch match1(
      map, w1->getElementId(), n1->getElementId(), shared_ptr<MatchThreshold>(),
      shared_ptr<PoiPolygonRfClassifier>());
    MatchSet matches;
    matches.insert(&match1);
    vector<Merger*> mergers;
    PoiPolygonMergerCreator uut;
    uut.setOsmMap(map.get());
    HOOT_STR_EQUALS(1, uut.createMergers(matches, mergers));
    HOOT_STR_EQUALS(1, mergers.size());
    HOOT_STR_EQUALS(1, (dynamic_cast<PoiPolygonMerger*>(mergers[0]) != 0));
  }

  /**
   * Creates two matches with overlap and should create a MarkForReviewMerger
   */
  void reviewTest()
  {
    OsmMap::resetCounters();
    OsmMapPtr map(new OsmMap());

    Coordinate c1[] = { Coordinate(0.0, 0.0), Coordinate(20.0, 0.0),
                        Coordinate(20.0, 20.0), Coordinate(0.0, 20.0),
                        Coordinate(0.0, 0.0),
                        Coordinate::getNull() };
    WayPtr w1 = TestUtils::createWay(map, Status::Unknown1, c1, 5, "w1");
    w1->getTags().set("building", true);
    w1->getTags()["name"] = "foo";
    w1->getTags()["amenity"] = "bar";

    Coordinate c2[] = { Coordinate(0.0, 0.0), Coordinate(5.0, 0.0),
                        Coordinate(5.0, 5.0), Coordinate(0.0, 5.0),
                        Coordinate(0.0, 0.0),
                        Coordinate::getNull() };
    WayPtr w2 = TestUtils::createWay(map, Status::Unknown2, c2, 5, "w2");
    w2->getTags().set("building", true);
    w2->getTags()["name"] = "goofie";

    NodePtr n1(new Node(Status::Unknown2, 1, 19, 19, 5));
    n1->getTags()["name"] = "foo";
    n1->getTags()["amenity"] = "cafe";
    map->addNode(n1);

    vector<const Match*> matchesV;
    PoiPolygonMatch match1(map, w1->getElementId(), n1->getElementId(), shared_ptr<MatchThreshold>(),
                           shared_ptr<PoiPolygonRfClassifier>());
    matchesV.push_back(&match1);
    shared_ptr<const MatchThreshold> threshold(new MatchThreshold(0.5, 0.5, 0.5));
    BuildingMatchCreator().createMatches(map, matchesV, threshold);

    PoiPolygonMatch match2(map, w2->getElementId(), n1->getElementId(), shared_ptr<MatchThreshold>(),
                           shared_ptr<PoiPolygonRfClassifier>());
    LOG_VAR(match2);

    MatchSet matches;
    matches.insert(matchesV.begin(), matchesV.end());
    vector<Merger*> mergers;
    PoiPolygonMergerCreator uut;
    uut.setOsmMap(map.get());
    HOOT_STR_EQUALS(1, uut.createMergers(matches, mergers));
    HOOT_STR_EQUALS(1, mergers.size());
    LOG_VAR(*mergers[0]);
    HOOT_STR_EQUALS(1, (dynamic_cast<MarkForReviewMerger*>(mergers[0]) != 0));
  }

  void setUp()
  {
    TestUtils::resetEnvironment();
    conf().set(ConfigOptions().getMatchCreatorsKey(), "hoot::PoiPolygonMatchCreator");
    conf().set(ConfigOptions().getMergerCreatorsKey(), "hoot::PoiPolygonMergerCreator");
  }

  void tearDown()
  {
    TestUtils::resetEnvironment();
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(PoiPolygonMergerCreatorTest, "slow");

}
