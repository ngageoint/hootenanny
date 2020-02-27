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
 * @copyright Copyright (C) 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

//  hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/io/OsmApiWriter.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/util/HootNetworkUtils.h>
#include <hoot/core/util/Log.h>

#include "OsmApiWriterTestServer.h"

//  Run tests against a local test server
#define RUN_LOCAL_TEST_SERVER
//  Run tests against a local copy of OSM API
//#define RUN_LOCAL_OSM_API_SERVER

namespace hoot
{

class OsmApiWriterTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(OsmApiWriterTest);
  CPPUNIT_TEST(runParseStatusTest);
  CPPUNIT_TEST(runParseCapabilitiesTest);
  CPPUNIT_TEST(runCapabilitesTest);
  CPPUNIT_TEST(runParsePermissionsTest);
  CPPUNIT_TEST(runPermissionsTest);
#ifdef RUN_LOCAL_TEST_SERVER
  CPPUNIT_TEST(runRetryConflictsTest);
  CPPUNIT_TEST(runVersionConflictResolutionTest);
  CPPUNIT_TEST(runChangesetOutputTest);
#endif
  /* These tests are for local testing and require additional resources to complete */
#ifdef RUN_LOCAL_OSM_API_SERVER
  CPPUNIT_TEST(runChangesetTest);
  CPPUNIT_TEST(runChangesetTrottleTest);
  CPPUNIT_TEST(runChangesetConflictTest);
  CPPUNIT_TEST(oauthTest);
#endif
  CPPUNIT_TEST_SUITE_END();

public:

  const QString OSM_API_URL = "https://www.openstreetmap.org";
  const QString LOCAL_TEST_API_URL = "http://localhost:%1";
#ifdef RUN_LOCAL_OSM_API_SERVER
  //  Replace with local OSM server IP/name
  const QString LOCAL_OSM_API_URL = "http://local-osm";
#endif
  const QString TEST_USER_INFO = "test01:hoottest";

  /** Separate port numbers so that tests can run in parallel */
  const int PORT_CAPABILITIES = 9800;
  const int PORT_PERMISSIONS =  9801;
  const int PORT_CONFLICTS =    9802;
  const int PORT_VERSION =      9803;
  const int PORT_DEBUG_OUTPUT = 9804;

  OsmApiWriterTest()
    : HootTestFixture("test-files/io/OsmChangesetElementTest/",
                      "test-output/io/OsmChangesetElementTest/")
  {
    setResetType(ResetBasic);
  }

  void runParseStatusTest()
  {
    OsmApiWriter writer;
    CPPUNIT_ASSERT_EQUAL(writer._parseStatus("online"), OsmApiStatus::ONLINE);
    CPPUNIT_ASSERT_EQUAL(writer._parseStatus("readonly"), OsmApiStatus::READONLY);
    CPPUNIT_ASSERT_EQUAL(writer._parseStatus("offline"), OsmApiStatus::OFFLINE);
  }

  void runParseCapabilitiesTest()
  {
    OsmApiWriter writer;
    OsmApiCapabilites capabilities = writer._parseCapabilities(OsmApiSampleRequestResponse::SAMPLE_CAPABILITIES_RESPONSE);
    HOOT_STR_EQUALS(capabilities.getVersion(), QString("0.6"));
    CPPUNIT_ASSERT_EQUAL(capabilities.getTracepoints(), static_cast<long>(5000));
    CPPUNIT_ASSERT_EQUAL(capabilities.getWayNodes(), static_cast<long>(2000));
    CPPUNIT_ASSERT_EQUAL(capabilities.getChangesets(), static_cast<long>(10000));
    CPPUNIT_ASSERT_EQUAL(capabilities.getTimeout(), static_cast<long>(300));
    CPPUNIT_ASSERT_EQUAL(capabilities.getDatabaseStatus(), OsmApiStatus::ONLINE);
    CPPUNIT_ASSERT_EQUAL(capabilities.getApiStatus(), OsmApiStatus::ONLINE);
    CPPUNIT_ASSERT_EQUAL(capabilities.getGpxStatus(), OsmApiStatus::ONLINE);
  }

