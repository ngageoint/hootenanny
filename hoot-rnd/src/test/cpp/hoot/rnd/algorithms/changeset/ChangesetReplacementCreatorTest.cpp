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
#include <hoot/core/util/PositiveIdGenerator.h>
#include <hoot/core/util/DefaultIdGenerator.h>
//#include <hoot/core/visitors/AddUuidVisitor.h>

namespace hoot
{

/*
 * These tests are very similar to the tests in Service*ReplacementTest.sh, which test the
 * replacement changeset workflow against API DB data sources. Differences here are that these
 * tests test against file data sources only, don't interact with a database, and don't try to
 * apply the output changeset.
 *
 * This test file is in hoot-rnd since it needs to use PertyOp.
 */
class ChangesetReplacementCreatorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(ChangesetReplacementCreatorTest);
  CPPUNIT_TEST(runPolyLenientOsmTest);
  CPPUNIT_TEST(runPolyStrictOsmTest);
  CPPUNIT_TEST(runPoiStrictOsmTest);
  CPPUNIT_TEST(runLinearLenientOsmTest);
  CPPUNIT_TEST(runLinearStrictOsmTest);
  CPPUNIT_TEST(runPolyLenientJsonTest);
  CPPUNIT_TEST(runPolyStrictJsonTest);
  CPPUNIT_TEST(runPoiStrictJsonTest);
  CPPUNIT_TEST(runLinearLenientJsonTest);
  CPPUNIT_TEST(runLinearStrictJsonTest);
  // TODO: multiple geometry filter test
  // TODO: additional filter test
  // TODO: geometry and additional filter test
  // TODO: empty filter test
  CPPUNIT_TEST_SUITE_END();

public:

  ChangesetReplacementCreatorTest() :
  HootTestFixture(
    "test-files/rnd/algorithms/changeset/ChangesetReplacementCreatorTest/",
    "test-output/rnd/algorithms/changeset/ChangesetReplacementCreatorTest/"),
  _goldFileDirBase("test-files/cmd/glacial/serial/")
  {
    setResetType(ResetAll);

    conf().set(ConfigOptions::getUuidHelperRepeatableKey(), true);
    conf().set(ConfigOptions::getWriterIncludeDebugTagsKey(), true);
    conf().set(ConfigOptions::getReaderAddSourceDatetimeKey(), false);
    conf().set(ConfigOptions::getWriterIncludeCircularErrorTagsKey(), false);
//    conf().set(
//      ConfigOptions::getLogClassFilterKey(),
//      "ChangesetReplacementCreatorTest");
  }

  void runPolyLenientOsmTest()
  {     
    // This is here to avoid seeing the missing element warnings from the readers after cropping.
    DisableLog dl;

    const QString testName = "runPolyLenientOsmTest";
    LOG_DEBUG("Running test: " << testName << "...");
    const GeometryType geometryType = GeometryType::Polygon;
//    const QString goldTestName = "ServiceBuildingReplacementTest";
//    const QString goldFile =
//      _goldFileDirBase + goldTestName + "/" + goldTestName + "-changeset-1.osc";

    _prepInputData(testName, geometryType);
    _runTest(testName, "osm", geometryType, true, 632, 0, 583, "");
  }

  void runPolyStrictOsmTest()
  {
    DisableLog dl;

    const QString testName = "runPolyStrictOsmTest";
    const GeometryType geometryType = GeometryType::Polygon;
/*    const QString goldTestName = "ServiceBuildingStrictReplacementTest";
    const QString goldFile =
      _goldFileDirBase + goldTestName + "/" + goldTestName + "-changeset-1.osc"*/;

    _prepInputData(testName, geometryType);
    _runTest(
      testName, "osm", geometryType, false, 529, 1, 517, "");
  }

  void runPoiStrictOsmTest()
  {
    DisableLog dl;

    const QString testName = "runPoiStrictOsmTest";
    const GeometryType geometryType = GeometryType::Point;
//    const QString goldTestName = "ServicePoiStrictReplacementTest";
//    const QString goldFile =
//      _goldFileDirBase + goldTestName + "/" + goldTestName + "-changeset-1.osc";

    _prepInputData(testName, geometryType);
    _runTest(testName, "osm", geometryType, false, 3, 1, 1, "");
  }

  void runLinearLenientOsmTest()
  {
    DisableLog dl;

    const QString testName = "runLinearLenientOsmTest";
    const GeometryType geometryType = GeometryType::Line;
//    const QString goldTestName = "ServiceRoadReplacementTest";
//    const QString goldFile =
//      _goldFileDirBase + goldTestName + "/" + goldTestName + "-changeset-1.osc";

    _prepInputData(testName, geometryType);
    _runTest(testName, "osm", geometryType, true, 146, 7, 141, "");
  }

