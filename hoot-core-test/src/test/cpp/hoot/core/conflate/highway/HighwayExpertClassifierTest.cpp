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
 * @copyright Copyright (C) 2014, 2015, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/conflate/highway/HighwayExpertClassifier.h>
#include <hoot/core/conflate/matching/MatchThreshold.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/elements/MapProjector.h>

// Tgs
#include <tgs/StreamUtils.h>

using namespace geos::geom;

namespace hoot
{

class HighwayExpertClassifierTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(HighwayExpertClassifierTest);
  CPPUNIT_TEST(runSimpleIntersectionTest);
  CPPUNIT_TEST(runSimpleOverlapTest);
  CPPUNIT_TEST(runSmallOverlapTest);
  CPPUNIT_TEST_SUITE_END();

public:

  HighwayExpertClassifierTest()
  {
    setResetType(ResetBasic);
  }

  /**
   * Two identical ways that are perpendicular. All straight and 100m long.
   * x and o signifiy Unknown1 and Unknown2, respectively.
   *
   *      o
   *      |
   * x---------x
   *      |
   *      o
   */
  void runSimpleIntersectionTest()
  {
    OsmMapPtr map(new OsmMap());
    OGREnvelope env;
    env.MinX = 0;
    env.MinY = 0;
    env.MaxX = 1;
    env.MaxY = 1;
    MapProjector::projectToOrthographic(map, env);

    Coordinate w1c[] = { Coordinate(0, 0), Coordinate(100, 0), Coordinate::getNull() };
    WayPtr w1 = TestUtils::createWay(map, w1c, "", Status::Unknown1);

    Coordinate w2c[] = { Coordinate(50, 50), Coordinate(50, -50), Coordinate::getNull() };
    WayPtr w2 = TestUtils::createWay(map, w2c, "", Status::Unknown2);

    std::shared_ptr<HighwayExpertClassifier> classifier(new HighwayExpertClassifier());

    WaySublineMatch match(WaySubline(WayLocation(map, w1, 0, 0.0), WayLocation(map, w1, 1, 0.0)),
                          WaySubline(WayLocation(map, w2, 0, 0.0), WayLocation(map, w2, 1, 0.0)));

    MatchClassification result = classifier->classify(map, match);

    HOOT_STR_EQUALS("match: 0.00352096 miss: 0.996479 review: 0", result.toString());
  }

  /**
   * Two identical ways that are 5m apart. All straight and 100m long.
   * x and o signifiy Unknown1 and Unknown2, respectively.
   *
   * x---------x
   * o---------o
   */
  void runSimpleOverlapTest()
  {
    OsmMapPtr map(new OsmMap());
    OGREnvelope env;
    env.MinX = 0;
    env.MinY = 0;
    env.MaxX = 1;
    env.MaxY = 1;
    MapProjector::projectToOrthographic(map, env);

    Coordinate w1c[] = { Coordinate(0, 0), Coordinate(100, 0), Coordinate::getNull() };
    WayPtr w1 = TestUtils::createWay(map, w1c, "", Status::Unknown1);

    Coordinate w2c[] = { Coordinate(0, 5), Coordinate(100, 5), Coordinate::getNull() };
    WayPtr w2 = TestUtils::createWay(map, w2c, "", Status::Unknown2);

    std::shared_ptr<HighwayExpertClassifier> classifier(new HighwayExpertClassifier());

    WaySublineMatch match(WaySubline(WayLocation(map, w1, 0, 0.0), WayLocation(map, w1, 1, 0.0)),
                          WaySubline(WayLocation(map, w2, 0, 0.0), WayLocation(map, w2, 1, 0.0)));

    MatchClassification result = classifier->classify(map, match);

    HOOT_STR_EQUALS("match: 0.138093 miss: 0.861907 review: 0", result.toString());
  }

  /**
   * Two identical ways are 5m apart and have a small overlap. All straight and 100m long.
   * x and o signifiy Unknown1 and Unknown2, respectively.
   *
   * x---------x
   *          o---------o
   */
  void runSmallOverlapTest()
  {
    OsmMapPtr map(new OsmMap());
    OGREnvelope env;
    env.MinX = 0;
    env.MinY = 0;
    env.MaxX = 1;
    env.MaxY = 1;
    MapProjector::projectToOrthographic(map, env);

    Coordinate w1c[] = { Coordinate(0, 0), Coordinate(100, 0), Coordinate::getNull() };
    WayPtr w1 = TestUtils::createWay(map, w1c, "", Status::Unknown1);

    Coordinate w2c[] = { Coordinate(90, 5), Coordinate(190, 5), Coordinate::getNull() };
    WayPtr w2 = TestUtils::createWay(map, w2c, "", Status::Unknown2);

    std::shared_ptr<HighwayExpertClassifier> classifier(new HighwayExpertClassifier());

    WaySublineMatch match(WaySubline(WayLocation(map, w1, 0, 0.0), WayLocation(map, w1, 1, 0.0)),
                          WaySubline(WayLocation(map, w2, 0, 0.0), WayLocation(map, w2, 1, 0.0)));

    MatchClassification result = classifier->classify(map, match);

    HOOT_STR_EQUALS("match: 6.75908e-6 miss: 0.999993 review: 0", result.toString());
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(HighwayExpertClassifierTest, "quick");

}
