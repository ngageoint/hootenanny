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
 * @copyright Copyright (C) 2019, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/algorithms/splitter/OsmMapSplitter.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/io/OsmMapReaderFactory.h>

namespace hoot
{

class OsmMapSplitterTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(OsmMapSplitterTest);
  CPPUNIT_TEST(runTestSmall);
  CPPUNIT_TEST(runTestBoston);
  CPPUNIT_TEST(runTestOutside);
  CPPUNIT_TEST_SUITE_END();

public:
  OsmMapSplitterTest()
    : HootTestFixture("test-files/algorithms/splitter/",
                      "test-output/algorithms/splitter/")
  {
    setResetType(ResetConfigs);
  }

  void runTestSmall()
  {
    //  Load in the tile map
    OsmMapPtr tiles = std::make_shared<OsmMap>();
    OsmMapReaderFactory::read(tiles, _inputPath + "OsmMapSplitterTestTiles_ToyTest.geojson");
    //  Load in the map to split
    OsmMapPtr map = std::make_shared<OsmMap>();
    OsmMapReaderFactory::read(map, "test-files/ToyBuildingsTestA.osm");

    OsmMapSplitter splitter(map, tiles);
    splitter.apply();

    splitter.writeMaps(_outputPath + "OsmMapSplitterTestOutput_ToyTest.osm");

    HOOT_FILE_EQUALS( _inputPath + "OsmMapSplitterTestOutput_ToyTest.osm",
                     _outputPath + "OsmMapSplitterTestOutput_ToyTest.osm");
    HOOT_FILE_EQUALS( _inputPath + "OsmMapSplitterTestOutput_ToyTest-00001.osm",
                     _outputPath + "OsmMapSplitterTestOutput_ToyTest-00001.osm");
    HOOT_FILE_EQUALS( _inputPath + "OsmMapSplitterTestOutput_ToyTest-00002.osm",
                     _outputPath + "OsmMapSplitterTestOutput_ToyTest-00002.osm");
    HOOT_FILE_EQUALS( _inputPath + "OsmMapSplitterTestOutput_ToyTest-00003.osm",
                     _outputPath + "OsmMapSplitterTestOutput_ToyTest-00003.osm");
  }

  void runTestBoston()
  {
    Settings::getInstance().set(ConfigOptions::getLogWarnMessageLimitKey(), 100);
    //  Load in the tile map
    OsmMapPtr tiles = std::make_shared<OsmMap>();
    OsmMapReaderFactory::read(tiles, _inputPath + "OsmMapSplitterTestTiles_Boston.geojson");
    //  Load in the map to split
    OsmMapPtr map = std::make_shared<OsmMap>();
    OsmMapReaderFactory::read(map, _inputPath + "OsmMapSplitterTestInput_Boston.osm");

    OsmMapSplitter splitter(map, tiles);
    splitter.apply();

    splitter.writeMaps(_outputPath + "OsmMapSplitterTestOutput_Boston.osm");

    HOOT_FILE_EQUALS( _inputPath + "OsmMapSplitterTestOutput_Boston.osm",
                     _outputPath + "OsmMapSplitterTestOutput_Boston.osm");
    HOOT_FILE_EQUALS( _inputPath + "OsmMapSplitterTestOutput_Boston-00001.osm",
                     _outputPath + "OsmMapSplitterTestOutput_Boston-00001.osm");
    HOOT_FILE_EQUALS( _inputPath + "OsmMapSplitterTestOutput_Boston-00002.osm",
                     _outputPath + "OsmMapSplitterTestOutput_Boston-00002.osm");
    HOOT_FILE_EQUALS( _inputPath + "OsmMapSplitterTestOutput_Boston-00003.osm",
                     _outputPath + "OsmMapSplitterTestOutput_Boston-00003.osm");
  }

  void runTestOutside()
  {
    //  Load in the tile map
    OsmMapPtr tiles = std::make_shared<OsmMap>();
    OsmMapReaderFactory::read(tiles, _inputPath + "OsmMapSplitterTestTiles_ToyTest_Outside.geojson");
    //  Load in the map to split
    OsmMapPtr map = std::make_shared<OsmMap>();
    OsmMapReaderFactory::read(map, "test-files/ToyBuildingsTestA.osm");

    OsmMapSplitter splitter(map, tiles);
    splitter.apply();

    splitter.writeMaps(_outputPath + "OsmMapSplitterTestOutput_ToyTest_Outside.osm");

    HOOT_FILE_EQUALS( _inputPath + "OsmMapSplitterTestOutput_ToyTest_Outside.osm",
                     _outputPath + "OsmMapSplitterTestOutput_ToyTest_Outside.osm");
    HOOT_FILE_EQUALS( _inputPath + "OsmMapSplitterTestOutput_ToyTest_Outside-00001.osm",
                     _outputPath + "OsmMapSplitterTestOutput_ToyTest_Outside-00001.osm");

  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(OsmMapSplitterTest, "slow");

}
