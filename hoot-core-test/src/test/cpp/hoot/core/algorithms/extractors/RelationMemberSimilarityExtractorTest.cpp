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
 * @copyright Copyright (C) 2020, 2021 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/algorithms/extractors/RelationMemberSimilarityExtractor.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/elements/MapProjector.h>

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

namespace hoot
{

class RelationMemberSimilarityExtractorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(RelationMemberSimilarityExtractorTest);
  CPPUNIT_TEST(runTest);
  CPPUNIT_TEST_SUITE_END();

public:

  RelationMemberSimilarityExtractorTest() :
  HootTestFixture(
    "test-files/algorithms/extractors/RelationMemberSimilarityExtractorTest/",
    "test-output/algorithms/extractors/RelationMemberSimilarityExtractorTest/")
  {
  }

  void runTest()
  {
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map, "test-files/conflate/merging/RelationMergerTest/runTestInput.osm", true);

    RelationMemberSimilarityExtractor uut;
    const double score =
      uut.extract(
        *map, map->getElement(ElementId(ElementType::Relation, 7387470)),
        map->getElement(ElementId(ElementType::Relation, -1)));

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.233, score, 1e-3);
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(RelationMemberSimilarityExtractorTest, "quick");

}
