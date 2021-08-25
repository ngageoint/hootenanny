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
#include <hoot/core/criterion/LinearCriterion.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/ops/UnconnectedWaySnapper.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/visitors/ElementIdsVisitor.h>

namespace hoot
{

class UnconnectedWaySnapperTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(UnconnectedWaySnapperTest);
  CPPUNIT_TEST(runSnapTest);
  CPPUNIT_TEST(runConfigOptionsValidationTest);
  CPPUNIT_TEST(runStaticSnapTest);
  CPPUNIT_TEST(runReviewSnappedTest);
  CPPUNIT_TEST(runMarkOnlyTest);
  CPPUNIT_TEST(runTypeMatchTest);
  CPPUNIT_TEST(runTypeExcludeTest);
  CPPUNIT_TEST(runSnapMultipleTypesTest);
  CPPUNIT_TEST(runSnapOneOfMultipleTypesTest);
  CPPUNIT_TEST(runSnapToDifferentTypeTest);
  CPPUNIT_TEST_SUITE_END();

public:

  UnconnectedWaySnapperTest() :
  HootTestFixture("test-files/ops/UnconnectedWaySnapper/", "test-output/ops/UnconnectedWaySnapper/")
  {
    setResetType(ResetAllNoMatchFactory);
  }

  void runSnapTest()
  {
    const QString testName = "runSnapTest";

    OsmXmlReader reader;
    OsmMapPtr map = std::make_shared<OsmMap>();
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
    uut.setWayToSnapCriteria(QStringList("HighwayCriterion"));
    uut.setWayToSnapToCriteria(QStringList("HighwayCriterion"));
    uut.setMarkOnly(false);
    uut.setReviewSnappedWays(false);
    uut.setTypeExcludeKvps(ConfigOptions().getSnapUnconnectedWaysExcludeTypes());
    uut.apply(map);

    MapProjector::projectToWgs84(map);

    OsmXmlWriter writer;
    writer.setIsDebugMap(true);
    writer.write(map, _outputPath + testName +  + "Out.osm");

    CPPUNIT_ASSERT_EQUAL(42L, uut.getNumFeaturesAffected());
    CPPUNIT_ASSERT_EQUAL(5L, uut.getNumSnappedToWayNodes());
    CPPUNIT_ASSERT_EQUAL(37L, uut.getNumSnappedToWays());
    HOOT_FILE_EQUALS(_inputPath + testName +  + "Out.osm", _outputPath + testName +  + "Out.osm");
  }

  void runSnapMultipleTypesTest()
  {
    // test against very simple data to ensure more than one type of data gets snapped

    const QString testName = "runSnapMultipleTypesTest";

    OsmMapPtr map = std::make_shared<OsmMap>();
    OsmMapReaderFactory::read(map, false, true, _inputPath + testName + "In.osm");

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
    QStringList types("HighwayCriterion");
    types.append("RailwayCriterion");
    uut.setWayToSnapCriteria(types);
    uut.setWayToSnapToCriteria(types);
    uut.setMarkOnly(false);
    uut.setReviewSnappedWays(false);
    uut.setTypeExcludeKvps(ConfigOptions().getSnapUnconnectedWaysExcludeTypes());
    uut.apply(map);

    MapProjector::projectToWgs84(map);

    OsmMapWriterFactory::write(map, _outputPath + testName +  + "Out.osm", true, true);

    CPPUNIT_ASSERT_EQUAL(2L, uut.getNumFeaturesAffected());
    CPPUNIT_ASSERT_EQUAL(2L, uut.getNumSnappedToWayNodes());
    CPPUNIT_ASSERT_EQUAL(0L, uut.getNumSnappedToWays());
    HOOT_FILE_EQUALS(_inputPath + testName +  + "Out.osm", _outputPath + testName +  + "Out.osm");
  }

  void runSnapOneOfMultipleTypesTest()
  {
    // test against very simple data to ensure that when data has multiple types, only the types
    // specified get snapped

    const QString testName = "runSnapOneOfMultipleTypesTest";

    OsmMapPtr map = std::make_shared<OsmMap>();
    OsmMapReaderFactory::read(map, false, true, _inputPath + "runSnapMultipleTypesTestIn.osm");

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
    uut.setWayToSnapCriteria(QStringList("RailwayCriterion"));
    uut.setWayToSnapToCriteria(QStringList("RailwayCriterion"));
    uut.setMarkOnly(false);
    uut.setReviewSnappedWays(false);
    uut.setTypeExcludeKvps(ConfigOptions().getSnapUnconnectedWaysExcludeTypes());
    uut.apply(map);

    MapProjector::projectToWgs84(map);

    OsmMapWriterFactory::write(map, _outputPath + testName +  + "Out.osm", true, true);

    CPPUNIT_ASSERT_EQUAL(1L, uut.getNumFeaturesAffected());
    CPPUNIT_ASSERT_EQUAL(1L, uut.getNumSnappedToWayNodes());
    CPPUNIT_ASSERT_EQUAL(0L, uut.getNumSnappedToWays());
    HOOT_FILE_EQUALS(_inputPath + testName +  + "Out.osm", _outputPath + testName +  + "Out.osm");
  }

