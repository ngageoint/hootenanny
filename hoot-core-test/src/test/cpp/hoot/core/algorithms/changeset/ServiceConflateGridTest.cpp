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

// Qt
#include <QElapsedTimer>

namespace hoot
{

static const QString USER_EMAIL = "ServiceConflateGridTest@hoottestcpp.org";
//static const QString DATA_TO_REPLACE_URL = ServicesDbTestUtils::getOsmApiDbUrl().toString();
// enables diff conflate to calculate the difference between the final replaced data and the
// original data used for replacement
//static const bool CALC_DIFF_BETWEEN_REPLACED_AND_REPLACEMENT = false;
// TODO
static const bool WRITE_NON_CONFLATABLE = false;

/*
 * TODO: finish
 */
class ServiceConflateGridTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(ServiceConflateGridTest);

  //CPPUNIT_TEST(orphanedNodes1Test);

  CPPUNIT_TEST_SUITE_END();

public:

  ServiceConflateGridTest() :
  HootTestFixture(
    "test-files/algorithms/changeset/ServiceConflateGridTest",
    "test-output/algorithms/changeset/ServiceConflateGridTest")
  {
    setResetType(ResetAll);
  }

  virtual void setUp()
  {
    HootTestFixture::setUp(); // Be sure the parent cleans out the config before each test.
    _subTaskTimer.start();

  }

  virtual void tearDown()
  {

  }

//  void github4216UniformTest()
//  {
//    // larger AOI version of orphanedNodes2Test; good intermediately sized test dataset

//    _testName = "github4216UniformTest";
//    const QString rootDir = "/home/vagrant/hoot/tmp/4158";
//    const QString outDir = rootDir + "/" + _testName;
//    conf().set(ConfigOptions::getDebugMapsFilenameKey(), outDir + "/debug.osm");
//    QDir(outDir).removeRecursively();
//    QDir().mkpath(outDir);
//    _prepInput(
//      rootDir + "/combined-data/NOMEData.osm", rootDir + "/combined-data/OSMData.osm",
//      "-115.1260,36.1525,-115.0246,36.2227", outDir);

//    ChangesetTaskGridReplacer uut;
//    //uut.setKillAfterNumChangesetDerivations(2);
//    uut.setChangesetsOutputDir(outDir);
//    QString finalOutput = outDir + "/" + _testName + "-out.osm";
//    uut.setWriteFinalOutput(finalOutput);
//    uut.setOriginalDataSize(_originalDataSize);
//    uut.setTagQualityIssues(true);
//    const TaskGrid taskGrid =
//      UniformTaskGridGenerator(
//        "-115.1208,36.1550,-115.0280,36.2182", 2,
//        outDir + "/" + _testName + "-" + "taskGridBounds.osm")
//        .generateTaskGrid();
//    OsmMapPtr outputMap = uut.replace(DATA_TO_REPLACE_URL, _replacementDataUrl, taskGrid);

//    if (WRITE_NON_CONFLATABLE)
//    {
//      _writeNonConflatable(outputMap, finalOutput.replace(".osm", "-non-conflatable.osm"));
//    }
//    if (CALC_DIFF_BETWEEN_REPLACED_AND_REPLACEMENT)
//    {
//      _writeDiffBetweenReplacedAndReplacement(
//        taskGrid.getBounds(), finalOutput.replace(".osm", "-diff.osm"));
//    }

//    CPPUNIT_ASSERT_EQUAL(0, uut.getOutputMetrics().getNumOrphanedNodes());
//    CPPUNIT_ASSERT_EQUAL(0, uut.getOutputMetrics().getNumDisconnectedWays());
//    CPPUNIT_ASSERT_EQUAL(0, uut.getOutputMetrics().getNumEmptyWays());
//    CPPUNIT_ASSERT_EQUAL(28, uut.getOutputMetrics().getNumDuplicateElementPairs());
//  }

private:

  QString _testName;

  QElapsedTimer _subTaskTimer;


};

#ifdef HOOT_HAVE_SERVICES
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ServiceConflateGridTest, "glacial");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ServiceConflateGridTest, "serial");
#endif

}
