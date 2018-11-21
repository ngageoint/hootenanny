
// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/criterion/HasAddressCriterion.h>

namespace hoot
{

class HasAddressCriterionTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(HasAddressCriterionTest);
  CPPUNIT_TEST(runBasicTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runBasicTest()
  {
    HasAddressCriterion uut;

    NodePtr node1(new Node(Status::Unknown1, -1, geos::geom::Coordinate(0.0, 0.0), 15.0));
    node1->getTags().set(TestUtils::FULL_ADDRESS_TAG_NAME, "123 Main Street");
    CPPUNIT_ASSERT(uut.isSatisfied(node1));

    WayPtr way1(new Way(Status::Unknown1, -1, 15.0));
    way1->getTags().set(TestUtils::FULL_ADDRESS_TAG_NAME, "blah");
    CPPUNIT_ASSERT(!uut.isSatisfied(way1));

    WayPtr way2(new Way(Status::Unknown1, -1, 15.0));
    way2->getTags().set("blah", "123 Main Street");
    CPPUNIT_ASSERT(!uut.isSatisfied(way2));
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(HasAddressCriterionTest, "slow");

}
