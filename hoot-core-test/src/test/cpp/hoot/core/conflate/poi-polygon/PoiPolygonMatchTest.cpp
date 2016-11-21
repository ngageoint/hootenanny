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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include "../../TestUtils.h"
#include <hoot/core/conflate/poi-polygon/PoiPolygonMatch.h>

namespace hoot
{

class PoiPolygonMatchTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(PoiPolygonMatchTest);
  CPPUNIT_TEST(matchTest);
  CPPUNIT_TEST(missTest);
  CPPUNIT_TEST(reviewTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void matchTest()
  {
    OsmMap::resetCounters();

    OsmMapPtr map(new OsmMap());

    Coordinate c1[] = { Coordinate(0.0, 0.0), Coordinate(20.0, 0.0),
                        Coordinate(20.0, 20.0), Coordinate(0.0, 20.0),
                        Coordinate(0.0, 0.0),
                        Coordinate::getNull() };
    WayPtr w1 = TestUtils::createWay(map, Status::Unknown1, c1, 5, "w1");
    w1->getTags().set("area", true);
    w1->getTags().set("poi", true);
    w1->getTags()["name"] = "United Kingdoms";
    NodePtr n1(new Node(Status::Unknown1, 1, 10, 10, 5));
    n1->getTags().set("poi", true);
    n1->getTags()["name"] = "United Kingdom";
    map->addNode(n1);

    {
      PoiPolygonMatch uut(map, w1->getElementId(), n1->getElementId(), shared_ptr<MatchThreshold>(),
                          shared_ptr<PoiPolygonRfClassifier>(), 0, 0, 0.8, 0.8, 1.0);
      HOOT_STR_EQUALS("match: 1 miss: 0 review: 0", uut.getClassification());
    }

    w1->getTags().set("name", "foo");
    n1->getTags().set("name", "foo");

    {
      PoiPolygonMatch uut(map, n1->getElementId(), w1->getElementId(), shared_ptr<MatchThreshold>(),
                          shared_ptr<PoiPolygonRfClassifier>(), 0, 0, 0.8, 0.8, 1.0);
      HOOT_STR_EQUALS("match: 1 miss: 0 review: 0", uut.getClassification());
    }
  }

  void missTest()
  {
    OsmMap::resetCounters();

    OsmMapPtr map(new OsmMap());

    Coordinate c1[] = { Coordinate(0.0, 0.0), Coordinate(20.0, 0.0),
                        Coordinate(20.0, 20.0), Coordinate(0.0, 20.0),
                        Coordinate(0.0, 0.0),
                        Coordinate::getNull() };
    WayPtr w1 = TestUtils::createWay(map, Status::Unknown1, c1, 5, "w1");
    w1->getTags().set("building", true);
    NodePtr n1(new Node(Status::Unknown1, 1, -1, 10, 5));
    map->addNode(n1);
    w1->getTags().set("name", "foo");
    n1->getTags().set("name", "bar");
    n1->getTags().set("poi", true);

    {
      PoiPolygonMatch uut(map, w1->getElementId(), n1->getElementId(), shared_ptr<MatchThreshold>(),
                          shared_ptr<PoiPolygonRfClassifier>(), 0, 0, 0.8, 0.8, 1.0);
      HOOT_STR_EQUALS("match: 0 miss: 1 review: 0", uut.getClassification());
    }

    /// @todo fix; see #1177
    n1->getTags().set("name", "foo");
    n1->setX(-8);
    {
      PoiPolygonMatch uut(map, n1->getElementId(), w1->getElementId(), shared_ptr<MatchThreshold>(),
                          shared_ptr<PoiPolygonRfClassifier>(), 0, 0, 0.8, 0.8, 1.0);
      HOOT_STR_EQUALS("match: 0 miss: 1 review: 0", uut.getClassification());
    }
  }

  void reviewTest()
  {
    OsmMap::resetCounters();

    OsmMapPtr map(new OsmMap());

    Coordinate c1[] = { Coordinate(0.0, 0.0), Coordinate(20.0, 0.0),
                        Coordinate(20.0, 20.0), Coordinate(0.0, 20.0),
                        Coordinate(0.0, 0.0),
                        Coordinate::getNull() };
    WayPtr w1 = TestUtils::createWay(map, Status::Unknown1, c1, 5, "w1");
    w1->getTags().set("building", true);
    NodePtr n1(new Node(Status::Unknown1, 1, 10, 10, 5));
    map->addNode(n1);
    w1->getTags().set("name", "foo");
    n1->getTags().set("name", "bar");
    n1->getTags().set("poi", true);

    {
      PoiPolygonMatch uut(map, w1->getElementId(), n1->getElementId(), shared_ptr<MatchThreshold>(),
                          shared_ptr<PoiPolygonRfClassifier>(), 0, 0, 0.8, 0.8, 1.0);
      HOOT_STR_EQUALS("match: 0 miss: 0 review: 1", uut.getClassification());
    }

    n1->getTags().set("name", "foo");
    n1->setX(-5);
    {
      PoiPolygonMatch uut(map, n1->getElementId(), w1->getElementId(), shared_ptr<MatchThreshold>(),
                          shared_ptr<PoiPolygonRfClassifier>(), 0, 0, 0.8, 0.8, 1.0);
      HOOT_STR_EQUALS("match: 0 miss: 0 review: 1", uut.getClassification());
    }

    n1->getTags().set("name", "foo");
    n1->setX(-7);
    {
      PoiPolygonMatch uut(map, n1->getElementId(), w1->getElementId(), shared_ptr<MatchThreshold>(),
                          shared_ptr<PoiPolygonRfClassifier>(), 0, 0, 0.8, 0.8, 1.0);
      HOOT_STR_EQUALS("match: 0 miss: 0 review: 1", uut.getClassification());
    }
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(PoiPolygonMatchTest, "quick");

}
