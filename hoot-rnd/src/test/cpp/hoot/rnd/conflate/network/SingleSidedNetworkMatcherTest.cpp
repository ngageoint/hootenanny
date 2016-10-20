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
 * @copyright Copyright (C) 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/MapProjector.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/conflate/MapCleaner.h>
#include <hoot/core/filters/ChainCriterion.h>
#include <hoot/core/filters/HighwayCriterion.h>
#include <hoot/core/filters/StatusCriterion.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/rnd/conflate/network/DebugNetworkMapCreator.h>
#include <hoot/rnd/conflate/network/SingleSidedNetworkMatcher.h>
#include <hoot/rnd/conflate/network/OsmNetworkExtractor.h>

namespace hoot
{

class SingleSidedNetworkMatcherTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(SingleSidedNetworkMatcherTest);
  CPPUNIT_TEST(toyTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void writeDebugMap(OsmMapPtr map, SingleSidedNetworkMatcher& uut, int index)
  {
    OsmMapPtr copy(new OsmMap(map));
    DebugNetworkMapCreator().addDebugElements(copy, uut.getAllEdgeScores(),
      uut.getAllVertexScores());

    MapProjector::projectToWgs84(copy);
    conf().set(ConfigOptions().getWriterIncludeDebugKey(), true);
    OsmMapWriterFactory::getInstance().write(copy, QString("tmp/dum-%1.osm").arg(index, 3, 10,
      QLatin1Char('0')));
  }

  /**
   * Extract a toy network and verify that the result is as expected.
   */
  void toyTest()
  {
    OsmMapPtr map(new OsmMap());

    OsmMapReaderFactory::getInstance().read(map, "test-files/conflate/network/ToyTestB1.osm", true,
      Status::Unknown1);
    OsmMapReaderFactory::getInstance().read(map, "test-files/conflate/network/ToyTestB2.osm", false,
      Status::Unknown2);

//    OsmMapReaderFactory::getInstance().read(map, "test-files/conflate/network/DcGisRoads.osm", true,
//      Status::Unknown1);
//    OsmMapReaderFactory::getInstance().read(map, "tmp/dcperb2.osm", false,
//      Status::Unknown2);

//    OsmMapReaderFactory::getInstance().read(map, "tmp/sub1.osm", true,
//      Status::Unknown1);
//    OsmMapReaderFactory::getInstance().read(map, "tmp/sub2.osm", false,
//      Status::Unknown2);

//    OsmMapReaderFactory::getInstance().read(map, "test-files/conflate/network/ToyTestD1.osm", true,
//      Status::Unknown1);
//    OsmMapReaderFactory::getInstance().read(map, "test-files/conflate/network/ToyTestD2.osm", true,
//      Status::Unknown2);

//    OsmMapReaderFactory::getInstance().read(map, "test-files/cases/hoot-rnd/network/highway-009/Input1.osm", false,
//      Status::Unknown1);
//    OsmMapReaderFactory::getInstance().read(map, "test-files/cases/hoot-rnd/network/highway-009/Input2.osm", false,
//      Status::Unknown2);

    MapCleaner().apply(map);
    MapProjector::projectToPlanar(map);

    OsmNetworkExtractor one;

    ElementCriterionPtr c1(
      new ChainCriterion(new HighwayCriterion(), new StatusCriterion(Status::Unknown1)));
    one.setCriterion(c1);
    OsmNetworkPtr network1 = one.extractNetwork(map);

    ElementCriterionPtr c2(
      new ChainCriterion(new HighwayCriterion(), new StatusCriterion(Status::Unknown2)));
    one.setCriterion(c2);
    OsmNetworkPtr network2 = one.extractNetwork(map);

    SingleSidedNetworkMatcherPtr uut = SingleSidedNetworkMatcher::create();
    uut->matchNetworks(map, network1, network2);

    writeDebugMap(map, *uut, 0);
    for (int i = 1; i <= 20; ++i)
    {
      LOG_VAR(i);
      uut->iterate();
      writeDebugMap(map, *uut, i);
    }

    TestUtils::resetEnvironment();
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(SingleSidedNetworkMatcherTest, "slow");

}
