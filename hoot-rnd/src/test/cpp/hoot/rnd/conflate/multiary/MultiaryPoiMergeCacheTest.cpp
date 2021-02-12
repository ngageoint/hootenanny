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
 * @copyright Copyright (C) 2017, 2018, 2019, 2020 Maxar (http://www.maxar.com/)
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
#include <hoot/rnd/conflate/multiary/MultiaryPoiMergeCache.h>

using namespace std;

namespace hoot
{

class MultiaryPoiMergeCacheTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(MultiaryPoiMergeCacheTest);
  CPPUNIT_TEST(basicTest);
  CPPUNIT_TEST_SUITE_END();

public:

  MultiaryPoiMergeCacheTest()
  {
    setResetType(ResetAll);
  }

  /**
   * Basic set of tests to see if the merged elements look as expected.
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

    MultiaryPoiMergeCache cache(map, matchCreator, mergerCreator);

    // compare a cluster of 2 to a cluster of 1. This isn't strictly the way it will operate
    // under real conditions, but it makes a reasonable test.
    {
      MultiaryClusterPtr mc1(new MultiaryCluster());
      mc1->append(map->getNode(-2));
      mc1->append(map->getNode(-3));

      MultiaryClusterPtr mc2(new MultiaryCluster());
      mc2->append(map->getNode(-1));

      MultiaryClusterPtr out = cache.merge(mc1, mc2);

      // -1 is the first element by input number.
      HOOT_STR_EQUALS(-1, out->mergedElement->getId());
      // while the IDs are the same, the merged element should be a new instance.
      HOOT_STR_EQUALS("amenity = pub\nhoot:hash = AAA\nname = my restaurant\n", map->getNode(-1)->getTags());
      HOOT_STR_EQUALS("amenity = pub\nhoot:hash = \nname = my restaurant\nalt_name = My Restaurant;Not the same\nsource:hash = AAA;BBB;CCC\n",
        out->mergedElement->getTags());
    }

    // make sure the merger isn't deleted twice. Kinda kludgy.
    mergerFactory.reset();
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(MultiaryPoiMergeCacheTest, "glacial");

}
