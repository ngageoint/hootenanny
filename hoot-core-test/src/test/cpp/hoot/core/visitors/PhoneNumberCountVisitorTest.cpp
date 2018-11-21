
// hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/visitors/PhoneNumberCountVisitor.h>

namespace hoot
{

class PhoneNumberCountVisitorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(PhoneNumberCountVisitorTest);
  CPPUNIT_TEST(runBasicTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runBasicTest()
  {
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map,
      "test-files/cmd/glacial/PoiPolygonConflateStandaloneTest/PoiPolygon1.osm",
      false,
      Status::Unknown1);

    PhoneNumberCountVisitor uut;
    map->visitRo(uut);
    CPPUNIT_ASSERT_EQUAL(1, (int)uut.getStat());
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(PhoneNumberCountVisitorTest, "quick");

}


