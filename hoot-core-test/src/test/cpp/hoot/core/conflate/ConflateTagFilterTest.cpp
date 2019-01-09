
// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/conflate/Conflator.h>
#include <hoot/core/elements/Element.h>
#include <hoot/core/io/OsmPbfReader.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/util/MapProjector.h>

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

namespace hoot
{

class ConflateTagFilterTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(ConflateTagFilterTest);
  //TODO
  //CPPUNIT_TEST(runBasicTest);
  CPPUNIT_TEST_SUITE_END();

public:

  ConflateTagFilterTest()
  {
    TestUtils::mkpath("test-output");
  }

  void runBasicTest()
  {
//    OsmPbfReader reader(true);

//    OsmMapPtr map(new OsmMap());
//    reader.setUseFileStatus(true);
//    reader.read("test-files/ToyTestCombined.pbf", map);

//    Conflator uut;
//    uut.loadSource(map);
//    uut.conflate();

//    OsmMapPtr out(new OsmMap(uut.getBestMap()));
//    MapProjector::projectToWgs84(out);

//    OsmXmlWriter writer;
//    writer.setIncludeIds(true);
//    writer.write(out, "test-output/ConflatorPbfTest.osm");

//    CPPUNIT_ASSERT_EQUAL((size_t)15, out->getWays().size());
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ConflateTagFilterTest, "quick");

}
