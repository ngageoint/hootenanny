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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/conflate/poi-polygon/PoiPolygonMatch.h>
#include <hoot/core/ops/RecursiveElementRemover.h>
#include <hoot/core/util/Log.h>

using namespace geos::geom;

namespace hoot
{

class PoiPolygonMatchTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(PoiPolygonMatchTest);
  CPPUNIT_TEST(matchTest);
  CPPUNIT_TEST(missTest);
  CPPUNIT_TEST(reviewTest);
  CPPUNIT_TEST(reviewIfMatchedTypedTest);
  CPPUNIT_TEST(badMatchDistanceInputsTest);
  CPPUNIT_TEST(badReviewDistanceInputsTest);
  CPPUNIT_TEST(badNameScoreThresholdInputsTest);
  CPPUNIT_TEST(badTypeScoreThresholdInputsTest);
  CPPUNIT_TEST(badReviewIfMatchedTypesInputsTest);
  CPPUNIT_TEST(exactSourceMatchDisableConflationTest);
  CPPUNIT_TEST(sourceMatchTagKeyPrefixOnlyDisableConflationTest);
  CPPUNIT_TEST(sourceTagKeyMismatchDisableConflationTest);
  CPPUNIT_TEST(missingSourceTagTest);
  CPPUNIT_TEST(multiUseBuildingTest);
  CPPUNIT_TEST(disableIntradatasetConflationTest);
  CPPUNIT_TEST_SUITE_END();

public:

  PoiPolygonMatchTest()
  {
    setResetType(ResetBasic);
  }

  void matchTest()
  {
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
      PoiPolygonMatch uut(
        map, std::shared_ptr<MatchThreshold>(), std::shared_ptr<PoiPolygonRfClassifier>(),
        PoiPolygonInfoCachePtr(new PoiPolygonInfoCache(map)));
      uut.setEnableReviewReduction(true);
      uut.setMatchDistanceThreshold(0.0);
      uut.setReviewDistanceThreshold(0.0);
      uut.setNameScoreThreshold(0.8);
      uut.setTypeScoreThreshold(0.8);
      uut.setMatchEvidenceThreshold(3);
      uut.setReviewEvidenceThreshold(1);

      uut.calculateMatch(w1->getElementId(), n1->getElementId());

      HOOT_STR_EQUALS("match: 1 miss: 0 review: 0", uut.getClassification());
    }

    w1->getTags().set("name", "foo");
    n1->getTags().set("name", "foo");

