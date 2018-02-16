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
 * @copyright Copyright (C) 2012, 2013, 2015, 2016, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/util/Settings.h>
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

class SettingsTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(SettingsTest);
  CPPUNIT_TEST(envTest);
  CPPUNIT_TEST(replaceTest);
  CPPUNIT_TEST(storeTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void envTest()
  {
    Settings uut;
    uut.loadEnvironment();
    char* path = getenv("PATH");
    CPPUNIT_ASSERT_EQUAL(QString(path), uut.getString("PATH"));

    uut.set("mypath", "my path: ${PATH}");
    CPPUNIT_ASSERT_EQUAL(QString("my path: ") + path, uut.getString("mypath"));
  }

  void replaceTest()
  {
    Settings uut;
    uut.loadDefaults();
    uut.set("perty.csm.D", "2");
    uut.set("perty.test.num.runs", 2);
    uut.set("osm.map.writer.factory.writer", "1");
    uut.set("osm.map.reader.factory.reader", "${perty.csm.D}");
    CPPUNIT_ASSERT_EQUAL(QString("2"), uut.getString("osm.map.reader.factory.reader"));

    uut.set("osm.map.reader.factory.reader", "${perty.csm.D} ${osm.map.writer.factory.writer}");
    CPPUNIT_ASSERT_EQUAL(QString("2 1"), uut.getString("osm.map.reader.factory.reader"));

    uut.set("osm.map.reader.factory.reader", "${osm.map.writer.factory.writer} ${osm.map.writer.factory.writer}");
    CPPUNIT_ASSERT_EQUAL(QString("1 1"), uut.getString("osm.map.reader.factory.reader"));

    uut.set("perty.csm.D", "${doesnt.exist}");
    CPPUNIT_ASSERT_EQUAL(QString(""), uut.getString("perty.csm.D"));

    HOOT_STR_EQUALS("", uut.getValue("${doesnt.exist}"));
    HOOT_STR_EQUALS("1", uut.getValue("${osm.map.writer.factory.writer}"));
    HOOT_STR_EQUALS(1, uut.getDoubleValue("${osm.map.writer.factory.writer}"));
    HOOT_STR_EQUALS(2, uut.getDoubleValue("${perty.test.num.runs}"));
  }

  void storeTest()
  {
    Settings uut;
    uut.loadDefaults();
    uut.set("perty.csm.D", "2");
    uut.set("osm.map.writer.factory.writer", "1");
    uut.set("osm.map.reader.factory.reader", "${perty.csm.D}");

    TestUtils::mkpath("test-output/utils");
    uut.storeJson("test-output/utils/SettingsTest.json");

    Settings uut2;
    uut2.loadDefaults();
    uut2.loadJson("test-output/utils/SettingsTest.json");
    HOOT_STR_EQUALS(uut.getString("perty.csm.D"), "2");
    HOOT_STR_EQUALS(uut.getString("osm.map.writer.factory.writer"), "1");
    HOOT_STR_EQUALS(uut.getString("osm.map.reader.factory.reader"), "2");

    Settings uut3;
    uut3.loadDefaults();
    uut3.loadFromString(uut.toString());
    HOOT_STR_EQUALS(uut.getString("perty.csm.D"), "2");
    HOOT_STR_EQUALS(uut.getString("osm.map.writer.factory.writer"), "1");
    HOOT_STR_EQUALS(uut.getString("osm.map.reader.factory.reader"), "2");
  }
};

}

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(hoot::SettingsTest, "quick");

