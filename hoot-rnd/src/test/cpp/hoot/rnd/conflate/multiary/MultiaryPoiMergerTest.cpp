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
 * @copyright Copyright (C) 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/conflate/MatchFactory.h>
#include <hoot/core/conflate/MergerFactory.h>
#include <hoot/core/conflate/UnifyingConflator.h>
#include <hoot/core/io/OsmJsonReader.h>
#include <hoot/core/io/OsmJsonWriter.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/rnd/conflate/multiary/MultiaryPoiMergeCache.h>
#include <hoot/rnd/conflate/multiary/MultiaryPoiMergerCreator.h>

using namespace std;

namespace hoot
{

class MultiaryPoiMergerTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(MultiaryPoiMergerTest);
  CPPUNIT_TEST(basicTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void initMultiary()
  {
    TestUtils::resetEnvironment();

    // have to do this or getInstance will re-register the default match creators.
    MatchFactory& factory = MatchFactory::getInstance();
    factory.reset();
    factory.registerCreator("hoot::ScriptMatchCreator,MultiaryPoiGeneric.js");
  }

  /**
   * Simple conflation test case. Should merge 1/2 and 3/4. There should be a review generated
   * between 1/2 and 3/4 due to the amenity=pub tag.
   */
  void basicTest()
  {
    initMultiary();

    QString testJsonStr =
      "{                                      \n"
      " 'elements': [                         \n"
      " { 'type': 'node', 'id': -1, 'lat': 2.0, 'lon': -3.0, \n"
      "   'tags': { 'amenity': 'pub', 'name': 'My Restaurant' } },\n"
      " { 'type': 'node', 'id': -2, 'lat': 3.0, 'lon': -3.0, \n"
      "   'tags': { 'amenity': 'pub', 'name': 'my Restaurant' } },\n"
      " { 'type': 'node', 'id': -3, 'lat': 14.0, 'lon': -3.0, \n"
      "   'tags': { 'amenity': 'pub', 'name': 'Not the same' } },\n"
      " { 'type': 'node', 'id': -4, 'lat': 14.0, 'lon': -3.0, \n"
      "   'tags': { 'place': 'locality', 'name': 'Not the same' } }\n"
      "]                                      \n"
      "}                                      \n";

    OsmMapPtr map = OsmJsonReader().loadFromString(testJsonStr);
    map->setProjection(MapProjector::createOrthographic(0, 0));

    map->getNode(-1)->setStatus(Status::fromInput(0));
    map->getNode(-2)->setStatus(Status::fromInput(1));
    map->getNode(-3)->setStatus(Status::fromInput(2));
    map->getNode(-4)->setStatus(Status::fromInput(3));

    boost::shared_ptr<MergerFactory> mf(new MergerFactory());
    mf->registerCreator(new MultiaryPoiMergerCreator());

    MatchThresholdPtr mt(new MatchThreshold(0.39, 0.61, 1.1));
    // call new conflation routine
    UnifyingConflator conflator(mt);
    conflator.setMergerFactory(mf);
    conflator.apply(map);

    //LOG_VAR(TestUtils::toQuotedString(OsmJsonWriter().toString(map)));
    HOOT_STR_EQUALS("{\"version\": 0.6,\"generator\": \"Hootenanny\",\"elements\": [\n"
                    "{\"type\":\"node\",\"id\":-1,\"lat\":2,\"lon\":-3,\"tags\":{\"alt_name\":\"my Restaurant\",\"name\":\"My Restaurant\",\"amenity\":\"pub\",\"error:circular\":\"15\"}},\n"
                    "{\"type\":\"node\",\"id\":-3,\"lat\":14,\"lon\":-3,\"tags\":{\"place\":\"locality\",\"name\":\"Not the same\",\"amenity\":\"pub\",\"error:circular\":\"15\"}},\n"
                    "{\"type\":\"relation\",\"id\":-1,\"members\":[\n"
                    "{\"type\":\"node\",\"ref\":-3,\"role\":\"reviewee\"},\n"
                    "{\"type\":\"node\",\"ref\":-1,\"role\":\"reviewee\"}],\"tags\":{\"hoot:review:needs\":\"yes\",\"hoot:review:type\":\"POI\",\"hoot:review:members\":\"2\",\"hoot:review:score\":\"-1\",\"hoot:review:note\":\"Somewhat similar (12m) - very close together, similar POI type\",\"error:circular\":\"15\"}]\n"
                    "}\n", OsmJsonWriter().toString(map));
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(MultiaryPoiMergerTest, "slow");

}
