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
 * @copyright Copyright (C) 2015, 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/filters/TagCriterion.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/util/GeometryUtils.h>
#include <hoot/rnd/conflate/network/DebugNetworkMapCreator.h>
#include <hoot/rnd/conflate/network/EdgeMatchSetFinder.h>
#include <hoot/rnd/conflate/network/OsmNetworkExtractor.h>

namespace hoot
{

class NetworkDetailsTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(NetworkDetailsTest);
  CPPUNIT_TEST(calculateDistanceTest);
  CPPUNIT_TEST(calculateHeadingTest);
  CPPUNIT_TEST(calculateNearestLocationTest);
  CPPUNIT_TEST(calculateStringDistanceTest);
  CPPUNIT_TEST(extendEdgeMatchTest);
  CPPUNIT_TEST(extendEdgeStringTest);
  CPPUNIT_TEST_SUITE_END();

public:
  NetworkEdgePtr _e1ab, _e1bc, _e1cd, _e1df;
  NetworkEdgePtr _e2ab, _e2bc;

  NetworkDetailsPtr createSampleDetails()
  {
    TestUtils::resetEnvironment();
    OsmMapPtr map(new OsmMap());
    OsmMap::resetCounters();
    auto_ptr<OGREnvelope> env(GeometryUtils::toOGREnvelope(Envelope(0, 1, 0, 1)));
    MapProjector::projectToPlanar(map, *env);


    NodePtr n1a(new Node(Status::Invalid, -1, 0, 0, 15.0));
    NetworkVertexPtr v1(new NetworkVertex(n1a));
    NodePtr n1b(new Node(Status::Invalid, -2, 0, 100, 15.0));
    NetworkVertexPtr v2(new NetworkVertex(n1b));
    NodePtr n1c(new Node(Status::Invalid, -3, 0, 300, 15.0));
    NetworkVertexPtr v3(new NetworkVertex(n1c));
    NodePtr n1d(new Node(Status::Invalid, -4, 100, 300, 15.0));
    NetworkVertexPtr v4(new NetworkVertex(n1d));
    NodePtr n1e(new Node(Status::Invalid, -5, 150, 350, 15.0));
    NetworkVertexPtr v5(new NetworkVertex(n1e));
    NodePtr n1f(new Node(Status::Invalid, -6, 200, 300, 15.0));
    NetworkVertexPtr v6(new NetworkVertex(n1f));

    WayPtr w1(new Way(Status::Invalid, -1, 15.0));
    w1->addNode(-1);
    w1->addNode(-2);
    WayPtr w2(new Way(Status::Invalid, -2, 15.0));
    w2->addNode(-2);
    w2->addNode(-3);
    WayPtr w3(new Way(Status::Invalid, -3, 15.0));
    w3->addNode(-3);
    w3->addNode(-4);
    WayPtr w4(new Way(Status::Invalid, -4, 15.0));
    w4->addNode(-4);
    w4->addNode(-5);
    w4->addNode(-6);

    map->addElement(n1a);
    map->addElement(n1b);
    map->addElement(n1c);
    map->addElement(n1d);
    map->addElement(n1e);
    map->addElement(n1f);
    map->addElement(w1);
    map->addElement(w2);
    map->addElement(w3);
    map->addElement(w4);

    _e1ab.reset(new NetworkEdge(v1, v2, false));
    _e1bc.reset(new NetworkEdge(v2, v3, false));
    _e1cd.reset(new NetworkEdge(v3, v4, false));
    _e1df.reset(new NetworkEdge(v4, v6, false));

    _e1ab->addMember(w1);
    _e1bc->addMember(w2);
    _e1cd->addMember(w3);
    _e1df->addMember(w4);

    OsmNetworkPtr network1(new OsmNetwork());
    network1->addVertex(v1);
    network1->addVertex(v2);
    network1->addVertex(v3);
    network1->addVertex(v4);
    network1->addVertex(v5);
    network1->addVertex(v6);

    network1->addEdge(_e1ab);
    network1->addEdge(_e1bc);
    network1->addEdge(_e1cd);
    network1->addEdge(_e1df);

    NodePtr n2a = TestUtils::createNode(map, Status::Unknown2, 2, -50);
    NodePtr n2b = TestUtils::createNode(map, Status::Unknown2, 2, 100);
    NodePtr n2c = TestUtils::createNode(map, Status::Unknown2, 2, 400);
    WayPtr w2ab = TestUtils::createWay(map, QList<NodePtr>() << n2a << n2b);
    WayPtr w2bc = TestUtils::createWay(map, QList<NodePtr>() << n2b << n2c);

    NetworkVertexPtr v2a(new NetworkVertex(n2a));
    NetworkVertexPtr v2b(new NetworkVertex(n2b));
    NetworkVertexPtr v2c(new NetworkVertex(n2c));

    _e2ab.reset(new NetworkEdge(v2a, v2b, false, w2ab));
    _e2bc.reset(new NetworkEdge(v2b, v2c, false, w2bc));

    OsmNetworkPtr network2(new OsmNetwork());
    network2->addVertex(v2a);
    network2->addVertex(v2b);
    network2->addVertex(v2c);

    network2->addEdge(_e2ab);
    network2->addEdge(_e2bc);

    NetworkDetailsPtr details(new NetworkDetails(map, network1, network2));

    return details;
  }

