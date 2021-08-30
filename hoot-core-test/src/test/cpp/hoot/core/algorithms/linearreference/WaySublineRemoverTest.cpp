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
#include <hoot/core/algorithms/linearreference/WaySublineRemover.h>
#include <hoot/core/elements/ElementGeometryUtils.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/elements/MapUtils.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>

namespace hoot
{

class WaySublineRemoverTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(WaySublineRemoverTest);
  CPPUNIT_TEST(runSplitInTheMiddleTest);
  CPPUNIT_TEST(runSplitAtStartTest);
  CPPUNIT_TEST(runSplitAtEndTest);
  CPPUNIT_TEST(runMapNotInPlanarTest);
  CPPUNIT_TEST_SUITE_END();

public:

  WaySublineRemoverTest() :
  HootTestFixture(
    "test-files/algorithms/linearreference/WaySublineRemoverTest/",
    "test-output/algorithms/linearreference/WaySublineRemoverTest/")
  {
  }

  void runSplitInTheMiddleTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    OsmMapReaderFactory::read(map, "test-files/ToyTestA.osm", false, Status::Unknown1);
    MapProjector::projectToPlanar(map);

    WayPtr way = std::dynamic_pointer_cast<Way>(MapUtils::getFirstElementWithNote(map, "1"));

    WayLocation start(map, way, 25.0);
    WayLocation end(map, way, 50.0);
    std::vector<ElementId> splitWayIds = WaySublineRemover::removeSubline(way, start, end, map);
    LOG_VART(splitWayIds.size());

    CPPUNIT_ASSERT_EQUAL((size_t)2, splitWayIds.size());

    OsmMapWriterFactory::write(map, _outputPath + "runSplitInTheMiddleTest-out.osm", false, true);
    HOOT_FILE_EQUALS(
      _inputPath + "runSplitInTheMiddleTest-out.osm",
      _outputPath + "runSplitInTheMiddleTest-out.osm");
  }

  void runSplitAtStartTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    OsmMapReaderFactory::read(map, "test-files/ToyTestA.osm", false, Status::Unknown1);
    MapProjector::projectToPlanar(map);

    WayPtr way = std::dynamic_pointer_cast<Way>(MapUtils::getFirstElementWithNote(map, "1"));

    WayLocation start(map, way, 0.0);
    WayLocation end(map, way, 50.0);
    std::vector<ElementId> splitWayIds = WaySublineRemover::removeSubline(way, start, end, map);
    LOG_VART(splitWayIds.size());

    CPPUNIT_ASSERT_EQUAL((size_t)1, splitWayIds.size());

    OsmMapWriterFactory::write(map, _outputPath + "runSplitAtStartTest-out.osm", false, true);
    HOOT_FILE_EQUALS(
      _inputPath + "runSplitAtStartTest-out.osm", _outputPath + "runSplitAtStartTest-out.osm");
  }

  void runSplitAtEndTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    OsmMapReaderFactory::read(map, "test-files/ToyTestA.osm", false, Status::Unknown1);
    MapProjector::projectToPlanar(map);

    WayPtr way = std::dynamic_pointer_cast<Way>(MapUtils::getFirstElementWithNote(map, "1"));

    WayLocation start(map, way, 25.0);
    WayLocation end(map, way, ElementGeometryUtils::calculateLength(way, map));
    std::vector<ElementId> splitWayIds = WaySublineRemover::removeSubline(way, start, end, map);
    LOG_VART(splitWayIds.size());

    CPPUNIT_ASSERT_EQUAL((size_t)1, splitWayIds.size());

    OsmMapWriterFactory::write(map, _outputPath + "runSplitAtEndTest-out.osm", false, true);
    HOOT_FILE_EQUALS(
      _inputPath + "runSplitAtEndTest-out.osm", _outputPath + "runSplitAtEndTest-out.osm");
  }

  void runMapNotInPlanarTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    // empty map defaults to wgs84; don't reproject it to planar
    WayPtr way;

    QString exceptionMsg;
    try
    {
      WaySublineRemover::removeSubline(way, WayLocation(), WayLocation(), map);
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT_EQUAL(
      QString("Map must be in a planar projection.").toStdString(), exceptionMsg.toStdString());
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(WaySublineRemoverTest, "quick");

}
