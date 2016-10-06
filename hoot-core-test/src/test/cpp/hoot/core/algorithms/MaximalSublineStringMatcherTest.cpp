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

// GEOS
#include <geos/geom/LineString.h>

// Hoot
#include <hoot/core/MapProjector.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/algorithms/MaximalSublineStringMatcher.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/visitors/FindWaysVisitor.h>
using namespace hoot;


// Boost
using namespace boost;

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Qt
#include <QDebug>
#include <QDir>

// Standard
#include <sstream>
#include <iomanip>
using namespace std;

#include "../TestUtils.h"

namespace hoot
{

class MaximalSublineStringMatcherTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(MaximalSublineStringMatcherTest);
  CPPUNIT_TEST(evaluateMatchTest);
  CPPUNIT_TEST(runFindMatchTest);
  CPPUNIT_TEST(runBackwardsPartialTest);
  CPPUNIT_TEST(runBackwardsPartial2Test);
  CPPUNIT_TEST(runEndSnapTest);
  CPPUNIT_TEST_SUITE_END();

public:

  OsmMapPtr createMap()
  {
    OsmMap::resetCounters();
    shared_ptr<OsmMap> map(new OsmMap());
    OGREnvelope env;
    env.MinX = 0;
    env.MinY = 0;
    env.MaxX = 1;
    env.MaxY = 1;
    MapProjector::projectToOrthographic(map, env);

    return map;
  }

  WayPtr createWay(OsmMapPtr map, Status s, Coordinate c[], Meters ce, QString note)
  {
    WayPtr result(new Way(s, map->createNextWayId(), ce));
    for (size_t i = 0; c[i].isNull() == false; i++)
    {
      NodePtr n(new Node(s, map->createNextNodeId(), c[i], ce));
      map->addNode(n);
      result->addNode(n->getId());
    }

    result->getTags().addNote(note);
    map->addWay(result);
    return result;
  }

