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
#include <hoot/core/conflate/point/PoiMatchDistance.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/FileUtils.h>

namespace hoot
{

class PoiMatchDistanceTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(PoiMatchDistanceTest);
  CPPUNIT_TEST(runFileTest);
  CPPUNIT_TEST(runJsonTest);
  CPPUNIT_TEST_SUITE_END();

public:

  PoiMatchDistanceTest() :
  HootTestFixture("test-files/conflate/point/PoiMatchDistanceTest/", UNUSED_PATH)
  {
    setResetType(ResetBasic);
  }

  void runFileTest()
  {
    _validateDistances(PoiMatchDistance::readDistances(_inputPath + "input.json"));
  }

  void runJsonTest()
  {
    _validateDistances(
      PoiMatchDistance::readDistances(FileUtils::readFully(_inputPath + "input.json")));
  }

private:

  void _validateDistances(const QList<PoiMatchDistance>& distances)
  {
    CPPUNIT_ASSERT_EQUAL(4, distances.size());

    PoiMatchDistance distance1 = distances.at(0);
    HOOT_STR_EQUALS("amenity", distance1.getKey().toStdString());
    HOOT_STR_EQUALS("", distance1.getValue().toStdString());
    CPPUNIT_ASSERT_EQUAL(100, distance1.getMaxMatchDistance());
    CPPUNIT_ASSERT_EQUAL(200, distance1.getMaxReviewDistance());

    PoiMatchDistance distance2 = distances.at(1);
    HOOT_STR_EQUALS("amenity", distance2.getKey().toStdString());
    HOOT_STR_EQUALS("grave_yard", distance2.getValue().toStdString());
    CPPUNIT_ASSERT_EQUAL(500, distance2.getMaxMatchDistance());
    CPPUNIT_ASSERT_EQUAL(1000, distance2.getMaxReviewDistance());

    PoiMatchDistance distance3 = distances.at(2);
    HOOT_STR_EQUALS("building", distance3.getKey().toStdString());
    HOOT_STR_EQUALS("", distance3.getValue().toStdString());
    CPPUNIT_ASSERT_EQUAL(100, distance3.getMaxMatchDistance());
    CPPUNIT_ASSERT_EQUAL(200, distance3.getMaxReviewDistance());

    PoiMatchDistance distance4 = distances.at(3);
    HOOT_STR_EQUALS("building", distance4.getKey().toStdString());
    HOOT_STR_EQUALS("hospital", distance4.getValue().toStdString());
    CPPUNIT_ASSERT_EQUAL(300, distance4.getMaxMatchDistance());
    CPPUNIT_ASSERT_EQUAL(500, distance4.getMaxReviewDistance());
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(PoiMatchDistanceTest, "quick");

}
