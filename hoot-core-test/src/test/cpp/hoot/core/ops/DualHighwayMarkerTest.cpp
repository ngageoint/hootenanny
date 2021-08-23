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
 * @copyright Copyright (C) 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/ops/DualHighwayMarker.h>

#include <hoot/core/elements/MapProjector.h>

namespace hoot
{

class DualHighwayMarkerTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(DualHighwayMarkerTest);
  CPPUNIT_TEST(runBasicTest);
  CPPUNIT_TEST(runCrossingRoadsTest);
  CPPUNIT_TEST(runConfigureTest);
  CPPUNIT_TEST_SUITE_END();

public:

  DualHighwayMarkerTest() :
  HootTestFixture(
    "test-files/ops/DualHighwayMarkerTest/", "test-output/ops/DualHighwayMarkerTest/")
  {
  }

  void runBasicTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    OsmMapReaderFactory::read(map, _inputPath + "runBasicTestInput.osm", true);
    MapProjector::projectToPlanar(map);

    DualHighwayMarker uut;
    uut.setMarkCrossingRoads(false);
    uut.setMaxCrossingRoadsParallelScore(0.4);
    uut.setMinParallelScore(0.9);
    uut.apply(map);

    MapProjector::projectToWgs84(map);
    OsmMapWriterFactory::write(map, _outputPath + "runBasicTestOutput.osm");
    HOOT_FILE_EQUALS(_inputPath + "runBasicTestOutput.osm", _outputPath + "runBasicTestOutput.osm");
  }

  void runCrossingRoadsTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    OsmMapReaderFactory::read(map, _inputPath + "runCrossingRoadsTestInput.osm", true);
    MapProjector::projectToPlanar(map);

    DualHighwayMarker uut;
    uut.setMarkCrossingRoads(true);
    uut.setMaxCrossingRoadsParallelScore(0.4);
    uut.setMinParallelScore(0.9);
    uut.apply(map);

    MapProjector::projectToWgs84(map);
    OsmMapWriterFactory::write(map, _outputPath + "runCrossingRoadsTestOutput.osm");
    HOOT_FILE_EQUALS(
      _inputPath + "runCrossingRoadsTestOutput.osm",
      _outputPath + "runCrossingRoadsTestOutput.osm");
  }

  void runConfigureTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    OsmMapReaderFactory::read(map, _inputPath + "runBasicTestInput.osm", true);
    MapProjector::projectToPlanar(map);

    Settings settings;
    settings.set(ConfigOptions::getDualHighwayMarkerCrossingRoadsParallelScoreThresholdKey(), 0.4);
    settings.set(ConfigOptions::getDualHighwayMarkerMarkCrossingRoadsKey(), false);
    settings.set(ConfigOptions::getDualHighwayMarkerParallelScoreThresholdKey(), 0.9);
    DualHighwayMarker uut;
    uut.setConfiguration(settings);
    uut.apply(map);

    MapProjector::projectToWgs84(map);
    OsmMapWriterFactory::write(map, _outputPath + "runConfigureTestOutput.osm");
    HOOT_FILE_EQUALS(
      _inputPath + "runBasicTestOutput.osm", _outputPath + "runConfigureTestOutput.osm");
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(DualHighwayMarkerTest, "quick");

}
