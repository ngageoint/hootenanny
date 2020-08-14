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
#include <hoot/core/util/GeometryUtils.h>
#include <hoot/core/io/ServicesDbTestUtils.h>
#include <hoot/core/io/HootApiDbWriter.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/ops/MapCropper.h>
#include <hoot/core/algorithms/changeset/ChangesetTaskGridReplacer.h>

namespace hoot
{

static const QString USER_EMAIL = "ServiceChangesetReplacementGridTest@hoottestcpp.org";
static const QString DATA_TO_REPLACE_URL = ServicesDbTestUtils::getOsmApiDbUrl().toString();

/*
 * This test allows for testing the Cut and Replace workflow across adjacent AOI's. Either a node
 * density generated task grid or a task grid from an input file may be used.
 */
class ServiceChangesetReplacementGridTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(ServiceChangesetReplacementGridTest);
  CPPUNIT_TEST(orphanedNodes1Test);
 // ENABLE THESE TESTS FOR DEBUGGING ONLY
//  CPPUNIT_TEST(vegasSmallTest);
//  CPPUNIT_TEST(vegasMediumTest);
//  CPPUNIT_TEST(vegasLargeTest);
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

  void orphanedNodes1Test()
  {
    _testName = "orphanedNodes1Test";
    _prepInput(
      _inputPath + "/329_ReferenceNOMEData_31JULY2020.osm",
      _inputPath + "/329_SecondaryOSMData_31JULY2020.osm", "");

    ChangesetTaskGridReplacer uut;
    uut.setTaskGridType(ChangesetTaskGridReplacer::GridType::InputFile);
    uut.setGridInput(_inputPath + "/Task14and15.osm");
    uut.setChangesetsOutputDir(_outputPath);
    const QString outFile = _testName + "-out.osm";
    const QString outFull = _outputPath + "/" + outFile;
    uut.setWriteFinalOutput(outFull);
    uut.setOriginalDataSize(_originalDataSize);
    uut.replace(DATA_TO_REPLACE_URL, _replacementDataUrl);

    _cleanup();

    HOOT_FILE_EQUALS(_inputPath + "/" + outFile, outFull);
  }

  void vegasSmallTest()
  {
    // 4 sq blocks of the city - 4 changesets

    _testName = "vegasSmallTest";
    const QString rootDir = "/home/vagrant/hoot/tmp/4158/combined-data";
    _prepInput(
      rootDir + "/NOMEData.osm", rootDir + "/OSMData.osm", "-115.3314,36.2825,-115.2527,36.3387");

    ChangesetTaskGridReplacer uut;
    uut.setTaskGridType(ChangesetTaskGridReplacer::GridType::NodeDensity);
    uut.setKillAfterNumChangesetDerivations(2);
    uut.setNodeDensityGridBounds("-115.3059,36.2849,-115.2883,36.2991");
    uut.setNodeDensityMaxNodesPerCell(1000);
    uut.setNodeDensityTaskGridOutputFile(
      _outputPath + "/" + _testName + "-" + "taskGridBounds.osm");
    uut.setChangesetsOutputDir(_outputPath);
    const QString finalOutput = _outputPath + "/" + _testName + "-out.osm";
    uut.setWriteFinalOutput(finalOutput);
    uut.setOriginalDataSize(_originalDataSize);
    uut.replace(DATA_TO_REPLACE_URL, _replacementDataUrl);

    _cleanup();
  }

  void vegasMediumTest()
  {
    // ~1/4 of city - 64 changesets

    _testName = "vegasMediumTest";
    const QString rootDir = "/home/vagrant/hoot/tmp/4158/combined-data";
    _prepInput(
      rootDir + "/NOMEData.osm", rootDir + "/OSMData.osm", "-115.3441,36.2012,-115.1942,36.3398");

    ChangesetTaskGridReplacer uut;
    uut.setTaskGridType(ChangesetTaskGridReplacer::GridType::NodeDensity);
    uut.setKillAfterNumChangesetDerivations(2);
    uut.setNodeDensityGridBounds("-115.3332,36.2178,-115.1837,36.3400");
    uut.setNodeDensityMaxNodesPerCell(10000);
    uut.setNodeDensityTaskGridOutputFile(
      _outputPath + "/" + _testName + "-" + "taskGridBounds.osm");
    uut.setChangesetsOutputDir(_outputPath);
    const QString finalOutput = _outputPath + "/" + _testName + "-out.osm";
    uut.setWriteFinalOutput(finalOutput);
    uut.setOriginalDataSize(_originalDataSize);
    uut.replace(DATA_TO_REPLACE_URL, _replacementDataUrl);

    _cleanup();
  }

  void vegasLargeTest()
  {
    // whole city - 64 changesets

    _testName = "vegasLargeTest";
    const QString rootDir = "/home/vagrant/hoot/tmp/4158/combined-data";
    _prepInput(rootDir + "/NOMEData.osm", rootDir + "/OSMData.osm", "");

    ChangesetTaskGridReplacer uut;
    uut.setTaskGridType(ChangesetTaskGridReplacer::GridType::NodeDensity);
    uut.setKillAfterNumChangesetDerivations(2);
    uut.setNodeDensityGridBounds("-115.3528,36.0919,-114.9817,36.3447");
    uut.setNodeDensityMaxNodesPerCell(100000);
    uut.setNodeDensityTaskGridOutputFile(
      _outputPath + "/" + _testName + "-" + "taskGridBounds.osm");
    uut.setChangesetsOutputDir(_outputPath);
    const QString finalOutput = _outputPath + "/" + _testName + "-out.osm";
    uut.setWriteFinalOutput(finalOutput);
    uut.setOriginalDataSize(_originalDataSize);
    uut.replace(DATA_TO_REPLACE_URL, _replacementDataUrl);

    _cleanup();
  }

private:

