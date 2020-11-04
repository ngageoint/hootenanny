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
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/visitors/WayGeneralizeVisitor.h>
#include <hoot/core/criterion/HighwayCriterion.h>

namespace hoot
{

class WayGeneralizeVisitorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(WayGeneralizeVisitorTest);
  CPPUNIT_TEST(runBasicTest);
  CPPUNIT_TEST(runCritTest);
  CPPUNIT_TEST(runRemoveSharedNodeTest);
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

    WayGeneralizeVisitor uut;
    uut.setEpsilon(5.0);
    uut.setRemoveNodesSharedByWays(true);
    map->visitRw(uut);

    const QString outputFile = _outputPath + "runBasicTest.osm";
    OsmMapWriterFactory::write(map, outputFile);

    HOOT_FILE_EQUALS(_inputPath + "runBasicTest.osm", outputFile);
  }

  void runCritTest()
  {
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map, "test-files/conflate/unified/AllDataTypesA.osm", false, Status::Unknown1);

    WayGeneralizeVisitor uut;
    uut.setEpsilon(0.1);
    uut.setRemoveNodesSharedByWays(true);
    uut.addCriterion(std::shared_ptr<HighwayCriterion>(new HighwayCriterion()));
    map->visitRw(uut);

    const QString outputFile = _outputPath + "runCritTest.osm";
    OsmMapWriterFactory::write(map, outputFile);

    HOOT_FILE_EQUALS(_inputPath + "runCritTest.osm", outputFile);
  }

  void runRemoveSharedNodeTest()
  {
    // It would probably be better to test this from RdpWayGeneralizerTest.

    WayGeneralizeVisitor uut;
    uut.setEpsilon(1.0);

    uut.setRemoveNodesSharedByWays(true);
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(map, _inputPath + "runRemoveSharedNodeTest.osm");
    map->visitRw(uut);

    // In this output, you'll see a shared node between the bottom two buildings that got
    // simplified out and which disrupts the geometries.
    QString outputFile = _outputPath + "runRemoveSharedNodeTest-removed.osm";
    OsmMapWriterFactory::write(map, outputFile);
    HOOT_FILE_EQUALS(_inputPath + "runRemoveSharedNodeTest-removed.osm", outputFile);

    uut.setRemoveNodesSharedByWays(false);
    map.reset(new OsmMap());
    OsmMapReaderFactory::read(map, _inputPath + "runRemoveSharedNodeTest.osm");
    map->visitRw(uut);

    // In this output, the same node previously in question will not be removed. However, you will
    // see a node removed in the bottom-most building that distorts its geometry...but its not a
    // shared node.
    outputFile = _outputPath + "runRemoveSharedNodeTest-not-removed.osm";
    OsmMapWriterFactory::write(map, outputFile);
    HOOT_FILE_EQUALS(_inputPath + "runRemoveSharedNodeTest-not-removed.osm", outputFile);
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(WayGeneralizeVisitorTest, "quick");

}
