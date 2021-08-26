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
#include <hoot/core/conflate/merging/MarkForReviewMerger.h>
#include <hoot/core/conflate/matching/MatchThreshold.h>
#include <hoot/core/conflate/poi-polygon/PoiPolygonMatch.h>
#include <hoot/core/conflate/poi-polygon/PoiPolygonMerger.h>
#include <hoot/core/conflate/poi-polygon/PoiPolygonMergerCreator.h>
#include <hoot/core/conflate/polygon/BuildingMatchCreator.h>
#include <hoot/core/conflate/polygon/BuildingMatch.h>
#include <hoot/core/conflate/polygon/BuildingRfClassifier.h>
#include <hoot/core/io/OsmJsonWriter.h>
#include <hoot/core/util/ConfigOptions.h>

using namespace geos::geom;
using namespace std;

namespace hoot
{

class PoiPolygonMergerCreatorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(PoiPolygonMergerCreatorTest);
  CPPUNIT_TEST(basicTest);
  CPPUNIT_TEST(reviewTest);
  CPPUNIT_TEST(crossConflateMergeTest);
  CPPUNIT_TEST_SUITE_END();

public:

  PoiPolygonMergerCreatorTest()
  {
    // This test passes with ResetEnvironment in parallel but crashes in series without
    // ResetAll.
    setResetType(ResetAll);
  }

  void setUp() override
  {
    HootTestFixture::setUp();
    conf().set(ConfigOptions().getMatchCreatorsKey(), "hoot::PoiPolygonMatchCreator");
    conf().set(ConfigOptions().getMergerCreatorsKey(), "hoot::PoiPolygonMergerCreator");
  }

  void basicTest()
  {
    // Creates a single match and should result in a PoiPolygonMerger

    OsmMapPtr map = std::make_shared<OsmMap>();

    Coordinate c1[] = { Coordinate(0.0, 0.0), Coordinate(20.0, 0.0),
                        Coordinate(20.0, 20.0), Coordinate(0.0, 20.0),
                        Coordinate(0.0, 0.0),
                        Coordinate::getNull() };
    WayPtr w1 = TestUtils::createWay(map, c1, "w1", Status::Unknown1, 5);
    w1->getTags().set("area", true);
    w1->getTags()["name"] = "foo";
    w1->getTags()["amenity"] = "bar";
    NodePtr n1 = std::make_shared<Node>(Status::Unknown2, 1, 10, 10, 5);
    n1->getTags()["name"] = "bar";
    n1->getTags()["amenity"] = "cafe";
    map->addNode(n1);

    std::shared_ptr<PoiPolygonMatch> match1 =
      std::make_shared<PoiPolygonMatch>(
        map, std::shared_ptr<MatchThreshold>(), std::shared_ptr<PoiPolygonRfClassifier>(),
        std::make_shared<PoiPolygonInfoCache>(map));
    match1->setMatchEvidenceThreshold(3);
    match1->setReviewEvidenceThreshold(1);
    match1->calculateMatch(w1->getElementId(), n1->getElementId());

    MatchSet matches;
    matches.insert(match1);
    vector<MergerPtr> mergers;
    PoiPolygonMergerCreator uut;
    uut.setOsmMap(map.get());
    HOOT_STR_EQUALS(1, uut.createMergers(matches, mergers));
    HOOT_STR_EQUALS(1, mergers.size());
    HOOT_STR_EQUALS(1, (dynamic_pointer_cast<PoiPolygonMerger>(mergers[0]) != 0));
  }

