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
 * @copyright Copyright (C) 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/ops/ImmediatelyConnectedOutOfBoundsWayTagger.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>

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
  //CPPUNIT_TEST(runBasicTest);
  CPPUNIT_TEST_SUITE_END();

public:

  ImmediatelyConnectedOutOfBoundsWayTaggerTest() :
  HootTestFixture(
    "test-files/ops/ImmediatelyConnectedOutOfBoundsWayTagger",
    "test-output/ops/ImmediatelyConnectedOutOfBoundsWayTagger")
  {
  }

  void runBasicTest()
  {
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map, true, true, "test-files/ops/ElementIdToVersionMapper/runBasicTest-in.osm");

    ImmediatelyConnectedOutOfBoundsWayTagger uut(geos::geom::Envelope(0, 0, 0, 0), true);
    uut.apply(map);

    MapProjector::projectToWgs84(map);
    OsmMapWriterFactory::write(map, _outputPath + "/runBasicTest-out.osm");
    HOOT_FILE_EQUALS(
      _inputPath + "/runBasicTest-out.osm", _outputPath + "/runBasicTest-out.osm");
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ImmediatelyConnectedOutOfBoundsWayTaggerTest, "quick");

}