  OsmMapPtr createMatchTestMap()
  {
    OsmMapPtr map(new OsmMap());
    OsmMap::resetCounters();

    OGREnvelope env;
    env.MinX = 0;
    env.MaxX = 1;
    env.MinY = 0;
    env.MaxY = 1;
    MapProjector::projectToPlanar(map, env);

    // Many of these scenarios are taken directly from "Hootenanny - Multilinestring *.pptx"

    // the simplest form of match.
    // 2 ---------
    // 1 ---------
    Coordinate ca1[] = { Coordinate(0, 0), Coordinate(100, 0), Coordinate::getNull() };
    createWay(map, Status::Unknown1, ca1, 15.0, "a1");

    Coordinate ca2[] = { Coordinate(0, 5), Coordinate(100, 5), Coordinate::getNull() };
    createWay(map, Status::Unknown2, ca2, 15.0, "a2");

    // Multiple Partial Matches
    //     +--+
    // 2 --+  +---
    // 1 ---------
    Coordinate cb1[] = { Coordinate(100, 0), Coordinate(100, 100), Coordinate::getNull() };
    createWay(map, Status::Unknown1, cb1, 15.0, "b1");

    Coordinate cb2[] = { Coordinate(105, 0), Coordinate(105, 40),
                         Coordinate(150, 40), Coordinate(150, 60),
                         Coordinate(105, 60), Coordinate(105, 100),
                         Coordinate::getNull() };
    createWay(map, Status::Unknown2, cb2, 15.0, "b2");

    // Offset Partial Match
    // 2     ---------
    // 1 ---------
    Coordinate cc1[] = { Coordinate(200, 0), Coordinate(200, 100), Coordinate::getNull() };
    createWay(map, Status::Unknown1, cc1, 15.0, "c1");

    Coordinate cc2[] = { Coordinate(200, 50), Coordinate(200, 150), Coordinate::getNull() };
    createWay(map, Status::Unknown2, cc2, 15.0, "c2");

    // Centered Partial Match
    // 2   ----
    // 1 ---------
    Coordinate cd1[] = { Coordinate(300, 0), Coordinate(300, 100), Coordinate::getNull() };
    createWay(map, Status::Unknown1, cd1, 15.0, "d1");

    Coordinate cd2[] = { Coordinate(300, 30), Coordinate(300, 70), Coordinate::getNull() };
    createWay(map, Status::Unknown2, cd2, 15.0, "d2");

    // Multiple Matches
    // 2 ------ ----
    // 1   ---- -------
    Coordinate ce1a[] = { Coordinate(400, 50), Coordinate(400, 100), Coordinate::getNull() };
    createWay(map, Status::Unknown1, ce1a, 15.0, "e1");

    Coordinate ce1b[] = { Coordinate(400, 120), Coordinate(400, 200), Coordinate::getNull() };
    createWay(map, Status::Unknown1, ce1b, 15.0, "e1");

    Coordinate ce2a[] = { Coordinate(400, 0), Coordinate(400, 100), Coordinate::getNull() };
    createWay(map, Status::Unknown2, ce2a, 15.0, "e2");

    Coordinate ce2b[] = { Coordinate(400, 120), Coordinate(400, 150), Coordinate::getNull() };
    createWay(map, Status::Unknown2, ce2b, 15.0, "e2");

    // Offset Partial Match - two parts
    // 2    ---x-------
    // 1 -----------
    Coordinate cf1[] = { Coordinate(500, 0), Coordinate(500, 100), Coordinate::getNull() };
    createWay(map, Status::Unknown1, cf1, 15.0, "f1");

    Coordinate cf2a[] = { Coordinate(500, 50), Coordinate(500, 70), Coordinate::getNull() };
    createWay(map, Status::Unknown2, cf2a, 15.0, "f2");

    Coordinate cf2b[] = { Coordinate(500, 70), Coordinate(500, 150), Coordinate::getNull() };
    createWay(map, Status::Unknown2, cf2b, 15.0, "f2");

    // U-Match - two parts
    // 2   -----------x
    //                |
    //     -----------+
    // 1 -----------+
    //              |
    //   -----------+
    double dx = 600;
    Coordinate cg1[] = { Coordinate(dx + 0, 0), Coordinate(dx + 100, 0),
                         Coordinate(dx + 100, 10), Coordinate(dx + 0, 10),
                         Coordinate::getNull() };
    createWay(map, Status::Unknown1, cg1, 15.0, "g1");

    double err = 0;
    Coordinate cg2a[] = { Coordinate(dx + 0 + err, 0 + err), Coordinate(dx + 100 + err, 0 + err),
                          Coordinate(dx + 100 + err, 10 + err), Coordinate::getNull() };
    WayPtr wg2a = createWay(map, Status::Unknown2, cg2a, 15.0, "g2");

    Coordinate cg2b[] = { Coordinate(dx + 100 + err, 10 + err),
                          Coordinate(dx + 0 + err, 10 + err),
                          Coordinate::getNull() };
    WayPtr wg2b = createWay(map, Status::Unknown2, cg2b, 15.0, "g2");

    RelationPtr r(new Relation(Status::Unknown1, map->createNextRelationId(), 5,
      "multilinestring"));
    r->getTags()["note"] = "rg2";
    r->addElement("", wg2a);
    r->addElement("", wg2b);
    map->addElement(r);

    // U-Match - two parts, on backwards
    // 2   ----->-----x
    //                |
    //     ----->-----+
    // 1 -----<-----+
    //              |
    //   ----->-----+
    dx = 700;
    double ce = 3;
    Coordinate ch1[] = { Coordinate(dx + 0, 0), Coordinate(dx + 100, 0),
                         Coordinate(dx + 100, 10), Coordinate(dx + 0, 10),
                         Coordinate::getNull() };
    createWay(map, Status::Unknown1, ch1, ce, "h1");

    err = 0;
    Coordinate ch2a[] = { Coordinate(dx + 0 + err, 0 + err), Coordinate(dx + 100 + err, 0 + err),
                          Coordinate(dx + 100 + err, 10 + err), Coordinate::getNull() };
    WayPtr wh2a = createWay(map, Status::Unknown2, ch2a, ce, "h2");

    Coordinate ch2b[] = { Coordinate(dx + 0 + err, 10 + err),
                          Coordinate(dx + 100 + err, 10 + err),
                          Coordinate::getNull() };
    WayPtr wh2b = createWay(map, Status::Unknown2, ch2b, ce, "h2");

    RelationPtr rh2(new Relation(Status::Unknown1, map->createNextRelationId(), 3,
      "multilinestring"));
    rh2->getTags()["note"] = "rh2";
    rh2->addElement("", wh2a);
    rh2->addElement("", wh2b);
    map->addElement(rh2);

    // U-Match - two parts, one backwards and partial
    // 2 -------->------x
    //                  |
    //       ----->-----+
    // 1   -----<-----+
    //                |
    //     ----->-----+
    dx = 800;
    ce = 3;
    Coordinate ci1[] = { Coordinate(dx + 0, 0), Coordinate(dx + 100, 0),
                         Coordinate(dx + 100, 10), Coordinate(dx + 0, 10),
                         Coordinate::getNull() };
    createWay(map, Status::Unknown1, ci1, ce, "i1");

    err = 0;
    Coordinate ci2a[] = { Coordinate(dx + 0 + err, 0 + err), Coordinate(dx + 100 + err, 0 + err),
                          Coordinate(dx + 100 + err, 10 + err), Coordinate::getNull() };
    WayPtr wi2a = createWay(map, Status::Unknown2, ci2a, ce, "i2");

    Coordinate ci2b[] = { Coordinate(dx + -50 + err, 10 + err),
                          Coordinate(dx + 100 + err, 10 + err),
                          Coordinate::getNull() };
    WayPtr wi2b = createWay(map, Status::Unknown2, ci2b, ce, "i2");

    RelationPtr ri2(new Relation(Status::Unknown1, map->createNextRelationId(), 3,
      "multilinestring"));
    ri2->getTags()["note"] = "ri2";
    ri2->addElement("", wi2a);
    ri2->addElement("", wi2b);
    map->addElement(ri2);

    return map;
  }

