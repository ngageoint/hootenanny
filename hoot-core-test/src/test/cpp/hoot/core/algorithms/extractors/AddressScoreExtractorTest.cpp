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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/algorithms/extractors/AddressScoreExtractor.h>
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

/*
 * Its possible we might want to eventually break the parsing and normalization tests out
 * separately.
 *
 * This test took a hit in readability when moved from PoiPolygonAddressScoreExtractor to the more
 * generic AddressScoreExtractor, so could use some simplification.
 */
class AddressScoreExtractorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(AddressScoreExtractorTest);
  CPPUNIT_TEST(runTagTest);
  CPPUNIT_TEST(runCombinedTagTest);
  CPPUNIT_TEST(runRangeTest);
  CPPUNIT_TEST(runAltFormatTest);
  CPPUNIT_TEST(runSubLetterTest);
  CPPUNIT_TEST(runIntersectionTest);
  CPPUNIT_TEST(runWayTest);
  CPPUNIT_TEST(runRelationTest);
  CPPUNIT_TEST(translateTagValueTest);
  CPPUNIT_TEST(invalidFullAddressTest);
  CPPUNIT_TEST(invalidComponentAddressTest);
  // TODO: need to get this test back into the mix after reworking its Singleton access
  //CPPUNIT_TEST(additionalTagsTest);
  CPPUNIT_TEST(noStreetNumberTest);
  CPPUNIT_TEST_SUITE_END();

public:

  AddressScoreExtractorTest()
  {
  }

  void runTagTest()
  {
    AddressScoreExtractor uut;
    uut.setConfiguration(conf());
    // The cache needs to be disabled since its static and all of these tests will run as threads
    // within the same process, thus sharing the cache. If we weren't using the same element IDs
    // across tests, then it wouldn't have to be disabled.
    uut.setCacheEnabled(false);
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

    NodePtr node3(new Node(Status::Unknown1, -1, Coordinate(0.0, 0.0), 15.0));
    node3->getTags().set(TestUtils::FULL_ADDRESS_TAG_NAME, "123 Main Street");
    map->addNode(node3);
    WayPtr way3(new Way(Status::Unknown2, -1, 15.0));
    way3->getTags().set(TestUtils::FULL_ADDRESS_TAG_NAME, "123 main St");
    map->addWay(way3);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node3, way3), 0.0);

    NodePtr node4(new Node(Status::Unknown1, -1, Coordinate(0.0, 0.0), 15.0));
    node4->getTags().set(TestUtils::FULL_ADDRESS_TAG_NAME, "123 Main Avenue");
    map->addNode(node4);
    WayPtr way4(new Way(Status::Unknown2, -1, 15.0));
    way4->getTags().set(TestUtils::FULL_ADDRESS_TAG_NAME, "123 main Ave");
    map->addWay(way4);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node4, way4), 0.0);
  }

  void runCombinedTagTest()
  {
    AddressScoreExtractor uut;
    uut.setConfiguration(conf());
    uut.setCacheEnabled(false);
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
    AddressScoreExtractor uut;
    uut.setConfiguration(conf());
    uut.setCacheEnabled(false);
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
    AddressScoreExtractor uut;
    uut.setConfiguration(conf());
    uut.setCacheEnabled(false);
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
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.0, uut.extract(*map, node2, way2), 0.01);

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
    AddressScoreExtractor uut;
    uut.setConfiguration(conf());
    uut.setCacheEnabled(false);
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

  void runIntersectionTest()
  {
    AddressScoreExtractor uut;
    uut.setConfiguration(conf());
    uut.setCacheEnabled(false);

    OsmMapPtr map(new OsmMap());
    NodePtr node1(new Node(Status::Unknown1, -1, Coordinate(0.0, 0.0), 15.0));
    map->addNode(node1);
    WayPtr way1(new Way(Status::Unknown2, -1, 15.0));
    map->addWay(way1);

    // TODO: explain

    node1->getTags().set(TestUtils::FULL_ADDRESS_TAG_NAME, "Jones Street and Bryant Street");
    way1->getTags().set(TestUtils::FULL_ADDRESS_TAG_NAME, "Jones Street and Bryant Street");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node1, way1), 0.0);

    node1->getTags().set(TestUtils::FULL_ADDRESS_TAG_NAME, "16th &amp; Bryant Street");
    way1->getTags().set(TestUtils::FULL_ADDRESS_TAG_NAME, "16th and Bryant Street");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node1, way1), 0.0);

    node1->getTags().set(TestUtils::FULL_ADDRESS_TAG_NAME, "16th St and Bryant Street");
    way1->getTags().set(TestUtils::FULL_ADDRESS_TAG_NAME, "16th and Bryant Street");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node1, way1), 0.0);

    node1->getTags().set(TestUtils::FULL_ADDRESS_TAG_NAME, "Jones St and Bryant Street");
    way1->getTags().set(TestUtils::FULL_ADDRESS_TAG_NAME, "Jones Street and Bryant Street");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node1, way1), 0.0);

    node1->getTags().set(TestUtils::FULL_ADDRESS_TAG_NAME, "Jones St and Bryant Street");
    way1->getTags().set(TestUtils::FULL_ADDRESS_TAG_NAME, "Jones and Bryant Street");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node1, way1), 0.0);

    node1->getTags().set(TestUtils::FULL_ADDRESS_TAG_NAME, "Jones St and Bryant Street");
    way1->getTags().set(TestUtils::FULL_ADDRESS_TAG_NAME, "Jones and Bryant Street");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node1, way1), 0.0);

    node1->getTags().clear();
    node1->getTags().set("name", "Jones St and Bryant Street");
    way1->getTags().set(TestUtils::FULL_ADDRESS_TAG_NAME, "Jones Street and Bryant Street");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node1, way1), 0.0);

    node1->getTags().clear();
    node1->getTags().set("name", "Jones and Bryant Street");
    way1->getTags().set(TestUtils::FULL_ADDRESS_TAG_NAME, "Jones Street and Bryant Street");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node1, way1), 0.0);

    node1->getTags().set(TestUtils::FULL_ADDRESS_TAG_NAME, "Jones St and Bryant Street");
    way1->getTags().set(TestUtils::FULL_ADDRESS_TAG_NAME, "Jones and Bryant");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.0, uut.extract(*map, node1, way1), 0.0);

    node1->getTags().set(TestUtils::FULL_ADDRESS_TAG_NAME, "Jones St and Bryant Street");
    way1->getTags().set(TestUtils::FULL_ADDRESS_TAG_NAME, "Jones &amp; Bryant");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.0, uut.extract(*map, node1, way1), 0.0);

    node1->getTags().clear();
    node1->getTags().set("name", "Jones and Bryant");
    way1->getTags().set(TestUtils::FULL_ADDRESS_TAG_NAME, "Jones Street and Bryant Street");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.0, uut.extract(*map, node1, way1), 0.0);

    node1->getTags().clear();
    node1->getTags().set("name", "Jones Street and Bryant");
    way1->getTags().set(TestUtils::FULL_ADDRESS_TAG_NAME, "Jones Street and Bryant Street");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.0, uut.extract(*map, node1, way1), 0.0);

    // Haven't wrapped my head around how to deal with these yet but not worrying too much yet, b/c
    // haven't found any instances where the inability to match address intersection has kept
    // something from conflating. If we need it, we should be able to come up with some custom logic
    // to handle cases like below if there's no way to make libpostal do it. AddressParser may need
    // some refactoring, though.

