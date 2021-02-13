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
 * @copyright Copyright (C) 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/ops/RelationWithMostMembersOp.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/elements/MapProjector.h>

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

namespace hoot
{

class RelationWithMostMembersOpTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(RelationWithMostMembersOpTest);
  CPPUNIT_TEST(runBasicTest);
  CPPUNIT_TEST_SUITE_END();

public:

  const QString inputPath = "test-files/ops/RelationWithMostMembersOp/";
  const QString outputPath = "test-output/ops/RelationWithMostMembersOp/";

  RelationWithMostMembersOpTest()
  {
    setResetType(ResetBasic);
    FileUtils::makeDir(outputPath);
  }

  void runBasicTest()
  {
    OsmXmlReader reader;
    OsmMapPtr map(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.read("test-files/criterion/ComplexBuildings.osm", map);

    RelationWithMostMembersOp uut;
    uut.setCriterion("hoot::BuildingCriterion", "relation", conf());
    uut.setCriterion("hoot::BuildingCriterion", "member", conf());
    uut.apply(map);

    MapProjector::projectToWgs84(map);
    OsmXmlWriter writer;
    writer.write(map, outputPath + "RelationWithMostMembersOpOut.osm");
    HOOT_FILE_EQUALS(inputPath + "RelationWithMostMembersOpOut.osm",
                     outputPath + "RelationWithMostMembersOpOut.osm");
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(RelationWithMostMembersOpTest, "quick");

}
