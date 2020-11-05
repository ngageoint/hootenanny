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

namespace hoot
{

static const QString USER_EMAIL = "ServiceChangesetReplacementGridTest@hoottestcpp.org";
static const QString DATA_TO_REPLACE_URL = ServicesDbTestUtils::getOsmApiDbUrl().toString();

/*
 * This test harness allows for testing the Cut and Replace workflow across adjacent task grid
 * cells. By removing the processes of input data retrieval and changeset application via API from
 * the test workflow, bugs can more easily be narrowed down to just those caused by
 * ChangesetReplacementGenerator (most of the time).
 *
 * Its worth noting that both the orphaned node and duplicate elements counts may be a little
 * dubious at this point, but at least they give us a baseline until their accuracy can be improved.
 *
 * TODO: run all validation on input before and after
 */
class ServiceChangesetReplacementGridTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(ServiceChangesetReplacementGridTest);

  CPPUNIT_TEST(orphanedNodes1Test);
  CPPUNIT_TEST(orphanedNodes2Test);
  CPPUNIT_TEST(droppedNodes1Test);
  CPPUNIT_TEST(droppedPointPolyRelationMembers1Test);
  CPPUNIT_TEST(badPolyIdSync1Test);
  CPPUNIT_TEST(badPolyIdSync2Test);

  // ENABLE THESE TESTS FOR DEBUGGING ONLY

  //CPPUNIT_TEST(github4216UniformTest);
  //CPPUNIT_TEST(northVegasLargeUniformTest);

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

    ChangesetTaskGridReplacer uut;
    uut.setChangesetsOutputDir(_outputPath);
    const QString outFile = _testName + "-out.osm";
    const QString outFull = _outputPath + "/" + outFile;
    uut.setWriteFinalOutput(outFull);
    uut.setOriginalDataSize(_originalDataSize);
    uut.setTagQualityIssues(true);
    uut.setCalcDiffWithReplacement(false);
    uut.setOutputNonConflatable(false);
    uut.replace(
      DATA_TO_REPLACE_URL,
      _replacementDataUrl,
      BoundsFileTaskGridGenerator(
        QStringList(_inputPath + "/orphanedNodes1Test-task-grid.osm")).generateTaskGrid());

    CPPUNIT_ASSERT_EQUAL(0, uut.getNumOrphanedNodesInOutput());
    CPPUNIT_ASSERT_EQUAL(0, uut.getNumDisconnectedWaysInOutput());
    CPPUNIT_ASSERT_EQUAL(0, uut.getNumEmptyWaysInOutput());
    CPPUNIT_ASSERT_EQUAL(28, uut.getNumDuplicateElementPairsInOutput());
    HOOT_FILE_EQUALS(_inputPath + "/" + outFile, outFull);
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

    ChangesetTaskGridReplacer uut;
    uut.setChangesetsOutputDir(_outputPath);
    const QString outFile = _testName + "-out.osm";
    const QString outFull = _outputPath + "/" + outFile;
    uut.setWriteFinalOutput(outFull);
    uut.setOriginalDataSize(_originalDataSize);
    uut.setTagQualityIssues(true);
    uut.setCalcDiffWithReplacement(false);
    uut.setOutputNonConflatable(false);
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

    CPPUNIT_ASSERT_EQUAL(0, uut.getNumOrphanedNodesInOutput());
    CPPUNIT_ASSERT_EQUAL(0, uut.getNumDisconnectedWaysInOutput());
    CPPUNIT_ASSERT_EQUAL(0, uut.getNumEmptyWaysInOutput());
    CPPUNIT_ASSERT_EQUAL(2, uut.getNumDuplicateElementPairsInOutput());
    HOOT_FILE_EQUALS(_inputPath + "/" + outFile, outFull);
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
    conf().set(ConfigOptions::getDebugMapsFilenameKey(), _outputPath + "/debug.osm");

    ChangesetTaskGridReplacer uut;
    uut.setChangesetsOutputDir(_outputPath);
    const QString outFile = _testName + "-out.osm";
    const QString outFull = _outputPath + "/" + outFile;
    uut.setWriteFinalOutput(outFull);
    uut.setOriginalDataSize(_originalDataSize);
    uut.setTagQualityIssues(true);
    uut.setCalcDiffWithReplacement(false);
    uut.setOutputNonConflatable(false);
    uut.replace(
      DATA_TO_REPLACE_URL,
      _replacementDataUrl,
      UniformTaskGridGenerator(
        "-115.3274,36.2640,-115.3106,36.2874", 2,
        _outputPath + "/" + _testName + "-" + "taskGridBounds.osm")
        .generateTaskGrid());

