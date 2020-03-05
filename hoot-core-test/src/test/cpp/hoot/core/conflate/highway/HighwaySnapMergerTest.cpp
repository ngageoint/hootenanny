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
 * @copyright Copyright (C) 2014, 2015, 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// GEOS
#include <geos/geom/LineString.h>

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/algorithms/subline-matching/MaximalSublineStringMatcher.h>
#include <hoot/core/conflate/highway/HighwayExpertClassifier.h>
#include <hoot/core/conflate/highway/HighwayMatch.h>
#include <hoot/core/conflate/highway/HighwaySnapMerger.h>
#include <hoot/core/conflate/matching/MatchThreshold.h>
#include <hoot/core/elements/ElementConverter.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/io/OsmJsonWriter.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/io/OsmJsonReader.h>

// Tgs
#include <tgs/StreamUtils.h>

using namespace geos::geom;
using namespace std;

namespace hoot
{

class HighwaySnapMergerTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(HighwaySnapMergerTest);
  CPPUNIT_TEST(runReverseTest);
  CPPUNIT_TEST(runSimpleTest);
  CPPUNIT_TEST(runSmallScrapTest);
  CPPUNIT_TEST(runTagsTest);
  CPPUNIT_TEST(runTagsSplitTest);
  CPPUNIT_TEST(runTagsNoRelationTest);
  CPPUNIT_TEST(reviewMarkingTest);
  CPPUNIT_TEST_SUITE_END();

public:

  HighwaySnapMergerTest() :
  HootTestFixture(
    "test-files/conflate/highway/HighwaySnapMergerTest/",
    "test-output/conflate/highway/HighwaySnapMergerTest/")
  {
    setResetType(ResetBasic);
  }

  OsmMapPtr createMap()
  {
    OsmMapPtr map(new OsmMap());
    OGREnvelope env;
    env.MinX = 0;
    env.MinY = 0;
    env.MaxX = 1;
    env.MaxY = 1;
    MapProjector::projectToOrthographic(map, env);

    return map;
  }

  /**
   * Create two ways like this:
   *
   * x and o signifiy Unknown1 and Unknown2, respectively.
   *
   *    x--->---x
   * o---<---o
   *
   * Should create a conflated bit in the middle.
   *
   */
  void runReverseTest()
  {
    OsmMapPtr map = createMap();

    Coordinate w1c[] = { Coordinate(50, 0), Coordinate(150, 0), Coordinate::getNull() };
    WayPtr w1 = TestUtils::createWay(map, Status::Unknown1, w1c);

    Coordinate w2c[] = { Coordinate(100, 0), Coordinate(0, 0), Coordinate::getNull() };
    WayPtr w2 = TestUtils::createWay(map, Status::Unknown2, w2c);

    std::shared_ptr<MaximalSublineStringMatcher> sublineMatcher(new MaximalSublineStringMatcher());
    sublineMatcher->setMinSplitSize(5.0);
    sublineMatcher->setMaxRelevantAngle(toRadians(60.0));

    set<pair<ElementId, ElementId>> pairs;
    pairs.insert(pair<ElementId, ElementId>(w1->getElementId(), w2->getElementId()));

    HighwaySnapMerger merger(pairs, sublineMatcher);

    vector<pair<ElementId, ElementId>> replaced;
    merger.apply(map, replaced);

    ElementConverter ec(map);
    HOOT_STR_EQUALS("[2]{(Way(-1), Way(-5)), (Way(-2), Way(-7))}", replaced);
    HOOT_STR_EQUALS("LINESTRING (50.0000000000000000 0.0000000000000000, 100.0000000000000000 0.0000000000000000)",
      ec.convertToLineString(map->getWay(-1))->toString());
    HOOT_STR_EQUALS("LINESTRING (100.0000000000000000 0.0000000000000000, 150.0000000000000000 0.0000000000000000)",
      ec.convertToLineString(map->getWay(-5))->toString());
    HOOT_STR_EQUALS("LINESTRING (50.0000000000000000 0.0000000000000000, 0.0000000000000000 0.0000000000000000)",
      ec.convertToLineString(map->getWay(-7))->toString());
    HOOT_STR_EQUALS("way(-1)\n"
                    "nodes: [2]{-1, -6}\n"
                    "tags: cached envelope: 0.0000000000000000,-1.0000000000000000,0.0000000000000000,-1.0000000000000000\n"
                    "status: conflated\n"
                    "version: 0\n"
                    "visible: 1\n"
                    "circular error: 15\n",
                    map->getWay(-1)->toString());
    HOOT_STR_EQUALS("way(-5)\n"
                    "nodes: [2]{-6, -2}\n"
                    "tags: cached envelope: 0.0000000000000000,-1.0000000000000000,0.0000000000000000,-1.0000000000000000\n"
                    "status: unknown1\n"
                    "version: 0\n"
                    "visible: 1\n"
                    "circular error: 15\n"
                    "parent id: (-1)\n",
                    map->getWay(-5)->toString());
    HOOT_STR_EQUALS("way(-7)\n"
                    "nodes: [2]{-1, -4}\n"
                    "tags: cached envelope: 0.0000000000000000,-1.0000000000000000,0.0000000000000000,-1.0000000000000000\n"
                    "status: unknown2\n"
                    "version: 0\n"
                    "visible: 1\n"
                    "circular error: 15\n",
                    map->getWay(-7)->toString());
  }

