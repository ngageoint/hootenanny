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
 * @copyright Copyright (C) 2013, 2014, 2015, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/ops/BuildingPartMergeOp.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/elements/MapProjector.h>

// geos
#include <geos/io/WKTReader.h>
#include <geos/geom/Point.h>

// TGS
#include <tgs/Statistics/Random.h>
using namespace Tgs;

namespace hoot
{

class BuildingPartMergeOpTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(BuildingPartMergeOpTest);
  CPPUNIT_TEST(runToyTest);
  CPPUNIT_TEST(runToyMultithreadTest);
  CPPUNIT_TEST(runPreserveTypesTest);
  CPPUNIT_TEST_SUITE_END();

public:

  BuildingPartMergeOpTest() :
  HootTestFixture("test-files/ops/BuildingPartMergeOp/", "test-output/ops/BuildingPartMergeOp/")
  {
    setResetType(ResetEnvironment);
  }

  void runToyTest()
  {
    OsmXmlReader reader;
    OsmMapPtr map = std::make_shared<OsmMap>();
    reader.setDefaultStatus(Status::Unknown1);
    reader.read(_inputPath + "ToyBuildings.osm", map);

    BuildingPartMergeOp uut;
    uut.setThreadCount(1);
    uut.apply(map);

    MapProjector::projectToWgs84(map);
    OsmXmlWriter writer;
    writer.write(map, _outputPath + "runToyTestOut.osm");

    CPPUNIT_ASSERT_EQUAL(15L, uut.getNumFeaturesAffected());
    CPPUNIT_ASSERT_EQUAL(14, uut.getTotalBuildingGroupsProcessed());
    CPPUNIT_ASSERT_EQUAL(6, uut.getNumBuildingGroupsMerged());
    HOOT_FILE_EQUALS(_inputPath + "runToyTestOut.osm", _outputPath + "runToyTestOut.osm");
  }

  void runToyMultithreadTest()
  {
    OsmXmlReader reader;
    OsmMapPtr map = std::make_shared<OsmMap>();
    reader.setDefaultStatus(Status::Unknown1);
    reader.read(_inputPath + "ToyBuildings.osm", map);

    BuildingPartMergeOp uut;
    uut.setThreadCount(2);
    uut.apply(map);

    MapProjector::projectToWgs84(map);
    OsmXmlWriter writer;
    writer.write(map, _outputPath + "runToyMultithreadTestOut.osm");

    CPPUNIT_ASSERT_EQUAL(15L, uut.getNumFeaturesAffected());
    CPPUNIT_ASSERT_EQUAL(14, uut.getTotalBuildingGroupsProcessed());
    CPPUNIT_ASSERT_EQUAL(6, uut.getNumBuildingGroupsMerged());
    HOOT_FILE_EQUALS(
      _inputPath + "runToyTestOut.osm", _outputPath + "runToyMultithreadTestOut.osm");
  }

  void runPreserveTypesTest()
  {
    OsmXmlReader reader;
    OsmMapPtr map = std::make_shared<OsmMap>();
    reader.setDefaultStatus(Status::Unknown1);
    const QString inputPath =
      "test-files/cases/attribute/unifying/building/building-3136-many-to-many-auto-merge-1";
    reader.read(inputPath + "/Input1.osm", map);
    reader.setDefaultStatus(Status::Unknown2);
    reader.read(inputPath + "/Input2.osm", map);

    BuildingPartMergeOp uut;
    uut.setThreadCount(1);
    uut.setPreserveTypes(true);
    conf().set(ConfigOptions().getTagMergerTypesOverwriteReferenceKey(), true);
    uut.apply(map);

    MapProjector::projectToWgs84(map);
    OsmXmlWriter writer;
    writer.write(map, _outputPath + "runPreserveTypesTestOut.osm");

    CPPUNIT_ASSERT_EQUAL(5L, uut.getNumFeaturesAffected());
    CPPUNIT_ASSERT_EQUAL(1, uut.getTotalBuildingGroupsProcessed());
    CPPUNIT_ASSERT_EQUAL(1, uut.getNumBuildingGroupsMerged());
    HOOT_FILE_EQUALS(
      _inputPath + "runPreserveTypesTestOut.osm", _outputPath + "runPreserveTypesTestOut.osm");
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(BuildingPartMergeOpTest, "quick");

}
