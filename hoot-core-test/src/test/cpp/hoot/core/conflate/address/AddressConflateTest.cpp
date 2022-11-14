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
 * @copyright Copyright (C) 2015, 2016, 2018, 2019, 2021, 2022 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/algorithms/extractors/AddressScoreExtractor.h>
#include <hoot/core/conflate/ConflateExecutor.h>
#include <hoot/core/criterion/HasAddressCriterion.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/language/ToEnglishDictionaryTranslator.h>
#include <hoot/core/schema/OverwriteTagMerger.h>
#include <hoot/core/visitors/AddressCountVisitor.h>
#include <hoot/core/visitors/NormalizeAddressesVisitor.h>

namespace hoot
{

/**
 * @brief The AddressConflateTest class is the single stop shop for any testing done with the
 * address data that gets loaded by libpostal.
 *
 * Read the Hootenanny Developer Guide section entitled, "Testing with Address Data", to understand
 * why address related testing is done in this manner (single CPPUnit tests calling multiple test
 * functions). Note that not actually all of these tests are running conflation. The ones that do
 * need a full environment reset per test in order to allow for changing match/merger creators (they
 * were ported over from conflate case tests).
 */
class AddressConflateTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(AddressConflateTest);
  CPPUNIT_TEST(runTest);
  CPPUNIT_TEST_SUITE_END();

public:

  AddressConflateTest() :
  HootTestFixture(
    "test-files/conflate/address/AddressConflateTest/",
    "test-output/conflate/address/AddressConflateTest/")
  {
    setResetType(ResetAll);
  }

  void setUp() override
  {
    HootTestFixture::setUp();
    setupTestConfig();
  }

  void setupTestConfig()
  {
    //  Enable address matching
    conf().set(ConfigOptions::getAddressMatchEnabledKey(), true);
    conf().set(ConfigOptions::getAddressScorerEnableCachingKey(), false);
    conf().set(ConfigOptions::getUuidHelperRepeatableKey(), true);
    conf().set(ConfigOptions::getPoiPolygonTagMergerKey(), OverwriteTag2Merger::className());
  }

  void resetTest()
  {
    //  Reset the entire environment
    TestUtils::resetAll();
    //  Setup the address matching configuration settings
    setupTestConfig();
  }

  void runTest()
  {
    _addressCountVisitorBasicTest();

    _hasAddressCriterionBasicTest();

    _normalizeAddressesVisitorBasicTest();

    _addressScoreExtractorTagTest();
    _addressScoreExtractorCombinedTagTest();
    _addressScoreExtractorRangeTest();
    _addressScoreExtractorAltFormatTest();
    _addressScoreExtractorSubLetterTest();
    _addressScoreExtractorIntersectionTest();
    _addressScoreExtractorWayTest();
    _addressScoreExtractorRelationTest();
    _addressScoreExtractorTranslateTagValueTest();
    _addressScoreExtractorInvalidFullAddressTest();
    _addressScoreExtractorInvalidComponentAddressTest();
    _addressScoreExtractorNoStreetNumberTest();
    _addressScoreExtractorPartialMatchTest();

    //  Test order in important here, do not change
    _building3441Addresses1Test();
    _building3441Addresses2Test();
    _poiPolygon10Test();
    _poiPolygonAutoMerge13Test();
    _poiPolygonAutoMerge14Test();
    _poiPolygonRecursiveWayAddress3267_1Test();
    _poiPolygonReviewConflict4331_3Test();
  }

private:

  void _addressCountVisitorBasicTest()
  {
    LOG_DEBUG("_addressCountVisitorBasicTest");

    conf().set(ConfigOptions::getAddressMatchEnabledKey(), true);

    OsmMapPtr map = std::make_shared<OsmMap>();
    OsmMapReaderFactory::read(
      map,
      "test-files/cmd/glacial/PoiPolygonConflateStandaloneTest/PoiPolygon2.osm",
      false,
      Status::Unknown1);

    AddressCountVisitor uut;
    map->visitRo(uut);
    CPPUNIT_ASSERT_EQUAL(28, (int)uut.getStat());
    OsmMap::resetCounters();
  }

  void _hasAddressCriterionBasicTest()
  {
    LOG_DEBUG("_hasAddressCriterionBasicTest");

    HasAddressCriterion uut;
    uut.setConfiguration(conf());

    NodePtr node1 =
      std::make_shared<Node>(Status::Unknown1, -1, geos::geom::Coordinate(0.0, 0.0), 15.0);
    node1->getTags().set(AddressTagKeys::FULL_ADDRESS_TAG_NAME, "123 Main Street");
    CPPUNIT_ASSERT(uut.isSatisfied(node1));

    WayPtr way1 = std::make_shared<Way>(Status::Unknown1, -1, 15.0);
    way1->getTags().set(AddressTagKeys::FULL_ADDRESS_TAG_NAME, "blah");
    CPPUNIT_ASSERT(!uut.isSatisfied(way1));

    WayPtr way2 = std::make_shared<Way>(Status::Unknown1, -1, 15.0);
    way2->getTags().set("blah", "123 Main Street");
    CPPUNIT_ASSERT(!uut.isSatisfied(way2));
  }

  void _normalizeAddressesVisitorBasicTest()
  {
    LOG_DEBUG("_normalizeAddressesVisitorBasicTest");

    OsmMapPtr map = std::make_shared<OsmMap>();
    OsmMapReaderFactory::read(
      map,
      "test-files/cmd/glacial/PoiPolygonConflateStandaloneTest/PoiPolygon2.osm",
      false,
      Status::Unknown1);

    NormalizeAddressesVisitor uut;
    map->visitRw(uut);

    const QString outputFile = _outputPath + "normalizeAddressesVisitorBasicTest-out.osm";
    OsmMapWriterFactory::write(map, outputFile);

    CPPUNIT_ASSERT_EQUAL(28, uut._addressNormalizer.getNumNormalized());
    HOOT_FILE_EQUALS(_inputPath + "normalizeAddressesVisitorBasicTest-out.osm", outputFile);
    OsmMap::resetCounters();
  }

