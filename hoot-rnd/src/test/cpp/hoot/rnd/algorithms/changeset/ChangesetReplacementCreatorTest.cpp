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
#include <hoot/core/visitors/AddUuidVisitor.h>
#include <hoot/core/criterion/GeometryTypeCriterion.h>
#include <hoot/core/visitors/FilteredVisitor.h>
#include <hoot/core/criterion/TagCriterion.h>

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
  // TODO: add error handling tests
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
  //CPPUNIT_TEST(runMultipleGeometryFilter1LenientTest);
  //CPPUNIT_TEST(runMultipleGeometryFilter1StrictTest);
  //CPPUNIT_TEST(runMultipleGeometryFilter2LenientTest);
//  CPPUNIT_TEST(runMultipleGeometryFilter2StrictTest);
//  CPPUNIT_TEST(runAdditionalFilterTest);
//  CPPUNIT_TEST(runGeometryAndAdditionalFilterTest);
//  CPPUNIT_TEST(runEmptyGeometryFilterStrictTest);
   //CPPUNIT_TEST(runEmptyGeometryFilterLenientTest);
  CPPUNIT_TEST_SUITE_END();

public:

  ChangesetReplacementCreatorTest() :
  HootTestFixture(
    "test-files/rnd/algorithms/changeset/ChangesetReplacementCreatorTest/",
    "test-output/rnd/algorithms/changeset/ChangesetReplacementCreatorTest/")
  {
    setResetType(ResetAll);

    conf().set(ConfigOptions::getUuidHelperRepeatableKey(), true);
    conf().set(ConfigOptions::getWriterIncludeDebugTagsKey(), true);
    conf().set(ConfigOptions::getReaderAddSourceDatetimeKey(), false);
    conf().set(ConfigOptions::getWriterIncludeCircularErrorTagsKey(), false);
//    conf().set(
//      ConfigOptions::getLogClassFilterKey(),
//      "ChangesetReplacementCreatorTest");
    // for testing only
    //conf().set(ConfigOptions::getDebugMapsFilenameKey(), "/home/vagrant/hoot/tmp/debug.osm");
  }

  void runPolyLenientOsmTest()
  {     
    _runSingleGeometryFilterTest(
      "runPolyLenientOsmTest", GeometryTypeCriterion::GeometryType::Polygon, true, 632, 0, 583);
  }

  void runPolyStrictOsmTest()
  {
    _runSingleGeometryFilterTest(
      "runPolyStrictOsmTest", GeometryTypeCriterion::GeometryType::Polygon, false, 529, 1, 517);
  }

  void runPoiStrictOsmTest()
  {  
    _runSingleGeometryFilterTest(
      "runPoiStrictOsmTest", GeometryTypeCriterion::GeometryType::Point, false, 3, 1, 1);
  }

  void runLinearLenientOsmTest()
  {
    _runSingleGeometryFilterTest(
      "runLinearLenientOsmTest", GeometryTypeCriterion::GeometryType::Line, true, 146, 7, 141);
  }

  void runLinearStrictOsmTest()
  {
    _runSingleGeometryFilterTest(
      "runLinearStrictOsmTest", GeometryTypeCriterion::GeometryType::Line, false, 47, 5, 36);
  }

  void runPolyLenientJsonTest()
  {
    _runSingleGeometryFilterTest(
      "runPolyLenientJsonTest", GeometryTypeCriterion::GeometryType::Polygon, true, 632, 0, 583);
  }

  void runPolyStrictJsonTest()
  {
    _runSingleGeometryFilterTest(
      "runPolyStrictJsonTest", GeometryTypeCriterion::GeometryType::Polygon, false, 529, 1, 517);
  }

  void runPoiStrictJsonTest()
  {
    _runSingleGeometryFilterTest(
      "runPoiStrictJsonTest", GeometryTypeCriterion::GeometryType::Point, false, 3, 1, 1);
  }

  void runLinearLenientJsonTest()
  {
    _runSingleGeometryFilterTest(
      "runLinearLenientJsonTest", GeometryTypeCriterion::GeometryType::Line, true, 146, 7, 141);
  }

  void runLinearStrictJsonTest()
  {
    _runSingleGeometryFilterTest(
      "runLinearStrictJsonTest", GeometryTypeCriterion::GeometryType::Line, false, 47, 5, 36);
  }

  void runMultipleGeometryFilter1LenientTest()
  {
    // buildings and roads; original non-point AOI
    QStringList geometryFilters;
    geometryFilters.append(QString::fromStdString(HighwayCriterion::className()));
    geometryFilters.append(QString::fromStdString(BuildingCriterion::className()));
    _runMultipleFilterXmlTest(
      "runMultipleGeometryFilter1LenientTest",
      geos::geom::Envelope(-71.4698, -71.4657, 42.4866, 42.4902), geometryFilters, QStringList(),
      true, 0, 0, 0);
  }

  void runMultipleGeometryFilter1StrictTest()
  {
    // buildings and roads; original non-point AOI
    QStringList geometryFilters;
    geometryFilters.append(QString::fromStdString(HighwayCriterion::className()));
    geometryFilters.append(QString::fromStdString(BuildingCriterion::className()));
    _runMultipleFilterXmlTest(
      "runMultipleGeometryFilter1StrictTest",
      geos::geom::Envelope(-71.4698, -71.4657, 42.4866, 42.4902), geometryFilters, QStringList(),
      false, 0, 0, 0);
  }

  void runMultipleGeometryFilter2LenientTest()
  {
    // buildings and pois; new AOI
    QStringList geometryFilters;
    geometryFilters.append(QString::fromStdString(BuildingCriterion::className()));
    geometryFilters.append(QString::fromStdString(PoiCriterion::className()));
    _runMultipleFilterXmlTest(
      "runMultipleGeometryFilter2LenientTest",
      geos::geom::Envelope(-71.47355, -71.4657, 42.47595, 42.47675), geometryFilters, QStringList(),
      true, 0, 0, 0);
  }

  void runMultipleGeometryFilter2StrictTest()
  {
    // buildings and pois; new AOI
    QStringList geometryFilters;
    geometryFilters.append(QString::fromStdString(BuildingCriterion::className()));
    geometryFilters.append(QString::fromStdString(PoiCriterion::className()));
    _runMultipleFilterXmlTest(
      "runMultipleGeometryFilter2StrictTest",
      geos::geom::Envelope(-71.47355, -71.4657, 42.47595, 42.47675), geometryFilters, QStringList(),
      false, 0, 0, 0);
  }

  void runAdditionalFilterTest()
  {
    // no geometry filter over original non-point AOI with some tag crit added
    QStringList additionalFilters;
    additionalFilters.append(QString::fromStdString(TagCriterion::className()));
    conf().set("tag.criterion.kvps", "amenity=restaurant");
    _runMultipleFilterXmlTest(
      "runAdditionalFilterTest",
      geos::geom::Envelope(-71.4698, -71.4657, 42.4866, 42.4902), QStringList(),
      additionalFilters, true, 0, 0, 0);
  }

  void runGeometryAndAdditionalFilterTest()
  {
    // buildings and pois; new AOI with some tag crit added
    QStringList geometryFilters;
    geometryFilters.append(QString::fromStdString(BuildingCriterion::className()));
    geometryFilters.append(QString::fromStdString(PoiCriterion::className()));
    QStringList additionalFilters;
    additionalFilters.append(QString::fromStdString(TagCriterion::className()));
    _runMultipleFilterXmlTest(
      "runGeometryAndAdditionalFilterTest",
      geos::geom::Envelope(-71.47355, -71.4657, 42.47595, 42.47675), geometryFilters,
      additionalFilters, true, 0, 0, 0);
  }

  void runEmptyGeometryFilterLenientTest()
  {
    //original non-point AOI
    _runMultipleFilterXmlTest(
      "runEmptyGeometryFilterLenientTest",
      geos::geom::Envelope(-71.4698, -71.4657, 42.4866, 42.4902), QStringList(), QStringList(),
      true, 0, 0, 0);
  }

  void runEmptyGeometryFilterStrictTest()
  {
    //original non-point AOI
    _runMultipleFilterXmlTest(
      "runEmptyGeometryFilterStrictTest",
      geos::geom::Envelope(-71.4698, -71.4657, 42.4866, 42.4902), QStringList(), QStringList(),
      false, 0, 0, 0);
  }

