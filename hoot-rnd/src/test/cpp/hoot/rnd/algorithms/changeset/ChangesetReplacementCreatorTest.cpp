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
 * @copyright Copyright (C) 2012, 2013, 2014, 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/algorithms/changeset/ChangesetReplacementCreator.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/criterion/HighwayCriterion.h>
#include <hoot/core/criterion/BuildingCriterion.h>
#include <hoot/core/criterion/PoiCriterion.h>
#include <hoot/core/io/DataConverter.h>
#include <hoot/core/util/IoUtils.h>
#include <hoot/core/visitors/SetTagValueVisitor.h>
#include <hoot/rnd/perty/PertyOp.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/util/GeometryUtils.h>

namespace hoot
{

/*
 * These tests are very similar to the tests in Service*ReplacementTest.sh. The main differences are
 * that these tests don't interact with a database and don't try to apply the output changeset.
 *
 * This test file is in hoot-rnd since it needs to use PertyOp.
 */
class ChangesetReplacementCreatorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(ChangesetReplacementCreatorTest);
  // We're already testing API DB inputs with command tests, so skipping those here.
  CPPUNIT_TEST(runPolyLenientOsmTest);  // passing
  CPPUNIT_TEST(runPolyStrictOsmTest);   // passing
  CPPUNIT_TEST(runPoiStrictOsmTest);
  CPPUNIT_TEST(runLinearLenientOsmTest);
  CPPUNIT_TEST(runLinearStrictOsmTest);
//  CPPUNIT_TEST(runPolyLenientJsonTest);
//  CPPUNIT_TEST(runPolyStrictJsonTest);
//  CPPUNIT_TEST(runPoiStrictJsonTest);
//  CPPUNIT_TEST(runLinearLenientJsonTest);
//  CPPUNIT_TEST(runLinearStrictJsonTest);
  CPPUNIT_TEST_SUITE_END();

public:

  ChangesetReplacementCreatorTest() :
  HootTestFixture(
    "test-files/rnd/algorithms/changeset/ChangesetReplacementCreatorTest/",
    "test-output/rnd/algorithms/changeset/ChangesetReplacementCreatorTest/"),
  _bounds(geos::geom::Envelope(-71.4698, -71.4657, 42.4866, 42.4902))
  {
    setResetType(ResetAll);

    conf().set(ConfigOptions::getUuidHelperRepeatableKey(), true);
    conf().set(ConfigOptions::getWriterIncludeDebugTagsKey(), true);
    conf().set(ConfigOptions::getReaderAddSourceDatetimeKey(), false);
    conf().set(ConfigOptions::getWriterIncludeCircularErrorTagsKey(), false);

    // TODO: remove
//    conf().set(
//      ConfigOptions::getLogClassFilterKey(),
//      "ChangesetReplacementCreator;MapCropper;OsmXmlReader;MapProjector;ChangesetCreator;ChangesetDeriver;IoUtils;ImmediatelyConnectedOutOfBoundsWayTagger;InBoundsCriterion");
//    conf().set(ConfigOptions::getDebugMapsWriteKey(), true);
  }

  void runPolyLenientOsmTest()
  {     
    const QString testName = "runPolyLenientOsmTest";
    const GeometryType geometryType = GeometryType::Polygon;

    _prepInputData(testName, geometryType);
    _runTest(testName, "osm", geometryType, true);

    CPPUNIT_ASSERT_EQUAL(632, _getChangesetDeriver()->getNumCreateChanges());
    CPPUNIT_ASSERT_EQUAL(0, _getChangesetDeriver()->getNumModifyChanges());
    CPPUNIT_ASSERT_EQUAL(583, _getChangesetDeriver()->getNumDeleteChanges());
  }

  void runPolyStrictOsmTest()
  {
    const QString testName = "runPolyStrictOsmTest";
    const GeometryType geometryType = GeometryType::Polygon;

    _prepInputData(testName, geometryType);
    _runTest(testName, "osm", geometryType, false);

    CPPUNIT_ASSERT_EQUAL(529, _getChangesetDeriver()->getNumCreateChanges());
    CPPUNIT_ASSERT_EQUAL(1, _getChangesetDeriver()->getNumModifyChanges());
    CPPUNIT_ASSERT_EQUAL(517, _getChangesetDeriver()->getNumDeleteChanges());
  }

  void runPoiStrictOsmTest()
  {
    const QString testName = "runPoiStrictOsmTest";
    const GeometryType geometryType = GeometryType::Point;

    _prepInputData(testName, geometryType);
    _runTest(testName, "osm", geometryType, false);

    CPPUNIT_ASSERT_EQUAL(3, _getChangesetDeriver()->getNumCreateChanges());
    CPPUNIT_ASSERT_EQUAL(1, _getChangesetDeriver()->getNumModifyChanges());
    CPPUNIT_ASSERT_EQUAL(1, _getChangesetDeriver()->getNumDeleteChanges());
  }

