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
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/ops/PointsToPolysConverter.h>

namespace hoot
{

class PointsToPolysConverterTest : public HootTestFixture
{
    CPPUNIT_TEST_SUITE(PointsToPolysConverterTest);
    CPPUNIT_TEST(runBasicTest);
    CPPUNIT_TEST_SUITE_END();

public:

  PointsToPolysConverterTest() :
  HootTestFixture(
    "test-files/ops/PointsToPolysConverterTest/", "test-output/ops/PointsToPolysConverterTest/")
  {
  }

  void runBasicTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    OsmMapReaderFactory::read(map, "test-files/mapcruzinpoi_clip.osm");

    PointsToPolysConverter uut(1.0);
    uut.apply(map);

    MapProjector::projectToWgs84(map);
    OsmMapWriterFactory::write(map, _outputPath + "runBasicTest-out.osm");
    HOOT_FILE_EQUALS(_inputPath + "runBasicTest-out.osm", _outputPath + "runBasicTest-out.osm");
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(PointsToPolysConverterTest, "quick");

}
