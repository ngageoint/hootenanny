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
 * @copyright Copyright (C) 2020, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/conflate/point/PoiSearchRadius.h>
#include <hoot/core/util/FileUtils.h>

namespace hoot
{

class PoiSearchRadiusTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(PoiSearchRadiusTest);
  CPPUNIT_TEST(runFileTest);
  CPPUNIT_TEST(runJsonTest);
  CPPUNIT_TEST(runToStringTest);
  CPPUNIT_TEST(runMissingFileTest);
  CPPUNIT_TEST(runBadJsonTest);
  CPPUNIT_TEST(runMissingKeyTest);
  CPPUNIT_TEST(runMissingDistanceTest);
  CPPUNIT_TEST_SUITE_END();

public:

  PoiSearchRadiusTest()
    : HootTestFixture("test-files/conflate/point/PoiSearchRadiusTest/", UNUSED_PATH)
  {
  }

  void runFileTest()
  {
    _validateRadii(PoiSearchRadius::readSearchRadii(_inputPath + "input.json"));
  }

  void runJsonTest()
  {
    _validateRadii(PoiSearchRadius::readSearchRadii(FileUtils::readFully(_inputPath + "input.json")));
  }

  void runToStringTest()
  {
    PoiSearchRadius uut("foo", "bar", 10);
    HOOT_STR_EQUALS("Key: foo, Value: bar, distance: 10", uut.toString());
  }

  void runMissingFileTest()
  {
    QString exceptionMsg;
    try
    {
      PoiSearchRadius::readSearchRadii("blah.json");
    }
    catch (const IllegalArgumentException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT_EQUAL(
      QString("POI to POI search radii file does not exist.").toStdString(),
      exceptionMsg.toStdString());
  }

  void runBadJsonTest()
  {
    QString exceptionMsg;
    try
    {
      PoiSearchRadius::readSearchRadii(_inputPath + "runBadJsonTest-in.json");
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT(exceptionMsg.startsWith("Error parsing JSON"));
  }

  void runMissingKeyTest()
  {
    QString exceptionMsg;
    try
    {
      PoiSearchRadius::readSearchRadii(_inputPath + "runMissingKeyTest-in.json");
    }
    catch (const IllegalArgumentException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT_EQUAL(
      QString("Missing 'key' in POI search radius entry.").toStdString(),
      exceptionMsg.toStdString());
  }

  void runMissingDistanceTest()
  {
    QString exceptionMsg;
    try
    {
      PoiSearchRadius::readSearchRadii(_inputPath + "runMissingDistanceTest-in.json");
    }
    catch (const IllegalArgumentException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT_EQUAL(
      QString("Missing 'distance' in POI search radius entry.").toStdString(),
      exceptionMsg.toStdString());
  }

private:

  void _validateRadii(const QList<PoiSearchRadius>& radii)
  {
    CPPUNIT_ASSERT_EQUAL(4, radii.size());

    PoiSearchRadius radius1 = radii.at(0);
    HOOT_STR_EQUALS("amenity", radius1.getKey().toStdString());
    HOOT_STR_EQUALS("", radius1.getValue().toStdString());
    CPPUNIT_ASSERT_EQUAL(200, radius1.getDistance());

    PoiSearchRadius radius2 = radii.at(1);
    HOOT_STR_EQUALS("amenity", radius2.getKey().toStdString());
    HOOT_STR_EQUALS("grave_yard", radius2.getValue().toStdString());
    CPPUNIT_ASSERT_EQUAL(1000, radius2.getDistance());

    PoiSearchRadius radius3 = radii.at(2);
    HOOT_STR_EQUALS(MetadataTags::Building(), radius3.getKey().toStdString());
    HOOT_STR_EQUALS("", radius3.getValue().toStdString());
    CPPUNIT_ASSERT_EQUAL(200, radius3.getDistance());

    PoiSearchRadius radius4 = radii.at(3);
    HOOT_STR_EQUALS(MetadataTags::Building(), radius4.getKey().toStdString());
    HOOT_STR_EQUALS("hospital", radius4.getValue().toStdString());
    CPPUNIT_ASSERT_EQUAL(500, radius4.getDistance());
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(PoiSearchRadiusTest, "quick");

}
