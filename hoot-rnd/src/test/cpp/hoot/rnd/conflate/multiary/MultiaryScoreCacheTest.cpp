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
#include <hoot/core/conflate/matching/MatchClassification.h>
#include <hoot/core/conflate/matching/MatchFactory.h>
#include <hoot/core/conflate/merging/MergerFactory.h>
#include <hoot/core/conflate/UnifyingConflator.h>
#include <hoot/core/io/OsmJsonReader.h>
#include <hoot/core/io/OsmJsonWriter.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/rnd/conflate/multiary/MultiaryScoreCache.h>

using namespace std;

namespace hoot
{

class MultiaryScoreCacheTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(MultiaryScoreCacheTest);
  CPPUNIT_TEST(basicTest);
  CPPUNIT_TEST_SUITE_END();

public:

  /**
   * Basic set of tests to see if the scores returned are sensible.
   */
  void basicTest()
  {
    QString testJsonStr =
      "{                                      \n"
      " 'elements': [                         \n"
      " { 'type': 'node', 'id': -1, 'lat': 2.0, 'lon': -3.0, \n"
      "   'tags': { 'amenity': 'pub', 'name': 'My Restaurant' } },\n"
      " { 'type': 'node', 'id': -2, 'lat': 3.0, 'lon': -3.0, \n"
      "   'tags': { 'amenity': 'pub', 'name': 'My Restaurant' } },\n"
      " { 'type': 'node', 'id': -3, 'lat': 14.0, 'lon': -3.0, \n"
      "   'tags': { 'amenity': 'pub', 'name': 'Not the same' } },\n"
      " { 'type': 'node', 'id': -4, 'lat': 14.0, 'lon': -3.0, \n"
      "   'tags': { 'place': 'locality', 'name': 'Not the same' } },\n"
      " { 'type': 'node', 'id': -5, 'lat': 14.0, 'lon': -3.0, \n"
      "   'tags': { 'military': 'yes', 'name': 'Not the same' } }\n"
      "]                                      \n"
      "}                                      \n";

    OsmMapPtr map = std::make_shared<OsmMap>();
    OsmJsonReader().loadFromString(testJsonStr, map);

    map->setProjection(MapProjector::createOrthographic(0, 0));

    map->getNode(-1)->setStatus(Status::fromInput(0));
    map->getNode(-2)->setStatus(Status::fromInput(1));
    map->getNode(-3)->setStatus(Status::fromInput(2));
    map->getNode(-4)->setStatus(Status::fromInput(3));
    map->getNode(-5)->setStatus(Status::fromInput(4));

    TestUtils::resetEnvironment();

    // have to do this or getInstance will re-register the default match creators.
    MatchFactory& factory = MatchFactory::getInstance();
    factory.reset();
    factory.registerCreator("hoot::ScriptMatchCreator,MultiaryPoi.js");

    std::shared_ptr<MatchCreator> matchCreator = factory.getCreators()[0];

    MultiaryScoreCache cache(map, matchCreator);

    // simple comparisons should return faux-probability values.
    HOOT_STR_EQUALS("match: 0.757576 miss: 0.242424 review: 0",
      cache.getScore(map->getNode(-1), map->getNode(-2)));
    HOOT_STR_EQUALS("match: 0.542857 miss: 0.457143 review: 0",
      cache.getScore(map->getNode(-1), map->getNode(-3)));
    HOOT_STR_EQUALS("match: 0.542857 miss: 0.457143 review: 0",
      cache.getScore(map->getNode(-2), map->getNode(-3)));
    cache.getScore(map->getNode(-1), map->getNode(-4));
    LOG_VAR(cache.getLastExplainText());
    HOOT_STR_EQUALS("match: 0.4 miss: 0.6 review: 0",
      cache.getScore(map->getNode(-1), map->getNode(-4)));
    HOOT_STR_EQUALS("match: 0.542857 miss: 0.457143 review: 0",
      cache.getScore(map->getNode(-1), map->getNode(-3)));
    // pub to military base
    HOOT_STR_EQUALS("match: 0 miss: 1 review: 0",
      cache.getScore(map->getNode(-1), map->getNode(-5)));
    // locality to military base w/ same name
    HOOT_STR_EQUALS("match: 0.757576 miss: 0.242424 review: 0",
      cache.getScore(map->getNode(-4), map->getNode(-5)));

    // compare a cluster of 2 to a cluster of 1. This isn't strictly the way it will operate
    // under real conditions, but it makes a reasonable test.
    {
      MultiaryClusterPtr mc1(new MultiaryCluster());
      mc1->append(map->getNode(-2));
      mc1->append(map->getNode(-3));
      mc1->mergedElement = map->getNode(-2);

      MultiaryClusterPtr mc2(new MultiaryCluster());
      mc2->append(map->getNode(-1));
      mc2->mergedElement = map->getNode(-1);

      HOOT_STR_EQUALS("match: 0.757576 miss: 0.242424 review: 0", cache.getScore(mc1, mc2));
    }

    // compare a cluster of 2 to a cluster of 2.
    {
      MultiaryClusterPtr mc1(new MultiaryCluster());
      mc1->append(map->getNode(-1));
      mc1->append(map->getNode(-2));
      mc1->mergedElement = map->getNode(-2);

      MultiaryClusterPtr mc2(new MultiaryCluster());
      mc2->append(map->getNode(-3));
      mc2->append(map->getNode(-4));
      mc2->mergedElement = map->getNode(-3);

      HOOT_STR_EQUALS("match: 0.542857 miss: 0.457143 review: 0", cache.getScore(mc1, mc2));
    }

    // compare a cluster of 2 pubs to a cluster w/ locality and military base.
    {
      MultiaryClusterPtr mc1(new MultiaryCluster());
      mc1->append(map->getNode(-1));
      mc1->append(map->getNode(-2));
      mc1->mergedElement = map->getNode(-2);

      MultiaryClusterPtr mc2(new MultiaryCluster());
      mc2->append(map->getNode(-4));
      mc2->append(map->getNode(-5));
      mc2->mergedElement = map->getNode(-5);

      HOOT_STR_EQUALS("match: 0 miss: 1 review: 0", cache.getScore(mc1, mc2));
    }
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(MultiaryScoreCacheTest, "glacial");

}