  void _addressScoreExtractorTagTest()
  {
    LOG_DEBUG("_addressScoreExtractorTagTest");

    AddressScoreExtractor uut;
    uut.setConfiguration(conf());
    // The cache needs to be disabled since its static and all of these tests will run as threads
    // within the same process, thus sharing the cache. If we weren't using the same element IDs
    // across tests, then it wouldn't have to be disabled.
    uut.setCacheEnabled(false);
    OsmMapPtr map = std::make_shared<OsmMap>();

    NodePtr node1 =
      std::make_shared<Node>(Status::Unknown1, -1, geos::geom::Coordinate(0.0, 0.0), 15.0);
    node1->getTags().set(AddressTagKeys::FULL_ADDRESS_TAG_NAME, "123 Main Street");
    map->addNode(node1);
    WayPtr way1 = std::make_shared<Way>(Status::Unknown2, -1, 15.0);
    way1->getTags().set(AddressTagKeys::FULL_ADDRESS_TAG_NAME, "123 main street");
    map->addWay(way1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node1, way1), 0.0);

    NodePtr node2 =
      std::make_shared<Node>(Status::Unknown1, -2, geos::geom::Coordinate(0.0, 0.0), 15.0);
    node2->getTags().set(AddressTagKeys::FULL_ADDRESS_TAG_NAME, "123 Main Street");
    map->addNode(node2);
    WayPtr way2 = std::make_shared<Way>(Status::Unknown2, -2, 15.0);
    way2->getTags().set(AddressTagKeys::FULL_ADDRESS_TAG_NAME, "567 first street");
    map->addWay(way2);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, uut.extract(*map, node2, way2), 0.01);

    NodePtr node3 =
      std::make_shared<Node>(Status::Unknown1, -1, geos::geom::Coordinate(0.0, 0.0), 15.0);
    node3->getTags().set(AddressTagKeys::FULL_ADDRESS_TAG_NAME, "123 Main Street");
    map->addNode(node3);
    WayPtr way3 = std::make_shared<Way>(Status::Unknown2, -1, 15.0);
    way3->getTags().set(AddressTagKeys::FULL_ADDRESS_TAG_NAME, "123 main St");
    map->addWay(way3);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node3, way3), 0.0);

    NodePtr node4 =
      std::make_shared<Node>(Status::Unknown1, -1, geos::geom::Coordinate(0.0, 0.0), 15.0);
    node4->getTags().set(AddressTagKeys::FULL_ADDRESS_TAG_NAME, "123 Main Avenue");
    map->addNode(node4);
    WayPtr way4 = std::make_shared<Way>(Status::Unknown2, -1, 15.0);
    way4->getTags().set(AddressTagKeys::FULL_ADDRESS_TAG_NAME, "123 main Ave");
    map->addWay(way4);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node4, way4), 0.0);
  }

  void _addressScoreExtractorCombinedTagTest()
  {
    LOG_DEBUG("_addressScoreExtractorCombinedTagTest");

    AddressScoreExtractor uut;
    uut.setConfiguration(conf());
    uut.setCacheEnabled(false);
    OsmMapPtr map = std::make_shared<OsmMap>();

    NodePtr node1 =
      std::make_shared<Node>(Status::Unknown1, -1, geos::geom::Coordinate(0.0, 0.0), 15.0);
    node1->getTags().set(AddressTagKeys::HOUSE_NUMBER_TAG_NAME, "123");
    node1->getTags().set(AddressTagKeys::STREET_TAG_NAME, "Main Street");
    map->addNode(node1);
    WayPtr way1 = std::make_shared<Way>(Status::Unknown2, -1, 15.0);
    way1->getTags().set(AddressTagKeys::HOUSE_NUMBER_TAG_NAME, "123");
    way1->getTags().set(AddressTagKeys::STREET_TAG_NAME, "main street");
    map->addWay(way1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node1, way1), 0.0);

    NodePtr node2 =
      std::make_shared<Node>(Status::Unknown1, -2, geos::geom::Coordinate(0.0, 0.0), 15.0);
    node2->getTags().set(AddressTagKeys::HOUSE_NUMBER_TAG_NAME, "123");
    node2->getTags().set(AddressTagKeys::STREET_TAG_NAME, "Main Street");
    map->addNode(node2);
    WayPtr way2 = std::make_shared<Way>(Status::Unknown2, -2, 15.0);
    way2->getTags().set(AddressTagKeys::HOUSE_NUMBER_TAG_NAME, "567");
    way2->getTags().set(AddressTagKeys::STREET_TAG_NAME, "first street");
    map->addWay(way2);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, uut.extract(*map, node2, way2), 0.01);
  }

  void _addressScoreExtractorRangeTest()
  {
    LOG_DEBUG("_addressScoreExtractorRangeTest");

    AddressScoreExtractor uut;
    uut.setConfiguration(conf());
    uut.setCacheEnabled(false);
    OsmMapPtr map = std::make_shared<OsmMap>();

    NodePtr node1 =
      std::make_shared<Node>(Status::Unknown1, -1, geos::geom::Coordinate(0.0, 0.0), 15.0);
    node1->getTags().set(AddressTagKeys::HOUSE_NUMBER_TAG_NAME, "123-125");
    node1->getTags().set(AddressTagKeys::STREET_TAG_NAME, "Main Street");
    map->addNode(node1);
    WayPtr way1 = std::make_shared<Way>(Status::Unknown2, -1, 15.0);
    way1->getTags().set(AddressTagKeys::HOUSE_NUMBER_TAG_NAME, "124");
    way1->getTags().set(AddressTagKeys::STREET_TAG_NAME, "main street");
    map->addWay(way1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node1, way1), 0.0);

    NodePtr node2 =
      std::make_shared<Node>(Status::Unknown1, -2, geos::geom::Coordinate(0.0, 0.0), 15.0);
    node2->getTags().set(AddressTagKeys::HOUSE_NUMBER_TAG_NAME, "123-125");
    node2->getTags().set(AddressTagKeys::STREET_TAG_NAME, "Main Street");
    map->addNode(node2);
    WayPtr way2 = std::make_shared<Way>(Status::Unknown2, -2, 15.0);
    way2->getTags().set(AddressTagKeys::HOUSE_NUMBER_TAG_NAME, "123");
    way2->getTags().set(AddressTagKeys::STREET_TAG_NAME, "main street");
    map->addWay(way2);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node2, way2), 0.0);

    NodePtr node3 =
      std::make_shared<Node>(Status::Unknown1, -3, geos::geom::Coordinate(0.0, 0.0), 15.0);
    node3->getTags().set(AddressTagKeys::HOUSE_NUMBER_TAG_NAME, "123-125");
    node3->getTags().set(AddressTagKeys::STREET_TAG_NAME, "Main Street");
    map->addNode(node3);
    WayPtr way3 = std::make_shared<Way>(Status::Unknown2, -3, 15.0);
    way3->getTags().set(AddressTagKeys::HOUSE_NUMBER_TAG_NAME, "567");
    way3->getTags().set(AddressTagKeys::STREET_TAG_NAME, "first street");
    map->addWay(way3);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, uut.extract(*map, node3, way3), 0.01);
  }

  void _addressScoreExtractorAltFormatTest()
  {
    LOG_DEBUG("_addressScoreExtractorAltFormatTest");

    AddressScoreExtractor uut;
    uut.setConfiguration(conf());
    uut.setCacheEnabled(false);
    OsmMapPtr map = std::make_shared<OsmMap>();

    NodePtr node1 =
      std::make_shared<Node>(Status::Unknown1, -1, geos::geom::Coordinate(0.0, 0.0), 15.0);
    node1->getTags().set(
      AddressTagKeys::FULL_ADDRESS_TAG_NAME_2, "Main Street 123 20121 mytown");
    map->addNode(node1);
    WayPtr way1 = std::make_shared<Way>(Status::Unknown2, -1, 15.0);
    way1->getTags().set(AddressTagKeys::FULL_ADDRESS_TAG_NAME_2, "main street 123");
    map->addWay(way1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node1, way1), 0.0);

    NodePtr node2 =
      std::make_shared<Node>(Status::Unknown1, -2, geos::geom::Coordinate(0.0, 0.0), 15.0);
    node2->getTags().set(
      AddressTagKeys::FULL_ADDRESS_TAG_NAME_2, "Main Street 123 20121 mytown");
    map->addNode(node2);
    WayPtr way2 = std::make_shared<Way>(Status::Unknown2, -2, 15.0);
    way2->getTags().set(
      AddressTagKeys::FULL_ADDRESS_TAG_NAME_2, "first street 567");
    map->addWay(way2);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.0, uut.extract(*map, node2, way2), 0.01);

    NodePtr node3 =
      std::make_shared<Node>(Status::Unknown1, -1, geos::geom::Coordinate(0.0, 0.0), 15.0);
    node3->getTags().set(
      AddressTagKeys::FULL_ADDRESS_TAG_NAME_2, "ZENTRALLÄNDSTRASSE 40 81379 MÜNCHEN");
    map->addNode(node3);
    WayPtr way3 = std::make_shared<Way>(Status::Unknown2, -1, 15.0);
    way3->getTags().set(AddressTagKeys::FULL_ADDRESS_TAG_NAME_2, "40 ZENTRALLÄNDSTRASSE");
    map->addWay(way3);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node3, way3), 0.0);
  }

  void _addressScoreExtractorSubLetterTest()
  {
    LOG_DEBUG("_addressScoreExtractorSubLetterTest");

    AddressScoreExtractor uut;
    uut.setConfiguration(conf());
    uut.setCacheEnabled(false);
    OsmMapPtr map = std::make_shared<OsmMap>();

    NodePtr node1 =
      std::make_shared<Node>(Status::Unknown1, -1, geos::geom::Coordinate(0.0, 0.0), 15.0);
    node1->getTags().set(AddressTagKeys::HOUSE_NUMBER_TAG_NAME, "123");
    node1->getTags().set(AddressTagKeys::STREET_TAG_NAME, "Main Street");
    map->addNode(node1);
    WayPtr way1 = std::make_shared<Way>(Status::Unknown2, -1, 15.0);
    way1->getTags().set(AddressTagKeys::HOUSE_NUMBER_TAG_NAME, "123a");
    way1->getTags().set(AddressTagKeys::STREET_TAG_NAME, "main street");
    map->addWay(way1);

    uut.setAllowLenientHouseNumberMatching(true);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node1, way1), 0.0);

    uut.setAllowLenientHouseNumberMatching(false);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, uut.extract(*map, node1, way1), 0.0);
  }

  void _addressScoreExtractorIntersectionTest()
  {
    LOG_DEBUG("_addressScoreExtractorIntersectionTest");

    AddressScoreExtractor uut;
    uut.setConfiguration(conf());
    uut.setCacheEnabled(false);

    OsmMapPtr map = std::make_shared<OsmMap>();
    NodePtr node1 =
      std::make_shared<Node>(Status::Unknown1, -1, geos::geom::Coordinate(0.0, 0.0), 15.0);
    map->addNode(node1);
    WayPtr way1 = std::make_shared<Way>(Status::Unknown2, -1, 15.0);
    map->addWay(way1);

    node1->getTags().set(AddressTagKeys::FULL_ADDRESS_TAG_NAME, "Jones Street and Bryant Street");
    way1->getTags().set(AddressTagKeys::FULL_ADDRESS_TAG_NAME, "Jones Street and Bryant Street");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node1, way1), 0.0);

    node1->getTags().set(AddressTagKeys::FULL_ADDRESS_TAG_NAME, "16th &amp; Bryant Street");
    way1->getTags().set(AddressTagKeys::FULL_ADDRESS_TAG_NAME, "16th and Bryant Street");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node1, way1), 0.0);

    node1->getTags().set(AddressTagKeys::FULL_ADDRESS_TAG_NAME, "16th St and Bryant Street");
    way1->getTags().set(AddressTagKeys::FULL_ADDRESS_TAG_NAME, "16th and Bryant Street");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node1, way1), 0.0);

    node1->getTags().set(AddressTagKeys::FULL_ADDRESS_TAG_NAME, "Jones St and Bryant Street");
    way1->getTags().set(AddressTagKeys::FULL_ADDRESS_TAG_NAME, "Jones Street and Bryant Street");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node1, way1), 0.0);

    node1->getTags().set(AddressTagKeys::FULL_ADDRESS_TAG_NAME, "Jones St and Bryant Street");
    way1->getTags().set(AddressTagKeys::FULL_ADDRESS_TAG_NAME, "Jones and Bryant Street");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node1, way1), 0.0);

    node1->getTags().set(AddressTagKeys::FULL_ADDRESS_TAG_NAME, "Jones St and Bryant Street");
    way1->getTags().set(AddressTagKeys::FULL_ADDRESS_TAG_NAME, "Jones and Bryant Street");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node1, way1), 0.0);

    node1->getTags().set(AddressTagKeys::FULL_ADDRESS_TAG_NAME, "Jones St and Bryant Street");
    way1->getTags().set(AddressTagKeys::FULL_ADDRESS_TAG_NAME, "Jones and Bryant");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node1, way1), 0.0);

    node1->getTags().set(AddressTagKeys::FULL_ADDRESS_TAG_NAME, "Jones St and Bryant Street");
    way1->getTags().set(AddressTagKeys::FULL_ADDRESS_TAG_NAME, "Jones &amp; Bryant");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node1, way1), 0.0);

    // If an intersection address comes from a name tag, we must be more strict. In that case, we
    // require that the address end with a street type token. However, I don't think this is
    // foolproof yet and will deal with exceptions as they come along.

    node1->getTags().clear();
    node1->getTags().set("name", "Jones St and Bryant Street");
    way1->getTags().set(AddressTagKeys::FULL_ADDRESS_TAG_NAME, "Jones Street and Bryant Street");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node1, way1), 0.0);

    node1->getTags().set("name", "Jones and Bryant Street");
    way1->getTags().set(AddressTagKeys::FULL_ADDRESS_TAG_NAME, "Jones Street and Bryant Street");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node1, way1), 0.0);

    node1->getTags().set("name", "Jones and Bryant");
    way1->getTags().set(AddressTagKeys::FULL_ADDRESS_TAG_NAME, "Jones Street and Bryant Street");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.0, uut.extract(*map, node1, way1), 0.0);

    node1->getTags().set("name", "Jones Street and Bryant");
    way1->getTags().set(AddressTagKeys::FULL_ADDRESS_TAG_NAME, "Jones Street and Bryant Street");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.0, uut.extract(*map, node1, way1), 0.0);

    // sometimes the road type is expressed as plural with intersections
    node1->getTags().clear();
    node1->getTags().set("address", "CASTRO AND DUBOCE STREETS");
    way1->getTags().clear();
    way1->getTags().set(AddressTagKeys::HOUSE_NUMBER_TAG_NAME, "45");
    way1->getTags().set(AddressTagKeys::STREET_TAG_NAME, "Castro Street");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.8, uut.extract(*map, node1, way1), 0.0);

    node1->getTags().clear();
    node1->getTags().set(AddressTagKeys::FULL_ADDRESS_TAG_NAME, "16th &amp; Bryant Street");
    way1->getTags().clear();
    way1->getTags().set(AddressTagKeys::FULL_ADDRESS_TAG_NAME, "16th and Bryant Streets");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node1, way1), 0.0);

    node1->getTags().clear();
    node1->getTags().set("address", "19th Ave & Wawona St");
    way1->getTags().clear();
    way1->getTags().set(AddressTagKeys::HOUSE_NUMBER_TAG_NAME, "2695");
    way1->getTags().set(AddressTagKeys::STREET_TAG_NAME, "19th Avenue");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.8, uut.extract(*map, node1, way1), 0.0);

    // Below are some that have been seen but haven't figured out how to deal with them yet. Not
    // worrying about them at this time, b/c haven't found any instances where the inability to
    // match address intersection has kept something from conflating.

