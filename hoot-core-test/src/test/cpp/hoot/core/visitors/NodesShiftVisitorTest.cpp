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
 * @copyright Copyright (C) 2023 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/visitors/NodesShiftVisitor.h>

namespace hoot
{

class NodesShiftVisitorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(NodesShiftVisitorTest);
  CPPUNIT_TEST(singleNodeApplyTest);
  CPPUNIT_TEST(applyToMapTest);
  CPPUNIT_TEST_SUITE_END();

public:

  NodesShiftVisitorTest()
    : HootTestFixture("test-files/visitors/NodesShiftVisitorTest/",
                      "test-output/visitors/NodesShiftVisitorTest/")
  {
  }

  void singleNodeApplyTest()
  {
    std::shared_ptr<Node> n = std::make_shared<Node>(Status::Unknown1, 1, 0.0, 0.0);
    //  Positive moves
    NodesShiftVisitor visitor(10.0, 12.0);
    visitor.visit(n);
    CPPUNIT_ASSERT_EQUAL(12.0, n->getX());
    CPPUNIT_ASSERT_EQUAL(10.0, n->getY());
    //  Negative moves
    visitor.setShift(-2.0, -100.0);
    visitor.visit(n);
    CPPUNIT_ASSERT_EQUAL(-88.0, n->getX());
    CPPUNIT_ASSERT_EQUAL(8.0, n->getY());
  }

  void applyToMapTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    OsmMapReaderFactory::read(map, "test-files/ToyTestA.osm", false, Status::Unknown1);
    MapProjector::projectToPlanar(map);

    conf().set(ConfigOptions::getShiftNodesXyKey(), "10.0,20.0");

    NodesShiftVisitor uut;
    map->visitRw(uut);

    MapProjector::projectToWgs84(map);
    OsmMapWriterFactory::write(map, _outputPath + "applyToMapOutput1.osm");

    HOOT_FILE_EQUALS(_inputPath + "applyToMapExpected1.osm",
                     _outputPath + "applyToMapOutput1.osm");

    map->clear();
    OsmMapReaderFactory::read(map, "test-files/ToyTestA.osm", false, Status::Unknown1);
    MapProjector::projectToPlanar(map);

    uut.setShift(-100.0, -55.0);

    map->visitRw(uut);

    MapProjector::projectToWgs84(map);
    OsmMapWriterFactory::write(map, _outputPath + "applyToMapOutput2.osm");

    HOOT_FILE_EQUALS(_inputPath + "applyToMapExpected2.osm",
                     _outputPath + "applyToMapOutput2.osm");

    conf().set(ConfigOptions::getShiftNodesXyKey(), ConfigOptions::getShiftNodesXyDefaultValue());
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(NodesShiftVisitorTest, "quick");

}