  void calculateDistanceTest()
  {
    NetworkDetailsPtr details = createSampleDetails();

    HOOT_STR_EQUALS(0,
      details->calculateDistance(ConstEdgeLocationPtr(new EdgeLocation(_e1ab, 0.0))));
    HOOT_STR_EQUALS(50,
      details->calculateDistance(ConstEdgeLocationPtr(new EdgeLocation(_e1ab, 0.5))));
    HOOT_STR_EQUALS(180,
      details->calculateDistance(ConstEdgeLocationPtr(new EdgeLocation(_e1bc, 0.9))));
    HOOT_STR_EQUALS(100,
      details->calculateDistance(ConstEdgeLocationPtr(new EdgeLocation(_e1cd, 1.0))));
  }

  void calculateHeadingTest()
  {
    NetworkDetailsPtr details = createSampleDetails();

    // heading is relative to north (north is zero degrees, rather than the typical x axis is zero)
    HOOT_STR_EQUALS(toRadians(45),
      details->calculateHeading(ConstEdgeLocationPtr(new EdgeLocation(_e1df, 0.0))));
    HOOT_STR_EQUALS(toRadians(45),
      details->calculateHeading(ConstEdgeLocationPtr(new EdgeLocation(_e1df, 0.1))));
    HOOT_STR_EQUALS(toRadians(90),
      details->calculateHeading(ConstEdgeLocationPtr(new EdgeLocation(_e1df, 0.5))));
    HOOT_STR_EQUALS(toRadians(135),
      details->calculateHeading(ConstEdgeLocationPtr(new EdgeLocation(_e1df, 0.75))));
    HOOT_STR_EQUALS(toRadians(135),
      details->calculateHeading(ConstEdgeLocationPtr(new EdgeLocation(_e1df, 1.0))));
  }

