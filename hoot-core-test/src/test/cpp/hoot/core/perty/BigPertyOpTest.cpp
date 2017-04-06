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

// CPP Unit
#include "../TestUtils.h"

// Hoot
#include <hoot/core/util/Exception.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/perty/BigPertyOp.h>
#include <hoot/core/util/GeometryUtils.h>
#include <hoot/core/util/Log.h>

// tbs
#include <tbs/stats/SampleStats.h>

// Qt
#include <QDir>
#include <QSet>

using namespace std;

namespace hoot
{

class BigPertyOpTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(BigPertyOpTest);
  CPPUNIT_TEST(runBasicTest);
  CPPUNIT_TEST(runMaxDistanceTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runBasicTest()
  {
    TestUtils::resetEnvironment();
    boost::shared_ptr<OsmMap> map(new OsmMap());
    OGREnvelope env;
    env.MinX = 0;
    env.MinY = 0;
    env.MaxX = 1;
    env.MaxY = 1;

    for (double x = 0.0; x <= 4.0; x += 1.0)
    {
      for (double y = 0.0; y <= 4.0; y += 1.0)
      {
        NodePtr n(new Node(Status::Unknown1, map->createNextNodeId(), x, y, 10));
        map->addNode(n);
      }
    }

    BigPertyOp uut;
    Envelope env2(0.5, 2.5, 1.5, 3.5);
    uut.setBounds(env2);
    uut.setSigma(10);
    uut.apply(map);

    ////
    // Handy bit for regenerating the test values, _AFTER_ it has been visually verified.
    ////
    QSet<long> nids;
    NodeMap::const_iterator it = map->getNodes().begin();
    while (it != map->getNodes().end()) {
      nids.insert(it->first);
      it++;
    }
    QList<long> keys = QList<long>::fromSet(nids);
    qSort(keys);
//    OsmXmlWriter writer;
//    QDir().mkpath("test-output/perty");
//    MapProjector::reprojectToWgs84(map);
//    writer.write(map, "test-output/perty/BasicTest.osm");
//    QString result = "";
//    for (int i = 0; i < keys.size(); i++)
//    {
//      const NodePtr& n = map->getNode(keys[i]);
//      result += QString("n = map->getNode(keys[%1]);\n").arg(i);
//      result += QString("CPPUNIT_ASSERT_DOUBLES_EQUAL(%1, n->getX(), 1e-6);\n").arg(n->getX(), 0,
//                                                                                    'g', 9);
//      result += QString("CPPUNIT_ASSERT_DOUBLES_EQUAL(%1, n->getY(), 1e-6);\n").arg(n->getY(), 0,
//                                                                                    'g', 9);
//    }
//    LOG_INFO(result);

    NodePtr n;
    n = map->getNode(keys[0]);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(4, n->getX(), 1e-6);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(4, n->getY(), 1e-6);
    n = map->getNode(keys[1]);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(4, n->getX(), 1e-6);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3, n->getY(), 1e-6);
    n = map->getNode(keys[2]);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(4, n->getX(), 1e-6);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2, n->getY(), 1e-6);
    n = map->getNode(keys[3]);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(4, n->getX(), 1e-6);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1, n->getY(), 1e-6);
    n = map->getNode(keys[4]);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(4, n->getX(), 1e-6);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0, n->getY(), 1e-6);
    n = map->getNode(keys[5]);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3, n->getX(), 1e-6);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(4, n->getY(), 1e-6);
    n = map->getNode(keys[6]);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3, n->getX(), 1e-6);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3, n->getY(), 1e-6);
    n = map->getNode(keys[7]);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3, n->getX(), 1e-6);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2, n->getY(), 1e-6);
    n = map->getNode(keys[8]);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3, n->getX(), 1e-6);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1, n->getY(), 1e-6);
    n = map->getNode(keys[9]);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3, n->getX(), 1e-6);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0, n->getY(), 1e-6);
    n = map->getNode(keys[10]);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2, n->getX(), 1e-6);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(4, n->getY(), 1e-6);
    n = map->getNode(keys[11]);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.00000001, n->getX(), 1e-6);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.00003791, n->getY(), 1e-6);
    n = map->getNode(keys[12]);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.9998872, n->getX(), 1e-6);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.99984757, n->getY(), 1e-6);
    n = map->getNode(keys[13]);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2, n->getX(), 1e-6);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1, n->getY(), 1e-6);
    n = map->getNode(keys[14]);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2, n->getX(), 1e-6);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0, n->getY(), 1e-6);
    n = map->getNode(keys[15]);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1, n->getX(), 1e-6);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(4, n->getY(), 1e-6);
    n = map->getNode(keys[16]);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.999988319, n->getX(), 1e-6);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.00005715, n->getY(), 1e-6);
    n = map->getNode(keys[17]);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.999991482, n->getX(), 1e-6);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.9999098, n->getY(), 1e-6);
    n = map->getNode(keys[18]);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1, n->getX(), 1e-6);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1, n->getY(), 1e-6);
    n = map->getNode(keys[19]);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1, n->getX(), 1e-6);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0, n->getY(), 1e-6);
    n = map->getNode(keys[20]);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0, n->getX(), 1e-6);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(4, n->getY(), 1e-6);
    n = map->getNode(keys[21]);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0, n->getX(), 1e-6);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3, n->getY(), 1e-6);
    n = map->getNode(keys[22]);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0, n->getX(), 1e-6);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2, n->getY(), 1e-6);
    n = map->getNode(keys[23]);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0, n->getX(), 1e-6);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1, n->getY(), 1e-6);
    n = map->getNode(keys[24]);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0, n->getX(), 1e-6);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0, n->getY(), 1e-6);
  }

  void runMaxDistanceTest()
  {
    boost::shared_ptr<OsmMap> map(new OsmMap());
    OGREnvelope env;
    env.MinX = 0;
    env.MinY = 0;
    env.MaxX = 1;
    env.MaxY = 1;

    QList<long> ids;
    for (double x = 0.0; x <= 4.0; x += 0.1)
    {
      for (double y = 0.0; y <= 4.0; y += 0.1)
      {
        NodePtr n(new Node(Status::Unknown1, map->createNextNodeId(), x, y, 10));
        map->addNode(n);
        ids.append(n->getId());
      }
    }

    BigPertyOp uut;
    uut.setSigma(10);
    uut.setMaxDistance(15);
    uut.apply(map);

    int i = 0;
    for (double x = 0.0; x <= 4.0; x += 0.1)
    {
      for (double y = 0.0; y <= 4.0; y += 0.1)
      {
        long id = ids[i++];
        NodePtr n = map->getNode(id);
        double d = GeometryUtils::haversine(n->toCoordinate(), Coordinate(x, y));
        CPPUNIT_ASSERT_EQUAL(true, d <= 15);
      }
    }

  }
};

//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(BigPertyOpTest, "current");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(BigPertyOpTest, "quick");

}