  void runCapabilitesTest()
  {
    QUrl osm;
#ifdef RUN_LOCAL_TEST_SERVER
    osm.setUrl(LOCAL_TEST_API_URL.arg(PORT_CAPABILITIES));
    CapabilitiesTestServer server(PORT_CAPABILITIES);
    server.start();
#else
    osm.setUrl(OSM_API_URL);
#endif

    QList<QString> changesets;
    HootNetworkRequestPtr request(new HootNetworkRequest());
    OsmApiWriter writer(osm, changesets);
    CPPUNIT_ASSERT(writer.queryCapabilities(request));
    CPPUNIT_ASSERT_EQUAL(request->getHttpStatus(), HttpResponseCode::HTTP_OK);
    HOOT_STR_EQUALS(writer._capabilities.getVersion(), QString("0.6"));
    CPPUNIT_ASSERT_EQUAL(writer._capabilities.getTracepoints(), static_cast<long>(5000));
    CPPUNIT_ASSERT_EQUAL(writer._capabilities.getWayNodes(), static_cast<long>(2000));
    CPPUNIT_ASSERT_EQUAL(writer._capabilities.getChangesets(), static_cast<long>(10000));
    CPPUNIT_ASSERT_EQUAL(writer._capabilities.getTimeout(), static_cast<long>(300));
    CPPUNIT_ASSERT_EQUAL(writer._capabilities.getDatabaseStatus(), OsmApiStatus::ONLINE);
    CPPUNIT_ASSERT_EQUAL(writer._capabilities.getApiStatus(), OsmApiStatus::ONLINE);
    CPPUNIT_ASSERT_EQUAL(writer._capabilities.getGpxStatus(), OsmApiStatus::ONLINE);
#ifdef RUN_LOCAL_TEST_SERVER
    server.shutdown();
#endif
  }

  void runParsePermissionsTest()
  {
    OsmApiWriter writer;
    CPPUNIT_ASSERT(writer._parsePermissions(OsmApiSampleRequestResponse::SAMPLE_PERMISSIONS_RESPONSE));
  }

  void runPermissionsTest()
  {
    QUrl osm;
    osm.setUserInfo(TEST_USER_INFO);
#ifdef RUN_LOCAL_TEST_SERVER
    osm.setUrl(LOCAL_TEST_API_URL.arg(PORT_PERMISSIONS));
    PermissionsTestServer server(PORT_PERMISSIONS);
    server.start();
#else
    osm.setUrl(ME_API_URL);
#endif

    QList<QString> changesets;
    HootNetworkRequestPtr request(new HootNetworkRequest());
    OsmApiWriter writer(osm, changesets);
    CPPUNIT_ASSERT(writer.validatePermissions(request));
    CPPUNIT_ASSERT_EQUAL(request->getHttpStatus(), HttpResponseCode::HTTP_OK);
#ifdef RUN_LOCAL_TEST_SERVER
    server.shutdown();
#endif
  }

  void runChangesetTest()
  {
#ifdef RUN_LOCAL_OSM_API_SERVER
    QUrl osm;
    osm.setUrl(LOCAL_OSM_API_URL);
    osm.setUserInfo(TEST_USER_INFO);

    QList<QString> changesets;
    changesets.append(_inputPath + "ToyTestA.osc");

    OsmApiWriter writer(osm, changesets);

    Settings s;
    s.set(ConfigOptions::getChangesetApidbWritersMaxKey(), 2);
    s.set(ConfigOptions::getChangesetApidbSizeMaxKey(), 10);
    writer.setConfiguration(s);

    writer.apply();
#endif
  }

  void runChangesetTrottleTest()
  {
#ifdef RUN_LOCAL_OSM_API_SERVER
    QUrl osm;
    osm.setUrl(LOCAL_OSM_API_URL);
    osm.setUserInfo(TEST_USER_INFO);

    QList<QString> changesets;
    changesets.append(_inputPath + "ToyTestA.osc");

    OsmApiWriter writer(osm, changesets);

    Settings s;
    s.set(ConfigOptions::getChangesetApidbWritersMaxKey(), 2);
    s.set(ConfigOptions::getChangesetApidbSizeMaxKey(), 2);
    s.set(ConfigOptions::getChangesetApidbWritersThrottleKey(), true);
    s.set(ConfigOptions::getChangesetApidbWritersThrottleTimeKey(), 2);
    writer.setConfiguration(s);

    writer.apply();
#endif
  }

