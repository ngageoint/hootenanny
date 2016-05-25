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
 * @copyright Copyright (C) 2012, 2013, 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/util/OsmJsonReader.h>
#include <hoot/core/util/Log.h>
using namespace hoot;

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Qt
#include <QDir>

#include "../TestUtils.h"

namespace hoot
{

class OsmJsonReaderTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(OsmJsonReaderTest);
  CPPUNIT_TEST(nodeTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void nodeTest()
  {
    QString testJsonStr =
     "{                                      \n"
     " 'version': 0.6,                       \n"
     " 'generator': 'Overpass API',          \n"
     " 'osm3s': {                            \n"
     "   'timestamp_osm_base': 'date',       \n"
     "   'copyright': 'The data included in this document is from www.openstreetmap.org. It has there been collected by a large group of contributors. For individual attribution of each item please refer to http://www.openstreetmap.org/api/0.6/[node|way|relation]/#id/history'\n"
     " },                                    \n"
     " 'elements': [                         \n"
     " {                                     \n"
     "   'type': 'node',                     \n"
     "   'id': -1,                           \n"
     "   'lat': 2.0,                         \n"
     "   'lon': -3.0                         \n"
     " },                                    \n"
     " {                                     \n"
     "   'type': 'node',                     \n"
     "   'id': -2,                           \n"
     "   'lat': 2.0,                         \n"
     "   'lon': -3.0,                        \n"
     "   'timestamp': '2010-01-01T00:00:00Z',\n"
     "   'version': 4,                       \n"
     "   'changeset': 5,                     \n"
     "   'user': 'somebody',                 \n"
     "   'uid': 6                            \n"
     " },                                    \n"
     " {                                     \n"
     "   'type': 'node',                     \n"
     "   'id': -3,                           \n"
     "   'lat': 2.0,                         \n"
     "   'lon': -3.0,                        \n"
     "   'tags': {                           \n"
     "     'highway': 'bus_stop',            \n"
     "     'name': 'Main Street'             \n"
     "   }                                   \n"
     " }                                     \n"
     "]                                      \n"
     "}                                      \n";


    OsmJsonReader uut;
    OsmMapPtr pMap = uut.loadFromString(testJsonStr);

    int i = 0;
    i++;

    /*
    CPPUNIT_ASSERT_EQUAL(QString(path), uut.getString("PATH"));

    uut.set("mypath", "my path: ${PATH}");
    CPPUNIT_ASSERT_EQUAL(QString("my path: ") + path, uut.getString("mypath"));
    */
  }

  /*
  void replaceTest()
  {
    Settings uut;
    uut.loadDefaults();
    uut.set("perty.algorithm", "2");
    uut.set("perty.test.num.runs", 2);
    uut.set("osm.map.writer.factory.writer", "1");
    uut.set("osm.map.reader.factory.reader", "${perty.algorithm}");
    CPPUNIT_ASSERT_EQUAL(QString("2"), uut.getString("osm.map.reader.factory.reader"));

    uut.set("osm.map.reader.factory.reader", "${perty.algorithm} ${osm.map.writer.factory.writer}");
    CPPUNIT_ASSERT_EQUAL(QString("2 1"), uut.getString("osm.map.reader.factory.reader"));

    uut.set("osm.map.reader.factory.reader", "${osm.map.writer.factory.writer} ${osm.map.writer.factory.writer}");
    CPPUNIT_ASSERT_EQUAL(QString("1 1"), uut.getString("osm.map.reader.factory.reader"));

    uut.set("perty.algorithm", "${doesnt.exist}");
    CPPUNIT_ASSERT_EQUAL(QString(""), uut.getString("perty.algorithm"));

    HOOT_STR_EQUALS("", uut.getValue("${doesnt.exist}"));
    HOOT_STR_EQUALS("1", uut.getValue("${osm.map.writer.factory.writer}"));
    HOOT_STR_EQUALS(1, uut.getDoubleValue("${osm.map.writer.factory.writer}"));
    HOOT_STR_EQUALS(2, uut.getDoubleValue("${perty.test.num.runs}"));
  }

  void storeTest()
  {
    Settings uut;
    uut.loadDefaults();
    uut.set("perty.algorithm", "2");
    uut.set("osm.map.writer.factory.writer", "1");
    uut.set("osm.map.reader.factory.reader", "${perty.algorithm}");

    QDir().mkpath("test-output/utils");
    uut.storeJson("test-output/utils/SettingsTest.json");

    Settings uut2;
    uut2.loadDefaults();
    uut2.loadJson("test-output/utils/SettingsTest.json");
    HOOT_STR_EQUALS(uut.getString("perty.algorithm"), "2");
    HOOT_STR_EQUALS(uut.getString("osm.map.writer.factory.writer"), "1");
    HOOT_STR_EQUALS(uut.getString("osm.map.reader.factory.reader"), "2");

    Settings uut3;
    uut3.loadDefaults();
    uut3.loadFromString(uut.toString());
    HOOT_STR_EQUALS(uut.getString("perty.algorithm"), "2");
    HOOT_STR_EQUALS(uut.getString("osm.map.writer.factory.writer"), "1");
    HOOT_STR_EQUALS(uut.getString("osm.map.reader.factory.reader"), "2");
  }
  */
};

}

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(hoot::OsmJsonReaderTest, "current");
//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(hoot::OsmJsonReaderTest, "quick");

