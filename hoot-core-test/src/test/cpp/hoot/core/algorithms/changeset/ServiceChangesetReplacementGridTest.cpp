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
 * @copyright Copyright (C) 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/io/HootApiDb.h>
#include <hoot/core/geometry/GeometryUtils.h>
#include <hoot/core/io/ServicesDbTestUtils.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/ConfigUtils.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/ops/MapCropper.h>
#include <hoot/core/algorithms/changeset/ChangesetTaskGridReplacer.h>
#include <hoot/core/algorithms/changeset/BoundsFileTaskGridGenerator.h>
#include <hoot/core/algorithms/changeset/NodeDensityTaskGridGenerator.h>
#include <hoot/core/algorithms/changeset/UniformTaskGridGenerator.h>
#include <hoot/core/algorithms/changeset/BoundsStringTaskGridGenerator.h>
#include <hoot/core/conflate/ConflateUtils.h>
#include <hoot/core/visitors/SetTagValueVisitor.h>
#include <hoot/core/criterion/WayNodeCriterion.h>
#include <hoot/core/criterion/NotCriterion.h>
#include <hoot/core/visitors/FilteredVisitor.h>
#include <hoot/core/io/OsmApiDbSqlChangesetApplier.h>

namespace hoot
{

static const QString USER_EMAIL = "ServiceChangesetReplacementGridTest@hoottestcpp.org";
static const QString DATA_TO_REPLACE_URL = ServicesDbTestUtils::getOsmApiDbUrl().toString();
// enables diff conflate to calculate the difference between the final replaced data and the
// original data used for replacement
static const bool CALC_DIFF_BETWEEN_REPLACED_AND_REPLACEMENT = false;
// If true, unconflatable data for certain tests is output to its own file.
static const bool WRITE_NON_CONFLATABLE = false;
// allows for bypassing some of the metric asserts so you view their values
static const bool DISPLAY_METRICS_ONLY = false;

/*
 * This test harness allows for testing the Cut and Replace workflow across adjacent task grid
 * cells. By removing the processes of input data retrieval and changeset application via API from
 * the test workflow, bugs can more easily be narrowed down to just those caused by
 * ChangesetReplacementGenerator (most of the time). This test class can also be used for simple
 * single bounds replacement by generating a task grid of size = 1.
 */
class ServiceChangesetReplacementGridTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(ServiceChangesetReplacementGridTest);

  // single cell tests
  CPPUNIT_TEST(badPolyIdSync1Test);
  CPPUNIT_TEST(badPolyIdSync2Test);
  CPPUNIT_TEST(differingTypes1Test);
  CPPUNIT_TEST(outOfSpecMixedRelations1Test);
  CPPUNIT_TEST(poi1Test);
  CPPUNIT_TEST(refFilteredToEmpty1Test);
  CPPUNIT_TEST(refSinglePoint1Test);
  CPPUNIT_TEST(relationCrop1Test);
  CPPUNIT_TEST(riverbank1Test);
  CPPUNIT_TEST(roundabouts1Test);
  CPPUNIT_TEST(secFilteredToEmpty1Test);

  // multiple cell tests
  CPPUNIT_TEST(droppedNodes1Test);
  CPPUNIT_TEST(droppedPointPolyRelationMembers1Test);
  CPPUNIT_TEST(orphanedNodes1Test);
  CPPUNIT_TEST(orphanedNodes2Test);

  // ENABLE THESE TESTS FOR DEBUGGING ONLY

  //CPPUNIT_TEST(github4216UniformTest);
  //CPPUNIT_TEST(northVegasLargeUniformTest);
  //CPPUNIT_TEST(auditionTest);

  CPPUNIT_TEST_SUITE_END();

public:

  ServiceChangesetReplacementGridTest() :
  HootTestFixture(
    "test-files/algorithms/changeset/ServiceChangesetReplacementGridTest",
    "test-output/algorithms/changeset/ServiceChangesetReplacementGridTest"),
  _originalDataSize(0)
  {
    setResetType(ResetAll);
  }

  virtual void setUp()
  {
    HootTestFixture::setUp(); // Be sure the parent cleans out the config before each test.
    _subTaskTimer.start();
    _initConfig();
  }

  virtual void tearDown()
  {
    _cleanupDataToReplace();
    _cleanupReplacementData();
  }

  void badPolyIdSync1Test()
  {
    // part of github 4297 - The landuse=residential poly surrounding Flanagan Drive should not be
    // corrupted in the output. Before this fix, skipped element ID synchronizations were causing
    // the upper right node to drop out and mangle the poly.

    _testName = "badPolyIdSync1Test";
    _prepInput(
      _inputPath + "/" + _testName + "-Input1.osm",
      _inputPath + "/" + _testName + "-Input2.osm",
      "");
    conf().set(
      ConfigOptions::getDebugMapsFilenameKey(), _outputPath + "/" + _testName + "-debug.osm");

    ChangesetTaskGridReplacer uut;
    uut.setChangesetsOutputDir(_outputPath);
    const QString outFile = _testName + "-out.osm";
    const QString outFull = _outputPath + "/" + outFile;
    uut.setJobName(_testName);
    uut.setWriteFinalOutput(outFull);
    uut.setOriginalDataSize(_originalDataSize);
    uut.setTagQualityIssues(true);
    uut.replace(
      DATA_TO_REPLACE_URL,
      _replacementDataUrl,
      UniformTaskGridGenerator(
        "-115.2434,36.3022,-115.2317,36.3136", 1,
        _outputPath + "/" + _testName + "-" + "taskGridBounds.osm")
        .generateTaskGrid());

    const QMap<QString, long> changesetStats = uut.getChangesetStats();
    if (DISPLAY_METRICS_ONLY)
    {
      LOG_WARN(_testName + ": " << changesetStats);
      LOG_VARW(uut.getOutputMetrics().getNumOrphanedNodesInOutput());
      LOG_VARW(uut.getOutputMetrics().getNumDisconnectedWaysInOutput());
      LOG_VARW(uut.getOutputMetrics().getNumEmptyWaysInOutput());
      LOG_VARW(uut.getOutputMetrics().getNumDuplicateElementPairsInOutput());
    }
    else
    {
      CPPUNIT_ASSERT_EQUAL(123L, changesetStats[OsmApiDbSqlChangesetApplier::NODE_CREATE_KEY]);
      CPPUNIT_ASSERT_EQUAL(0L, changesetStats[OsmApiDbSqlChangesetApplier::NODE_MODIFY_KEY]);
      CPPUNIT_ASSERT_EQUAL(51L, changesetStats[OsmApiDbSqlChangesetApplier::NODE_DELETE_KEY]);
      CPPUNIT_ASSERT_EQUAL(80L, changesetStats[OsmApiDbSqlChangesetApplier::WAY_CREATE_KEY]);
      CPPUNIT_ASSERT_EQUAL(0L, changesetStats[OsmApiDbSqlChangesetApplier::WAY_MODIFY_KEY]);
      CPPUNIT_ASSERT_EQUAL(63L, changesetStats[OsmApiDbSqlChangesetApplier::WAY_DELETE_KEY]);
      CPPUNIT_ASSERT_EQUAL(2L, changesetStats[OsmApiDbSqlChangesetApplier::RELATION_CREATE_KEY]);
      CPPUNIT_ASSERT_EQUAL(2L, changesetStats[OsmApiDbSqlChangesetApplier::RELATION_MODIFY_KEY]);
      CPPUNIT_ASSERT_EQUAL(1L, changesetStats[OsmApiDbSqlChangesetApplier::RELATION_DELETE_KEY]);

      CPPUNIT_ASSERT_EQUAL(0, uut.getOutputMetrics().getNumOrphanedNodesInOutput());
      CPPUNIT_ASSERT_EQUAL(0, uut.getOutputMetrics().getNumDisconnectedWaysInOutput());
      CPPUNIT_ASSERT_EQUAL(0, uut.getOutputMetrics().getNumEmptyWaysInOutput());
      CPPUNIT_ASSERT_EQUAL(0, uut.getOutputMetrics().getNumDuplicateElementPairsInOutput());

      HOOT_FILE_EQUALS(_inputPath + "/" + outFile, outFull);
    }
  }

  void badPolyIdSync2Test()
  {
    // part of github 4297 - The longer surface parking poly in the output should match that in the
    // replacement data. Before this fix, skipped element ID synchronizations were causing the lower
    // right section to be truncated.

    _testName = "badPolyIdSync2Test";
    _prepInput(
      _inputPath + "/" + _testName + "-Input1.osm",
      _inputPath + "/" + _testName + "-Input2.osm",
      "");
    conf().set(
      ConfigOptions::getDebugMapsFilenameKey(), _outputPath + "/" + _testName + "-debug.osm");

    ChangesetTaskGridReplacer uut;
    uut.setChangesetsOutputDir(_outputPath);
    const QString outFile = _testName + "-out.osm";
    const QString outFull = _outputPath + "/" + outFile;
    uut.setJobName(_testName);
    uut.setWriteFinalOutput(outFull);
    uut.setOriginalDataSize(_originalDataSize);
    uut.setTagQualityIssues(true);
    uut.replace(
      DATA_TO_REPLACE_URL,
      _replacementDataUrl,
      UniformTaskGridGenerator(
        "-115.2822,36.2226,-115.2779,36.2261", 1,
        _outputPath + "/" + _testName + "-" + "taskGridBounds.osm")
        .generateTaskGrid());

    const QMap<QString, long> changesetStats = uut.getChangesetStats();
    if (DISPLAY_METRICS_ONLY)
    {
      LOG_WARN(_testName + ": " << changesetStats);
      LOG_VARW(uut.getOutputMetrics().getNumOrphanedNodesInOutput());
      LOG_VARW(uut.getOutputMetrics().getNumDisconnectedWaysInOutput());
      LOG_VARW(uut.getOutputMetrics().getNumEmptyWaysInOutput());
      LOG_VARW(uut.getOutputMetrics().getNumDuplicateElementPairsInOutput());
    }
    else
    {
      CPPUNIT_ASSERT_EQUAL(2533L, changesetStats[OsmApiDbSqlChangesetApplier::NODE_CREATE_KEY]);
      CPPUNIT_ASSERT_EQUAL(0L, changesetStats[OsmApiDbSqlChangesetApplier::NODE_MODIFY_KEY]);
      CPPUNIT_ASSERT_EQUAL(184L, changesetStats[OsmApiDbSqlChangesetApplier::NODE_DELETE_KEY]);
      CPPUNIT_ASSERT_EQUAL(66L, changesetStats[OsmApiDbSqlChangesetApplier::WAY_CREATE_KEY]);
      CPPUNIT_ASSERT_EQUAL(0L, changesetStats[OsmApiDbSqlChangesetApplier::WAY_MODIFY_KEY]);
      CPPUNIT_ASSERT_EQUAL(26L, changesetStats[OsmApiDbSqlChangesetApplier::WAY_DELETE_KEY]);
      CPPUNIT_ASSERT_EQUAL(3L, changesetStats[OsmApiDbSqlChangesetApplier::RELATION_CREATE_KEY]);
      CPPUNIT_ASSERT_EQUAL(0L, changesetStats[OsmApiDbSqlChangesetApplier::RELATION_MODIFY_KEY]);
      CPPUNIT_ASSERT_EQUAL(1L, changesetStats[OsmApiDbSqlChangesetApplier::RELATION_DELETE_KEY]);

      CPPUNIT_ASSERT_EQUAL(0, uut.getOutputMetrics().getNumOrphanedNodesInOutput());
      CPPUNIT_ASSERT_EQUAL(0, uut.getOutputMetrics().getNumDisconnectedWaysInOutput());
      CPPUNIT_ASSERT_EQUAL(0, uut.getOutputMetrics().getNumEmptyWaysInOutput());
      CPPUNIT_ASSERT_EQUAL(0, uut.getOutputMetrics().getNumDuplicateElementPairsInOutput());

      HOOT_FILE_EQUALS(_inputPath + "/" + outFile, outFull);
    }
  }

