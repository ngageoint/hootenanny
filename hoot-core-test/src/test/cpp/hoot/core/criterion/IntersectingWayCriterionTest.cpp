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
 * @copyright Copyright (C) 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/criterion/IntersectingWayCriterion.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>

namespace hoot
{

class IntersectingWayCriterionTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(IntersectingWayCriterionTest);
  CPPUNIT_TEST(runBasicTest);
  CPPUNIT_TEST(runConfTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runBasicTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    OsmMapReaderFactory::read(map, "test-files/DcGisRoads.osm");

    QSet<long> sourceWayIds;
    sourceWayIds.insert(-13148);
    sourceWayIds.insert(-9169);
    IntersectingWayCriterion uut(sourceWayIds, map);

    CPPUNIT_ASSERT(uut.isSatisfied(map->getElement(ElementId::way(-10908))));
    CPPUNIT_ASSERT(uut.isSatisfied(map->getElement(ElementId::way(-10212))));
    CPPUNIT_ASSERT(!uut.isSatisfied(map->getElement(ElementId::way(-11134))));
  }

  void runConfTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    OsmMapReaderFactory::read(map, "test-files/DcGisRoads.osm");

    QStringList sourceWayIds;
    sourceWayIds.append("-13148");
    sourceWayIds.append("-9169");
    Settings conf;
    conf.set(ConfigOptions::getIntersectingWayCriterionSourceWayIdsKey(), sourceWayIds);
    IntersectingWayCriterion uut(map);
    uut.setConfiguration(conf);

    CPPUNIT_ASSERT(uut.isSatisfied(map->getElement(ElementId::way(-10908))));
    CPPUNIT_ASSERT(uut.isSatisfied(map->getElement(ElementId::way(-10212))));
    CPPUNIT_ASSERT(!uut.isSatisfied(map->getElement(ElementId::way(-11134))));
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(IntersectingWayCriterionTest, "quick");

}
