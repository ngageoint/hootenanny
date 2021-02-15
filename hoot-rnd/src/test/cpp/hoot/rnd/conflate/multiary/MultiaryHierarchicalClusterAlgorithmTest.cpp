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
 * @copyright Copyright (C) 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/conflate/matching/MatchFactory.h>
#include <hoot/core/conflate/merging/MergerFactory.h>
#include <hoot/core/io/OsmJsonReader.h>
#include <hoot/core/io/OsmJsonWriter.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/rnd/conflate/multiary/MultiaryHierarchicalClusterAlgorithm.h>

using namespace std;

namespace hoot
{

class MultiaryHierarchicalClusterAlgorithmTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(MultiaryHierarchicalClusterAlgorithmTest);
  CPPUNIT_TEST(basicTest);
  CPPUNIT_TEST_SUITE_END();

public:

  MultiaryHierarchicalClusterAlgorithmTest()
  {
    setResetType(ResetBasic);
  }

  /**
   * Cluster a simple set of nodes and verify that the output is reasonable
   */
  void basicTest()
  {
    QString testJsonStr =
      "{                                      \n"
      " 'elements': [                         \n"
      " { 'type': 'node', 'id': -1, 'lat': 2.0, 'lon': -3.0, \n"
      "   'tags': { 'amenity': 'pub', 'name': 'my restaurant', 'hoot:hash': 'AAA' } },\n"
      " { 'type': 'node', 'id': -2, 'lat': 3.0, 'lon': -3.0, \n"
      "   'tags': { 'amenity': 'pub', 'name': 'My Restaurant', 'hoot:hash': 'BBB' } },\n"
      " { 'type': 'node', 'id': -3, 'lat': 14.0, 'lon': -3.0, \n"
      "   'tags': { 'amenity': 'pub', 'name': 'Not the same', 'hoot:hash': 'CCC' } },\n"
      " { 'type': 'node', 'id': -4, 'lat': 14.0, 'lon': -3.0, \n"
      "   'tags': { 'place': 'locality', 'name': 'Not the same', 'hoot:hash': 'DDD' } },\n"
      " { 'type': 'node', 'id': -5, 'lat': 14.0, 'lon': -3.0, \n"
      "   'tags': { 'military': 'yes', 'name': 'Not the same', 'hoot:hash': 'EEE' } }\n"
      "]                                      \n"
      "}                                      \n";

    OsmMapPtr map(new OsmMap());
    OsmJsonReader().loadFromString(testJsonStr, map);


    map->setProjection(MapProjector::createOrthographic(0, 0));

    map->getNode(-1)->setStatus(Status::fromInput(0));
    map->getNode(-2)->setStatus(Status::fromInput(1));
    map->getNode(-3)->setStatus(Status::fromInput(2));
    map->getNode(-4)->setStatus(Status::fromInput(3));
    map->getNode(-5)->setStatus(Status::fromInput(4));

    TestUtils::resetEnvironment();

    // have to do this or getInstance will re-register the default match creators.
    MatchFactory& matchFactory = MatchFactory::getInstance();
    matchFactory.reset();
    matchFactory.registerCreator("hoot::ScriptMatchCreator,MultiaryPoi.js");

    MergerCreatorPtr mergerCreator(
      Factory::getInstance().constructObject<MergerCreator>(
        QString("hoot::ScriptMergerCreator")));
    MergerFactory& mergerFactory = MergerFactory::getInstance();
    mergerFactory.reset();
    mergerFactory.registerCreator(mergerCreator);

    std::shared_ptr<MatchCreator> matchCreator = matchFactory.getCreators()[0];

    MultiaryScoreCachePtr scoreCache(new MultiaryScoreCache(map, matchCreator));
    MultiaryPoiMergeCachePtr mergeCache(
      new MultiaryPoiMergeCache(map, matchCreator, mergerCreator));
    MatchThreshold mt(0.6, 0.6, 0.4);
    MultiaryHierarchicalClusterAlgorithm uut(mergeCache, scoreCache, mt);

    // compare a cluster of 2 to a cluster of 1. This isn't strictly the way it will operate
    // under real conditions, but it makes a reasonable test.
    {
      std::set<pair<ElementId, ElementId>> pairs;
      pairs.insert(pair<ElementId, ElementId>(ElementId::node(-1), ElementId::node(-2)));
      pairs.insert(pair<ElementId, ElementId>(ElementId::node(-1), ElementId::node(-3)));
      pairs.insert(pair<ElementId, ElementId>(ElementId::node(-2), ElementId::node(-3)));
      pairs.insert(pair<ElementId, ElementId>(ElementId::node(-3), ElementId::node(-4)));
      pairs.insert(pair<ElementId, ElementId>(ElementId::node(-4), ElementId::node(-5)));
      MultiaryClusterAlgorithm::ClusterList clusters = uut.calculateClusters(map, pairs);

      HOOT_STR_EQUALS("[3]{{ 'merged': Node(-5), 'members': [Node(-5)] }, { 'merged': Node(-1), 'members': [Node(-2), Node(-1)] }, { 'merged': Node(-3), 'members': [Node(-4), Node(-3)] }}",
        clusters);

      HOOT_STR_EQUALS("[1]{{ 'score': match: 0.542857 miss: 0.457143 review: 0, 'a': { 'merged': Node(-3), 'members': [Node(-4), Node(-3)] }, 'b': { 'merged': Node(-1), 'members': [Node(-2), Node(-1)] }, 'explainText': 'Somewhat similar (12m) - very close together, similar POI type'}}",
        uut.takeReviews());
    }

    // make sure the merger isn't deleted twice. Kinda kludgy.
    mergerFactory.reset();
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(MultiaryHierarchicalClusterAlgorithmTest, "glacial");

}
