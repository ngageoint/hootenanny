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
 * @copyright Copyright (C) 2021-2023 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/elements/OsmUtils.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/util/FileUtils.h>

namespace hoot
{

class OsmUtilsTest : public HootTestFixture
{
    CPPUNIT_TEST_SUITE(OsmUtilsTest);
    CPPUNIT_TEST(runElementsDetailStringTest);
    CPPUNIT_TEST(runRelationDetailStringTest);
    CPPUNIT_TEST_SUITE_END();

public:

  OsmUtilsTest()
    : HootTestFixture("test-files/elements/OsmUtilsTest/", "test-output/elements/OsmUtilsTest/")
  {
  }

  void runElementsDetailStringTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    OsmMapReaderFactory::read(map, "test-files/cmd/quick/BuildingOutlineUpdateOpTest/BuildingOutlineUpdateOpTest.osm");

    std::vector<ElementPtr> elements;
    NodePtr node = map->getNode(-13395959);
    elements.push_back(node);
    WayPtr way = map->getWay(-13396440);
    elements.push_back(way);
    RelationPtr relation = map->getRelation(-2);
    elements.push_back(relation);

    HOOT_STR_EQUALS(
      FileUtils::readFully(_inputPath + "OsmUtilsTest-runElementsDetailStringTest-out").trimmed(),
                           OsmUtils::getElementsDetailString(elements, map).trimmed());
  }

  void runRelationDetailStringTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    OsmMapReaderFactory::read(map, "test-files/cmd/quick/BuildingOutlineUpdateOpTest/BuildingOutlineUpdateOpTest.osm");

    ConstRelationPtr relation = map->getRelation(-2);

    HOOT_STR_EQUALS(
      FileUtils::readFully(_inputPath + "OsmUtilsTest-runRelationDetailStringTest-out").trimmed(),
                           OsmUtils::getRelationDetailString(relation, map).trimmed());
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(OsmUtilsTest, "quick");

}