  void calculateNearestLocationTest()
  {
    NetworkDetailsPtr details = createSampleDetails();

    EdgeStringPtr str(new EdgeString());
    str->appendEdge(EdgeSublinePtr(new EdgeSubline(
      ConstEdgeLocationPtr(new EdgeLocation(_e1ab, 0.4)),
      ConstEdgeLocationPtr(new EdgeLocation(_e1ab, 1.0)))));
    str->appendEdge(_e1bc);
    str->appendEdge(EdgeSublinePtr(new EdgeSubline(
      ConstEdgeLocationPtr(new EdgeLocation(_e1cd, 0.0)),
      ConstEdgeLocationPtr(new EdgeLocation(_e1cd, 0.6)))));

    ConstEdgeLocationPtr elStr, elSub;

    details->calculateNearestLocation(str,
      ConstEdgeSublinePtr(new EdgeSubline(
        ConstEdgeLocationPtr(new EdgeLocation(_e1ab, 0.1)),
        ConstEdgeLocationPtr(new EdgeLocation(_e1ab, 0.2)))),
      elStr, elSub);
    HOOT_STR_EQUALS("{ _e: (0) Node:-1 -- Way:-1 -- (1) Node:-2, _portion: 0.4 }", elStr);
    HOOT_STR_EQUALS("{ _e: (0) Node:-1 -- Way:-1 -- (1) Node:-2, _portion: 0.2 }", elSub);

    details->calculateNearestLocation(str,
      ConstEdgeSublinePtr(new EdgeSubline(
        ConstEdgeLocationPtr(new EdgeLocation(_e1ab, 0.1)),
        ConstEdgeLocationPtr(new EdgeLocation(_e1ab, 0.6)))),
      elStr, elSub);
    // There are a range of valid values.
    CPPUNIT_ASSERT(elStr == elSub);
    CPPUNIT_ASSERT(elStr->getEdge() == _e1ab);
    CPPUNIT_ASSERT(elStr->getPortion() >= 0.4 && elStr->getPortion() <= 0.6);

    details->calculateNearestLocation(str,
      ConstEdgeSublinePtr(new EdgeSubline(
        ConstEdgeLocationPtr(new EdgeLocation(_e1ab, 0.7)),
        ConstEdgeLocationPtr(new EdgeLocation(_e1ab, 0.3)))),
      elStr, elSub);
    // There are a range of valid values.
    CPPUNIT_ASSERT(elStr == elSub);
    CPPUNIT_ASSERT(elStr->getEdge() == _e1ab);
    CPPUNIT_ASSERT(elStr->getPortion() >= 0.4 && elStr->getPortion() <= 0.7);

    details->calculateNearestLocation(str,
      ConstEdgeSublinePtr(new EdgeSubline(
        ConstEdgeLocationPtr(new EdgeLocation(_e1cd, 0.7)),
        ConstEdgeLocationPtr(new EdgeLocation(_e1cd, 0.3)))),
      elStr, elSub);
    // There are a range of valid values.
    CPPUNIT_ASSERT(elStr == elSub);
    CPPUNIT_ASSERT(elStr->getEdge() == _e1cd);
    CPPUNIT_ASSERT(elStr->getPortion() >= 0.3 && elStr->getPortion() <= 0.6);

    details->calculateNearestLocation(str,
      ConstEdgeSublinePtr(new EdgeSubline(
        ConstEdgeLocationPtr(new EdgeLocation(_e1bc, 0.0)),
        ConstEdgeLocationPtr(new EdgeLocation(_e1bc, 1.0)))),
      elStr, elSub);
    // There are a range of valid values.
    CPPUNIT_ASSERT(elStr == elSub);
    CPPUNIT_ASSERT(elStr->getEdge() == _e1bc);
    CPPUNIT_ASSERT(elStr->getPortion() >= 0.0 && elStr->getPortion() <= 1.0);

    details->calculateNearestLocation(str,
      ConstEdgeSublinePtr(new EdgeSubline(
        ConstEdgeLocationPtr(new EdgeLocation(_e1bc, 0.5)),
        ConstEdgeLocationPtr(new EdgeLocation(_e1bc, 0.51)))),
      elStr, elSub);
    // There are a range of valid values.
    CPPUNIT_ASSERT(elStr == elSub);
    CPPUNIT_ASSERT(elStr->getEdge() == _e1bc);
    CPPUNIT_ASSERT(elStr->getPortion() >= 0.5 && elStr->getPortion() <= 0.51);

    EdgeStringPtr str2(new EdgeString());
    str2->appendEdge(_e1bc);
    str2->appendEdge(_e1cd);

    details->calculateNearestLocation(str2,
      ConstEdgeSublinePtr(new EdgeSubline(
        ConstEdgeLocationPtr(new EdgeLocation(_e1df, 0.0)),
        ConstEdgeLocationPtr(new EdgeLocation(_e1df, 0.3)))),
      elStr, elSub);
    HOOT_STR_EQUALS("{ _e: (2) Node:-3 -- Way:-3 -- (3) Node:-4, _portion: 1 }", elStr);
    HOOT_STR_EQUALS("{ _e: (2) Node:-3 -- Way:-3 -- (3) Node:-4, _portion: 1 }", elSub);

    details->calculateNearestLocation(str2,
      ConstEdgeSublinePtr(new EdgeSubline(
        ConstEdgeLocationPtr(new EdgeLocation(_e1df, 0.6)),
        ConstEdgeLocationPtr(new EdgeLocation(_e1df, 0.0)))),
      elStr, elSub);
    HOOT_STR_EQUALS("{ _e: (2) Node:-3 -- Way:-3 -- (3) Node:-4, _portion: 1 }", elStr);
    HOOT_STR_EQUALS("{ _e: (2) Node:-3 -- Way:-3 -- (3) Node:-4, _portion: 1 }", elSub);

    details->calculateNearestLocation(str2,
      ConstEdgeSublinePtr(new EdgeSubline(
        ConstEdgeLocationPtr(new EdgeLocation(_e1ab, 0.6)),
        ConstEdgeLocationPtr(new EdgeLocation(_e1ab, 1.0)))),
      elStr, elSub);
    HOOT_STR_EQUALS("{ _e: (1) Node:-2 -- Way:-2 -- (2) Node:-3, _portion: 0 }", elStr);
    HOOT_STR_EQUALS("{ _e: (1) Node:-2 -- Way:-2 -- (2) Node:-3, _portion: 0 }", elSub);

    details->calculateNearestLocation(str2,
      ConstEdgeSublinePtr(new EdgeSubline(
        ConstEdgeLocationPtr(new EdgeLocation(_e1ab, 1.0)),
        ConstEdgeLocationPtr(new EdgeLocation(_e1ab, 0.0)))),
      elStr, elSub);
    HOOT_STR_EQUALS("{ _e: (1) Node:-2 -- Way:-2 -- (2) Node:-3, _portion: 0 }", elStr);
    HOOT_STR_EQUALS("{ _e: (1) Node:-2 -- Way:-2 -- (2) Node:-3, _portion: 0 }", elSub);

  }