  void runChangesetConflictTest()
  {
#ifdef RUN_LOCAL_OSM_API_SERVER
    QUrl osm;
    osm.setUrl(LOCAL_OSM_API_URL);
    osm.setUserInfo(TEST_USER_INFO);

    //  Load up the all-create ToyTestA
    {
      QList<QString> changesets;
      changesets.append(_inputPath + "ToyTestA.osc");

      OsmApiWriter writer(osm, changesets);

      Settings s;
      //  Force the changeset to give consistent results with no parallelism
      s.set(ConfigOptions::getChangesetApidbWritersMaxKey(), 1);
      s.set(ConfigOptions::getChangesetApidbSizeMaxKey(), 1000);
      writer.setConfiguration(s);

      writer.apply();
    }

    //  Load up the conflict dataset
    {
      QList<QString> changesets;
      changesets.append(_inputPath + "ToyTestAConflicts.osc");

      OsmApiWriter writer(osm, changesets);
      writer.setErrorPathname(_outputPath + "ChangesetConflictOutput-error.osc");

      Settings s;
      s.set(ConfigOptions::getChangesetApidbWritersMaxKey(), 1);
      s.set(ConfigOptions::getChangesetApidbSizeMaxKey(), 10);
      writer.setConfiguration(s);

      writer.apply();

      //  Make sure that some of the changes failed
      CPPUNIT_ASSERT(writer.containsFailed());
      HOOT_STR_EQUALS(
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<osmChange version=\"0.6\" generator=\"hootenanny\">\n"
        "\t<delete>\n"
        "\t\t<node id=\"-1\" version=\"0\" lat=\"38.8549321261880536\" lon=\"-104.8979050333482093\" timestamp=\"\" changeset=\"0\">\n"
        "\t\t\t<tag k=\"node\" v=\"Should fail\"/>\n"
        "\t\t</node>\n"
        "\t\t<node id=\"2\" version=\"1\" lat=\"38.8549524185660573\" lon=\"-104.8987388916486054\" timestamp=\"\" changeset=\"0\"/>\n"
        "\t</delete>\n"
        "</osmChange>\n",
        writer.getFailedChangeset());
      //  Check the changeset error file
      HOOT_FILE_EQUALS( _inputPath + "ChangesetConflictExpected-error.osc",
                       _outputPath + "ChangesetConflictOutput-error.osc");
    }
#endif
  }

  void runRetryConflictsTest()
  {
#ifdef RUN_LOCAL_TEST_SERVER
    //  Suppress the OsmApiWriter errors by temporarily changing the log level
    //  when the log level is Info or above because we expect the all of the errors.
    //  Below Info is Debug and Trace, those are set because we want to see everything
    Log::WarningLevel logLevel = Log::getInstance().getLevel();
    if (Log::getInstance().getLevel() >= Log::Info)
      Log::getInstance().setLevel(Log::Fatal);

    //  Setup the test
    QUrl osm;
    osm.setUrl(LOCAL_TEST_API_URL.arg(PORT_CONFLICTS));
    osm.setUserInfo(TEST_USER_INFO);

    //  Kick off the conflict test server
    RetryConflictsTestServer server(PORT_CONFLICTS);
    server.start();

    QList<QString> changesets;
    changesets.append(_inputPath + "ToyTestAConflicts.osc");

    OsmApiWriter writer(osm, changesets);
    writer.setErrorPathname(_outputPath + "RetryConflictOutput-error.osc");

    Settings s;
    s.set(ConfigOptions::getChangesetApidbWritersMaxKey(), 1);
    s.set(ConfigOptions::getChangesetApidbSizeMaxKey(), 100);
    writer.setConfiguration(s);

    writer.apply();

    //  Wait for the test server to finish
    server.shutdown();

    Log::getInstance().setLevel(logLevel);

    //  Make sure that some of the changes failed
    CPPUNIT_ASSERT(writer.containsFailed());
    HOOT_STR_EQUALS(
          FileUtils::readFully(_inputPath + "ToyTestAConflicts.osc")
            .replace("timestamp=\"\"", "timestamp=\"\" changeset=\"0\"")
            .replace("    ", "\t"),
      writer.getFailedChangeset());
    //  Check the stats
    checkStats(writer.getStats(), 3, 2, 0, 2, 1, 2, 5);
    //  Check the changeset error file
    HOOT_FILE_EQUALS( _inputPath + "RetryConflictExpected-error.osc",
                     _outputPath + "RetryConflictOutput-error.osc");
#endif
  }

  void runVersionConflictResolutionTest()
  {
#ifdef RUN_LOCAL_TEST_SERVER
    //  Suppress the OsmApiWriter errors by temporarily changing the log level
    //  when the log level is Info or above because we expect the all of the errors.
    //  Below Info is Debug and Trace, those are set because we want to see everything
    Log::WarningLevel logLevel = Log::getInstance().getLevel();
    if (Log::getInstance().getLevel() >= Log::Info)
      Log::getInstance().setLevel(Log::Fatal);

    //  Setup the test
    QUrl osm;
    osm.setUrl(LOCAL_TEST_API_URL.arg(PORT_VERSION));
    osm.setUserInfo(TEST_USER_INFO);

    //  Kick off the version conflict test server
    RetryVersionTestServer server(PORT_VERSION);
    server.start();

    OsmApiWriter writer(osm, OsmApiSampleRequestResponse::SAMPLE_CHANGESET_REQUEST);

    Settings s;
    s.set(ConfigOptions::getChangesetApidbWritersMaxKey(), 1);
    s.set(ConfigOptions::getChangesetApidbSizeMaxKey(), 100);
    writer.setConfiguration(s);

    writer.apply();

    //  Wait for the test server to finish
    server.shutdown();

    Log::getInstance().setLevel(logLevel);

    //  Make sure that none of the changes failed
    CPPUNIT_ASSERT(!writer.containsFailed());
    //  Check the stats
    checkStats(writer.getStats(), 0, 4, 0, 0, 4, 0, 0);
#endif
  }