  /**
   * Creates two ways like this:
   *
   * x and o signifiy Unknown1 and Unknown2, respectively.
   *
   * x-------x
   * o-------o
   *
   * These two ways should simply be merged w/o any scraps, etc.
   */
  void runSimpleTest()
  {
    OsmMapPtr map = createMap();

    Coordinate w1c[] = { Coordinate(0, 0), Coordinate(100, 0), Coordinate::getNull() };
    WayPtr w1 = TestUtils::createWay(map, Status::Unknown1, w1c);

    Coordinate w2c[] = { Coordinate(0, 0), Coordinate(100, 0), Coordinate::getNull() };
    WayPtr w2 = TestUtils::createWay(map, Status::Unknown2, w2c);

    std::shared_ptr<MaximalSublineStringMatcher> sublineMatcher(new MaximalSublineStringMatcher());
    sublineMatcher->setMinSplitSize(5.0);
    sublineMatcher->setMaxRelevantAngle(toRadians(60.0));

    set<pair<ElementId, ElementId>> pairs;
    pairs.insert(pair<ElementId, ElementId>(w1->getElementId(), w2->getElementId()));

    HighwaySnapMerger merger(pairs, sublineMatcher);

    vector<pair<ElementId, ElementId>> replaced;
    merger.apply(map, replaced);

    HOOT_STR_EQUALS(0, replaced.size());
    HOOT_STR_EQUALS(1, map->getWays().size());
    HOOT_STR_EQUALS("way(-1)\n"
                    "nodes: [2]{-1, -2}\n"
                    "tags: cached envelope: 0.0000000000000000,-1.0000000000000000,0.0000000000000000,-1.0000000000000000\n"
                    "status: conflated\n"
                    "version: 0\n"
                    "visible: 1\n"
                    "circular error: 15\n",
                    map->getWay(-1)->toString());
  }

  /**
   * Creates two ways like this:
   *
   * x and o signifiy Unknown1 and Unknown2, respectively.
   *
   * x----x
   * o-------o
   *
   * These two ways should simply be merged w/o any scraps, etc.
   */
  void runSmallScrapTest()
  {
    OsmMapPtr map = createMap();

    Coordinate w1c[] = { Coordinate(0, 5), Coordinate(60, 5), Coordinate::getNull() };
    WayPtr w1 = TestUtils::createWay(map, Status::Unknown1, w1c);

    Coordinate w2c[] = { Coordinate(0, 0), Coordinate(100, 0), Coordinate::getNull() };
    WayPtr w2 = TestUtils::createWay(map, Status::Unknown2, w2c);

    std::shared_ptr<MaximalSublineStringMatcher> sublineMatcher(new MaximalSublineStringMatcher());
    sublineMatcher->setMinSplitSize(5.0);
    sublineMatcher->setMaxRelevantAngle(toRadians(60.0));

    set<pair<ElementId, ElementId>> pairs;
    pairs.insert(pair<ElementId, ElementId>(w1->getElementId(), w2->getElementId()));

    HighwaySnapMerger merger(pairs, sublineMatcher);

    vector<pair<ElementId, ElementId>> replaced;
    merger.apply(map, replaced);

    // there should be one element replaced
    HOOT_STR_EQUALS("[1]{(Way(-2), Way(-6))}", replaced);
    // the end of Way:-4 should match up with Way:-6
    HOOT_STR_EQUALS("[2]{-1, -2}", map->getWay(-4)->getNodeIds());
    HOOT_STR_EQUALS("[2]{-2, -4}", map->getWay(-6)->getNodeIds());
  }

