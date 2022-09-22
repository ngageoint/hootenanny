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
 * @copyright Copyright (C) 2013, 2014, 2015, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/conflate/matching/MatchThreshold.h>
#include <hoot/core/conflate/polygon/BuildingMatch.h>
#include <hoot/core/conflate/polygon/BuildingMatchCreator.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/elements/MapUtils.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/ops/RemoveWayByEid.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/visitors/ElementIdsVisitor.h>

// Tgs
#include <tgs/StreamUtils.h>

using namespace std;

namespace hoot
{

class BuildingMatchCreatorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(BuildingMatchCreatorTest);
  CPPUNIT_TEST(runMatchTest);
  CPPUNIT_TEST(runIsCandidateTest);
  CPPUNIT_TEST(runReviewIfSecondaryNewerTest);
  CPPUNIT_TEST(runReviewIfSecondaryNewer2Test);
  CPPUNIT_TEST(runReviewIfSecondaryNewerMismatchingDateKeyTest);
  CPPUNIT_TEST(runReviewIfSecondaryNewerBadDateFormatTest);
  CPPUNIT_TEST(runReviewNonOneToOneMatchesTest);
  CPPUNIT_TEST(runReviewNonOneToOneMatches2Test);
  CPPUNIT_TEST_SUITE_END();

public:

  BuildingMatchCreatorTest()
    : HootTestFixture("test-files/", UNUSED_PATH)
  {
    setResetType(ResetEnvironment);
  }

  ConstWayPtr getWay(ConstOsmMapPtr map, const QString& key, const QString& value)
  {
    std::vector<long> wids = ElementIdsVisitor::findElementsByTag(map, ElementType::Way, key, value);
    CPPUNIT_ASSERT_EQUAL((size_t)1, wids.size());
    return map->getWay(wids[0]);
  }

  bool contains(const vector<ConstMatchPtr>& matches, ElementId eid1, ElementId eid2)
  {
    for (const auto& match : matches)
    {
      set<pair<ElementId, ElementId>> s = match->getMatchPairs();
      if (match->getProbability() > 0.5)
      {
        for (const auto& p : s)
        {
          if (p.first == eid1 && p.second == eid2)
            return true;
        }
      }
    }
    return false;
  }

  OsmMapPtr getTestMap(const bool targetWaysOnly = true)
  {
    OsmXmlReader reader;
    OsmMapPtr map = std::make_shared<OsmMap>();
    reader.setDefaultStatus(Status::Unknown1);
    reader.read(_inputPath + "ToyBuildingsTestA.osm", map);
    reader.setDefaultStatus(Status::Unknown2);
    reader.read(_inputPath + "ToyBuildingsTestB.osm", map);
    MapProjector::projectToPlanar(map);

    LOG_VARD(targetWaysOnly);
    if (targetWaysOnly)
    {
      //  Make a copy of the ways to remove some later
      WayMap wm = map->getWays();
      for (WayMap::const_iterator it = wm.begin(); it != wm.end(); ++it)
      {
        const ConstWayPtr& w = it->second;
        const Tags& t = w->getTags();
        //remove some ways we don't need for some of these tests
        if (t[MetadataTags::Ref1()] != "Target" && t[MetadataTags::Ref2()] != "Target")
          RemoveWayByEid::removeWay(map, it->first);
      }
    }
    return map;
  }

  void runMatchTest()
  {
    OsmMapPtr map = getTestMap();

    BuildingMatchCreator uut;
    vector<ConstMatchPtr> matches;

    std::shared_ptr<const MatchThreshold> threshold = std::make_shared<MatchThreshold>(0.6, 0.6);
    uut.createMatches(map, matches, threshold);

    CPPUNIT_ASSERT_EQUAL(3, int(matches.size()));
    CPPUNIT_ASSERT_EQUAL(true, contains(matches, ElementId::way(-7), ElementId::way(-15)));
    CPPUNIT_ASSERT_EQUAL(true, contains(matches, ElementId::way(-7), ElementId::way(-14)));
    CPPUNIT_ASSERT_EQUAL(true, contains(matches, ElementId::way(-7), ElementId::way(-13)));
  }