  void runSnapToDifferentTypeTest()
  {
    // This tests that different types of features may be snapped together. This isn't commonly
    // done, but in some cases you could have untyped data that needs to be snapped to typed data.

    const QString testName = "runSnapToDifferentTypeTest";

    OsmMapPtr map = std::make_shared<OsmMap>();
    OsmMapReaderFactory::read(map, false, true, _inputPath + testName + "In.osm");

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
    // Setting this to empty allows untyped sec ways to be snapped back to the ref.
    uut.setWayToSnapCriteria(QStringList());
    QStringList snapToTypes("HighwayCriterion");
    snapToTypes.append("RailwayCriterion");
    uut.setWayToSnapToCriteria(snapToTypes);
    uut.setMarkOnly(false);
    uut.setReviewSnappedWays(false);
    uut.setTypeExcludeKvps(ConfigOptions().getSnapUnconnectedWaysExcludeTypes());
    uut.apply(map);

    MapProjector::projectToWgs84(map);

    OsmMapWriterFactory::write(map, _outputPath + testName +  + "Out.osm", true, true);

    CPPUNIT_ASSERT_EQUAL(2L, uut.getNumFeaturesAffected());
    CPPUNIT_ASSERT_EQUAL(2L, uut.getNumSnappedToWayNodes());
    CPPUNIT_ASSERT_EQUAL(0L, uut.getNumSnappedToWays());
    HOOT_FILE_EQUALS(_inputPath + testName +  + "Out.osm", _outputPath + testName +  + "Out.osm");
  }

  void runReviewSnappedTest()
  {
    const QString testName = "runReviewSnappedTest";

    OsmXmlReader reader;
    OsmMapPtr map = std::make_shared<OsmMap>();
    reader.setDefaultStatus(Status::Unknown1);
    reader.read(_inputPath + "runSnapTestIn1.osm", map);
    reader.setDefaultStatus(Status::Unknown2);
    reader.read(_inputPath + "runSnapTestIn2.osm", map);

    UnconnectedWaySnapper uut;
    uut.setAddCeToSearchDistance(false);
    uut.setMaxNodeReuseDistance(0.5);
    uut.setMaxSnapDistance(5.0);
    uut.setMarkSnappedNodes(false);
    uut.setMarkSnappedWays(false);
    uut.setSnapToExistingWayNodes(true);
    uut.setWayDiscretizationSpacing(1.0);
    uut.setSnapToWayStatuses(QStringList(Status(Status::Unknown1).toString()));
    uut.setSnapWayStatuses(QStringList(Status(Status::Unknown2).toString()));
    uut.setWayToSnapCriteria(QStringList("HighwayCriterion"));
    uut.setWayToSnapToCriteria(QStringList("HighwayCriterion"));
    uut.setMarkOnly(false);
    uut.setReviewSnappedWays(true);
    uut.setTypeExcludeKvps(ConfigOptions().getSnapUnconnectedWaysExcludeTypes());
    uut.apply(map);

    MapProjector::projectToWgs84(map);

    OsmXmlWriter writer;
    writer.setIsDebugMap(true);
    writer.write(map, _outputPath + testName +  + "Out.osm");

    CPPUNIT_ASSERT_EQUAL(42L, uut.getNumFeaturesAffected());
    CPPUNIT_ASSERT_EQUAL(5L, uut.getNumSnappedToWayNodes());
    CPPUNIT_ASSERT_EQUAL(37L, uut.getNumSnappedToWays());
    HOOT_FILE_EQUALS(_inputPath + testName +  + "Out.osm", _outputPath + testName +  + "Out.osm");
  }

  void runMarkOnlyTest()
  {
    const QString testName = "runMarkOnlyTest";

    OsmXmlReader reader;
    OsmMapPtr map = std::make_shared<OsmMap>();
    reader.setDefaultStatus(Status::Unknown1);
    reader.read(_inputPath + "runSnapTestIn1.osm", map);
    reader.setDefaultStatus(Status::Unknown2);
    reader.read(_inputPath + "runSnapTestIn2.osm", map);

    UnconnectedWaySnapper uut;
    uut.setAddCeToSearchDistance(false);
    uut.setMaxNodeReuseDistance(0.5);
    uut.setMaxSnapDistance(5.0);
    uut.setMarkSnappedNodes(false);
    uut.setMarkSnappedWays(false);
    uut.setSnapToExistingWayNodes(true);
    uut.setWayDiscretizationSpacing(1.0);
    uut.setSnapToWayStatuses(QStringList(Status(Status::Unknown1).toString()));
    uut.setSnapWayStatuses(QStringList(Status(Status::Unknown2).toString()));
    uut.setWayToSnapCriteria(QStringList("HighwayCriterion"));
    uut.setWayToSnapToCriteria(QStringList("HighwayCriterion"));
    uut.setMarkOnly(true);
    uut.setReviewSnappedWays(true);
    uut.setTypeExcludeKvps(ConfigOptions().getSnapUnconnectedWaysExcludeTypes());
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

    uut.setWayToSnapToCriteria(QStringList(""));
    CPPUNIT_ASSERT_EQUAL(1, uut._wayToSnapToCriteria.size());
    HOOT_STR_EQUALS(LinearCriterion::className(), uut._wayToSnapToCriteria.at(0));

    uut.setWayToSnapCriteria(QStringList(" "));
    CPPUNIT_ASSERT_EQUAL(1, uut._wayToSnapCriteria.size());
    HOOT_STR_EQUALS(LinearCriterion::className(), uut._wayToSnapCriteria.at(0));
  }

