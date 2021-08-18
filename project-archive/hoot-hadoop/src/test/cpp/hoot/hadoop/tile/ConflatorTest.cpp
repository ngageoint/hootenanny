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
 * @copyright Copyright (C) 2012, 2013, 2014, 2018 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/conflate/Conflator.h>
#include <hoot/core/elements/Element.h>
#include <hoot/core/io/OsmPbfReader.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/util/MapProjector.h>

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

namespace hoot
{

class ConflatorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(ConflatorTest);
  CPPUNIT_TEST(runPbfTest);
  CPPUNIT_TEST(runTest);
  CPPUNIT_TEST(runMergeTest);
  CPPUNIT_TEST_SUITE_END();

public:

  ConflatorTest()
  {
    TestUtils::mkpath("test-output");
  }

  void runPbfTest()
  {
    OsmPbfReader reader(true);

    OsmMapPtr map(new OsmMap());
    reader.setUseFileStatus(true);
    reader.read("test-files/ToyTestCombined.osm.pbf", map);

    Conflator uut;
    uut.loadSource(map);
    uut.conflate();

    OsmMapPtr out(new OsmMap(uut.getBestMap()));
    MapProjector::projectToWgs84(out);

    OsmXmlWriter writer;
    writer.setIncludeIds(true);
    writer.write(out, "test-output/ConflatorPbfTest.osm");

    CPPUNIT_ASSERT_EQUAL((size_t)15, out->getWays().size());
  }

  void runTest()
  {
    OsmXmlReader reader;

    OsmMapPtr map(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.read("test-files/ToyTestA.osm", map);
    reader.setDefaultStatus(Status::Unknown2);
    reader.read("test-files/ToyTestB.osm", map);

    Conflator uut;
    uut.loadSource(map);
    uut.conflate();

    OsmMapPtr out(new OsmMap(uut.getBestMap()));
    MapProjector::projectToWgs84(out);

    OsmXmlWriter writer;
    writer.setIncludeIds(true);
    writer.write(out, "test-output/ConflatorTest.osm");

    CPPUNIT_ASSERT_EQUAL((size_t)9, out->getWays().size());
  }

  //This test shows the fix for ticket #249.
  //Now the river/building never get merged together.
  void runMergeTest()
  {
    OsmXmlReader reader;

    OsmMapPtr map(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.read("test-files/building_Valqueire.osm", map);
    reader.setDefaultStatus(Status::Unknown2);
    reader.read("test-files/river_Valqueire.osm", map);

    Conflator uut;
    uut.loadSource(map);
    uut.conflate();

    OsmMapPtr out(new OsmMap(uut.getBestMap()));
    MapProjector::projectToWgs84(out);

    OsmXmlWriter writer;
    writer.setIncludeIds(true);
    writer.write(out, "test-output/RiverBuildingConflatorTest.osm");

    CPPUNIT_ASSERT_EQUAL((size_t)2, out->getNodes().size());
    CPPUNIT_ASSERT_EQUAL((size_t)0, out->getRelations().size());
  }
};

//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ConflatorTest, "current");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ConflatorTest, "quick");

}
