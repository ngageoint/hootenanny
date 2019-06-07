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
#include <hoot/core/elements/Way.h>
#include <hoot/core/algorithms/extractors//poi-polygon/PoiPolygonAddressScoreExtractor.h>
#include <hoot/core/language/ToEnglishDictionaryTranslator.h>
#include <hoot/core/conflate/address/AddressTagKeys.h>

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

using namespace geos::geom;

namespace hoot
{

class PoiPolygonAddressScoreExtractorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(PoiPolygonAddressScoreExtractorTest);
  CPPUNIT_TEST(runTagTest);
  CPPUNIT_TEST(runCombinedTagTest);
  CPPUNIT_TEST(runRangeTest);
  CPPUNIT_TEST(runAltFormatTest);
  CPPUNIT_TEST(runSubLetterTest);
  CPPUNIT_TEST(runWayTest);
  CPPUNIT_TEST(runRelationTest);
  CPPUNIT_TEST(translateTagValueTest);
  CPPUNIT_TEST(additionalTagsTest);
  CPPUNIT_TEST(invalidFullAddressTest);
  CPPUNIT_TEST(invalidComponentAddressTest);
  CPPUNIT_TEST(addressNormalizationTest);
  CPPUNIT_TEST_SUITE_END();

public:

  PoiPolygonAddressScoreExtractorTest()
  {
  }

  void runTagTest()
  {
    PoiPolygonAddressScoreExtractor uut;
    uut.setConfiguration(conf());
    OsmMapPtr map(new OsmMap());

    NodePtr node1(new Node(Status::Unknown1, -1, Coordinate(0.0, 0.0), 15.0));
    node1->getTags().set(TestUtils::FULL_ADDRESS_TAG_NAME, "123 Main Street");
    map->addNode(node1);
    WayPtr way1(new Way(Status::Unknown2, -1, 15.0));
    way1->getTags().set(TestUtils::FULL_ADDRESS_TAG_NAME, "123 main street");
    map->addWay(way1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node1, way1), 0.0);