  void runIsCandidateTest()
  {
    BuildingMatchCreator uut;

    OsmXmlReader reader;
    OsmMapPtr map = std::make_shared<OsmMap>();

    reader.setDefaultStatus(Status::Unknown1);
    reader.read(_inputPath + "ToyBuildingsTestA.osm", map);
    MapProjector::projectToPlanar(map);

    CPPUNIT_ASSERT(
      uut.isMatchCandidate(
        map->getWay(ElementIdsVisitor::findElementsByTag(map, ElementType::Way, "name", "Panera Bread")[0]), map));

    map = std::make_shared<OsmMap>();
    reader.setDefaultStatus(Status::Unknown1);
    reader.read(_inputPath + "ToyTestA.osm", map);
    MapProjector::projectToPlanar(map);

    CPPUNIT_ASSERT(
      !uut.isMatchCandidate(
        map->getWay(ElementIdsVisitor::findElementsByTag(map, ElementType::Way, "note", "1")[0]), map));
  }

  void runReviewIfSecondaryNewerTest()
  {
    OsmMapPtr map = getTestMap();

    //set date tags on a primary and ref feature that will match, making the date on the secondary
    //feature newer - should trigger a review
    //ref feature
    MapUtils::getFirstElementWithTag(map, "name", "Target")->getTags()
      .appendValue("source:date", "2018-02-14T10:55");
    //secondary feature
    MapUtils::getFirstElementWithTag(map, "name", "Target Pharmacy")->getTags()
      .appendValue("source:date", "2018-02-14T10:56");

    conf().set(ConfigOptions::getBuildingDateFormatKey(), "yyyy-MM-ddTHH:mm");
    conf().set(ConfigOptions::getBuildingDateTagKeyKey(), "source:date");
    conf().set(ConfigOptions::getBuildingReviewIfSecondaryNewerKey(), "true");

    BuildingMatchCreator uut;
    vector<ConstMatchPtr> matches;
    std::shared_ptr<const MatchThreshold> threshold = std::make_shared<MatchThreshold>(0.6, 0.6);
    uut.createMatches(map, matches, threshold);
    LOG_VARD(matches);

    /*
     * matches: [3]{
     * BuildingMatch: Way:-7, Way:-15 p: match: 0.9 miss: 0.1 review: 0,
     * BuildingMatch: Way:-7, Way:-14 p: match: 0.925 miss: 0.075 review: 0,
     * BuildingMatch: Way:-7, Way:-13 p: match: 0.825 miss: 0.15 review: 0.025}
     *
     * map->getElement(ElementId::way(-7))->getTags().get("name"): Target
       map->getElement(ElementId::way(-15))->getTags().get("name"): Target Pharmacy
       map->getElement(ElementId::way(-14))->getTags().get("name"): Target - Aurora South
       map->getElement(ElementId::way(-13))->getTags().get("name"): Target Grocery
     */

    CPPUNIT_ASSERT_EQUAL(3, int(matches.size()));
    for (const auto& match : matches)
    {
      std::set<std::pair<ElementId, ElementId>> matchPairs = match->getMatchPairs();
      LOG_VART(matchPairs.size());
      assert(matchPairs.size() == 1);
      ElementId refId = matchPairs.begin()->first;
      ElementId secId = matchPairs.begin()->second;
      LOG_VART(refId);
      LOG_VART(secId);
      assert(refId.getId() == -7);
      if (secId.getId() == -15)
      {
        CPPUNIT_ASSERT_EQUAL(0.0, match->getClassification().getMatchP());
        CPPUNIT_ASSERT_EQUAL(0.0, match->getClassification().getMissP());
        CPPUNIT_ASSERT_EQUAL(1.0, match->getClassification().getReviewP());
      }
      else if (secId.getId() == -14 || secId.getId() == -13)
      {
        CPPUNIT_ASSERT(match->getClassification().getReviewP() != 1.0);
      }
    }
  }

