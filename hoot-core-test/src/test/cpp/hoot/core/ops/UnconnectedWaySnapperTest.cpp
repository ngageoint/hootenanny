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
 * @copyright Copyright (C) 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/ops/UnconnectedWaySnapper.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/visitors/ElementIdsVisitor.h>

// CPP Unit
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>

namespace hoot
{

class UnconnectedWaySnapperTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(UnconnectedWaySnapperTest);
  CPPUNIT_TEST(runSnapTest);
  CPPUNIT_TEST(runConfigOptionsValidationTest);
  CPPUNIT_TEST(runStaticSnapTest);
  CPPUNIT_TEST_SUITE_END();

public:

  UnconnectedWaySnapperTest()
    : HootTestFixture("test-files/ops/UnconnectedWaySnapper/",
                      "test-output/ops/UnconnectedWaySnapper/")
  {
    setResetType(ResetAll);
  }

  void runSnapTest()
  {
    const QString testName = "runSnapTest";

    OsmXmlReader reader;
    OsmMapPtr map(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.read(_inputPath + testName + "In1.osm", map);
    reader.setDefaultStatus(Status::Unknown2);
    reader.read(_inputPath + testName + "In2.osm", map);

    UnconnectedWaySnapper uut;
    uut.setAddCeToSearchDistance(false);
    uut.setMaxNodeReuseDistance(0.5);
    uut.setMaxSnapDistance(5.0);
    uut.setMarkSnappedNodes(true);
    uut.setMarkSnappedWays(true);
    uut.setSnapToExistingWayNodes(true);
    uut.setWayDiscretizationSpacing(1.0);
    uut.setSnapToWayStatuses(QStringList(Status(Status::Unknown1).toString()));
    uut.setSnapWayStatuses(QStringList(Status(Status::Unknown2).toString()));
    uut.setWayNodeToSnapToCriterionClassName("hoot::HighwayNodeCriterion");
    uut.setWayToSnapCriterionClassName("hoot::HighwayCriterion");
    uut.setWayToSnapToCriterionClassName("hoot::HighwayCriterion");
    uut.apply(map);

    MapProjector::projectToWgs84(map);

    OsmXmlWriter writer;
    writer.setIsDebugMap(true);
    writer.write(map, _outputPath + testName +  + "Out.osm");

    CPPUNIT_ASSERT_EQUAL(43L, uut.getNumFeaturesAffected());
    CPPUNIT_ASSERT_EQUAL(5L, uut.getNumSnappedToWayNodes());
    CPPUNIT_ASSERT_EQUAL(38L, uut.getNumSnappedToWays());
    HOOT_FILE_EQUALS(_inputPath + testName +  + "Out.osm", _outputPath + testName +  + "Out.osm");
  }

  void runConfigOptionsValidationTest()
  {
    DisableLog dl;
    UnconnectedWaySnapper uut;
    QString exceptionMsg;

    try
    {
      uut.setMaxNodeReuseDistance(0.0);
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT(
      exceptionMsg.startsWith(
        "Invalid " + ConfigOptions::getSnapUnconnectedWaysExistingWayNodeToleranceKey() +
        " value:"));

    try
    {
      uut.setMaxSnapDistance(0.0);
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT(
      exceptionMsg.startsWith(
        "Invalid " + ConfigOptions::getSnapUnconnectedWaysSnapToleranceKey() + " value:"));

    try
    {
      uut.setWayDiscretizationSpacing(0.0);
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT(
      exceptionMsg.startsWith(
        "Invalid " + ConfigOptions::getSnapUnconnectedWaysDiscretizationSpacingKey() + " value:"));

    uut.setWayToSnapToCriterionClassName("");
    HOOT_STR_EQUALS("hoot::WayCriterion", uut._wayToSnapToCriterionClassName);

    uut.setWayToSnapCriterionClassName(" ");
    HOOT_STR_EQUALS("hoot::WayCriterion", uut._wayToSnapCriterionClassName);

    uut.setWayNodeToSnapToCriterionClassName(" ");
    HOOT_STR_EQUALS("hoot::WayNodeCriterion", uut._wayNodeToSnapToCriterionClassName);
  }

  void runStaticSnapTest()
  {
    const QString testName = "runStaticSnapTest";

    OsmXmlReader reader;
    OsmMapPtr map(new OsmMap());
    reader.setUseDataSourceIds(true);
    reader.setDefaultStatus(Status::Unknown1);
    reader.read(_inputPath + testName + "In1.osm", map);
    reader.setDefaultStatus(Status::Unknown2);
    reader.read(_inputPath + testName + "In2.osm", map);

    MapProjector::projectToPlanar(map);

    WayPtr way1 =
      map->getWay(ElementIdsVisitor::findElementsByTag(map, ElementType::Way, "note", "way1")[0]);
    WayPtr way2 =
      map->getWay(ElementIdsVisitor::findElementsByTag(map, ElementType::Way, "note", "way2")[0]);
    WayPtr way3 =
      map->getWay(ElementIdsVisitor::findElementsByTag(map, ElementType::Way, "note", "way3")[0]);

    UnconnectedWaySnapper::snapClosestEndpointToWay(map, way2, way1);
    UnconnectedWaySnapper::snapClosestEndpointToWay(map, way3, way1);

    MapProjector::projectToWgs84(map);
    OsmXmlWriter writer;
    writer.setIsDebugMap(true);
    writer.write(map, _outputPath + testName +  + "Out.osm");

    HOOT_FILE_EQUALS(_inputPath + testName +  + "Out.osm", _outputPath + testName +  + "Out.osm");
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(UnconnectedWaySnapperTest, "quick");

}
