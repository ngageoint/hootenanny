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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/conflate/poi-polygon/extractors/PoiPolygonAlphaShapeDistanceExtractor.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
using namespace hoot;

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

#include "../../../TestUtils.h"

namespace hoot
{

class PoiPolygonAlphaShapeDistanceExtractorTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(PoiPolygonAlphaShapeDistanceExtractorTest);
  CPPUNIT_TEST(runBasicTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runBasicTest()
  {
    PoiPolygonAlphaShapeDistanceExtractor uut;

    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map,
      "test-files/conflate/poi-polygon/extractors/PoiPolygonAlphaShapeDistanceExtractorTest1.osm",
      false,
      Status::Unknown1);
    OsmMapReaderFactory::read(
      map,
      "test-files/conflate/poi-polygon/extractors/PoiPolygonAlphaShapeDistanceExtractorTest2.osm",
      false,
      Status::Unknown2);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      0.0,
      uut.extract(
        *map,
        TestUtils::getElementWithTag(map, "name", "test1"), //poly
        TestUtils::getElementWithTag(map, "name", "test2")), //poi
      0.0001);
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(PoiPolygonAlphaShapeDistanceExtractorTest, "quick");

}