  void oauthTest()
  {
#ifdef RUN_LOCAL_OSM_API_SERVER
    QUrl osm;
    osm.setUrl(LOCAL_OSM_API_URL);

    QList<QString> changesets;
    changesets.append("test-files/ToyTestA.osm");

    OsmApiWriter writer(osm, changesets);

    Settings s;
    s.set(ConfigOptions::getChangesetApidbWritersMaxKey(),      2);
    s.set(ConfigOptions::getChangesetApidbSizeMaxKey(),         10);
    s.set(ConfigOptions::getHootOsmAuthConsumerKeyKey(),        "<consumer_key_here>");
    s.set(ConfigOptions::getHootOsmAuthConsumerSecretKey(),     "<consumer_secret_here>");
    s.set(ConfigOptions::getHootOsmAuthAccessTokenKey(),        "<access_token_here>");
    s.set(ConfigOptions::getHootOsmAuthAccessTokenSecretKey(),  "<access_secret_here>");

    writer.setConfiguration(s);

    writer.apply();
#endif
  }

  void runChangesetOutputTest()
  {
#ifdef RUN_LOCAL_TEST_SERVER
    //  Setup the test
    QUrl osm;
    osm.setUrl(LOCAL_TEST_API_URL.arg(PORT_DEBUG_OUTPUT));
    osm.setUserInfo(TEST_USER_INFO);

    //  Kick off the file output test server
    ChangesetOutputTestServer server(PORT_DEBUG_OUTPUT);
    server.start();

    OsmApiWriter writer(osm, OsmApiSampleRequestResponse::SAMPLE_CHANGESET_REQUEST);

    Settings s;
    s.set(ConfigOptions::getChangesetApidbWritersMaxKey(), 1);
    s.set(ConfigOptions::getChangesetApidbSizeMaxKey(), 2);
    s.set(ConfigOptions::getChangesetApidbWriterDebugOutputKey(), true);
    s.set(ConfigOptions::getChangesetApidbWriterDebugOutputPathKey(), _outputPath);
    writer.setConfiguration(s);
    writer.apply();

    //  Wait for the test server to finish
    server.shutdown();

    //  Make sure that none of the changes failed
    CPPUNIT_ASSERT(!writer.containsFailed());
    //  Check the stats
    checkStats(writer.getStats(), 0, 4, 0, 0, 4, 0, 0);
    //  Compare the files
    HOOT_FILE_EQUALS( _inputPath + "ChangesetOutput-Request--1.osc",
                     _outputPath + "OsmApiWriter-000001-00001-Request--000.osc");
    HOOT_FILE_EQUALS( _inputPath + "ChangesetOutput-Response-1.osc",
                     _outputPath + "OsmApiWriter-000001-00001-Response-200.osc");
    HOOT_FILE_EQUALS( _inputPath + "ChangesetOutput-Request--2.osc",
                     _outputPath + "OsmApiWriter-000002-00001-Request--000.osc");
    HOOT_FILE_EQUALS( _inputPath + "ChangesetOutput-Response-2.osc",
                     _outputPath + "OsmApiWriter-000002-00001-Response-200.osc");
#endif
  }

  void checkStats(QList<SingleStat> stats,
                  int nodes, int ways, int relations,
                  int created, int modified, int deleted,
                  int errors)
  {
    for (int i = 0; i < stats.size(); ++i)
    {
      SingleStat stat = stats[i];
      if (stat.name == "Total Nodes in Changeset")
        testStat(stat, nodes);
      else if (stat.name == "Total Ways in Changeset")
        testStat(stat, ways);
      else if (stat.name == "Total Relations in Changeset")
        testStat(stat, relations);
      else if (stat.name == "Total Elements Created")
        testStat(stat, created);
      else if (stat.name == "Total Elements Modified")
        testStat(stat, modified);
      else if (stat.name == "Total Elements Deleted")
        testStat(stat, deleted);
      else if (stat.name == "Total Errors")
        testStat(stat, errors);
    }
  }

  void testStat(SingleStat stat, int value)
  {
    HOOT_STR_EQUALS(QString("%1: %2").arg(stat.name).arg(value), stat.toString());
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(OsmApiWriterTest, "quick");

}
