
// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/ops/RelationWithMostMembersOp.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/MapProjector.h>

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

namespace hoot
{

class RelationWithMostMembersOpTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(RelationWithMostMembersOpTest);
  CPPUNIT_TEST(runBasicTest);
  CPPUNIT_TEST_SUITE_END();

public:

  const QString inputPath = "test-files/ops/RelationWithMostMembersOp/";
  const QString outputPath = "test-output/ops/RelationWithMostMembersOp/";

  RelationWithMostMembersOpTest()
  {
    setResetType(ResetBasic);
    TestUtils::mkpath(outputPath);
  }

  void runBasicTest()
  {
    OsmXmlReader reader;
    OsmMapPtr map(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.read("test-files/criterion/ComplexBuildings.osm", map);

    RelationWithMostMembersOp uut;
    uut.setCriterion("hoot::BuildingCriterion", "relation", conf());
    uut.setCriterion("hoot::BuildingCriterion", "member", conf());
    uut.apply(map);

    MapProjector::projectToWgs84(map);
    OsmXmlWriter writer;
    writer.write(map, outputPath + "RelationWithMostMembersOpOut.osm");
    HOOT_FILE_EQUALS(inputPath + "RelationWithMostMembersOpOut.osm",
                     outputPath + "RelationWithMostMembersOpOut.osm");
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(RelationWithMostMembersOpTest, "quick");

}
