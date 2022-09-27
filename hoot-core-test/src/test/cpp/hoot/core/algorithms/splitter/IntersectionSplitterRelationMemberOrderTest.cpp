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
 * @copyright Copyright (C) 2012, 2013, 2014, 2015, 2016, 2017, 2018, 2019, 2021, 2022 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/algorithms/splitter/IntersectionSplitter.h>
#include <hoot/core/criterion/RelationCriterion.h>
#include <hoot/core/criterion/IntersectingWayCriterion.h>
#include <hoot/core/criterion/NetworkTypeCriterion.h>
#include <hoot/core/criterion/OrCriterion.h>
#include <hoot/core/criterion/TagCriterion.h>
#include <hoot/core/elements/ElementIdUtils.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/ops/CopyMapSubsetOp.h>
#include <hoot/core/TestUtils.h>

namespace hoot
{

/*
 * Moved this out of IntersectionSplitterTest due to its runtime.
 */
class IntersectionSplitterRelationMemberOrderTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(IntersectionSplitterRelationMemberOrderTest);
  CPPUNIT_TEST(runRelationMemberOrderTest);
  CPPUNIT_TEST(runRelationMemberOrder2Test);
  CPPUNIT_TEST_SUITE_END();

public:

  IntersectionSplitterRelationMemberOrderTest()
    : HootTestFixture("test-files/algorithms/splitter/IntersectionSplitterRelationMemberOrderTest/",
                      "test-output/algorithms/splitter/IntersectionSplitterRelationMemberOrderTest/")
  {
  }

  void runRelationMemberOrderTest()
  {
    // This test checks that relation member order is preserved after splitting. Examine the route
    // relation with tag, "ref=36". Its members consist of intersection split roads, but they should
    // retain the ordering of the input relation.

    OsmMapPtr map = std::make_shared<OsmMap>();
    OsmMapReaderFactory::read(map, "test-files/cases/reference/unifying/multiple/highway-3906/Input1.osm");

    IntersectionSplitter::splitIntersections(map);

    MapProjector::projectToWgs84(map);
    OsmMapWriterFactory::write(map, _outputPath + "runRelationMemberOrderTestOut.osm");

    HOOT_FILE_EQUALS( _inputPath + "runRelationMemberOrderTestOut.osm",
                     _outputPath + "runRelationMemberOrderTestOut.osm");
  }

  void runRelationMemberOrder2Test()
  {
    // This tests the member ordering of a relation that is a superset of the one in
    // runRelationMemberOrderTest, so a little more complex. Its also identical to one in
    // RelationMergeTest, but its easier to debug member ordering issues within this test.

    OsmMapPtr rawMap = std::make_shared<OsmMap>();
    OsmMapReaderFactory::read(rawMap, "test-files/cmd/glacial/RelationMergeTest/input2.osm");

    // We're only interested in seeing the splitting done on this one relation in order to cut down
    // on processing time.
    ElementCriterionPtr relationCrit =
      std::make_shared<ChainCriterion>(std::make_shared<RelationCriterion>("route"), std::make_shared<TagCriterion>("ref", "36"));

    // Filter the input map down to a temp map with just the relation in question.
    OsmMapPtr tempMap = std::make_shared<OsmMap>();
    CopyMapSubsetOp(rawMap, relationCrit).apply(tempMap);
    LOG_VART(tempMap->size());
    const RelationMap& relations = tempMap->getRelations();
    RelationPtr relation = relations.begin()->second;

    // Get all the road member IDs for the relation.
    const QSet<long> roadMemberIds = ElementIdUtils::elementIdsToIds(relation->getMemberIds(ElementType::Way));
    LOG_VART(roadMemberIds);

    // Create a criterion for finding all ways that intersect the member roads.
    ElementCriterionPtr intersectingCrit = std::make_shared<IntersectingWayCriterion>(roadMemberIds, rawMap);

    // Create a criterion for what types of features are splittable (same used by
    // IntersectionSplitter).
    ElementCriterionPtr intersectionSplittableCrit = std::make_shared<NetworkTypeCriterion>(rawMap);

    // Filter the map to process down to the relation we're examining and its members plus all
    // splittable features that intersect those members.
    OsmMapPtr filteredMap = std::make_shared<OsmMap>();
    CopyMapSubsetOp(
      rawMap,
      std::make_shared<OrCriterion>(
        relationCrit,
        std::make_shared<ChainCriterion>(intersectingCrit, intersectionSplittableCrit)))
      .apply(filteredMap);
    LOG_VART(filteredMap->size());

    IntersectionSplitter::splitIntersections(filteredMap);

    MapProjector::projectToWgs84(filteredMap);
    OsmMapWriterFactory::write(filteredMap, _outputPath + "runRelationMemberOrder2TestOut.osm");

    HOOT_FILE_EQUALS( _inputPath + "runRelationMemberOrder2TestOut.osm",
                     _outputPath + "runRelationMemberOrder2TestOut.osm");
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(IntersectionSplitterRelationMemberOrderTest, "glacial");

}