  void differingTypes1Test()
  {
    // This tests replacement changeset generation where the reference map includes feature types
    // not found in the secondary map. In that respect it is similar to
    // ServiceChangesetReplacementSecFilteredToEmptyTest, except the feature types involved are
    // different.

    _testName = "differingTypes1Test";
    _prepInput(
      _inputPath + "/" + _testName + "-Input1.osm",
      _inputPath + "/" + _testName + "-Input2.osm",
      "");
    conf().set(
      ConfigOptions::getDebugMapsFilenameKey(), _outputPath + "/" + _testName + "-debug.osm");

    ChangesetTaskGridReplacer uut;
    uut.setChangesetsOutputDir(_outputPath);
    const QString outFile = _testName + "-out.osm";
    const QString outFull = _outputPath + "/" + outFile;
    uut.setJobName(_testName);
    uut.setWriteFinalOutput(outFull);
    uut.setOriginalDataSize(_originalDataSize);
    uut.setTagQualityIssues(true);
    const QString taskGridFileName = _testName + "-" + "taskGridBounds.osm";
    conf().set(ConfigOptions::getSnapUnconnectedWaysExistingWayNodeToleranceKey(), 5.0);
    conf().set(ConfigOptions::getSnapUnconnectedWaysSnapToleranceKey(), 5.0);

    uut.replace(
      DATA_TO_REPLACE_URL,
      _replacementDataUrl,
      BoundsStringTaskGridGenerator(
        "11.361053,8.507705,11.363033,8.509146", _outputPath + "/" + taskGridFileName)
        .generateTaskGrid());

    const QMap<QString, long> changesetStats = uut.getChangesetStats();
    if (DISPLAY_METRICS_ONLY)
    {
      LOG_WARN(_testName + ": " << changesetStats);
      LOG_VARW(uut.getOutputMetrics().getNumOrphanedNodesInOutput());
      LOG_VARW(uut.getOutputMetrics().getNumDisconnectedWaysInOutput());
      LOG_VARW(uut.getOutputMetrics().getNumEmptyWaysInOutput());
      LOG_VARW(uut.getOutputMetrics().getNumDuplicateElementPairsInOutput());
    }
    else
    {
      CPPUNIT_ASSERT_EQUAL(1L, changesetStats[OsmApiDbSqlChangesetApplier::NODE_CREATE_KEY]);
      CPPUNIT_ASSERT_EQUAL(0L, changesetStats[OsmApiDbSqlChangesetApplier::NODE_MODIFY_KEY]);
      CPPUNIT_ASSERT_EQUAL(10L, changesetStats[OsmApiDbSqlChangesetApplier::NODE_DELETE_KEY]);
      CPPUNIT_ASSERT_EQUAL(1L, changesetStats[OsmApiDbSqlChangesetApplier::WAY_CREATE_KEY]);
      CPPUNIT_ASSERT_EQUAL(0L, changesetStats[OsmApiDbSqlChangesetApplier::WAY_MODIFY_KEY]);
      CPPUNIT_ASSERT_EQUAL(4L, changesetStats[OsmApiDbSqlChangesetApplier::WAY_DELETE_KEY]);
      CPPUNIT_ASSERT_EQUAL(0L, changesetStats[OsmApiDbSqlChangesetApplier::RELATION_CREATE_KEY]);
      CPPUNIT_ASSERT_EQUAL(0L, changesetStats[OsmApiDbSqlChangesetApplier::RELATION_MODIFY_KEY]);
      CPPUNIT_ASSERT_EQUAL(0L, changesetStats[OsmApiDbSqlChangesetApplier::RELATION_DELETE_KEY]);

      CPPUNIT_ASSERT_EQUAL(0, uut.getOutputMetrics().getNumOrphanedNodesInOutput());
      CPPUNIT_ASSERT_EQUAL(0, uut.getOutputMetrics().getNumDisconnectedWaysInOutput());
      CPPUNIT_ASSERT_EQUAL(0, uut.getOutputMetrics().getNumEmptyWaysInOutput());
      CPPUNIT_ASSERT_EQUAL(0, uut.getOutputMetrics().getNumDuplicateElementPairsInOutput());

      HOOT_FILE_EQUALS(_inputPath + "/" + outFile, outFull);
    }
  }

  void droppedNodes1Test()
  {
    // part of github 4228 - None of the highway=street_lamp features near Edmundo Escobedo Sr.
    // Middle School (Westmost building) should get dropped in the output.

    _testName = "droppedNodes1Test";
    _prepInput(
      _inputPath + "/" + _testName + "-Input1.osm",
      _inputPath + "/" + _testName + "-Input2.osm",
      "");
    conf().set(
      ConfigOptions::getDebugMapsFilenameKey(), _outputPath + "/" + _testName + "-debug.osm");

    ChangesetTaskGridReplacer uut;
    uut.setChangesetsOutputDir(_outputPath);
    const QString outFile = _testName + "-out.osm";
    const QString outFull = _outputPath + "/" + outFile;
    uut.setJobName(_testName);
    uut.setWriteFinalOutput(outFull);
    uut.setOriginalDataSize(_originalDataSize);
    uut.setTagQualityIssues(true);
    uut.replace(
      DATA_TO_REPLACE_URL,
      _replacementDataUrl,
      UniformTaskGridGenerator(
        "-115.3274,36.2640,-115.3106,36.2874", 2,
        _outputPath + "/" + _testName + "-" + "taskGridBounds.osm")
        .generateTaskGrid());

    const QMap<QString, long> changesetStats = uut.getChangesetStats();
    if (DISPLAY_METRICS_ONLY)
    {
      LOG_WARN(_testName + ": " << changesetStats);
      LOG_VARW(uut.getOutputMetrics().getNumOrphanedNodesInOutput());
      LOG_VARW(uut.getOutputMetrics().getNumDisconnectedWaysInOutput());
      LOG_VARW(uut.getOutputMetrics().getNumEmptyWaysInOutput());
      LOG_VARW(uut.getOutputMetrics().getNumDuplicateElementPairsInOutput());
    }
    else
    {
      CPPUNIT_ASSERT_EQUAL(2230L, changesetStats[OsmApiDbSqlChangesetApplier::NODE_CREATE_KEY]);
      CPPUNIT_ASSERT_EQUAL(0L, changesetStats[OsmApiDbSqlChangesetApplier::NODE_MODIFY_KEY]);
      CPPUNIT_ASSERT_EQUAL(742L, changesetStats[OsmApiDbSqlChangesetApplier::NODE_DELETE_KEY]);
      CPPUNIT_ASSERT_EQUAL(603L, changesetStats[OsmApiDbSqlChangesetApplier::WAY_CREATE_KEY]);
      CPPUNIT_ASSERT_EQUAL(0L, changesetStats[OsmApiDbSqlChangesetApplier::WAY_MODIFY_KEY]);
      CPPUNIT_ASSERT_EQUAL(255L, changesetStats[OsmApiDbSqlChangesetApplier::WAY_DELETE_KEY]);
      CPPUNIT_ASSERT_EQUAL(10L, changesetStats[OsmApiDbSqlChangesetApplier::RELATION_CREATE_KEY]);
      CPPUNIT_ASSERT_EQUAL(4L, changesetStats[OsmApiDbSqlChangesetApplier::RELATION_MODIFY_KEY]);
      CPPUNIT_ASSERT_EQUAL(0L, changesetStats[OsmApiDbSqlChangesetApplier::RELATION_DELETE_KEY]);

      CPPUNIT_ASSERT_EQUAL(0, uut.getOutputMetrics().getNumOrphanedNodesInOutput());
      CPPUNIT_ASSERT_EQUAL(0, uut.getOutputMetrics().getNumDisconnectedWaysInOutput());
      CPPUNIT_ASSERT_EQUAL(0, uut.getOutputMetrics().getNumEmptyWaysInOutput());
      CPPUNIT_ASSERT_EQUAL(0, uut.getOutputMetrics().getNumDuplicateElementPairsInOutput());

      HOOT_FILE_EQUALS(_inputPath + "/" + outFile, outFull);
    }
  }

