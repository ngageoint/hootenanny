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
#include <hoot/core/conflate/poi-polygon/PoiPolygonMatch.h>
#include <hoot/core/util/Log.h>

#include "../../TestUtils.h"

using namespace geos::geom;

namespace hoot
{

class PoiPolygonMatchTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(PoiPolygonMatchTest);
  CPPUNIT_TEST(matchTest);
  CPPUNIT_TEST(missTest);
  CPPUNIT_TEST(reviewTest);
  CPPUNIT_TEST(reviewIfMatchedTest);
  CPPUNIT_TEST(badMatchDistanceInputsTest);
  CPPUNIT_TEST(badReviewDistanceInputsTest);
  CPPUNIT_TEST(badNameScoreThresholdInputsTest);
  CPPUNIT_TEST(badTypeScoreThresholdInputsTest);
  CPPUNIT_TEST(badReviewIfMatchedTypesInputsTest);
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
      PoiPolygonMatch uut(map, boost::shared_ptr<MatchThreshold>(), boost::shared_ptr<PoiPolygonRfClassifier>());
      uut.setEnableAdvancedMatching(false);
      uut.setEnableReviewReduction(true);
      uut.setMatchDistanceThreshold(0.0);
      uut.setReviewDistanceThreshold(0.0);
      uut.setNameScoreThreshold(0.8);
      uut.setTypeScoreThreshold(0.8);
      uut.calculateMatch(w1->getElementId(), n1->getElementId());