  void runReviewIfSecondaryNewer2Test()
  {
    OsmMapPtr map = getTestMap();

    //set date tags on a primary and ref feature that will match, making the date on the ref
    //feature newer - should not trigger a review
    //ref feature
    MapUtils::getFirstElementWithTag(map, "name", "Target")->getTags()
      .appendValue("source:date", "2018-02-14T10:56");
    //secondary feature
    MapUtils::getFirstElementWithTag(map, "name", "Target Pharmacy")->getTags()
      .appendValue("source:date", "2018-02-14T10:55");

    conf().set(ConfigOptions::getBuildingDateFormatKey(), "yyyy-MM-ddTHH:mm");
    conf().set(ConfigOptions::getBuildingDateTagKeyKey(), "source:date");
    conf().set(ConfigOptions::getBuildingReviewIfSecondaryNewerKey(), "true");

    BuildingMatchCreator uut;
    vector<ConstMatchPtr> matches;
    std::shared_ptr<const MatchThreshold> threshold = std::make_shared<MatchThreshold>(0.6, 0.6);
    uut.createMatches(map, matches, threshold);

    CPPUNIT_ASSERT_EQUAL(3, int(matches.size()));
    CPPUNIT_ASSERT_EQUAL(true, contains(matches, ElementId::way(-7), ElementId::way(-15)));
    CPPUNIT_ASSERT_EQUAL(true, contains(matches, ElementId::way(-7), ElementId::way(-14)));
    CPPUNIT_ASSERT_EQUAL(true, contains(matches, ElementId::way(-7), ElementId::way(-13)));
  }

  void runReviewIfSecondaryNewerMismatchingDateKeyTest()
  {
    OsmMapPtr map = getTestMap();

    //make the date tag added to one of the features have a different key than what is expected -
    //should not trigger a review
    //ref feature
    MapUtils::getFirstElementWithTag(map, "name", "Target")->getTags()
      .appendValue("source:date", "2018-02-14T10:56");
    //secondary feature
    MapUtils::getFirstElementWithTag(map, "name", "Target Pharmacy")->getTags()
      .appendValue("date", "2018-02-14T10:55");

    conf().set(ConfigOptions::getBuildingDateFormatKey(), "yyyy-MM-ddTHH:mm");
    conf().set(ConfigOptions::getBuildingDateTagKeyKey(), "source:date");
    conf().set(ConfigOptions::getBuildingReviewIfSecondaryNewerKey(), "true");

    BuildingMatchCreator uut;
    vector<ConstMatchPtr> matches;
    std::shared_ptr<const MatchThreshold> threshold = std::make_shared<MatchThreshold>(0.6, 0.6);
    uut.createMatches(map, matches, threshold);

    CPPUNIT_ASSERT_EQUAL(3, int(matches.size()));
    CPPUNIT_ASSERT_EQUAL(true, contains(matches, ElementId::way(-7), ElementId::way(-15)));
    CPPUNIT_ASSERT_EQUAL(true, contains(matches, ElementId::way(-7), ElementId::way(-14)));
    CPPUNIT_ASSERT_EQUAL(true, contains(matches, ElementId::way(-7), ElementId::way(-13)));
  }