  void droppedPointPolyRelationMembers1Test()
  {
    // part of github 4228 - The POI, Desert Shores, belongs in a relation with a landuse poly.
    // Neither should be dropped from the output due to being in a relation with members of mixed
    // geometry types.

    // This is needed to suppress some ElementToGeometryConverter warning messages that should
    // eventually be looked into.
    DisableLog dl;

    _testName = "droppedPointPolyRelationMembers1Test";
    _prepInput(
      _inputPath + "/" + _testName + "-Input1.osm",
      _inputPath + "/" + _testName + "-Input2.osm",
      "");
    conf().set(
      ConfigOptions::getDebugMapsFilenameKey(), _outputPath + "/" + _testName + "-debug.osm");

    ChangesetTaskGridReplacer uut;
    uut.setChangesetsOutputDir(_outputPath);
    const QString outFile = _testName + "-out.osm";
    const QString outFull = _outputPath + "/" + outFile;
    uut.setJobName(_testName);
    uut.setWriteFinalOutput(outFull);
    uut.setOriginalDataSize(_originalDataSize);
    uut.setTagQualityIssues(true);
    uut.replace(
      DATA_TO_REPLACE_URL,
      _replacementDataUrl,
      UniformTaskGridGenerator(
        "-115.3064,36.1867,-115.2136,36.2498", 2,
        _outputPath + "/" + _testName + "-" + "taskGridBounds.osm")
        .generateTaskGrid());

    const QMap<QString, long> changesetStats = uut.getChangesetStats();
    if (DISPLAY_METRICS_ONLY)
    {
      LOG_WARN(_testName + ": " << changesetStats);
      LOG_VARW(uut.getOutputMetrics().getNumOrphanedNodesInOutput());
      LOG_VARW(uut.getOutputMetrics().getNumDisconnectedWaysInOutput());
      LOG_VARW(uut.getOutputMetrics().getNumEmptyWaysInOutput());
      LOG_VARW(uut.getOutputMetrics().getNumDuplicateElementPairsInOutput());
    }
    else
    {
      CPPUNIT_ASSERT_EQUAL(18456L, changesetStats[OsmApiDbSqlChangesetApplier::NODE_CREATE_KEY]);
      CPPUNIT_ASSERT_EQUAL(0L, changesetStats[OsmApiDbSqlChangesetApplier::NODE_MODIFY_KEY]);
      CPPUNIT_ASSERT_EQUAL(1525L, changesetStats[OsmApiDbSqlChangesetApplier::NODE_DELETE_KEY]);
      CPPUNIT_ASSERT_EQUAL(2682L, changesetStats[OsmApiDbSqlChangesetApplier::WAY_CREATE_KEY]);
      CPPUNIT_ASSERT_EQUAL(0L, changesetStats[OsmApiDbSqlChangesetApplier::WAY_MODIFY_KEY]);
      CPPUNIT_ASSERT_EQUAL(343L, changesetStats[OsmApiDbSqlChangesetApplier::WAY_DELETE_KEY]);
      CPPUNIT_ASSERT_EQUAL(279L, changesetStats[OsmApiDbSqlChangesetApplier::RELATION_CREATE_KEY]);
      CPPUNIT_ASSERT_EQUAL(27L, changesetStats[OsmApiDbSqlChangesetApplier::RELATION_MODIFY_KEY]);
      CPPUNIT_ASSERT_EQUAL(46L, changesetStats[OsmApiDbSqlChangesetApplier::RELATION_DELETE_KEY]);

      CPPUNIT_ASSERT_EQUAL(0, uut.getOutputMetrics().getNumOrphanedNodesInOutput());
      CPPUNIT_ASSERT_EQUAL(0, uut.getOutputMetrics().getNumDisconnectedWaysInOutput());
      CPPUNIT_ASSERT_EQUAL(0, uut.getOutputMetrics().getNumEmptyWaysInOutput());
      CPPUNIT_ASSERT_EQUAL(8, uut.getOutputMetrics().getNumDuplicateElementPairsInOutput());

      HOOT_FILE_EQUALS(_inputPath + "/" + outFile, outFull);
    }
  }

  void orphanedNodes1Test()
  {
    // VGI 1622 - This tests that orphaned nodes are not left behind after two adjoining, successive
    // grid cell replacements with some data overlapping due to using the lenient bounds
    // interpretation. The road "Hot Springs Drive" should remain snapped to connecting roads and no
    // orphaned nodes should be hidden underneath it. Other roads to check in the are include
    // "Santa Barbara Drive".

    _testName = "orphanedNodes1Test";
    _prepInput(
      _inputPath + "/" + _testName + "-Input1.osm",
      _inputPath + "/" + _testName + "-Input2.osm",
      "");
    conf().set(
      ConfigOptions::getDebugMapsFilenameKey(), _outputPath + "/" + _testName + "-debug.osm");

    ChangesetTaskGridReplacer uut;
    uut.setChangesetsOutputDir(_outputPath);
    const QString outFile = _testName + "-out.osm";
    const QString outFull = _outputPath + "/" + outFile;
    uut.setJobName(_testName);
    uut.setWriteFinalOutput(outFull);
    uut.setOriginalDataSize(_originalDataSize);
    uut.setTagQualityIssues(true);
    uut.replace(
      DATA_TO_REPLACE_URL,
      _replacementDataUrl,
      BoundsFileTaskGridGenerator(
        QStringList(_inputPath + "/orphanedNodes1Test-task-grid.osm")).generateTaskGrid());

    const QMap<QString, long> changesetStats = uut.getChangesetStats();
    if (DISPLAY_METRICS_ONLY)
    {
      LOG_WARN(_testName + ": " << changesetStats);
      LOG_VARW(uut.getOutputMetrics().getNumOrphanedNodesInOutput());
      LOG_VARW(uut.getOutputMetrics().getNumDisconnectedWaysInOutput());
      LOG_VARW(uut.getOutputMetrics().getNumEmptyWaysInOutput());
      LOG_VARW(uut.getOutputMetrics().getNumDuplicateElementPairsInOutput());
    }
    else
    {
      CPPUNIT_ASSERT_EQUAL(732L, changesetStats[OsmApiDbSqlChangesetApplier::NODE_CREATE_KEY]);
      CPPUNIT_ASSERT_EQUAL(0L, changesetStats[OsmApiDbSqlChangesetApplier::NODE_MODIFY_KEY]);
      CPPUNIT_ASSERT_EQUAL(106L, changesetStats[OsmApiDbSqlChangesetApplier::NODE_DELETE_KEY]);
      CPPUNIT_ASSERT_EQUAL(94L, changesetStats[OsmApiDbSqlChangesetApplier::WAY_CREATE_KEY]);
      CPPUNIT_ASSERT_EQUAL(0L, changesetStats[OsmApiDbSqlChangesetApplier::WAY_MODIFY_KEY]);
      CPPUNIT_ASSERT_EQUAL(75L, changesetStats[OsmApiDbSqlChangesetApplier::WAY_DELETE_KEY]);
      CPPUNIT_ASSERT_EQUAL(0L, changesetStats[OsmApiDbSqlChangesetApplier::RELATION_CREATE_KEY]);
      CPPUNIT_ASSERT_EQUAL(0L, changesetStats[OsmApiDbSqlChangesetApplier::RELATION_MODIFY_KEY]);
      CPPUNIT_ASSERT_EQUAL(0L, changesetStats[OsmApiDbSqlChangesetApplier::RELATION_DELETE_KEY]);

      CPPUNIT_ASSERT_EQUAL(0, uut.getOutputMetrics().getNumOrphanedNodesInOutput());
      CPPUNIT_ASSERT_EQUAL(0, uut.getOutputMetrics().getNumDisconnectedWaysInOutput());
      CPPUNIT_ASSERT_EQUAL(0, uut.getOutputMetrics().getNumEmptyWaysInOutput());
      CPPUNIT_ASSERT_EQUAL(28, uut.getOutputMetrics().getNumDuplicateElementPairsInOutput());

      HOOT_FILE_EQUALS(_inputPath + "/" + outFile, outFull);
    }
  }

  void orphanedNodes2Test()
  {
    // github 4216; similar to orphanedNodes1Test - There should be no orphaned nodes in the
    // output.

    _testName = "orphanedNodes2Test";
    _prepInput(
      _inputPath + "/" + _testName + "-Input1.osm",
      _inputPath + "/" + _testName + "-Input2.osm",
      "");
    conf().set(
      ConfigOptions::getDebugMapsFilenameKey(), _outputPath + "/" + _testName + "-debug.osm");

    ChangesetTaskGridReplacer uut;
    uut.setChangesetsOutputDir(_outputPath);
    const QString outFile = _testName + "-out.osm";
    const QString outFull = _outputPath + "/" + outFile;
    uut.setJobName(_testName);
    uut.setWriteFinalOutput(outFull);
    uut.setOriginalDataSize(_originalDataSize);
    uut.setTagQualityIssues(true);
    const QString taskGridFileName = _testName + "-" + "taskGridBounds.osm";

    // to suppress a SpatialIndexer warning that should be looked into at some point
    Log::WarningLevel logLevel = Log::getInstance().getLevel();
    Log::getInstance().setLevel(Log::Error);

    uut.replace(
      DATA_TO_REPLACE_URL,
      _replacementDataUrl,
      UniformTaskGridGenerator(
        "-115.0793,36.1832,-115.0610,36.1986", 2, _outputPath + "/" + taskGridFileName)
        .generateTaskGrid());

    Log::getInstance().setLevel(logLevel);

    const QMap<QString, long> changesetStats = uut.getChangesetStats();
    if (DISPLAY_METRICS_ONLY)
    {
      LOG_WARN(_testName + ": " << changesetStats);
      LOG_VARW(uut.getOutputMetrics().getNumOrphanedNodesInOutput());
      LOG_VARW(uut.getOutputMetrics().getNumDisconnectedWaysInOutput());
      LOG_VARW(uut.getOutputMetrics().getNumEmptyWaysInOutput());
      LOG_VARW(uut.getOutputMetrics().getNumDuplicateElementPairsInOutput());
    }
    else
    {
      CPPUNIT_ASSERT_EQUAL(18416L, changesetStats[OsmApiDbSqlChangesetApplier::NODE_CREATE_KEY]);
      CPPUNIT_ASSERT_EQUAL(0L, changesetStats[OsmApiDbSqlChangesetApplier::NODE_MODIFY_KEY]);
      CPPUNIT_ASSERT_EQUAL(608L, changesetStats[OsmApiDbSqlChangesetApplier::NODE_DELETE_KEY]);
      CPPUNIT_ASSERT_EQUAL(2852L, changesetStats[OsmApiDbSqlChangesetApplier::WAY_CREATE_KEY]);
      CPPUNIT_ASSERT_EQUAL(0L, changesetStats[OsmApiDbSqlChangesetApplier::WAY_MODIFY_KEY]);
      CPPUNIT_ASSERT_EQUAL(479L, changesetStats[OsmApiDbSqlChangesetApplier::WAY_DELETE_KEY]);
      CPPUNIT_ASSERT_EQUAL(40L, changesetStats[OsmApiDbSqlChangesetApplier::RELATION_CREATE_KEY]);
      CPPUNIT_ASSERT_EQUAL(17L, changesetStats[OsmApiDbSqlChangesetApplier::RELATION_MODIFY_KEY]);
      CPPUNIT_ASSERT_EQUAL(12L, changesetStats[OsmApiDbSqlChangesetApplier::RELATION_DELETE_KEY]);

      CPPUNIT_ASSERT_EQUAL(0, uut.getOutputMetrics().getNumOrphanedNodesInOutput());
      CPPUNIT_ASSERT_EQUAL(0, uut.getOutputMetrics().getNumDisconnectedWaysInOutput());
      CPPUNIT_ASSERT_EQUAL(0, uut.getOutputMetrics().getNumEmptyWaysInOutput());
      CPPUNIT_ASSERT_EQUAL(1, uut.getOutputMetrics().getNumDuplicateElementPairsInOutput());

      HOOT_FILE_EQUALS(_inputPath + "/" + outFile, outFull);
    }
  }