  void runBackwardsPartialTest()
  {
    OsmMapPtr map = createMap();

    Coordinate w1c[] = { Coordinate(50, 0), Coordinate(150, 0), Coordinate::getNull() };
    WayPtr w1 = createWay(map, Status::Unknown1, w1c, 5, "w1");

    Coordinate w2c[] = { Coordinate(100, 0), Coordinate(0, 0), Coordinate::getNull() };
    WayPtr w2 = createWay(map, Status::Unknown1, w2c, 5, "w2");

    shared_ptr<MaximalSublineStringMatcher> sublineMatcher(new MaximalSublineStringMatcher());
    sublineMatcher->setMinSplitSize(5.0);
    sublineMatcher->setMaxRelevantAngle(toRadians(60.0));

    WaySublineMatchString s = sublineMatcher->findMatch(map, w1, w2);
    HOOT_STR_EQUALS(
      "matches:\n"
      "reversed, subline 1: start: way: -1 index: 0 fraction: 0 end: way: -1 index: 0 fraction: 0.5\n"
      "subline 2: start: way: -2 index: 0 fraction: 0 end: way: -2 index: 0 fraction: 0.5",
      s);

    CPPUNIT_ASSERT_EQUAL((ConstWayPtr)w1, s.getMatches()[0].getSubline1().getStart().getWay());
  }

  void runBackwardsPartial2Test()
  {
    OsmMapPtr map = createMap();

    Coordinate w1c[] = { Coordinate(0, 0),
                         Coordinate(30, 0),
                         Coordinate(70, 0),
                         Coordinate(120, 0),
                         Coordinate(180, 0),
                         Coordinate(250, 0),
                         Coordinate(330, 0),
                         Coordinate::getNull() };
    WayPtr w1 = createWay(map, Status::Unknown1, w1c, 5, "w1");

    Coordinate w2c[] = { Coordinate(90, -5), Coordinate(85, -5), Coordinate(80, -5),
                         Coordinate::getNull() };
    WayPtr w2 = createWay(map, Status::Unknown1, w2c, 5, "w2");

    shared_ptr<MaximalSublineStringMatcher> sublineMatcher(new MaximalSublineStringMatcher());
    sublineMatcher->setMinSplitSize(5.0);
    sublineMatcher->setMaxRelevantAngle(toRadians(60.0));

    WaySublineMatchString s = sublineMatcher->findMatch(map, w1, w2);
    HOOT_STR_EQUALS(
      "matches:\n"
      "reversed, subline 1: start: way: -1 index: 2 fraction: 0.2 end: way: -1 index: 2 fraction: 0.4\n"
      "subline 2: start: way: -2 index: 0 fraction: 0 end: way: -2 index: 2 fraction: 0",
      s);

    CPPUNIT_ASSERT_EQUAL((ConstWayPtr)w1, s.getMatches()[0].getSubline1().getStart().getWay());
  }

