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
#include <hoot/core/algorithms/MapStitcher.h>
#include <hoot/core/io/IoUtils.h>

namespace hoot
{

class MapStitcherTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(MapStitcherTest);
  CPPUNIT_TEST(toyStitchPosIdsTest);
  CPPUNIT_TEST(toyStitchNegIdsTest);
  CPPUNIT_TEST(toyStitchMixedIdsTest);
  CPPUNIT_TEST(multipleDirectionTest);
  CPPUNIT_TEST(overAndBackTest);
  CPPUNIT_TEST_SUITE_END();

public:

  MapStitcherTest()
    : HootTestFixture("test-files/algorithms/MapStitcher/", "test-output/algorithms/MapStitcher/")
  {
  }

  void toyStitchPosIdsTest()
  {
    _runStitchTest(_inputPath + "ToyTestACrop1.osm", true,
                   _inputPath + "ToyTestACrop2.osm", true,
                   _outputPath + "ToyTestAStitch-Positive.osm",
                   _inputPath + "ToyTestAStitchExpected-Positive.osm");
  }

  void toyStitchNegIdsTest()
  {
    _runStitchTest(_inputPath + "ToyTestACrop1.osm", false,
                   _inputPath + "ToyTestACrop2.osm", false,
                   _outputPath + "ToyTestAStitch-Negative.osm",
                   _inputPath + "ToyTestAStitchExpected-Negative.osm");
  }

  void toyStitchMixedIdsTest()
  {
    _runStitchTest(_inputPath + "ToyTestACrop1.osm", true,
                   _inputPath + "ToyTestACrop2.osm", false,
                   _outputPath + "ToyTestAStitch-Mixed.osm",
                   _inputPath + "ToyTestAStitchExpected-Mixed.osm");
  }

  void multipleDirectionTest()
  {
    _runStitchTest(_inputPath + "OrderTest1.osm", true,
                   _inputPath + "OrderTest2.osm", true,
                   _outputPath + "OrderTest.osm",
                   _inputPath + "OrderTest-Expected.osm");
  }

  void overAndBackTest()
  {
    _runStitchTest(_inputPath + "OverAndBackTest1.osm", true,
                   _inputPath + "OverAndBackTest2.osm", true,
                   _outputPath + "OverAndBackTest.osm",
                   _inputPath + "OverAndBackTest-Expected.osm");
  }

private:

  void _runStitchTest(const QString& input1, bool use_file_ids_input1,
                      const QString& input2, bool use_file_ids_input2,
                      const QString& output, const QString& expected)
  {
    OsmMapPtr baseMap = std::make_shared<OsmMap>();
    OsmMapPtr secondaryMap = std::make_shared<OsmMap>();

    //  Load in the two maps for the map stitcher
    IoUtils::loadMap(baseMap, input1, use_file_ids_input1, Status::Unknown1);
    IoUtils::loadMap(secondaryMap, input2, use_file_ids_input2, Status::Unknown2);

    //  Run the stitcher on the two maps
    MapStitcher stitcher(baseMap);
    stitcher.stitchMap(secondaryMap);

    //  Save the output map
    IoUtils::saveMap(baseMap, output);

    HOOT_FILE_EQUALS(expected, output);
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(MapStitcherTest, "quick");

}

