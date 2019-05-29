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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/algorithms/extractors/poi-polygon/PoiPolygonTypeScoreExtractor.h>
#include <hoot/core/elements/Way.h>

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

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

  void runTest()
  {
    PoiPolygonTypeScoreExtractor uut;
    uut.setConfiguration(conf());
    OsmMapPtr map(new OsmMap());

    NodePtr node1(new Node(Status::Unknown1, -1, Coordinate(0.0, 0.0), 15.0));
    node1->getTags().set("amenity", "school");
    WayPtr way1(new Way(Status::Unknown2, -1, 15.0));
    way1->getTags().set("amenity", "school");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node1, way1), 0.0);

    NodePtr node2(new Node(Status::Unknown1, -1, Coordinate(0.0, 0.0), 15.0));
    node2->getTags().set("amenity", "hospital");
    WayPtr way2(new Way(Status::Unknown2, -1, 15.0));
    way2->getTags().set("amenity", "clinic");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.6, uut.extract(*map, node2, way2), 0.0001);

    NodePtr node3(new Node(Status::Unknown1, -1, Coordinate(0.0, 0.0), 15.0));
    node3->getTags().set("amenity", "drinking_water");
    WayPtr way3(new Way(Status::Unknown2, -1, 15.0));
    way3->getTags().set("building", "yes");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, uut.extract(*map, node3, way3), 0.0);
  }

  void translateTagValueTest()
  {
    PoiPolygonTypeScoreExtractor uut;
    Settings settings = conf();
    OsmMapPtr map(new OsmMap());

    settings.set("poi.polygon.type.translate.to.english", "true");
    settings.set("language.translation.translator", "hoot::ToEnglishDictionaryTranslator");
    uut.setConfiguration(settings);

    NodePtr node1(new Node(Status::Unknown1, -1, Coordinate(0.0, 0.0), 15.0));
    node1->getTags().set("amenity", "ticket_office");
    WayPtr way1(new Way(Status::Unknown2, -1, 15.0));
    way1->getTags().set("amenity", "Fahrscheinschalter");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node1, way1), 0.0);

    settings.set("poi.polygon.type.translate.to.english", "false");
    uut.setConfiguration(settings);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, uut.extract(*map, node1, way1), 0.0);
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(PoiPolygonTypeScoreExtractorTest, "quick");

}