    {
      PoiPolygonMatch uut(
        map, std::shared_ptr<MatchThreshold>(), std::shared_ptr<PoiPolygonRfClassifier>(),
        PoiPolygonInfoCachePtr(new PoiPolygonInfoCache(map)));
      uut.setEnableReviewReduction(true);
      uut.setMatchDistanceThreshold(0.0);
      uut.setReviewDistanceThreshold(0.0);
      uut.setNameScoreThreshold(0.8);
      uut.setTypeScoreThreshold(0.8);
      uut.setMatchEvidenceThreshold(3);
      uut.setReviewEvidenceThreshold(1);

      uut.calculateMatch(n1->getElementId(), w1->getElementId());

      HOOT_STR_EQUALS("match: 1 miss: 0 review: 0", uut.getClassification());
    }
  }

  void missTest()
  {
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
      PoiPolygonMatch uut(
        map, std::shared_ptr<MatchThreshold>(), std::shared_ptr<PoiPolygonRfClassifier>(),
        PoiPolygonInfoCachePtr(new PoiPolygonInfoCache(map)));
      uut.setEnableReviewReduction(true);
      uut.setMatchDistanceThreshold(0.0);
      uut.setReviewDistanceThreshold(0.0);
      uut.setNameScoreThreshold(0.8);
      uut.setTypeScoreThreshold(0.8);
      uut.setMatchEvidenceThreshold(3);
      uut.setReviewEvidenceThreshold(1);

      uut.calculateMatch(w1->getElementId(), n1->getElementId());

      HOOT_STR_EQUALS("match: 0 miss: 1 review: 0", uut.getClassification());
    }

    // need to fix; see #1177
    n1->getTags().set("name", "foo");
    n1->setX(-8);
    {
      PoiPolygonMatch uut(
        map, std::shared_ptr<MatchThreshold>(), std::shared_ptr<PoiPolygonRfClassifier>(),
        PoiPolygonInfoCachePtr(new PoiPolygonInfoCache(map)));
      uut.setEnableReviewReduction(true);
      uut.setMatchDistanceThreshold(0.0);
      uut.setReviewDistanceThreshold(0.0);
      uut.setNameScoreThreshold(0.8);
      uut.setTypeScoreThreshold(0.8);
      uut.setMatchEvidenceThreshold(3);
      uut.setReviewEvidenceThreshold(1);

      uut.calculateMatch(n1->getElementId(), w1->getElementId());

      HOOT_STR_EQUALS("match: 0 miss: 1 review: 0", uut.getClassification());
    }
  }

  void reviewTest()
  {
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
      PoiPolygonMatch uut(
        map, std::shared_ptr<MatchThreshold>(), std::shared_ptr<PoiPolygonRfClassifier>(),
        PoiPolygonInfoCachePtr(new PoiPolygonInfoCache(map)));
      uut.setEnableReviewReduction(true);
      uut.setMatchDistanceThreshold(0.0);
      uut.setReviewDistanceThreshold(0.0);
      uut.setNameScoreThreshold(0.8);
      uut.setTypeScoreThreshold(0.8);
      uut.setMatchEvidenceThreshold(3);
      uut.setReviewEvidenceThreshold(1);

      uut.calculateMatch(w1->getElementId(), n1->getElementId());

      HOOT_STR_EQUALS("match: 0 miss: 0 review: 1", uut.getClassification());
    }

    n1->getTags().set("name", "foo");
    n1->setX(-5);
    {
      PoiPolygonMatch uut(
        map, std::shared_ptr<MatchThreshold>(), std::shared_ptr<PoiPolygonRfClassifier>(),
        PoiPolygonInfoCachePtr(new PoiPolygonInfoCache(map)));
      uut.setEnableReviewReduction(true);
      uut.setMatchDistanceThreshold(0.0);
      uut.setReviewDistanceThreshold(0.0);
      uut.setNameScoreThreshold(0.8);
      uut.setTypeScoreThreshold(0.8);
      uut.setMatchEvidenceThreshold(3);
      uut.setReviewEvidenceThreshold(1);
      uut.setDisableSameSourceConflation(false);

      uut.calculateMatch(n1->getElementId(), w1->getElementId());

      HOOT_STR_EQUALS("match: 0 miss: 0 review: 1", uut.getClassification());
    }

    n1->getTags().set("name", "foo");
    n1->setX(-7);
    {
      PoiPolygonMatch uut(
        map, std::shared_ptr<MatchThreshold>(), std::shared_ptr<PoiPolygonRfClassifier>(),
        PoiPolygonInfoCachePtr(new PoiPolygonInfoCache(map)));
      uut.setEnableReviewReduction(true);
      uut.setMatchDistanceThreshold(0.0);
      uut.setReviewDistanceThreshold(0.0);
      uut.setNameScoreThreshold(0.8);
      uut.setTypeScoreThreshold(0.8);
      uut.setMatchEvidenceThreshold(3);
      uut.setReviewEvidenceThreshold(1);

      uut.calculateMatch(n1->getElementId(), w1->getElementId());

      HOOT_STR_EQUALS("match: 0 miss: 0 review: 1", uut.getClassification());
    }
  }

  void reviewIfMatchedTypedTest()
  {
    //we let the user specify a custom list of types that always force a review if a match was found
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
      PoiPolygonMatch uut(
        map, std::shared_ptr<MatchThreshold>(), std::shared_ptr<PoiPolygonRfClassifier>(),
        PoiPolygonInfoCachePtr(new PoiPolygonInfoCache(map)));
      uut.setEnableReviewReduction(true);
      uut.setMatchDistanceThreshold(0.0);
      uut.setReviewDistanceThreshold(0.0);
      uut.setNameScoreThreshold(0.8);
      uut.setTypeScoreThreshold(0.8);
      uut.setMatchEvidenceThreshold(3);
      uut.setReviewEvidenceThreshold(1);

      QStringList reviewIfMatchedTypes;
      reviewIfMatchedTypes.append("area=yes");
      reviewIfMatchedTypes.append("blah=bleh");
      uut.setReviewIfMatchedTypes(reviewIfMatchedTypes);

      uut.calculateMatch(w1->getElementId(), n1->getElementId());

      HOOT_STR_EQUALS("match: 0 miss: 0 review: 1", uut.getClassification());
    }

    {
      PoiPolygonMatch uut(
        map, std::shared_ptr<MatchThreshold>(), std::shared_ptr<PoiPolygonRfClassifier>(),
        PoiPolygonInfoCachePtr(new PoiPolygonInfoCache(map)));
      uut.setEnableReviewReduction(true);
      uut.setMatchDistanceThreshold(0.0);
      uut.setReviewDistanceThreshold(0.0);
      uut.setNameScoreThreshold(0.8);
      uut.setTypeScoreThreshold(0.8);
      uut.setMatchEvidenceThreshold(3);
      uut.setReviewEvidenceThreshold(1);

      QStringList reviewIfMatchedTypes;
      reviewIfMatchedTypes.append("building=yes");
      reviewIfMatchedTypes.append("blah=bleh");
      uut.setReviewIfMatchedTypes(reviewIfMatchedTypes);

      uut.calculateMatch(w1->getElementId(), n1->getElementId());

      HOOT_STR_EQUALS("match: 0 miss: 0 review: 1", uut.getClassification());
    }

    {
      PoiPolygonMatch uut(
        map, std::shared_ptr<MatchThreshold>(), std::shared_ptr<PoiPolygonRfClassifier>(),
        PoiPolygonInfoCachePtr(new PoiPolygonInfoCache(map)));
      uut.setEnableReviewReduction(true);
      uut.setMatchDistanceThreshold(0.0);
      uut.setReviewDistanceThreshold(0.0);
      uut.setNameScoreThreshold(0.8);
      uut.setTypeScoreThreshold(0.8);
      uut.setMatchEvidenceThreshold(3);
      uut.setReviewEvidenceThreshold(1);

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
      map, std::shared_ptr<MatchThreshold>(), std::shared_ptr<PoiPolygonRfClassifier>(),
      PoiPolygonInfoCachePtr(new PoiPolygonInfoCache(map)));

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
      map, std::shared_ptr<MatchThreshold>(), std::shared_ptr<PoiPolygonRfClassifier>(),
      PoiPolygonInfoCachePtr(new PoiPolygonInfoCache(map)));

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
      map, std::shared_ptr<MatchThreshold>(), std::shared_ptr<PoiPolygonRfClassifier>(),
      PoiPolygonInfoCachePtr(new PoiPolygonInfoCache(map)));

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
      map, std::shared_ptr<MatchThreshold>(), std::shared_ptr<PoiPolygonRfClassifier>(),
      PoiPolygonInfoCachePtr(new PoiPolygonInfoCache(map)));

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
      map, std::shared_ptr<MatchThreshold>(), std::shared_ptr<PoiPolygonRfClassifier>(),
      PoiPolygonInfoCachePtr(new PoiPolygonInfoCache(map)));

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

  void exactSourceMatchDisableConflationTest()
  {
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
      w1->getTags()["source"] = "mySource:a";
      n1->getTags()["source"] = "mySource:a";

      PoiPolygonMatch uut(
        map, std::shared_ptr<MatchThreshold>(), std::shared_ptr<PoiPolygonRfClassifier>(),
        PoiPolygonInfoCachePtr(new PoiPolygonInfoCache(map)));
      uut.setEnableReviewReduction(true);
      uut.setMatchDistanceThreshold(0.0);
      uut.setReviewDistanceThreshold(0.0);
      uut.setNameScoreThreshold(0.8);
      uut.setTypeScoreThreshold(0.8);
      uut.setMatchEvidenceThreshold(3);
      uut.setReviewEvidenceThreshold(1);
      uut.setDisableSameSourceConflation(true);
      uut.setDisableSameSourceConflationMatchTagKeyPrefixOnly(false);
      uut.setSourceTagKey("source");

      uut.calculateMatch(w1->getElementId(), n1->getElementId());

      HOOT_STR_EQUALS("match: 0 miss: 1 review: 0", uut.getClassification());
    }

    {
      w1->getTags()["source"] = "mySource:a";
      n1->getTags()["source"] = "mySource:b";

      PoiPolygonMatch uut(
        map, std::shared_ptr<MatchThreshold>(), std::shared_ptr<PoiPolygonRfClassifier>(),
        PoiPolygonInfoCachePtr(new PoiPolygonInfoCache(map)));
      uut.setEnableReviewReduction(true);
      uut.setMatchDistanceThreshold(0.0);
      uut.setReviewDistanceThreshold(0.0);
      uut.setNameScoreThreshold(0.8);
      uut.setTypeScoreThreshold(0.8);
      uut.setMatchEvidenceThreshold(3);
      uut.setReviewEvidenceThreshold(1);
      uut.setDisableSameSourceConflation(true);
      uut.setDisableSameSourceConflationMatchTagKeyPrefixOnly(false);
      uut.setSourceTagKey("source");

      uut.calculateMatch(w1->getElementId(), n1->getElementId());

      HOOT_STR_EQUALS("match: 1 miss: 0 review: 0", uut.getClassification());
    }
  }

  void sourceMatchTagKeyPrefixOnlyDisableConflationTest()
  {
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
      w1->getTags()["source"] = "mySource:a";
      n1->getTags()["source"] = "mySource:b";

      PoiPolygonMatch uut(
        map, std::shared_ptr<MatchThreshold>(), std::shared_ptr<PoiPolygonRfClassifier>(),
        PoiPolygonInfoCachePtr(new PoiPolygonInfoCache(map)));
      uut.setEnableReviewReduction(true);
      uut.setMatchDistanceThreshold(0.0);
      uut.setReviewDistanceThreshold(0.0);
      uut.setNameScoreThreshold(0.8);
      uut.setTypeScoreThreshold(0.8);
      uut.setMatchEvidenceThreshold(3);
      uut.setReviewEvidenceThreshold(1);
      uut.setDisableSameSourceConflation(true);
      uut.setDisableSameSourceConflationMatchTagKeyPrefixOnly(true);
      uut.setSourceTagKey("source");

      uut.calculateMatch(w1->getElementId(), n1->getElementId());

      HOOT_STR_EQUALS("match: 0 miss: 1 review: 0", uut.getClassification());
    }

    {
      w1->getTags()["source"] = "mySource:a";
      n1->getTags()["source"] = "mySource1:a";

      PoiPolygonMatch uut(
        map, std::shared_ptr<MatchThreshold>(), std::shared_ptr<PoiPolygonRfClassifier>(),
        PoiPolygonInfoCachePtr(new PoiPolygonInfoCache(map)));
      uut.setEnableReviewReduction(true);
      uut.setMatchDistanceThreshold(0.0);
      uut.setReviewDistanceThreshold(0.0);
      uut.setNameScoreThreshold(0.8);
      uut.setTypeScoreThreshold(0.8);
      uut.setMatchEvidenceThreshold(3);
      uut.setReviewEvidenceThreshold(1);
      uut.setDisableSameSourceConflation(true);
      uut.setDisableSameSourceConflationMatchTagKeyPrefixOnly(true);
      uut.setSourceTagKey("source");

      uut.calculateMatch(w1->getElementId(), n1->getElementId());

      HOOT_STR_EQUALS("match: 1 miss: 0 review: 0", uut.getClassification());
    }
  }

  void sourceTagKeyMismatchDisableConflationTest()
  {
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
      w1->getTags()["source"] = "mySource:a";
      n1->getTags()["source"] = "mySource:a";

      PoiPolygonMatch uut(
        map, std::shared_ptr<MatchThreshold>(), std::shared_ptr<PoiPolygonRfClassifier>(),
        PoiPolygonInfoCachePtr(new PoiPolygonInfoCache(map)));
      uut.setEnableReviewReduction(true);
      uut.setMatchDistanceThreshold(0.0);
      uut.setReviewDistanceThreshold(0.0);
      uut.setNameScoreThreshold(0.8);
      uut.setTypeScoreThreshold(0.8);
      uut.setMatchEvidenceThreshold(3);
      uut.setReviewEvidenceThreshold(1);
      uut.setDisableSameSourceConflation(true);
      uut.setDisableSameSourceConflationMatchTagKeyPrefixOnly(false);
      uut.setSourceTagKey("source1");

      uut.calculateMatch(w1->getElementId(), n1->getElementId());

      HOOT_STR_EQUALS("match: 1 miss: 0 review: 0", uut.getClassification());
    }
  }

  void missingSourceTagTest()
  {
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
    n1->getTags()["source"] = "mySource:a";
    map->addNode(n1);

    {
      PoiPolygonMatch uut(
        map, std::shared_ptr<MatchThreshold>(), std::shared_ptr<PoiPolygonRfClassifier>(),
        PoiPolygonInfoCachePtr(new PoiPolygonInfoCache(map)));
      uut.setEnableReviewReduction(true);
      uut.setMatchDistanceThreshold(0.0);
      uut.setReviewDistanceThreshold(0.0);
      uut.setNameScoreThreshold(0.8);
      uut.setTypeScoreThreshold(0.8);
      uut.setMatchEvidenceThreshold(3);
      uut.setReviewEvidenceThreshold(1);
      uut.setDisableSameSourceConflation(true);
      uut.setDisableSameSourceConflationMatchTagKeyPrefixOnly(false);
      uut.setSourceTagKey("source");

      uut.calculateMatch(w1->getElementId(), n1->getElementId());

      HOOT_STR_EQUALS("match: 1 miss: 0 review: 0", uut.getClassification());
    }

    {
      PoiPolygonMatch uut(
        map, std::shared_ptr<MatchThreshold>(), std::shared_ptr<PoiPolygonRfClassifier>(),
       PoiPolygonInfoCachePtr(new PoiPolygonInfoCache(map)));
      uut.setEnableReviewReduction(true);
      uut.setMatchDistanceThreshold(0.0);
      uut.setReviewDistanceThreshold(0.0);
      uut.setNameScoreThreshold(0.8);
      uut.setTypeScoreThreshold(0.8);
      uut.setMatchEvidenceThreshold(3);
      uut.setReviewEvidenceThreshold(1);
      uut.setDisableSameSourceConflation(true);
      uut.setDisableSameSourceConflationMatchTagKeyPrefixOnly(true);
      uut.setSourceTagKey("source");

      uut.calculateMatch(w1->getElementId(), n1->getElementId());

      HOOT_STR_EQUALS("match: 1 miss: 0 review: 0", uut.getClassification());
    }
  }

  void multiUseBuildingTest()
  {
    OsmMapPtr map(new OsmMap());
    Coordinate c1[] = { Coordinate(0.0, 0.0), Coordinate(20.0, 0.0),
                        Coordinate(20.0, 20.0), Coordinate(0.0, 20.0),
                        Coordinate(0.0, 0.0),
                        Coordinate::getNull() };

    NodePtr n1(new Node(Status::Unknown1, 1, 10, 10, 5));
    n1->getTags().set("poi", true);
    map->addNode(n1);

    PoiPolygonMatch uut(
      map, std::shared_ptr<MatchThreshold>(), std::shared_ptr<PoiPolygonRfClassifier>(),
      PoiPolygonInfoCachePtr(new PoiPolygonInfoCache(map)));
    uut.setConfiguration(conf());
    uut.setEnableReviewReduction(true);
    uut.setMatchDistanceThreshold(0.0);
    uut.setReviewDistanceThreshold(0.0);
    uut.setNameScoreThreshold(0.8);
    uut.setTypeScoreThreshold(0.8);
    uut.setMatchEvidenceThreshold(3);
    uut.setReviewEvidenceThreshold(1);

    uut.setReviewMultiUseBuildings(true);

    {
      n1->getTags()["name"] = "Staunton Elementary";

      WayPtr w1 = TestUtils::createWay(map, Status::Unknown1, c1, 5, "w1");
      w1->getTags()["name"] = "Staunton Elementary";
      w1->getTags().set("building", "yes");
      w1->getTags().set("building:use", "multipurpose");

      uut.calculateMatch(w1->getElementId(), n1->getElementId());

      HOOT_STR_EQUALS("match: 0 miss: 0 review: 1", uut.getClassification());
      CPPUNIT_ASSERT(uut.explain().contains("Match involves a multi-use building"));

      RecursiveElementRemover(w1->getElementId()).apply(map);
    }

    {
      n1->getTags()["name"] = "Staunton Elementary";

      WayPtr w1 = TestUtils::createWay(map, Status::Unknown1, c1, 5, "w1");
      w1->getTags()["name"] = "Staunton Elementary";
      w1->getTags().set("amenity", "school");
      w1->getTags().set("building:use", "multipurpose");

      uut.calculateMatch(w1->getElementId(), n1->getElementId());

      HOOT_STR_EQUALS("match: 0 miss: 0 review: 1", uut.getClassification());
      CPPUNIT_ASSERT(uut.explain().contains("Match involves a multi-use building"));

      RecursiveElementRemover(w1->getElementId()).apply(map);
    }

    {
      n1->getTags()["name"] = "Honey Creek Mall";
      n1->getTags()["shop"] = "mall";

      WayPtr w1 = TestUtils::createWay(map, Status::Unknown1, c1, 5, "w1");
      w1->getTags()["name"] = "Honey Creek Mall";
      w1->getTags().set("building", "yes");
      w1->getTags().set("shop", "mall");

      uut.calculateMatch(w1->getElementId(), n1->getElementId());

      LOG_VART(uut.getClassification());
      HOOT_STR_EQUALS("match: 0 miss: 0 review: 1", uut.getClassification());
      CPPUNIT_ASSERT(uut.explain().contains("Match involves a multi-use building"));

      RecursiveElementRemover(w1->getElementId()).apply(map);
    }

    // shop=mall has now been added to the building category, so whereas previously it couldn't
    // be considered a building without also having building=yes, now it can be considered a
    // building with the tag alone.
    {
      n1->getTags()["name"] = "Honey Creek Mall";
      n1->getTags()["shop"] = "mall";

      WayPtr w1 = TestUtils::createWay(map, Status::Unknown1, c1, 5, "w1");
      w1->getTags()["name"] = "Honey Creek Mall";
      w1->getTags().set("shop", "mall");

      uut.calculateMatch(w1->getElementId(), n1->getElementId());

      HOOT_STR_EQUALS("match: 0 miss: 0 review: 1", uut.getClassification());
      CPPUNIT_ASSERT(uut.explain().contains("Match involves a multi-use building"));

      RecursiveElementRemover(w1->getElementId()).apply(map);
    }

    {
      n1->getTags()["name"] = "Staunton Elementary";

      WayPtr w1 = TestUtils::createWay(map, Status::Unknown1, c1, 5, "w1");
      w1->getTags()["name"] = "Staunton Elementary";
      w1->getTags().set("amenity", "school");
      w1->getTags().set("building:use", "multipurpose");

      uut.calculateMatch(w1->getElementId(), n1->getElementId());

      HOOT_STR_EQUALS("match: 0 miss: 0 review: 1", uut.getClassification());
      CPPUNIT_ASSERT(uut.explain().contains("Match involves a multi-use building"));

      RecursiveElementRemover(w1->getElementId()).apply(map);
    }

    {
      n1->getTags()["name"] = "Staunton Elementary";

      WayPtr w1 = TestUtils::createWay(map, Status::Unknown1, c1, 5, "w1");
      w1->getTags()["name"] = "Staunton Elementary";
      w1->getTags().set("building", "yes");

      uut.calculateMatch(w1->getElementId(), n1->getElementId());

      HOOT_STR_EQUALS("match: 1 miss: 0 review: 0", uut.getClassification());
      CPPUNIT_ASSERT(!uut.explain().contains("Match involves a multi-use building"));

      RecursiveElementRemover(w1->getElementId()).apply(map);
    }

    {
      n1->getTags()["name"] = "Staunton Elementary";

      WayPtr w1 = TestUtils::createWay(map, Status::Unknown1, c1, 5, "w1");
      w1->getTags()["name"] = "Staunton Elementary";
      w1->getTags().set("amenity", "school");

      uut.calculateMatch(w1->getElementId(), n1->getElementId());

      HOOT_STR_EQUALS("match: 1 miss: 0 review: 0", uut.getClassification());
      CPPUNIT_ASSERT(!uut.explain().contains("Match involves a multi-use building"));

      RecursiveElementRemover(w1->getElementId()).apply(map);
    }

    {
      n1->getTags()["name"] = "Staunton Elementary";

      WayPtr w1 = TestUtils::createWay(map, Status::Unknown1, c1, 5, "w1");
      w1->getTags()["name"] = "Staunton Elementary";
      w1->getTags().set("area", "yes");
      w1->getTags().set("building:use", "multipurpose");

      uut.calculateMatch(w1->getElementId(), n1->getElementId());

      HOOT_STR_EQUALS("match: 1 miss: 0 review: 0", uut.getClassification());
      CPPUNIT_ASSERT(!uut.explain().contains("Match involves a multi-use building"));

      RecursiveElementRemover(w1->getElementId()).apply(map);
    }
  }

  void disableIntradatasetConflationTest()
  {
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
      // both of these elements get the same status
      w1->setStatus(Status::Unknown1);
      n1->setStatus(Status::Unknown1);

      PoiPolygonMatch uut(
        map, std::shared_ptr<MatchThreshold>(), std::shared_ptr<PoiPolygonRfClassifier>(),
        PoiPolygonInfoCachePtr(new PoiPolygonInfoCache(map)));
      uut.setEnableReviewReduction(true);
      uut.setMatchDistanceThreshold(0.0);
      uut.setReviewDistanceThreshold(0.0);
      uut.setNameScoreThreshold(0.8);
      uut.setTypeScoreThreshold(0.8);
      uut.setMatchEvidenceThreshold(3);
      uut.setReviewEvidenceThreshold(1);
      // this prevents them from matching when they have the same status
      uut.setDisableIntradatasetConflation1(true);

      uut.calculateMatch(w1->getElementId(), n1->getElementId());

      HOOT_STR_EQUALS("match: 0 miss: 1 review: 0", uut.getClassification());
    }

    {
      // these element have different statuses
      w1->setStatus(Status::Unknown1);
      n1->setStatus(Status::Unknown2);

      PoiPolygonMatch uut(
        map, std::shared_ptr<MatchThreshold>(), std::shared_ptr<PoiPolygonRfClassifier>(),
        PoiPolygonInfoCachePtr(new PoiPolygonInfoCache(map)));
      uut.setEnableReviewReduction(true);
      uut.setMatchDistanceThreshold(0.0);
      uut.setReviewDistanceThreshold(0.0);
      uut.setNameScoreThreshold(0.8);
      uut.setTypeScoreThreshold(0.8);
      uut.setMatchEvidenceThreshold(3);
      uut.setReviewEvidenceThreshold(1);
      // so this setting has no effect
      uut.setDisableIntradatasetConflation1(true);

      uut.calculateMatch(w1->getElementId(), n1->getElementId());

      HOOT_STR_EQUALS("match: 1 miss: 0 review: 0", uut.getClassification());
    }

    {
      // both of these elements get the same status
      w1->setStatus(Status::Unknown2);
      n1->setStatus(Status::Unknown2);

      PoiPolygonMatch uut(
        map, std::shared_ptr<MatchThreshold>(), std::shared_ptr<PoiPolygonRfClassifier>(),
        PoiPolygonInfoCachePtr(new PoiPolygonInfoCache(map)));
      uut.setEnableReviewReduction(true);
      uut.setMatchDistanceThreshold(0.0);
      uut.setReviewDistanceThreshold(0.0);
      uut.setNameScoreThreshold(0.8);
      uut.setTypeScoreThreshold(0.8);
      uut.setMatchEvidenceThreshold(3);
      uut.setReviewEvidenceThreshold(1);
      // this prevents them from matching when they have the same status
      uut.setDisableIntradatasetConflation2(true);

      uut.calculateMatch(w1->getElementId(), n1->getElementId());

      HOOT_STR_EQUALS("match: 0 miss: 1 review: 0", uut.getClassification());
    }

    {
      // these element have different statuses
      w1->setStatus(Status::Unknown2);
      n1->setStatus(Status::Unknown1);

      PoiPolygonMatch uut(
        map, std::shared_ptr<MatchThreshold>(), std::shared_ptr<PoiPolygonRfClassifier>(),
        PoiPolygonInfoCachePtr(new PoiPolygonInfoCache(map)));
      uut.setEnableReviewReduction(true);
      uut.setMatchDistanceThreshold(0.0);
      uut.setReviewDistanceThreshold(0.0);
      uut.setNameScoreThreshold(0.8);
      uut.setTypeScoreThreshold(0.8);
      uut.setMatchEvidenceThreshold(3);
      uut.setReviewEvidenceThreshold(1);
      // so this setting has no effect
      uut.setDisableIntradatasetConflation2(true);

      uut.calculateMatch(w1->getElementId(), n1->getElementId());

      HOOT_STR_EQUALS("match: 1 miss: 0 review: 0", uut.getClassification());
    }
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(PoiPolygonMatchTest, "quick");

}
