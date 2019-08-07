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
 * These tests are very similar to the tests in
 * test-files/cmd/glacial/serial/Service*ReplacementTest.sh. The main differences are that these
 * don't interact with a database, don't try to apply the output changeset, and only generate the
 * input data once.
 *
 * This test file is in hoot-rnd since it needs to use PertyOp.
 */
class ChangesetReplacementCreatorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(ChangesetReplacementCreatorTest);
  // We're already testing API DB inputs with command tests, so skipping those here.
  CPPUNIT_TEST(runPolyLenientOsmTest);
  CPPUNIT_TEST(runPolyStrictOsmTest);
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
    "test-output/rnd/algorithms/changeset/ChangesetReplacementCreatorTest/")//,
  //_bounds(geos::geom::Envelope(-71.4698, -71.4657, 42.4866, 42.4902))
  {
    setResetType(ResetAll);

    _bounds = GeometryUtils::envelopeFromConfigString("-71.4698,42.4866,-71.4657,42.4902");

    conf().set(ConfigOptions::getUuidHelperRepeatableKey(), true);
    conf().set(ConfigOptions::getWriterIncludeDebugTagsKey(), true);
    conf().set(ConfigOptions::getReaderAddSourceDatetimeKey(), false);
    conf().set(ConfigOptions::getWriterIncludeCircularErrorTagsKey(), false);

    _prepInputData();
  }

  void runPolyLenientOsmTest()
  {   
    _runTest("runPolyLenientOsmTest", "osm", GeometryType::Polygon, true);

    CPPUNIT_ASSERT_EQUAL(632, _getChangesetDeriver()->getNumCreateChanges());
    CPPUNIT_ASSERT_EQUAL(0, _getChangesetDeriver()->getNumModifyChanges());
    CPPUNIT_ASSERT_EQUAL(583, _getChangesetDeriver()->getNumDeleteChanges());
  }

  void runPolyStrictOsmTest()
  {
    _runTest("runPolyStrictOsmTest", "osm", GeometryType::Polygon, false);

    CPPUNIT_ASSERT_EQUAL(529, _getChangesetDeriver()->getNumCreateChanges());
    CPPUNIT_ASSERT_EQUAL(1, _getChangesetDeriver()->getNumModifyChanges());
    CPPUNIT_ASSERT_EQUAL(517, _getChangesetDeriver()->getNumDeleteChanges());
  }

  void runPoiStrictOsmTest()
  {
    _runTest("runPoiStrictOsmTest", "osm", GeometryType::Point, false);

    CPPUNIT_ASSERT_EQUAL(3, _getChangesetDeriver()->getNumCreateChanges());
    CPPUNIT_ASSERT_EQUAL(1, _getChangesetDeriver()->getNumModifyChanges());
    CPPUNIT_ASSERT_EQUAL(1, _getChangesetDeriver()->getNumDeleteChanges());
  }

  void runLinearLenientOsmTest()
  {
    _runTest("runLinearLenientOsmTest", "osm", GeometryType::Line, true);

    CPPUNIT_ASSERT_EQUAL(146, _getChangesetDeriver()->getNumCreateChanges());
    CPPUNIT_ASSERT_EQUAL(7, _getChangesetDeriver()->getNumModifyChanges());
    CPPUNIT_ASSERT_EQUAL(141, _getChangesetDeriver()->getNumDeleteChanges());
  }

  void runLinearStrictOsmTest()
  {
    _runTest("runLinearStrictOsmTest", "osm", GeometryType::Line, false);

    CPPUNIT_ASSERT_EQUAL(47, _getChangesetDeriver()->getNumCreateChanges());
    CPPUNIT_ASSERT_EQUAL(5, _getChangesetDeriver()->getNumModifyChanges());
    CPPUNIT_ASSERT_EQUAL(36, _getChangesetDeriver()->getNumDeleteChanges());
  }

  void runPolyLenientJsonTest()
  {
    _runTest("runPolyLenientJsonTest", "json", GeometryType::Polygon, true);

    CPPUNIT_ASSERT_EQUAL(632, _getChangesetDeriver()->getNumCreateChanges());
    CPPUNIT_ASSERT_EQUAL(0, _getChangesetDeriver()->getNumModifyChanges());
    CPPUNIT_ASSERT_EQUAL(583, _getChangesetDeriver()->getNumDeleteChanges());
  }

  void runPolyStrictJsonTest()
  {
    _runTest("runPolyStrictJsonTest", "json", GeometryType::Polygon, false);

    CPPUNIT_ASSERT_EQUAL(529, _getChangesetDeriver()->getNumCreateChanges());
    CPPUNIT_ASSERT_EQUAL(1, _getChangesetDeriver()->getNumModifyChanges());
    CPPUNIT_ASSERT_EQUAL(517, _getChangesetDeriver()->getNumDeleteChanges());
  }

  void runPoiStrictJsonTest()
  {
    _runTest("runPoiStrictJsonTest", "json", GeometryType::Point, false);

    CPPUNIT_ASSERT_EQUAL(3, _getChangesetDeriver()->getNumCreateChanges());
    CPPUNIT_ASSERT_EQUAL(1, _getChangesetDeriver()->getNumModifyChanges());
    CPPUNIT_ASSERT_EQUAL(1, _getChangesetDeriver()->getNumDeleteChanges());
  }

  void runLinearLenientJsonTest()
  {
    _runTest("runLinearLenientJsonTest", "json", GeometryType::Line, true);

    CPPUNIT_ASSERT_EQUAL(146, _getChangesetDeriver()->getNumCreateChanges());
    CPPUNIT_ASSERT_EQUAL(7, _getChangesetDeriver()->getNumModifyChanges());
    CPPUNIT_ASSERT_EQUAL(141, _getChangesetDeriver()->getNumDeleteChanges());
  }

  void runLinearStrictJsonTest()
  {
    _runTest("runLinearStrictJsonTest", "json", GeometryType::Line, false);

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

  void _prepInputData()
  {
    // TODO: JSON isn't setting versions properly

    const QString refInXml = _outputPath + "ref-in.osm";
    if (!QFile(refInXml).exists())
    {
      _generateXml("hoot::PositiveIdGenerator", "Feature 1", true, Status::Unknown1, refInXml);
    }
    const QString refInJson = _outputPath + "ref-in.json";
    if (!QFile(refInJson).exists())
    {
      conf().set(ConfigOptions::getReaderUseDataSourceIdsKey(), true);
      DataConverter().convert(refInXml, refInJson);
    }
    const QString secInXml = _outputPath + "sec-in.osm";
    if (!QFile(secInXml).exists())
    {
      _generateXml("hoot::DefaultIdGenerator", "Feature 2", false, Status::Unknown2, secInXml);
    }
    const QString secInJson = _outputPath + "sec-in.json";
    if (!QFile(secInJson).exists())
    {
      conf().set(ConfigOptions::getReaderUseDataSourceIdsKey(), true);
      DataConverter().convert(secInXml, secInJson);
    }
  }

  void _generateXml(const QString& idGen, const QString& note, const bool perturb,
                    const Status& status, const QString& outFile)
  {
    // TODO: projections issues here

    conf().set(ConfigOptions::getIdGeneratorKey(), idGen);
    conf().set(ConfigOptions::getReaderUseDataSourceIdsKey(), false);

    OsmMapPtr map(new OsmMap());
    IoUtils::loadMap(map, "test-files/BostonSubsetRoadBuilding_FromOsm.osm", false, status);
    MapProjector::projectToWgs84(map);

    SetTagValueVisitor tagSetter("note", note);
    map->visitRw(tagSetter);

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
      _outputPath + "ref-in." + fileExtension, _outputPath + "sec-in." + fileExtension,
      _bounds, _getFilterCrit(geometryType), lenientBounds, outFile);

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