//    node1->getTags().set(AddressTagKeys::FULL_ADDRESS_TAG_NAME, "6TH &amp; HOFF ST. PARKING GARAGE");
//    way1->getTags().set(
//      AddressTagKeys::FULL_ADDRESS_TAG_NAME, "6th street and Hoff Street Parking Garage");
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node1, way1), 0.0);

    // Have also seen this: 'name = Valencia @ 14th Streets'. Not really sure @ should be added to
    // the intersection split tokens, but maybe.
  }

  void _addressScoreExtractorWayTest()
  {
    LOG_DEBUG("_addressScoreExtractorWayTest");

    AddressScoreExtractor uut;
    uut.setConfiguration(conf());
    uut.setCacheEnabled(false);
    OsmMapPtr map = std::make_shared<OsmMap>();

    NodePtr node1 =
      std::make_shared<Node>(Status::Unknown1, -1, geos::geom::Coordinate(0.0, 0.0), 15.0);
    node1->getTags().set(AddressTagKeys::HOUSE_NUMBER_TAG_NAME, "123");
    node1->getTags().set(AddressTagKeys::STREET_TAG_NAME, "Main Street");
    map->addNode(node1);
    WayPtr way1 = std::make_shared<Way>(Status::Unknown2, -1, 15.0);
    NodePtr node2 =
      std::make_shared<Node>(Status::Unknown1, -2, geos::geom::Coordinate(0.0, 0.0), 15.0);
    node2->getTags().set(AddressTagKeys::HOUSE_NUMBER_TAG_NAME, "123");
    node2->getTags().set(AddressTagKeys::STREET_TAG_NAME, "main street");
    map->addNode(node2);
    way1->addNode(node2->getId());
    map->addWay(way1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node1, way1), 0.0);

    NodePtr node3 =
      std::make_shared<Node>(Status::Unknown1, -3, geos::geom::Coordinate(0.0, 0.0), 15.0);
    node3->getTags().set(AddressTagKeys::HOUSE_NUMBER_TAG_NAME, "123");
    node3->getTags().set(AddressTagKeys::STREET_TAG_NAME, "Main Street");
    map->addNode(node3);
    WayPtr way2 = std::make_shared<Way>(Status::Unknown2, -2, 15.0);
    NodePtr node4 =
      std::make_shared<Node>(Status::Unknown2, -4, geos::geom::Coordinate(0.0, 0.0), 15.0);
    node4->getTags().set(AddressTagKeys::HOUSE_NUMBER_TAG_NAME, "567");
    node4->getTags().set(AddressTagKeys::STREET_TAG_NAME, "first street");
    map->addNode(node4);
    way2->addNode(node4->getId());
    map->addWay(way2);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, uut.extract(*map, node3, way2), 0.01);

    NodePtr node5 =
      std::make_shared<Node>(Status::Unknown1, -5, geos::geom::Coordinate(0.0, 0.0), 15.0);
    map->addNode(node5);
    WayPtr way3 = std::make_shared<Way>(Status::Unknown2, -3, 15.0);
    way3->getTags().set(AddressTagKeys::HOUSE_NUMBER_TAG_NAME, "123");
    way3->getTags().set(AddressTagKeys::STREET_TAG_NAME, "Main Street");
    way3->addNode(node5->getId());
    map->addWay(way3);
    WayPtr way4 = std::make_shared<Way>(Status::Unknown2, -4, 15.0);
    way4->getTags().set(AddressTagKeys::HOUSE_NUMBER_TAG_NAME, "123");
    way4->getTags().set(AddressTagKeys::STREET_TAG_NAME, "Main Street");
    way4->addNode(node5->getId());
    map->addWay(way4);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, way3, way4), 0.01);
  }

  void _addressScoreExtractorRelationTest()
  {
    LOG_DEBUG("_addressScoreExtractorRelationTest");

    AddressScoreExtractor uut;
    uut.setConfiguration(conf());
    uut.setCacheEnabled(false);
    OsmMapPtr map = std::make_shared<OsmMap>();

    NodePtr node1 =
      std::make_shared<Node>(Status::Unknown1, -1, geos::geom::Coordinate(0.0, 0.0), 15.0);
    node1->getTags().set(AddressTagKeys::HOUSE_NUMBER_TAG_NAME, "123");
    node1->getTags().set(AddressTagKeys::STREET_TAG_NAME, "Main Street");
    map->addNode(node1);
    RelationPtr relation1 = std::make_shared<Relation>(Status::Unknown2, -1, 15.0);
    NodePtr node2 =
      std::make_shared<Node>(Status::Unknown1, -2, geos::geom::Coordinate(0.0, 0.0), 15.0);
    node2->getTags().set(AddressTagKeys::HOUSE_NUMBER_TAG_NAME, "123");
    node2->getTags().set(AddressTagKeys::STREET_TAG_NAME, "main street");
    map->addNode(node2);
    relation1->addElement("test", node2->getElementId());
    map->addRelation(relation1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node1, relation1), 0.0);

    NodePtr node4 =
      std::make_shared<Node>(Status::Unknown1, -4, geos::geom::Coordinate(0.0, 0.0), 15.0);
    node4->getTags().set(AddressTagKeys::HOUSE_NUMBER_TAG_NAME, "123");
    node4->getTags().set(AddressTagKeys::STREET_TAG_NAME, "Main Street");
    map->addNode(node4);
    RelationPtr relation3 = std::make_shared<Relation>(Status::Unknown2, -3, 15.0);
    NodePtr node5 =
      std::make_shared<Node>(Status::Unknown1, -5, geos::geom::Coordinate(0.0, 0.0), 15.0);
    node5->getTags().set(AddressTagKeys::HOUSE_NUMBER_TAG_NAME, "567");
    node5->getTags().set(AddressTagKeys::STREET_TAG_NAME, "first street");
    map->addNode(node5);
    relation3->addElement("test", node5->getElementId());
    map->addRelation(relation3);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, uut.extract(*map, node4, relation3), 0.01);

    NodePtr node6 =
      std::make_shared<Node>(Status::Unknown1, -6, geos::geom::Coordinate(0.0, 0.0), 15.0);
    node6->getTags().set(AddressTagKeys::HOUSE_NUMBER_TAG_NAME, "567");
    node6->getTags().set(AddressTagKeys::STREET_TAG_NAME, "first street");
    map->addNode(node6);
    RelationPtr relation4 = std::make_shared<Relation>(Status::Unknown2, -4, 15.0);
    WayPtr way3 = std::make_shared<Way>(Status::Unknown2, -3, 15.0);
    way3->getTags().set(AddressTagKeys::HOUSE_NUMBER_TAG_NAME, "123");
    way3->getTags().set(AddressTagKeys::STREET_TAG_NAME, "main street");
    map->addWay(way3);
    relation4->addElement("test", way3->getElementId());
    map->addRelation(relation4);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, uut.extract(*map, node6, relation4), 0.01);

    NodePtr node7 =
      std::make_shared<Node>(Status::Unknown1, -7, geos::geom::Coordinate(0.0, 0.0), 15.0);
    node7->getTags().set(AddressTagKeys::HOUSE_NUMBER_TAG_NAME, "123");
    node7->getTags().set(AddressTagKeys::STREET_TAG_NAME, "main street");
    map->addNode(node7);
    RelationPtr relation5 = std::make_shared<Relation>(Status::Unknown2, -5, 15.0);
    WayPtr way4 = std::make_shared<Way>(Status::Unknown2, -4, 15.0);
    way4->getTags().set(AddressTagKeys::HOUSE_NUMBER_TAG_NAME, "123");
    way4->getTags().set(AddressTagKeys::STREET_TAG_NAME, "main street");
    map->addWay(way4);
    relation5->addElement("test", way4->getElementId());
    map->addRelation(relation5);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node7, relation5), 0.01);

    NodePtr node8 =
      std::make_shared<Node>(Status::Unknown1, -8, geos::geom::Coordinate(0.0, 0.0), 15.0);
    map->addNode(node8);
    WayPtr way5 = std::make_shared<Way>(Status::Unknown2, -5, 15.0);
    map->addWay(way5);
    RelationPtr relation6 = std::make_shared<Relation>(Status::Unknown2, -6, 15.0);
    relation6->addElement("test", way5->getElementId());
    relation6->getTags().set(AddressTagKeys::HOUSE_NUMBER_TAG_NAME, "123");
    relation6->getTags().set(AddressTagKeys::STREET_TAG_NAME, "main street");
    map->addRelation(relation5);
    RelationPtr relation7 = std::make_shared<Relation>(Status::Unknown2, -7, 15.0);
    relation7->addElement("test", node8->getElementId());
    relation7->getTags().set(AddressTagKeys::HOUSE_NUMBER_TAG_NAME, "123");
    relation7->getTags().set(AddressTagKeys::STREET_TAG_NAME, "main street");
    map->addRelation(relation7);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, relation6, relation7), 0.01);
  }

  void _addressScoreExtractorTranslateTagValueTest()
  {
    LOG_DEBUG("_addressScoreExtractorTranslateTagValueTest");

    AddressScoreExtractor uut;
    Settings settings = conf();
    OsmMapPtr map = std::make_shared<OsmMap>();

    settings.set(ConfigOptions::getAddressTranslateToEnglishKey(), "true");
    settings.set(
      ConfigOptions::getLanguageTranslationTranslatorKey(),
      ToEnglishDictionaryTranslator::className());
    settings.set(ConfigOptions::getAddressUseDefaultLanguageTranslationOnlyKey(), "false");
    uut.setConfiguration(settings);
    uut.setCacheEnabled(false);
    std::shared_ptr<ToEnglishDictionaryTranslator> dictTranslator =
      std::dynamic_pointer_cast<ToEnglishDictionaryTranslator>(
        ToEnglishAddressTranslator::_translator);
    dictTranslator->setTokenizeInput(false);

    NodePtr node1 =
      std::make_shared<Node>(Status::Unknown1, -1, geos::geom::Coordinate(0.0, 0.0), 15.0);
    node1->getTags().set(AddressTagKeys::FULL_ADDRESS_TAG_NAME, "123 Main Street");
    map->addNode(node1);
    WayPtr way1 = std::make_shared<Way>(Status::Unknown2, -1, 15.0);
    way1->getTags().set(
      AddressTagKeys::FULL_ADDRESS_TAG_NAME, QString::fromUtf8("123 Hauptstraße"));
    map->addWay(way1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node1, way1), 0.0);

    NodePtr node2 =
      std::make_shared<Node>(Status::Unknown1, -1, geos::geom::Coordinate(0.0, 0.0), 15.0);
    node2->getTags().set(
      AddressTagKeys::FULL_ADDRESS_TAG_NAME_2, "Central Border Street 40 81379");
    map->addNode(node2);
    WayPtr way2 = std::make_shared<Way>(Status::Unknown2, -1, 15.0);
    way2->getTags().set(
      AddressTagKeys::FULL_ADDRESS_TAG_NAME_2,
      QString::fromUtf8("ZENTRALLÄNDE STRASSE 40 81379 MÜNCHEN"));
    map->addWay(way2);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node2, way2), 0.0);

    settings.set(ConfigOptions::getAddressTranslateToEnglishKey(), "false");
    uut.setConfiguration(settings);
    uut.setCacheEnabled(false);
     CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, uut.extract(*map, node1, way1), 0.0);
  }

  void _addressScoreExtractorInvalidFullAddressTest()
  {
    LOG_DEBUG("_addressScoreExtractorInvalidFullAddressTest");

    AddressScoreExtractor uut;
    uut.setConfiguration(conf());
    uut.setCacheEnabled(false);

    OsmMapPtr map = std::make_shared<OsmMap>();
    NodePtr node1 =
      std::make_shared<Node>(Status::Unknown1, -1, geos::geom::Coordinate(0.0, 0.0), 15.0);
    map->addNode(node1);
    WayPtr way1 = std::make_shared<Way>(Status::Unknown2, -1, 15.0);
    map->addWay(way1);

    node1->getTags().set(AddressTagKeys::FULL_ADDRESS_TAG_NAME, "123 Main Street");
    way1->getTags().set(AddressTagKeys::FULL_ADDRESS_TAG_NAME, "this isn't an address");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.0, uut.extract(*map, node1, way1), 0.0);

    node1->getTags().clear();
    node1->getTags().set(AddressTagKeys::FULL_ADDRESS_TAG_NAME, "this isn't an address");
    way1->getTags().clear();
    way1->getTags().set(AddressTagKeys::FULL_ADDRESS_TAG_NAME, "123 Main Street");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.0, uut.extract(*map, node1, way1), 0.0);

    node1->getTags().clear();
    node1->getTags().set(AddressTagKeys::FULL_ADDRESS_TAG_NAME, "123 this isn't an address");
    way1->getTags().clear();
    way1->getTags().set(AddressTagKeys::FULL_ADDRESS_TAG_NAME, "123 Main Street");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, uut.extract(*map, node1, way1), 0.0);

    node1->getTags().clear();
    node1->getTags().set(AddressTagKeys::FULL_ADDRESS_TAG_NAME, "this isn't an address street");
    way1->getTags().clear();
    way1->getTags().set(AddressTagKeys::FULL_ADDRESS_TAG_NAME, "123 Main Street");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.0, uut.extract(*map, node1, way1), 0.0);
  }

  void _addressScoreExtractorInvalidComponentAddressTest()
  {
    LOG_DEBUG("_addressScoreExtractorInvalidComponentAddressTest");

    AddressScoreExtractor uut;
    uut.setConfiguration(conf());
    uut.setCacheEnabled(false);

    OsmMapPtr map = std::make_shared<OsmMap>();
    NodePtr node1 =
      std::make_shared<Node>(Status::Unknown1, -1, geos::geom::Coordinate(0.0, 0.0), 15.0);
    map->addNode(node1);
    WayPtr way1 = std::make_shared<Way>(Status::Unknown2, -1, 15.0);
    map->addWay(way1);

    node1->getTags().set(AddressTagKeys::FULL_ADDRESS_TAG_NAME, "123 Main Street");

    way1->getTags().set(AddressTagKeys::HOUSE_NUMBER_TAG_NAME, "123");
    way1->getTags().set(AddressTagKeys::STREET_TAG_NAME, "this isn't an address");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, uut.extract(*map, node1, way1), 0.0);

    way1->getTags().clear();
    way1->getTags().set(AddressTagKeys::HOUSE_NUMBER_TAG_NAME, "blah");
    way1->getTags().set(AddressTagKeys::STREET_TAG_NAME, "main street");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, uut.extract(*map, node1, way1), 0.0);

    way1->getTags().clear();
    way1->getTags().set(AddressTagKeys::HOUSE_NUMBER_TAG_NAME, "blah");
    way1->getTags().set(AddressTagKeys::STREET_TAG_NAME, "street");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, uut.extract(*map, node1, way1), 0.0);

    way1->getTags().clear();
    way1->getTags().set(AddressTagKeys::HOUSE_NUMBER_TAG_NAME, "");
    way1->getTags().set(AddressTagKeys::STREET_TAG_NAME, "main street");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.0, uut.extract(*map, node1, way1), 0.0);

    way1->getTags().clear();
    way1->getTags().set(AddressTagKeys::HOUSE_NUMBER_TAG_NAME, "123");
    way1->getTags().set(AddressTagKeys::STREET_TAG_NAME, "");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.0, uut.extract(*map, node1, way1), 0.0);
  }

  void _addressScoreExtractorNoStreetNumberTest()
  {
    LOG_DEBUG("_addressScoreExtractorNoStreetNumberTest");

    AddressScoreExtractor uut;
    uut.setConfiguration(conf());
    uut.setCacheEnabled(false);

    {
      OsmMapPtr map = std::make_shared<OsmMap>();

      NodePtr node =
        std::make_shared<Node>(Status::Unknown1, -1, geos::geom::Coordinate(0.0, 0.0), 15.0);
      node->getTags().set(AddressTagKeys::FULL_ADDRESS_TAG_NAME, "Main Street");
      map->addNode(node);

      WayPtr way = std::make_shared<Way>(Status::Unknown2, -1, 15.0);
      way->getTags().set(AddressTagKeys::FULL_ADDRESS_TAG_NAME, "123 Main Street");
      map->addWay(way);

      CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.0, uut.extract(*map, node, way), 0.0);
    }

    {
      OsmMapPtr map = std::make_shared<OsmMap>();

      NodePtr node =
        std::make_shared<Node>(Status::Unknown1, -1, geos::geom::Coordinate(0.0, 0.0), 15.0);
      node->getTags().set(AddressTagKeys::FULL_ADDRESS_TAG_NAME, "123 Main Street");
      map->addNode(node);

      WayPtr way = std::make_shared<Way>(Status::Unknown2, -1, 15.0);
      way->getTags().set(AddressTagKeys::FULL_ADDRESS_TAG_NAME, "Main Street");
      map->addWay(way);

      CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.0, uut.extract(*map, node, way), 0.0);
    }

    {
      OsmMapPtr map = std::make_shared<OsmMap>();

      NodePtr node =
        std::make_shared<Node>(Status::Unknown1, -1, geos::geom::Coordinate(0.0, 0.0), 15.0);
      node->getTags().set(AddressTagKeys::FULL_ADDRESS_TAG_NAME, "Main Street");
      map->addNode(node);

      WayPtr way = std::make_shared<Way>(Status::Unknown2, -1, 15.0);
      way->getTags().set(AddressTagKeys::FULL_ADDRESS_TAG_NAME, "Main Street");
      map->addWay(way);

      CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.0, uut.extract(*map, node, way), 0.0);
    }

    {
      OsmMapPtr map = std::make_shared<OsmMap>();

      WayPtr way = std::make_shared<Way>(Status::Unknown1, -1, 15.0);
      way->getTags().set(AddressTagKeys::CITY_TAG_NAME, "San Ignacio");
      way->getTags().set(AddressTagKeys::STREET_TAG_NAME, "Hudson Street");
      map->addWay(way);

      NodePtr node =
        std::make_shared<Node>(Status::Unknown1, -1, geos::geom::Coordinate(0.0, 0.0), 15.0);
      node->getTags().set(
        AddressTagKeys::FULL_ADDRESS_TAG_NAME, "Hudson Street, San Ignacio, Belize");
      map->addNode(node);

      CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.0, uut.extract(*map, node, way), 0.0);
    }

    {
      OsmMapPtr map = std::make_shared<OsmMap>();

      WayPtr way = std::make_shared<Way>(Status::Unknown1, -1, 15.0);
      // See note in AddressTagKeys::_getAddressTag. Not completely sure that this is a valid
      // address but going with it for now.
      way->getTags().set(AddressTagKeys::HOUSE_NAME_TAG_NAME, "462");
      way->getTags().set(AddressTagKeys::STREET_TAG_NAME, "Duboce Avenue");
      map->addWay(way);

      NodePtr node =
        std::make_shared<Node>(Status::Unknown1, -1, geos::geom::Coordinate(0.0, 0.0), 15.0);
      node->getTags().set(AddressTagKeys::HOUSE_NAME_TAG_NAME, "462");
      node->getTags().set(AddressTagKeys::STREET_TAG_NAME, "Duboce Avenue");
      map->addNode(node);

      CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node, way), 0.0);
    }

    {
      OsmMapPtr map = std::make_shared<OsmMap>();

      WayPtr way = std::make_shared<Way>(Status::Unknown1, -1, 15.0);
      // See note in AddressTagKeys::_getAddressTag. Don't allow anything other than a number for
      // addr:housename posing as a street num.
      way->getTags().set(AddressTagKeys::HOUSE_NAME_TAG_NAME, "462 blah");
      way->getTags().set(AddressTagKeys::STREET_TAG_NAME, "Duboce Avenue");
      map->addWay(way);

      NodePtr node =
        std::make_shared<Node>(Status::Unknown1, -1, geos::geom::Coordinate(0.0, 0.0), 15.0);
      node->getTags().set(AddressTagKeys::STREET_TAG_NAME, "Duboce Avenue");
      map->addNode(node);

      CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.0, uut.extract(*map, node, way), 0.0);
    }

    {
      OsmMapPtr map = std::make_shared<OsmMap>();

      WayPtr way = std::make_shared<Way>(Status::Unknown1, -1, 15.0);
      way->getTags().set(AddressTagKeys::HOUSE_NAME_TAG_NAME, "462");
      way->getTags().set(AddressTagKeys::STREET_TAG_NAME, "Duboce Avenue");
      map->addWay(way);

      NodePtr node =
        std::make_shared<Node>(Status::Unknown1, -1, geos::geom::Coordinate(0.0, 0.0), 15.0);
      node->getTags().set(AddressTagKeys::STREET_TAG_NAME, "Duboce Avenue");
      map->addNode(node);

      CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.0, uut.extract(*map, node, way), 0.0);
    }
  }

  void _addressScoreExtractorPartialMatchTest()
  {
    LOG_DEBUG("_addressScoreExtractorPartialMatchTest");

    AddressScoreExtractor uut;
    uut.setConfiguration(conf());
    uut.setCacheEnabled(false);

    {
      // matching street names and house numbers, one doesn't have a suffix

      OsmMapPtr map = std::make_shared<OsmMap>();

      NodePtr node =
        std::make_shared<Node>(Status::Unknown1, -1, geos::geom::Coordinate(0.0, 0.0), 15.0);
      node->getTags().set("address", "670 Brunswick");
      map->addNode(node);

      WayPtr way = std::make_shared<Way>(Status::Unknown2, -1, 15.0);
      way->getTags().set(AddressTagKeys::HOUSE_NUMBER_TAG_NAME, "670");
      way->getTags().set(AddressTagKeys::STREET_TAG_NAME, "Brunswick Street");
      map->addWay(way);

      CPPUNIT_ASSERT_DOUBLES_EQUAL(0.8, uut.extract(*map, node, way), 0.0);
    }

    {
      // street names are spelled very slightly differently

      OsmMapPtr map = std::make_shared<OsmMap>();

      NodePtr node =
        std::make_shared<Node>(Status::Unknown1, -1, geos::geom::Coordinate(0.0, 0.0), 15.0);
      node->getTags().set("name", "100 Whitney Young Circle");
      map->addNode(node);

      WayPtr way = std::make_shared<Way>(Status::Unknown2, -1, 15.0);
      way->getTags().set(AddressTagKeys::HOUSE_NUMBER_TAG_NAME, "100");
      way->getTags().set(AddressTagKeys::HOUSE_NAME_TAG_NAME, "Whitney Yound Circle");
      map->addWay(way);

      CPPUNIT_ASSERT_DOUBLES_EQUAL(0.8, uut.extract(*map, node, way), 0.0);
    }

    {
      // range address where one has no suffix

      OsmMapPtr map = std::make_shared<OsmMap>();

      NodePtr node =
        std::make_shared<Node>(Status::Unknown1, -1, geos::geom::Coordinate(0.0, 0.0), 15.0);
      node->getTags().set("address", "150 Sutter Street");
      map->addNode(node);

      WayPtr way = std::make_shared<Way>(Status::Unknown2, -1, 15.0);
      way->getTags().set(AddressTagKeys::HOUSE_NAME_TAG_NAME, "Hallidie Building");
      way->getTags().set(AddressTagKeys::HOUSE_NUMBER_TAG_NAME, "130-150");
      way->getTags().set(AddressTagKeys::STREET_TAG_NAME, "Sutter");
      map->addWay(way);

      CPPUNIT_ASSERT_DOUBLES_EQUAL(0.8, uut.extract(*map, node, way), 0.0);
    }
  }

  void _building3441Addresses1Test()
  {
    LOG_DEBUG("_building3441Addresses1Test");

    resetTest();
    conf().set(ConfigOptions::getMatchCreatorsKey(), "BuildingMatchCreator");
    conf().set(ConfigOptions::getMergerCreatorsKey(), "BuildingMergerCreator");

    const QString outFile = _outputPath + "building-3441-addresses-1-out.osm";
    ConflateExecutor().conflate(
      _inputPath + "building-3441-addresses-1-in-1.osm",
      _inputPath + "building-3441-addresses-1-in-2.osm", outFile);

    HOOT_FILE_EQUALS(_inputPath + "building-3441-addresses-1-out.osm", outFile);
  }

  void _building3441Addresses2Test()
  {
    LOG_DEBUG("_building3441Addresses2Test");

    resetTest();
    conf().set(ConfigOptions::getMatchCreatorsKey(), "BuildingMatchCreator");
    conf().set(ConfigOptions::getMergerCreatorsKey(), "BuildingMergerCreator");

    const QString outFile = _outputPath + "building-3441-addresses-2-out.osm";
    ConflateExecutor().conflate(
      _inputPath + "building-3441-addresses-2-in-1.osm",
      _inputPath + "building-3441-addresses-2-in-2.osm", outFile);

    HOOT_FILE_EQUALS(_inputPath + "building-3441-addresses-2-out.osm", outFile);
  }

  void _poiPolygon10Test()
  {
    LOG_DEBUG("_poiPolygon10Test");

    resetTest();
    conf().set(
      ConfigOptions::getMatchCreatorsKey(),
      "BuildingMatchCreator;PoiPolygonMatchCreator");
    conf().set(
      ConfigOptions::getMergerCreatorsKey(),
      "BuildingMergerCreator;PoiPolygonMergerCreator");
    conf().set(ConfigOptions::getPoiPolygonMatchEvidenceThresholdKey(), 2);
    conf().set(ConfigOptions::getPoiPolygonPromotePointsWithAddressesToPoisKey(), true);

    const QString outFile = _outputPath + "poi-polygon-10-out.osm";
    ConflateExecutor().conflate(
      _inputPath + "poi-polygon-10-in-1.osm", _inputPath + "poi-polygon-10-in-2.osm", outFile);

    HOOT_FILE_EQUALS(_inputPath + "poi-polygon-10-out.osm", outFile);
  }

  void _poiPolygonAutoMerge13Test()
  {
    LOG_DEBUG("_poiPolygonAutoMerge13Test");

    resetTest();
    conf().set(
      ConfigOptions::getMatchCreatorsKey(),
      "BuildingMatchCreator;PoiPolygonMatchCreator");
    conf().set(
      ConfigOptions::getMergerCreatorsKey(),
      "BuildingMergerCreator;PoiPolygonMergerCreator");

    const QString outFile = _outputPath + "poi-polygon-auto-merge-13-out.osm";
    ConflateExecutor().conflate(
      _inputPath + "poi-polygon-auto-merge-13-in-1.osm",
      _inputPath + "poi-polygon-auto-merge-13-in-2.osm", outFile);

    HOOT_FILE_EQUALS(_inputPath + "poi-polygon-auto-merge-13-out.osm", outFile);
  }

  void _poiPolygonAutoMerge14Test()
  {
    LOG_DEBUG("_poiPolygonAutoMerge14Test");

    resetTest();
    conf().set(ConfigOptions::getMatchCreatorsKey(), "BuildingMatchCreator;PoiPolygonMatchCreator");
    conf().set(
      ConfigOptions::getMergerCreatorsKey(), "BuildingMergerCreator;PoiPolygonMergerCreator");
    conf().set(ConfigOptions::getPoiPolygonAutoMergeManyPoiToOnePolyMatchesKey(), true);

    const QString outFile = _outputPath + "poi-polygon-auto-merge-14-out.osm";
    ConflateExecutor().conflate(
      _inputPath + "poi-polygon-auto-merge-14-in-1.osm",
      _inputPath + "poi-polygon-auto-merge-14-in-2.osm", outFile);

    HOOT_FILE_EQUALS(_inputPath + "poi-polygon-auto-merge-14-out.osm", outFile);
  }

  void _poiPolygonRecursiveWayAddress3267_1Test()
  {
    LOG_DEBUG("_poiPolygonRecursiveWayAddress3267_1Test");

    resetTest();
    conf().set(ConfigOptions::getMatchCreatorsKey(), "PoiPolygonMatchCreator");
    conf().set(ConfigOptions::getMergerCreatorsKey(), "PoiPolygonMergerCreator");

    const QString outFile = _outputPath + "poi-polygon-recursive-way-address-3267-1-out.osm";
    ConflateExecutor().conflate(
      _inputPath + "poi-polygon-recursive-way-address-3267-1-in-1.osm",
      _inputPath + "poi-polygon-recursive-way-address-3267-1-in-2.osm", outFile);

    HOOT_FILE_EQUALS(_inputPath + "poi-polygon-recursive-way-address-3267-1-out.osm", outFile);
  }

  void _poiPolygonReviewConflict4331_3Test()
  {
    LOG_DEBUG("_poiPolygonReviewConflict4331_3Test");

    resetTest();
    conf().set(
      ConfigOptions::getMatchCreatorsKey(),
      "BuildingMatchCreator;PoiPolygonMatchCreator;ScriptMatchCreator,Poi.js");
    conf().set(
      ConfigOptions::getMergerCreatorsKey(),
      "BuildingMergerCreator;PoiPolygonMergerCreator;ScriptMergerCreator");
    conf().set(ConfigOptions::getPoiPolygonNameStringComparerKey(), "KskipBigramDistance");

    const QString outFile = _outputPath + "poi-polygon-review-conflict-4331-3-out.osm";
    ConflateExecutor().conflate(
      _inputPath + "poi-polygon-review-conflict-4331-3-in-1.osm",
      _inputPath + "poi-polygon-review-conflict-4331-3-in-2.osm", outFile);

    HOOT_FILE_EQUALS(_inputPath + "poi-polygon-review-conflict-4331-3-out.osm", outFile);
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(AddressConflateTest, "slow");

}