  /**
   * This is a nasty situation that turns up from time to time when a way with one set of tags
   * is grouped in a relation to make a highway with another set of tags. I personally think
   * this is a bad idea, but we work with the data we get...
   *
   * In this example we have a foot path that goes on top of a wall (x) that is being matched with
   * another path (o).
   *
   *      footway relation
   *       /         \
   *     x---x------wall------x
   * o-------------o
   *
   * The expected output:
   *
   *              footway relation
   *                     |
   *    -x---x-wall-x---wall--x
   * o-/   \    /
   *        \  /
   *  conflated path relation
   */
  void runTagsTest()
  {
    OsmMapPtr map = createMap();

    Coordinate w1c[] = { Coordinate(20, 5), Coordinate(60, 5), Coordinate::getNull() };
    WayPtr w1 = TestUtils::createWay(map, Status::Unknown1, w1c);
    w1->getTags()["uuid"] = "w1";

    Coordinate w2c[] = { Coordinate(60, 5), Coordinate(150, 5), Coordinate::getNull() };
    WayPtr w2 = TestUtils::createWay(map, Status::Unknown1, w2c);
    w2->getTags()["barrier"] = "wall";
    w2->getTags()["uuid"] = "wall";

    Coordinate w3c[] = { Coordinate(-30, 0), Coordinate(100, 0), Coordinate::getNull() };
    WayPtr w3 = TestUtils::createWay(map, Status::Unknown2, w3c);
    w3->getTags()["highway"] = "path";
    w3->getTags()["uuid"] = "w3";

    RelationPtr r(new Relation(Status::Unknown1, 0, 15, MetadataTags::RelationMultilineString()));
    r->addElement("", w1);
    r->addElement("", w2);
    r->setTag("uuid", "r");
    r->setTag("highway", "footway");
    map->addElement(r);

    std::shared_ptr<MaximalSublineStringMatcher> sublineMatcher(new MaximalSublineStringMatcher());
    sublineMatcher->setMinSplitSize(5.0);
    sublineMatcher->setMaxRelevantAngle(toRadians(60.0));

    set<pair<ElementId, ElementId>> pairs;
    pairs.insert(pair<ElementId, ElementId>(r->getElementId(), w3->getElementId()));

    HighwaySnapMerger merger(pairs, sublineMatcher);

    vector<pair<ElementId, ElementId>> replaced;
    merger.apply(map, replaced);

    const QString testFileName = "runTagsTest.json";
    OsmJsonWriter writer;
    writer.setIncludeCompatibilityTags(false);
    writer.open(_outputPath + testFileName);
    MapProjector::projectToWgs84(map);
    writer.write(map);
    writer.close();
    HOOT_FILE_EQUALS(_inputPath + testFileName, _outputPath + testFileName);
  }

  /**
   * In this case we have a road that is split into two roads and a new relation is created for
   * the split pieces. We need to make sure the tags get moved around properly.
   *
   * x-------w1--------x
   *      o--w2---o
   *
   * The expected output:
   *
   *     w1 relation
   *    /           \
   * x----x-w1;w2-x----x
   */
  void runTagsSplitTest()
  {
    OsmMapPtr map = createMap();

    Coordinate w1c[] = { Coordinate(0, 5), Coordinate(100, 5), Coordinate::getNull() };
    WayPtr w1 = TestUtils::createWay(map, Status::Unknown1, w1c);
    w1->getTags()["highway"] = "road";
    w1->getTags()["uuid"] = "w1";

    Coordinate w2c[] = { Coordinate(40, 0), Coordinate(60, 0), Coordinate::getNull() };
    WayPtr w2 = TestUtils::createWay(map, Status::Unknown2, w2c);
    w2->getTags()["highway"] = "path";
    w2->getTags()["uuid"] = "w2";

    std::shared_ptr<MaximalSublineStringMatcher> sublineMatcher(new MaximalSublineStringMatcher());
    sublineMatcher->setMinSplitSize(5.0);
    sublineMatcher->setMaxRelevantAngle(toRadians(60.0));

    set<pair<ElementId, ElementId>> pairs;
    pairs.insert(pair<ElementId, ElementId>(w1->getElementId(), w2->getElementId()));

    HighwaySnapMerger merger(pairs, sublineMatcher);

    vector<pair<ElementId, ElementId>> replaced;
    merger.apply(map, replaced);

    const QString testFileName = "runTagsSplitTest.json";
    OsmJsonWriter writer;
    writer.setIncludeCompatibilityTags(false);
    writer.open(_outputPath + testFileName);
    MapProjector::projectToWgs84(map);
    writer.write(map);
    writer.close();
    HOOT_FILE_EQUALS(_inputPath + testFileName, _outputPath + testFileName);
  }