  void runReviewIfSecondaryNewerBadDateFormatTest()
  {
    OsmMapPtr map = getTestMap();

    //make the date tag added to one of the features have a date format that is unexpected
    //ref feature
    MapUtils::getFirstElementWithTag(map, "name", "Target")->getTags()
      .appendValue("source:date", "2018-02-14T10:56:00");
    //secondary feature
    MapUtils::getFirstElementWithTag(map, "name", "Target Pharmacy")->getTags()
      .appendValue("source:date", "2018-02-14T10:55");

    conf().set(ConfigOptions::getBuildingDateFormatKey(), "yyyy-MM-ddTHH:mm");
    conf().set(ConfigOptions::getBuildingDateTagKeyKey(), "source:date");
    conf().set(ConfigOptions::getBuildingReviewIfSecondaryNewerKey(), "true");

    BuildingMatchCreator uut;
    vector<ConstMatchPtr> matches;
    std::shared_ptr<const MatchThreshold> threshold = std::make_shared<MatchThreshold>(0.6, 0.6);

    QString exceptionMsg("");
    try
    {
      uut.createMatches(map, matches, threshold);
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT(exceptionMsg.contains("Invalid configured building date format"));
  }

  void runReviewNonOneToOneMatchesTest()
  {
    OsmMapPtr map = getTestMap();

    conf().set(ConfigOptions::getBuildingReviewMatchesOtherThanOneToOneKey(), "true");

    BuildingMatchCreator uut;
    vector<ConstMatchPtr> matches;
    std::shared_ptr<const MatchThreshold> threshold = std::make_shared<MatchThreshold>(0.6, 0.6);
    uut.createMatches(map, matches, threshold);
    LOG_VARD(matches);

    CPPUNIT_ASSERT_EQUAL(3, int(matches.size()));
    for (const auto& match : matches)
      CPPUNIT_ASSERT_EQUAL(1.0, match->getClassification().getReviewP());
  }

  void runReviewNonOneToOneMatches2Test()
  {
    //test to prove that 1:1 matches are preserved when
    //building.review.matches.other.than.one.to.one is enabled

    OsmMapPtr map = getTestMap(false);

    conf().set(ConfigOptions::getBuildingReviewMatchesOtherThanOneToOneKey(), "true");

    BuildingMatchCreator uut;
    vector<ConstMatchPtr> matches;
    std::shared_ptr<const MatchThreshold> threshold = std::make_shared<MatchThreshold>(0.6, 0.6);
    uut.createMatches(map, matches, threshold);
    LOG_VARD(matches);

    /*
     * with setting disabled:
     *
     * {
        BuildingMatch: Way:-10, Way:-11 p: match: 0.775 miss: 0.125 review: 0.1,
        BuildingMatch: Way:-9, Way:-12 p: match: 0.8 miss: 0.2 review: 0,
        BuildingMatch: Way:-7, Way:-15 p: match: 0.9 miss: 0.1 review: 0,
        BuildingMatch: Way:-7, Way:-14 p: match: 0.925 miss: 0.075 review: 0,
        BuildingMatch: Way:-7, Way:-13 p: match: 0.825 miss: 0.15 review: 0.025,
        BuildingMatch: Way:-1, Way:-18 p: match: 0.825 miss: 0.175 review: 0,
        BuildingMatch: Way:-1, Way:-17 p: match: 0.95 miss: 0.025 review: 0.025}
     */

    CPPUNIT_ASSERT_EQUAL(7, int(matches.size()));
    for (const auto& match : matches)
    {
      std::set<std::pair<ElementId, ElementId>> matchPairs = match->getMatchPairs();
      LOG_VART(matchPairs.size());
      assert(matchPairs.size() == 1);
      ElementId refId = matchPairs.begin()->first;
      LOG_VART(refId);
      //these were involved in non 1:1 matches
      if (refId.getId() == -7 || refId.getId() == -1)
      {
        CPPUNIT_ASSERT_EQUAL(0.0, match->getClassification().getMatchP());
        CPPUNIT_ASSERT_EQUAL(0.0, match->getClassification().getMissP());
        CPPUNIT_ASSERT_EQUAL(1.0, match->getClassification().getReviewP());
      }
      //everything else was only involved in 1:1 matches
      else
      {
        CPPUNIT_ASSERT(match->getClassification().getMatchP() >=
                       uut.getMatchThreshold()->getMatchThreshold());
      }
    }
  }
};

//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(BuildingMatchCreatorTest, "current");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(BuildingMatchCreatorTest, "quick");

}
