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
 * @copyright Copyright (C) 2021, 2022 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/scoring/MapCompareUtils.h>
#include <hoot/core/io/OsmMapReaderFactory.h>

namespace hoot
{

class MapCompareUtilsTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(MapCompareUtilsTest);
  CPPUNIT_TEST(runBasicTest);
  CPPUNIT_TEST(runEmptyMapTest);
  CPPUNIT_TEST_SUITE_END();

public:

  MapCompareUtilsTest() = default;

  void runBasicTest()
  {
    OsmMapPtr map1 = std::make_shared<OsmMap>();
    OsmMapReaderFactory::read(map1, "test-files/ToyTestA.osm");

    OsmMapPtr map2 = std::make_shared<OsmMap>();
    OsmMapReaderFactory::read(map2, "test-files/ToyTestB.osm");

    CPPUNIT_ASSERT_EQUAL(1000, MapCompareUtils::getAttributeComparisonFinalScore(map1, map2, 1));
    CPPUNIT_ASSERT_EQUAL(976, MapCompareUtils::getGraphComparisonFinalScore(map1, map2));
    CPPUNIT_ASSERT_EQUAL(886, MapCompareUtils::getRasterComparisonFinalScore(map1, map2));
  }

  void runEmptyMapTest()
  {
    OsmMapPtr map1 = std::make_shared<OsmMap>();
    OsmMapReaderFactory::read(map1, "test-files/ToyTestA.osm");

    // Leave this map empty.
    OsmMapPtr map2 = std::make_shared<OsmMap>();

    QString exceptionMsg;
    try
    {
      CPPUNIT_ASSERT_EQUAL(0, MapCompareUtils::getAttributeComparisonFinalScore(map1, map2, 1));
    }
    catch (const EmptyMapInputException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT_EQUAL(
      QString("Empty map input.").toStdString(), exceptionMsg.toStdString());

    try
    {
      CPPUNIT_ASSERT_EQUAL(0, MapCompareUtils::getGraphComparisonFinalScore(map1, map2));
    }
    catch (const EmptyMapInputException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT_EQUAL(
      QString("Empty map input.").toStdString(), exceptionMsg.toStdString());

    try
    {
      CPPUNIT_ASSERT_EQUAL(0, MapCompareUtils::getRasterComparisonFinalScore(map1, map2));
    }
    catch (const EmptyMapInputException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT_EQUAL(
      QString("Empty map input.").toStdString(), exceptionMsg.toStdString());
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(MapCompareUtilsTest, "slow");

}
