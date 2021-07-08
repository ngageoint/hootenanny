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
 * @copyright Copyright (C) 2015, 2016, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/algorithms/extractors//poi-polygon/PoiPolygonAlphaShapeDistanceExtractor.h>
#include <hoot/core/elements/MapUtils.h>
#include <hoot/core/io/OsmMapReaderFactory.h>

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

namespace hoot
{

class PoiPolygonAlphaShapeDistanceExtractorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(PoiPolygonAlphaShapeDistanceExtractorTest);
  CPPUNIT_TEST(runBasicTest);
  CPPUNIT_TEST_SUITE_END();

public:

  PoiPolygonAlphaShapeDistanceExtractorTest()
    : HootTestFixture("test-files/algorithms/extractors/poi-polygon/",
                      UNUSED_PATH)
  {
  }

  void runBasicTest()
  {
    PoiPolygonAlphaShapeDistanceExtractor uut;

    OsmMapPtr map = std::make_shared<OsmMap>();
    OsmMapReaderFactory::read(
      map,
      _inputPath + "PoiPolygonAlphaShapeDistanceExtractorTest1.osm",
      false,
      Status::Unknown1);
    OsmMapReaderFactory::read(
      map,
      _inputPath + "PoiPolygonAlphaShapeDistanceExtractorTest2.osm",
      false,
      Status::Unknown2);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      0.0,
      uut.extract(
        *map,
        MapUtils::getFirstElementWithTag(map, "name", "test1"), //poly
        MapUtils::getFirstElementWithTag(map, "name", "test2")), //poi
      0.0001);
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(PoiPolygonAlphaShapeDistanceExtractorTest, "quick");

}