  /**
   * In this case we have a road that is split into two roads and a new relation is created for
   * the split pieces. We need to make sure the tags get moved around properly.
   *
   * x--w1---x
   * o--------------o
   *         |
   *   w2 relation
   *
   * The expected output:
   *
   * x-w1;w2-x
   *         o--w2--o
   */
  void runTagsNoRelationTest()
  {
    OsmMapPtr map = createMap();

    Coordinate w1c[] = { Coordinate(0, 5), Coordinate(50, 5), Coordinate::getNull() };
    WayPtr w1 = TestUtils::createWay(map, Status::Unknown1, w1c);
    w1->getTags()["highway"] = "road";
    w1->getTags()["uuid"] = "w1";

    Coordinate w2c[] = { Coordinate(0, 0), Coordinate(100, 0), Coordinate::getNull() };
    WayPtr w2 = TestUtils::createWay(map, Status::Unknown2, w2c);

    RelationPtr r(new Relation(Status::Unknown2, 0, 15, MetadataTags::RelationMultilineString()));
    r->addElement("", w2);
    r->setTag("uuid", "r");
    r->setTag("highway", "footway");
    map->addElement(r);

    std::shared_ptr<MaximalSublineStringMatcher> sublineMatcher(new MaximalSublineStringMatcher());
    sublineMatcher->setMinSplitSize(5.0);
    sublineMatcher->setMaxRelevantAngle(toRadians(60.0));

    set<pair<ElementId, ElementId>> pairs;
    pairs.insert(pair<ElementId, ElementId>(w1->getElementId(), r->getElementId()));

    HighwaySnapMerger merger(pairs, sublineMatcher);

    vector<pair<ElementId, ElementId>> replaced;
    merger.apply(map, replaced);

    const QString testFileName = "runTagsNoRelationTest.json";
    OsmJsonWriter writer;
    writer.setIncludeCompatibilityTags(false);
    writer.open(_outputPath + testFileName);
    MapProjector::projectToWgs84(map);
    writer.write(map);
    writer.close();
    HOOT_FILE_EQUALS(_inputPath + testFileName, _outputPath + testFileName);
  }

  //simple test to make sure review note/type strings don't get interchanged
  void reviewMarkingTest()
  {
    OsmMapPtr map = createMap();

    Coordinate w1c[] = { Coordinate(0, 0), Coordinate(100, 0), Coordinate::getNull() };
    WayPtr w1 = TestUtils::createWay(map, Status::Unknown1, w1c);

    Coordinate w2c[] = { Coordinate(0, 0), Coordinate(100, 0), Coordinate::getNull() };
    WayPtr w2 = TestUtils::createWay(map, Status::Unknown2, w2c);

    std::shared_ptr<MaximalSublineStringMatcher> sublineMatcher(new MaximalSublineStringMatcher());
    sublineMatcher->setMinSplitSize(5.0);
    sublineMatcher->setMaxRelevantAngle(toRadians(60.0));

    set<pair<ElementId, ElementId>> pairs;
    pairs.insert(pair<ElementId, ElementId>(w1->getElementId(), w2->getElementId()));

    HighwaySnapMerger merger(pairs, sublineMatcher);

    vector<pair<ElementId, ElementId>> replaced;
    merger.apply(map, replaced);

    merger._markNeedsReview(map, w1, w2, "a review note", "a review type");

    CPPUNIT_ASSERT_EQUAL((size_t)1, map->getRelations().size());
    //will throw an exception on failure
    ConstRelationPtr reviewRelation =
      std::dynamic_pointer_cast<Relation>(
        TestUtils::getElementWithTag(map, MetadataTags::HootReviewNote(), "a review note"));
    HOOT_STR_EQUALS("a review type", reviewRelation->getTags().get(MetadataTags::HootReviewType()));
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(HighwaySnapMergerTest, "quick");

}
