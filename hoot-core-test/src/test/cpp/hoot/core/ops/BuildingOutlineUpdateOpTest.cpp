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
 * @copyright Copyright (C) 2013, 2014, 2015, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/ops/BuildingOutlineUpdateOp.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/elements/MapProjector.h>

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

namespace hoot
{

class BuildingOutlineUpdateOpTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(BuildingOutlineUpdateOpTest);
  CPPUNIT_TEST(runSelfIntersectingRelationTest);
  CPPUNIT_TEST(runUncleanableToplogyTest);
  CPPUNIT_TEST_SUITE_END();

public:

  BuildingOutlineUpdateOpTest()
    : HootTestFixture("test-files/ops/BuildingOutlineUpdateOp/",
                      "test-output/ops/BuildingOutlineUpdateOp/")
  {
    setResetType(ResetBasic);
  }

  void runSelfIntersectingRelationTest()
  {
    DisableLog dl;

    OsmXmlReader reader;

    OsmMapPtr map(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.read(_inputPath + "SelfIntersectingRelationsIn.osm", map);

    BuildingOutlineUpdateOp uut;
    uut.apply(map);

    MapProjector::projectToWgs84(map);

    // This output includes two reviews instead of the expected 1 review. See ticket #7043 for
    // an idea to clean this up (need to port issue to github).
    OsmXmlWriter writer;
    writer.write(map, _outputPath + "SelfIntersectingRelationsOut.osm");
    HOOT_FILE_EQUALS(_inputPath + "SelfIntersectingRelationsOut.osm",
                     _outputPath + "SelfIntersectingRelationsOut.osm");
  }

  // see https://github.com/ngageoint/hootenanny/issues/442
  //
  // This test is only testing the uncleanable relations right now.  We may still need to test for
  // uncleanable ways too.
  void runUncleanableToplogyTest()
  {
    DisableLog dl;

    OsmXmlReader reader;

    OsmMapPtr map(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.read(_inputPath + "UncleanableTopologiesIn.osm", map);

    BuildingOutlineUpdateOp uut;
    uut.apply(map);

    MapProjector::projectToWgs84(map);

    OsmXmlWriter writer;
    writer.write(map, _outputPath + "UncleanableTopologiesOut.osm");
    HOOT_FILE_EQUALS(_inputPath + "UncleanableTopologiesOut.osm",
                     _outputPath + "UncleanableTopologiesOut.osm");
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(BuildingOutlineUpdateOpTest, "quick");

}
