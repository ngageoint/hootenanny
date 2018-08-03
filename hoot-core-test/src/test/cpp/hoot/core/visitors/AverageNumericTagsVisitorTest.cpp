
// hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/visitors/AverageNumericTagsVisitor.h>

namespace hoot
{

class SumNumericTagsVisitorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(AverageNumericTagsVisitor);
  CPPUNIT_TEST(runBasicTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runBasicTest()
  {
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map,
      "test-files/visitors/SumNumericTagsVisitorTest.osm",
      false,
      Status::Unknown1);

    AverageNumericTagsVisitor uut("test1");
    map->visitRo(uut);
    const double avg = uut.getStat();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.667, avg, 1e-3);
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(AverageNumericTagsVisitor, "quick");

}


