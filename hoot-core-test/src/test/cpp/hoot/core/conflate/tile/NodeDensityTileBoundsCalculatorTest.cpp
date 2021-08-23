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
 * @copyright Copyright (C) 2012, 2013, 2014, 2015, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/conflate/tile/NodeDensityTileBoundsCalculator.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/io/OsmXmlWriter.h>


using namespace geos::geom;
using namespace std;

namespace hoot
{

/*
 * A lot of more recently added functionality in NodeDensityTileBoundsCalculator is tested in
 * NodeDensityTilesCmdTest.sh.
 */
class NodeDensityTileBoundsCalculatorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(NodeDensityTileBoundsCalculatorTest);
  CPPUNIT_TEST(runToyTest);
  CPPUNIT_TEST_SUITE_END();

public:

  NodeDensityTileBoundsCalculatorTest() :
  HootTestFixture("test-files/conflate/tile/", "test-output/conflate/tile/")
  {
    setResetType(ResetBasic);
  }

  void addEnvelope(OsmMapPtr map, Envelope& e, int tx, int ty)
  {
    WayPtr w = std::make_shared<Way>(Status::Unknown1, map->createNextWayId(), 10.0);
    NodePtr n1 =
      std::make_shared<Node>(
        Status::Unknown1, map->createNextNodeId(), e.getMinX(), e.getMinY(), 10.0);
    NodePtr n2 =
      std::make_shared<Node>(
        Status::Unknown1, map->createNextNodeId(), e.getMaxX(), e.getMinY(), 10.0);
    NodePtr n3 =
      std::make_shared<Node>(
        Status::Unknown1, map->createNextNodeId(), e.getMinX(), e.getMaxY(), 10.0);
    NodePtr n4 =
      std::make_shared<Node>(
        Status::Unknown1, map->createNextNodeId(), e.getMaxX(), e.getMaxY(), 10.0);

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
    OsmMapPtr map = std::make_shared<OsmMap>();
    reader.setDefaultStatus(Status::Unknown1);
    reader.read("test-files/DcGisRoads.osm", map);
    reader.setDefaultStatus(Status::Unknown2);
    reader.read("test-files/DcTigerRoads.osm", map);

    NodeDensityTileBoundsCalculator uut;
    uut.setPixelSize(0.1 / 360.0);
    uut.setMaxNodesPerTile(1000);
    uut.setSlop(0.1);
    uut.setMaxNumTries(1);
    uut._renderImage(map);

    uut._calculateTiles();
    vector<vector<Envelope>> e = uut.getTiles();

    OsmMapPtr bounds = std::make_shared<OsmMap>();

    for (size_t tx = 0; tx < e.size(); tx++)
    {
      for (size_t ty = 0; ty < e[tx].size(); ty++)
      {
        addEnvelope(bounds, e[tx][ty], tx, ty);
      }
    }

    OsmXmlWriter writer;
    writer.write(bounds, _outputPath + "TileBounds.osm");

    HOOT_FILE_EQUALS(_inputPath + "TileBounds.osm", _outputPath + "TileBounds.osm");
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(NodeDensityTileBoundsCalculatorTest, "quick");

}