    NodePtr node2(new Node(Status::Unknown1, -2, Coordinate(0.0, 0.0), 15.0));
    node2->getTags().set(TestUtils::FULL_ADDRESS_TAG_NAME, "123 Main Street");
    map->addNode(node2);
    WayPtr way2(new Way(Status::Unknown2, -2, 15.0));
    way2->getTags().set(TestUtils::FULL_ADDRESS_TAG_NAME, "567 first street");
    map->addWay(way2);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, uut.extract(*map, node2, way2), 0.01);
  }

  void runCombinedTagTest()
  {
    PoiPolygonAddressScoreExtractor uut;
    uut.setConfiguration(conf());
    OsmMapPtr map(new OsmMap());

    NodePtr node1(new Node(Status::Unknown1, -1, Coordinate(0.0, 0.0), 15.0));
    node1->getTags().set(TestUtils::HOUSE_NUMBER_TAG_NAME, "123");
    node1->getTags().set(TestUtils::STREET_TAG_NAME, "Main Street");
    map->addNode(node1);
    WayPtr way1(new Way(Status::Unknown2, -1, 15.0));
    way1->getTags().set(TestUtils::HOUSE_NUMBER_TAG_NAME, "123");
    way1->getTags().set(TestUtils::STREET_TAG_NAME, "main street");
    map->addWay(way1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node1, way1), 0.0);

    NodePtr node2(new Node(Status::Unknown1, -2, Coordinate(0.0, 0.0), 15.0));
    node2->getTags().set(TestUtils::HOUSE_NUMBER_TAG_NAME, "123");
    node2->getTags().set(TestUtils::STREET_TAG_NAME, "Main Street");
    map->addNode(node2);
    WayPtr way2(new Way(Status::Unknown2, -2, 15.0));
    way2->getTags().set(TestUtils::HOUSE_NUMBER_TAG_NAME, "567");
    way2->getTags().set(TestUtils::STREET_TAG_NAME, "first street");
    map->addWay(way2);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, uut.extract(*map, node2, way2), 0.01);
  }

  void runRangeTest()
  {
    PoiPolygonAddressScoreExtractor uut;
    uut.setConfiguration(conf());
    OsmMapPtr map(new OsmMap());

    NodePtr node1(new Node(Status::Unknown1, -1, Coordinate(0.0, 0.0), 15.0));
    node1->getTags().set(TestUtils::HOUSE_NUMBER_TAG_NAME, "123-125");
    node1->getTags().set(TestUtils::STREET_TAG_NAME, "Main Street");
    map->addNode(node1);
    WayPtr way1(new Way(Status::Unknown2, -1, 15.0));
    way1->getTags().set(TestUtils::HOUSE_NUMBER_TAG_NAME, "124");
    way1->getTags().set(TestUtils::STREET_TAG_NAME, "main street");
    map->addWay(way1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node1, way1), 0.0);

    NodePtr node2(new Node(Status::Unknown1, -2, Coordinate(0.0, 0.0), 15.0));
    node2->getTags().set(TestUtils::HOUSE_NUMBER_TAG_NAME, "123-125");
    node2->getTags().set(TestUtils::STREET_TAG_NAME, "Main Street");
    map->addNode(node2);
    WayPtr way2(new Way(Status::Unknown2, -2, 15.0));
    way2->getTags().set(TestUtils::HOUSE_NUMBER_TAG_NAME, "123");
    way2->getTags().set(TestUtils::STREET_TAG_NAME, "main street");
    map->addWay(way2);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node2, way2), 0.0);

    NodePtr node3(new Node(Status::Unknown1, -3, Coordinate(0.0, 0.0), 15.0));
    node3->getTags().set(TestUtils::HOUSE_NUMBER_TAG_NAME, "123-125");
    node3->getTags().set(TestUtils::STREET_TAG_NAME, "Main Street");
    map->addNode(node3);
    WayPtr way3(new Way(Status::Unknown2, -3, 15.0));
    way3->getTags().set(TestUtils::HOUSE_NUMBER_TAG_NAME, "567");
    way3->getTags().set(TestUtils::STREET_TAG_NAME, "first street");
    map->addWay(way3);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, uut.extract(*map, node3, way3), 0.01);
  }

  void runAltFormatTest()
  {
    PoiPolygonAddressScoreExtractor uut;
    uut.setConfiguration(conf());
    OsmMapPtr map(new OsmMap());

    NodePtr node1(new Node(Status::Unknown1, -1, Coordinate(0.0, 0.0), 15.0));
    node1->getTags().set(
      TestUtils::FULL_ADDRESS_TAG_NAME_2, "Main Street 123 20121 mytown");
    map->addNode(node1);
    WayPtr way1(new Way(Status::Unknown2, -1, 15.0));
    way1->getTags().set(TestUtils::FULL_ADDRESS_TAG_NAME_2, "main street 123");
    map->addWay(way1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node1, way1), 0.0);

    NodePtr node2(new Node(Status::Unknown1, -2, Coordinate(0.0, 0.0), 15.0));
    node2->getTags().set(
      TestUtils::FULL_ADDRESS_TAG_NAME_2, "Main Street 123 20121 mytown");
    map->addNode(node2);
    WayPtr way2(new Way(Status::Unknown2, -2, 15.0));
    way2->getTags().set(
      TestUtils::FULL_ADDRESS_TAG_NAME_2, "first street 567");
    map->addWay(way2);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, uut.extract(*map, node2, way2), 0.01);

    NodePtr node3(new Node(Status::Unknown1, -1, Coordinate(0.0, 0.0), 15.0));
    node3->getTags().set(
      TestUtils::FULL_ADDRESS_TAG_NAME_2, "ZENTRALLÄNDSTRASSE 40 81379 MÜNCHEN");
    map->addNode(node3);
    WayPtr way3(new Way(Status::Unknown2, -1, 15.0));
    way3->getTags().set(TestUtils::FULL_ADDRESS_TAG_NAME_2, "40 ZENTRALLÄNDSTRASSE");
    map->addWay(way3);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node3, way3), 0.0);
  }

  void runSubLetterTest()
  {
    PoiPolygonAddressScoreExtractor uut;
    uut.setConfiguration(conf());
    OsmMapPtr map(new OsmMap());

    NodePtr node1(new Node(Status::Unknown1, -1, Coordinate(0.0, 0.0), 15.0));
    node1->getTags().set(TestUtils::HOUSE_NUMBER_TAG_NAME, "123");
    node1->getTags().set(TestUtils::STREET_TAG_NAME, "Main Street");
    map->addNode(node1);
    WayPtr way1(new Way(Status::Unknown2, -1, 15.0));
    way1->getTags().set(TestUtils::HOUSE_NUMBER_TAG_NAME, "123a");
    way1->getTags().set(TestUtils::STREET_TAG_NAME, "main street");
    map->addWay(way1);

    uut.setAllowLenientHouseNumberMatching(true);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node1, way1), 0.0);

    uut.setAllowLenientHouseNumberMatching(false);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, uut.extract(*map, node1, way1), 0.0);
  }

  void runWayTest()
  {
    PoiPolygonAddressScoreExtractor uut;
    uut.setConfiguration(conf());
    OsmMapPtr map(new OsmMap());

    NodePtr node1(new Node(Status::Unknown1, -1, Coordinate(0.0, 0.0), 15.0));
    node1->getTags().set(TestUtils::HOUSE_NUMBER_TAG_NAME, "123");
    node1->getTags().set(TestUtils::STREET_TAG_NAME, "Main Street");
    map->addNode(node1);
    WayPtr way1(new Way(Status::Unknown2, -1, 15.0));
    NodePtr node2(new Node(Status::Unknown1, -2, Coordinate(0.0, 0.0), 15.0));
    node2->getTags().set(TestUtils::HOUSE_NUMBER_TAG_NAME, "123");
    node2->getTags().set(TestUtils::STREET_TAG_NAME, "main street");
    map->addNode(node2);
    way1->addNode(node2->getId());
    map->addWay(way1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node1, way1), 0.0);

    NodePtr node3(new Node(Status::Unknown1, -3, Coordinate(0.0, 0.0), 15.0));
    node3->getTags().set(TestUtils::HOUSE_NUMBER_TAG_NAME, "123");
    node3->getTags().set(TestUtils::STREET_TAG_NAME, "Main Street");
    map->addNode(node3);
    WayPtr way2(new Way(Status::Unknown2, -2, 15.0));
    NodePtr node4(new Node(Status::Unknown2, -4, Coordinate(0.0, 0.0), 15.0));
    node4->getTags().set(TestUtils::HOUSE_NUMBER_TAG_NAME, "567");
    node4->getTags().set(TestUtils::STREET_TAG_NAME, "first street");
    map->addNode(node4);
    way2->addNode(node4->getId());
    map->addWay(way2);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, uut.extract(*map, node3, way2), 0.01);
  }

  void runRelationTest()
  {
    PoiPolygonAddressScoreExtractor uut;
    uut.setConfiguration(conf());
    OsmMapPtr map(new OsmMap());

    NodePtr node1(new Node(Status::Unknown1, -1, Coordinate(0.0, 0.0), 15.0));
    node1->getTags().set(TestUtils::HOUSE_NUMBER_TAG_NAME, "123");
    node1->getTags().set(TestUtils::STREET_TAG_NAME, "Main Street");
    map->addNode(node1);
    RelationPtr relation1(new Relation(Status::Unknown2, -1, 15.0));
    NodePtr node2(new Node(Status::Unknown1, -2, Coordinate(0.0, 0.0), 15.0));
    node2->getTags().set(TestUtils::HOUSE_NUMBER_TAG_NAME, "123");
    node2->getTags().set(TestUtils::STREET_TAG_NAME, "main street");
    map->addNode(node2);
    relation1->addElement("test", node2->getElementId());
    map->addRelation(relation1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node1, relation1), 0.0);

    NodePtr node4(new Node(Status::Unknown1, -4, Coordinate(0.0, 0.0), 15.0));
    node4->getTags().set(TestUtils::HOUSE_NUMBER_TAG_NAME, "123");
    node4->getTags().set(TestUtils::STREET_TAG_NAME, "Main Street");
    map->addNode(node4);
    RelationPtr relation3(new Relation(Status::Unknown2, -3, 15.0));
    NodePtr node5(new Node(Status::Unknown1, -5, Coordinate(0.0, 0.0), 15.0));
    node5->getTags().set(TestUtils::HOUSE_NUMBER_TAG_NAME, "567");
    node5->getTags().set(TestUtils::STREET_TAG_NAME, "first street");
    map->addNode(node5);
    relation3->addElement("test", node5->getElementId());
    map->addRelation(relation3);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, uut.extract(*map, node4, relation3), 0.01);

    NodePtr node6(new Node(Status::Unknown1, -6, Coordinate(0.0, 0.0), 15.0));
    node6->getTags().set(TestUtils::HOUSE_NUMBER_TAG_NAME, "567");
    node6->getTags().set(TestUtils::STREET_TAG_NAME, "first street");
    map->addNode(node6);
    RelationPtr relation4(new Relation(Status::Unknown2, -4, 15.0));
    WayPtr way3(new Way(Status::Unknown2, -3, 15.0));
    way3->getTags().set(TestUtils::HOUSE_NUMBER_TAG_NAME, "123");
    way3->getTags().set(TestUtils::STREET_TAG_NAME, "main street");
    map->addWay(way3);
    relation4->addElement("test", way3->getElementId());
    map->addRelation(relation4);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, uut.extract(*map, node6, relation4), 0.01);
  }

  void translateTagValueTest()
  {
    PoiPolygonAddressScoreExtractor uut;
    Settings settings = conf();
    OsmMapPtr map(new OsmMap());

    settings.set("poi.polygon.address.translate.to.english", "true");
    settings.set("language.translation.translator", "hoot::ToEnglishDictionaryTranslator");
    settings.set("poi.polygon.address.use.default.language.translation.only", "false");
    uut.setConfiguration(settings);
    std::shared_ptr<ToEnglishDictionaryTranslator> dictTranslator =
      std::dynamic_pointer_cast<ToEnglishDictionaryTranslator>(
        ToEnglishAddressTranslator::_translator);
    dictTranslator->setTokenizeInput(false);

    NodePtr node1(new Node(Status::Unknown1, -1, Coordinate(0.0, 0.0), 15.0));
    node1->getTags().set(TestUtils::FULL_ADDRESS_TAG_NAME, "123 Main Street");
    map->addNode(node1);
    WayPtr way1(new Way(Status::Unknown2, -1, 15.0));
    way1->getTags().set(
      TestUtils::FULL_ADDRESS_TAG_NAME, QString::fromUtf8("123 Hauptstraße"));
    map->addWay(way1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node1, way1), 0.0);

    NodePtr node2(new Node(Status::Unknown1, -1, Coordinate(0.0, 0.0), 15.0));
    node2->getTags().set(
      TestUtils::FULL_ADDRESS_TAG_NAME_2, "Central Border Street 40 81379");
    map->addNode(node2);
    WayPtr way2(new Way(Status::Unknown2, -1, 15.0));
    way2->getTags().set(
      TestUtils::FULL_ADDRESS_TAG_NAME_2,
      QString::fromUtf8("ZENTRALLÄNDE STRASSE 40 81379 MÜNCHEN"));
    map->addWay(way2);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node2, way2), 0.0);

    settings.set("poi.polygon.address.translate.to.english", "false");
    uut.setConfiguration(settings);
     CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, uut.extract(*map, node1, way1), 0.0);
  }

  void additionalTagsTest()
  {
    PoiPolygonAddressScoreExtractor uut;
    uut.setConfiguration(conf());
    QSet<QString> additionalTagKeys;
    additionalTagKeys.insert("note");
    additionalTagKeys.insert("description");
    AddressTagKeysPtr addressTagKeys = AddressTagKeys::getInstance();
    addressTagKeys->_additionalTagKeys = additionalTagKeys;

    OsmMapPtr map(new OsmMap());
    NodePtr node1(new Node(Status::Unknown1, -1, Coordinate(0.0, 0.0), 15.0));
    map->addNode(node1);
    WayPtr way1(new Way(Status::Unknown2, -1, 15.0));
    map->addWay(way1);

    node1->getTags().set("note", "123 Main Street");
    way1->getTags().set("note", "123 main St");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node1, way1), 0.0);

    node1->getTags().clear();
    node1->getTags().set("description", "123 Main Street");
    way1->getTags().clear();
    way1->getTags().set("description", "123 main St");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node1, way1), 0.0);

    node1->getTags().clear();
    node1->getTags().set("blah", "123 Main Street");
    way1->getTags().clear();
    way1->getTags().set("blah", "123 main St");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, uut.extract(*map, node1, way1), 0.0);

    // name gets parsed by default
    node1->getTags().clear();
    node1->getTags().set("name", "123 Main Street");
    way1->getTags().clear();
    way1->getTags().set("name", "123 main St");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node1, way1), 0.0);
  }

  void invalidFullAddressTest()
  {
    PoiPolygonAddressScoreExtractor uut;
    uut.setConfiguration(conf());

    OsmMapPtr map(new OsmMap());
    NodePtr node1(new Node(Status::Unknown1, -1, Coordinate(0.0, 0.0), 15.0));
    map->addNode(node1);
    WayPtr way1(new Way(Status::Unknown2, -1, 15.0));
    map->addWay(way1);

    node1->getTags().set(TestUtils::FULL_ADDRESS_TAG_NAME, "123 Main Street");
    way1->getTags().set(TestUtils::FULL_ADDRESS_TAG_NAME, "this isn't an address");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, uut.extract(*map, node1, way1), 0.0);

    node1->getTags().clear();
    node1->getTags().set(TestUtils::FULL_ADDRESS_TAG_NAME, "this isn't an address");
    way1->getTags().clear();
    way1->getTags().set(TestUtils::FULL_ADDRESS_TAG_NAME, "123 Main Street");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, uut.extract(*map, node1, way1), 0.0);

    node1->getTags().clear();
    node1->getTags().set(TestUtils::FULL_ADDRESS_TAG_NAME, "123 this isn't an address");
    way1->getTags().clear();
    way1->getTags().set(TestUtils::FULL_ADDRESS_TAG_NAME, "123 Main Street");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, uut.extract(*map, node1, way1), 0.0);

    node1->getTags().clear();
    node1->getTags().set(TestUtils::FULL_ADDRESS_TAG_NAME, "this isn't an address street");
    way1->getTags().clear();
    way1->getTags().set(TestUtils::FULL_ADDRESS_TAG_NAME, "123 Main Street");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, uut.extract(*map, node1, way1), 0.0);
  }

  void invalidComponentAddressTest()
  {
    PoiPolygonAddressScoreExtractor uut;
    uut.setConfiguration(conf());

    OsmMapPtr map(new OsmMap());
    NodePtr node1(new Node(Status::Unknown1, -1, Coordinate(0.0, 0.0), 15.0));
    map->addNode(node1);
    WayPtr way1(new Way(Status::Unknown2, -1, 15.0));
    map->addWay(way1);

    node1->getTags().set(TestUtils::FULL_ADDRESS_TAG_NAME, "123 Main Street");

    way1->getTags().set(TestUtils::HOUSE_NUMBER_TAG_NAME, "123");
    way1->getTags().set(TestUtils::STREET_TAG_NAME, "this isn't an address");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, uut.extract(*map, node1, way1), 0.0);

    way1->getTags().clear();
    way1->getTags().set(TestUtils::HOUSE_NUMBER_TAG_NAME, "blah");
    way1->getTags().set(TestUtils::STREET_TAG_NAME, "main street");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, uut.extract(*map, node1, way1), 0.0);

    way1->getTags().clear();
    way1->getTags().set(TestUtils::HOUSE_NUMBER_TAG_NAME, "blah");
    way1->getTags().set(TestUtils::STREET_TAG_NAME, "street");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, uut.extract(*map, node1, way1), 0.0);

    way1->getTags().clear();
    way1->getTags().set(TestUtils::HOUSE_NUMBER_TAG_NAME, "");
    way1->getTags().set(TestUtils::STREET_TAG_NAME, "main street");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, uut.extract(*map, node1, way1), 0.0);

    way1->getTags().clear();
    way1->getTags().set(TestUtils::HOUSE_NUMBER_TAG_NAME, "123");
    way1->getTags().set(TestUtils::STREET_TAG_NAME, "");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, uut.extract(*map, node1, way1), 0.0);
  }

  void addressNormalizationTest()
  {
    PoiPolygonAddressScoreExtractor uut;
    uut.setConfiguration(conf());

    OsmMapPtr map(new OsmMap());
    NodePtr node1(new Node(Status::Unknown1, -1, Coordinate(0.0, 0.0), 15.0));
    map->addNode(node1);
    WayPtr way1(new Way(Status::Unknown2, -1, 15.0));
    map->addWay(way1);

    node1->getTags().set(
      TestUtils::FULL_ADDRESS_TAG_NAME,
      QString::fromUtf8("Quatre-vingt-douze Ave des Champs-Élysées"));
    way1->getTags().set(
      TestUtils::FULL_ADDRESS_TAG_NAME, QString::fromUtf8("92 avenue des champs-elysees"));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node1, way1), 0.0);

    node1->getTags().clear();
    node1->getTags().set(
      TestUtils::FULL_ADDRESS_TAG_NAME,
      QString::fromUtf8("Quatre-vingt-douze Ave des Champs-Élysées"));
    way1->getTags().clear();
    way1->getTags().set(
      TestUtils::FULL_ADDRESS_TAG_NAME, QString::fromUtf8("92 avenue des champs elysees"));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node1, way1), 0.0);
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(PoiPolygonAddressScoreExtractorTest, "slow");

}
