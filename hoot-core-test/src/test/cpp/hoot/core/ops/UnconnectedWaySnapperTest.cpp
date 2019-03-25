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
 * @copyright Copyright (C) 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/ops/UnconnectedWaySnapper.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/criterion/StatusCriterion.h>
#include <hoot/core/visitors/RemoveElementsVisitor.h>

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

namespace hoot
{

class UnconnectedWaySnapperTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(UnconnectedWaySnapperTest);
  CPPUNIT_TEST(runSnapTest);
  CPPUNIT_TEST(runConfigOptionsValidationTest);
  CPPUNIT_TEST_SUITE_END();

public:

  const QString inputPath = "test-files/ops/UnconnectedWaySnapper/";
  const QString outputPath = "test-output/ops/UnconnectedWaySnapper/";

  UnconnectedWaySnapperTest()
  {
    setResetType(ResetBasic);
    TestUtils::mkpath(outputPath);
  }

  void runSnapTest()
  {
    const QString testName = "runSnapTest";

    OsmXmlReader reader;
    OsmMapPtr map(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.read(inputPath + testName + "In1.osm", map);
    reader.setDefaultStatus(Status::Unknown2);
    reader.read(inputPath + testName + "In2.osm", map);

    UnconnectedWaySnapper uut;
    uut.setAddCeToSearchDistance(false);
    uut.setMaxNodeReuseDistance(0.5);
    uut.setMaxSnapDistance(5.0);
    uut.setMarkSnappedNodes(true);
    uut.setSnapToExistingWayNodes(true);
    uut.setWayDiscretizationSpacing(1.0);
    uut.setSnapToWayStatus(Status::Unknown1);
    uut.setSnapWayStatus(Status::Unknown2);
    uut.setWayNodeToSnapToCriterionClassName("hoot::HighwayNodeCriterion");
    uut.setWayToSnapCriterionClassName("hoot::HighwayCriterion");
    uut.setWayToSnapToCriterionClassName("hoot::HighwayCriterion");
    uut.apply(map);

    // Remove the ref data, so its easier to compare the snapped output to the pre-snapped
    // secondary input data.
    ElementCriterionPtr statusCrit(new StatusCriterion(Status::Unknown1));
    RemoveElementsVisitor removeRefVisitor(statusCrit);
    removeRefVisitor.setRecursive(true);
    map->visitRw(removeRefVisitor);

    MapProjector::projectToWgs84(map);
    OsmXmlWriter().write(map, outputPath + testName +  + "Out.osm");

    CPPUNIT_ASSERT_EQUAL(uut.getNumAffected(), 41L);
    CPPUNIT_ASSERT_EQUAL(uut.getNumSnappedToWayNodes(), 5L);
    CPPUNIT_ASSERT_EQUAL(uut.getNumSnappedToWays(), 36L);
    HOOT_FILE_EQUALS(inputPath + testName +  + "Out.osm", outputPath + testName +  + "Out.osm");
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
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(UnconnectedWaySnapperTest, "quick");

}
