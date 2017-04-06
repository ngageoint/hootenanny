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
 * @copyright Copyright (C) 2013, 2014, 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */
// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// geos
#include <geos/geom/Coordinate.h>
#include <geos/geom/LineString.h>

// Hoot
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/algorithms/MaximalNearestSubline.h>
#include <hoot/core/algorithms/MaximalSubline.h>
#include <hoot/core/algorithms/WaySplitter.h>
#include <hoot/core/util/ElementConverter.h>
#include <hoot/core/util/GeometryUtils.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/visitors/FindWaysVisitor.h>
#include <hoot/core/algorithms/linearreference/WaySublineMatch.h>
using namespace hoot;

// Qt
#include <QDir>
#include <QString>

// Standard
#include <string>

// TGS
#include <tgs/System/Time.h>

#include "../TestUtils.h"

namespace hoot
{

class MaximalSublineTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(MaximalSublineTest);
  CPPUNIT_TEST(runCircleTest);
  CPPUNIT_TEST(runDiagonalOffsetTest);
  CPPUNIT_TEST(runJoinTest);
  CPPUNIT_TEST(runRealWorld3Test);
  //CPPUNIT_TEST(runRealWorld4Test);
  CPPUNIT_TEST(runSmallLengthDiffTest);
  CPPUNIT_TEST(runToyTest);
  CPPUNIT_TEST(runUTest);
  CPPUNIT_TEST(runUTopTest);
  CPPUNIT_TEST(runVTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void setUp()
  {
    TestUtils::resetEnvironment();
  }

  void addEndNode(boost::shared_ptr<OsmMap> map, Coordinate c, QString note)
  {
    boost::shared_ptr<Node> n(new Node(Status::Unknown1, map->createNextNodeId(), c, 10));
    n->getTags()["note"] = note;
    map->addNode(n);
  }

  void addEndNodes(boost::shared_ptr<OsmMap> map, vector<WaySublineMatch>& m)
  {
    for (size_t i = 0; i < m.size(); i++)
    {
      addEndNode(map, m[i].getSubline1().getStart().getCoordinate(), "start");
      addEndNode(map, m[i].getSubline1().getEnd().getCoordinate(), "end");
      addEndNode(map, m[i].getSubline2().getStart().getCoordinate(), "start");
      addEndNode(map, m[i].getSubline2().getEnd().getCoordinate(), "end");
    }
  }

  OsmMapPtr createMap()
  {
    boost::shared_ptr<OsmMap> map(new OsmMap());
    OsmMap::resetCounters();
    auto_ptr<OGREnvelope> env(GeometryUtils::toOGREnvelope(Envelope(0, 1, 0, 1)));
    MapProjector::projectToPlanar(map, *env);

    return map;
  }

  /**
   * This is a nasty little situation that showed up in the DC data when doing some real world
   * testing.
   *
   * This creates a really poor match, but tests for an edge condition in MaximalSubline
   */
  void runCircleTest()
  {
    OsmXmlReader reader;

    boost::shared_ptr<OsmMap> map(new OsmMap());
    OsmMap::resetCounters();
    reader.read("test-files/algorithms/MaximalSublineCircleTestIn.osm", map);

    double score;

    MapProjector::projectToPlanar(map);

    {
      WayPtr w1 = map->getWay(FindWaysVisitor::findWaysByTag(map, "note", "1")[0]);
      WayPtr w2 = map->getWay(FindWaysVisitor::findWaysByTag(map, "note", "2")[0]);

      MaximalSubline uut(new MaximalSubline::ThresholdMatchCriteria(10.0, M_PI / 2.0), 10.0);

      vector<WaySublineMatch> m = uut.findAllMatches(map, w1, w2, score);
      // @todo this may need a little more thinking. Rather than finding the simplest match between
      // the beginning of w1 and all of w2, it finds two matches, one at the end of w1 to the
      // beginning of w2, then another a little into w2 and a little into w1. While this is
      // technically a correct answer, it isn't the best answer.
      HOOT_STR_EQUALS(2, m.size());
    }

    {
      WayPtr w1 = map->getWay(FindWaysVisitor::findWaysByTag(map, "note", "1")[0]);
      WayPtr w2 = map->getWay(FindWaysVisitor::findWaysByTag(map, "note", "2")[0]);
      w1->reverseOrder();

      MaximalSubline uut(new MaximalSubline::ThresholdMatchCriteria(40.0, M_PI / 2.0), 40.0);

      vector<WaySublineMatch> m = uut.findAllMatches(map, w1, w2, score);
      HOOT_STR_EQUALS(0, m.size());
    }
  }

  void runJoinTest()
  {
    OsmXmlReader reader;

    boost::shared_ptr<OsmMap> map(new OsmMap());
    OsmMap::resetCounters();
    reader.setDefaultStatus(Status::Unknown1);
    reader.read("test-files/algorithms/MaximalSublineTestIn.osm", map);

    MapProjector::projectToPlanar(map);

    std::vector<long> wids = FindWaysVisitor::findWaysByTag(map, "note", "trail");

    MaximalSubline uut(new MaximalSubline::ThresholdMatchCriteria(40.0, M_PI / 1.0), 40.0);

    boost::shared_ptr<Way> w1 = map->getWay(wids[0]);
    boost::shared_ptr<Way> w2 = map->getWay(wids[1]);

    double score;
    vector<WaySublineMatch> m = uut.findAllMatches(map, w1, w2, score);
    addEndNodes(map, m);

    QDir().mkpath("test-output/algorithms/");
    MapProjector::projectToWgs84(map);
    OsmXmlWriter().write(map, "test-output/algorithms/MaximalSublineJoinTestOut.osm");
  }

  void runDiagonalOffsetTest()
  {
    OsmMapPtr map = createMap();

    /*
     * Create ways like this:
     * w2     ----------
     * w1 ---------
     */
    Coordinate c1[] = { Coordinate(0.0, 0.0), Coordinate(20.0, 0.0), Coordinate::getNull() };
    WayPtr w1 = TestUtils::createWay(map, Status::Unknown1, c1, 10.0, "w1");

    Coordinate c2[] = { Coordinate(10.0, 10.0), Coordinate(30.0, 10.0), Coordinate::getNull() };
    WayPtr w2 = TestUtils::createWay(map, Status::Unknown2, c2, 10.0, "w2");

    MaximalSubline uut(new MaximalSubline::ThresholdMatchCriteria(40.0, M_PI / 1.0), 40.0);

    double score;
    vector<WaySublineMatch> m = uut.findAllMatches(map, w1, w2, score);

    HOOT_STR_EQUALS(1, m.size());
    HOOT_STR_EQUALS(
      "subline 1: start: way: -1 index: 0 fraction: 0 end: way: -1 index: 1 fraction: 0\n"
      "subline 2: start: way: -2 index: 0 fraction: 0 end: way: -2 index: 1 fraction: 0",
      m[0].toString());
  }

  void runVTest()
  {
    OsmMapPtr map = createMap();

    /*
     * Create ways like this:
     * w1 \    / w2
     *     \  /
     *      \/
     */
    Coordinate c1[] = { Coordinate(0.0, 0.0), Coordinate(-15.0, 50.0), Coordinate::getNull() };
    WayPtr w1 = TestUtils::createWay(map, Status::Unknown1, c1, 10.0, "w1");

    Coordinate c2[] = { Coordinate(0.0, 0.0), Coordinate(15.0, 50.0), Coordinate::getNull() };
    WayPtr w2 = TestUtils::createWay(map, Status::Unknown2, c2, 10.0, "w2");

    MaximalSubline uut(new MaximalSubline::ThresholdMatchCriteria(40.0, M_PI / 1.0), 40.0);

    double score;
    vector<WaySublineMatch> m = uut.findAllMatches(map, w1, w2, score);

    HOOT_STR_EQUALS(1, m.size());
    HOOT_STR_EQUALS(
      "subline 1: start: way: -1 index: 0 fraction: 0 end: way: -1 index: 1 fraction: 0\n"
      "subline 2: start: way: -2 index: 0 fraction: 0 end: way: -2 index: 1 fraction: 0",
      m[0].toString());
  }

  void runRealWorld3Test()
  {
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(map, "test-files/conflate/highway/HighwayMatchRealWorld3Test.osm",
      false, Status::Unknown1);
    MapProjector::projectToPlanar(map);

    WayPtr w52 = dynamic_pointer_cast<Way>(TestUtils::getElementWithNote(map, "-52"));
    WayPtr w812 = dynamic_pointer_cast<Way>(TestUtils::getElementWithNote(map, "-812"));

    MaximalSubline uut(new MaximalSubline::ThresholdMatchCriteria(25.08, 1.0471975511965976), 5);

    double bestScore;
    vector<WaySublineMatch> m = uut.findAllMatches(map, w52, w812, bestScore);
    HOOT_STR_EQUALS(
      "[1]{subline 1: start: way: -1 index: 2 fraction: 0.440972029007195 end: way: -1 index: 2 fraction: 0.553077810038132\n"
      "subline 2: start: way: -2 index: 2 fraction: 0 end: way: -2 index: 3 fraction: 0}",
      m);
  }


  /**
   * See #5408
   */
  void runRealWorld4Test()
  {
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(map, "test-files/conflate/waterway/RealWorld4Test.osm",
      false, Status::Unknown1);
    MapProjector::projectToPlanar(map);

    WayPtr w1 = dynamic_pointer_cast<Way>(TestUtils::getElementWithNote(map, "1"));
    WayPtr w2 = dynamic_pointer_cast<Way>(TestUtils::getElementWithNote(map, "2"));

    //MaximalSubline uut(new MaximalSubline::ThresholdMatchCriteria(36, 1.57079632679), 5);
    MaximalSubline uut(new MaximalSubline::ThresholdMatchCriteria(46, 1.5708), 5);

    double bestScore;
    vector<WaySublineMatch> m = uut.findAllMatches(map, w1, w2, bestScore);
    LOG_VAR(m);
    /// @todo not valid compare
//    HOOT_STR_EQUALS(
//      "[1]{subline 1: start: way: -18 index: 2 fraction: 0.440972029332128 end: way: -18 index: 2 fraction: 0.553077811243426\n"
//      "subline 2: start: way: -17 index: 2 fraction: 0 end: way: -17 index: 3 fraction: 0}",
//      m);
  }

  void runSmallLengthDiffTest()
  {
    {
      OsmMapPtr map = createMap();

      /*
       * Create ways like this:
       * w2  ---------
       * w1 ---------
       */
      Coordinate c1[] = { Coordinate(0.0, 0.0), Coordinate(100.0, 0.0), Coordinate::getNull() };
      WayPtr w1 = TestUtils::createWay(map, Status::Unknown1, c1, 10.0, "w1");

      Coordinate c2[] = { Coordinate(2.1, 0.0), Coordinate(100.0, 0.0), Coordinate::getNull() };
      WayPtr w2 = TestUtils::createWay(map, Status::Unknown2, c2, 10.0, "w2");

      MaximalSubline uut(new MaximalSubline::ThresholdMatchCriteria(0.5, M_PI / 1.0), 0.5);

      double score;
      vector<WaySublineMatch> m = uut.findAllMatches(map, w1, w2, score);

      HOOT_STR_EQUALS(1, m.size());
      HOOT_STR_EQUALS(
        "subline 1: start: way: -1 index: 0 fraction: 0.021 end: way: -1 index: 1 fraction: 0\n"
        "subline 2: start: way: -2 index: 0 fraction: 0 end: way: -2 index: 1 fraction: 0",
        m[0].toString());
    }

    {
      OsmMapPtr map = createMap();

      /*
       * Create ways like this:
       * w2 --------
       * w1 ---------
       */
      Coordinate c1[] = { Coordinate(0.0, 0.0), Coordinate(100.0, 0.0), Coordinate::getNull() };
      WayPtr w1 = TestUtils::createWay(map, Status::Unknown1, c1, 10.0, "w1");

      Coordinate c2[] = { Coordinate(0.0, 0.0), Coordinate(97.9, 0.0), Coordinate::getNull() };
      WayPtr w2 = TestUtils::createWay(map, Status::Unknown2, c2, 10.0, "w2");

      MaximalSubline uut(new MaximalSubline::ThresholdMatchCriteria(0.5, M_PI / 1.0), 0.5);

      double score;
      vector<WaySublineMatch> m = uut.findAllMatches(map, w1, w2, score);

      HOOT_STR_EQUALS(1, m.size());
      HOOT_STR_EQUALS(
        "subline 1: start: way: -1 index: 0 fraction: 0 end: way: -1 index: 0 fraction: 0.979\n"
        "subline 2: start: way: -2 index: 0 fraction: 0 end: way: -2 index: 1 fraction: 0",
        m[0].toString());
    }

    {
      OsmMapPtr map = createMap();

      /*
       * Create ways like this:
       * w2   -------
       * w1 ---------
       */
      Coordinate c1[] = { Coordinate(60.0, 5.0), Coordinate(100.0, 5.0), Coordinate::getNull() };
      WayPtr w1 = TestUtils::createWay(map, Status::Unknown1, c1, 10.0, "w1");

      Coordinate c2[] = { Coordinate(0.0, 0.0),
                          Coordinate(50, 0.0),
                          Coordinate(100, 0.0),
                          Coordinate::getNull() };
      WayPtr w2 = TestUtils::createWay(map, Status::Unknown2, c2, 10.0, "w2");

      MaximalSubline uut(new MaximalSubline::ThresholdMatchCriteria(10, M_PI / 1.0), 10);

      double score;
      vector<WaySublineMatch> m = uut.findAllMatches(map, w2, w1, score);

      HOOT_STR_EQUALS(1, m.size());
      HOOT_STR_EQUALS(
        "subline 1: start: way: -2 index: 1 fraction: 0.2 end: way: -2 index: 2 fraction: 0\n"
        "subline 2: start: way: -1 index: 0 fraction: 0 end: way: -1 index: 1 fraction: 0",
        m[0].toString());
    }

    {
      OsmMapPtr map = createMap();

      /*
       * Create ways like this:
       * w1 ------------
       * w2 -------
       */
      Coordinate c1[] = { Coordinate(-1.0, 5.0),
                          Coordinate(20.0, 5.0),
                          Coordinate(60.0, 5.0),
                          Coordinate(120.0, 5.0),
                          Coordinate(180.0, 5.0),
                          Coordinate::getNull() };
      WayPtr w1 = TestUtils::createWay(map, Status::Unknown1, c1, 10.0, "w1");

      Coordinate c2[] = { Coordinate(0.0, 0.0),
                          Coordinate(50, 0.0),
                          Coordinate(100, 0.0),
                          Coordinate::getNull() };
      WayPtr w2 = TestUtils::createWay(map, Status::Unknown2, c2, 10.0, "w2");

      MaximalSubline uut(new MaximalSubline::ThresholdMatchCriteria(10, M_PI / 1.0), 10);

      double score;
      vector<WaySublineMatch> m = uut.findAllMatches(map, w1, w2, score);

      HOOT_STR_EQUALS(1, m.size());
      HOOT_STR_EQUALS(
        "subline 1: start: way: -1 index: 0 fraction: 0 end: way: -1 index: 2 fraction: 0.666666666666667\n"
        "subline 2: start: way: -2 index: 0 fraction: 0 end: way: -2 index: 2 fraction: 0",
        m[0].toString());
    }
  }

  void runToyTest()
  {
    {
      boost::shared_ptr<OsmMap> map(new OsmMap());
      OsmXmlReader reader;
      OsmMap::resetCounters();
      reader.setDefaultStatus(Status::Unknown1);
      reader.read("test-files/ToyTestA.osm", map);
      reader.setDefaultStatus(Status::Unknown2);
      reader.read("test-files/ToyTestB.osm", map);
      MapProjector::projectToPlanar(map);

      {
        WayPtr w1 = map->getWay(FindWaysVisitor::findWaysByTag(map, "note", "0")[0]);
        WayPtr w2 = map->getWay(-6);

        MaximalSubline uut(new MaximalSubline::ThresholdMatchCriteria(10, M_PI / 2.0), 10);

        double score;
        vector<WaySublineMatch> m = uut.findAllMatches(map, w1, w2, score);

        HOOT_STR_EQUALS(1, m.size());
        HOOT_STR_EQUALS(
          "subline 1: start: way: -3 index: 0 fraction: 0 end: way: -3 index: 10 fraction: 0.834339910124657\n"
          "subline 2: start: way: -6 index: 0 fraction: 0 end: way: -6 index: 17 fraction: 0",
          m[0].toString());
      }

      // this one is backwards and shouldn't result in a match.
      {
        WayPtr w1 = map->getWay(FindWaysVisitor::findWaysByTag(map, "note", "2")[0]);
        WayPtr w2 = map->getWay(-7);

        MaximalSubline uut(new MaximalSubline::ThresholdMatchCriteria(10, M_PI / 2), 10);

        double score;
        vector<WaySublineMatch> m = uut.findAllMatches(map, w1, w2, score);

        HOOT_STR_EQUALS(0, m.size());
      }
    }
  }

  void runUTest()
  {
    {
      OsmMapPtr map = createMap();

      // U-Match - two parts
      // 2    -----------+
      //                 |
      //      -----------+
      // 1 -----------+
      //              |
      //   -----------+
      double err = 0;
      Coordinate c1[] = { Coordinate(0 + err, 0 + err), Coordinate(100 + err, 0 + err),
                          Coordinate(100 + err, 10 + err), Coordinate(0 + err, 10 + err),
                          Coordinate::getNull() };
      WayPtr w1 = TestUtils::createWay(map, Status::Unknown1, c1, 20.0, "w1");

      err = 20;
      Coordinate c2[] = { Coordinate(0 + err, 0 + err), Coordinate(100 + err, 0 + err),
                          Coordinate(100 + err, 10 + err), Coordinate(0 + err, 10 + err),
                          Coordinate::getNull() };
      WayPtr w2 = TestUtils::createWay(map, Status::Unknown2, c2, 20.0, "w2");

      MaximalSubline uut(new MaximalSubline::ThresholdMatchCriteria(40, M_PI / 1.0), 40);

      double score;
      vector<WaySublineMatch> m = uut.findAllMatches(map, w1, w2, score);

      HOOT_STR_EQUALS(1, m.size());
      HOOT_STR_EQUALS(
        "subline 1: start: way: -1 index: 0 fraction: 0 end: way: -1 index: 3 fraction: 0\n"
        "subline 2: start: way: -2 index: 0 fraction: 0 end: way: -2 index: 3 fraction: 0",
        m[0].toString());
    }
  }

  void runUTopTest()
  {
    {
      OsmMapPtr map = createMap();

      // U-Match - two parts
      // 2    -----------+
      //
      // 1 -----------+
      //              |
      //   -----------+
      double err = 0;
      Coordinate c1[] = { Coordinate(0 + err, 0 + err), Coordinate(100 + err, 0 + err),
                          Coordinate(100 + err, 10 + err), Coordinate(0 + err, 10 + err),
                          Coordinate::getNull() };
      WayPtr w1 = TestUtils::createWay(map, Status::Unknown1, c1, 20.0, "w1");

      err = 0;
      Coordinate c2[] = { Coordinate(100 + err, 10 + err), Coordinate(0 + err, 10 + err),
                          Coordinate::getNull() };
      WayPtr w2 = TestUtils::createWay(map, Status::Unknown2, c2, 20.0, "w2");

      MaximalSubline uut(new MaximalSubline::ThresholdMatchCriteria(40, M_PI / 1.0), 40);

      double score;
      vector<WaySublineMatch> m = uut.findAllMatches(map, w1, w2, score);

      HOOT_STR_EQUALS(1, m.size());
      HOOT_STR_EQUALS(
        "subline 1: start: way: -1 index: 2 fraction: 0 end: way: -1 index: 3 fraction: 0\n"
        "subline 2: start: way: -2 index: 0 fraction: 0 end: way: -2 index: 1 fraction: 0",
        m[0].toString());
    }
  }

  double splitLines(const OsmMapPtr& map, long wid1, long wid2)
  {
    MaximalSubline uut(new MaximalSubline::ThresholdMatchCriteria(15.0, M_PI / 4.0), 15);

    boost::shared_ptr<Way> w1 = map->getWay(wid1);
    boost::shared_ptr<Way> w2 = map->getWay(wid2);

    vector<WayLocation> wl1, wl2;
    bool found = uut.findMaximalSubline(map, w1, w2, wl1, wl2);
    double result = 0.0;
    if (found)
    {
      boost::shared_ptr<Way> ws1 = WaySubline(wl1[0], wl1[1]).toWay(map);
      boost::shared_ptr<Way> ws2 = WaySubline(wl2[0], wl2[1]).toWay(map);

      map->addWay(ws1);
      map->addWay(ws2);

      ElementConverter ec(map);
      result = (ec.calculateLength(ws1) + ec.calculateLength(ws2)) / 2.0;
    }
    else
    {
      LOG_INFO("Couldn't find a subline.");
    }

    return result;
  }

};

//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(MaximalSublineTest, "current");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(MaximalSublineTest, "quick");

}
