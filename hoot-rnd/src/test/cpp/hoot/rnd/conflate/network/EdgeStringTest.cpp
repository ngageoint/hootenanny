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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/MapProjector.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/util/GeometryUtils.h>
#include <hoot/rnd/conflate/network/EdgeString.h>
#include <hoot/rnd/conflate/network/OsmNetwork.h>

namespace hoot
{

class EdgeStringTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(EdgeStringTest);
  CPPUNIT_TEST(trimTest);
  CPPUNIT_TEST_SUITE_END();

public:
  NetworkEdgePtr _e12, _e23, _e34, _e46;
  OsmMapPtr _map;

  OsmNetworkPtr createSampleNetwork()
  {
    TestUtils::resetEnvironment();
    _map.reset(new OsmMap());
    OsmMap::resetCounters();
    auto_ptr<OGREnvelope> env(GeometryUtils::toOGREnvelope(Envelope(0, 1, 0, 1)));
    MapProjector::projectToPlanar(_map, *env);


    NodePtr n1(new Node(Status::Invalid, -1, 0, 0, 15.0));
    NetworkVertexPtr v1(new NetworkVertex(n1));
    NodePtr n2(new Node(Status::Invalid, -2, 0, 100, 15.0));
    NetworkVertexPtr v2(new NetworkVertex(n2));
    NodePtr n3(new Node(Status::Invalid, -3, 0, 300, 15.0));
    NetworkVertexPtr v3(new NetworkVertex(n3));
    NodePtr n4(new Node(Status::Invalid, -4, 100, 300, 15.0));
    NetworkVertexPtr v4(new NetworkVertex(n4));
    NodePtr n5(new Node(Status::Invalid, -5, 150, 350, 15.0));
    NetworkVertexPtr v5(new NetworkVertex(n5));
    NodePtr n6(new Node(Status::Invalid, -6, 200, 300, 15.0));
    NetworkVertexPtr v6(new NetworkVertex(n6));

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

    _map->addElement(n1);
    _map->addElement(n2);
    _map->addElement(n3);
    _map->addElement(n4);
    _map->addElement(n5);
    _map->addElement(n6);
    _map->addElement(w1);
    _map->addElement(w2);
    _map->addElement(w3);
    _map->addElement(w4);

    _e12.reset(new NetworkEdge(v1, v2, false));
    _e23.reset(new NetworkEdge(v2, v3, false));
    _e34.reset(new NetworkEdge(v3, v4, false));
    _e46.reset(new NetworkEdge(v4, v6, false));

    _e12->addMember(w1);
    _e23->addMember(w2);
    _e34->addMember(w3);
    _e46->addMember(w4);

    OsmNetworkPtr network1(new OsmNetwork());
    network1->addVertex(v1);
    network1->addVertex(v2);
    network1->addVertex(v3);
    network1->addVertex(v4);
    network1->addVertex(v5);
    network1->addVertex(v6);

    network1->addEdge(_e12);
    network1->addEdge(_e23);
    network1->addEdge(_e34);
    network1->addEdge(_e46);

    return network1;
  }

  void trimTest()
  {
    OsmNetworkPtr n = createSampleNetwork();

    EdgeStringPtr str;
    str.reset(new EdgeString());
    str->appendEdge(EdgeSublinePtr(new EdgeSubline(
      ConstEdgeLocationPtr(new EdgeLocation(_e12, 0.4)),
      ConstEdgeLocationPtr(new EdgeLocation(_e12, 1.0)))));
    str->appendEdge(_e23);
    str->appendEdge(EdgeSublinePtr(new EdgeSubline(
      ConstEdgeLocationPtr(new EdgeLocation(_e34, 0.0)),
      ConstEdgeLocationPtr(new EdgeLocation(_e34, 0.6)))));

    // trim the first and last edge
    str->trim(_map, 35, 300);
    HOOT_STR_EQUALS("[3]{{ _start: { _e: (0) Node:-1 -- Way:-1 -- (1) Node:-2, _portion: 0.75 }, _end: { _e: (0) Node:-1 -- Way:-1 -- (1) Node:-2, _portion: 1 } }, { _start: { _e: (1) Node:-2 -- Way:-2 -- (2) Node:-3, _portion: 0 }, _end: { _e: (1) Node:-2 -- Way:-2 -- (2) Node:-3, _portion: 1 } }, { _start: { _e: (2) Node:-3 -- Way:-3 -- (3) Node:-4, _portion: 0 }, _end: { _e: (2) Node:-3 -- Way:-3 -- (3) Node:-4, _portion: 0.4 } }}",
      str);

    str.reset(new EdgeString());
    str->appendEdge(_e12);

    // trim a single subline
    str->trim(_map, 35, 82);
    HOOT_STR_EQUALS("[1]{{ _start: { _e: (0) Node:-1 -- Way:-1 -- (1) Node:-2, _portion: 0.35 }, _end: { _e: (0) Node:-1 -- Way:-1 -- (1) Node:-2, _portion: 0.82 } }}",
      str);

    str.reset(new EdgeString());
    str->appendEdge(_e12);
    str->appendEdge(_e23);

    // trim off a whole subline
    str->trim(_map, 35, 82);
    HOOT_STR_EQUALS("[1]{{ _start: { _e: (0) Node:-1 -- Way:-1 -- (1) Node:-2, _portion: 0.35 }, _end: { _e: (0) Node:-1 -- Way:-1 -- (1) Node:-2, _portion: 0.82 } }}",
      str);

    str.reset(new EdgeString());
    str->appendEdge(_e12);
    str->appendEdge(_e23);

    // don't trim anything
    str->trim(_map, 0, 300);
    HOOT_STR_EQUALS("[2]{{ _start: { _e: (0) Node:-1 -- Way:-1 -- (1) Node:-2, _portion: 0 }, _end: { _e: (0) Node:-1 -- Way:-1 -- (1) Node:-2, _portion: 1 } }, { _start: { _e: (1) Node:-2 -- Way:-2 -- (2) Node:-3, _portion: 0 }, _end: { _e: (1) Node:-2 -- Way:-2 -- (2) Node:-3, _portion: 1 } }}",
      str);

    str.reset(new EdgeString());
    str->appendEdge(_e12);
    str->appendEdge(_e23);

    // trim off a whole subline
    str->trim(_map, 100, 120);
    HOOT_STR_EQUALS("[1]{{ _start: { _e: (1) Node:-2 -- Way:-2 -- (2) Node:-3, _portion: 0 }, _end: { _e: (1) Node:-2 -- Way:-2 -- (2) Node:-3, _portion: 0.1 } }}",
      str);
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(EdgeStringTest, "quick");

}