  void outOfSpecMixedRelations1Test()
  {
    /*
     * This test ensures that features belonging to relations that are not part of the OSM spec are
     * not dropped. There also shouldn't be any duplicated features in the output which can happen
     * when features belonging to relations are separated by geometry during processing.

       This also tests handling of relations whose members are missing from the input. The
       secondary input file has one way relation member commented out (unfortunately this makes it
       unreadable in JOSM, so you'll have to temporarily uncomment it to view the input files
       there). Any relations in the output owning the commented out member should be tagged with
       the 'hoot::missing_child' tag after the changeset is applied.

       The main one to check here is the large unlabeled green way that is inside the shop=mall way
       named "The Forum Shops at Caesars". That way may get unncessarily duplicated due to
       belonging to multiple relations. Drag it slightly to ensure a duplicate isn't hidden
       underneath of it. Also, make sure it is still connected to all the shop polys it should be.

       Output issues:
       1. There is a poorly snapped untyped way belonging to a multilinestring relation between
          "LVB Burger" and "Rhumber".
      */

    // This is needed to suppress some missing element warning messages.
    DisableLog dl;

    _testName = "outOfSpecMixedRelations1Test";
    _prepInput(
      _inputPath + "/" + _testName + "-Input1.osm",
      _inputPath + "/" + _testName + "-Input2.osm",
      "");
    conf().set(
      ConfigOptions::getDebugMapsFilenameKey(), _outputPath + "/" + _testName + "-debug.osm");

    ChangesetTaskGridReplacer uut;
    uut.setChangesetsOutputDir(_outputPath);
    const QString outFile = _testName + "-out.osm";
    const QString outFull = _outputPath + "/" + outFile;
    uut.setJobName(_testName);
    uut.setWriteFinalOutput(outFull);
    uut.setOriginalDataSize(_originalDataSize);
    uut.setTagQualityIssues(true);
    const QString taskGridFileName = _testName + "-" + "taskGridBounds.osm";
    conf().set(ConfigOptions::getSnapUnconnectedWaysExistingWayNodeToleranceKey(), 5.0);
    conf().set(ConfigOptions::getSnapUnconnectedWaysSnapToleranceKey(), 0.5);

    uut.replace(
      DATA_TO_REPLACE_URL,
      _replacementDataUrl,
      BoundsStringTaskGridGenerator(
        "-115.184767,36.031262,-115.048556,36.14796", _outputPath + "/" + taskGridFileName)
        .generateTaskGrid());

    const QMap<QString, long> changesetStats = uut.getChangesetStats();
    if (DISPLAY_METRICS_ONLY)
    {
      LOG_WARN(_testName + ": " << changesetStats);
      LOG_VARW(uut.getOutputMetrics().getNumOrphanedNodesInOutput());
      LOG_VARW(uut.getOutputMetrics().getNumDisconnectedWaysInOutput());
      LOG_VARW(uut.getOutputMetrics().getNumEmptyWaysInOutput());
      LOG_VARW(uut.getOutputMetrics().getNumDuplicateElementPairsInOutput());
    }
    else
    {
      CPPUNIT_ASSERT_EQUAL(434L, changesetStats[OsmApiDbSqlChangesetApplier::NODE_CREATE_KEY]);
      CPPUNIT_ASSERT_EQUAL(0L, changesetStats[OsmApiDbSqlChangesetApplier::NODE_MODIFY_KEY]);
      CPPUNIT_ASSERT_EQUAL(246L, changesetStats[OsmApiDbSqlChangesetApplier::NODE_DELETE_KEY]);
      CPPUNIT_ASSERT_EQUAL(185L, changesetStats[OsmApiDbSqlChangesetApplier::WAY_CREATE_KEY]);
      CPPUNIT_ASSERT_EQUAL(0L, changesetStats[OsmApiDbSqlChangesetApplier::WAY_MODIFY_KEY]);
      CPPUNIT_ASSERT_EQUAL(136L, changesetStats[OsmApiDbSqlChangesetApplier::WAY_DELETE_KEY]);
      CPPUNIT_ASSERT_EQUAL(25L, changesetStats[OsmApiDbSqlChangesetApplier::RELATION_CREATE_KEY]);
      CPPUNIT_ASSERT_EQUAL(0L, changesetStats[OsmApiDbSqlChangesetApplier::RELATION_MODIFY_KEY]);
      CPPUNIT_ASSERT_EQUAL(9L, changesetStats[OsmApiDbSqlChangesetApplier::RELATION_DELETE_KEY]);

      CPPUNIT_ASSERT_EQUAL(0, uut.getOutputMetrics().getNumOrphanedNodesInOutput());
      CPPUNIT_ASSERT_EQUAL(0, uut.getOutputMetrics().getNumDisconnectedWaysInOutput());
      CPPUNIT_ASSERT_EQUAL(0, uut.getOutputMetrics().getNumEmptyWaysInOutput());
      CPPUNIT_ASSERT_EQUAL(1, uut.getOutputMetrics().getNumDuplicateElementPairsInOutput());

      HOOT_FILE_EQUALS(_inputPath + "/" + outFile, outFull);
    }
  }

  void poi1Test()
  {
    // This tests replacement changeset generation with POIs being replaced over the entire AOI and
    // a strict interpretation of the AOI boundaries.

    _testName = "poi1Test";
    _prepInput(
      "test-files/cmd/glacial/PoiPolygonConflateStandaloneTest/PoiPolygon1.osm",
      "test-files/cmd/glacial/PoiPolygonConflateStandaloneTest/PoiPolygon2.osm",
      "-122.43384,37.76069,-122.42742,37.76869",
      _outputPath);
    conf().set(
      ConfigOptions::getDebugMapsFilenameKey(), _outputPath + "/" + _testName + "-debug.osm");

    ChangesetTaskGridReplacer uut;
    uut.setChangesetsOutputDir(_outputPath);
    const QString outFile = _testName + "-out.osm";
    const QString outFull = _outputPath + "/" + outFile;
    uut.setJobName(_testName);
    uut.setWriteFinalOutput(outFull);
    uut.setOriginalDataSize(_originalDataSize);
    uut.setTagQualityIssues(true);
    const QString taskGridFileName = _testName + "-" + "taskGridBounds.osm";
    conf().set(ConfigOptions::getSnapUnconnectedWaysExistingWayNodeToleranceKey(), 5.0);
    conf().set(ConfigOptions::getSnapUnconnectedWaysSnapToleranceKey(), 0.5);

    uut.replace(
      DATA_TO_REPLACE_URL,
      _replacementDataUrl,
      BoundsStringTaskGridGenerator(
        "-122.43204,37.7628,-122.4303457,37.76437", _outputPath + "/" + taskGridFileName)
        .generateTaskGrid());

    const QMap<QString, long> changesetStats = uut.getChangesetStats();
    if (DISPLAY_METRICS_ONLY)
    {
      LOG_WARN(_testName + ": " << changesetStats);
      LOG_VARW(uut.getOutputMetrics().getNumOrphanedNodesInOutput());
      LOG_VARW(uut.getOutputMetrics().getNumDisconnectedWaysInOutput());
      LOG_VARW(uut.getOutputMetrics().getNumEmptyWaysInOutput());
      LOG_VARW(uut.getOutputMetrics().getNumDuplicateElementPairsInOutput());
    }
    else
    {
      CPPUNIT_ASSERT_EQUAL(4L, changesetStats[OsmApiDbSqlChangesetApplier::NODE_CREATE_KEY]);
      CPPUNIT_ASSERT_EQUAL(0L, changesetStats[OsmApiDbSqlChangesetApplier::NODE_MODIFY_KEY]);
      CPPUNIT_ASSERT_EQUAL(6L, changesetStats[OsmApiDbSqlChangesetApplier::NODE_DELETE_KEY]);
      CPPUNIT_ASSERT_EQUAL(0L, changesetStats[OsmApiDbSqlChangesetApplier::WAY_CREATE_KEY]);
      CPPUNIT_ASSERT_EQUAL(0L, changesetStats[OsmApiDbSqlChangesetApplier::WAY_MODIFY_KEY]);
      CPPUNIT_ASSERT_EQUAL(0L, changesetStats[OsmApiDbSqlChangesetApplier::WAY_DELETE_KEY]);
      CPPUNIT_ASSERT_EQUAL(0L, changesetStats[OsmApiDbSqlChangesetApplier::RELATION_CREATE_KEY]);
      CPPUNIT_ASSERT_EQUAL(0L, changesetStats[OsmApiDbSqlChangesetApplier::RELATION_MODIFY_KEY]);
      CPPUNIT_ASSERT_EQUAL(0L, changesetStats[OsmApiDbSqlChangesetApplier::RELATION_DELETE_KEY]);

      CPPUNIT_ASSERT_EQUAL(0, uut.getOutputMetrics().getNumOrphanedNodesInOutput());
      CPPUNIT_ASSERT_EQUAL(0, uut.getOutputMetrics().getNumDisconnectedWaysInOutput());
      CPPUNIT_ASSERT_EQUAL(0, uut.getOutputMetrics().getNumEmptyWaysInOutput());
      CPPUNIT_ASSERT_EQUAL(2, uut.getOutputMetrics().getNumDuplicateElementPairsInOutput());

      HOOT_FILE_EQUALS(_inputPath + "/" + outFile, outFull);
    }
  }