    CPPUNIT_ASSERT_EQUAL(0, uut.getNumOrphanedNodesInOutput());
    CPPUNIT_ASSERT_EQUAL(0, uut.getNumDisconnectedWaysInOutput());
    CPPUNIT_ASSERT_EQUAL(0, uut.getNumEmptyWaysInOutput());
    CPPUNIT_ASSERT_EQUAL(0, uut.getNumDuplicateElementPairsInOutput());
    HOOT_FILE_EQUALS(_inputPath + "/" + outFile, outFull);
  }

  void droppedPointPolyRelationMembers1Test()
  {
    // part of github 4228 - The POI, Desert Shores, belongs in a relation with a landuse poly.
    // Neither should be dropped from the output due to being in a relation with members of mixed
    // geometry types.

    // This is needed to suppress some ElementToGeometryConverter warnings messages that should eventually be
    // looked into.
    DisableLog dl;

    _testName = "droppedPointPolyRelationMembers1Test";
    _prepInput(
      _inputPath + "/" + _testName + "-Input1.osm",
      _inputPath + "/" + _testName + "-Input2.osm",
      "");
    conf().set(ConfigOptions::getDebugMapsFilenameKey(), _outputPath + "/debug.osm");

    ChangesetTaskGridReplacer uut;
    uut.setChangesetsOutputDir(_outputPath);
    const QString outFile = _testName + "-out.osm";
    const QString outFull = _outputPath + "/" + outFile;
    uut.setWriteFinalOutput(outFull);
    uut.setOriginalDataSize(_originalDataSize);
    uut.setTagQualityIssues(true);
    uut.setCalcDiffWithReplacement(false);
    uut.setOutputNonConflatable(false);
    uut.replace(
      DATA_TO_REPLACE_URL,
      _replacementDataUrl,
      UniformTaskGridGenerator(
        "-115.3064,36.1867,-115.2136,36.2498", 2,
        _outputPath + "/" + _testName + "-" + "taskGridBounds.osm")
        .generateTaskGrid());

    CPPUNIT_ASSERT_EQUAL(0, uut.getNumOrphanedNodesInOutput());
    CPPUNIT_ASSERT_EQUAL(0, uut.getNumDisconnectedWaysInOutput());
    CPPUNIT_ASSERT_EQUAL(0, uut.getNumEmptyWaysInOutput());
    CPPUNIT_ASSERT_EQUAL(6, uut.getNumDuplicateElementPairsInOutput());
    HOOT_FILE_EQUALS(_inputPath + "/" + outFile, outFull);
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
    conf().set(ConfigOptions::getDebugMapsFilenameKey(), _outputPath + "/debug.osm");

    ChangesetTaskGridReplacer uut;
    uut.setChangesetsOutputDir(_outputPath);
    const QString outFile = _testName + "-out.osm";
    const QString outFull = _outputPath + "/" + outFile;
    uut.setWriteFinalOutput(outFull);
    uut.setOriginalDataSize(_originalDataSize);
    uut.setTagQualityIssues(true);
    uut.setCalcDiffWithReplacement(false);
    uut.setOutputNonConflatable(false);
    uut.replace(
      DATA_TO_REPLACE_URL,
      _replacementDataUrl,
      UniformTaskGridGenerator(
        "-115.2434,36.3022,-115.2317,36.3136", 1,
        _outputPath + "/" + _testName + "-" + "taskGridBounds.osm")
        .generateTaskGrid());

    CPPUNIT_ASSERT_EQUAL(0, uut.getNumOrphanedNodesInOutput());
    CPPUNIT_ASSERT_EQUAL(0, uut.getNumDisconnectedWaysInOutput());
    CPPUNIT_ASSERT_EQUAL(0, uut.getNumEmptyWaysInOutput());
    CPPUNIT_ASSERT_EQUAL(0, uut.getNumDuplicateElementPairsInOutput());
    HOOT_FILE_EQUALS(_inputPath + "/" + outFile, outFull);
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
    conf().set(ConfigOptions::getDebugMapsFilenameKey(), _outputPath + "/debug.osm");

    ChangesetTaskGridReplacer uut;
    uut.setChangesetsOutputDir(_outputPath);
    const QString outFile = _testName + "-out.osm";
    const QString outFull = _outputPath + "/" + outFile;
    uut.setWriteFinalOutput(outFull);
    uut.setOriginalDataSize(_originalDataSize);
    uut.setTagQualityIssues(true);
    uut.setCalcDiffWithReplacement(false);
    uut.setOutputNonConflatable(false);
    uut.replace(
      DATA_TO_REPLACE_URL,
      _replacementDataUrl,
      UniformTaskGridGenerator(
        "-115.2822,36.2226,-115.2779,36.2261", 1,
        _outputPath + "/" + _testName + "-" + "taskGridBounds.osm")
        .generateTaskGrid());

    CPPUNIT_ASSERT_EQUAL(0, uut.getNumOrphanedNodesInOutput());
    CPPUNIT_ASSERT_EQUAL(0, uut.getNumDisconnectedWaysInOutput());
    CPPUNIT_ASSERT_EQUAL(0, uut.getNumEmptyWaysInOutput());
    CPPUNIT_ASSERT_EQUAL(0, uut.getNumDuplicateElementPairsInOutput());
    HOOT_FILE_EQUALS(_inputPath + "/" + outFile, outFull);
  }

  ///////////////////////////////////////////////////////////////////////////////////////////

  void github4216UniformTest()
  {
    // larger AOI version of orphanedNodes2Test; good intermediately sized test dataset

    _testName = "github4216UniformTest";
    const QString rootDir = "/home/vagrant/hoot/tmp/4158";
    const QString outDir = rootDir + "/" + _testName;
    conf().set(ConfigOptions::getDebugMapsFilenameKey(), outDir + "/debug.osm");
    QDir(outDir).removeRecursively();
    QDir().mkpath(outDir);
    _prepInput(
      rootDir + "/combined-data/NOMEData.osm", rootDir + "/combined-data/OSMData.osm",
      "-115.1260,36.1525,-115.0246,36.2227", outDir);

    ChangesetTaskGridReplacer uut;
    //uut.setKillAfterNumChangesetDerivations(2);
    uut.setChangesetsOutputDir(outDir);
    uut.setWriteFinalOutput(outDir + "/" + _testName + "-out.osm");
    uut.setOriginalDataSize(_originalDataSize);
    uut.setTagQualityIssues(true);
    uut.setCalcDiffWithReplacement(false);
    uut.setOutputNonConflatable(false);
    uut.replace(
      DATA_TO_REPLACE_URL,
      _replacementDataUrl,
      UniformTaskGridGenerator(
        "-115.1208,36.1550,-115.0280,36.2182", 2,
        outDir + "/" + _testName + "-" + "taskGridBounds.osm")
        .generateTaskGrid());

    // TODO: after separating quality issue tagging from replacement, call these directly after
    // the replacement and before the diff calc
//    CPPUNIT_ASSERT_EQUAL(0, uut.getNumOrphanedNodesInOutput());
//    CPPUNIT_ASSERT_EQUAL(0, uut.getNumDisconnectedWaysInOutput());
//    CPPUNIT_ASSERT_EQUAL(0, uut.getNumEmptyWaysInOutput());
//    CPPUNIT_ASSERT_EQUAL(28, uut.getNumDuplicateElementPairsInOutput());
  }

  void northVegasLargeUniformTest()
  {
    // whole northern half of city - 64 changesets, ~33.0M changes, avg derivation: 47s,
    // total time: 1hr7min, ~492k changes/min, diff between replacement: 6.4k (1hr28m), 772
    // unconflatable

    _testName = "northVegasLargeUniformTest";
    const QString rootDir = "/home/vagrant/hoot/tmp/4158";
    const QString outDir = rootDir + "/" + _testName;
    conf().set(ConfigOptions::getDebugMapsFilenameKey(), outDir + "/debug.osm");
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
    uut.setWriteFinalOutput(outDir + "/" + _testName + "-out.osm");
    uut.setOriginalDataSize(_originalDataSize);
    uut.setTagQualityIssues(true);
    uut.setCalcDiffWithReplacement(false);
    uut.setOutputNonConflatable(false);
    // for cell subset debugging
//    QList<int> includeIds;
//    includeIds.append(59);
//    uut.setTaskCellIncludeIds(includeIds);
    //uut.setKillAfterNumChangesetDerivations(2);
    uut.replace(
      DATA_TO_REPLACE_URL,
      _replacementDataUrl,
      UniformTaskGridGenerator(
        "-115.3528,36.0919,-114.9817,36.3447", 8,
        outDir + "/" + _testName + "-" + "taskGridBounds.osm")
        .generateTaskGrid());
    // for cell subset debugging
//    uut.replace(
//      DATA_TO_REPLACE_URL,
//      _replacementDataUrl,
//      UniformTaskGridGenerator(
//        "-115.0280,36.2499,-114.9817,36.2817", 1,
//        outDir + "/" + _testName + "-" + "taskGridBounds.osm")
//        .generateTaskGrid());

    //CPPUNIT_ASSERT_EQUAL(26, uut.getNumOrphanedNodesInOutput());
    //CPPUNIT_ASSERT_EQUAL(0, uut.getNumDisconnectedWaysInOutput());
    //CPPUNIT_ASSERT_EQUAL(0, uut.getNumEmptyWaysInOutput());
    //CPPUNIT_ASSERT_EQUAL(506, uut.getNumDuplicateElementPairsInOutput());
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
    conf().set(ConfigOptions::getChangesetReplacementPassConflateReviewsKey(), true);
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
      MapCropper cropper(GeometryUtils::envelopeFromConfigString(cropBounds));
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
      MapCropper cropper(GeometryUtils::envelopeFromConfigString(cropBounds));
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

    // TODO: after separating quality issue tagging from replacement, grab starting quality metrics
    // for this data.

    LOG_STATUS("Loading the replacement data db to: ..." << _replacementDataUrl.right(25) << "...");
    OsmMapWriterFactory::write(map, _replacementDataUrl);
    LOG_STATUS(
      StringUtils::formatLargeNumber(map->size()) << " replacement elements loaded in: " <<
      StringUtils::millisecondsToDhms(_subTaskTimer.elapsed()));
    _subTaskTimer.restart();
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