private:

  void _copyJson(const QString& inXmlFile, const QString& outFile)
  {
    LOG_DEBUG("Converting xml: " << inXmlFile << " to json: " << outFile << "...");
    DataConverter().convert(inXmlFile, outFile);
  }

  // TODO: Can probably collapse the single/multiple filter code paths into one.

  void _runSingleGeometryFilterTest(
    const QString& testName, const GeometryTypeCriterion::GeometryType& geometryType,
    const bool lenientBounds, const int numExpectedCreateStatements,
    const int numExpectedModifyStatements, const int numExpectedDeleteStatements)
  {
    // This log disabling statement is here to avoid seeing the missing element warnings from the
    // readers after cropping. Turn off for testing only.
    DisableLog dl;

    // set the way snap opts regardless...will only end up being used if the geometry type is linear
    _setWaySnapOpts(lenientBounds);

    _prepSingleGeometryFilterInputData(testName, geometryType);

    const QString outFile = _outputPath + testName + "-out.osc";
    QString fileExtension;
    if (testName.toLower().contains("osm"))
    {
      fileExtension = "osm";
    }
    else
    {
      fileExtension = "json";
    }

    ChangesetReplacementCreator changesetCreator;
    changesetCreator.setLenientBounds(lenientBounds);
    changesetCreator.setGeometryFilters(
      QStringList(_getFilterCritForSingleGeometryType(geometryType)));
    changesetCreator.create(
      _outputPath + testName + "-ref-in." + fileExtension,
      _outputPath + testName + "-sec-in." + fileExtension,
      _getBoundsForSingleGeometryType(geometryType), outFile);

    // Going only with this level of checking for now. We could extend this to diff the actual
    // changeset files.
    CPPUNIT_ASSERT_EQUAL(
      numExpectedCreateStatements, changesetCreator._changesetCreator->getNumCreateChanges());
    CPPUNIT_ASSERT_EQUAL(
      numExpectedModifyStatements, changesetCreator._changesetCreator->getNumModifyChanges());
    CPPUNIT_ASSERT_EQUAL(
      numExpectedDeleteStatements, changesetCreator._changesetCreator->getNumDeleteChanges());
  }

  OsmMapPtr _getSingleGeometryFilterTestMap(
    const QString& sourceFile, const std::shared_ptr<IdGenerator>& idGen,
    const QString& customTagKey, const QString& customTagVal, const bool perturb)
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

  void _prepSingleGeometryFilterInputData(
    const QString& testName, const GeometryTypeCriterion::GeometryType& geometryType)
  {
    LOG_DEBUG("Preparing input data...");

    QString customTagKey = "";
    QString customTagVal = "";
    QString refSourceFile = "test-files/BostonSubsetRoadBuilding_FromOsm.osm";
    QString secSourceFile = refSourceFile;
    switch (geometryType)
    {
      case GeometryTypeCriterion::GeometryType::Point:
        refSourceFile = "test-files/cmd/glacial/PoiPolygonConflateStandaloneTest/PoiPolygon1.osm";
        secSourceFile = "test-files/cmd/glacial/PoiPolygonConflateStandaloneTest/PoiPolygon2.osm";
        break;
      case GeometryTypeCriterion::GeometryType::Line:
        customTagKey = "note";
        customTagVal = "Highway";
        break;
      case GeometryTypeCriterion::GeometryType::Polygon:
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
    const bool perturbRef = geometryType != GeometryTypeCriterion::GeometryType::Point;
    OsmMapPtr refMap =
      _getSingleGeometryFilterTestMap(
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
      _getSingleGeometryFilterTestMap(
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

  OsmMapPtr _getMultipleFilterXmlTestMap(
    const QString& sourceFile, const std::shared_ptr<IdGenerator>& idGen,
    const QList<std::shared_ptr<SetTagValueVisitor>>& tagVisitors, const bool perturb)
  {
    LOG_DEBUG("Preparing map from: " << sourceFile << "...");

    OsmMapPtr map(new OsmMap());
    map->setIdGenerator(idGen);
    IoUtils::loadMap(map, sourceFile, false);

    for (QList<std::shared_ptr<SetTagValueVisitor>>::const_iterator itr = tagVisitors.begin();
         itr != tagVisitors.end(); ++itr)
    {
      std::shared_ptr<SetTagValueVisitor> vis = *itr;
      map->visitRw(*vis);
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

  void _prepMultipleFilterXmlInputData(
    const QString& testName,  const QStringList& geometryFilters)
  {
    LOG_DEBUG("Preparing input data...");

    const QString refSourceFile = "test-files/BostonSubsetRoadBuilding_FromOsm.osm";
    const QString secSourceFile = refSourceFile;

    QList<std::shared_ptr<SetTagValueVisitor>> tagVisitors;

    if (geometryFilters.contains(QString::fromStdString(HighwayCriterion::className())))
    {
      tagVisitors.append(std::shared_ptr<SetTagValueVisitor>(
        new SetTagValueVisitor(
          "note", "Highway 1", false, QString::fromStdString(HighwayCriterion::className()))));
    }
    if (geometryFilters.contains(QString::fromStdString(BuildingCriterion::className())))
    {
      tagVisitors.append(std::shared_ptr<SetTagValueVisitor>(
        new SetTagValueVisitor(
          "name", "Building 1", false, QString::fromStdString(BuildingCriterion::className()))));
    }
    OsmMapPtr refMap =
      _getMultipleFilterXmlTestMap(
        refSourceFile, std::shared_ptr<IdGenerator>(new PositiveIdGenerator()), tagVisitors, true);
    IoUtils::saveMap(refMap, _outputPath + testName + "-ref-in.osm");

    tagVisitors.clear();
    if (geometryFilters.contains(QString::fromStdString(HighwayCriterion::className())))
    {
      tagVisitors.append(std::shared_ptr<SetTagValueVisitor>(
        new SetTagValueVisitor(
          "note", "Highway 2", false, QString::fromStdString(HighwayCriterion::className()))));
    }
    if (geometryFilters.contains(QString::fromStdString(BuildingCriterion::className())))
    {
      tagVisitors.append(std::shared_ptr<SetTagValueVisitor>(
        new SetTagValueVisitor(
          "name", "Building 2", false, QString::fromStdString(BuildingCriterion::className()))));
    }
    OsmMapPtr secMap =
      _getMultipleFilterXmlTestMap(
        secSourceFile, std::shared_ptr<IdGenerator>(new DefaultIdGenerator()), tagVisitors, false);
    IoUtils::saveMap(secMap, _outputPath + testName + "-sec-in.osm");

    _copyJson(_outputPath + testName + "-sec-in.osm", _outputPath + "temp-do-not-use.json");
  }

  void _runMultipleFilterXmlTest(
    const QString& testName, const geos::geom::Envelope& bounds, const QStringList& geometryFilters,
    const QStringList& additionalFilters, const bool lenientBounds,
    const int numExpectedCreateStatements, const int numExpectedModifyStatements,
    const int numExpectedDeleteStatements)
  {
    DisableLog dl;

    _setWaySnapOpts(lenientBounds);
    _prepMultipleFilterXmlInputData(testName, geometryFilters);

    const QString outFile = _outputPath + testName + "-out.osc";

    ChangesetReplacementCreator changesetCreator;
    changesetCreator.setLenientBounds(lenientBounds);
    changesetCreator.setGeometryFilters(geometryFilters);
    changesetCreator.setAdditionalFilters(additionalFilters);
    changesetCreator.create(
      _outputPath + testName + "-ref-in.osm", _outputPath + testName + "-sec-in.osm", bounds,
      outFile);

    CPPUNIT_ASSERT_EQUAL(
      numExpectedCreateStatements, changesetCreator._changesetCreator->getNumCreateChanges());
    CPPUNIT_ASSERT_EQUAL(
      numExpectedModifyStatements, changesetCreator._changesetCreator->getNumModifyChanges());
    CPPUNIT_ASSERT_EQUAL(
      numExpectedDeleteStatements, changesetCreator._changesetCreator->getNumDeleteChanges());
  }

  void _setWaySnapOpts(const bool lenientBounds)
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

  QString _getFilterCritForSingleGeometryType(
    const GeometryTypeCriterion::GeometryType& geometryType) const
  {
    std::string className;
    switch (geometryType)
    {
      case GeometryTypeCriterion::GeometryType::Point:
        className = PoiCriterion::className();
        break;
      case GeometryTypeCriterion::GeometryType::Line:
        className = HighwayCriterion::className();
        break;
      case GeometryTypeCriterion::GeometryType::Polygon:
        className = BuildingCriterion::className();
        break;
      default:
        throw IllegalArgumentException("Invalid geometry type.");
    }
    return QString::fromStdString(className);
  }

  geos::geom::Envelope _getBoundsForSingleGeometryType(
    const GeometryTypeCriterion::GeometryType& geometryType) const
  {
    switch (geometryType)
    {
      case GeometryTypeCriterion::GeometryType::Point:
        return geos::geom::Envelope(-122.43204, -122.4303457, 37.7628, 37.76437);
      case GeometryTypeCriterion::GeometryType::Line:
        return geos::geom::Envelope(-71.4698, -71.4657, 42.4866, 42.4902);
      case GeometryTypeCriterion::GeometryType::Polygon:
        return geos::geom::Envelope(-71.4698, -71.4657, 42.4866, 42.4902);
      default:
        throw IllegalArgumentException("Invalid geometry type.");
    }
    return geos::geom::Envelope();
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ChangesetReplacementCreatorTest, "glacial");

}
