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
#include <hoot/core/conflate/UnifyingConflator.h>
#include <hoot/core/io/OsmJsonReader.h>
#include <hoot/core/io/OsmJsonWriter.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/elements/MapProjector.h>

#include <hoot/rnd/conflate/multiary/MultiaryUtilities.h>

using namespace std;

namespace hoot
{

class MultiaryPoiMergerTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(MultiaryPoiMergerTest);
  CPPUNIT_TEST(basicTest);
  CPPUNIT_TEST_SUITE_END();

public:

  MultiaryPoiMergerTest() :
  HootTestFixture(
    "test-files/rnd/conflate/multiary/MultiaryPoiMergerTest/",
    "test-output/rnd/conflate/multiary/MultiaryPoiMergerTest/")
  {
    setResetType(ResetAllNoMatchFactory);
  }

  /**
   * Simple conflation test case. Should merge 1/2 and 3/4. There should be a review generated
   * between 1/2 and 3/4 due to the amenity=pub tag.
   */
  void basicTest()
  {
    conf().set(ConfigOptions::getUuidHelperRepeatableKey(), true);

    QString testJsonStr =
      "{                                      \n"
      " 'elements': [                         \n"
      " { 'type': 'node', 'id': -1, 'lat': 2.0, 'lon': -3.0, \n"
      "   'tags': { 'amenity': 'pub', 'name': 'My Restaurant', 'hoot:hash': 'AAA' } },\n"
      " { 'type': 'node', 'id': -2, 'lat': 3.0, 'lon': -3.0, \n"
      "   'tags': { 'amenity': 'pub', 'name': 'my Restaurant', 'hoot:hash': 'BBB' } },\n"
      " { 'type': 'node', 'id': -3, 'lat': 14.0, 'lon': -3.0, \n"
      "   'tags': { 'amenity': 'pub', 'name': 'Not the same', 'hoot:hash': 'CCC' } },\n"
      " { 'type': 'node', 'id': -4, 'lat': 14.0, 'lon': -3.0, \n"
      "   'tags': { 'place': 'locality', 'name': 'Not the same', 'hoot:hash': 'DDD' } }\n"
      "]                                      \n"
      "}                                      \n";

    OsmMapPtr map = std::make_shared<OsmMap>();
    OsmJsonReader().loadFromString(testJsonStr, map);

    map->setProjection(MapProjector::createOrthographic(0, 0));

    map->getNode(-1)->setStatus(Status::fromInput(0));
    map->getNode(-2)->setStatus(Status::fromInput(1));
    map->getNode(-3)->setStatus(Status::fromInput(2));
    map->getNode(-4)->setStatus(Status::fromInput(3));

    MultiaryUtilities::conflate(map);

    const QString testFileName = "basicTest.json";
    OsmJsonWriter writer;
    writer.setIncludeCircularError(false);
    writer.setIncludeCompatibilityTags(false);
    writer.open(_outputPath + testFileName);
    MapProjector::projectToWgs84(map);
    writer.write(map);
    writer.close();
    HOOT_FILE_EQUALS(_inputPath + testFileName, _outputPath + testFileName);
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(MultiaryPoiMergerTest, "glacial");

}
