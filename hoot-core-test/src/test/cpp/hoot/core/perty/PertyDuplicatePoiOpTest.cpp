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
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Hoot
#include <hoot/core/util/Exception.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/perty/PertyDuplicatePoiOp.h>
#include <hoot/core/util/Log.h>

// tbs
#include <tbs/stats/SampleStats.h>

// Qt
#include <QDir>
#include <QSet>

using namespace std;

namespace hoot
{

class PertyDuplicatePoiOpTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(PertyDuplicatePoiOpTest);
  CPPUNIT_TEST(runBasicTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runBasicTest()
  {
    boost::shared_ptr<OsmMap> map(new OsmMap());
    OGREnvelope env;
    env.MinX = 0;
    env.MaxX = 1;
    env.MinY = 0;
    env.MaxY = 1;
    MapProjector::projectToPlanar(map, env);

    map->resetCounters();
    for (int i = 0; i < 10; i++)
    {
      NodePtr n(new Node(Status::Unknown1, map->createNextNodeId(), i * 100, 0, 10));
      n->getTags()["name"] = QString("n%1").arg(i);
      map->addNode(n);
    }

    boost::minstd_rand rng;
    rng.seed(1);
    PertyDuplicatePoiOp uut;
    uut.setRng(rng);
    uut.setDuplicateSigma(1.0);
    uut.setMoveMultiplier(0.5);
    uut.setProbability(0.5);
    uut.apply(map);

    QSet<long> nids;
    NodeMap::const_iterator it = map->getNodes().begin();
    while (it != map->getNodes().end()) {
      nids.insert(it->first);
      it++;
    }
    QList<long> keys = QList<long>::fromSet(nids);

    qSort(keys);

    // handy for recreating all the CPPUNIT_ASSERT* statements
//    QString str;
//    for (int i = 0; i < keys.size(); i++)
//    {
//      NodePtr n = map->getNode(keys[i]);
//      str += QString("n = map->getNode(keys[%1]);\n").arg(i);
//      str += QString("CPPUNIT_ASSERT_DOUBLES_EQUAL(%1, n->getX(), 1e-3);\n").arg(n->getX());
//      str += QString("CPPUNIT_ASSERT_DOUBLES_EQUAL(%1, n->getY(), 1e-3);\n").arg(n->getY());
//    }
//    LOG_WARN(str);

    NodePtr n;
    n = map->getNode(keys[0]);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(802.711, n->getX(), 1e-3);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.70974, n->getY(), 1e-3);
    n = map->getNode(keys[1]);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(799.6659, n->getX(), 1e-3);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(4.11058, n->getY(), 1e-3);
    n = map->getNode(keys[2]);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(901.11255, n->getX(), 1e-3);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.79405, n->getY(), 1e-3);
    n = map->getNode(keys[3]);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(905.09512, n->getX(), 1e-3);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-4.12888, n->getY(), 1e-3);
    n = map->getNode(keys[4]);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(900, n->getX(), 1e-3);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0, n->getY(), 1e-3);
    n = map->getNode(keys[5]);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(800, n->getX(), 1e-3);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0, n->getY(), 1e-3);
    n = map->getNode(keys[6]);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(700, n->getX(), 1e-3);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0, n->getY(), 1e-3);
    n = map->getNode(keys[7]);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(600, n->getX(), 1e-3);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0, n->getY(), 1e-3);
    n = map->getNode(keys[8]);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(500, n->getX(), 1e-3);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0, n->getY(), 1e-3);
    n = map->getNode(keys[9]);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(400, n->getX(), 1e-3);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0, n->getY(), 1e-3);
    n = map->getNode(keys[10]);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(300, n->getX(), 1e-3);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0, n->getY(), 1e-3);
    n = map->getNode(keys[11]);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(200, n->getX(), 1e-3);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0, n->getY(), 1e-3);
    n = map->getNode(keys[12]);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(100, n->getX(), 1e-3);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0, n->getY(), 1e-3);
    n = map->getNode(keys[13]);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0, n->getX(), 1e-3);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0, n->getY(), 1e-3);

  }
};

//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(PertyDuplicatePoiOpTest, "current");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(PertyDuplicatePoiOpTest, "quick");

}
