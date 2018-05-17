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
 * @copyright Copyright (C) 2012, 2013, 2014, 2015, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/conflate/tile/TileBoundsCalculator.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/util/Log.h>

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Qt
#include <QDebug>
#include <QDir>

#include "../../TestUtils.h"

using namespace geos::geom;
using namespace std;

namespace hoot
{

class TileBoundsCalculatorTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(TileBoundsCalculatorTest);
  CPPUNIT_TEST(runToyTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void setUp()
  {
    TestUtils::mkpath("test-output/conflate");
  }

  void addEnvelope(OsmMapPtr map, Envelope& e, int tx, int ty)
  {
    WayPtr w(new Way(Status::Unknown1, map->createNextWayId(), 10.0));
    NodePtr n1(new Node(Status::Unknown1, map->createNextNodeId(), e.getMinX(), e.getMinY(),
                                 10.0));
    NodePtr n2(new Node(Status::Unknown1, map->createNextNodeId(), e.getMaxX(), e.getMinY(),
                                 10.0));
    NodePtr n3(new Node(Status::Unknown1, map->createNextNodeId(), e.getMinX(), e.getMaxY(),
                                 10.0));
    NodePtr n4(new Node(Status::Unknown1, map->createNextNodeId(), e.getMaxX(), e.getMaxY(),
                                 10.0));

    map->addNode(n1);
    map->addNode(n2);
    map->addNode(n3);
    map->addNode(n4);

    w->addNode(n1->getId());
    w->addNode(n2->getId());
    w->addNode(n4->getId());
    w->addNode(n3->getId());
    w->addNode(n1->getId());
    w->setTag("tx", QString("%1").arg(tx));
    w->setTag("ty", QString("%1").arg(ty));

    map->addWay(w);
  }

  void runToyTest()
  {
    OsmXmlReader reader;

    OsmMap::resetCounters();
    OsmMapPtr map(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.read("test-files/DcGisRoads.osm", map);
    reader.setDefaultStatus(Status::Unknown2);
    reader.read("test-files/DcTigerRoads.osm", map);

    TileBoundsCalculator uut(0.1 / 360.0);

    uut.setMaxNodesPerBox(1000);
    uut.setSlop(0.10);
    uut.renderImage(map);

    vector< vector<Envelope> > e = uut.calculateTiles();

    OsmMapPtr bounds(new OsmMap());

    for (size_t tx = 0; tx < e.size(); tx++)
    {
      for (size_t ty = 0; ty < e[tx].size(); ty++)
      {
        addEnvelope(bounds, e[tx][ty], tx, ty);
      }
    }

    OsmXmlWriter writer;
    writer.write(bounds, "test-output/conflate/TileBounds.osm");

    HOOT_FILE_EQUALS("test-files/conflate/TileBounds.osm",
                     "test-output/conflate/TileBounds.osm");

  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(TileBoundsCalculatorTest, "quick");
//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(TileBoundsCalculatorTest, "current");

}