  void refFilteredToEmpty1Test()
  {
    // This tests replacement changeset generation with inputs that result in an empty ref map after
    // filtering when generating the points changeset.

    _testName = "refFilteredToEmpty1Test";
    _prepInput(
      _inputPath + "/" + _testName + "-Input1.osm",
      _inputPath + "/" + _testName + "-Input2.osm",
      "");
    conf().set(
      ConfigOptions::getDebugMapsFilenameKey(), _outputPath + "/" + _testName + "-debug.osm");

    ChangesetTaskGridReplacer uut;
    uut.setChangesetsOutputDir(_outputPath);
    const QString outFile = _testName + "-out.osm";
    const QString outFull = _outputPath + "/" + outFile;
    uut.setJobName(_testName);
    uut.setWriteFinalOutput(outFull);
    uut.setOriginalDataSize(_originalDataSize);
    uut.setTagQualityIssues(true);
    const QString taskGridFileName = _testName + "-" + "taskGridBounds.osm";
    conf().set(ConfigOptions::getSnapUnconnectedWaysExistingWayNodeToleranceKey(), 5.0);
    conf().set(ConfigOptions::getSnapUnconnectedWaysSnapToleranceKey(), 5.0);

    uut.replace(
      DATA_TO_REPLACE_URL,
      _replacementDataUrl,
      BoundsStringTaskGridGenerator(
        "-77.114735,38.884001,-77.112229,38.885158", _outputPath + "/" + taskGridFileName)
        .generateTaskGrid());

    const QMap<QString, long> changesetStats = uut.getChangesetStats();
    if (DISPLAY_METRICS_ONLY)
    {
      LOG_WARN(_testName + ": " << changesetStats);
      LOG_VARW(uut.getOutputMetrics().getNumOrphanedNodesInOutput());
      LOG_VARW(uut.getOutputMetrics().getNumDisconnectedWaysInOutput());
      LOG_VARW(uut.getOutputMetrics().getNumEmptyWaysInOutput());
      LOG_VARW(uut.getOutputMetrics().getNumDuplicateElementPairsInOutput());
    }
    else
    {
      CPPUNIT_ASSERT_EQUAL(544L, changesetStats[OsmApiDbSqlChangesetApplier::NODE_CREATE_KEY]);
      CPPUNIT_ASSERT_EQUAL(0L, changesetStats[OsmApiDbSqlChangesetApplier::NODE_MODIFY_KEY]);
      CPPUNIT_ASSERT_EQUAL(4L, changesetStats[OsmApiDbSqlChangesetApplier::NODE_DELETE_KEY]);
      CPPUNIT_ASSERT_EQUAL(143L, changesetStats[OsmApiDbSqlChangesetApplier::WAY_CREATE_KEY]);
      CPPUNIT_ASSERT_EQUAL(0L, changesetStats[OsmApiDbSqlChangesetApplier::WAY_MODIFY_KEY]);
      CPPUNIT_ASSERT_EQUAL(22L, changesetStats[OsmApiDbSqlChangesetApplier::WAY_DELETE_KEY]);
      CPPUNIT_ASSERT_EQUAL(0L, changesetStats[OsmApiDbSqlChangesetApplier::RELATION_CREATE_KEY]);
      CPPUNIT_ASSERT_EQUAL(0L, changesetStats[OsmApiDbSqlChangesetApplier::RELATION_MODIFY_KEY]);
      CPPUNIT_ASSERT_EQUAL(0L, changesetStats[OsmApiDbSqlChangesetApplier::RELATION_DELETE_KEY]);

      CPPUNIT_ASSERT_EQUAL(0, uut.getOutputMetrics().getNumOrphanedNodesInOutput());
      CPPUNIT_ASSERT_EQUAL(0, uut.getOutputMetrics().getNumDisconnectedWaysInOutput());
      CPPUNIT_ASSERT_EQUAL(0, uut.getOutputMetrics().getNumEmptyWaysInOutput());
      CPPUNIT_ASSERT_EQUAL(0, uut.getOutputMetrics().getNumDuplicateElementPairsInOutput());

      HOOT_FILE_EQUALS(_inputPath + "/" + outFile, outFull);
    }
  }

  void refSinglePoint1Test()
  {
    // This tests that alpha shape generation doesn't fail if the ref map gets filtered down to a
    // single POI.

    _testName = "refSinglePoint1Test";
    _prepInput(
      _inputPath + "/" + _testName + "-Input1.osm",
      _inputPath + "/" + _testName + "-Input2.osm",
      "");
    conf().set(
      ConfigOptions::getDebugMapsFilenameKey(), _outputPath + "/" + _testName + "-debug.osm");

    ChangesetTaskGridReplacer uut;
    uut.setChangesetsOutputDir(_outputPath);
    const QString outFile = _testName + "-out.osm";
    const QString outFull = _outputPath + "/" + outFile;
    uut.setJobName(_testName);
    uut.setWriteFinalOutput(outFull);
    uut.setOriginalDataSize(_originalDataSize);
    uut.setTagQualityIssues(true);
    const QString taskGridFileName = _testName + "-" + "taskGridBounds.osm";
    conf().set(ConfigOptions::getSnapUnconnectedWaysExistingWayNodeToleranceKey(), 5.0);
    conf().set(ConfigOptions::getSnapUnconnectedWaysSnapToleranceKey(), 5.0);

    uut.replace(
      DATA_TO_REPLACE_URL,
      _replacementDataUrl,
      BoundsStringTaskGridGenerator(
        "52.938359,32.727214,53.769359,32.853214", _outputPath + "/" + taskGridFileName)
        .generateTaskGrid());

    const QMap<QString, long> changesetStats = uut.getChangesetStats();
    if (DISPLAY_METRICS_ONLY)
    {
      LOG_WARN(_testName + ": " << changesetStats);
      LOG_VARW(uut.getOutputMetrics().getNumOrphanedNodesInOutput());
      LOG_VARW(uut.getOutputMetrics().getNumDisconnectedWaysInOutput());
      LOG_VARW(uut.getOutputMetrics().getNumEmptyWaysInOutput());
      LOG_VARW(uut.getOutputMetrics().getNumDuplicateElementPairsInOutput());
    }
    else
    {
      CPPUNIT_ASSERT_EQUAL(2187L, changesetStats[OsmApiDbSqlChangesetApplier::NODE_CREATE_KEY]);
      CPPUNIT_ASSERT_EQUAL(0L, changesetStats[OsmApiDbSqlChangesetApplier::NODE_MODIFY_KEY]);
      CPPUNIT_ASSERT_EQUAL(294L, changesetStats[OsmApiDbSqlChangesetApplier::NODE_DELETE_KEY]);
      CPPUNIT_ASSERT_EQUAL(494L, changesetStats[OsmApiDbSqlChangesetApplier::WAY_CREATE_KEY]);
      CPPUNIT_ASSERT_EQUAL(1L, changesetStats[OsmApiDbSqlChangesetApplier::WAY_MODIFY_KEY]);
      CPPUNIT_ASSERT_EQUAL(96L, changesetStats[OsmApiDbSqlChangesetApplier::WAY_DELETE_KEY]);
      CPPUNIT_ASSERT_EQUAL(0L, changesetStats[OsmApiDbSqlChangesetApplier::RELATION_CREATE_KEY]);
      CPPUNIT_ASSERT_EQUAL(0L, changesetStats[OsmApiDbSqlChangesetApplier::RELATION_MODIFY_KEY]);
      CPPUNIT_ASSERT_EQUAL(0L, changesetStats[OsmApiDbSqlChangesetApplier::RELATION_DELETE_KEY]);

      CPPUNIT_ASSERT_EQUAL(0, uut.getOutputMetrics().getNumOrphanedNodesInOutput());
      CPPUNIT_ASSERT_EQUAL(0, uut.getOutputMetrics().getNumDisconnectedWaysInOutput());
      CPPUNIT_ASSERT_EQUAL(0, uut.getOutputMetrics().getNumEmptyWaysInOutput());
      CPPUNIT_ASSERT_EQUAL(8, uut.getOutputMetrics().getNumDuplicateElementPairsInOutput());

      HOOT_FILE_EQUALS(_inputPath + "/" + outFile, outFull);
    }
  }

  void relationCrop1Test()
  {
    // Tests to make sure cropping of relations doesn't leave behind missing relation member
    // references.

    _testName = "relationCrop1Test";
    _prepInput(
      _inputPath + "/" + _testName + "-Input1.osm",
      _inputPath + "/" + _testName + "-Input2.osm",
      "");
    conf().set(
      ConfigOptions::getDebugMapsFilenameKey(), _outputPath + "/" + _testName + "-debug.osm");

    ChangesetTaskGridReplacer uut;
    uut.setChangesetsOutputDir(_outputPath);
    const QString outFile = _testName + "-out.osm";
    const QString outFull = _outputPath + "/" + outFile;
    uut.setJobName(_testName);
    uut.setWriteFinalOutput(outFull);
    uut.setOriginalDataSize(_originalDataSize);
    uut.setTagQualityIssues(true);
    const QString taskGridFileName = _testName + "-" + "taskGridBounds.osm";
    conf().set(ConfigOptions::getSnapUnconnectedWaysExistingWayNodeToleranceKey(), 5.0);
    conf().set(ConfigOptions::getSnapUnconnectedWaysSnapToleranceKey(), 0.5);

    uut.replace(
      DATA_TO_REPLACE_URL,
      _replacementDataUrl,
      BoundsStringTaskGridGenerator(
        "7.910156,5.003394,7.998047,5.090946", _outputPath + "/" + taskGridFileName)
        .generateTaskGrid());

    const QMap<QString, long> changesetStats = uut.getChangesetStats();
    if (DISPLAY_METRICS_ONLY)
    {
      LOG_WARN(_testName + ": " << changesetStats);
      LOG_VARW(uut.getOutputMetrics().getNumOrphanedNodesInOutput());
      LOG_VARW(uut.getOutputMetrics().getNumDisconnectedWaysInOutput());
      LOG_VARW(uut.getOutputMetrics().getNumEmptyWaysInOutput());
      LOG_VARW(uut.getOutputMetrics().getNumDuplicateElementPairsInOutput());
    }
    else
    {
      CPPUNIT_ASSERT_EQUAL(16001L, changesetStats[OsmApiDbSqlChangesetApplier::NODE_CREATE_KEY]);
      CPPUNIT_ASSERT_EQUAL(0L, changesetStats[OsmApiDbSqlChangesetApplier::NODE_MODIFY_KEY]);
      CPPUNIT_ASSERT_EQUAL(1333L, changesetStats[OsmApiDbSqlChangesetApplier::NODE_DELETE_KEY]);
      CPPUNIT_ASSERT_EQUAL(2450L, changesetStats[OsmApiDbSqlChangesetApplier::WAY_CREATE_KEY]);
      CPPUNIT_ASSERT_EQUAL(0L, changesetStats[OsmApiDbSqlChangesetApplier::WAY_MODIFY_KEY]);
      CPPUNIT_ASSERT_EQUAL(97L, changesetStats[OsmApiDbSqlChangesetApplier::WAY_DELETE_KEY]);
      CPPUNIT_ASSERT_EQUAL(25L, changesetStats[OsmApiDbSqlChangesetApplier::RELATION_CREATE_KEY]);
      CPPUNIT_ASSERT_EQUAL(0L, changesetStats[OsmApiDbSqlChangesetApplier::RELATION_MODIFY_KEY]);
      CPPUNIT_ASSERT_EQUAL(0L, changesetStats[OsmApiDbSqlChangesetApplier::RELATION_DELETE_KEY]);

      CPPUNIT_ASSERT_EQUAL(0, uut.getOutputMetrics().getNumOrphanedNodesInOutput());
      CPPUNIT_ASSERT_EQUAL(0, uut.getOutputMetrics().getNumDisconnectedWaysInOutput());
      CPPUNIT_ASSERT_EQUAL(0, uut.getOutputMetrics().getNumEmptyWaysInOutput());
      CPPUNIT_ASSERT_EQUAL(0, uut.getOutputMetrics().getNumDuplicateElementPairsInOutput());

      HOOT_FILE_EQUALS(_inputPath + "/" + outFile, outFull);
    }
  }

