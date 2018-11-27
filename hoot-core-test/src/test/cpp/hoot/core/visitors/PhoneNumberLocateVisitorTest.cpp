
// hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/rnd/visitors/PhoneNumberLocateVisitor.h>

namespace hoot
{

static const QString outputRoot = "test-output/visitors/PhoneNumberLocateVisitorTest";

class PhoneNumberLocateVisitorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(PhoneNumberLocateVisitorTest);
  CPPUNIT_TEST(runBasicTest);
  CPPUNIT_TEST_SUITE_END();

public:

  PhoneNumberLocateVisitorTest()
  {
    setResetType(ResetBasic);
    TestUtils::mkpath(outputRoot);
  }

  void runBasicTest()
  {
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map,
      "test-files/cmd/glacial/PoiPolygonConflateStandaloneTest/PoiPolygon2.osm",
      false,
      Status::Unknown1);

    PhoneNumberLocateVisitor uut;
    uut._phoneNumberLocator.setRegionCode("US");
    map->visitRw(uut);

    const QString outputFile = outputRoot + "/out.osm";
    OsmMapWriterFactory::getInstance().write(map, outputFile);

    CPPUNIT_ASSERT_EQUAL(12, uut._phoneNumberLocator.getNumLocated());
    HOOT_FILE_EQUALS("test-files/visitors/PhoneNumberLocateVisitorTest/gold.osm", outputFile);
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(PhoneNumberLocateVisitorTest, "quick");

}


