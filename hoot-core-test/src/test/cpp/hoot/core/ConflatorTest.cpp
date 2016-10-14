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
 * @copyright Copyright (C) 2012, 2013, 2014 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/Conflator.h>
#include <hoot/core/MapProjector.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/elements/Element.h>
#include <hoot/core/io/PbfReader.h>
#include <hoot/core/io/OsmReader.h>
#include <hoot/core/io/OsmWriter.h>
using namespace hoot;


// Boost
using namespace boost;

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Qt
#include <QDebug>

class TDistributionTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(TDistributionTest);
  CPPUNIT_TEST(runPbfTest);
  CPPUNIT_TEST(runTest);
  CPPUNIT_TEST(runMergeTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runPbfTest()
  {
    PbfReader reader(true);

    shared_ptr<OsmMap> map(new OsmMap());
    reader.setUseFileStatus(true);
    reader.read("test-files/ToyTestCombined.pbf", map);

    Conflator uut;
    uut.loadSource(map);
    uut.conflate();

    shared_ptr<OsmMap> out(new OsmMap(uut.getBestMap()));
    MapProjector::projectToWgs84(out);

    CPPUNIT_ASSERT_EQUAL((size_t)15, out->getWays().size());

    OsmWriter writer;
    writer.setIncludeIds(true);
    writer.write(out, "test-output/ConflatorPbfTest.osm");
  }

  void runTest()
  {
    OsmReader reader;

    shared_ptr<OsmMap> map(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.read("test-files/ToyTestA.osm", map);
    reader.setDefaultStatus(Status::Unknown2);
    reader.read("test-files/ToyTestB.osm", map);

    Conflator uut;
    uut.loadSource(map);
    uut.conflate();

    shared_ptr<OsmMap> out(new OsmMap(uut.getBestMap()));
    MapProjector::projectToWgs84(out);

    CPPUNIT_ASSERT_EQUAL((size_t)9, out->getWays().size());

    OsmWriter writer;
    writer.setIncludeIds(true);
    writer.write(out, "test-output/ConflatorTest.osm");
  }

  //This test shows the fix for ticket #249.
  //Now the river/building never get mergeed together.
  void runMergeTest()
  {
    OsmReader reader;

    shared_ptr<OsmMap> map(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.read("test-files/building_Valqueire.osm", map);
    reader.setDefaultStatus(Status::Unknown2);
    reader.read("test-files/river_Valqueire.osm", map);

    Conflator uut;
    uut.loadSource(map);
    uut.conflate();

    shared_ptr<OsmMap> out(new OsmMap(uut.getBestMap()));
    MapProjector::projectToWgs84(out);

    CPPUNIT_ASSERT_EQUAL((size_t)2, out->getNodeMap().size());
    CPPUNIT_ASSERT_EQUAL((size_t)0, out->getRelationMap().size());

    OsmWriter writer;
    writer.setIncludeIds(true);
    writer.write(out, "test-output/RiverBuildingConflatorTest.osm");
  }
};

//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ConflatorTest, "current");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(TDistributionTest, "quick");