  void reviewTest()
  {
    // Create a building and poi/poly match with feature overlap and ensure they create reviews and
    // don't merge together when cross feature conflate merging is not allowed.

    OsmMapPtr map = std::make_shared<OsmMap>();

    Coordinate c1[] = { Coordinate(0.0, 0.0), Coordinate(20.0, 0.0),
                        Coordinate(20.0, 20.0), Coordinate(0.0, 20.0),
                        Coordinate(0.0, 0.0),
                        Coordinate::getNull() };
    WayPtr w1 = TestUtils::createWay(map, c1, "w1", Status::Unknown1, 5);
    w1->getTags().set("building", true);
    w1->getTags()["name"] = "foo";
    w1->getTags()["amenity"] = "bar";

    Coordinate c2[] = { Coordinate(0.0, 0.0), Coordinate(5.0, 0.0),
                        Coordinate(5.0, 5.0), Coordinate(0.0, 5.0),
                        Coordinate(0.0, 0.0),
                        Coordinate::getNull() };
    WayPtr w2 = TestUtils::createWay(map, c2, "w2", Status::Unknown2, 5);
    w2->getTags().set("building", true);
    w2->getTags()["name"] = "goofie";

    NodePtr n1 = std::make_shared<Node>(Status::Unknown2, 1, 19, 19, 5);
    n1->getTags()["name"] = "foo";
    n1->getTags()["amenity"] = "cafe";
    map->addNode(n1);

    vector<ConstMatchPtr> matchesV;

    std::shared_ptr<PoiPolygonMatch> match1 =
      std::make_shared<PoiPolygonMatch>(
        map, std::shared_ptr<MatchThreshold>(), std::shared_ptr<PoiPolygonRfClassifier>(),
        std::make_shared<PoiPolygonInfoCache>(map));
    match1->setMatchEvidenceThreshold(3);
    match1->setReviewEvidenceThreshold(1);
    match1->calculateMatch(w1->getElementId(), n1->getElementId());
    matchesV.push_back(match1);
    std::shared_ptr<const MatchThreshold> threshold =
      std::make_shared<MatchThreshold>(0.5, 0.5, 0.5);
    BuildingMatchCreator().createMatches(map, matchesV, threshold);

    std::shared_ptr<PoiPolygonMatch> match2 =
      std::make_shared<PoiPolygonMatch>(
        map, std::shared_ptr<MatchThreshold>(), std::shared_ptr<PoiPolygonRfClassifier>(),
        std::make_shared<PoiPolygonInfoCache>(map));
    match2->setMatchEvidenceThreshold(3);
    match2->setReviewEvidenceThreshold(1);
    match2->calculateMatch(w2->getElementId(), n1->getElementId());
    matchesV.push_back(match2);
    LOG_VAR(match2);

    MatchSet matches;
    matches.insert(matchesV.begin(), matchesV.end());
    vector<MergerPtr> mergers;
    PoiPolygonMergerCreator uut;
    uut.setOsmMap(map.get());

    HOOT_STR_EQUALS(1, uut.createMergers(matches, mergers));
    HOOT_STR_EQUALS(1, mergers.size());
    LOG_VART(*mergers[0]);
    HOOT_STR_EQUALS(1, (dynamic_pointer_cast<MarkForReviewMerger>(mergers[0]) != 0));
  }

  void crossConflateMergeTest()
  {
    // Create a building and poi/poly match with feature overlap and ensure they all merge together
    // when cross feature conflate merging is allowed.

    vector<ConstMatchPtr> matchesV;

    std::shared_ptr<BuildingMatch> match1 =
      std::make_shared<BuildingMatch>(std::make_shared<const MatchThreshold>(0.5, 0.5, 0.5));
    match1->_p.setMatch();
    match1->_eid1 = ElementId(ElementType::Way, 1);
    match1->_eid2 = ElementId(ElementType::Node, 1);
    matchesV.push_back(match1);

    std::shared_ptr<PoiPolygonMatch> match2(
      new PoiPolygonMatch(std::shared_ptr<const MatchThreshold>(new MatchThreshold(0.6, 0.6, 0.6))));
    match2->_class.setMatch();
    match2->_eid1 = ElementId(ElementType::Way, 2);
    match2->_eid2 = ElementId(ElementType::Node, 1);
    matchesV.push_back(match2);

    MatchSet matches;
    matches.insert(matchesV.begin(), matchesV.end());
    vector<MergerPtr> mergers;
    PoiPolygonMergerCreator uut;
    // Neither of the match types used here actually require a map to calculate isConflicting, but
    // since the merger creator requires a map we'll pass in an empty one.
    OsmMapPtr emptyMap = std::make_shared<OsmMap>();
    uut.setOsmMap(emptyMap.get());
    uut.setAllowCrossConflationMerging(true);

    HOOT_STR_EQUALS(1, uut.createMergers(matches, mergers));
    HOOT_STR_EQUALS(1, mergers.size());
    LOG_VART(*mergers[0]);
    HOOT_STR_EQUALS(1, (dynamic_pointer_cast<PoiPolygonMerger>(mergers[0]) != 0));
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(PoiPolygonMergerCreatorTest, "slow");

}
