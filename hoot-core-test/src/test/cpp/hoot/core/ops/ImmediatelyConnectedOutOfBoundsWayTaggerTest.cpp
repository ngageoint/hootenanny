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
 * @copyright Copyright (C) 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/ops/ImmediatelyConnectedOutOfBoundsWayTagger.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/geometry/GeometryUtils.h>

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

namespace hoot
{

class ImmediatelyConnectedOutOfBoundsWayTaggerTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(ImmediatelyConnectedOutOfBoundsWayTaggerTest);
  CPPUNIT_TEST(runStrictBoundsTest);
  CPPUNIT_TEST(runLenientBoundsTest);
  CPPUNIT_TEST_SUITE_END();

public:

  ImmediatelyConnectedOutOfBoundsWayTaggerTest() :
  HootTestFixture(
    "test-files/ops/ImmediatelyConnectedOutOfBoundsWayTagger/",
    "test-output/ops/ImmediatelyConnectedOutOfBoundsWayTagger/")
  {
  }

  void runStrictBoundsTest()
  {
    const QString testName = "runStrictBoundsTest";
    geos::geom::Envelope bounds(38.91362, 38.915478, 15.37365, 15.37506);
    OsmMapWriterFactory::write(
      GeometryUtils::createMapFromBounds(bounds), _outputPath + testName + "-bounds.osm");

    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(map, true, true,  _inputPath + "in.osm");

    ImmediatelyConnectedOutOfBoundsWayTagger uut(GeometryUtils::envelopeToPolygon(bounds), true);
    uut.apply(map);

    const QString outFileName = testName + "-out.osm";
    OsmMapWriterFactory::write(map, _outputPath + outFileName, false, true);

    // The way we're considering as the source way is completely within the bounds and we are
    // strictly interpreting the bounds. Both of the two ways connected to it are completely outside
    // of the bounds. So, those two should get tagged.
    CPPUNIT_ASSERT_EQUAL(2L, uut.getNumTagged());
    HOOT_FILE_EQUALS(_inputPath + outFileName, _outputPath + outFileName);
  }

  void runLenientBoundsTest()
  {
    const QString testName = "runLenientBoundsTest";
    geos::geom::Envelope bounds(38.91404, 38.91506, 15.3740, 15.37513);
    OsmMapWriterFactory::write(
      GeometryUtils::createMapFromBounds(bounds), _outputPath + testName + "-bounds.osm");

    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(map, true, true,  _inputPath + "in.osm");

    ImmediatelyConnectedOutOfBoundsWayTagger uut(GeometryUtils::envelopeToPolygon(bounds), false);
    uut.apply(map);

    const QString outFileName = testName + "-out.osm";
    OsmMapWriterFactory::write(map, _outputPath + outFileName, false, true);

    // The way we're considering as the source way is crosses the bounds and we are interpreting the
    // bounds in a lenient fashion. Both of the two ways connected to it are completely outside of
    // the bounds. So, those two should get tagged.
    CPPUNIT_ASSERT_EQUAL(2L, uut.getNumTagged());
    HOOT_FILE_EQUALS(_inputPath + outFileName, _outputPath + outFileName);
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ImmediatelyConnectedOutOfBoundsWayTaggerTest, "quick");

}
