

// Hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/conflate/poi-polygon/extractors/PoiPolygonPhoneNumberScoreExtractor.h>
#include <hoot/core/util/Log.h>

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

//#include <phonenumbers/phonenumberutil.h>
//#include <phonenumbers/geocoding/phonenumber_offline_geocoder.h>
//using namespace i18n::phonenumbers;

namespace hoot
{

class PoiPolygonPhoneNumberScoreExtractorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(PoiPolygonPhoneNumberScoreExtractorTest);
  CPPUNIT_TEST(basicTest);
  CPPUNIT_TEST(regionValidationTest);
  CPPUNIT_TEST(invalidNumberTest);
  CPPUNIT_TEST(tagKeyTest);
  //CPPUNIT_TEST(tagNumberParse);
  CPPUNIT_TEST_SUITE_END();

public:

  void basicTest()
  {
    PoiPolygonPhoneNumberScoreExtractor uut;
    OsmMapPtr map(new OsmMap());

    NodePtr node1(new Node(Status::Unknown1, -1, geos::geom::Coordinate(0.0, 0.0), 15.0));
    WayPtr way1(new Way(Status::Unknown2, -1, 15.0));

    node1->getTags().set("phone", "(123) 456 7890");
    way1->getTags().set("phone", "123 456 7890");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node1, way1), 0.0);
  }

  void regionValidationTest()
  {
    PoiPolygonPhoneNumberScoreExtractor uut;
    OsmMapPtr map(new OsmMap());

    NodePtr node1(new Node(Status::Unknown1, -1, geos::geom::Coordinate(0.0, 0.0), 15.0));
    WayPtr way1(new Way(Status::Unknown2, -1, 15.0));

    node1->getTags().set("phone", "(123) 456 7890");
    way1->getTags().set("phone", "123 456 7890");

    uut.setRegionCode("US");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node1, way1), 0.0);

    uut.setRegionCode("CH");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, uut.extract(*map, node1, way1), 0.0);

    node1->getTags().set("phone", "+41 44 668 1800");
    way1->getTags().set("phone", "044 668 1800");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node1, way1), 0.0);
  }

  void invalidNumberTest()
  {
    PoiPolygonPhoneNumberScoreExtractor uut;
    OsmMapPtr map(new OsmMap());

    NodePtr node1(new Node(Status::Unknown1, -1, geos::geom::Coordinate(0.0, 0.0), 15.0));
    WayPtr way1(new Way(Status::Unknown2, -1, 15.0));

    node1->getTags().set("phone", "blah");
    way1->getTags().set("phone", "123 456 7890");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, uut.extract(*map, node1, way1), 0.0);
  }

  void tagKeyTest()
  {
    PoiPolygonPhoneNumberScoreExtractor uut;
    OsmMapPtr map(new OsmMap());

    NodePtr node1(new Node(Status::Unknown1, -1,geos::geom:: Coordinate(0.0, 0.0), 15.0));
    WayPtr way1(new Way(Status::Unknown2, -1, 15.0));

    node1->getTags().set("source:phone", "(123) 456 7890");
    way1->getTags().set("phone", "123 456 7890");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node1, way1), 0.0);
  }

//  void tagNumberParse()
//  {
//    PhoneNumber number1;
//    PhoneNumberUtil::ErrorType parseError =
//      PhoneNumberUtil::GetInstance()->Parse("(123) 456 7890", "US", &number1);
//    LOG_VARW(parseError);
//    parseError = PhoneNumberUtil::GetInstance()->Parse("(123) 456 7890", "", &number1);
//    LOG_VARW(parseError);
//    parseError = PhoneNumberUtil::GetInstance()->Parse("+41 44 668 1800", "CH", &number1);
//    LOG_VARW(parseError);
//    parseError = PhoneNumberUtil::GetInstance()->Parse("+41 44 668 1800", "", &number1);
//    LOG_VARW(parseError);

//    LOG_VARW(PhoneNumberUtil::GetInstance()->IsPossibleNumberForString("(123) 456 7890", "US"));
//    LOG_VARW(PhoneNumberUtil::GetInstance()->IsPossibleNumberForString("(123) 456 7890", ""));
//    LOG_VARW(PhoneNumberUtil::GetInstance()->IsPossibleNumberForString("+41 44 668 1800", "CH"));
//    LOG_VARW(PhoneNumberUtil::GetInstance()->IsPossibleNumberForString("+41 44 668 1800", ""));

//    PhoneNumber number2;
//    parseError = PhoneNumberUtil::GetInstance()->Parse("(123) 456 7890", "US", &number2);
//    LOG_VARW(parseError);
//    LOG_VARW(PhoneNumberUtil::GetInstance()->IsValidNumber(number2));
//    LOG_VARW(PhoneNumberUtil::GetInstance()->IsValidNumberForRegion(number2, "US"));
//    LOG_VARW(PhoneNumberUtil::GetInstance()->IsValidNumberForRegion(number2, "CH"));

//    PhoneNumberOfflineGeocoder geocoder;
//    PhoneNumber number3;
//    parseError = PhoneNumberUtil::GetInstance()->Parse("+41 44 668 1800", "CH", &number3);
//    LOG_VARW(geocoder.GetDescriptionForValidNumber(number3, icu::Locale::getUS()));
//  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(PoiPolygonPhoneNumberScoreExtractorTest, "quick");

}