  void runLinearStrictOsmTest()
  {
    DisableLog dl;

    const QString testName = "runLinearStrictOsmTest";
    const GeometryType geometryType = GeometryType::Line;
//    const QString goldTestName = "ServiceRoadStrictReplacementTest";
//    const QString goldFile =
//      _goldFileDirBase + goldTestName + "/" + goldTestName + "-changeset-1.osc";

    _prepInputData(testName, geometryType);
    _runTest(testName, "osm", geometryType, false, 47, 5, 36, "");
  }

  void runPolyLenientJsonTest()
  {
    DisableLog dl;

    const QString testName = "runPolyLenientJsonTest";
    LOG_DEBUG("Running test: " << testName << "...");
    const GeometryType geometryType = GeometryType::Polygon;
//    const QString goldTestName = "ServiceBuildingReplacementTest";
//    const QString goldFile =
//      _goldFileDirBase + goldTestName + "/" + goldTestName + "-changeset-1.osc";

    _prepInputData(testName, geometryType);
    _runTest(testName, "json", geometryType, true, 632, 0, 583, "");
  }

  void runPolyStrictJsonTest()
  {
    DisableLog dl;

    const QString testName = "runPolyStrictJsonTest";
    const GeometryType geometryType = GeometryType::Polygon;
/*    const QString goldTestName = "ServiceBuildingStrictReplacementTest";
    const QString goldFile =
      _goldFileDirBase + goldTestName + "/" + goldTestName + "-changeset-1.osc"*/;

    _prepInputData(testName, geometryType);
    _runTest(testName, "json", geometryType, false, 529, 1, 517, "");
  }

  void runPoiStrictJsonTest()
  {
    DisableLog dl;

    const QString testName = "runPoiStrictJsonTest";
    const GeometryType geometryType = GeometryType::Point;
//    const QString goldTestName = "ServicePoiStrictReplacementTest";
//    const QString goldFile =
//      _goldFileDirBase + goldTestName + "/" + goldTestName + "-changeset-1.osc";

    _prepInputData(testName, geometryType);
    _runTest(testName, "json", geometryType, false, 3, 1, 1, "");
  }

  void runLinearLenientJsonTest()
  {
    DisableLog dl;

    const QString testName = "runLinearLenientJsonTest";
    const GeometryType geometryType = GeometryType::Line;
//    const QString goldTestName = "ServiceRoadReplacementTest";
//    const QString goldFile =
//      _goldFileDirBase + goldTestName + "/" + goldTestName + "-changeset-1.osc";

    _prepInputData(testName, geometryType);
    _runTest(testName, "json", geometryType, true, 146, 7, 141, "");
  }

  void runLinearStrictJsonTest()
  {
    DisableLog dl;

    const QString testName = "runLinearStrictJsonTest";
    const GeometryType geometryType = GeometryType::Line;
//    const QString goldTestName = "ServiceRoadStrictReplacementTest";
//    const QString goldFile =
//      _goldFileDirBase + goldTestName + "/" + goldTestName + "-changeset-1.osc";

    _prepInputData(testName, geometryType);
    _runTest(testName, "json", geometryType, false, 47, 5, 36, "");
  }

private:

  QString _goldFileDirBase;

  enum GeometryType
  {
    Point = 0,
    Line,
    Polygon
  };

