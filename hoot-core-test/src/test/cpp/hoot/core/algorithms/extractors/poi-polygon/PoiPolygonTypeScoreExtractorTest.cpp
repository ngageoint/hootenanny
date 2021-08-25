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
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/algorithms/extractors/poi-polygon/PoiPolygonTypeScoreExtractor.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/conflate/poi-polygon/PoiPolygonInfoCache.h>
#include <hoot/core/conflate/poi-polygon/PoiPolygonDistanceTruthRecorder.h>
#include <hoot/core/util/FileUtils.h>

using namespace geos::geom;

namespace hoot
{

class PoiPolygonTypeScoreExtractorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(PoiPolygonTypeScoreExtractorTest);
  CPPUNIT_TEST(runTest);
  CPPUNIT_TEST(translateTagValueTest);
  CPPUNIT_TEST_SUITE_END();

public:

  PoiPolygonTypeScoreExtractorTest() :
  HootTestFixture(
    "test-files/conflate/poi-polygon/PoiPolygonTypeScoreExtractorTest/",
    "test-output/conflate/poi-polygon/PoiPolygonTypeScoreExtractorTest/")
  {
  }

  void runTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    PoiPolygonDistanceTruthRecorder::resetMatchDistanceInfo();
    PoiPolygonInfoCachePtr infoCache = std::make_shared<PoiPolygonInfoCache>(map);
    infoCache->setConfiguration(conf());
    PoiPolygonTypeScoreExtractor uut(infoCache);
    uut.setConfiguration(conf());
    uut.setCalculateMatchDistanceTruth(true);

    NodePtr node1 = std::make_shared<Node>(Status::Unknown1, -1, Coordinate(0.0, 0.0), 15.0);
    node1->getTags().set("amenity", "school");
    WayPtr way1 = std::make_shared<Way>(Status::Unknown2, -1, 15.0);
    way1->getTags().set("amenity", "school");
    // Mocking a feature distance here that would have been otherwise set by PoiPolygonMatch.
    uut.setFeatureDistance(20.5);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node1, way1), 0.0);

    NodePtr node2 = std::make_shared<Node>(Status::Unknown1, -1, Coordinate(0.0, 0.0), 15.0);
    node2->getTags().set("amenity", "hospital");
    WayPtr way2 = std::make_shared<Way>(Status::Unknown2, -1, 15.0);
    way2->getTags().set("amenity", "clinic");
    uut.setFeatureDistance(10.2);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.6, uut.extract(*map, node2, way2), 0.0001);

    NodePtr node3 = std::make_shared<Node>(Status::Unknown1, -1, Coordinate(0.0, 0.0), 15.0);
    node3->getTags().set("amenity", "drinking_water");
    WayPtr way3 = std::make_shared<Way>(Status::Unknown2, -1, 15.0);
    way3->getTags().set("building", "yes");
    uut.setFeatureDistance(5.1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, uut.extract(*map, node3, way3), 0.0);

    FileUtils::writeFully(
      _outputPath + "PoiPolygonTypeScoreExtractorTest-runTest-matchDistanceInfo",
      PoiPolygonDistanceTruthRecorder::getMatchDistanceInfo().trimmed());
    HOOT_FILE_EQUALS(
      _inputPath + "PoiPolygonTypeScoreExtractorTest-runTest-matchDistanceInfo",
      _outputPath + "PoiPolygonTypeScoreExtractorTest-runTest-matchDistanceInfo");
  }

  void translateTagValueTest()
  {
    Settings settings = conf();
    OsmMapPtr map = std::make_shared<OsmMap>();

    settings.set("poi.polygon.type.translate.to.english", "true");
    settings.set("language.translation.translator", "ToEnglishDictionaryTranslator");
    PoiPolygonInfoCachePtr infoCache = std::make_shared<PoiPolygonInfoCache>(map);
    infoCache->setConfiguration(settings);
    PoiPolygonTypeScoreExtractor uut(infoCache);
    uut.setConfiguration(settings);

    NodePtr node1 = std::make_shared<Node>(Status::Unknown1, -1, Coordinate(0.0, 0.0), 15.0);
    node1->getTags().set("amenity", "ticket_office");
    WayPtr way1 = std::make_shared<Way>(Status::Unknown2, -1, 15.0);
    way1->getTags().set("amenity", "Fahrscheinschalter");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node1, way1), 0.0);

    settings.set("poi.polygon.type.translate.to.english", "false");
    uut.setConfiguration(settings);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, uut.extract(*map, node1, way1), 0.0);
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(PoiPolygonTypeScoreExtractorTest, "quick");

}