      HOOT_STR_EQUALS("match: 1 miss: 0 review: 0", uut.getClassification());
    }

    w1->getTags().set("name", "foo");
    n1->getTags().set("name", "foo");

    {
      PoiPolygonMatch uut(map, boost::shared_ptr<MatchThreshold>(), boost::shared_ptr<PoiPolygonRfClassifier>());
      uut.setEnableAdvancedMatching(false);
      uut.setEnableReviewReduction(true);
      uut.setMatchDistanceThreshold(0.0);
      uut.setReviewDistanceThreshold(0.0);
      uut.setNameScoreThreshold(0.8);
      uut.setTypeScoreThreshold(0.8);
      uut.calculateMatch(n1->getElementId(), w1->getElementId());

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
      PoiPolygonMatch uut(map, boost::shared_ptr<MatchThreshold>(), boost::shared_ptr<PoiPolygonRfClassifier>());
      uut.setEnableAdvancedMatching(false);
      uut.setEnableReviewReduction(true);
      uut.setMatchDistanceThreshold(0.0);
      uut.setReviewDistanceThreshold(0.0);
      uut.setNameScoreThreshold(0.8);
      uut.setTypeScoreThreshold(0.8);
      uut.calculateMatch(w1->getElementId(), n1->getElementId());

      HOOT_STR_EQUALS("match: 0 miss: 1 review: 0", uut.getClassification());
    }

    /// @todo fix; see #1177
    n1->getTags().set("name", "foo");
    n1->setX(-8);
    {
      PoiPolygonMatch uut(map, boost::shared_ptr<MatchThreshold>(), boost::shared_ptr<PoiPolygonRfClassifier>());
      uut.setEnableAdvancedMatching(false);
      uut.setEnableReviewReduction(true);
      uut.setMatchDistanceThreshold(0.0);
      uut.setReviewDistanceThreshold(0.0);
      uut.setNameScoreThreshold(0.8);
      uut.setTypeScoreThreshold(0.8);
      uut.calculateMatch(n1->getElementId(), w1->getElementId());

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
      PoiPolygonMatch uut(map, boost::shared_ptr<MatchThreshold>(), boost::shared_ptr<PoiPolygonRfClassifier>());
      uut.setEnableAdvancedMatching(false);
      uut.setEnableReviewReduction(true);
      uut.setMatchDistanceThreshold(0.0);
      uut.setReviewDistanceThreshold(0.0);
      uut.setNameScoreThreshold(0.8);
      uut.setTypeScoreThreshold(0.8);
      uut.calculateMatch(w1->getElementId(), n1->getElementId());

      HOOT_STR_EQUALS("match: 0 miss: 0 review: 1", uut.getClassification());
    }

    n1->getTags().set("name", "foo");
    n1->setX(-5);
    {
      PoiPolygonMatch uut(map, boost::shared_ptr<MatchThreshold>(), boost::shared_ptr<PoiPolygonRfClassifier>());
      uut.setEnableAdvancedMatching(false);
      uut.setEnableReviewReduction(true);
      uut.setMatchDistanceThreshold(0.0);
      uut.setReviewDistanceThreshold(0.0);
      uut.setNameScoreThreshold(0.8);
      uut.setTypeScoreThreshold(0.8);
      uut.calculateMatch(n1->getElementId(), w1->getElementId());

      HOOT_STR_EQUALS("match: 0 miss: 0 review: 1", uut.getClassification());
    }

    n1->getTags().set("name", "foo");
    n1->setX(-7);
    {
      PoiPolygonMatch uut(map, boost::shared_ptr<MatchThreshold>(), boost::shared_ptr<PoiPolygonRfClassifier>());
      uut.setEnableAdvancedMatching(false);
      uut.setEnableReviewReduction(true);
      uut.setMatchDistanceThreshold(0.0);
      uut.setReviewDistanceThreshold(0.0);
      uut.setNameScoreThreshold(0.8);
      uut.setTypeScoreThreshold(0.8);
      uut.calculateMatch(n1->getElementId(), w1->getElementId());

      HOOT_STR_EQUALS("match: 0 miss: 0 review: 1", uut.getClassification());
    }
  }

  void reviewIfMatchedTest()
  {
    //we let the user specify a custom list of types that always force a review if a match was found

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
    n1->getTags().set("building", "yes");
    n1->getTags()["name"] = "United Kingdom";
    map->addNode(n1);

    {
      PoiPolygonMatch uut(map, boost::shared_ptr<MatchThreshold>(), boost::shared_ptr<PoiPolygonRfClassifier>());
      uut.setEnableAdvancedMatching(false);
      uut.setEnableReviewReduction(true);
      uut.setMatchDistanceThreshold(0.0);
      uut.setReviewDistanceThreshold(0.0);
      uut.setNameScoreThreshold(0.8);
      uut.setTypeScoreThreshold(0.8);
      QStringList reviewIfMatchedTypes;
      reviewIfMatchedTypes.append("area=yes");
      reviewIfMatchedTypes.append("blah=bleh");
      uut.setReviewIfMatchedTypes(reviewIfMatchedTypes);
      uut.calculateMatch(w1->getElementId(), n1->getElementId());

      HOOT_STR_EQUALS("match: 0 miss: 0 review: 1", uut.getClassification());
    }

    {
      PoiPolygonMatch uut(map, boost::shared_ptr<MatchThreshold>(), boost::shared_ptr<PoiPolygonRfClassifier>());
      uut.setEnableAdvancedMatching(false);
      uut.setEnableReviewReduction(true);
      uut.setMatchDistanceThreshold(0.0);
      uut.setReviewDistanceThreshold(0.0);
      uut.setNameScoreThreshold(0.8);
      uut.setTypeScoreThreshold(0.8);
      QStringList reviewIfMatchedTypes;
      reviewIfMatchedTypes.append("building=yes");
      reviewIfMatchedTypes.append("blah=bleh");
      uut.setReviewIfMatchedTypes(reviewIfMatchedTypes);
      uut.calculateMatch(w1->getElementId(), n1->getElementId());

      HOOT_STR_EQUALS("match: 0 miss: 0 review: 1", uut.getClassification());
    }

    {
      PoiPolygonMatch uut(map, boost::shared_ptr<MatchThreshold>(), boost::shared_ptr<PoiPolygonRfClassifier>());
      uut.setEnableAdvancedMatching(false);
      uut.setEnableReviewReduction(true);
      uut.setMatchDistanceThreshold(0.0);
      uut.setReviewDistanceThreshold(0.0);
      uut.setNameScoreThreshold(0.8);
      uut.setTypeScoreThreshold(0.8);
      QStringList reviewIfMatchedTypes;
      reviewIfMatchedTypes.append("blah=bleh");
      uut.setReviewIfMatchedTypes(reviewIfMatchedTypes);
      uut.calculateMatch(w1->getElementId(), n1->getElementId());

      HOOT_STR_EQUALS("match: 1 miss: 0 review: 0", uut.getClassification());
    }
  }

  void badMatchDistanceInputsTest()
  {
    OsmMapPtr map;
    PoiPolygonMatch uut(
      map, boost::shared_ptr<MatchThreshold>(), boost::shared_ptr<PoiPolygonRfClassifier>());

    QString exceptionMsg1("");
    try
    {
      uut.setMatchDistanceThreshold(-1.0);
    }
    catch (const IllegalArgumentException& e)
    {
      exceptionMsg1 = e.what();
    }
    CPPUNIT_ASSERT(
      exceptionMsg1.contains("Invalid POI/Polygon match distance configuration option value"));

    QString exceptionMsg2("");
    try
    {
      uut.setMatchDistanceThreshold(5001.0);
    }
    catch (const IllegalArgumentException& e)
    {
      exceptionMsg2 = e.what();
    }
    CPPUNIT_ASSERT(
      exceptionMsg2.contains("Invalid POI/Polygon match distance configuration option value"));
  }

  void badReviewDistanceInputsTest()
  {
    OsmMapPtr map;
    PoiPolygonMatch uut(
      map, boost::shared_ptr<MatchThreshold>(), boost::shared_ptr<PoiPolygonRfClassifier>());

    QString exceptionMsg1("");
    try
    {
      uut.setReviewDistanceThreshold(-1.0);
    }
    catch (const IllegalArgumentException& e)
    {
      exceptionMsg1 = e.what();
    }
    CPPUNIT_ASSERT(
      exceptionMsg1.contains("Invalid POI/Polygon review distance configuration option value"));

    QString exceptionMsg2("");
    try
    {
      uut.setReviewDistanceThreshold(5001.0);
    }
    catch (const IllegalArgumentException& e)
    {
      exceptionMsg2 = e.what();
    }
    CPPUNIT_ASSERT(
      exceptionMsg2.contains("Invalid POI/Polygon review distance configuration option value"));
  }

  void badNameScoreThresholdInputsTest()
  {
    OsmMapPtr map;
    PoiPolygonMatch uut(
      map, boost::shared_ptr<MatchThreshold>(), boost::shared_ptr<PoiPolygonRfClassifier>());

    QString exceptionMsg1("");
    try
    {
      uut.setNameScoreThreshold(0.001);
    }
    catch (const IllegalArgumentException& e)
    {
      exceptionMsg1 = e.what();
    }
    CPPUNIT_ASSERT(
      exceptionMsg1.contains("Invalid POI/Polygon name score threshold configuration option value"));

    QString exceptionMsg2("");
    try
    {
      uut.setNameScoreThreshold(1.1);
    }
    catch (const IllegalArgumentException& e)
    {
      exceptionMsg2 = e.what();
    }
    CPPUNIT_ASSERT(
      exceptionMsg2.contains("Invalid POI/Polygon name score threshold configuration option value"));
  }

  void badTypeScoreThresholdInputsTest()
  {
    OsmMapPtr map;
    PoiPolygonMatch uut(
      map, boost::shared_ptr<MatchThreshold>(), boost::shared_ptr<PoiPolygonRfClassifier>());

    QString exceptionMsg1("");
    try
    {
      uut.setTypeScoreThreshold(0.001);
    }
    catch (const IllegalArgumentException& e)
    {
      exceptionMsg1 = e.what();
    }
    CPPUNIT_ASSERT(
      exceptionMsg1.contains("Invalid POI/Polygon type score threshold configuration option value"));

    QString exceptionMsg2("");
    try
    {
      uut.setTypeScoreThreshold(1.1);
    }
    catch (const IllegalArgumentException& e)
    {
      exceptionMsg2 = e.what();
    }
    CPPUNIT_ASSERT(
      exceptionMsg2.contains("Invalid POI/Polygon type score threshold configuration option value"));
  }

  void badReviewIfMatchedTypesInputsTest()
  {
    OsmMapPtr map;
    PoiPolygonMatch uut(
      map, boost::shared_ptr<MatchThreshold>(), boost::shared_ptr<PoiPolygonRfClassifier>());

    QString exceptionMsg1("");
    QStringList badKvp1;
    badKvp1.append("key=value");
    badKvp1.append("blah");
    try
    {
      uut.setReviewIfMatchedTypes(badKvp1);
    }
    catch (const IllegalArgumentException& e)
    {
      exceptionMsg1 = e.what();
    }
    CPPUNIT_ASSERT(
      exceptionMsg1.contains(
        "Invalid POI/Polygon review if matched type configuration option value"));

    QString exceptionMsg2("");
    QStringList badKvp2;
    badKvp2.append("blah=");
    badKvp2.append("key=value");
    try
    {
      uut.setReviewIfMatchedTypes(badKvp2);
    }
    catch (const IllegalArgumentException& e)
    {
      exceptionMsg2 = e.what();
    }
    CPPUNIT_ASSERT(
      exceptionMsg2.contains(
        "Invalid POI/Polygon review if matched type configuration option value"));

    QString exceptionMsg3("");
    QStringList badKvp3;
    badKvp3.append("key=value");
    badKvp3.append("=blah");
    try
    {
      uut.setReviewIfMatchedTypes(badKvp3);
    }
    catch (const IllegalArgumentException& e)
    {
      exceptionMsg3 = e.what();
    }
    CPPUNIT_ASSERT(
      exceptionMsg3.contains(
        "Invalid POI/Polygon review if matched type configuration option value"));

    QString exceptionMsg4("");
    QStringList badKvp4;
    badKvp4.append("");
    badKvp4.append("key=value");
    try
    {
      uut.setReviewIfMatchedTypes(badKvp4);
    }
    catch (const IllegalArgumentException& e)
    {
      exceptionMsg4 = e.what();
    }
    CPPUNIT_ASSERT(
      exceptionMsg4.contains(
        "Invalid POI/Polygon review if matched type configuration option value"));
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(PoiPolygonMatchTest, "quick");

}