  void _prepInputData(const QString& testName, const GeometryType& geometryType)
  {
    LOG_DEBUG("Preparing input data...");

    QString customTagKey = "";
    QString customTagVal = "";
    QString refSourceFile = "test-files/BostonSubsetRoadBuilding_FromOsm.osm";
    QString secSourceFile = refSourceFile;
    switch (geometryType)
    {
      case GeometryType::Point:
        refSourceFile = "test-files/cmd/glacial/PoiPolygonConflateStandaloneTest/PoiPolygon1.osm";
        secSourceFile = "test-files/cmd/glacial/PoiPolygonConflateStandaloneTest/PoiPolygon2.osm";
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

    QString modifiedCustomTagVal = "";
    if (!customTagVal.isEmpty())
    {
      modifiedCustomTagVal = customTagVal + " 1";
    }
    const bool perturbRef = geometryType != GeometryType::Point;
    OsmMapPtr refMap =
      _getTestMap(
        refSourceFile, std::shared_ptr<IdGenerator>(new PositiveIdGenerator()), customTagKey,
        modifiedCustomTagVal, perturbRef);
    QString outFile = _outputPath + testName + "-ref-in.";
    if (testName.toLower().contains("osm"))
    {
      outFile += "osm";
    }
    else
    {
      outFile += "json";
    }
    IoUtils::saveMap(refMap, outFile);

    if (!customTagVal.isEmpty())
    {
      modifiedCustomTagVal = customTagVal + " 2";
    }
    OsmMapPtr secMap =
      _getTestMap(
        secSourceFile, std::shared_ptr<IdGenerator>(new DefaultIdGenerator()), customTagKey,
        modifiedCustomTagVal, false);
    outFile = outFile.replace("ref", "sec");
    IoUtils::saveMap(secMap, outFile);

    // TODO: This is very strange... If I don't call this method at the end, a couple of tests fail.
    // The only thing I can imagine is that DataConverter is setting some global config that happens
    // to be needed by the tests. I've tried what's commented out below and none of them do the
    // trick.
    _copyJson(outFile, _outputPath + "temp-do-not-use.json");
    //conf().set(ConfigOptions::getReaderUseFileStatusKey(), true);
    //conf().set(ConfigOptions::getReaderKeepStatusTagKey(), true);
  }

  OsmMapPtr _getTestMap(const QString& sourceFile, const std::shared_ptr<IdGenerator>& idGen,
                        const QString& customTagKey, const QString& customTagVal,
                        const bool perturb)
  {
    LOG_DEBUG("Preparing map from: " << sourceFile << "...");

    OsmMapPtr map(new OsmMap());
    map->setIdGenerator(idGen);
    IoUtils::loadMap(map, sourceFile, false);

    if (!customTagKey.isEmpty() && !customTagVal.isEmpty())
    {
      LOG_DEBUG("Setting custom tag: " << customTagKey << "=" << customTagVal << "...");
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
      LOG_DEBUG("Perturbing map...");
      PertyOp perturber;
      perturber.setSystematicError(15.0, 15.0);
      perturber.setSeed(1);
      perturber.setNamedOps(QStringList());
      perturber.apply(map);
      MapProjector::projectToWgs84(map);  // perty works in planar
    }

    //AddUuidVisitor uuidAdder("uuid");
    //map->visitRw(uuidAdder);

    return map;
  }

  void _copyJson(const QString& inXmlFile, const QString& outFile)
  {
    LOG_DEBUG("Converting xml: " << inXmlFile << " to json: " << outFile << "...");
    DataConverter().convert(inXmlFile, outFile);
  }

  void _runTest(const QString& testName, const QString& fileExtension,
                const GeometryType& geometryType, const bool lenientBounds,
                const int numExpectedCreateStatements, const int numExpectedModifyStatements,
                const int numExpectedDeleteStatements, const QString goldChangesetFile)
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

    ChangesetReplacementCreator changesetCreator;
    changesetCreator.setLenientBounds(lenientBounds);
    changesetCreator.setGeometryFilters(QStringList(_getFilterCrit(geometryType)));
    //changesetCreator.setAdditionalFilters(additionalFilters);
    //changesetCreator.setChainAdditionalFilters(false);
    changesetCreator.create(
      _outputPath + testName + "-ref-in." + fileExtension,
      _outputPath + testName + "-sec-in." + fileExtension, _getBounds(geometryType), outFile);

    // Going only with this level of checking for now. We could extend this to diff the actual
    // changeset files.
    CPPUNIT_ASSERT_EQUAL(
      numExpectedCreateStatements, changesetCreator._changesetCreator->getNumCreateChanges());
    CPPUNIT_ASSERT_EQUAL(
      numExpectedModifyStatements, changesetCreator._changesetCreator->getNumModifyChanges());
    CPPUNIT_ASSERT_EQUAL(
      numExpectedDeleteStatements, changesetCreator._changesetCreator->getNumDeleteChanges());
    // ignoring this for now
    if (!goldChangesetFile.isEmpty())
    {
      HOOT_FILE_EQUALS(goldChangesetFile, outFile);
    }
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

  geos::geom::Envelope _getBounds(const GeometryType& geometryType) const
  {
    switch (geometryType)
    {
      case GeometryType::Point:
        return geos::geom::Envelope(-122.43204, -122.4303457, 37.7628, 37.76437);
      case GeometryType::Line:
        return geos::geom::Envelope(-71.4698, -71.4657, 42.4866, 42.4902);
      case GeometryType::Polygon:
        return geos::geom::Envelope(-71.4698, -71.4657, 42.4866, 42.4902);
      default:
        throw IllegalArgumentException("Invalid geometry type.");
    }
    return geos::geom::Envelope();
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ChangesetReplacementCreatorTest, "glacial");

}
