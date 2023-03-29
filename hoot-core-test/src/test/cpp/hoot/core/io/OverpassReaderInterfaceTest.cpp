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
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2022-2023 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/io/OverpassReaderInterface.h>

namespace hoot
{

class OverpassReaderInterfaceTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(OverpassReaderInterfaceTest);
  CPPUNIT_TEST(checkUrlTest);
  CPPUNIT_TEST(checkOverpassOutputFormatTest);
  CPPUNIT_TEST(checkOverpassQueryFileTest);
  CPPUNIT_TEST(parseOverpassErrorTest);
  CPPUNIT_TEST_SUITE_END();

public:

  OverpassReaderInterfaceTest() = default;

  const QString overpass_url = "https://overpass-api.de/api/interpreter";

  void checkUrlTest()
  {
    OverpassReaderInterface uut;
    uut.setOverpassUrl(overpass_url);
    CPPUNIT_ASSERT(uut._isOverpass);
    uut.setOverpassUrl(QString(overpass_url).replace("https", "http"));
    CPPUNIT_ASSERT(uut._isOverpass);
    conf().set(ConfigOptions::getOverpassApiHostKey(), "this.is.not.the.right.host");
    uut.setOverpassUrl(overpass_url);
    CPPUNIT_ASSERT(!uut._isOverpass);
    conf().set(ConfigOptions::getOverpassApiHostKey(), ConfigOptions::getOverpassApiHostDefaultValue());
  }

  void checkOverpassOutputFormatTest()
  {
    //  URL query strings
    QString noQuery = "?data=(node({{bbox}});(._;<;););out meta;";
    QString xmlQuery = "?data=[out:xml];(node({{bbox}});(._;<;););out meta;";
    QString jsonQuery = "?data=[out:json];(node({{bbox}});(._;<;););out meta;";
    OverpassReaderInterface uut;
    //  Test out the query strings
    CPPUNIT_ASSERT(uut.isOverpassXml(overpass_url + noQuery));
    CPPUNIT_ASSERT(uut.isOverpassXml(overpass_url + xmlQuery));
    CPPUNIT_ASSERT(uut.isOverpassJson(overpass_url + jsonQuery));
    //  Test out the negative query strings
    CPPUNIT_ASSERT(!uut.isOverpassJson(overpass_url + noQuery));
    CPPUNIT_ASSERT(!uut.isOverpassJson(overpass_url + xmlQuery));
    CPPUNIT_ASSERT(!uut.isOverpassXml(overpass_url + jsonQuery));
    //  Test out the file query
    uut.setQueryFilePath("test-files/io/OsmJsonReaderTest/overpass_query.overpassql");
    CPPUNIT_ASSERT(uut.isOverpassJson(overpass_url));
    CPPUNIT_ASSERT(!uut.isOverpassXml(overpass_url));
  }

  void checkOverpassQueryFileTest()
  {
    OverpassReaderInterface uut;
    QString overpass_expected =
        "[out:json][bbox];\n"
        "(\n"
        "way;\n"
        "relation;\n"
        ");\n"
        "out;\n";
    QString overpass_query = uut._readOverpassQueryFile("test-files/io/OsmJsonReaderTest/overpass_query.overpassql", false);
    HOOT_STR_EQUALS(overpass_expected, overpass_query);
    //  Check a bad filename, disable warnings beforehand
    DisableLog disable;
    HOOT_STR_EQUALS("", uut._readOverpassQueryFile("/this/file/should/never/exist/right"));
    //  Test out removing comments from the query file
    QString comment_query = uut._readOverpassQueryFile("test-files/io/OsmJsonReaderTest/overpass_comments_query.overpassql", false);
    HOOT_STR_EQUALS(overpass_expected, comment_query);
    //  Test out stripping comments and new lines
    comment_query = uut._readOverpassQueryFile("test-files/io/OsmJsonReaderTest/overpass_comments_query.overpassql", true);
    HOOT_STR_EQUALS(overpass_expected.replace("\n", ""), comment_query);
  }

  void parseOverpassErrorTest()
  {
    QString sample_error =
      "<?xml version='1.0' encoding='UTF-8'?>\n"
      "<!DOCTYPE html PUBLIC '-//W3C//DTD XHTML 1.0 Strict//EN' 'http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd'>\n"
      "<html xmlns='http://www.w3.org/1999/xhtml' xml:lang='en' lang='en'>\n"
      "<head>\n"
      "  <meta http-equiv='content-type' content='text/html; charset=utf-8' lang='en'/>\n"
      "  <title>OSM3S Response</title>\n"
      "</head>\n"
      "<body>\n"
      "<p>The data included in this document is from www.openstreetmap.org. The data is made available under ODbL.</p>\n"
      "<p><strong style='color:#FF0000'>Error</strong>: line 1: parse error: Unknown type &quot;xxxx&quot; </p>\n"
      "<p><strong style='color:#FF0000'>Error</strong>: line 1: parse error: ')' expected - ';' found. </p>\n"
      "<p><strong style='color:#FF0000'>Error</strong>: line 1: parse error: Unknown query clause </p>\n"
      "<p><strong style='color:#FF0000'>Error</strong>: line 1: parse error: Unknown type &quot;)&quot; </p>\n"
      "<p><strong style='color:#FF0000'>Error</strong>: line 1: parse error: An empty query is not allowed </p>\n"
      "</body>\n"
      "</html>";
    QString expected_error = "line 1: parse error: Unknown type &quot;xxxx&quot;\n"
                             "line 1: parse error: ')' expected - ';' found.\n"
                             "line 1: parse error: Unknown query clause\n"
                             "line 1: parse error: Unknown type &quot;)&quot;\n"
                             "line 1: parse error: An empty query is not allowed";
    OverpassReaderInterface uut;
    QString parsed_error = uut.parseOverpassError(sample_error);
    HOOT_STR_EQUALS(expected_error, parsed_error);
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(OverpassReaderInterfaceTest, "quick");

}
