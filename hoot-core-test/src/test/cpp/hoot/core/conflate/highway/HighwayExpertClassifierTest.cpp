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

// Hoot
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/conflate/highway/HighwayExpertClassifier.h>
#include <hoot/core/conflate/MatchThreshold.h>
#include <hoot/core/elements/Way.h>

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

class HighwayExpertClassifierTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(HighwayExpertClassifierTest);
  CPPUNIT_TEST(runSimpleIntersectionTest);
  CPPUNIT_TEST(runSimpleOverlapTest);
  CPPUNIT_TEST(runSmallOverlapTest);
  CPPUNIT_TEST_SUITE_END();

public:

  WayPtr createWay(OsmMapPtr map, Coordinate* c)
  {
    WayPtr result((new Way(Status::Unknown1, map->createNextWayId(), 15.0)));

    for (int i = 0; c[i].isNull() == false; i++)
    {
      NodePtr n(new Node(Status::Unknown1, map->createNextNodeId(), c[i], 15.0));
      map->addNode(n);
      result->addNode(n->getId());
    }

    map->addWay(result);

    return result;
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
    OsmMap::resetCounters();
    boost::shared_ptr<OsmMap> map(new OsmMap());
    OGREnvelope env;
    env.MinX = 0;
    env.MinY = 0;
    env.MaxX = 1;
    env.MaxY = 1;
    MapProjector::projectToOrthographic(map, env);

    Coordinate w1c[] = { Coordinate(0, 0), Coordinate(100, 0), Coordinate::getNull() };
    WayPtr w1 = createWay(map, w1c);

    Coordinate w2c[] = { Coordinate(50, 50), Coordinate(50, -50), Coordinate::getNull() };
    WayPtr w2 = createWay(map, w2c);
    w2->setStatus(Status::Unknown2);

    boost::shared_ptr<HighwayExpertClassifier> classifier(new HighwayExpertClassifier());

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
    OsmMap::resetCounters();
    boost::shared_ptr<OsmMap> map(new OsmMap());
    OGREnvelope env;
    env.MinX = 0;
    env.MinY = 0;
    env.MaxX = 1;
    env.MaxY = 1;
    MapProjector::projectToOrthographic(map, env);

    Coordinate w1c[] = { Coordinate(0, 0), Coordinate(100, 0), Coordinate::getNull() };
    WayPtr w1 = createWay(map, w1c);

    Coordinate w2c[] = { Coordinate(0, 5), Coordinate(100, 5), Coordinate::getNull() };
    WayPtr w2 = createWay(map, w2c);
    w2->setStatus(Status::Unknown2);

    boost::shared_ptr<HighwayExpertClassifier> classifier(new HighwayExpertClassifier());

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
    OsmMap::resetCounters();
    boost::shared_ptr<OsmMap> map(new OsmMap());
    OGREnvelope env;
    env.MinX = 0;
    env.MinY = 0;
    env.MaxX = 1;
    env.MaxY = 1;
    MapProjector::projectToOrthographic(map, env);

    Coordinate w1c[] = { Coordinate(0, 0), Coordinate(100, 0), Coordinate::getNull() };
    WayPtr w1 = createWay(map, w1c);

    Coordinate w2c[] = { Coordinate(90, 5), Coordinate(190, 5), Coordinate::getNull() };
    WayPtr w2 = createWay(map, w2c);
    w2->setStatus(Status::Unknown2);

    boost::shared_ptr<HighwayExpertClassifier> classifier(new HighwayExpertClassifier());

    WaySublineMatch match(WaySubline(WayLocation(map, w1, 0, 0.0), WayLocation(map, w1, 1, 0.0)),
                          WaySubline(WayLocation(map, w2, 0, 0.0), WayLocation(map, w2, 1, 0.0)));

    MatchClassification result = classifier->classify(map, match);

    HOOT_STR_EQUALS("match: 6.75908e-06 miss: 0.999993 review: 0", result.toString());
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(HighwayExpertClassifierTest, "quick");

}