  vector<ConstWayPtr> toWayVector(OsmMapPtr map, QString note)
  {
    vector<ConstWayPtr> result;
    vector<long> wids = FindWaysVisitor::findWaysByTag(map, "note", note);
    for (size_t i = 0; i < wids.size(); i++)
    {
      result.push_back(map->getWay(wids[i]));
    }
    return result;
  }

  ConstWayPtr toWay(OsmMapPtr map, QString note)
  {
    vector<long> wids = FindWaysVisitor::findWaysByTag(map, "note", note);
    for (size_t i = 0; i < wids.size(); i++)
    {
      return map->getWay(wids[i]);
    }
    throw InternalErrorException();
  }

  ConstRelationPtr toRelation(OsmMapPtr map, QString note)
  {
    vector<ConstRelationPtr> result;
    const RelationMap& relations = map->getRelationMap();
    for (RelationMap::const_iterator it = relations.begin(); it != relations.end(); ++it)
    {
      if (it->second->getTags().get("note") == note)
      {
        return it->second;
      }
    }
    throw InternalErrorException();
  }

  void evaluateMatchTest()
  {
    OsmMapPtr map = createMatchTestMap();

    MaximalSublineStringMatcher uut;
    uut.setMaxRelevantAngle(toRadians(ConfigOptions().getWayMatcherMaxAngle()));

    // test the simplest case
    {
      vector<bool> reversed1(1, false), reversed2(1, false);
      MaximalSublineStringMatcher::ScoredMatch sm =
          uut._evaluateMatch(map, 10, toWayVector(map, "a1"), toWayVector(map, "a2"),
                             reversed1, reversed2);

      HOOT_STR_EQUALS("score: 100\n"
        "matches:\n"
        "subline 1: start: way: -1 index: 0 fraction: 0 end: way: -1 index: 1 fraction: 0\n"
        "subline 2: start: way: -2 index: 0 fraction: 0 end: way: -2 index: 1 fraction: 0",
        sm.toString());
    }

    // test the backwards simple case
    {
      vector<bool> reversed1(1, false), reversed2(1, true);
      MaximalSublineStringMatcher::ScoredMatch sm =
          uut._evaluateMatch(map, 10, toWayVector(map, "a1"), toWayVector(map, "a2"),
                             reversed1, reversed2);

      HOOT_STR_EQUALS("score: 0\n"
        "matches:\n",
        sm.toString());
    }

    // test the Multiple Partial Matches case
    {
      vector<bool> reversed1(1, false), reversed2(1, false);

      MaximalSublineStringMatcher::ScoredMatch sm =
          uut._evaluateMatch(map, 10, toWayVector(map, "b1"), toWayVector(map, "b2"),
                             reversed1, reversed2);

      // @todo This test fails with the default subline matcher. The MaximalSublineMatcher
      // performs better, but has other flaws.
      // r2701 should resolve this.
//      HOOT_STR_EQUALS("score: 80\n"
//        "matches:\n"
//        "subline 1: start: way: -3 index: 0 fraction: 0 end: way: -3 index: 0 fraction: 0.4\n"
//        "subline 2: start: way: -4 index: 0 fraction: 0 end: way: -4 index: 1 fraction: 0\n"
//        "subline 1: start: way: -3 index: 0 fraction: 0.6 end: way: -3 index: 1 fraction: 0\n"
//        "subline 2: start: way: -4 index: 4 fraction: 0 end: way: -4 index: 5 fraction: 0",
//        sm.toString());
    }

    // test the Offset Partial Match case
    {
      vector<bool> reversed1(1, false), reversed2(1, false);
      MaximalSublineStringMatcher::ScoredMatch sm =
          uut._evaluateMatch(map, 10, toWayVector(map, "c1"), toWayVector(map, "c2"),
                             reversed1, reversed2);

      HOOT_STR_EQUALS("score: 50\n"
        "matches:\n"
        "subline 1: start: way: -5 index: 0 fraction: 0.5 end: way: -5 index: 1 fraction: 0\n"
        "subline 2: start: way: -6 index: 0 fraction: 0 end: way: -6 index: 0 fraction: 0.5",
        sm.toString());
    }

    // test the Centered Partial Match case
    {
      vector<bool> reversed1(1, false), reversed2(1, false);
      MaximalSublineStringMatcher::ScoredMatch sm =
          uut._evaluateMatch(map, 10, toWayVector(map, "d1"), toWayVector(map, "d2"),
                             reversed1, reversed2);

      HOOT_STR_EQUALS("score: 40\n"
        "matches:\n"
        "subline 1: start: way: -7 index: 0 fraction: 0.3 end: way: -7 index: 0 fraction: 0.7\n"
        "subline 2: start: way: -8 index: 0 fraction: 0 end: way: -8 index: 1 fraction: 0",
        sm.toString());
    }

    // test the Multiple Matches Case
    {
      vector<bool> reversed1(2, false), reversed2(2, false);
      MaximalSublineStringMatcher::ScoredMatch sm =
          uut._evaluateMatch(map, 10, toWayVector(map, "e1"), toWayVector(map, "e2"),
                             reversed1, reversed2);

      HOOT_STR_EQUALS("score: 80\n"
        "matches:\n"
        "subline 1: start: way: -10 index: 0 fraction: 0 end: way: -10 index: 0 fraction: 0.375\n"
        "subline 2: start: way: -12 index: 0 fraction: 0 end: way: -12 index: 1 fraction: 0\n"
        "subline 1: start: way: -9 index: 0 fraction: 0 end: way: -9 index: 1 fraction: 0\n"
        "subline 2: start: way: -11 index: 0 fraction: 0.5 end: way: -11 index: 1 fraction: 0",
        sm.toString());
    }

    // test the Offset Partial Match Multiple lines
    {
      vector<bool> reversed1(2, false), reversed2(2, false);
      MaximalSublineStringMatcher::ScoredMatch sm =
          uut._evaluateMatch(map, 10, toWayVector(map, "f1"), toWayVector(map, "f2"),
                             reversed1, reversed2);

      HOOT_STR_EQUALS("score: 50\n"
        "matches:\n"
        "subline 1: start: way: -13 index: 0 fraction: 0.7 end: way: -13 index: 1 fraction: 0\n"
        "subline 2: start: way: -15 index: 0 fraction: 0 end: way: -15 index: 0 fraction: 0.375\n"
        "subline 1: start: way: -13 index: 0 fraction: 0.5 end: way: -13 index: 0 fraction: 0.7\n"
        "subline 2: start: way: -14 index: 0 fraction: 0 end: way: -14 index: 1 fraction: 0",
        sm.toString());
    }

    // test the U-Shaped Multiple lines
    {
      vector<bool> reversed1(2, false), reversed2(2, false);
      MaximalSublineStringMatcher::ScoredMatch sm =
          uut._evaluateMatch(map, 15, toWayVector(map, "g1"), toWayVector(map, "g2"),
                             reversed1, reversed2);

      HOOT_STR_EQUALS(210,
        sm.matches[0].getSubline1().getLength() + sm.matches[1].getSubline1().getLength());
      HOOT_STR_EQUALS(210,
        sm.matches[0].getSubline2().getLength() + sm.matches[1].getSubline2().getLength());

      HOOT_STR_EQUALS("score: 210\n"
        "matches:\n"
        "subline 1: start: way: -16 index: 2 fraction: 0 end: way: -16 index: 3 fraction: 0\n"
        "subline 2: start: way: -18 index: 0 fraction: 0 end: way: -18 index: 1 fraction: 0\n"
        "subline 1: start: way: -16 index: 0 fraction: 0 end: way: -16 index: 2 fraction: 0\n"
        "subline 2: start: way: -17 index: 0 fraction: 0 end: way: -17 index: 2 fraction: 0",
        sm.toString());
    }

  }

