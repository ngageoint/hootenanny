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
 * @copyright Copyright (C) 2012, 2013, 2014, 2015, 2016, 2017, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/criterion/ChainCriterion.h>
#include <hoot/core/criterion/ElementTypeCriterion.h>
#include <hoot/core/criterion/TagCriterion.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/algorithms/splitter/IntersectionSplitter.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/ops/CopyMapSubsetOp.h>

#include <hoot/core/TestUtils.h>

namespace hoot
{

class IntersectionSplitterTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(IntersectionSplitterTest);
  CPPUNIT_TEST(runTest);
  CPPUNIT_TEST(runTestSimple);
  CPPUNIT_TEST(runRelationMemberOrderTest);
  CPPUNIT_TEST(runRelationMemberOrder2Test);
  CPPUNIT_TEST_SUITE_END();

public:

  IntersectionSplitterTest() :
  HootTestFixture(
    "test-files/algorithms/splitter/IntersectionSplitterTest/",
    "test-output/algorithms/splitter/IntersectionSplitterTest/")
  {
    setResetType(ResetBasic);
  }

  void runTest()
  {
    OsmXmlReader reader;
    OsmMapPtr map(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.read(_inputPath + "IntersectionSplitter.osm", map);

    IntersectionSplitter::splitIntersections(map);

    OsmXmlWriter writer;
    writer.setIncludeCompatibilityTags(false);
    writer.write(map, _outputPath + "IntersectionSplitterTest.osm");

    HOOT_FILE_EQUALS(_inputPath + "IntersectionSplitterOut.osm",
                     _outputPath + "IntersectionSplitterTest.osm");
  }

  void runTestSimple()
  {
    OsmXmlReader reader;
    OsmMapPtr map(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.read(_inputPath + "SimpleSplitter.osm", map);

    IntersectionSplitter::splitIntersections(map);

    OsmXmlWriter writer;
    writer.setIncludeCompatibilityTags(false);
    writer.write(map, _outputPath + "SimpleSplitterOutput.osm");

    HOOT_FILE_EQUALS(_inputPath + "SimpleSplitterExpected.osm",
                     _outputPath + "SimpleSplitterOutput.osm");
  }

  void runRelationMemberOrderTest()
  {
    // This test checks that relation member order is preserved after splitting. Examine the route
    // relation with tag, "ref=36". Its members consist of intersection split roads, but they should
    // retain the ordering of the input relation.

    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map, "test-files/cases/reference/unifying/multiple/highway-3906/Input1.osm");

    IntersectionSplitter::splitIntersections(map);

    MapProjector::projectToWgs84(map);
    OsmMapWriterFactory::write(map, _outputPath + "runRelationMemberOrderTestOut.osm");

    HOOT_FILE_EQUALS(
      _inputPath + "runRelationMemberOrderTestOut.osm",
      _outputPath + "runRelationMemberOrderTestOut.osm");
  }

  void runRelationMemberOrder2Test()
  {
    // This is similar to runRelationMemberOrderTest but with more of the original relation intact.
    // runRelationMemberOrderTest is a simpler starting point than this for debugging issues.

    OsmMapPtr rawMap(new OsmMap());
    OsmMapReaderFactory::read(
      rawMap, "test-files/cmd/glacial/RelationMergeTest/input1.osm");

    OsmMapPtr filteredMap(new OsmMap());
    CopyMapSubsetOp(
      rawMap,
      std::make_shared<ChainCriterion>(
        std::make_shared<RelationCriterion>("route"),
        std::make_shared<TagCriterion>("ref", "36")))
      .apply(filteredMap);
    LOG_VART(filteredMap->size());

    IntersectionSplitter::splitIntersections(filteredMap);

    MapProjector::projectToWgs84(filteredMap);
    OsmMapWriterFactory::write(filteredMap, _outputPath + "runRelationMemberOrder2TestOut.osm");

    HOOT_FILE_EQUALS(
      _inputPath + "runRelationMemberOrder2TestOut.osm",
      _outputPath + "runRelationMemberOrder2TestOut.osm");
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(IntersectionSplitterTest, "quick");

}
