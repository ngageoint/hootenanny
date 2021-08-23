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
 * @copyright Copyright (C) 2019, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/ops/ElementIdToVersionMapper.h>

#include <hoot/core/io/OsmMapReaderFactory.h>

namespace hoot
{

class ElementIdToVersionMapperTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(ElementIdToVersionMapperTest);
  CPPUNIT_TEST(runBasicTest);
  CPPUNIT_TEST(runNoVerionsTest);
  CPPUNIT_TEST_SUITE_END();

public:

  ElementIdToVersionMapperTest() :
  HootTestFixture(
    "test-files/ops/ElementIdToVersionMapper", "test-output/ops/ElementIdToVersionMapper")
  {
  }

  void runBasicTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    OsmMapReaderFactory::read(map, true, true, _inputPath + "/runBasicTest-in.osm");

    ElementIdToVersionMapper uut;
    uut.apply(map);
    const QMap<ElementId, long> mappings = uut.getMappings();

    CPPUNIT_ASSERT_EQUAL(42, mappings.size());
    CPPUNIT_ASSERT_EQUAL(1L, mappings[ElementId(ElementType::Node, -5144884)]);
    CPPUNIT_ASSERT_EQUAL(2L, mappings[ElementId(ElementType::Node, -5144896)]);
    CPPUNIT_ASSERT_EQUAL(1L, mappings[ElementId(ElementType::Way, -5144955)]);
    CPPUNIT_ASSERT_EQUAL(2L, mappings[ElementId(ElementType::Way, -5144956)]);
    CPPUNIT_ASSERT_EQUAL(1L, mappings[ElementId(ElementType::Relation, -5144964)]);
    CPPUNIT_ASSERT_EQUAL(2L, mappings[ElementId(ElementType::Relation, -5144965)]);
  }

  void runNoVerionsTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    OsmMapReaderFactory::read(map, true, true, "test-files/ToyTestA.osm");

    ElementIdToVersionMapper uut;
    uut.apply(map);
    const QList<long> mappings = uut.getMappings().values();

    CPPUNIT_ASSERT_EQUAL(40, mappings.size());
    // If the elements in the input have no versions, all mapped versions will equal zero.
    int zeroVersionCtr = 0;
    for (QList<long>::const_iterator mappingItr = mappings.constBegin();
         mappingItr != mappings.constEnd(); ++mappingItr)
    {
      if (*mappingItr == 0)
      {
        zeroVersionCtr++;
      }
    }
    CPPUNIT_ASSERT_EQUAL(40, zeroVersionCtr);
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ElementIdToVersionMapperTest, "quick");

}