  void runStaticSnapTest()
  {
    const QString testName = "runStaticSnapTest";

    OsmXmlReader reader;
    OsmMapPtr map = std::make_shared<OsmMap>();
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

    UnconnectedWaySnapper::snapClosestWayEndpointToWay(map, way2, way1);
    UnconnectedWaySnapper::snapClosestWayEndpointToWay(map, way3, way1);

    MapProjector::projectToWgs84(map);
    OsmXmlWriter writer;
    writer.setIsDebugMap(true);
    writer.write(map, _outputPath + testName +  + "Out.osm");

    HOOT_FILE_EQUALS(_inputPath + testName +  + "Out.osm", _outputPath + testName +  + "Out.osm");
  }

  void runTypeMatchTest()
  {
    // Specifically, this tests that road_marking=solid_stop_line features don't get snapped to
    // roads.

    const QString testName = "runTypeMatch";

    OsmXmlReader reader;
    OsmMapPtr map = std::make_shared<OsmMap>();
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
    // Not setting a type will default this to snapping all ways.
    uut.setMarkOnly(false);
    uut.setReviewSnappedWays(false);
    // By default, way type comparison are turned off and only the snap criteria influence what
    // may be snapped together (score == -1.0). This adds the requirement that the two ways being
    // snapped together must have at a minimum a type similarity as defined by the schema.
    uut.setMinTypeMatchScore(0.8);
    uut.setTypeExcludeKvps(ConfigOptions().getSnapUnconnectedWaysExcludeTypes());
    uut.apply(map);

    MapProjector::projectToWgs84(map);

    OsmXmlWriter writer;
    writer.setIsDebugMap(true);
    writer.write(map, _outputPath + testName +  + "Out.osm");

    CPPUNIT_ASSERT_EQUAL(117L, uut.getNumFeaturesAffected());
    CPPUNIT_ASSERT_EQUAL(114L, uut.getNumSnappedToWayNodes());
    CPPUNIT_ASSERT_EQUAL(3L, uut.getNumSnappedToWays());
    HOOT_FILE_EQUALS(_inputPath + testName +  + "Out.osm", _outputPath + testName +  + "Out.osm");
  }

  void runTypeExcludeTest()
  {
    // Specifically, this tests that road_marking=solid_stop_line features don't get snapped to
    // anything.

    const QString testName = "runTypeExcludeTest";

    OsmXmlReader reader;
    OsmMapPtr map = std::make_shared<OsmMap>();
    reader.setDefaultStatus(Status::Unknown1);
    reader.read(_inputPath + "runTypeMatchIn1.osm", map);
    reader.setDefaultStatus(Status::Unknown2);
    reader.read(_inputPath + "runTypeMatchIn2.osm", map);

    UnconnectedWaySnapper uut;
    uut.setAddCeToSearchDistance(false);
    uut.setMaxNodeReuseDistance(0.5);
    // Lengthen the snap distance to the min that would make some of the stop lines snap to each
    // other.
    uut.setMaxSnapDistance(15.0);
    uut.setMarkSnappedNodes(true);
    uut.setMarkSnappedWays(true);
    uut.setSnapToExistingWayNodes(true);
    uut.setWayDiscretizationSpacing(1.0);
    uut.setSnapToWayStatuses(QStringList(Status(Status::Unknown1).toString()));
    uut.setSnapWayStatuses(QStringList(Status(Status::Unknown2).toString()));
    // Not setting a type will default this to snapping all ways.
    uut.setMarkOnly(false);
    uut.setReviewSnappedWays(false);
    // Ensure that road_marking=solid_stop_line's can't be snapped at all.
    uut.setTypeExcludeKvps(ConfigOptions().getSnapUnconnectedWaysExcludeTypes());
    QStringList typeExcludeKvps = ConfigOptions().getSnapUnconnectedWaysExcludeTypes();
    typeExcludeKvps.append("road_marking=solid_stop_line");
    uut.setTypeExcludeKvps(typeExcludeKvps);
    uut.apply(map);

    MapProjector::projectToWgs84(map);

    OsmXmlWriter writer;
    writer.setIsDebugMap(true);
    writer.write(map, _outputPath + testName +  + "Out.osm");

    CPPUNIT_ASSERT_EQUAL(131L, uut.getNumFeaturesAffected());
    CPPUNIT_ASSERT_EQUAL(125L, uut.getNumSnappedToWayNodes());
    CPPUNIT_ASSERT_EQUAL(6L, uut.getNumSnappedToWays());
    HOOT_FILE_EQUALS(_inputPath + testName +  + "Out.osm", _outputPath + testName +  + "Out.osm");
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(UnconnectedWaySnapperTest, "quick");

}
