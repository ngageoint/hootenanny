

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

namespace hoot
{

class PoiPolygonPhoneNumberScoreExtractorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(PoiPolygonPhoneNumberScoreExtractorTest);
  CPPUNIT_TEST(basicTest);
  CPPUNIT_TEST(regionValidationTest);
  CPPUNIT_TEST(invalidNumberTest);
  CPPUNIT_TEST(tagKeyTest);
  CPPUNIT_TEST(invalidRegionCodeTest);
  CPPUNIT_TEST(additionalTagKeysTest);
  CPPUNIT_TEST(findInTextTest);
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

  void invalidRegionCodeTest()
  {
    PoiPolygonPhoneNumberScoreExtractor uut;
    QString exceptionMsg("");
    try
    {
      uut.setRegionCode("blah");
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT(exceptionMsg.startsWith("Invalid phone number region code"));
  }

  void additionalTagKeysTest()
  {
    PoiPolygonPhoneNumberScoreExtractor uut;
    OsmMapPtr map(new OsmMap());

    NodePtr node1(new Node(Status::Unknown1, -1, geos::geom::Coordinate(0.0, 0.0), 15.0));
    WayPtr way1(new Way(Status::Unknown2, -1, 15.0));

    node1->getTags().set("note", "(123) 456 7890");
    way1->getTags().set("phone", "123 456 7890");

    uut.setAdditionalTagKeys(QStringList("note"));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node1, way1), 0.0);

    uut.setAdditionalTagKeys(QStringList());
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, uut.extract(*map, node1, way1), 0.0);
  }

  void findInTextTest()
  {
    PoiPolygonPhoneNumberScoreExtractor uut;
    OsmMapPtr map(new OsmMap());

    NodePtr node1(new Node(Status::Unknown1, -1, geos::geom::Coordinate(0.0, 0.0), 15.0));
    WayPtr way1(new Way(Status::Unknown2, -1, 15.0));

    node1->getTags().set(
      "phone",
      "this is some text with a phone number: (123) 456 7890; and here's another one: 123 456 7890");
    way1->getTags().set("phone", "123 456 7890");

    uut.setRegionCode("US");
    uut.setSearchInText(true);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node1, way1), 0.0);

    uut.setSearchInText(false);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, uut.extract(*map, node1, way1), 0.0);
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(PoiPolygonPhoneNumberScoreExtractorTest, "quick");

}
