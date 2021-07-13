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
#include <hoot/core/elements/Way.h>
#include <hoot/core/algorithms/extractors/poi-polygon/PoiPolygonNameScoreExtractor.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/language/ToEnglishDictionaryTranslator.h>

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

using namespace geos::geom;

namespace hoot
{

class PoiPolygonNameScoreExtractorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(PoiPolygonNameScoreExtractorTest);
  CPPUNIT_TEST(scoreTest);
  CPPUNIT_TEST(translateTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void scoreTest()
  {
    PoiPolygonNameScoreExtractor uut;
    uut.setConfiguration(conf());
    OsmMapPtr map = std::make_shared<OsmMap>();

    NodePtr node1(new Node(Status::Unknown1, -1, Coordinate(0.0, 0.0), 15.0));
    node1->getTags().set("name", "blah");
    WayPtr way1 = std::make_shared<Way>(Status::Unknown2, -1, 15.0);
    way1->getTags().set("name", "blah");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node1, way1), 0.0);

    WayPtr way2(new Way(Status::Unknown2, -1, 15.0));
    way2->getTags().set("name", "dfghdgf");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, uut.extract(*map, node1, way2), 0.001);
  }

  void translateTest()
  {
    PoiPolygonNameScoreExtractor uut;
    Settings settings = conf();
    OsmMapPtr map = std::make_shared<OsmMap>();

    settings.set("poi.polygon.name.translate.to.english", "true");
    settings.set("language.translation.translator", "hoot::ToEnglishDictionaryTranslator");
    uut.setConfiguration(settings);
    std::shared_ptr<ToEnglishDictionaryTranslator> dictTranslator =
      std::dynamic_pointer_cast<ToEnglishDictionaryTranslator>(
        PoiPolygonNameScoreExtractor::_translator);
    dictTranslator->setTokenizeInput(false);

    // ToEnglishDictionaryTranslator has some support for acronyms in the same manner that it
    // supports to English translations.
    NodePtr node1(new Node(Status::Unknown1, -1, Coordinate(0.0, 0.0), 15.0));
    node1->getTags().set("name", "Kentucky Fried Chicken");
    WayPtr way1 = std::make_shared<Way>(Status::Unknown2, -1, 15.0);
    way1->getTags().set("name", "KFC");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node1, way1), 0.0);

    settings.set("poi.polygon.name.translate.to.english", "false");
    uut.setConfiguration(settings);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, uut.extract(*map, node1, way1), 0.001);
  }

  // for misc name debug testing only
//  void miscTest()
//  {
//    //conf().set(ConfigOptions::getPoiPolygonStringComparerKey(), "hoot::KskipBigramDistance");
//    PoiPolygonNameScoreExtractor uut;
//    uut.setConfiguration(conf());
//    OsmMapPtr map = std::make_shared<OsmMap>();
//    NodePtr node1(new Node(Status::Unknown1, -1, Coordinate(0.0, 0.0), 15.0));
//    WayPtr way1 = std::make_shared<Way>(Status::Unknown2, -1, 15.0));

//    node1->getTags().set("name", "54 Mint");
//    way1->getTags().set("name", "San Francisco Mint");
//    LOG_VARW(uut.extract(*map, node1, way1));

//    node1->getTags().set("name", "Rincon hill Dog Park");
//    way1->getTags().set("name", "Rincon Hill");
//    LOG_VARW(uut.extract(*map, node1, way1));
//  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(PoiPolygonNameScoreExtractorTest, "quick");

}