  void calculateStringDistanceTest()
  {
    NetworkDetailsPtr details = createSampleDetails();

    EdgeStringPtr str(new EdgeString());
    str->appendEdge(EdgeSublinePtr(new EdgeSubline(
      ConstEdgeLocationPtr(new EdgeLocation(_e1ab, 0.4)),
      ConstEdgeLocationPtr(new EdgeLocation(_e1ab, 1.0)))));
    str->appendEdge(_e1bc);
    str->appendEdge(EdgeSublinePtr(new EdgeSubline(
      ConstEdgeLocationPtr(new EdgeLocation(_e1cd, 0.0)),
      ConstEdgeLocationPtr(new EdgeLocation(_e1cd, 0.6)))));

    HOOT_STR_EQUALS(-40,
      details->calculateStringLocation(str, ConstEdgeLocationPtr(new EdgeLocation(_e1ab, 0.0))));
    HOOT_STR_EQUALS(0,
      details->calculateStringLocation(str, ConstEdgeLocationPtr(new EdgeLocation(_e1ab, 0.4))));
    HOOT_STR_EQUALS(60,
      details->calculateStringLocation(str, ConstEdgeLocationPtr(new EdgeLocation(_e1ab, 1.0))));
    HOOT_STR_EQUALS(110,
      details->calculateStringLocation(str, ConstEdgeLocationPtr(new EdgeLocation(_e1bc, 0.25))));
    HOOT_STR_EQUALS(260,
      details->calculateStringLocation(str, ConstEdgeLocationPtr(new EdgeLocation(_e1bc, 1.0))));
    HOOT_STR_EQUALS(260,
      details->calculateStringLocation(str, ConstEdgeLocationPtr(new EdgeLocation(_e1cd, 0.0))));
    HOOT_STR_EQUALS(360,
      details->calculateStringLocation(str, ConstEdgeLocationPtr(new EdgeLocation(_e1cd, 1.0))));
  }