  void riverbank1Test()
  {
    // This tests replacement of water features.

    _testName = "riverbank1Test";
    _prepInput(
      _inputPath + "/" + _testName + "-Input1.osm",
      _inputPath + "/" + _testName + "-Input2.osm",
      "");
    conf().set(
      ConfigOptions::getDebugMapsFilenameKey(), _outputPath + "/" + _testName + "-debug.osm");

    ChangesetTaskGridReplacer uut;
    uut.setChangesetsOutputDir(_outputPath);
    const QString outFile = _testName + "-out.osm";
    const QString outFull = _outputPath + "/" + outFile;
    uut.setJobName(_testName);
    uut.setWriteFinalOutput(outFull);
    uut.setOriginalDataSize(_originalDataSize);
    uut.setTagQualityIssues(true);
    const QString taskGridFileName = _testName + "-" + "taskGridBounds.osm";
    conf().set(ConfigOptions::getSnapUnconnectedWaysExistingWayNodeToleranceKey(), 5.0);
    conf().set(ConfigOptions::getSnapUnconnectedWaysSnapToleranceKey(), 0.5);

    uut.replace(
      DATA_TO_REPLACE_URL,
      _replacementDataUrl,
      BoundsStringTaskGridGenerator(
        "130.31148,33.2521,130.3163,33.2588", _outputPath + "/" + taskGridFileName)
        .generateTaskGrid());

    const QMap<QString, long> changesetStats = uut.getChangesetStats();
    if (DISPLAY_METRICS_ONLY)
    {
      LOG_WARN(_testName + ": " << changesetStats);
      LOG_VARW(uut.getOutputMetrics().getNumOrphanedNodesInOutput());
      LOG_VARW(uut.getOutputMetrics().getNumDisconnectedWaysInOutput());
      LOG_VARW(uut.getOutputMetrics().getNumEmptyWaysInOutput());
      LOG_VARW(uut.getOutputMetrics().getNumDuplicateElementPairsInOutput());
    }
    else
    {
      CPPUNIT_ASSERT_EQUAL(4136L, changesetStats[OsmApiDbSqlChangesetApplier::NODE_CREATE_KEY]);
      CPPUNIT_ASSERT_EQUAL(0L, changesetStats[OsmApiDbSqlChangesetApplier::NODE_MODIFY_KEY]);
      CPPUNIT_ASSERT_EQUAL(345L, changesetStats[OsmApiDbSqlChangesetApplier::NODE_DELETE_KEY]);
      CPPUNIT_ASSERT_EQUAL(679L, changesetStats[OsmApiDbSqlChangesetApplier::WAY_CREATE_KEY]);
      CPPUNIT_ASSERT_EQUAL(0L, changesetStats[OsmApiDbSqlChangesetApplier::WAY_MODIFY_KEY]);
      CPPUNIT_ASSERT_EQUAL(60L, changesetStats[OsmApiDbSqlChangesetApplier::WAY_DELETE_KEY]);
      CPPUNIT_ASSERT_EQUAL(4L, changesetStats[OsmApiDbSqlChangesetApplier::RELATION_CREATE_KEY]);
      CPPUNIT_ASSERT_EQUAL(0L, changesetStats[OsmApiDbSqlChangesetApplier::RELATION_MODIFY_KEY]);
      CPPUNIT_ASSERT_EQUAL(0L, changesetStats[OsmApiDbSqlChangesetApplier::RELATION_DELETE_KEY]);

      CPPUNIT_ASSERT_EQUAL(0, uut.getOutputMetrics().getNumOrphanedNodesInOutput());
      CPPUNIT_ASSERT_EQUAL(0, uut.getOutputMetrics().getNumDisconnectedWaysInOutput());
      CPPUNIT_ASSERT_EQUAL(0, uut.getOutputMetrics().getNumEmptyWaysInOutput());
      CPPUNIT_ASSERT_EQUAL(2, uut.getOutputMetrics().getNumDuplicateElementPairsInOutput());

      HOOT_FILE_EQUALS(_inputPath + "/" + outFile, outFull);
    }
  }

  void roundabouts1Test()
  {
    /* Tests to see if we're handling roundabouts during conflate correctly in the replacement
     * changeset workflow. Roundabouts within the replacement AOI in the output (1 of 2 for this
     * test) should remain connected to roads that were feeding them from the input.

       Note that there are a ton of duplicated and unnconnected roads in the ref input whose
       creation must have been the result of a mistake while processing the original source data.
       This causes dropped feature problems in the output after recent changes made to C&R to
       prevent unnecessary create/delete changeset statements by preserving ref IDs. The data should
       probably eventually be cleaned up, which will be tedious (de-duplicate commmand isn't
       removing all the dupes). In the meantime, however, the replacement AOI was shrunk to just
       cover one of the roundabouts where the dropped feature problem doesn't occur.
     */

    _testName = "roundabouts1Test";
    _prepInput(
      _inputPath + "/" + _testName + "-Input1.osm",
      _inputPath + "/" + _testName + "-Input2.osm",
      "");
    conf().set(
      ConfigOptions::getDebugMapsFilenameKey(), _outputPath + "/" + _testName + "-debug.osm");

    ChangesetTaskGridReplacer uut;
    uut.setChangesetsOutputDir(_outputPath);
    const QString outFile = _testName + "-out.osm";
    const QString outFull = _outputPath + "/" + outFile;
    uut.setJobName(_testName);
    uut.setWriteFinalOutput(outFull);
    uut.setOriginalDataSize(_originalDataSize);
    uut.setTagQualityIssues(true);
    const QString taskGridFileName = _testName + "-" + "taskGridBounds.osm";
    conf().set(ConfigOptions::getSnapUnconnectedWaysExistingWayNodeToleranceKey(), 5.0);
    conf().set(ConfigOptions::getSnapUnconnectedWaysSnapToleranceKey(), 0.5);

    uut.replace(
      DATA_TO_REPLACE_URL,
      _replacementDataUrl,
      BoundsStringTaskGridGenerator(
        "-89.6219483,20.9953,-89.6199,20.9979", _outputPath + "/" + taskGridFileName)
        .generateTaskGrid());

    const QMap<QString, long> changesetStats = uut.getChangesetStats();
    if (DISPLAY_METRICS_ONLY)
    {
      LOG_WARN(_testName + ": " << changesetStats);
      LOG_VARW(uut.getOutputMetrics().getNumOrphanedNodesInOutput());
      LOG_VARW(uut.getOutputMetrics().getNumDisconnectedWaysInOutput());
      LOG_VARW(uut.getOutputMetrics().getNumEmptyWaysInOutput());
      LOG_VARW(uut.getOutputMetrics().getNumDuplicateElementPairsInOutput());
    }
    else
    {
      CPPUNIT_ASSERT_EQUAL(15L, changesetStats[OsmApiDbSqlChangesetApplier::NODE_CREATE_KEY]);
      CPPUNIT_ASSERT_EQUAL(0L, changesetStats[OsmApiDbSqlChangesetApplier::NODE_MODIFY_KEY]);
      CPPUNIT_ASSERT_EQUAL(518L, changesetStats[OsmApiDbSqlChangesetApplier::NODE_DELETE_KEY]);
      CPPUNIT_ASSERT_EQUAL(30L, changesetStats[OsmApiDbSqlChangesetApplier::WAY_CREATE_KEY]);
      CPPUNIT_ASSERT_EQUAL(0L, changesetStats[OsmApiDbSqlChangesetApplier::WAY_MODIFY_KEY]);
      CPPUNIT_ASSERT_EQUAL(73L, changesetStats[OsmApiDbSqlChangesetApplier::WAY_DELETE_KEY]);
      CPPUNIT_ASSERT_EQUAL(0L, changesetStats[OsmApiDbSqlChangesetApplier::RELATION_CREATE_KEY]);
      CPPUNIT_ASSERT_EQUAL(0L, changesetStats[OsmApiDbSqlChangesetApplier::RELATION_MODIFY_KEY]);
      CPPUNIT_ASSERT_EQUAL(0L, changesetStats[OsmApiDbSqlChangesetApplier::RELATION_DELETE_KEY]);

      CPPUNIT_ASSERT_EQUAL(0, uut.getOutputMetrics().getNumOrphanedNodesInOutput());
      CPPUNIT_ASSERT_EQUAL(0, uut.getOutputMetrics().getNumDisconnectedWaysInOutput());
      CPPUNIT_ASSERT_EQUAL(0, uut.getOutputMetrics().getNumEmptyWaysInOutput());
      // This number is extremely high due to dupes in the input. See comment above.
      CPPUNIT_ASSERT_EQUAL(538, uut.getOutputMetrics().getNumDuplicateElementPairsInOutput());

      HOOT_FILE_EQUALS(_inputPath + "/" + outFile, outFull);
    }
  }

