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
 * @copyright Copyright (C) 2014 DigitalGlobe (http://www.digitalglobe.com/)
 */

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// GEOS
#include <geos/geom/LineString.h>

// Hoot
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/algorithms/MaximalSublineStringMatcher.h>
#include <hoot/core/conflate/highway/HighwayExpertClassifier.h>
#include <hoot/core/conflate/highway/HighwayMatch.h>
#include <hoot/core/conflate/highway/HighwaySnapMerger.h>
#include <hoot/core/conflate/MatchThreshold.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/io/OsmJsonWriter.h>
#include <hoot/core/util/ElementConverter.h>
#include <hoot/core/util/MetadataTags.h>

// Qt
#include <QDebug>
#include <QDir>
#include <QBuffer>
#include <QByteArray>

// Tgs
#include <tgs/StreamUtils.h>

#include "../../TestUtils.h"

namespace hoot
{

class HighwaySnapMergerTest : public CppUnit::TestFixture
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

  OsmMapPtr createMap()
  {
    OsmMap::resetCounters();
    boost::shared_ptr<OsmMap> map(new OsmMap());
    OGREnvelope env;
    env.MinX = 0;
    env.MinY = 0;
    env.MaxX = 1;
    env.MaxY = 1;
    MapProjector::projectToOrthographic(map, env);

    return map;
  }

  WayPtr createWay(OsmMapPtr map, Coordinate* c, Status s = Status::Unknown1)
  {
    WayPtr result((new Way(s, map->createNextWayId(), 15.0)));

    for (int i = 0; c[i].isNull() == false; i++)
    {
      NodePtr n(new Node(s, map->createNextNodeId(), c[i], 15.0));
      map->addNode(n);
      result->addNode(n->getId());
    }

    map->addWay(result);

    return result;
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
    WayPtr w1 = createWay(map, w1c, Status::Unknown1);

    Coordinate w2c[] = { Coordinate(100, 0), Coordinate(0, 0), Coordinate::getNull() };
    WayPtr w2 = createWay(map, w2c, Status::Unknown2);

    boost::shared_ptr<MaximalSublineStringMatcher> sublineMatcher(new MaximalSublineStringMatcher());
    sublineMatcher->setMinSplitSize(5.0);
    sublineMatcher->setMaxRelevantAngle(toRadians(60.0));

    set< pair<ElementId, ElementId> > pairs;
    pairs.insert(pair<ElementId, ElementId>(w1->getElementId(), w2->getElementId()));

    HighwaySnapMerger merger(5, pairs, sublineMatcher);

    vector< pair<ElementId, ElementId> > replaced;
    merger.apply(map, replaced);

    ElementConverter ec(map);
    HOOT_STR_EQUALS("[2]{(Way:-1, Way:-5), (Way:-2, Way:-7)}", replaced);
    HOOT_STR_EQUALS("LINESTRING (50.0000000000000000 0.0000000000000000, 100.0000000000000000 0.0000000000000000)",
      ec.convertToLineString(map->getWay(-4))->toString());
    HOOT_STR_EQUALS("LINESTRING (100.0000000000000000 0.0000000000000000, 150.0000000000000000 0.0000000000000000)",
      ec.convertToLineString(map->getWay(-5))->toString());
    HOOT_STR_EQUALS("LINESTRING (50.0000000000000000 0.0000000000000000, 0.0000000000000000 0.0000000000000000)",
      ec.convertToLineString(map->getWay(-7))->toString());
    HOOT_STR_EQUALS("way(-4)\n"
                    "nodes: [2]{-1, -6}\n"
                    "tags: cached envelope: 0,-1,0,-1\n"
                    "status: conflated\n"
                    "version: 0\n"
                    "visible: 1\n"
                    "circular error: 15",
                    map->getWay(-4)->toString());
    HOOT_STR_EQUALS("way(-5)\n"
                    "nodes: [2]{-6, -2}\n"
                    "tags: cached envelope: 0,-1,0,-1\n"
                    "status: unknown1\n"
                    "version: 0\n"
                    "visible: 1\n"
                    "circular error: 15",
                    map->getWay(-5)->toString());
    HOOT_STR_EQUALS("way(-7)\n"
                    "nodes: [2]{-1, -4}\n"
                    "tags: cached envelope: 0,-1,0,-1\n"
                    "status: unknown2\n"
                    "version: 0\n"
                    "visible: 1\n"
                    "circular error: 15",
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
    WayPtr w1 = createWay(map, w1c, Status::Unknown1);

    Coordinate w2c[] = { Coordinate(0, 0), Coordinate(100, 0), Coordinate::getNull() };
    WayPtr w2 = createWay(map, w2c, Status::Unknown2);

    boost::shared_ptr<MaximalSublineStringMatcher> sublineMatcher(new MaximalSublineStringMatcher());
    sublineMatcher->setMinSplitSize(5.0);
    sublineMatcher->setMaxRelevantAngle(toRadians(60.0));

    set< pair<ElementId, ElementId> > pairs;
    pairs.insert(pair<ElementId, ElementId>(w1->getElementId(), w2->getElementId()));

    HighwaySnapMerger merger(5, pairs, sublineMatcher);

    vector< pair<ElementId, ElementId> > replaced;
    merger.apply(map, replaced);

    HOOT_STR_EQUALS(0, replaced.size());
    HOOT_STR_EQUALS(1, map->getWays().size());
    HOOT_STR_EQUALS("way(-4)\n"
                    "nodes: [2]{-1, -2}\n"
                    "tags: cached envelope: 0,-1,0,-1\n"
                    "status: conflated\n"
                    "version: 0\n"
                    "visible: 1\n"
                    "circular error: 15",
                    map->getWay(-4)->toString());
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
    WayPtr w1 = createWay(map, w1c, Status::Unknown1);

    Coordinate w2c[] = { Coordinate(0, 0), Coordinate(100, 0), Coordinate::getNull() };
    WayPtr w2 = createWay(map, w2c, Status::Unknown2);

    boost::shared_ptr<MaximalSublineStringMatcher> sublineMatcher(new MaximalSublineStringMatcher());
    sublineMatcher->setMinSplitSize(5.0);
    sublineMatcher->setMaxRelevantAngle(toRadians(60.0));

    set< pair<ElementId, ElementId> > pairs;
    pairs.insert(pair<ElementId, ElementId>(w1->getElementId(), w2->getElementId()));

    HighwaySnapMerger merger(5, pairs, sublineMatcher);

    vector< pair<ElementId, ElementId> > replaced;
    merger.apply(map, replaced);

    // there should be one element replaced
    HOOT_STR_EQUALS("[1]{(Way:-2, Way:-6)}", replaced);
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
    WayPtr w1 = createWay(map, w1c, Status::Unknown1);
    w1->getTags()["uuid"] = "w1";

    Coordinate w2c[] = { Coordinate(60, 5), Coordinate(150, 5), Coordinate::getNull() };
    WayPtr w2 = createWay(map, w2c, Status::Unknown1);
    w2->getTags()["barrier"] = "wall";
    w2->getTags()["uuid"] = "wall";

    Coordinate w3c[] = { Coordinate(-30, 0), Coordinate(100, 0), Coordinate::getNull() };
    WayPtr w3 = createWay(map, w3c, Status::Unknown2);
    w3->getTags()["highway"] = "path";
    w3->getTags()["uuid"] = "w3";

    RelationPtr r(new Relation(Status::Unknown1, 0, 15, Relation::MULTILINESTRING));
    r->addElement("", w1);
    r->addElement("", w2);
    r->setTag("uuid", "r");
    r->setTag("highway", "footway");
    map->addElement(r);

    boost::shared_ptr<MaximalSublineStringMatcher> sublineMatcher(new MaximalSublineStringMatcher());
    sublineMatcher->setMinSplitSize(5.0);
    sublineMatcher->setMaxRelevantAngle(toRadians(60.0));

    set< pair<ElementId, ElementId> > pairs;
    pairs.insert(pair<ElementId, ElementId>(r->getElementId(), w3->getElementId()));

    HighwaySnapMerger merger(5, pairs, sublineMatcher);

    vector< pair<ElementId, ElementId> > replaced;
    merger.apply(map, replaced);

    QString json = OsmJsonWriter().toString(map);

    QDir().mkpath("tmp");
    MapProjector::projectToWgs84(map);
    OsmMapWriterFactory::write(map, "tmp/dum.osm");

    QString expected = QString("{'version': 0.6,'generator': 'Hootenanny','elements': [\n"
        "{'type':'node','id':-1,'lat':5,'lon':20},\n"
        "{'type':'node','id':-2,'lat':5,'lon':60},\n"
        "{'type':'node','id':-3,'lat':5,'lon':60},\n"
        "{'type':'node','id':-4,'lat':5,'lon':150},\n"
        "{'type':'node','id':-5,'lat':0,'lon':-30},\n"
        "{'type':'node','id':-6,'lat':0,'lon':100},\n"
        "{'type':'node','id':-11,'lat':5,'lon':100},\n"
        "{'type':'node','id':-12,'lat':0,'lon':20},\n"
        "{'type':'node','id':-13,'lat':0,'lon':60},\n"
        "{'type':'way','id':-17,'nodes':[-5,-1],'tags':{'uuid':'w3','highway':'path','" + MetadataTags::ErrorCircular() + "':'15'},\n"
        "{'type':'way','id':-14,'nodes':[-11,-4],'tags':{'barrier':'wall','uuid':'wall','" + MetadataTags::ErrorCircular() + "':'15'},\n"
        "{'type':'way','id':-13,'nodes':[-3,-11],'tags':{'barrier':'wall','uuid':'wall','" + MetadataTags::ErrorCircular() + "':'15'},\n"
        "{'type':'way','id':-12,'nodes':[-1,-2],'tags':{'uuid':'w1','" + MetadataTags::ErrorCircular() + "':'15'},\n"
        "{'type':'relation','id':-2,'members':[\n"
        "{'type':'way','ref':-14,'role':''}],'tags':{'uuid':'r','highway':'footway','" + MetadataTags::ErrorCircular() + "':'15'},\n"
        "{'type':'relation','id':-1,'members':[\n"
        "{'type':'way','ref':-12,'role':''},\n"
        "{'type':'way','ref':-13,'role':''}],'tags':{'uuid':'r;w3','highway':'footway','" + MetadataTags::ErrorCircular() + "':'15'}]\n"
        "}\n"
        "").replace("'", "\"");
    HOOT_STR_EQUALS(expected, json);

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
    WayPtr w1 = createWay(map, w1c, Status::Unknown1);
    w1->getTags()["highway"] = "road";
    w1->getTags()["uuid"] = "w1";

    Coordinate w2c[] = { Coordinate(40, 0), Coordinate(60, 0), Coordinate::getNull() };
    WayPtr w2 = createWay(map, w2c, Status::Unknown2);
    w2->getTags()["highway"] = "path";
    w2->getTags()["uuid"] = "w2";

    boost::shared_ptr<MaximalSublineStringMatcher> sublineMatcher(new MaximalSublineStringMatcher());
    sublineMatcher->setMinSplitSize(5.0);
    sublineMatcher->setMaxRelevantAngle(toRadians(60.0));

    set< pair<ElementId, ElementId> > pairs;
    pairs.insert(pair<ElementId, ElementId>(w1->getElementId(), w2->getElementId()));

    HighwaySnapMerger merger(5, pairs, sublineMatcher);

    vector< pair<ElementId, ElementId> > replaced;
    merger.apply(map, replaced);

    QString json = OsmJsonWriter().toString(map);

//    MapProjector::reprojectToWgs84(map);
//    OsmMapWriterFactory::write(map, "tmp/dum.osm");

    QString expected = QString(
      "{'version': 0.6,'generator': 'Hootenanny','elements': [\n"
      "{'type':'node','id':-1,'lat':5,'lon':0},\n"
      "{'type':'node','id':-2,'lat':5,'lon':100},\n"
      "{'type':'node','id':-3,'lat':0,'lon':40},\n"
      "{'type':'node','id':-4,'lat':0,'lon':60},\n"
      "{'type':'node','id':-7,'lat':5,'lon':40},\n"
      "{'type':'node','id':-8,'lat':5,'lon':60},\n"
      "{'type':'way','id':-6,'nodes':[-8,-2],'tags':{'" + MetadataTags::ErrorCircular() + "':'15'},\n"
      "{'type':'way','id':-5,'nodes':[-1,-7],'tags':{'" + MetadataTags::ErrorCircular() + "':'15'},\n"
      "{'type':'way','id':-4,'nodes':[-7,-8],'tags':{'uuid':'w1;w2','highway':'road','" + MetadataTags::ErrorCircular() + "':'15'},\n"
      "{'type':'relation','id':-1,'members':[\n"
      "{'type':'way','ref':-5,'role':''},\n"
      "{'type':'way','ref':-6,'role':''}],'tags':{'uuid':'w1','highway':'road','" + MetadataTags::ErrorCircular() + "':'15'}]\n"
      "}\n"
      "").replace("'", "\"");
    HOOT_STR_EQUALS(expected, json);

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
    WayPtr w1 = createWay(map, w1c, Status::Unknown1);
    w1->getTags()["highway"] = "road";
    w1->getTags()["uuid"] = "w1";

    Coordinate w2c[] = { Coordinate(0, 0), Coordinate(100, 0), Coordinate::getNull() };
    WayPtr w2 = createWay(map, w2c, Status::Unknown2);

    RelationPtr r(new Relation(Status::Unknown2, 0, 15, Relation::MULTILINESTRING));
    r->addElement("", w2);
    r->setTag("uuid", "r");
    r->setTag("highway", "footway");
    map->addElement(r);

    boost::shared_ptr<MaximalSublineStringMatcher> sublineMatcher(new MaximalSublineStringMatcher());
    sublineMatcher->setMinSplitSize(5.0);
    sublineMatcher->setMaxRelevantAngle(toRadians(60.0));

    set< pair<ElementId, ElementId> > pairs;
    pairs.insert(pair<ElementId, ElementId>(w1->getElementId(), r->getElementId()));

    HighwaySnapMerger merger(5, pairs, sublineMatcher);

    vector< pair<ElementId, ElementId> > replaced;
    merger.apply(map, replaced);

    QString json = OsmJsonWriter().toString(map);
    //LOG_VAR(TestUtils::toQuotedString(json.replace("\"", "'")));

    QString expected = QString(
      "{'version': 0.6,'generator': 'Hootenanny','elements': [\n"
      "{'type':'node','id':-1,'lat':5,'lon':0},\n"
      "{'type':'node','id':-2,'lat':5,'lon':50},\n"
      "{'type':'node','id':-3,'lat':0,'lon':0},\n"
      "{'type':'node','id':-4,'lat':0,'lon':100},\n"
      "{'type':'node','id':-5,'lat':0,'lon':50},\n"
      "{'type':'way','id':-6,'nodes':[-2,-4],'tags':{'uuid':'r','highway':'footway','" + MetadataTags::ErrorCircular() + "':'15'},\n"
      "{'type':'way','id':-4,'nodes':[-1,-2],'tags':{'uuid':'w1;r','highway':'road','" + MetadataTags::ErrorCircular() + "':'15'}]\n"
      "}\n"
      "").replace("'", "\"");
    HOOT_STR_EQUALS(expected, json);
  }

  //simple test to make sure review note/type strings don't get interchanged
  void reviewMarkingTest()
  {
    OsmMapPtr map = createMap();

    Coordinate w1c[] = { Coordinate(0, 0), Coordinate(100, 0), Coordinate::getNull() };
    WayPtr w1 = createWay(map, w1c, Status::Unknown1);

    Coordinate w2c[] = { Coordinate(0, 0), Coordinate(100, 0), Coordinate::getNull() };
    WayPtr w2 = createWay(map, w2c, Status::Unknown2);

    boost::shared_ptr<MaximalSublineStringMatcher> sublineMatcher(new MaximalSublineStringMatcher());
    sublineMatcher->setMinSplitSize(5.0);
    sublineMatcher->setMaxRelevantAngle(toRadians(60.0));

    set< pair<ElementId, ElementId> > pairs;
    pairs.insert(pair<ElementId, ElementId>(w1->getElementId(), w2->getElementId()));

    HighwaySnapMerger merger(5, pairs, sublineMatcher);

    vector< pair<ElementId, ElementId> > replaced;
    merger.apply(map, replaced);

    merger._markNeedsReview(map, w1, w2, "a review note", "a review type");

    CPPUNIT_ASSERT_EQUAL((size_t)1, map->getRelations().size());
    //will throw an exception on failure
    ConstRelationPtr reviewRelation =
      dynamic_pointer_cast<Relation>(
        TestUtils::getElementWithTag(map, MetadataTags::HootReviewNote(), "a review note"));
    HOOT_STR_EQUALS("a review type", reviewRelation->getTags().get(MetadataTags::HootReviewType()));
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(HighwaySnapMergerTest, "quick");

}
