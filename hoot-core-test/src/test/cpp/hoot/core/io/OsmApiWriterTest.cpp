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
 * @copyright Copyright (C) 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

//  hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/io/OsmApiWriter.h>
#include <hoot/core/util/Log.h>

//  Qt
#include <QNetworkReply>

namespace hoot
{

class OsmApiWriterTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(OsmApiWriterTest);
  CPPUNIT_TEST(runParseStatusTest);
  CPPUNIT_TEST(runParseCapabilitiesTest);
  CPPUNIT_TEST(runCapabilitesTest);
  CPPUNIT_TEST(runParsePermissionsTest);
  /* These tests are for local testing and require additional resources to complete */
//  CPPUNIT_TEST(runPermissionsTest);
//  CPPUNIT_TEST(runChangesetTest);
//  CPPUNIT_TEST(runChangesetConflictTest);
//  CPPUNIT_TEST(oauthTest);
  CPPUNIT_TEST_SUITE_END();

public:

  const QString OSM_API_URL = "https://www.openstreetmap.org";
  const QString ME_API_URL = "http://ec2-34-237-221-226.compute-1.amazonaws.com";

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
    QString xml =
      "<?xml version='1.0' encoding='UTF-8'?>"
      "<osm version='0.6' generator='OpenStreetMap server' copyright='OpenStreetMap and contributors' attribution='https://www.openstreetmap.org/copyright' license='http://opendatacommons.org/licenses/odbl/1-0/'>"
      "  <api>"
      "    <version minimum='0.6' maximum='0.6'/>"
      "    <area maximum='0.25'/>"
      "    <note_area maximum='25'/>"
      "    <tracepoints per_page='5000'/>"
      "    <waynodes maximum='2000'/>"
      "    <changesets maximum_elements='10000'/>"
      "    <timeout seconds='300'/>"
      "    <status database='online' api='online' gpx='online'/>"
      "  </api>"
      "  <policy>"
      "    <imagery>"
      "      <blacklist regex='http://xdworld\\.vworld\\.kr:8080/.*'/>"
      "      <blacklist regex='.*\\.here\\.com[/:].*'/>"
      "    </imagery>"
      "  </policy>"
      "</osm>";
    OsmApiCapabilites capabilities = writer._parseCapabilities(xml);
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
    osm.setUrl(OSM_API_URL);

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
  }

  void runParsePermissionsTest()
  {
    OsmApiWriter writer;
    QString xml =
      "<?xml version='1.0' encoding='UTF-8'?>"
      "<osm version='0.6' generator='OpenStreetMap server'>"
      "  <permissions>"
      "    <permission name='allow_read_prefs'/>"
      "    <permission name='allow_read_gpx'/>"
      "    <permission name='allow_write_api'/>"
      "    <permission name='allow_write_gpx'/>"
      "  </permissions>"
      "</osm>";
    CPPUNIT_ASSERT(writer._parsePermissions(xml));
  }

  void runPermissionsTest()
  {
    QUrl osm;
    osm.setUrl(ME_API_URL);
    osm.setUserInfo("test01:hoottest");

    QList<QString> changesets;
    HootNetworkRequestPtr request(new HootNetworkRequest());
    OsmApiWriter writer(osm, changesets);
    CPPUNIT_ASSERT(writer.validatePermissions(request));
    CPPUNIT_ASSERT_EQUAL(request->getHttpStatus(), 200);
  }

  void runChangesetTest()
  {
    QUrl osm;
    osm.setUrl(ME_API_URL);
    osm.setUserInfo("test01:hoottest");

    QList<QString> changesets;
    changesets.append("test-files/io/OsmChangesetElementTest/ToyTestA.osc");

    OsmApiWriter writer(osm, changesets);

    Settings s;
    s.set("changeset.apidb.max.writers", 2);
    s.set("changeset.apidb.max.size", 10);
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
      changesets.append("test-files/io/OsmChangesetElementTest/ToyTestA.osc");

      OsmApiWriter writer(osm, changesets);

      Settings s;
      //  Force the changeset to give consistent results with no parallelism
      s.set("changeset.apidb.max.writers", 1);
      s.set("changeset.apidb.max.size", 1000);
      writer.setConfiguration(s);

      writer.apply();
    }

    //  Load up the conflict dataset
    {
      QList<QString> changesets;
      changesets.append("test-files/io/OsmChangesetElementTest/ToyTestAConflicts.osc");

      OsmApiWriter writer(osm, changesets);

      Settings s;
      s.set("changeset.apidb.max.writers", 1);
      s.set("changeset.apidb.max.size", 10);
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
        "\t</delete>\n"
        "</osmChange>\n",
        writer.getFailedChangeset());
    }
  }

  void oauthTest()
  {
    QUrl osm;
    osm.setUrl(ME_API_URL);

    QList<QString> changesets;
    changesets.append("test-files/ToyTestA.osm");

    OsmApiWriter writer(osm, changesets);

    Settings s;
    s.set("changeset.apidb.max.writers",        2);
    s.set("changeset.apidb.max.size",           10);
    s.set("hoot.osm.auth.consumer.key",         "<consumer_key_here>");
    s.set("hoot.osm.auth.consumer.secret",      "<consumer_secret_here>");
    s.set("hoot.osm.auth.access.token",         "<access_token_here>");
    s.set("hoot.osm.auth.access.token.secret",  "<access_secret_here>");

    writer.setConfiguration(s);

    writer.apply();
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(OsmApiWriterTest, "quick");
//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(OsmApiWriterTest, "current");

}
