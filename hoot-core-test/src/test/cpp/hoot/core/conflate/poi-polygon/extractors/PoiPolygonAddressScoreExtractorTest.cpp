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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/conflate/poi-polygon/extractors/PoiPolygonAddressScoreExtractor.h>
#include <hoot/core/language/DictionaryTranslator.h>
#include <hoot/core/conflate/poi-polygon/extractors/LocalAddressValidationDataSource.h>

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// libaddressinput
#include <libaddressinput/address_data.h>
#include <libaddressinput/address_formatter.h>
#include <libaddressinput/address_normalizer.h>
#include <libaddressinput/address_validator.h>
#include <libaddressinput/preload_supplier.h>
#include <libaddressinput/null_storage.h>
#include <libaddressinput/callback.h>
#include <libaddressinput/address_field.h>
#include <libaddressinput/address_problem.h>

using namespace geos::geom;
using namespace i18n::addressinput;

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
  CPPUNIT_TEST(addressFormatTest);
  CPPUNIT_TEST(addressNormalizationTest);
  CPPUNIT_TEST(addressValidation1Test);
  CPPUNIT_TEST(addressValidation2Test);
  CPPUNIT_TEST(addressValidation3Test);
  CPPUNIT_TEST_SUITE_END();

public:

  void runTagTest()
  {
    PoiPolygonAddressScoreExtractor uut;
    uut.setConfiguration(conf());
    OsmMapPtr map(new OsmMap());

    NodePtr node1(new Node(Status::Unknown1, -1, Coordinate(0.0, 0.0), 15.0));
    node1->getTags().set(PoiPolygonAddressScoreExtractor::FULL_ADDRESS_TAG_NAME, "123 Main Street");
    map->addNode(node1);
    WayPtr way1(new Way(Status::Unknown2, -1, 15.0));
    way1->getTags().set(PoiPolygonAddressScoreExtractor::FULL_ADDRESS_TAG_NAME, "123 main street");
    map->addWay(way1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node1, way1), 0.0);

    NodePtr node2(new Node(Status::Unknown1, -2, Coordinate(0.0, 0.0), 15.0));
    node2->getTags().set(PoiPolygonAddressScoreExtractor::FULL_ADDRESS_TAG_NAME, "123 Main Street");
    map->addNode(node2);
    WayPtr way2(new Way(Status::Unknown2, -2, 15.0));
    way2->getTags().set(PoiPolygonAddressScoreExtractor::FULL_ADDRESS_TAG_NAME, "567 first street");
    map->addWay(way2);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, uut.extract(*map, node2, way2), 0.01);
  }

  void runCombinedTagTest()
  {
    PoiPolygonAddressScoreExtractor uut;
    uut.setConfiguration(conf());
    OsmMapPtr map(new OsmMap());

    NodePtr node1(new Node(Status::Unknown1, -1, Coordinate(0.0, 0.0), 15.0));
    node1->getTags().set(PoiPolygonAddressScoreExtractor::HOUSE_NUMBER_TAG_NAME, "123");
    node1->getTags().set(PoiPolygonAddressScoreExtractor::STREET_TAG_NAME, "Main Street");
    map->addNode(node1);
    WayPtr way1(new Way(Status::Unknown2, -1, 15.0));
    way1->getTags().set(PoiPolygonAddressScoreExtractor::HOUSE_NUMBER_TAG_NAME, "123");
    way1->getTags().set(PoiPolygonAddressScoreExtractor::STREET_TAG_NAME, "main street");
    map->addWay(way1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node1, way1), 0.0);

    NodePtr node2(new Node(Status::Unknown1, -2, Coordinate(0.0, 0.0), 15.0));
    node2->getTags().set(PoiPolygonAddressScoreExtractor::HOUSE_NUMBER_TAG_NAME, "123");
    node2->getTags().set(PoiPolygonAddressScoreExtractor::STREET_TAG_NAME, "Main Street");
    map->addNode(node2);
    WayPtr way2(new Way(Status::Unknown2, -2, 15.0));
    way2->getTags().set(PoiPolygonAddressScoreExtractor::HOUSE_NUMBER_TAG_NAME, "567");
    way2->getTags().set(PoiPolygonAddressScoreExtractor::STREET_TAG_NAME, "first street");
    map->addWay(way2);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, uut.extract(*map, node2, way2), 0.01);
  }

  void runRangeTest()
  {
    PoiPolygonAddressScoreExtractor uut;
    uut.setConfiguration(conf());
    OsmMapPtr map(new OsmMap());

    NodePtr node1(new Node(Status::Unknown1, -1, Coordinate(0.0, 0.0), 15.0));
    node1->getTags().set(PoiPolygonAddressScoreExtractor::HOUSE_NUMBER_TAG_NAME, "123-125");
    node1->getTags().set(PoiPolygonAddressScoreExtractor::STREET_TAG_NAME, "Main Street");
    map->addNode(node1);
    WayPtr way1(new Way(Status::Unknown2, -1, 15.0));
    way1->getTags().set(PoiPolygonAddressScoreExtractor::HOUSE_NUMBER_TAG_NAME, "124");
    way1->getTags().set(PoiPolygonAddressScoreExtractor::STREET_TAG_NAME, "main street");
    map->addWay(way1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node1, way1), 0.0);

    NodePtr node2(new Node(Status::Unknown1, -2, Coordinate(0.0, 0.0), 15.0));
    node2->getTags().set(PoiPolygonAddressScoreExtractor::HOUSE_NUMBER_TAG_NAME, "123-125");
    node2->getTags().set(PoiPolygonAddressScoreExtractor::STREET_TAG_NAME, "Main Street");
    map->addNode(node2);
    WayPtr way2(new Way(Status::Unknown2, -2, 15.0));
    way2->getTags().set(PoiPolygonAddressScoreExtractor::HOUSE_NUMBER_TAG_NAME, "123");
    way2->getTags().set(PoiPolygonAddressScoreExtractor::STREET_TAG_NAME, "main street");
    map->addWay(way2);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node2, way2), 0.0);

    NodePtr node3(new Node(Status::Unknown1, -3, Coordinate(0.0, 0.0), 15.0));
    node3->getTags().set(PoiPolygonAddressScoreExtractor::HOUSE_NUMBER_TAG_NAME, "123-125");
    node3->getTags().set(PoiPolygonAddressScoreExtractor::STREET_TAG_NAME, "Main Street");
    map->addNode(node3);
    WayPtr way3(new Way(Status::Unknown2, -3, 15.0));
    way3->getTags().set(PoiPolygonAddressScoreExtractor::HOUSE_NUMBER_TAG_NAME, "567");
    way3->getTags().set(PoiPolygonAddressScoreExtractor::STREET_TAG_NAME, "first street");
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
      PoiPolygonAddressScoreExtractor::FULL_ADDRESS_TAG_NAME_2, "Main Street 123 20121 mytown");
    map->addNode(node1);
    WayPtr way1(new Way(Status::Unknown2, -1, 15.0));
    way1->getTags().set(PoiPolygonAddressScoreExtractor::FULL_ADDRESS_TAG_NAME_2, "main street 123");
    map->addWay(way1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node1, way1), 0.0);

    NodePtr node2(new Node(Status::Unknown1, -2, Coordinate(0.0, 0.0), 15.0));
    node2->getTags().set(
      PoiPolygonAddressScoreExtractor::FULL_ADDRESS_TAG_NAME_2, "Main Street 123 20121 mytown");
    map->addNode(node2);
    WayPtr way2(new Way(Status::Unknown2, -2, 15.0));
    way2->getTags().set(
      PoiPolygonAddressScoreExtractor::FULL_ADDRESS_TAG_NAME_2, "first street 567");
    map->addWay(way2);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, uut.extract(*map, node2, way2), 0.01);
  }

  void runSubLetterTest()
  {
    PoiPolygonAddressScoreExtractor uut;
    uut.setConfiguration(conf());
    OsmMapPtr map(new OsmMap());

    NodePtr node1(new Node(Status::Unknown1, -1, Coordinate(0.0, 0.0), 15.0));
    node1->getTags().set(PoiPolygonAddressScoreExtractor::HOUSE_NUMBER_TAG_NAME, "123");
    node1->getTags().set(PoiPolygonAddressScoreExtractor::STREET_TAG_NAME, "Main Street");
    map->addNode(node1);
    WayPtr way1(new Way(Status::Unknown2, -1, 15.0));
    way1->getTags().set(PoiPolygonAddressScoreExtractor::HOUSE_NUMBER_TAG_NAME, "123a");
    way1->getTags().set(PoiPolygonAddressScoreExtractor::STREET_TAG_NAME, "main street");
    map->addWay(way1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node1, way1), 0.0);
  }

  void runWayTest()
  {
    PoiPolygonAddressScoreExtractor uut;
    uut.setConfiguration(conf());
    OsmMapPtr map(new OsmMap());

    NodePtr node1(new Node(Status::Unknown1, -1, Coordinate(0.0, 0.0), 15.0));
    node1->getTags().set(PoiPolygonAddressScoreExtractor::HOUSE_NUMBER_TAG_NAME, "123");
    node1->getTags().set(PoiPolygonAddressScoreExtractor::STREET_TAG_NAME, "Main Street");
    map->addNode(node1);
    WayPtr way1(new Way(Status::Unknown2, -1, 15.0));
    NodePtr node2(new Node(Status::Unknown1, -2, Coordinate(0.0, 0.0), 15.0));
    node2->getTags().set(PoiPolygonAddressScoreExtractor::HOUSE_NUMBER_TAG_NAME, "123");
    node2->getTags().set(PoiPolygonAddressScoreExtractor::STREET_TAG_NAME, "main street");
    map->addNode(node2);
    way1->addNode(node2->getId());
    map->addWay(way1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node1, way1), 0.0);

    NodePtr node3(new Node(Status::Unknown1, -3, Coordinate(0.0, 0.0), 15.0));
    node3->getTags().set(PoiPolygonAddressScoreExtractor::HOUSE_NUMBER_TAG_NAME, "123");
    node3->getTags().set(PoiPolygonAddressScoreExtractor::STREET_TAG_NAME, "Main Street");
    map->addNode(node3);
    WayPtr way2(new Way(Status::Unknown2, -2, 15.0));
    NodePtr node4(new Node(Status::Unknown2, -4, Coordinate(0.0, 0.0), 15.0));
    node4->getTags().set(PoiPolygonAddressScoreExtractor::HOUSE_NUMBER_TAG_NAME, "567");
    node4->getTags().set(PoiPolygonAddressScoreExtractor::STREET_TAG_NAME, "first street");
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
    node1->getTags().set(PoiPolygonAddressScoreExtractor::HOUSE_NUMBER_TAG_NAME, "123");
    node1->getTags().set(PoiPolygonAddressScoreExtractor::STREET_TAG_NAME, "Main Street");
    map->addNode(node1);
    RelationPtr relation1(new Relation(Status::Unknown2, -1, 15.0));
    NodePtr node2(new Node(Status::Unknown1, -2, Coordinate(0.0, 0.0), 15.0));
    node2->getTags().set(PoiPolygonAddressScoreExtractor::HOUSE_NUMBER_TAG_NAME, "123");
    node2->getTags().set(PoiPolygonAddressScoreExtractor::STREET_TAG_NAME, "main street");
    map->addNode(node2);
    relation1->addElement("test", node2->getElementId());
    map->addRelation(relation1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node1, relation1), 0.0);

    NodePtr node4(new Node(Status::Unknown1, -4, Coordinate(0.0, 0.0), 15.0));
    node4->getTags().set(PoiPolygonAddressScoreExtractor::HOUSE_NUMBER_TAG_NAME, "123");
    node4->getTags().set(PoiPolygonAddressScoreExtractor::STREET_TAG_NAME, "Main Street");
    map->addNode(node4);
    RelationPtr relation3(new Relation(Status::Unknown2, -3, 15.0));
    NodePtr node5(new Node(Status::Unknown1, -5, Coordinate(0.0, 0.0), 15.0));
    node5->getTags().set(PoiPolygonAddressScoreExtractor::HOUSE_NUMBER_TAG_NAME, "567");
    node5->getTags().set(PoiPolygonAddressScoreExtractor::STREET_TAG_NAME, "first street");
    map->addNode(node5);
    relation3->addElement("test", node5->getElementId());
    map->addRelation(relation3);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, uut.extract(*map, node4, relation3), 0.01);

    NodePtr node6(new Node(Status::Unknown1, -6, Coordinate(0.0, 0.0), 15.0));
    node6->getTags().set(PoiPolygonAddressScoreExtractor::HOUSE_NUMBER_TAG_NAME, "567");
    node6->getTags().set(PoiPolygonAddressScoreExtractor::STREET_TAG_NAME, "first street");
    map->addNode(node6);
    RelationPtr relation4(new Relation(Status::Unknown2, -4, 15.0));
    WayPtr way3(new Way(Status::Unknown2, -3, 15.0));
    way3->getTags().set(PoiPolygonAddressScoreExtractor::HOUSE_NUMBER_TAG_NAME, "123");
    way3->getTags().set(PoiPolygonAddressScoreExtractor::STREET_TAG_NAME, "main street");
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

    settings.set("poi.polygon.translate.addresses.to.english", "true");
    settings.set("language.translation.translator", "hoot::DictionaryTranslator");
    uut.setConfiguration(settings);
    boost::shared_ptr<DictionaryTranslator> dictTranslator =
      boost::dynamic_pointer_cast<DictionaryTranslator>(
        PoiPolygonAddressScoreExtractor::_translator);
    dictTranslator->setTokenizeInput(false);

    NodePtr node1(new Node(Status::Unknown1, -1, Coordinate(0.0, 0.0), 15.0));
    node1->getTags().set(PoiPolygonAddressScoreExtractor::FULL_ADDRESS_TAG_NAME, "123 Main Street");
    map->addNode(node1);
    WayPtr way1(new Way(Status::Unknown2, -1, 15.0));
    way1->getTags().set(
      PoiPolygonAddressScoreExtractor::FULL_ADDRESS_TAG_NAME, QString::fromUtf8("123 Hauptstraße"));
    map->addWay(way1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node1, way1), 0.0);

    NodePtr node2(new Node(Status::Unknown1, -1, Coordinate(0.0, 0.0), 15.0));
    node2->getTags().set(
      PoiPolygonAddressScoreExtractor::FULL_ADDRESS_TAG_NAME_2, "Central Border Street 40 81379");
    map->addNode(node2);
    WayPtr way2(new Way(Status::Unknown2, -1, 15.0));
    way2->getTags().set(
      PoiPolygonAddressScoreExtractor::FULL_ADDRESS_TAG_NAME_2,
      QString::fromUtf8("ZENTRALLÄNDE STRASSE 40 81379 MÜNCHEN"));
    map->addWay(way2);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node2, way2), 0.0);

    settings.set("poi.polygon.translate.addresses.to.english", "false");
    uut.setConfiguration(settings);
     CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, uut.extract(*map, node1, way1), 0.0);
  }

  void addressFormatTest()
  {
    std::string result;

    AddressData address1;
    address1.region_code = "US";
    address1.administrative_area = "Virginia";
    address1.locality = "Centreville";
    address1.postal_code = "20121";
    address1.address_line.push_back("13937 Winding Ridge Lane");
    GetStreetAddressLinesAsSingleLine(address1, &result);
    HOOT_STR_EQUALS("13937 Winding Ridge Lane", result);

    AddressData address2;
    address2.region_code = "DE";
    address2.administrative_area = "";
    address2.locality = "MÜNCHEN";
    address2.postal_code = "81379";
    address2.address_line.push_back("ZENTRALLÄNDE STRASSE 40");
    GetStreetAddressLinesAsSingleLine(address2, &result);
    HOOT_STR_EQUALS("ZENTRALLÄNDE STRASSE 40", result);

    GetFormattedNationalAddressLine(address1, &result);
    LOG_VART(result);
    GetFormattedNationalAddressLine(address2, &result);
    LOG_VART(result);

    std::vector<std::string> lines;
    GetFormattedNationalAddress(address2, &lines);
    LOG_VART(lines);
    CPPUNIT_ASSERT_EQUAL(2, (int)lines.size());
  }

  void addressNormalizationTest()
  {
    PreloadSupplier supplier(new LocalAddressValidationDataSource(true), new NullStorage());
    const std::unique_ptr<const PreloadSupplier::Callback> loaded(
      BuildCallback(this, &PoiPolygonAddressScoreExtractorTest::_onAddressDataLoaded));
    const AddressNormalizer normalizer(&supplier);
    supplier.LoadRules("US", *loaded);

    AddressData address;
    address.language_code = "en-US";
    address.region_code = "US";
    address.administrative_area = "California";
    address.locality = "Mountain View";
    normalizer.Normalize(&address);
    HOOT_STR_EQUALS("CA", address.administrative_area);
  }

  void addressValidation1Test()
  {
    PreloadSupplier supplier(new LocalAddressValidationDataSource(true), new NullStorage());
    const std::unique_ptr<const PreloadSupplier::Callback> loaded(
      BuildCallback(this, &PoiPolygonAddressScoreExtractorTest::_onAddressDataLoaded));
    const std::unique_ptr<const AddressValidator::Callback> validated(
      BuildCallback(this, &PoiPolygonAddressScoreExtractorTest::_onAddressValidated1));
    const AddressValidator validator(&supplier);
    supplier.LoadRules("US", *loaded);

    AddressData address1;
    address1.region_code = "US";
    address1.administrative_area = "Virginia";
    address1.locality = "Centreville";
    address1.postal_code = "20121";
    address1.address_line.push_back("13937 Winding Ridge Lane");

    FieldProblemMap filter;
    FieldProblemMap problems;
    validator.Validate(
      address1,
      false,
      false,
      &filter,
      &problems,
      *validated);
  }

  void addressValidation2Test()
  {
    PreloadSupplier supplier(new LocalAddressValidationDataSource(true), new NullStorage());
    const std::unique_ptr<const PreloadSupplier::Callback> loaded(
      BuildCallback(this, &PoiPolygonAddressScoreExtractorTest::_onAddressDataLoaded));
    const std::unique_ptr<const AddressValidator::Callback> validated(
      BuildCallback(this, &PoiPolygonAddressScoreExtractorTest::_onAddressValidated2));
    const AddressValidator validator(&supplier);
    supplier.LoadRules("US", *loaded);

    AddressData address1;
    address1.region_code = "US";
    address1.administrative_area = "blah";
    address1.locality = "Centreville";
    address1.postal_code = "2343255";
    address1.address_line.push_back("13937 Winding Ridge");

    FieldProblemMap filter;
    FieldProblemMap problems;
    validator.Validate(
      address1,
      false,
      false,
      &filter,
      &problems,
      *validated);
  }

  void addressValidation3Test()
  {
    PreloadSupplier supplier(new LocalAddressValidationDataSource(true), new NullStorage());
    const std::unique_ptr<const PreloadSupplier::Callback> loaded(
      BuildCallback(this, &PoiPolygonAddressScoreExtractorTest::_onAddressDataLoaded));
    const std::unique_ptr<const AddressValidator::Callback> validated(
      BuildCallback(this, &PoiPolygonAddressScoreExtractorTest::_onAddressValidated3));
    const AddressValidator validator(&supplier);
    supplier.LoadRules("US", *loaded);

    AddressData address1;
    address1.region_code = "US";
    address1.address_line.push_back("13937 Winding Ridge");

    FieldProblemMap filter;
    FieldProblemMap problems;
    validator.Validate(
      address1,
      false,
      false,
      &filter,
      &problems,
      *validated);
  }

private:

  void _onAddressDataLoaded(bool success, const std::string& region_code, int /*num_rules*/)
  {
    CPPUNIT_ASSERT(success);
    HOOT_STR_EQUALS("US", region_code);
  }

  void _onAddressValidated1(bool success, const AddressData& address,
                            const FieldProblemMap& problems)
  {
    CPPUNIT_ASSERT(success);
    HOOT_STR_EQUALS("US", address.region_code);
    CPPUNIT_ASSERT_EQUAL(0, (int)problems.size());
  }

  void _onAddressValidated2(bool success, const AddressData& address,
                            const FieldProblemMap& problems)
  {
    CPPUNIT_ASSERT(success);
    HOOT_STR_EQUALS("US", address.region_code);
    LOG_VART(problems);
    CPPUNIT_ASSERT_EQUAL(2, (int)problems.size());
  }

  void _onAddressValidated3(bool success, const AddressData& address,
                            const FieldProblemMap& problems)
  {
    CPPUNIT_ASSERT(success);
    HOOT_STR_EQUALS("US", address.region_code);
    LOG_VART(problems);
    CPPUNIT_ASSERT_EQUAL(3, (int)problems.size());
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(PoiPolygonAddressScoreExtractorTest, "quick");

}
