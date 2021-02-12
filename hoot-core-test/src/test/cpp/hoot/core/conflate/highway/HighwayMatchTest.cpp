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
 * @copyright Copyright (C) 2014, 2015, 2016, 2017, 2018, 2019, 2020 Maxar (http://www.maxar.com/)
 */

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/algorithms/subline-matching/MaximalNearestSublineMatcher.h>
#include <hoot/core/algorithms/subline-matching/MaximalSublineStringMatcher.h>
#include <hoot/core/conflate/highway/HighwayExpertClassifier.h>
#include <hoot/core/conflate/highway/HighwayMatch.h>
#include <hoot/core/conflate/matching/MatchThreshold.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/elements/MapProjector.h>

// Tgs
#include <tgs/StreamUtils.h>

using namespace geos::geom;

namespace hoot
{

class HighwayMatchTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(HighwayMatchTest);
  CPPUNIT_TEST(runMajorOverlapTest);
  CPPUNIT_TEST(runPartialMatchTest);
  CPPUNIT_TEST(runPartialOverlapTest);
  CPPUNIT_TEST(runSimpleConflictTest);
  CPPUNIT_TEST(runRealWorld1Test);
  CPPUNIT_TEST(runRealWorld2Test);
  CPPUNIT_TEST_SUITE_END();

public:

  HighwayMatchTest()
    : HootTestFixture("test-files/conflate/highway/",
                      UNUSED_PATH)
  {
    setResetType(ResetBasic);
  }

  /**
   * Creates three ways like this:
   *
   * x and o signify Unknown1 and Unknown2, respectively.
   *
   * x-------x
   * o---------o
   *   x-------x
   *
   * Because of the massive overlap, this is a conflict.
   */
  void runMajorOverlapTest()
  {
    OsmMapPtr map(new OsmMap());
    OGREnvelope env;
    env.MinX = 0;
    env.MinY = 0;
    env.MaxX = 1;
    env.MaxY = 1;
    MapProjector::projectToOrthographic(map, env);

    Coordinate w1c[] = { Coordinate(0, 0), Coordinate(90, 0), Coordinate::getNull() };
    WayPtr w1 = TestUtils::createWay(map, Status::Unknown1, w1c);

    Coordinate w2c[] = { Coordinate(0, 5), Coordinate(100, 5), Coordinate::getNull() };
    WayPtr w2 = TestUtils::createWay(map, Status::Unknown2, w2c);

    Coordinate w3c[] = { Coordinate(10, 10), Coordinate(100, 10), Coordinate::getNull() };
    WayPtr w3 = TestUtils::createWay(map, Status::Unknown1, w3c);

    ConstMatchThresholdPtr mt(new MatchThreshold(0.05, 0.6));
    std::shared_ptr<HighwayExpertClassifier> classifier(new HighwayExpertClassifier());
    std::shared_ptr<MaximalSublineStringMatcher> sublineMatcher(new MaximalSublineStringMatcher());
    sublineMatcher->setMinSplitSize(5.0);
    sublineMatcher->setMaxRelevantAngle(toRadians(60.0));
    MatchPtr match12(new HighwayMatch(classifier, sublineMatcher, map, w1->getElementId(), w2->getElementId(),
      mt));
    HOOT_STR_EQUALS("HighwayMatch Way(-1) Way(-2) P: match: 0.129353 miss: 0.870647 review: 0",
                    match12->toString());

    MatchPtr match23(new HighwayMatch(classifier, sublineMatcher, map, w2->getElementId(), w3->getElementId(),
      mt));
    HOOT_STR_EQUALS("HighwayMatch Way(-2) Way(-3) P: match: 0.129353 miss: 0.870647 review: 0",
                    match23->toString());

    CPPUNIT_ASSERT_EQUAL(true, match12->isConflicting(match23, map));
  }

  /**
   * Creates three ways like this:
   *
   * x and o signifiy Unknown1 and Unknown2, respectively.
   *
   * x----x
   * o---------o
   *      x----x
   *
   * While the EIDs match there is no conflict.
   */
  void runPartialMatchTest()
  {
    OsmMapPtr map(new OsmMap());
    OGREnvelope env;
    env.MinX = 0;
    env.MinY = 0;
    env.MaxX = 1;
    env.MaxY = 1;
    MapProjector::projectToOrthographic(map, env);

    Coordinate w1c[] = { Coordinate(0, 0), Coordinate(50, 0), Coordinate::getNull() };
    WayPtr w1 = TestUtils::createWay(map, Status::Unknown1, w1c);

    Coordinate w2c[] = { Coordinate(0, 5), Coordinate(100, 5), Coordinate::getNull() };
    WayPtr w2 = TestUtils::createWay(map, Status::Unknown2, w2c);
    w2->setStatus(Status::Unknown2);

    Coordinate w3c[] = { Coordinate(50, 10), Coordinate(100, 10), Coordinate::getNull() };
    WayPtr w3 = TestUtils::createWay(map, Status::Unknown1, w3c);

    ConstMatchThresholdPtr mt(new MatchThreshold(0.05, 0.9));

    std::shared_ptr<HighwayExpertClassifier> classifier(new HighwayExpertClassifier());
    std::shared_ptr<MaximalSublineStringMatcher> sublineMatcher(new MaximalSublineStringMatcher());
    sublineMatcher->setMinSplitSize(5.0);
    sublineMatcher->setMaxRelevantAngle(toRadians(60.0));
    MatchPtr match12(new HighwayMatch(classifier, sublineMatcher, map, w1->getElementId(), w2->getElementId(),
                         mt));
    HOOT_STR_EQUALS("HighwayMatch Way(-1) Way(-2) P: match: 0.0921884 miss: 0.907812 review: 0",
                    match12->toString());

    MatchPtr match23(new HighwayMatch(classifier, sublineMatcher, map, w2->getElementId(), w3->getElementId(),
                         mt));
    HOOT_STR_EQUALS("HighwayMatch Way(-2) Way(-3) P: match: 0.0921884 miss: 0.907812 review: 0",
                    match23->toString());

    CPPUNIT_ASSERT_EQUAL(false, match12->isConflicting(match23, map));
  }

  /**
   * Creates three ways like this:
   *
   * x and o signifiy Unknown1 and Unknown2, respectively.
   *
   * x-----x
   * o---------o
   *     x-----x
   *
   * While the EIDs match there is no conflict.
   */
  void runPartialOverlapTest()
  {
    OsmMapPtr map(new OsmMap());
    OGREnvelope env;
    env.MinX = 0;
    env.MinY = 0;
    env.MaxX = 1;
    env.MaxY = 1;
    MapProjector::projectToOrthographic(map, env);

    Coordinate w1c[] = { Coordinate(0, 0), Coordinate(60, 0), Coordinate::getNull() };
    WayPtr w1 = TestUtils::createWay(map, Status::Unknown1, w1c);

    Coordinate w2c[] = { Coordinate(0, 5), Coordinate(100, 5), Coordinate::getNull() };
    WayPtr w2 = TestUtils::createWay(map, Status::Unknown2, w2c);

    Coordinate w3c[] = { Coordinate(40, 10), Coordinate(100, 10), Coordinate::getNull() };
    WayPtr w3 = TestUtils::createWay(map, Status::Unknown1, w3c);

    ConstMatchThresholdPtr mt(new MatchThreshold(0.05, 0.95));
    std::shared_ptr<HighwayExpertClassifier> classifier(new HighwayExpertClassifier());
    std::shared_ptr<MaximalSublineStringMatcher> sublineMatcher(new MaximalSublineStringMatcher());
    sublineMatcher->setMinSplitSize(5.0);
    sublineMatcher->setMaxRelevantAngle(toRadians(60.0));
    MatchPtr match12(new HighwayMatch(classifier, sublineMatcher, map, w1->getElementId(), w2->getElementId(),
                         mt));
    HOOT_STR_EQUALS("HighwayMatch Way(-1) Way(-2) P: match: 0.101976 miss: 0.898024 review: 0",
                    match12->toString());

    MatchPtr match23(new HighwayMatch(classifier, sublineMatcher, map, w2->getElementId(), w3->getElementId(),
                         mt));
    HOOT_STR_EQUALS("HighwayMatch Way(-2) Way(-3) P: match: 0.101976 miss: 0.898024 review: 0",
                    match23->toString());

    CPPUNIT_ASSERT_EQUAL(false, match12->isConflicting(match23, map));
  }

  /**
   * This creates one shared way sandwhiched between two other ways. All straight and 100m long.
   * This clearly creates a conflict.
   * x and o signifiy Unknown1 and Unknown2, respectively.
   *
   * x---------x
   * o---------o
   * x---------x
   */
  void runSimpleConflictTest()
  {
    OsmMapPtr map(new OsmMap());
    OGREnvelope env;
    env.MinX = 0;
    env.MinY = 0;
    env.MaxX = 1;
    env.MaxY = 1;
    MapProjector::projectToOrthographic(map, env);

    Coordinate w1c[] = { Coordinate(0, 0), Coordinate(100, 0), Coordinate::getNull() };
    WayPtr w1 = TestUtils::createWay(map, Status::Unknown1, w1c);

    Coordinate w2c[] = { Coordinate(0, 5), Coordinate(100, 5), Coordinate::getNull() };
    WayPtr w2 = TestUtils::createWay(map, Status::Unknown2, w2c);

    Coordinate w3c[] = { Coordinate(0, 10), Coordinate(100, 10), Coordinate::getNull() };
    WayPtr w3 = TestUtils::createWay(map, Status::Unknown1, w3c);

    ConstMatchThresholdPtr mt(new MatchThreshold(0.1, 0.6));
    std::shared_ptr<HighwayExpertClassifier> classifier(new HighwayExpertClassifier());
    std::shared_ptr<MaximalSublineStringMatcher> sublineMatcher(new MaximalSublineStringMatcher());
    sublineMatcher->setMinSplitSize(5.0);
    sublineMatcher->setMaxRelevantAngle(toRadians(60.0));
    MatchPtr match12(new HighwayMatch(classifier, sublineMatcher, map, w1->getElementId(), w2->getElementId(),
                         mt));
    HOOT_STR_EQUALS("HighwayMatch Way(-1) Way(-2) P: match: 0.138093 miss: 0.861907 review: 0",
                    match12->toString());

    MatchPtr match23(new HighwayMatch(classifier, sublineMatcher, map, w2->getElementId(), w3->getElementId(),
                         mt));
    HOOT_STR_EQUALS("HighwayMatch Way(-2) Way(-3) P: match: 0.138093 miss: 0.861907 review: 0",
                    match23->toString());

    CPPUNIT_ASSERT_EQUAL(true, match12->isConflicting(match23, map));
  }

  /**
   * This creates one shared way sandwhiched between two other ways. All straight and 100m long.
   * This clearly creates a conflict.
   * x and o signifiy Unknown1 and Unknown2, respectively.
   *
   *      1
   * x----------x
   * o---o------o
   *   2     3
   */
  void runRealWorld1Test()
  {
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(map, _inputPath + "HighwayMatchRealWorld1Test.osm",
      false);
    MapProjector::projectToOrthographic(map);

    std::shared_ptr<MaximalSublineStringMatcher> sublineMatcher(new MaximalSublineStringMatcher());
    sublineMatcher->setMinSplitSize(5.0);
    sublineMatcher->setMaxRelevantAngle(toRadians(60.0));

    LOG_INFO(
      sublineMatcher->findMatch(
        map,
        std::dynamic_pointer_cast<Way>(TestUtils::getElementWithNote(map, "1")),
        std::dynamic_pointer_cast<Way>(TestUtils::getElementWithNote(map, "3"))));
    HOOT_STR_EQUALS("matches:\n"
      "subline 1: start: way(-3) index: 0 fraction: 0.354292349419726 end: way(-3) index: 1 fraction: 0\n"
      "subline 2: start: way(-2) index: 0 fraction: 0 end: way(-2) index: 1 fraction: 0",
      sublineMatcher->findMatch(
        map,
        std::dynamic_pointer_cast<Way>(TestUtils::getElementWithNote(map, "1")),
        std::dynamic_pointer_cast<Way>(TestUtils::getElementWithNote(map, "3"))));
  }

  /**
   */
  void runRealWorld2Test()
  {
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(map, _inputPath + "HighwayMatchRealWorld2Test.osm",
      false);
    MapProjector::projectToOrthographic(map);

    Settings conf;
    conf.set(ConfigOptions::getWaySublineMatcherKey(), MaximalNearestSublineMatcher::className());

    std::shared_ptr<HighwayExpertClassifier> classifier(new HighwayExpertClassifier());
    std::shared_ptr<MaximalSublineStringMatcher> sublineMatcher(new MaximalSublineStringMatcher());
    sublineMatcher->setConfiguration(conf);
    sublineMatcher->setMinSplitSize(5.0);
    sublineMatcher->setMaxRelevantAngle(toRadians(60.0));

    WayPtr w218 = std::dynamic_pointer_cast<Way>(TestUtils::getElementWithNote(map, "-218"));
    WayPtr w948 = std::dynamic_pointer_cast<Way>(TestUtils::getElementWithNote(map, "-948"));
    WayPtr w582 = std::dynamic_pointer_cast<Way>(TestUtils::getElementWithNote(map, "-582"));

    ConstMatchThresholdPtr mt(new MatchThreshold(0.0001, 0.9999));

    WaySublineMatchString m = sublineMatcher->findMatch(map, w218, w948);

    std::shared_ptr<HighwayMatch> match218v948(new HighwayMatch(classifier, sublineMatcher, map, w218->getElementId(),
      w948->getElementId(), mt));
    std::shared_ptr<HighwayMatch> match218v582(new HighwayMatch(classifier, sublineMatcher, map, w218->getElementId(),
      w582->getElementId(), mt));

    LOG_VAR(match218v948->getSublineMatch());
    LOG_VAR(match218v582->getSublineMatch());
    HOOT_STR_EQUALS(false, match218v948->isConflicting(match218v582, map));
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(HighwayMatchTest, "quick");

}
