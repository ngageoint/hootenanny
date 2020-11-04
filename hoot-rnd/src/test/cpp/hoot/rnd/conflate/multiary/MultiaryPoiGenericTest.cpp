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
 * @copyright Copyright (C) 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/conflate/matching/MatchFactory.h>
#include <hoot/core/conflate/merging/MergerFactory.h>
#include <hoot/core/conflate/UnifyingConflator.h>
#include <hoot/core/io/OsmJsonWriter.h>
#include <hoot/core/io/OsmJsonReader.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/elements/MapProjector.h>

using namespace std;

namespace hoot
{

class MultiaryPoiGenericTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(MultiaryPoiGenericTest);
  CPPUNIT_TEST(basicTest);
  CPPUNIT_TEST_SUITE_END();

public:

  MultiaryPoiGenericTest()
  {
    setResetType(ResetAll);
  }

  /**
   * @brief basicTest Should conflate the three POIs into a single POI.
   *
   * This uses a dirt simple merge operation that merges all POIs into a single output POI (no
   * clustering)
   */
  void basicTest()
  {
    OGREnvelope env;
    env.MinX = -1;
    env.MinY = -1;
    env.MaxX = 1;
    env.MaxY = 1;

    OsmMapPtr map(new OsmMap(MapProjector::getInstance().createPlanarProjection(env)));

    Tags foo;
    foo["poi"] = "yes";
    foo["name"] = "foo";
    foo["alt_name"] = "bar";
    foo["hoot:hash"] = "AAA";
    TestUtils::createNode(map, Status::fromInput(0), 0, 0, 15, foo);

    foo["alt_name"] = "baz";
    foo["hoot:hash"] = "BBB";
    TestUtils::createNode(map, Status::fromInput(1), 5, 0, 15, foo);

    foo["alt_name"] = "qux";
    foo["hoot:hash"] = "CCC";
    TestUtils::createNode(map, Status::fromInput(1), 10, 0, 15, foo);

    MatchFactory::getInstance().reset();
    MatchFactory::getInstance().registerCreator("hoot::ScriptMatchCreator,MultiaryPoi.js");
    MergerFactory::getInstance().reset();

    // call new conflation routine
    UnifyingConflator conflator;
    conflator.apply(map);

    OsmJsonWriter writer;
    writer.setIncludeCompatibilityTags(false);
    const QString actual = writer.toString(map);
    //LOG_VAR(TestUtils::toQuotedString(OsmJsonWriter().toString(map)));
    HOOT_STR_EQUALS("{\"version\": 0.6,\"generator\": \"Hootenanny\",\"elements\": [\n"
                    "{\"type\":\"node\",\"id\":-1,\"lat\":0,\"lon\":0,\"tags\":{\"hoot:hash\":\"\",\"name\":\"foo\",\"alt_name\":\"bar;baz;qux\",\"source:hash\":\"AAA;BBB;CCC\",\"poi\":\"yes\",\"error:circular\":\"15\"}}]\n"
                    "}\n",
                    actual);
    CPPUNIT_ASSERT(OsmJsonReader().isValidJson(actual));
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(MultiaryPoiGenericTest, "glacial");

}
