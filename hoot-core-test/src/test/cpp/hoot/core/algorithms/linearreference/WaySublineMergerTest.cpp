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
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/algorithms/linearreference/WaySublineMerger.h>
#include <hoot/core/elements/ElementGeometryUtils.h>
#include <hoot/core/elements/MapUtils.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/elements/MapProjector.h>

namespace hoot
{

class WaySublineMergerTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(WaySublineMergerTest);
  CPPUNIT_TEST(runBasicTest);
  CPPUNIT_TEST_SUITE_END();

public:

  WaySublineMergerTest() :
  HootTestFixture(
    "test-files/algorithms/linearreference/WaySublineMergerTest/",
    "test-output/algorithms/linearreference/WaySublineMergerTest/")
  {
    //setResetType(ResetBasic);
  }

  void runBasicTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    OsmMapReaderFactory::read(
      map, _inputPath + "WaySublineMergerTest-runBasicTest.osm", false, Status::Unknown1);
    MapProjector::projectToPlanar(map);

    WayPtr way1 = std::dynamic_pointer_cast<Way>(MapUtils::getFirstElementWithNote(map, "1"));
    WaySubline subline1(way1, map);
    WayPtr way2 = std::dynamic_pointer_cast<Way>(MapUtils::getFirstElementWithNote(map, "2"));
    WaySubline subline2(way2, map);

    WaySubline mergedSubline = WaySublineMerger::mergeSublines(subline1, subline2);
    OsmMapPtr outMap = std::make_shared<OsmMap>();
    WayPtr mergedWay = mergedSubline.toWay(outMap, nullptr, true);
    outMap->addWay(mergedWay);

    OsmMapWriterFactory::write(
      outMap, _outputPath + "WaySublineMergerTest-runBasicTest-out.osm", false, true);
//    HOOT_FILE_EQUALS(
//      _inputPath + "runSplitInTheMiddleTest-out.osm",
//      _outputPath + "runSplitInTheMiddleTest-out.osm");
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(WaySublineMergerTest, "quick");

}
