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
 * @copyright Copyright (C) 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/criterion/InBoundsCriterion.h>
#include <hoot/core/geometry/GeometryUtils.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/io/OsmMapReaderFactory.h>

namespace hoot
{

class InBoundsCriterionTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(InBoundsCriterionTest);
  CPPUNIT_TEST(runInBoundsStrictTest);
  CPPUNIT_TEST(runInBoundsLenientTest);
  CPPUNIT_TEST(runOutOfBoundsStrictTest);
  CPPUNIT_TEST(runOutOfBoundsLenientTest);
  CPPUNIT_TEST_SUITE_END();

public:

  InBoundsCriterionTest() :
  HootTestFixture(
    "test-files/criterion/InBoundsCriterion", "test-output/criterion/InBoundsCriterion")
  {
  }

  void runInBoundsStrictTest()
  {
    geos::geom::Envelope bounds(-104.9007, -104.8994, 38.8540, 38.8552);
    OsmMapWriterFactory::write(
      GeometryUtils::createMapFromBounds(bounds),
      _outputPath + "/runInBoundsStrictTest-bounds.osm");
    OsmMapPtr map = std::make_shared<OsmMap>();
    OsmMapReaderFactory::read(map, "test-files/ToyTestA.osm", true);

    InBoundsCriterion uut(true);
    uut.setBounds(bounds);
    uut.setOsmMap(map.get());

    // both elements completely within the bounding box
    CPPUNIT_ASSERT(uut.isSatisfied(map->getNode(-1669781)));
    CPPUNIT_ASSERT(uut.isSatisfied(map->getWay(-1669799)));
  }

  void runInBoundsLenientTest()
  {   
    geos::geom::Envelope bounds(-104.9007, -104.8994, 38.8540, 38.8552);
    OsmMapWriterFactory::write(
      GeometryUtils::createMapFromBounds(bounds),
      _outputPath + "/runInBoundsLenientTest-bounds.osm");
    OsmMapPtr map = std::make_shared<OsmMap>();
    OsmMapReaderFactory::read(map, "test-files/ToyTestA.osm", true);

    InBoundsCriterion uut(false);
    uut.setBounds(bounds);
    uut.setOsmMap(map.get());

    // node within the bounding box
    CPPUNIT_ASSERT(uut.isSatisfied(map->getNode(-1669781)));
    // way crosses the bounding box
    CPPUNIT_ASSERT(uut.isSatisfied(map->getWay(-1669797)));
  }

  void runOutOfBoundsStrictTest()
  {    
    geos::geom::Envelope bounds(-104.9007, -104.8994, 38.8540, 38.8552);
    OsmMapWriterFactory::write(
      GeometryUtils::createMapFromBounds(bounds),
      _outputPath + "/runOutOfBoundsStrictTest-bounds.osm");
    OsmMapPtr map = std::make_shared<OsmMap>();
    OsmMapReaderFactory::read(map, "test-files/ToyTestA.osm", true);

    InBoundsCriterion uut(true);
    uut.setBounds(bounds);
    uut.setOsmMap(map.get());

    // node completely outside the bounding box
    CPPUNIT_ASSERT(!uut.isSatisfied(map->getNode(-1669783)));
    // way crosses the bounding box
    CPPUNIT_ASSERT(!uut.isSatisfied(map->getWay(-1669797)));
  }

  void runOutOfBoundsLenientTest()
  { 
    geos::geom::Envelope bounds(-104.90002, -104.89934, 38.85413, 38.85523);
    OsmMapWriterFactory::write(
      GeometryUtils::createMapFromBounds(bounds),
      _outputPath + "/runOutOfBoundsLenientTest-bounds.osm");
    OsmMapPtr map = std::make_shared<OsmMap>();
    OsmMapReaderFactory::read(map, "test-files/ToyTestA.osm", true);

    InBoundsCriterion uut(false);
    uut.setBounds(bounds);
    uut.setOsmMap(map.get());

    // both elements completely outside the bounding box
    CPPUNIT_ASSERT(!uut.isSatisfied(map->getNode(-1669779)));
    CPPUNIT_ASSERT(!uut.isSatisfied(map->getWay(-1669799)));
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(InBoundsCriterionTest, "quick");

}
