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
#include <hoot/core/criterion/ElementIdCriterion.h>

namespace hoot
{

class ElementIdCriterionTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(ElementIdCriterionTest);
  CPPUNIT_TEST(runBasicTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runBasicTest()
  {
    std::shared_ptr<ElementIdCriterion> uut;

    NodePtr node1 =
      std::make_shared<Node>(Status::Unknown1, 1, geos::geom::Coordinate(0.0, 0.0), 15.0);
    NodePtr node2 =
      std::make_shared<Node>(Status::Unknown1, 2, geos::geom::Coordinate(0.0, 5.0), 15.0);
    NodePtr node3 =
      std::make_shared<Node>(Status::Unknown1, 3, geos::geom::Coordinate(0.0, 10.0), 15.0);

    Settings settings;
    settings.set(ConfigOptions::getElementIdCriterionIdsKey(), "node:1;node:2");
    uut = std::make_shared<ElementIdCriterion>();
    uut->setConfiguration(settings);
    CPPUNIT_ASSERT(uut->isSatisfied(node1));
    CPPUNIT_ASSERT(uut->isSatisfied(node2));
    CPPUNIT_ASSERT(!uut->isSatisfied(node3));

    uut = std::make_shared<ElementIdCriterion>(ElementId::node(1));
    CPPUNIT_ASSERT(uut->isSatisfied(node1));
    CPPUNIT_ASSERT(!uut->isSatisfied(node2));

    std::set<ElementId> elementIds;
    elementIds.insert(ElementId::node(1));
    elementIds.insert(ElementId::node(2));
    uut = std::make_shared<ElementIdCriterion>(elementIds);
    CPPUNIT_ASSERT(uut->isSatisfied(node1));
    CPPUNIT_ASSERT(uut->isSatisfied(node2));
    CPPUNIT_ASSERT(!uut->isSatisfied(node3));

    std::set<long> ids;
    ids.insert(1);
    ids.insert(2);
    uut = std::make_shared<ElementIdCriterion>(ElementType::Node, ids);
    CPPUNIT_ASSERT(uut->isSatisfied(node1));
    CPPUNIT_ASSERT(uut->isSatisfied(node2));
    CPPUNIT_ASSERT(!uut->isSatisfied(node3));
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ElementIdCriterionTest, "quick");

}
