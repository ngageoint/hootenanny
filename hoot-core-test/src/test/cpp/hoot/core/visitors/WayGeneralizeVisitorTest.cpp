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
 * @copyright Copyright (C) 2014, 2015, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/visitors/WayGeneralizeVisitor.h>

namespace hoot
{

class WayGeneralizeVisitorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(WayGeneralizeVisitorTest);
  CPPUNIT_TEST(runBasicTest);
  // TODO: update for custom crit
  CPPUNIT_TEST_SUITE_END();

public:

  WayGeneralizeVisitorTest()
    : HootTestFixture("test-files/visitors/WayGeneralizeVisitorTest/",
                      "test-output/visitors/WayGeneralizeVisitorTest/")
  {
    setResetType(ResetBasic);
  }

  void runBasicTest()
  {
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map,
      "test-files/visitors/RandomWayGeneralizerTest/RandomWayGeneralizerTest-in-1.osm");

    WayGeneralizeVisitor wayGeneralizeVisitor;
    wayGeneralizeVisitor.setEpsilon(5.0);
    map->visitRw(wayGeneralizeVisitor);

    const QString outputFile = _outputPath + "runBasicTest.osm";
    OsmMapWriterFactory::write(map, outputFile);

    HOOT_FILE_EQUALS(_inputPath + "runBasicTest.osm", outputFile);
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(WayGeneralizeVisitorTest, "quick");

}
