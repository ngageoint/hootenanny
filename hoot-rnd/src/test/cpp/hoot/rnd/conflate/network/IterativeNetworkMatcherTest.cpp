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
#include <hoot/core/conflate/MapCleaner.h>
#include <hoot/core/filters/ChainCriterion.h>
#include <hoot/core/filters/HighwayCriterion.h>
#include <hoot/core/filters/StatusCriterion.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/rnd/conflate/network/DebugNetworkMapCreator.h>
#include <hoot/rnd/conflate/network/IterativeNetworkMatcher.h>
#include <hoot/rnd/conflate/network/OsmNetworkExtractor.h>

namespace hoot
{

class IterativeNetworkMatcherTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(IterativeNetworkMatcherTest);
  CPPUNIT_TEST(toyTest);
  /// @todo this isn't being actively tested now, we're using vagabond for now...
  //CPPUNIT_TEST(edgeMatchTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void writeDebugMap(OsmMapPtr map, IterativeNetworkMatcher& uut, int index)
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
  void edgeMatchTest()
  {
    TestUtils::resetEnvironment();

    OsmMapPtr map(new OsmMap());

    OsmMapReaderFactory::getInstance().read(map, "test-files/conflate/network/ToyTestE1.osm", true,
      Status::Unknown1);
    OsmMapReaderFactory::getInstance().read(map, "test-files/conflate/network/ToyTestE2.osm", true,
      Status::Unknown2);
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

    IterativeNetworkMatcherPtr uut(new IterativeNetworkMatcher());
    uut->matchNetworks(map, network1, network2);

    /// @todo sloppy quick & dirty test to keep things moving
    HOOT_STR_EQUALS("[4]{(s1: [2]{Node:-225 -- Way:-232 -- Node:-230, Node:-288 -- Way:-236 -- Node:-230 (reverse)} s2: [2]{Node:-212 -- Way:-273 -- Node:-213, Node:-213 -- Way:-214 -- Node:-213}, 1), (s1: [2]{Node:-288 -- Way:-236 -- Node:-230, Node:-225 -- Way:-232 -- Node:-230 (reverse)} s2: [2]{Node:-213 -- Way:-214 -- Node:-213, Node:-212 -- Way:-273 -- Node:-213 (reverse)}, 1), (s1: [2]{Node:-33 -- Way:-45 -- Node:-37, Node:-41 -- Way:-47 -- Node:-37 (reverse)} s2: [1]{Node:-17 -- Way:-27 -- Node:-23}, 1), (s1: [2]{Node:-75 -- Way:-73 -- Node:-77, Node:-101 -- Way:-99 -- Node:-77 (reverse)} s2: [1]{Node:-144 -- Way:-148 -- Node:-151}, 1)}",
      uut->_edgeMatches);
  }

  /**
   * Extract a toy network and verify that the result is as expected.
   */
  void toyTest()
  {
    OsmMapPtr map(new OsmMap());

//    OsmMapReaderFactory::getInstance().read(map, "test-files/conflate/network/DcGisRoads.osm", false,
//      Status::Unknown1);
//    OsmMapReaderFactory::getInstance().read(map, "tmp/dcperb-multi10.osm", false,
//      Status::Unknown2);

    OsmMapReaderFactory::getInstance().read(map, "test-files/conflate/network/ToyTestB1.osm", false,
      Status::Unknown1);
    OsmMapReaderFactory::getInstance().read(map, "test-files/conflate/network/ToyTestB2.osm", false,
      Status::Unknown2);

//    OsmMapReaderFactory::getInstance().read(map, "test-files/cases/hoot-rnd/network/iterative/highway-001/Input1.osm", false,
//      Status::Unknown1);
//    OsmMapReaderFactory::getInstance().read(map, "test-files/cases/hoot-rnd/network/iterative/highway-001/Input2.osm", false,
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

    IterativeNetworkMatcherPtr uut = IterativeNetworkMatcher::create();
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

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(IterativeNetworkMatcherTest, "slow");

}