  void secFilteredToEmpty1Test()
  {
    /*
     * This tests replacement changeset generation with inputs that result in an empty secondary map
     * after filtering when generating the points changeset. Its using full replacement, so the
     * points in the reference dataset should get deleted.
     */

    _testName = "secFilteredToEmpty1Test";
    _prepInput(
      _inputPath + "/" + _testName + "-Input1.osm",
      _inputPath + "/" + _testName + "-Input2.osm",
      "");
    conf().set(
      ConfigOptions::getDebugMapsFilenameKey(), _outputPath + "/" + _testName + "-debug.osm");

    ChangesetTaskGridReplacer uut;
    uut.setChangesetsOutputDir(_outputPath);
    const QString outFile = _testName + "-out.osm";
    const QString outFull = _outputPath + "/" + outFile;
    uut.setJobName(_testName);
    uut.setWriteFinalOutput(outFull);
    uut.setOriginalDataSize(_originalDataSize);
    uut.setTagQualityIssues(true);
    const QString taskGridFileName = _testName + "-" + "taskGridBounds.osm";
    conf().set(ConfigOptions::getSnapUnconnectedWaysExistingWayNodeToleranceKey(), 5.0);
    conf().set(ConfigOptions::getSnapUnconnectedWaysSnapToleranceKey(), 5.0);

    uut.replace(
      DATA_TO_REPLACE_URL,
      _replacementDataUrl,
      BoundsStringTaskGridGenerator(
        "29.031372,1.345701,29.036865,1.351193", _outputPath + "/" + taskGridFileName)
        .generateTaskGrid());

    const QMap<QString, long> changesetStats = uut.getChangesetStats();
    if (DISPLAY_METRICS_ONLY)
    {
      LOG_WARN(_testName + ": " << changesetStats);
      LOG_VARW(uut.getOutputMetrics().getNumOrphanedNodesInOutput());
      LOG_VARW(uut.getOutputMetrics().getNumDisconnectedWaysInOutput());
      LOG_VARW(uut.getOutputMetrics().getNumEmptyWaysInOutput());
      LOG_VARW(uut.getOutputMetrics().getNumDuplicateElementPairsInOutput());
    }
    else
    {
      CPPUNIT_ASSERT_EQUAL(1975L, changesetStats[OsmApiDbSqlChangesetApplier::NODE_CREATE_KEY]);
      CPPUNIT_ASSERT_EQUAL(0L, changesetStats[OsmApiDbSqlChangesetApplier::NODE_MODIFY_KEY]);
      CPPUNIT_ASSERT_EQUAL(1443L, changesetStats[OsmApiDbSqlChangesetApplier::NODE_DELETE_KEY]);
      CPPUNIT_ASSERT_EQUAL(297L, changesetStats[OsmApiDbSqlChangesetApplier::WAY_CREATE_KEY]);
      CPPUNIT_ASSERT_EQUAL(0L, changesetStats[OsmApiDbSqlChangesetApplier::WAY_MODIFY_KEY]);
      CPPUNIT_ASSERT_EQUAL(297L, changesetStats[OsmApiDbSqlChangesetApplier::WAY_DELETE_KEY]);
      CPPUNIT_ASSERT_EQUAL(2L, changesetStats[OsmApiDbSqlChangesetApplier::RELATION_CREATE_KEY]);
      CPPUNIT_ASSERT_EQUAL(0L, changesetStats[OsmApiDbSqlChangesetApplier::RELATION_MODIFY_KEY]);
      CPPUNIT_ASSERT_EQUAL(1L, changesetStats[OsmApiDbSqlChangesetApplier::RELATION_DELETE_KEY]);

      CPPUNIT_ASSERT_EQUAL(0, uut.getOutputMetrics().getNumOrphanedNodesInOutput());
      CPPUNIT_ASSERT_EQUAL(0, uut.getOutputMetrics().getNumDisconnectedWaysInOutput());
      CPPUNIT_ASSERT_EQUAL(0, uut.getOutputMetrics().getNumEmptyWaysInOutput());
      CPPUNIT_ASSERT_EQUAL(0, uut.getOutputMetrics().getNumDuplicateElementPairsInOutput());

      HOOT_FILE_EQUALS(_inputPath + "/" + outFile, outFull);
    }
  }

  ///////////////////////////////////////////////////////////////////////////////////////////

  void github4216UniformTest()
  {
    // larger AOI version of orphanedNodes2Test; good intermediately sized test dataset

    _testName = "github4216UniformTest";
    const QString rootDir = "/home/vagrant/hoot/tmp/4158";
    const QString outDir = rootDir + "/" + _testName;
    conf().set(ConfigOptions::getDebugMapsFilenameKey(), outDir + "/" + _testName + "-debug.osm");
    QDir(outDir).removeRecursively();
    QDir().mkpath(outDir);
    _prepInput(
      rootDir + "/combined-data/NOMEData.osm", rootDir + "/combined-data/OSMData.osm",
      "-115.1260,36.1525,-115.0246,36.2227", outDir);

    ChangesetTaskGridReplacer uut;
    //uut.setKillAfterNumChangesetDerivations(2);
    uut.setChangesetsOutputDir(outDir);
    QString finalOutput = outDir + "/" + _testName + "-out.osm";
    uut.setJobName(_testName);
    uut.setWriteFinalOutput(finalOutput);
    uut.setOriginalDataSize(_originalDataSize);
    uut.setTagQualityIssues(true);
    const TaskGrid taskGrid =
      UniformTaskGridGenerator(
        "-115.1208,36.1550,-115.0280,36.2182", 2,
        outDir + "/" + _testName + "-" + "taskGridBounds.osm")
        .generateTaskGrid();
    OsmMapPtr outputMap = uut.replace(DATA_TO_REPLACE_URL, _replacementDataUrl, taskGrid);

    if (WRITE_NON_CONFLATABLE)
    {
      _writeNonConflatable(outputMap, finalOutput.replace(".osm", "-non-conflatable.osm"));
    }
    if (CALC_DIFF_BETWEEN_REPLACED_AND_REPLACEMENT)
    {
      _writeDiffBetweenReplacedAndReplacement(
        taskGrid.getBounds(), finalOutput.replace(".osm", "-diff.osm"));
    }

    CPPUNIT_ASSERT_EQUAL(0, uut.getOutputMetrics().getNumOrphanedNodesInOutput());
    CPPUNIT_ASSERT_EQUAL(0, uut.getOutputMetrics().getNumDisconnectedWaysInOutput());
    CPPUNIT_ASSERT_EQUAL(0, uut.getOutputMetrics().getNumEmptyWaysInOutput());
    CPPUNIT_ASSERT_EQUAL(28, uut.getOutputMetrics().getNumDuplicateElementPairsInOutput());
  }

  void northVegasLargeUniformTest()
  {
    // whole northern half of city - 64 changesets, ~33.0M changes, avg derivation: 41s,
    // total time: 59min39s, ~553k changes/min, diff between replacement: 6.4k (1hr28m)*, 772
    // unconflatable*

    _testName = "northVegasLargeUniformTest";
    const QString rootDir = "/home/vagrant/hoot/tmp/4158";
    const QString outDir = rootDir + "/" + _testName;
    conf().set(ConfigOptions::getDebugMapsFilenameKey(), outDir + "/" + _testName + "-debug.osm");
    QDir(outDir).removeRecursively();
    QDir().mkpath(outDir);
    _prepInput(
      rootDir + "/combined-data/NOMEData.osm",
      rootDir + "/combined-data/OSMData.osm",
      "");
    // for cell subset debugging
//    _prepInput(
//      rootDir + "/combined-data/NOMEData.osm",
//      rootDir + "/combined-data/OSMData.osm",
//      "-115.0317,36.2456,-114.9747,36.2870",
//      outDir);

    ChangesetTaskGridReplacer uut;
    uut.setChangesetsOutputDir(outDir);
    QString finalOutput = outDir + "/" + _testName + "-out.osm";
    uut.setJobName(_testName);
    uut.setWriteFinalOutput(finalOutput);
    uut.setOriginalDataSize(_originalDataSize);
    uut.setTagQualityIssues(true);
    // for cell subset debugging
//    QList<int> includeIds;
//    includeIds.append(59);
//    uut.setTaskCellIncludeIds(includeIds);
    //uut.setKillAfterNumChangesetDerivations(2);
    const TaskGrid taskGrid =
      UniformTaskGridGenerator(
        "-115.3528,36.0919,-114.9817,36.3447", 8,
        outDir + "/" + _testName + "-" + "taskGridBounds.osm")
        .generateTaskGrid();
    OsmMapPtr outputMap = uut.replace(DATA_TO_REPLACE_URL, _replacementDataUrl, taskGrid);
    // for cell subset debugging
//    uut.replace(
//      DATA_TO_REPLACE_URL,
//      _replacementDataUrl,
//      UniformTaskGridGenerator(
//        "-115.0280,36.2499,-114.9817,36.2817", 1,
//        outDir + "/" + _testName + "-" + "taskGridBounds.osm")
//        .generateTaskGrid());

    if (WRITE_NON_CONFLATABLE)
    {
      _writeNonConflatable(outputMap, finalOutput.replace(".osm", "-non-conflatable.osm"));
    }
    if (CALC_DIFF_BETWEEN_REPLACED_AND_REPLACEMENT)
    {
      _writeDiffBetweenReplacedAndReplacement(
        taskGrid.getBounds(), finalOutput.replace(".osm", "-diff.osm"));
    }

    CPPUNIT_ASSERT_EQUAL(4, uut.getOutputMetrics().getNumOrphanedNodesInOutput());
    CPPUNIT_ASSERT_EQUAL(0, uut.getOutputMetrics().getNumDisconnectedWaysInOutput());
    CPPUNIT_ASSERT_EQUAL(0, uut.getOutputMetrics().getNumEmptyWaysInOutput());
    CPPUNIT_ASSERT_EQUAL(517, uut.getOutputMetrics().getNumDuplicateElementPairsInOutput());
  }

  void auditionTest()
  {
    _testName = "auditionTest";
    const QString rootDir = "/home/vagrant/hoot/tmp/Task28";
    const QString outDir = rootDir + "/" + _testName;
    conf().set(ConfigOptions::getDebugMapsFilenameKey(), outDir + "/" + _testName + "-debug.osm");
    QDir(outDir).removeRecursively();
    QDir().mkpath(outDir);;
    _prepInput(
      rootDir + "/NOME_14992d.osm",
      rootDir + "/OSM_14992d.osm"/*,
      "-115.0317,36.2456,-114.9747,36.2870",
      outDir*/);

    ChangesetTaskGridReplacer uut;
    uut.setChangesetsOutputDir(outDir);
    QString finalOutput = outDir + "/" + _testName + "-out.osm";
    uut.setJobName(_testName);
    uut.setWriteFinalOutput(finalOutput);
    uut.setOriginalDataSize(_originalDataSize);
    uut.setTagQualityIssues(true);
    // for cell subset debugging
//    QList<int> includeIds;
//    includeIds.append(59);
//    uut.setTaskCellIncludeIds(includeIds);
    //uut.setKillAfterNumChangesetDerivations(2);
    uut.replace(
      DATA_TO_REPLACE_URL,
      _replacementDataUrl,
      UniformTaskGridGenerator(
        "-81.364746,28.516969,-81.35376,28.526623", 1,
        outDir + "/" + _testName + "-" + "taskGridBounds.osm")
        .generateTaskGrid());
  }

private:

  QString _testName;
  QElapsedTimer _subTaskTimer;
  QString _replacementDataUrl;
  // original size of the data to be replaced; TODO: remove this?
  int _originalDataSize;