  void runLinearLenientOsmTest()
  {
    const QString testName = "runLinearLenientOsmTest";
    const GeometryType geometryType = GeometryType::Line;

    _prepInputData(testName, geometryType);
    _runTest(testName, "osm", geometryType, true);

    CPPUNIT_ASSERT_EQUAL(146, _getChangesetDeriver()->getNumCreateChanges());
    CPPUNIT_ASSERT_EQUAL(7, _getChangesetDeriver()->getNumModifyChanges());
    CPPUNIT_ASSERT_EQUAL(141, _getChangesetDeriver()->getNumDeleteChanges());
  }

  void runLinearStrictOsmTest()
  {
    const QString testName = "runLinearStrictOsmTest";
    const GeometryType geometryType = GeometryType::Line;

    _prepInputData(testName, geometryType);
    _runTest(testName, "osm", geometryType, false);

    CPPUNIT_ASSERT_EQUAL(47, _getChangesetDeriver()->getNumCreateChanges());
    CPPUNIT_ASSERT_EQUAL(5, _getChangesetDeriver()->getNumModifyChanges());
    CPPUNIT_ASSERT_EQUAL(36, _getChangesetDeriver()->getNumDeleteChanges());
  }

  void runPolyLenientJsonTest()
  {
    const QString testName = "runPolyLenientJsonTest";
    const GeometryType geometryType = GeometryType::Polygon;

    _prepInputData(testName, geometryType);
    _runTest(testName, "json", geometryType, true);

    CPPUNIT_ASSERT_EQUAL(632, _getChangesetDeriver()->getNumCreateChanges());
    CPPUNIT_ASSERT_EQUAL(0, _getChangesetDeriver()->getNumModifyChanges());
    CPPUNIT_ASSERT_EQUAL(583, _getChangesetDeriver()->getNumDeleteChanges());
  }

  void runPolyStrictJsonTest()
  {
    const QString testName = "runPolyStrictJsonTest";
    const GeometryType geometryType = GeometryType::Polygon;

    _prepInputData(testName, geometryType);
    _runTest(testName, "json", geometryType, false);

    CPPUNIT_ASSERT_EQUAL(529, _getChangesetDeriver()->getNumCreateChanges());
    CPPUNIT_ASSERT_EQUAL(1, _getChangesetDeriver()->getNumModifyChanges());
    CPPUNIT_ASSERT_EQUAL(517, _getChangesetDeriver()->getNumDeleteChanges());
  }

  void runPoiStrictJsonTest()
  {
    const QString testName = "runPoiStrictJsonTest";
    const GeometryType geometryType = GeometryType::Point;

    _prepInputData(testName, geometryType);
    _runTest(testName, "json", geometryType, false);

    CPPUNIT_ASSERT_EQUAL(3, _getChangesetDeriver()->getNumCreateChanges());
    CPPUNIT_ASSERT_EQUAL(1, _getChangesetDeriver()->getNumModifyChanges());
    CPPUNIT_ASSERT_EQUAL(1, _getChangesetDeriver()->getNumDeleteChanges());
  }

  void runLinearLenientJsonTest()
  {
    const QString testName = "runLinearLenientJsonTest";
    const GeometryType geometryType = GeometryType::Line;

    _prepInputData(testName, geometryType);
    _runTest(testName, "json", geometryType, true);

    CPPUNIT_ASSERT_EQUAL(146, _getChangesetDeriver()->getNumCreateChanges());
    CPPUNIT_ASSERT_EQUAL(7, _getChangesetDeriver()->getNumModifyChanges());
    CPPUNIT_ASSERT_EQUAL(141, _getChangesetDeriver()->getNumDeleteChanges());
  }

  void runLinearStrictJsonTest()
  {
    const QString testName = "runLinearStrictJsonTest";
    const GeometryType geometryType = GeometryType::Line;

    _prepInputData(testName, geometryType);
    _runTest(testName, "json", geometryType, false);

    CPPUNIT_ASSERT_EQUAL(47, _getChangesetDeriver()->getNumCreateChanges());
    CPPUNIT_ASSERT_EQUAL(5, _getChangesetDeriver()->getNumModifyChanges());
    CPPUNIT_ASSERT_EQUAL(36, _getChangesetDeriver()->getNumDeleteChanges());
  }

private:

  enum GeometryType
  {
    Point = 0,
    Line,
    Polygon
  };

  geos::geom::Envelope _bounds;
  ChangesetReplacementCreator _changesetReplacementCreator;

  ChangesetDeriverPtr _getChangesetDeriver() const
  {
    return _changesetReplacementCreator._changesetCreator->_changesetDeriver;
  }

