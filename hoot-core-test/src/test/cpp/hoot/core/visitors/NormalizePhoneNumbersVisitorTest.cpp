
// hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/visitors/NormalizePhoneNumbersVisitor.h>

namespace hoot
{

class NormalizePhoneNumbersVisitorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(NormalizePhoneNumbersVisitorTest);
  CPPUNIT_TEST(runBasicTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runBasicTest()
  {
    OsmMapPtr map(new OsmMap());
    //TODO: make sure this data has elements with more than one phone number
    OsmMapReaderFactory::read(
      map,
      "test-files/cmd/glacial/PoiPolygonConflateStandaloneTest/PoiPolygon1.osm",
      false,
      Status::Unknown1);

    NormalizePhoneNumbersVisitor uut;
    map->visitRw(uut);

    const QString outputFile = "test-output/cmd/glacial/NormalizePhoneNumbersVisitorTest/out.osm";
    OsmMapWriterFactory::getInstance().write(map, outputFile);

    HOOT_FILE_EQUALS("test-files/cmd/quick/NormalizePhoneNumbersVisitorTest/gold.osm", outputFile);
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(NormalizePhoneNumbersVisitorTest, "quick");

}