//    // sometimes the road type is expressed as plural in this type of intersection naming
//    node1->getTags().set(TestUtils::FULL_ADDRESS_TAG_NAME, "16th &amp; Bryant Street");
//    way1->getTags().set(TestUtils::FULL_ADDRESS_TAG_NAME, "16th and Bryant Streets");
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node1, way1), 0.0);

//    node1->getTags().set(TestUtils::FULL_ADDRESS_TAG_NAME, "6TH &amp; HOFF ST. PARKING GARAGE");
//    way1->getTags().set(
//      TestUtils::FULL_ADDRESS_TAG_NAME, "6th street and Hoff Street Parking Garage");
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node1, way1), 0.0);
  }

  void runWayTest()
  {
    AddressScoreExtractor uut;
    uut.setConfiguration(conf());
    uut.setCacheEnabled(false);
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

    NodePtr node5(new Node(Status::Unknown1, -5, Coordinate(0.0, 0.0), 15.0));
    map->addNode(node5);
    WayPtr way3(new Way(Status::Unknown2, -3, 15.0));
    way3->getTags().set(TestUtils::HOUSE_NUMBER_TAG_NAME, "123");
    way3->getTags().set(TestUtils::STREET_TAG_NAME, "Main Street");
    way3->addNode(node5->getId());
    map->addWay(way3);
    WayPtr way4(new Way(Status::Unknown2, -4, 15.0));
    way4->getTags().set(TestUtils::HOUSE_NUMBER_TAG_NAME, "123");
    way4->getTags().set(TestUtils::STREET_TAG_NAME, "Main Street");
    way4->addNode(node5->getId());
    map->addWay(way4);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, way3, way4), 0.01);
  }

  void runRelationTest()
  {
    AddressScoreExtractor uut;
    uut.setConfiguration(conf());
    uut.setCacheEnabled(false);
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

    NodePtr node7(new Node(Status::Unknown1, -7, Coordinate(0.0, 0.0), 15.0));
    node7->getTags().set(TestUtils::HOUSE_NUMBER_TAG_NAME, "123");
    node7->getTags().set(TestUtils::STREET_TAG_NAME, "main street");
    map->addNode(node7);
    RelationPtr relation5(new Relation(Status::Unknown2, -5, 15.0));
    WayPtr way4(new Way(Status::Unknown2, -4, 15.0));
    way4->getTags().set(TestUtils::HOUSE_NUMBER_TAG_NAME, "123");
    way4->getTags().set(TestUtils::STREET_TAG_NAME, "main street");
    map->addWay(way4);
    relation5->addElement("test", way4->getElementId());
    map->addRelation(relation5);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node7, relation5), 0.01);

    NodePtr node8(new Node(Status::Unknown1, -8, Coordinate(0.0, 0.0), 15.0));
    map->addNode(node8);
    WayPtr way5(new Way(Status::Unknown2, -5, 15.0));
    map->addWay(way5);
    RelationPtr relation6(new Relation(Status::Unknown2, -6, 15.0));
    relation6->addElement("test", way5->getElementId());
    relation6->getTags().set(TestUtils::HOUSE_NUMBER_TAG_NAME, "123");
    relation6->getTags().set(TestUtils::STREET_TAG_NAME, "main street");
    map->addRelation(relation5);
    RelationPtr relation7(new Relation(Status::Unknown2, -7, 15.0));
    relation7->addElement("test", node8->getElementId());
    relation7->getTags().set(TestUtils::HOUSE_NUMBER_TAG_NAME, "123");
    relation7->getTags().set(TestUtils::STREET_TAG_NAME, "main street");
    map->addRelation(relation7);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, relation6, relation7), 0.01);
  }

  void translateTagValueTest()
  {
    AddressScoreExtractor uut;
    Settings settings = conf();
    OsmMapPtr map(new OsmMap());

    settings.set("address.translate.to.english", "true");
    settings.set("language.translation.translator", "hoot::ToEnglishDictionaryTranslator");
    settings.set("address.use.default.language.translation.only", "false");
    uut.setConfiguration(settings);
    uut.setCacheEnabled(false);
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

    settings.set("address.translate.to.english", "false");
    uut.setConfiguration(settings);
    uut.setCacheEnabled(false);
     CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, uut.extract(*map, node1, way1), 0.0);
  }

  void invalidFullAddressTest()
  {
    AddressScoreExtractor uut;
    uut.setConfiguration(conf());
    uut.setCacheEnabled(false);

    OsmMapPtr map(new OsmMap());
    NodePtr node1(new Node(Status::Unknown1, -1, Coordinate(0.0, 0.0), 15.0));
    map->addNode(node1);
    WayPtr way1(new Way(Status::Unknown2, -1, 15.0));
    map->addWay(way1);

    node1->getTags().set(TestUtils::FULL_ADDRESS_TAG_NAME, "123 Main Street");
    way1->getTags().set(TestUtils::FULL_ADDRESS_TAG_NAME, "this isn't an address");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.0, uut.extract(*map, node1, way1), 0.0);

    node1->getTags().clear();
    node1->getTags().set(TestUtils::FULL_ADDRESS_TAG_NAME, "this isn't an address");
    way1->getTags().clear();
    way1->getTags().set(TestUtils::FULL_ADDRESS_TAG_NAME, "123 Main Street");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.0, uut.extract(*map, node1, way1), 0.0);

    node1->getTags().clear();
    node1->getTags().set(TestUtils::FULL_ADDRESS_TAG_NAME, "123 this isn't an address");
    way1->getTags().clear();
    way1->getTags().set(TestUtils::FULL_ADDRESS_TAG_NAME, "123 Main Street");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, uut.extract(*map, node1, way1), 0.0);

    node1->getTags().clear();
    node1->getTags().set(TestUtils::FULL_ADDRESS_TAG_NAME, "this isn't an address street");
    way1->getTags().clear();
    way1->getTags().set(TestUtils::FULL_ADDRESS_TAG_NAME, "123 Main Street");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.0, uut.extract(*map, node1, way1), 0.0);
  }

  void invalidComponentAddressTest()
  {
    AddressScoreExtractor uut;
    uut.setConfiguration(conf());
    uut.setCacheEnabled(false);

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
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.0, uut.extract(*map, node1, way1), 0.0);

    way1->getTags().clear();
    way1->getTags().set(TestUtils::HOUSE_NUMBER_TAG_NAME, "123");
    way1->getTags().set(TestUtils::STREET_TAG_NAME, "");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.0, uut.extract(*map, node1, way1), 0.0);
  }