  void extendEdgeMatchTest()
  {
    NetworkDetailsPtr details = createSampleDetails();

    EdgeStringPtr es1, es2;
    EdgeMatchPtr em, result;

    ///     *-----*---es1----|-------*
    /// *---------*|--es2----|-----------------*
    ///     |=====match======|
    es1.reset(new EdgeString());
    es1->appendEdge(EdgeSublinePtr(new EdgeSubline(
      ConstEdgeLocationPtr(new EdgeLocation(_e1bc, 0.1)),
      ConstEdgeLocationPtr(new EdgeLocation(_e1bc, 0.5)))));

    es2.reset(new EdgeString());
    es2->appendEdge(EdgeSublinePtr(new EdgeSubline(
      ConstEdgeLocationPtr(new EdgeLocation(_e2bc, 0.07)),
      ConstEdgeLocationPtr(new EdgeLocation(_e2bc, 0.33)))));

    em.reset(new EdgeMatch(es1, es2));

    result = details->extendEdgeMatch(em, _e1ab, _e2ab);

    HOOT_STR_EQUALS("s1: [2]{{ _start: { _e: (0) Node:-1 -- Way:-1 -- (1) Node:-2, _portion: 0 }, _end: { _e: (0) Node:-1 -- Way:-1 -- (1) Node:-2, _portion: 1 } }, { _start: { _e: (1) Node:-2 -- Way:-2 -- (2) Node:-3, _portion: 0 }, _end: { _e: (1) Node:-2 -- Way:-2 -- (2) Node:-3, _portion: 0.5 } }} s2: [2]{{ _start: { _e: (6) Node:-7 -- Way:-5 -- (7) Node:-8, _portion: 0.333333 }, _end: { _e: (6) Node:-7 -- Way:-5 -- (7) Node:-8, _portion: 1 } }, { _start: { _e: (7) Node:-8 -- Way:-6 -- (8) Node:-9, _portion: 0 }, _end: { _e: (7) Node:-8 -- Way:-6 -- (8) Node:-9, _portion: 0.33 } }}",
      result);

    ///                      *
    ///                      | e1cd
    ///     *-es1-*---e1bc---*
    /// *---|-es2-*------e2bc--------*
    ///     |=====match======|
    es1.reset(new EdgeString());
    es1->appendEdge(EdgeSublinePtr(new EdgeSubline(
      ConstEdgeLocationPtr(new EdgeLocation(_e1ab, 0)),
      ConstEdgeLocationPtr(new EdgeLocation(_e1ab, 1)))));

    es2.reset(new EdgeString());
    es2->appendEdge(EdgeSublinePtr(new EdgeSubline(
      ConstEdgeLocationPtr(new EdgeLocation(_e2ab, 0.33)),
      ConstEdgeLocationPtr(new EdgeLocation(_e2ab, 1)))));

    em.reset(new EdgeMatch(es1, es2));

    result = details->extendEdgeMatch(em, _e1bc, _e2bc);

    HOOT_STR_EQUALS("s1: [2]{{ _start: { _e: (0) Node:-1 -- Way:-1 -- (1) Node:-2, _portion: 0 }, _end: { _e: (0) Node:-1 -- Way:-1 -- (1) Node:-2, _portion: 1 } }, { _start: { _e: (1) Node:-2 -- Way:-2 -- (2) Node:-3, _portion: 0 }, _end: { _e: (1) Node:-2 -- Way:-2 -- (2) Node:-3, _portion: 1 } }} s2: [2]{{ _start: { _e: (6) Node:-7 -- Way:-5 -- (7) Node:-8, _portion: 0.33 }, _end: { _e: (6) Node:-7 -- Way:-5 -- (7) Node:-8, _portion: 1 } }, { _start: { _e: (7) Node:-8 -- Way:-6 -- (8) Node:-9, _portion: 0 }, _end: { _e: (7) Node:-8 -- Way:-6 -- (8) Node:-9, _portion: 0.666667 } }}",
      result);

    result = details->extendEdgeMatch(result, _e1cd, _e2bc);

    HOOT_STR_EQUALS("null", result);
  }

