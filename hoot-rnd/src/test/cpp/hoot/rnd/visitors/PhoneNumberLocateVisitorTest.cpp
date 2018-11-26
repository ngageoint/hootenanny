
// hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/rnd/visitors/PhoneNumberLocateVisitor.h>

namespace hoot
{

class PhoneNumberLocateVisitorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(PhoneNumberLocateVisitorTest);
  //TODO: add test for non-US region code
  CPPUNIT_TEST(runBasicTest);
  CPPUNIT_TEST_SUITE_END();

public:

  PhoneNumberLocateVisitorTest()
  {
  }

  void runBasicTest()
  {
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map,
      "test-files/cmd/glacial/PoiPolygonConflateStandaloneTest/PoiPolygon1.osm",
      false,
      Status::Unknown1);

    PhoneNumberLocateVisitor uut;
    map->visitRw(uut);

    const QString outputFile = "test-output/cmd/glacial/PhoneNumberLocateVisitorTest/out.osm";
    OsmMapWriterFactory::getInstance().write(map, outputFile);

    HOOT_FILE_EQUALS("test-files/cmd/slow/PhoneNumberLocateVisitorTest/gold.osm", outputFile);
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(PhoneNumberLocateVisitorTest, "quick");

}