  /**
   * This test is making sure that both matches end at the end of w1 & w2. Also, the beginning of
   * the subline matches shouldn't be at the start of w1 & w2.
   */
  void runEndSnapTest()
  {
    OsmMapPtr map = createMap();

    Coordinate w1c[] = { Coordinate(0, 20),
                         Coordinate(20, 5),
                         Coordinate(50, 5),
                         Coordinate(100, 6),
                         Coordinate::getNull() };
    WayPtr w1 = createWay(map, Status::Unknown1, w1c, 5, "w1");

    Coordinate w2c[] = { Coordinate(0, 0),
                         Coordinate(30, 0),
                         Coordinate(80, 0),
                         Coordinate(101.23, 0),
                         Coordinate::getNull() };
    WayPtr w2 = createWay(map, Status::Unknown1, w2c, 5, "w2");

    shared_ptr<MaximalSublineStringMatcher> sublineMatcher(new MaximalSublineStringMatcher());
    sublineMatcher->setMinSplitSize(5.0);
    sublineMatcher->setMaxRelevantAngle(toRadians(60.0));

    WaySublineMatchString s = sublineMatcher->findMatch(map, w1, w2);
    HOOT_STR_EQUALS(
      "matches:\n"
      "subline 1: start: way: -1 index: 1 fraction: 0 end: way: -1 index: 3 fraction: 0\n"
      "subline 2: start: way: -2 index: 0 fraction: 0.666666666666667 end: way: -2 index: 3 fraction: 0",
      s);

    CPPUNIT_ASSERT_EQUAL((ConstWayPtr)w1, s.getMatches()[0].getSubline1().getStart().getWay());
  }

