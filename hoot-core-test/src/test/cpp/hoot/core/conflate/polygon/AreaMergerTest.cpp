// Hoot
#include "../../TestUtils.h"
#include <hoot/core/conflate/polygon/AreaMerger.h>

// Qt
#include <QDir>

namespace hoot
{

class AreaMergerTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(AreaMergerTest);
  //CPPUNIT_TEST(mergeWayAsPolyTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void setUp()
  {
    TestUtils::mkpath("test-output/conflate/polygon");
  }


};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(AreaMergerTest, "quick");

}
