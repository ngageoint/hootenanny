
// hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/visitors/NormalizeAddressesVisitor.h>

namespace hoot
{

class NormalizeAddressesVisitorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(NormalizeAddressesVisitorTest);
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

    NormalizeAddressesVisitor uut;
    map->visitRw(uut);

    const QString outputFile = "test-output/cmd/glacial/NormalizeAddressesVisitorTest/out.osm";
    OsmMapWriterFactory::getInstance().write(map, outputFile);

    HOOT_FILE_EQUALS("test-files/cmd/slow/NormalizeAddressesVisitorTest/gold.osm", outputFile);
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(NormalizeAddressesVisitorTest, "slow");

}