  QString _testName;

  QElapsedTimer _subTaskTimer;

  // for working with a subset of the input data; leave empty to disable
  QString _inputCropBounds;
  // read the replacement data just from the source file or load it into a hootapidb layer first?;
  // data replaced is always loaded into osmapidb for changeset application purposes
  //bool _writeReplacementDataToDb;
  QString _replacementDataUrl;
  int _originalDataSize;

  void _prepInput(const QString& toReplace, const QString& replacement,
                  const QString& cropInputBounds = "")
  {
    _subTaskTimer.start();
    _initConfig();
    _replacementDataUrl = ServicesDbTestUtils::getDbModifyUrl(_testName).toString();
    _cleanup();
    _loadDataToReplaceDb(toReplace, cropInputBounds);
    // TODO: later will make replacement db load optional and support file as well
    _loadReplacementDataDb(replacement, cropInputBounds);
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
    conf().set(ConfigOptions::getDebugMapsFilenameKey(), _outputPath + "/debug.osm");
    conf().set(ConfigOptions::getApidbReaderReadFullThenCropOnBoundedKey(), false);
    conf().set(ConfigOptions::getChangesetReplacementCacheInputFileMapsKey(), true);
    conf().set(ConfigOptions::getChangesetReplacementPassConflateReviewsKey(), true);

    // leave enabled for debugging only
    conf().set(ConfigOptions::getDebugMapsWriteKey(), false);
  }

  void _loadDataToReplaceDb(const QString& input, const QString& cropBounds = "")
  {
    LOG_STATUS("Initializing the data to replace db at: " << DATA_TO_REPLACE_URL << "...");
    ServicesDbTestUtils::deleteDataFromOsmApiTestDatabase();
    ApiDb::execSqlFile(DATA_TO_REPLACE_URL, "test-files/servicesdb/users.sql");
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(map, DATA_TO_REPLACE_URL, true, Status::Unknown1);
    if (map->size() != 0)
    {
      throw HootException("Data to replace db is not empty at start.");
    }
    LOG_STATUS(
      "Data to replace db initialized in: " <<
      StringUtils::millisecondsToDhms(_subTaskTimer.elapsed()));
    _subTaskTimer.restart();

    map.reset(new OsmMap());
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
      LOG_STATUS("Data to replace cropped size: " << StringUtils::formatLargeNumber(map->size()));
      LOG_STATUS(
        "Data to replace cropped in: " <<
        StringUtils::millisecondsToDhms(_subTaskTimer.elapsed()));
      _subTaskTimer.restart();
    }
    LOG_STATUS("Loading the data to replace db to: " << DATA_TO_REPLACE_URL << "...");
    OsmMapWriterFactory::write(map, DATA_TO_REPLACE_URL);
    _originalDataSize = (int)map->size();
    LOG_STATUS(
      StringUtils::formatLargeNumber(map->size()) << " elements to replace loaded in: " <<
      StringUtils::millisecondsToDhms(_subTaskTimer.elapsed()));
    _subTaskTimer.restart();
  }

  void _loadReplacementDataDb(const QString& input, const QString& cropBounds = "")
  {
    LOG_STATUS(
      "Loading the replacement data db from: ..." << input <<
      " to: " << _replacementDataUrl << "...");
    OsmMapPtr map(new OsmMap());
    LOG_STATUS("Reading the replacement data from: ..." << input.right(25) << "...");
    OsmMapReaderFactory::read(map, input, false, Status::Unknown2);
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
      LOG_STATUS("Replacement data cropped size: " << StringUtils::formatLargeNumber(map->size()));
      LOG_STATUS(
        "Replacement data cropped in: " <<
        StringUtils::millisecondsToDhms(_subTaskTimer.elapsed()));
      _subTaskTimer.restart();
    }
    LOG_STATUS("Loading the replacement data db to: " << _replacementDataUrl << "...");
    OsmMapWriterFactory::write(map, _replacementDataUrl);
    LOG_STATUS(
      StringUtils::formatLargeNumber(map->size()) << " replacement elements loaded in: " <<
      StringUtils::millisecondsToDhms(_subTaskTimer.elapsed()));
    _subTaskTimer.restart();
  }

  void _cleanup()
  {
    LOG_STATUS("Cleaning up the modified db at: " << DATA_TO_REPLACE_URL << "...");
    ServicesDbTestUtils::deleteDataFromOsmApiTestDatabase();
    LOG_STATUS(
      "Modified db cleaned in: " <<
      StringUtils::millisecondsToDhms(_subTaskTimer.elapsed()));
    _subTaskTimer.restart();

//    LOG_STATUS("Cleaning up the replacement data db at: " << _replacementDataUrl << "...");
//    ServicesDbTestUtils::deleteUser(USER_EMAIL);
//    HootApiDbWriter().deleteMap(_testName);
//    LOG_STATUS(
//      "Replacement data cleaned in: " <<
//      StringUtils::millisecondsToDhms(_subTaskTimer.elapsed()));
//    _subTaskTimer.restart();
  }
};

#ifdef HOOT_HAVE_SERVICES
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ServiceChangesetReplacementGridTest, "glacial");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ServiceChangesetReplacementGridTest, "serial");
#endif

}
