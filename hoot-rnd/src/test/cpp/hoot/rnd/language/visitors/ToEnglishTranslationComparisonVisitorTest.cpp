
// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/rnd/language/visitors/ToEnglishTranslationComparisonVisitor.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>

namespace hoot
{

class ToEnglishTranslationComparisonVisitorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(ToEnglishTranslationComparisonVisitorTest);
  CPPUNIT_TEST(runTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runTest()
  {

  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ToEnglishTranslationComparisonVisitorTest, "quick");

}