  void extendEdgeStringTest()
  {
    NetworkDetailsPtr details = createSampleDetails();

    EdgeStringPtr str(new EdgeString());
    str->appendEdge(EdgeSublinePtr(new EdgeSubline(
      ConstEdgeLocationPtr(new EdgeLocation(_e1bc, 0.4)),
      ConstEdgeLocationPtr(new EdgeLocation(_e1bc, 0.6)))));

    details->extendEdgeString(str, _e1ab);
    HOOT_STR_EQUALS("[2]{{ _start: { _e: (0) Node:-1 -- Way:-1 -- (1) Node:-2, _portion: 0 }, _end: { _e: (0) Node:-1 -- Way:-1 -- (1) Node:-2, _portion: 1 } }, { _start: { _e: (1) Node:-2 -- Way:-2 -- (2) Node:-3, _portion: 0 }, _end: { _e: (1) Node:-2 -- Way:-2 -- (2) Node:-3, _portion: 0.6 } }}", str);
    details->extendEdgeString(str, _e1bc);
    HOOT_STR_EQUALS("[2]{{ _start: { _e: (0) Node:-1 -- Way:-1 -- (1) Node:-2, _portion: 0 }, _end: { _e: (0) Node:-1 -- Way:-1 -- (1) Node:-2, _portion: 1 } }, { _start: { _e: (1) Node:-2 -- Way:-2 -- (2) Node:-3, _portion: 0 }, _end: { _e: (1) Node:-2 -- Way:-2 -- (2) Node:-3, _portion: 1 } }}", str);
    CPPUNIT_ASSERT_THROW(details->extendEdgeString(str, _e1df), IllegalArgumentException);

    // try again, but start with a backwards edge
    str.reset(new EdgeString());
    str->appendEdge(EdgeSublinePtr(new EdgeSubline(
      ConstEdgeLocationPtr(new EdgeLocation(_e1bc, 0.6)),
      ConstEdgeLocationPtr(new EdgeLocation(_e1bc, 0.4)))));

    details->extendEdgeString(str, _e1ab);
    HOOT_STR_EQUALS("[2]{{ _start: { _e: (1) Node:-2 -- Way:-2 -- (2) Node:-3, _portion: 0.6 }, _end: { _e: (1) Node:-2 -- Way:-2 -- (2) Node:-3, _portion: 0 } }, { _start: { _e: (0) Node:-1 -- Way:-1 -- (1) Node:-2, _portion: 1 }, _end: { _e: (0) Node:-1 -- Way:-1 -- (1) Node:-2, _portion: 0 } }}", str);
    details->extendEdgeString(str, _e1bc);
    HOOT_STR_EQUALS("[2]{{ _start: { _e: (1) Node:-2 -- Way:-2 -- (2) Node:-3, _portion: 1 }, _end: { _e: (1) Node:-2 -- Way:-2 -- (2) Node:-3, _portion: 0 } }, { _start: { _e: (0) Node:-1 -- Way:-1 -- (1) Node:-2, _portion: 1 }, _end: { _e: (0) Node:-1 -- Way:-1 -- (1) Node:-2, _portion: 0 } }}", str);
    CPPUNIT_ASSERT_THROW(details->extendEdgeString(str, _e1df), IllegalArgumentException);
    details->extendEdgeString(str, _e1cd);
    HOOT_STR_EQUALS("[3]{{ _start: { _e: (2) Node:-3 -- Way:-3 -- (3) Node:-4, _portion: 1 }, _end: { _e: (2) Node:-3 -- Way:-3 -- (3) Node:-4, _portion: 0 } }, { _start: { _e: (1) Node:-2 -- Way:-2 -- (2) Node:-3, _portion: 1 }, _end: { _e: (1) Node:-2 -- Way:-2 -- (2) Node:-3, _portion: 0 } }, { _start: { _e: (0) Node:-1 -- Way:-1 -- (1) Node:-2, _portion: 1 }, _end: { _e: (0) Node:-1 -- Way:-1 -- (1) Node:-2, _portion: 0 } }}", str);
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(NetworkDetailsTest, "slow");

}
