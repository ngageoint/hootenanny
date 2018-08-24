
// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/rnd/language/visitors/ToEnglishTranslationVisitor.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>

namespace hoot
{

class ToEnglishTranslationVisitorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(ToEnglishTranslationVisitorTest);
  CPPUNIT_TEST(runTranslateTest);
  CPPUNIT_TEST(runTranslateMultipleSourceLangsTest);
  CPPUNIT_TEST(runSkipPreTranslatedTagsTest);
  CPPUNIT_TEST(runSkipWordsInEnglishDictTest);
  CPPUNIT_TEST_SUITE_END();

public:

  static QString testInputRoot;
  static QString testOutputRoot;

  ToEnglishTranslationVisitorTest()
  {
    testInputRoot =
      "test-files/language/visitors/ToEnglishTranslationVisitorTest";
    testOutputRoot =
      "test-files/language/visitors/ToEnglishTranslationVisitorTest";

    setResetType(ResetBasic);
    TestUtils::mkpath(testOutputRoot);
  }

  void runTranslateTest()
  {
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map, testInputRoot + "/runTranslateTest.osm", false, Status::Unknown1);

    ToEnglishTranslationVisitor visitor;
    Settings conf;
    conf.set("", "");
    visitor.setConfiguration(conf);

    map->visitRw(visitor);

    OsmMapWriterFactory::getInstance().write(map, testOutputRoot + "/runTranslateTest.osm");

    HOOT_FILE_EQUALS(
      testInputRoot + "/runTranslateTest.osm", testOutputRoot + "/runTranslateTest.osm");
  }

  void runTranslateMultipleSourceLangsTest()
  {

  }

  void runSkipPreTranslatedTagsTest()
  {

  }

  void runSkipWordsInEnglishDictTest()
  {

  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ToEnglishTranslationVisitorTest, "quick");

}