//  void additionalTagsTest()
//  {
//    AddressScoreExtractor uut;
//    uut.setConfiguration(conf());
//    uut.setCacheEnabled(false);
//    QSet<QString> additionalTagKeys;
//    additionalTagKeys.insert("note");
//    additionalTagKeys.insert("description");
//    AddressTagKeysPtr addressTagKeys = AddressTagKeys::getInstance();
//    addressTagKeys->_additionalTagKeys = additionalTagKeys;

//     OsmMapPtr map(new OsmMap());
//    NodePtr node1(new Node(Status::Unknown1, -1, Coordinate(0.0, 0.0), 15.0));
//    map->addNode(node1);
//    WayPtr way1(new Way(Status::Unknown2, -1, 15.0));
//    map->addWay(way1);

//     node1->getTags().set("note", "123 Main Street");
//    way1->getTags().set("note", "123 main St");
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node1, way1), 0.0);

//     node1->getTags().clear();
//    node1->getTags().set("description", "123 Main Street");
//    way1->getTags().clear();
//    way1->getTags().set("description", "123 main St");
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node1, way1), 0.0);

//     node1->getTags().clear();
//    node1->getTags().set("blah", "123 Main Street");
//    way1->getTags().clear();
//    way1->getTags().set("blah", "123 main St");
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.0, uut.extract(*map, node1, way1), 0.0);