  void runFindMatchTest()
  {
    OsmMapPtr map = createMatchTestMap();

    MaximalSublineStringMatcher uut;
    uut.setMaxRelevantAngle(toRadians(ConfigOptions().getWayMatcherMaxAngle()));

    // test the simplest case
    {
      RelationPtr r(new Relation(Status::Unknown1, map->createNextRelationId(), 5,
        "multilinestring"));
      r->addElement("", toWay(map, "a1"));
      map->addElement(r);

      WaySublineMatchString match = uut.findMatch(map, r, toWay(map, "a2"));

      HOOT_STR_EQUALS("matches:\n"
        "subline 1: start: way: -1 index: 0 fraction: 0 end: way: -1 index: 1 fraction: 0\n"
        "subline 2: start: way: -2 index: 0 fraction: 0 end: way: -2 index: 1 fraction: 0",
        match.toString());
    }

    // test the U-Shaped Multiple lines
    {
      WaySublineMatchString match = uut.findMatch(map, toRelation(map, "rg2"), toWay(map, "g1"));

      HOOT_STR_EQUALS(
        "matches:\n"
        "subline 1: start: way: -17 index: 0 fraction: 0 end: way: -17 index: 2 fraction: 0\n"
        "subline 2: start: way: -16 index: 0 fraction: 0 end: way: -16 index: 2 fraction: 0\n"
        "subline 1: start: way: -18 index: 0 fraction: 0 end: way: -18 index: 1 fraction: 0\n"
        "subline 2: start: way: -16 index: 2 fraction: 0 end: way: -16 index: 3 fraction: 0",
        match.toString());
    }

    // test the U-Shaped Multiple lines, one reversed
    {
      WaySublineMatchString match = uut.findMatch(map, toRelation(map, "rh2"), toWay(map, "h1"));

      HOOT_STR_EQUALS(
        "matches:\n"
        "subline 1: start: way: -20 index: 0 fraction: 0 end: way: -20 index: 2 fraction: 0\n"
        "subline 2: start: way: -19 index: 0 fraction: 0 end: way: -19 index: 2 fraction: 0\n"
        "reversed, subline 1: start: way: -21 index: 0 fraction: 0 end: way: -21 index: 1 fraction: 0\n"
        "subline 2: start: way: -19 index: 2 fraction: 0 end: way: -19 index: 3 fraction: 0",
        match.toString());
    }

    // test the U-Shaped Multiple lines, one reversed and partial
    {
      WaySublineMatchString match = uut.findMatch(map, toRelation(map, "ri2"), toWay(map, "i1"));

      HOOT_STR_EQUALS(
       "matches:\n"
       "subline 1: start: way: -23 index: 0 fraction: 0 end: way: -23 index: 2 fraction: 0\n"
       "subline 2: start: way: -22 index: 0 fraction: 0 end: way: -22 index: 2 fraction: 0\n"
       "reversed, subline 1: start: way: -24 index: 0 fraction: 0.333333333333333 end: way: -24 index: 1 fraction: 0\n"
       "subline 2: start: way: -22 index: 2 fraction: 0 end: way: -22 index: 3 fraction: 0",
        match.toString());
    }
  }

};

CPPUNIT_TEST_SUITE_REGISTRATION(MaximalSublineStringMatcherTest);

}

