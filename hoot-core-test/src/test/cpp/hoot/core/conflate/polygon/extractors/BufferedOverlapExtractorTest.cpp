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
 * @copyright Copyright (C) 2013, 2014 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/conflate/polygon/extractors/BufferedOverlapExtractor.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/util/MetadataTags.h>
#include <hoot/core/visitors/FindWaysVisitor.h>

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Qt
#include <QDebug>
#include <QDir>
#include <QBuffer>
#include <QByteArray>

// Tgs
#include <tgs/StreamUtils.h>

#include "../../../TestUtils.h"

namespace hoot
{

class BufferedOverlapExtractorTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(BufferedOverlapExtractorTest);
  CPPUNIT_TEST(runRoadsTest);
  CPPUNIT_TEST(runBuildingsTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void setUp()
  {
    TestUtils::resetEnvironment();
  }

  boost::shared_ptr<OsmMap> _map;

  boost::shared_ptr<Node> createNode(double x, double y)
  {
    boost::shared_ptr<Node> n(new Node(Status::Unknown1, _map->createNextNodeId(), x, y, 10.0));
    _map->addNode(n);
    return n;
  }

  void runRoadsTest()
  {
    //test highway (linestring)
    boost::shared_ptr<OsmMap> map(new OsmMap());
    _map = map;

    boost::shared_ptr<Way> w1(new Way(Status::Unknown1, map->createNextWayId(), 13.0));
    w1->setTag("highway", "track");
    w1->setTag("name", "w1");
    w1->addNode(createNode(-104.9, 38.855)->getId());
    w1->addNode(createNode(-104.899, 38.8549)->getId());
    w1->addNode(createNode(-104.8991, 38.8544)->getId());
    _map->addWay(w1);

    boost::shared_ptr<Way> w2(new Way(Status::Unknown1, map->createNextWayId(), 13.0));
    w2->setTag("highway", "road");
    w2->setTag("name", "w2");
    w2->addNode(createNode(-104.91, 38.8548)->getId());
    w2->addNode(createNode(-104.8993, 38.8548)->getId());
    w2->addNode(createNode(-104.8991, 38.8544)->getId());
    _map->addWay(w2);

    BufferedOverlapExtractor uut(0.2);
    const OsmMap* constMap = const_cast<const OsmMap*>(_map.get());
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0,
                                 uut.extract(*constMap, boost::const_pointer_cast<const Way>(w1), boost::const_pointer_cast<const Way>(w2)),
                                 0.0);
  }

  void runBuildingsTest()
  {
    //test building (polygon)
    OsmXmlReader reader;

    OsmMap::resetCounters();
    boost::shared_ptr<OsmMap> map(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.read("test-files/ToyBuildingsTestA.osm", map);
    reader.setDefaultStatus(Status::Unknown2);
    reader.read("test-files/ToyBuildingsTestB.osm", map);

    MapProjector::projectToPlanar(map);

    vector<long> r1 = FindWaysVisitor::findWaysByTag(map, MetadataTags::Ref1(), "Target");
    vector<long> r2 = FindWaysVisitor::findWaysByTag(map, "name", "Target Grocery");

    const boost::shared_ptr<const Way> w = map->getWay(r1[0]);
    const boost::shared_ptr<const Way> w1 = map->getWay(r2[0]);

    MapProjector::projectToPlanar(map);

    const OsmMap* constMap = const_cast<const OsmMap*>(map.get());

    BufferedOverlapExtractor uut(0.2);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.671148, uut.extract(*constMap, w, w1), 0.00001);
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(BufferedOverlapExtractorTest, "quick");

}