//     // name gets parsed by default
//    node1->getTags().clear();
//    node1->getTags().set("name", "123 Main Street");
//    way1->getTags().clear();
//    way1->getTags().set("name", "123 main St");
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node1, way1), 0.0);
//  }

  void noStreetNumberTest()
  {
    AddressScoreExtractor uut;
    uut.setConfiguration(conf());
    uut.setCacheEnabled(false);

    {
      OsmMapPtr map(new OsmMap());

      NodePtr node(new Node(Status::Unknown1, -1, Coordinate(0.0, 0.0), 15.0));
      node->getTags().set(TestUtils::FULL_ADDRESS_TAG_NAME, "Main Street");
      map->addNode(node);

      WayPtr way(new Way(Status::Unknown2, -1, 15.0));
      way->getTags().set(TestUtils::FULL_ADDRESS_TAG_NAME, "123 Main Street");
      map->addWay(way);

      CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.0, uut.extract(*map, node, way), 0.0);
    }

    {
      OsmMapPtr map(new OsmMap());

      NodePtr node(new Node(Status::Unknown1, -1, Coordinate(0.0, 0.0), 15.0));
      node->getTags().set(TestUtils::FULL_ADDRESS_TAG_NAME, "123 Main Street");
      map->addNode(node);

      WayPtr way(new Way(Status::Unknown2, -1, 15.0));
      way->getTags().set(TestUtils::FULL_ADDRESS_TAG_NAME, "Main Street");
      map->addWay(way);

      CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.0, uut.extract(*map, node, way), 0.0);
    }

    {
      OsmMapPtr map(new OsmMap());

      NodePtr node(new Node(Status::Unknown1, -1, Coordinate(0.0, 0.0), 15.0));
      node->getTags().set(TestUtils::FULL_ADDRESS_TAG_NAME, "Main Street");
      map->addNode(node);

      WayPtr way(new Way(Status::Unknown2, -1, 15.0));
      way->getTags().set(TestUtils::FULL_ADDRESS_TAG_NAME, "Main Street");
      map->addWay(way);

      CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.0, uut.extract(*map, node, way), 0.0);
    }

    {
      OsmMapPtr map(new OsmMap());

      WayPtr way(new Way(Status::Unknown1, -1, 15.0));
      way->getTags().set(TestUtils::CITY_TAG_NAME, "San Ignacio");
      way->getTags().set(TestUtils::STREET_TAG_NAME, "Hudson Street");
      map->addWay(way);

      NodePtr node(new Node(Status::Unknown1, -1, Coordinate(0.0, 0.0), 15.0));
      node->getTags().set(TestUtils::FULL_ADDRESS_TAG_NAME, "Hudson Street, San Ignacio, Belize");
      map->addNode(node);

      CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.0, uut.extract(*map, node, way), 0.0);
    }
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(AddressScoreExtractorTest, "slow");

}