  void _prepInput(const QString& toReplace, const QString& replacement,
                  const QString& cropInputBounds = "", const QString& outDir = "",
                  const bool clearToReplaceDb = true)
  {
    _loadDataToReplaceDb(toReplace, cropInputBounds, outDir + "/starting-cropped-to-replace.osm",
                         clearToReplaceDb);

    _replacementDataUrl = ServicesDbTestUtils::getDbModifyUrl(_testName).toString();
    _loadReplacementDataDb(
      replacement, cropInputBounds, outDir + "/starting-cropped-replacement.osm");
  }

  void _initConfig()
  {
    conf().set(ConfigOptions::getWriterIncludeDebugTagsKey(), true);
    conf().set(ConfigOptions::getUuidHelperRepeatableKey(), true);
    conf().set(ConfigOptions::getDebugMapsRemoveMissingElementsKey(), true);
    conf().set(ConfigOptions::getApiDbEmailKey(), USER_EMAIL);
    conf().set(ConfigOptions::getHootapiDbWriterCreateUserKey(), true);
    conf().set(ConfigOptions::getHootapiDbWriterOverwriteMapKey(), true);
    conf().set(ConfigOptions::getChangesetUserIdKey(), 1);
    conf().set(ConfigOptions::getChangesetMaxSizeKey(), 999999);
    conf().set(ConfigOptions::getSnapUnconnectedWaysExistingWayNodeToleranceKey(), 0.5);
    conf().set(ConfigOptions::getSnapUnconnectedWaysSnapToleranceKey(), 5.0);
    conf().set(ConfigOptions::getApidbReaderReadFullThenCropOnBoundedKey(), false);
    conf().set(ConfigOptions::getLogWarningsForEmptyInputMapsKey(), false);
    // leave enabled for debugging only
    conf().set(ConfigOptions::getDebugMapsWriteKey(), false);
  }

  void _loadDataToReplaceDb(
    const QString& input, const QString& cropBounds = "", const QString& cropOut = "",
    const bool clearDb = true)
  {
    OsmMapPtr map(new OsmMap());

    // make sure the db is empty
    if (clearDb)
    {
      ServicesDbTestUtils::deleteDataFromOsmApiTestDatabase();
      ApiDb::execSqlFile(DATA_TO_REPLACE_URL, "test-files/servicesdb/users.sql");
      OsmMapReaderFactory::read(map, DATA_TO_REPLACE_URL, true, Status::Unknown1);
      if (map->size() != 0)
      {
        throw HootException("Data to replace db is not empty at start.");
      }
    }

    map.reset(new OsmMap());
    // TODO: replace the string truncation lengths with getProgressVarPrintLengthMax
    LOG_STATUS("Reading the data to replace from: ..." << input.right(25) << "...");
    OsmMapReaderFactory::read(map, input, true, Status::Unknown1);
    LOG_STATUS(
      StringUtils::formatLargeNumber(map->size()) << " elements to replace read in: " <<
      StringUtils::millisecondsToDhms(_subTaskTimer.elapsed()));
    _subTaskTimer.restart();
    if (!cropBounds.isEmpty())
    {
      MapCropper cropper;
      cropper.setBounds(GeometryUtils::boundsFromString(cropBounds));
      cropper.setRemoveMissingElements(false);
      cropper.setRemoveSuperflousFeatures(false);
      LOG_STATUS(
        "Data to replace pre-cropped size: " << StringUtils::formatLargeNumber(map->size()));
      LOG_STATUS(cropper.getInitStatusMessage());
      cropper.apply(map);
      LOG_STATUS(
        "Data to replace cropped size: " << StringUtils::formatLargeNumber(map->size()) <<
        "; cropped in: " << StringUtils::millisecondsToDhms(_subTaskTimer.elapsed()));
      if (ConfigOptions().getDebugMapsWrite())
      {
        if (cropOut.trimmed().isEmpty())
        {
          throw IllegalArgumentException("No crop output file path specified.");
        }
        LOG_STATUS("Writing cropped data to: ..." << cropOut.right(25) << "...");
        OsmMapWriterFactory::write(map, cropOut);
      }
      _subTaskTimer.restart();
    }

    // Add a tag specifying this is the to replace data, so we can see it in the output.
    SetTagValueVisitor addTagVis("note", "Source 1");
    NotCriterion addTagCrit(std::shared_ptr<WayNodeCriterion>(new WayNodeCriterion(map)));
    FilteredVisitor deleteExcludeTagVis(addTagCrit, addTagVis);
    map->visitRw(deleteExcludeTagVis);

    // TODO: after separating quality issue tagging from replacement, grab starting quality metrics
    // for this data.

    LOG_STATUS("Loading the data to replace db to: ..." << DATA_TO_REPLACE_URL.right(25) << "...");
    OsmMapWriterFactory::write(map, DATA_TO_REPLACE_URL);
    _originalDataSize = (int)map->size();
    LOG_STATUS(
      StringUtils::formatLargeNumber(map->size()) << " elements to replace loaded in: " <<
      StringUtils::millisecondsToDhms(_subTaskTimer.elapsed()));
    _subTaskTimer.restart();
  }

  void _loadReplacementDataDb(
    const QString& input, const QString& cropBounds = "", const QString& cropOut = "")
  {
    // TODO: Can this be converted over to use the bulk inserter?

    OsmMapPtr map(new OsmMap());
    LOG_STATUS("Reading the replacement data from: ..." << input.right(25) << "...");
    // Load in with the replacement source IDs to mimic production behavior.
    // ChangesetReplacementCreator will throw them out when the data is first loaded in to avoid ID
    // conflicts.
    OsmMapReaderFactory::read(map, input, true, Status::Unknown2);
    LOG_STATUS(
      StringUtils::formatLargeNumber(map->size()) << " replacement elements read in: " <<
      StringUtils::millisecondsToDhms(_subTaskTimer.elapsed()));
    _subTaskTimer.restart();
    if (!cropBounds.isEmpty())
    {
      MapCropper cropper;
      cropper.setBounds(GeometryUtils::boundsFromString(cropBounds));
      cropper.setRemoveMissingElements(false);
      cropper.setRemoveSuperflousFeatures(false);
      LOG_STATUS(
        "Replacement data pre-cropped size: " << StringUtils::formatLargeNumber(map->size()));
      LOG_STATUS(cropper.getInitStatusMessage());
      cropper.apply(map);
      LOG_STATUS(
        "Replacement data cropped size: " << StringUtils::formatLargeNumber(map->size()) <<
        "; cropped in: " << StringUtils::millisecondsToDhms(_subTaskTimer.elapsed()));
      if (ConfigOptions().getDebugMapsWrite())
      {
        if (cropOut.trimmed().isEmpty())
        {
          throw IllegalArgumentException("No crop output file path specified.");
        }
        LOG_STATUS("Writing cropped data to: ..." << cropOut.right(25) << "...");
        OsmMapWriterFactory::write(map, cropOut);
      }
      _subTaskTimer.restart();
    }

    // add a tag specifying this is the to replace data, so we can see it in the output
    SetTagValueVisitor addTagVis("note", "Source 2");
    NotCriterion addTagCrit(std::shared_ptr<WayNodeCriterion>(new WayNodeCriterion(map)));
    FilteredVisitor deleteExcludeTagVis(addTagCrit, addTagVis);
    map->visitRw(deleteExcludeTagVis);

    // TODO: after separating quality issue tagging from replacement, grab starting quality metrics
    // for this data.

    LOG_STATUS("Loading the replacement data db to: ..." << _replacementDataUrl.right(25) << "...");
    OsmMapWriterFactory::write(map, _replacementDataUrl);
    LOG_STATUS(
      StringUtils::formatLargeNumber(map->size()) << " replacement elements loaded in: " <<
      StringUtils::millisecondsToDhms(_subTaskTimer.elapsed()));
    _subTaskTimer.restart();
  }

  void _writeNonConflatable(const ConstOsmMapPtr& map, const QString& output)
  {
    // Output any features that hoot doesn't know how to conflate and, therefore won't show up in a
    // diff between replaced and replacement data, into their own file for debugging purposes.
    const int nonConflatableSize = ConflateUtils::writeNonConflatable(map, output);
    if (nonConflatableSize > 0)
    {
      LOG_STATUS(
        "Non-conflatable data of size: " <<
        StringUtils::formatLargeNumber(nonConflatableSize) << " written in: " <<
        StringUtils::millisecondsToDhms(_subTaskTimer.elapsed()));
    }
    else
    {
      LOG_STATUS("No non-conflatable elements present.");
    }
    _subTaskTimer.restart();
  }

  void _writeDiffBetweenReplacedAndReplacement(
    const geos::geom::Envelope& bounds, const QString& output)
  {
    // Calculate a diff between the data we just replaced and the original replacement data to
    // aid in finding any errors during the replacement process. We only want to calculate the
    // diff out to the task grid bounds, b/c that's the data that was actually replaced.
    //conf().set(ConfigOptions::getConvertBoundsKey(), GeometryUtils::envelopeToString(bounds));
    // use a lenient bounds
    conf().set(ConfigOptions::getConvertBoundsKeepEntireFeaturesCrossingBoundsKey(), true);
    conf().set(
      ConfigOptions::getConvertBoundsKeepImmediatelyConnectedWaysOutsideBoundsKey(), false);
    conf().set(ConfigOptions::getConvertBoundsKeepOnlyFeaturesInsideBoundsKey(), false);
    const QString replacedDataUrl = DATA_TO_REPLACE_URL;
    ConflateUtils::writeDiff(_replacementDataUrl, replacedDataUrl, bounds, output);
  }

  void _cleanupDataToReplace()
  {
    LOG_STATUS(
      "Cleaning up the data to replace db at: ..." << DATA_TO_REPLACE_URL.right(25) << "...");
    ServicesDbTestUtils::deleteDataFromOsmApiTestDatabase();
    LOG_INFO(
      "Data to replace db cleaned in: " <<
      StringUtils::millisecondsToDhms(_subTaskTimer.elapsed()));
    _subTaskTimer.restart();
  }

  void _cleanupReplacementData()
  {
    LOG_STATUS(
      "Cleaning up the replacement data db at: ..." << _replacementDataUrl.right(25) << "...");
    HootApiDb database;
    database.open(ServicesDbTestUtils::getDbModifyUrl(_testName).toString());
    database.deleteMap(database.getMapIdByName(_testName));
    database.close();
    ServicesDbTestUtils::deleteUser(USER_EMAIL);
    LOG_STATUS(
      "Replacement data cleaned in: " <<
      StringUtils::millisecondsToDhms(_subTaskTimer.elapsed()));
    _subTaskTimer.restart();
  }
};

#ifdef HOOT_HAVE_SERVICES
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ServiceChangesetReplacementGridTest, "glacial");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ServiceChangesetReplacementGridTest, "serial");
#endif

}
