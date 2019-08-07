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
 * @copyright Copyright (C) 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

//  hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/io/OsmApiWriter.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/FileUtils.h>

#include "OsmApiWriterTestServer.h"

//  Qt
#include <QNetworkReply>

//  Run tests locally or against a remote server
#define RUN_LOCAL_TESTS

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
  CPPUNIT_TEST(runRetryConflictsTest);
  /* These tests are for local testing and require additional resources to complete */
//  CPPUNIT_TEST(runChangesetTest);
//  CPPUNIT_TEST(runChangesetTrottleTest);
//  CPPUNIT_TEST(runChangesetConflictTest);
//  CPPUNIT_TEST(oauthTest);
  CPPUNIT_TEST_SUITE_END();

public:

  const QString OSM_API_URL = "https://www.openstreetmap.org";
  const QString ME_API_URL = "http://ec2-34-237-221-226.compute-1.amazonaws.com";
  const QString LOCAL_API_URL = "http://localhost:%1";

  const int PORT_CAPABILITIES = 9800;
  const int PORT_PERMISSIONS = 9801;
  const int PORT_CONFLICTS = 9802;

  OsmApiWriterTest()
    : HootTestFixture("test-files/io/OsmChangesetElementTest/",
                      UNUSED_PATH)
  {
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
    OsmApiCapabilites capabilities = writer._parseCapabilities(OsmApiSampleResponses::SAMPLE_CAPABILITIES);
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
#ifdef RUN_LOCAL_TESTS
    osm.setUrl(LOCAL_API_URL.arg(PORT_CAPABILITIES));
    CapabilitiesTestServer server(PORT_CAPABILITIES);
    server.start();
#else
    osm.setUrl(OSM_API_URL);
#endif

    QList<QString> changesets;
    HootNetworkRequestPtr request(new HootNetworkRequest());
    OsmApiWriter writer(osm, changesets);
    CPPUNIT_ASSERT(writer.queryCapabilities(request));
    CPPUNIT_ASSERT_EQUAL(request->getHttpStatus(), 200);
    HOOT_STR_EQUALS(writer._capabilities.getVersion(), QString("0.6"));
    CPPUNIT_ASSERT_EQUAL(writer._capabilities.getTracepoints(), static_cast<long>(5000));
    CPPUNIT_ASSERT_EQUAL(writer._capabilities.getWayNodes(), static_cast<long>(2000));
    CPPUNIT_ASSERT_EQUAL(writer._capabilities.getChangesets(), static_cast<long>(10000));
    CPPUNIT_ASSERT_EQUAL(writer._capabilities.getTimeout(), static_cast<long>(300));
    CPPUNIT_ASSERT_EQUAL(writer._capabilities.getDatabaseStatus(), OsmApiStatus::ONLINE);
    CPPUNIT_ASSERT_EQUAL(writer._capabilities.getApiStatus(), OsmApiStatus::ONLINE);
    CPPUNIT_ASSERT_EQUAL(writer._capabilities.getGpxStatus(), OsmApiStatus::ONLINE);
#ifdef RUN_LOCAL_TESTS
    server.wait();
#endif
  }

  void runParsePermissionsTest()
  {
    OsmApiWriter writer;
    CPPUNIT_ASSERT(writer._parsePermissions(OsmApiSampleResponses::SAMPLE_PERMISSIONS));
  }

  void runPermissionsTest()
  {
    QUrl osm;
    osm.setUserInfo("test01:hoottest");
#ifdef RUN_LOCAL_TESTS
    osm.setUrl(LOCAL_API_URL.arg(PORT_PERMISSIONS));
    PermissionsTestServer server(PORT_PERMISSIONS);
    server.start();
#else
    osm.setUrl(ME_API_URL);
#endif

    QList<QString> changesets;
    HootNetworkRequestPtr request(new HootNetworkRequest());
    OsmApiWriter writer(osm, changesets);
    CPPUNIT_ASSERT(writer.validatePermissions(request));
    CPPUNIT_ASSERT_EQUAL(request->getHttpStatus(), 200);
#ifdef RUN_LOCAL_TESTS
    server.shutdown();
#endif
  }

  void runChangesetTest()
  {
    QUrl osm;
    osm.setUrl(ME_API_URL);
    osm.setUserInfo("test01:hoottest");

    QList<QString> changesets;
    changesets.append(_inputPath + "ToyTestA.osc");

    OsmApiWriter writer(osm, changesets);

    Settings s;
    s.set(ConfigOptions::getChangesetApidbWritersMaxKey(), 2);
    s.set(ConfigOptions::getChangesetApidbSizeMaxKey(), 10);
    writer.setConfiguration(s);

    writer.apply();
  }

  void runChangesetTrottleTest()
  {
    QUrl osm;
    osm.setUrl(ME_API_URL);
    osm.setUserInfo("test01:hoottest");

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
  }

  void runChangesetConflictTest()
  {
    QUrl osm;
    osm.setUrl(ME_API_URL);
    osm.setUserInfo("test01:hoottest");

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
    }
  }

  void runRetryConflictsTest()
  {
    //  Suppress the OsmApiWriter errors by temporarily changing the log level.
    //  We expect the all of the errors
    Log::WarningLevel logLevel = Log::getInstance().getLevel();
    Log::getInstance().setLevel(Log::Fatal);

    //  Setup the test
    QUrl osm;
    osm.setUrl(LOCAL_API_URL.arg(PORT_CONFLICTS));
    osm.setUserInfo("test01:hoottest");

    //  Kick off the conflict test server
    RetryConflictsTestServer server(PORT_CONFLICTS);
    server.start();

    QList<QString> changesets;
    changesets.append(_inputPath + "ToyTestAConflicts.osc");

    OsmApiWriter writer(osm, changesets);

    Settings s;
    s.set(ConfigOptions::getChangesetApidbWritersMaxKey(), 1);
    s.set(ConfigOptions::getChangesetApidbSizeMaxKey(), 100);
    writer.setConfiguration(s);

    writer.apply();

    //  Wait for the test server to finish
    server.wait();

    Log::getInstance().setLevel(logLevel);

    //  Make sure that some of the changes failed
    CPPUNIT_ASSERT(writer.containsFailed());
    HOOT_STR_EQUALS(
          FileUtils::readFully(_inputPath + "ToyTestAConflicts.osc")
            .replace("timestamp=\"\"", "timestamp=\"\" changeset=\"0\"")
            .replace("    ", "\t"),
      writer.getFailedChangeset());
  }

  void oauthTest()
  {
    QUrl osm;
    osm.setUrl(ME_API_URL);

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
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(OsmApiWriterTest, "quick");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(OsmApiWriterTest, "serial");

}
