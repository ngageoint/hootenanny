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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/ops/MapCleaner.h>
#include <hoot/core/conflate/network/DebugNetworkMapCreator.h>
#include <hoot/core/conflate/network/OsmNetworkExtractor.h>
#include <hoot/core/criterion/ChainCriterion.h>
#include <hoot/core/criterion/HighwayCriterion.h>
#include <hoot/core/criterion/StatusCriterion.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/rnd/conflate/network/SingleSidedNetworkMatcher.h>

namespace hoot
{

class SingleSidedNetworkMatcherTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(SingleSidedNetworkMatcherTest);
  CPPUNIT_TEST(toyTest);
  CPPUNIT_TEST_SUITE_END();

public:

  SingleSidedNetworkMatcherTest()
    : HootTestFixture("test-files/conflate/network/",
                      "test-output/conflate/network/")
  {
    setResetType(ResetAll);
  }

  void writeDebugMap(OsmMapPtr map, SingleSidedNetworkMatcher& uut, int index)
  {
    FileUtils::makeDir("tmp");
    OsmMapPtr copy(new OsmMap(map));
    DebugNetworkMapCreator().addDebugElements(copy, uut.getAllEdgeScores(),
      uut.getAllVertexScores());

    MapProjector::projectToWgs84(copy);
    conf().set(ConfigOptions().getWriterIncludeDebugTagsKey(), true);
    OsmMapWriterFactory::write(copy, QString("tmp/SingleSidedNetworkMatcherTest-%1.osm").arg(index, 3, 10,
      QLatin1Char('0')));
  }

  /**
   * Extract a toy network and verify that the result is as expected.
   */
  void toyTest()
  {
    OsmMapPtr map(new OsmMap());

    OsmMapReaderFactory::read(map, _inputPath + "ToyTestB1.osm", true, Status::Unknown1);
    OsmMapReaderFactory::read(map, _inputPath + "ToyTestB2.osm", false, Status::Unknown2);

    MapCleaner().apply(map);
    MapProjector::projectToPlanar(map);

    OsmNetworkExtractor one;

    ElementCriterionPtr c1(
      new ChainCriterion(new HighwayCriterion(map), new StatusCriterion(Status::Unknown1)));
    one.setCriterion(c1);
    OsmNetworkPtr network1 = one.extractNetwork(map);

    ElementCriterionPtr c2(
      new ChainCriterion(new HighwayCriterion(map), new StatusCriterion(Status::Unknown2)));
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

    // write final map and compare
    DebugNetworkMapCreator().addDebugElements(map, uut->getAllEdgeScores(), uut->getAllVertexScores());
    MapProjector::projectToWgs84(map);
    conf().set(ConfigOptions().getWriterIncludeDebugTagsKey(), true);
    OsmMapWriterFactory::write(map, QString(_outputPath + "SingleSidedNetworkMatcherTestFinal.osm"));

    HOOT_FILE_EQUALS(_inputPath + "SingleSidedNetworkMatcherTestExpected.osm",
                    _outputPath + "SingleSidedNetworkMatcherTestFinal.osm");
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(SingleSidedNetworkMatcherTest, "glacial");

}