  void _prepInputData(const QString& testName, const GeometryType& geometryType)
  {
    // TODO: JSON isn't writing (?) versions properly

    QString customTagKey = "";
    QString customTagVal = "";
    switch (geometryType)
    {
      case GeometryType::Point:
        break;
      case GeometryType::Line:
        customTagKey = "note";
        customTagVal = "Highway";
        break;
      case GeometryType::Polygon:
        customTagKey = "name";
        customTagVal = "Building";
        break;
      default:
        throw IllegalArgumentException("Invalid geometry type.");
    }

    const QString refInXml = _outputPath + testName + "-ref-in.osm";
    QString modifiedCustomTagVal = "";
    if (!customTagVal.isEmpty())
    {
      modifiedCustomTagVal = customTagVal + " 1";
    }
    _generateXml(
      "hoot::PositiveIdGenerator", customTagKey, modifiedCustomTagVal, true, Status::Unknown1,
      refInXml);

    const QString refInJson = _outputPath + testName + "-ref-in.json";
    conf().set(ConfigOptions::getReaderUseDataSourceIdsKey(), true);
    DataConverter().convert(refInXml, refInJson);

    const QString secInXml = _outputPath + testName + "-sec-in.osm";
    if (!customTagVal.isEmpty())
    {
      modifiedCustomTagVal = customTagVal + " 2";
    }
    _generateXml(
      "hoot::DefaultIdGenerator", customTagKey, modifiedCustomTagVal, false, Status::Unknown2,
      secInXml);

    const QString secInJson = _outputPath + testName + "-sec-in.json";
    conf().set(ConfigOptions::getReaderUseDataSourceIdsKey(), true);
    DataConverter().convert(secInXml, secInJson);
  }

  void _generateXml(const QString& idGen, const QString& customTagKey, const QString& customTagVal,
                    const bool perturb, const Status& status, const QString& outFile)
  {
    conf().set(ConfigOptions::getIdGeneratorKey(), idGen);
    conf().set(ConfigOptions::getReaderUseDataSourceIdsKey(), false);

    OsmMapPtr map(new OsmMap());
    IoUtils::loadMap(map, "test-files/BostonSubsetRoadBuilding_FromOsm.osm", false, status);
    MapProjector::projectToWgs84(map);

    if (!customTagKey.isEmpty() && !customTagVal.isEmpty())
    {
      QString criterionName = "";
      if (customTagVal.toLower().contains("building"))
      {
        criterionName = QString::fromStdString(BuildingCriterion::className());
      }
      else
      {
        criterionName = QString::fromStdString(HighwayCriterion::className());
      }
      SetTagValueVisitor tagSetter(customTagKey, customTagVal, false, criterionName);
      map->visitRw(tagSetter);
    }

    if (perturb)
    {
      PertyOp perturber;
      perturber.setSystematicError(15.0, 15.0);
      perturber.setSeed(1);
      perturber.setNamedOps(QStringList());
      perturber.apply(map);
    }

    conf().set(ConfigOptions::getReaderUseDataSourceIdsKey(), true);
    MapProjector::projectToWgs84(map);
    IoUtils::saveMap(map, outFile);
  }

  void _runTest(const QString& testName, const QString& fileExtension,
                const GeometryType& geometryType, const bool lenientBounds)
  {
    if (geometryType == GeometryType::Line)
    {
      double existingWayNodeTolerance = 45.0;
      double snapTolerance = 45.0;
      if (lenientBounds)
      {
        existingWayNodeTolerance = 20.0;
        snapTolerance = 20.0;
      }
      conf().set(
        ConfigOptions::getSnapUnconnectedWaysExistingWayNodeToleranceKey(),
        existingWayNodeTolerance);
      conf().set(ConfigOptions::getSnapUnconnectedWaysSnapToleranceKey(), snapTolerance);
    }

    const QString outFile = _outputPath + testName + "-out.osc";

    _changesetReplacementCreator.create(
      _outputPath + testName + "-ref-in." + fileExtension,
      _outputPath + testName + "-sec-in." + fileExtension, _bounds, _getFilterCrit(geometryType),
      lenientBounds, outFile);

    //HOOT_STR_EQUALS(
      //FileUtils::readFully(_inputPath + testName + "-out.osc"), FileUtils::readFully(outFile));
  }

  QString _getFilterCrit(const GeometryType& geometryType) const
  {
    std::string className;
    switch (geometryType)
    {
      case GeometryType::Point:
        className = PoiCriterion::className();
        break;
      case GeometryType::Line:
        className = HighwayCriterion::className();
        break;
      case GeometryType::Polygon:
        className = BuildingCriterion::className();
        break;
      default:
        throw IllegalArgumentException("Invalid geometry type.");
    }
    return QString::fromStdString(className);
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ChangesetReplacementCreatorTest, "glacial");

}
