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
 * @copyright Copyright (C) 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/conflate/merging/CollectionRelationMerger.h>

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

namespace hoot
{

class CollectionRelationMergerTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(CollectionRelationMergerTest);
  CPPUNIT_TEST(runTest);
  CPPUNIT_TEST_SUITE_END();

public:

  AdminBoundsRelationMergerTest() :
  HootTestFixture(
    "test-files/conflate/merging/AdminBoundsRelationMergerTest/",
    "test-output/conflate/merging/AdminBoundsRelationMergerTest/")
  {
  }

  void runTest()
  {
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(map, _inputPath + "runTestInput.osm", true);

    CollectionRelationMerger uut;
    uut.setOsmMap(map.get());
    uut.merge(ElementId(ElementType::Relation, 7387470), ElementId(ElementType::Relation, -1));

    MapProjector::projectToWgs84(map);
    OsmMapWriterFactory::write(map, _outputPath + "runTestOutput.osm");

    HOOT_FILE_EQUALS(_inputPath + "runTestOutput.osm", _outputPath + "runTestOutput.osm");
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(CollectionRelationMergerTest, "quick");

}
