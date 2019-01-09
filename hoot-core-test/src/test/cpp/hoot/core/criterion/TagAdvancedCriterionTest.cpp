
// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/criterion/TagAdvancedCriterion.h>

namespace hoot
{

class TagAdvancedCriterionTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(TagAdvancedCriterionTest);
  CPPUNIT_TEST(runMustTest);
  CPPUNIT_TEST(runShouldTest);
  CPPUNIT_TEST(runMustNotTest);
  CPPUNIT_TEST(runKeyWildcard1Test);
  CPPUNIT_TEST(runKeyWildcard2Test);
  CPPUNIT_TEST(runKeyWildcard3Test);
  CPPUNIT_TEST(runKeyWildcard4Test);
  CPPUNIT_TEST(runValueWildcard1Test);
  CPPUNIT_TEST(runValueWildcard2Test);
  CPPUNIT_TEST(runValueWildcard3Test);
  CPPUNIT_TEST(runAliasTest);
  CPPUNIT_TEST(runSimilarityTest);
  CPPUNIT_TEST(runChildTest);
  CPPUNIT_TEST(runAncestorTest);
  CPPUNIT_TEST(runAssociatedWithTest);
  CPPUNIT_TEST(runCategoryTest);
  CPPUNIT_TEST(runContradictoryFilterTest);
  CPPUNIT_TEST(runMultiTest);
  CPPUNIT_TEST(runInvalidFilterTagJsonTest);
  CPPUNIT_TEST(runInvalidFilterSimilarityThresholdJsonTest);
  CPPUNIT_TEST(runInvalidFilterFormatJsonTest);
  CPPUNIT_TEST(runInvalidFilterCategoryJsonTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runMustTest()
  {
    TagAdvancedCriterion uut(
      "{ \"must\": [ { \"tag\": \"amenity=restaurant\" }, { \"tag\": \"poi=yes\" } ] }");

    NodePtr node(new Node(Status::Unknown1, -1, geos::geom::Coordinate(0.0, 0.0), 15.0));

    node->getTags().clear();
    node->getTags().set("amenity", "restaurant");
    CPPUNIT_ASSERT(!uut.isSatisfied(node));

    node->getTags().clear();
    node->getTags().set("poi", "yes");
    CPPUNIT_ASSERT(!uut.isSatisfied(node));

    node->getTags().clear();
    node->getTags().set("amenity", "hospital");
    CPPUNIT_ASSERT(!uut.isSatisfied(node));

    node->getTags().clear();
    node->getTags().set("amenity", "restaurant");
    node->getTags().set("poi", "yes");
    CPPUNIT_ASSERT(uut.isSatisfied(node));
  }

  void runShouldTest()
  {
    TagAdvancedCriterion uut(
      "{ \"should\": [ { \"tag\": \"amenity=restaurant\" }, { \"tag\": \"poi=yes\" } ] }");

    NodePtr node(new Node(Status::Unknown1, -1, geos::geom::Coordinate(0.0, 0.0), 15.0));

    node->getTags().clear();
    node->getTags().set("amenity", "restaurant");
    CPPUNIT_ASSERT(uut.isSatisfied(node));

    node->getTags().clear();
    node->getTags().set("poi", "yes");
    CPPUNIT_ASSERT(uut.isSatisfied(node));

    node->getTags().clear();
    node->getTags().set("amenity", "hospital");
    CPPUNIT_ASSERT(!uut.isSatisfied(node));

    node->getTags().clear();
    node->getTags().set("amenity", "restaurant");
    node->getTags().set("poi", "yes");
    CPPUNIT_ASSERT(uut.isSatisfied(node));
  }

  void runMustNotTest()
  {
    TagAdvancedCriterion uut(
      "{ \"must_not\": [ { \"tag\": \"amenity=restaurant\" }, { \"tag\": \"poi=yes\" } ] }");

    NodePtr node(new Node(Status::Unknown1, -1, geos::geom::Coordinate(0.0, 0.0), 15.0));

    node->getTags().clear();
    node->getTags().set("amenity", "restaurant");
    CPPUNIT_ASSERT(!uut.isSatisfied(node));

    node->getTags().clear();
    node->getTags().set("poi", "yes");
    CPPUNIT_ASSERT(!uut.isSatisfied(node));

    node->getTags().clear();
    node->getTags().set("amenity", "hospital");
    CPPUNIT_ASSERT(uut.isSatisfied(node));

    node->getTags().clear();
    node->getTags().set("amenity", "restaurant");
    node->getTags().set("poi", "yes");
    CPPUNIT_ASSERT(!uut.isSatisfied(node));
  }

  void runKeyWildcard1Test()
  {
    TagAdvancedCriterion uut("{ \"should\": [ { \"tag\": \"*=restaurant\" } ] }");

    NodePtr node(new Node(Status::Unknown1, -1, geos::geom::Coordinate(0.0, 0.0), 15.0));

    node->getTags().clear();
    node->getTags().set("amenity", "restaurant");
    CPPUNIT_ASSERT(uut.isSatisfied(node));

    node->getTags().clear();
    node->getTags().set("poi", "yes");
    CPPUNIT_ASSERT(!uut.isSatisfied(node));

    node->getTags().clear();
    node->getTags().set("amenity", "hospital");
    CPPUNIT_ASSERT(!uut.isSatisfied(node));

    node->getTags().clear();
    node->getTags().set("amenity", "restaurant");
    node->getTags().set("poi", "yes");
    CPPUNIT_ASSERT(uut.isSatisfied(node));
  }

  void runKeyWildcard2Test()
  {
    TagAdvancedCriterion uut("{ \"should\": [ { \"tag\": \"amenity*=restaurant\" } ] }");

    NodePtr node(new Node(Status::Unknown1, -1, geos::geom::Coordinate(0.0, 0.0), 15.0));

    node->getTags().clear();
    node->getTags().set("amenity", "restaurant");
    CPPUNIT_ASSERT(uut.isSatisfied(node));

    node->getTags().clear();
    node->getTags().set("amenity_2", "restaurant");
    CPPUNIT_ASSERT(uut.isSatisfied(node));

    node->getTags().clear();
    node->getTags().set("my_amenity", "restaurant");
    CPPUNIT_ASSERT(!uut.isSatisfied(node));

    node->getTags().clear();
    node->getTags().set("poi", "yes");
    CPPUNIT_ASSERT(!uut.isSatisfied(node));

    node->getTags().clear();
    node->getTags().set("amenity_2", "restaurant");
    node->getTags().set("poi", "yes");
    CPPUNIT_ASSERT(uut.isSatisfied(node));
  }

  void runKeyWildcard3Test()
  {
    TagAdvancedCriterion uut("{ \"should\": [ { \"tag\": \"*amenity=restaurant\" } ] }");

    NodePtr node(new Node(Status::Unknown1, -1, geos::geom::Coordinate(0.0, 0.0), 15.0));

    node->getTags().clear();
    node->getTags().set("amenity", "restaurant");
    CPPUNIT_ASSERT(uut.isSatisfied(node));

    node->getTags().clear();
    node->getTags().set("amenity_2", "restaurant");
    CPPUNIT_ASSERT(!uut.isSatisfied(node));

    node->getTags().clear();
    node->getTags().set("my_amenity", "restaurant");
    CPPUNIT_ASSERT(uut.isSatisfied(node));

    node->getTags().clear();
    node->getTags().set("poi", "yes");
    CPPUNIT_ASSERT(!uut.isSatisfied(node));

    node->getTags().clear();
    node->getTags().set("my_amenity", "restaurant");
    node->getTags().set("poi", "yes");
    CPPUNIT_ASSERT(uut.isSatisfied(node));
  }

  void runKeyWildcard4Test()
  {
    TagAdvancedCriterion uut("{ \"should\": [ { \"tag\": \"*amenity*=restaurant\" } ] }");

    NodePtr node(new Node(Status::Unknown1, -1, geos::geom::Coordinate(0.0, 0.0), 15.0));

    node->getTags().clear();
    node->getTags().set("amenity", "restaurant");
    CPPUNIT_ASSERT(uut.isSatisfied(node));

    node->getTags().clear();
    node->getTags().set("amenity_2", "restaurant");
    CPPUNIT_ASSERT(uut.isSatisfied(node));

    node->getTags().clear();
    node->getTags().set("my_amenity", "restaurant");
    CPPUNIT_ASSERT(uut.isSatisfied(node));

    node->getTags().clear();
    node->getTags().set("poi", "yes");
    CPPUNIT_ASSERT(!uut.isSatisfied(node));

    node->getTags().clear();
    node->getTags().set("my_amenity", "restaurant");
    node->getTags().set("poi", "yes");
    CPPUNIT_ASSERT(uut.isSatisfied(node));
  }

  void runValueWildcard1Test()
  {
    TagAdvancedCriterion uut("{ \"should\": [ { \"tag\": \"amenity=*\" } ] }");

    NodePtr node(new Node(Status::Unknown1, -1, geos::geom::Coordinate(0.0, 0.0), 15.0));

    node->getTags().clear();
    node->getTags().set("amenity", "restaurant");
    CPPUNIT_ASSERT(uut.isSatisfied(node));

    node->getTags().clear();
    node->getTags().set("poi", "yes");
    CPPUNIT_ASSERT(!uut.isSatisfied(node));

    node->getTags().clear();
    node->getTags().set("amenity", "hospital");
    CPPUNIT_ASSERT(uut.isSatisfied(node));

    node->getTags().clear();
    node->getTags().set("amenity", "restaurant");
    node->getTags().set("poi", "yes");
    CPPUNIT_ASSERT(uut.isSatisfied(node));
  }

  void runValueWildcard2Test()
  {
    TagAdvancedCriterion uut("{ \"should\": [ { \"tag\": \"amenity=water*\" } ] }");

    NodePtr node(new Node(Status::Unknown1, -1, geos::geom::Coordinate(0.0, 0.0), 15.0));

    node->getTags().clear();
    node->getTags().set("amenity", "water");
    CPPUNIT_ASSERT(uut.isSatisfied(node));

    node->getTags().clear();
    node->getTags().set("amenity", "water_park");
    CPPUNIT_ASSERT(uut.isSatisfied(node));

    node->getTags().clear();
    node->getTags().set("amenity", "new_water_park");
    CPPUNIT_ASSERT(!uut.isSatisfied(node));

    node->getTags().clear();
    node->getTags().set("poi", "yes");
    CPPUNIT_ASSERT(!uut.isSatisfied(node));

    node->getTags().clear();
    node->getTags().set("amenity", "water_park");
    node->getTags().set("poi", "yes");
    CPPUNIT_ASSERT(uut.isSatisfied(node));
  }

  void runValueWildcard3Test()
  {
    TagAdvancedCriterion uut("{ \"should\": [ { \"tag\": \"amenity=*water\" } ] }");

    NodePtr node(new Node(Status::Unknown1, -1, geos::geom::Coordinate(0.0, 0.0), 15.0));

    node->getTags().clear();
    node->getTags().set("amenity", "water");
    CPPUNIT_ASSERT(uut.isSatisfied(node));

    node->getTags().clear();
    node->getTags().set("amenity", "water_park");
    CPPUNIT_ASSERT(!uut.isSatisfied(node));

    node->getTags().clear();
    node->getTags().set("amenity", "new_water");
    CPPUNIT_ASSERT(uut.isSatisfied(node));

    node->getTags().clear();
    node->getTags().set("poi", "yes");
    CPPUNIT_ASSERT(!uut.isSatisfied(node));

    node->getTags().clear();
    node->getTags().set("amenity", "new_water");
    node->getTags().set("poi", "yes");
    CPPUNIT_ASSERT(uut.isSatisfied(node));
  }

  void runValueWildcard4Test()
  {
    TagAdvancedCriterion uut("{ \"should\": [ { \"tag\": \"amenity=*water*\" } ] }");

    NodePtr node(new Node(Status::Unknown1, -1, geos::geom::Coordinate(0.0, 0.0), 15.0));

    node->getTags().clear();
    node->getTags().set("amenity", "water");
    CPPUNIT_ASSERT(uut.isSatisfied(node));

    node->getTags().clear();
    node->getTags().set("amenity", "water_park");
    CPPUNIT_ASSERT(uut.isSatisfied(node));

    node->getTags().clear();
    node->getTags().set("amenity", "new_water");
    CPPUNIT_ASSERT(uut.isSatisfied(node));

    node->getTags().clear();
    node->getTags().set("poi", "yes");
    CPPUNIT_ASSERT(!uut.isSatisfied(node));

    node->getTags().clear();
    node->getTags().set("amenity", "new_water");
    node->getTags().set("poi", "yes");
    CPPUNIT_ASSERT(uut.isSatisfied(node));
  }

  void runAliasTest()
  {
    boost::shared_ptr<TagAdvancedCriterion> uut;
    NodePtr node(new Node(Status::Unknown1, -1, geos::geom::Coordinate(0.0, 0.0), 15.0));

    node->getTags().clear();
    // This is an alias for amenity=charging_station as defined in the hoot schema.
    node->getTags().set("amenity", "ev_charging");

    // allowAliases is false by default
    uut.reset(new TagAdvancedCriterion("{ \"must\": [ { \"tag\": \"amenity=charging_station\" } ] }"));
    CPPUNIT_ASSERT(!uut->isSatisfied(node));

    uut.reset(
      new TagAdvancedCriterion(
        "{ \"must\": [ { \"tag\": \"amenity=charging_station\", \"allowAliases\": \"true\" } ] }"));
    CPPUNIT_ASSERT(uut->isSatisfied(node));

    uut.reset(
      new TagAdvancedCriterion(
        "{ \"must\": [ { \"tag\": \"amenity=charging_station\", \"allowAliases\": \"false\" } ] }"));
    CPPUNIT_ASSERT(!uut->isSatisfied(node));

    uut.reset(
      new TagAdvancedCriterion(
        "{ \"must\": [ { \"tag\": \"amenity=charging_station*\", \"allowAliases\": \"true\" } ] }"));
    CPPUNIT_ASSERT(uut->isSatisfied(node));

    uut.reset(
      new TagAdvancedCriterion(
        "{ \"must\": [ { \"tag\": \"amenity=charging*\", \"allowAliases\": \"true\" } ] }"));
    CPPUNIT_ASSERT(!uut->isSatisfied(node));

    uut.reset(
      new TagAdvancedCriterion(
        "{ \"must\": [ { \"tag\": \"amenity=*\", \"allowAliases\": \"true\" } ] }"));
    CPPUNIT_ASSERT(uut->isSatisfied(node));
  }

  void runSimilarityTest()
  {
    boost::shared_ptr<TagAdvancedCriterion> uut;
    NodePtr node(new Node(Status::Unknown1, -1, geos::geom::Coordinate(0.0, 0.0), 15.0));

    node->getTags().clear();
    // amenity=community_centre has a similarity score of 0.7 with amenity=arts_centre as defined
    // in the hoot schema.
    node->getTags().set("amenity", "community_centre");

    // similarityThreshold is disabled by default
    uut.reset(new TagAdvancedCriterion("{ \"must\": [ { \"tag\": \"amenity=arts_centre\" } ] }"));
    CPPUNIT_ASSERT(!uut->isSatisfied(node));

    // a similarity score of -1.0 is the same as not using similarity scoring at all
    uut.reset(
      new TagAdvancedCriterion(
        "{ \"must\": [ { \"tag\": \"amenity=arts_centre\", \"similarityThreshold\": \"-1.0\" } ] }"));
    CPPUNIT_ASSERT(!uut->isSatisfied(node));

    uut.reset(
      new TagAdvancedCriterion(
        "{ \"must\": [ { \"tag\": \"amenity=arts_centre\", \"similarityThreshold\": \"0.7\" } ] }"));
    CPPUNIT_ASSERT(uut->isSatisfied(node));

    uut.reset(
      new TagAdvancedCriterion(
        "{ \"must\": [ { \"tag\": \"amenity=arts_centre\", \"similarityThreshold\": \"0.6\" } ] }"));
    CPPUNIT_ASSERT(uut->isSatisfied(node));

    uut.reset(
      new TagAdvancedCriterion(
        "{ \"must\": [ { \"tag\": \"amenity=arts_centre\", \"similarityThreshold\": \"0.8\" } ] }"));
    CPPUNIT_ASSERT(!uut->isSatisfied(node));
  }

  void runChildTest()
  {
    boost::shared_ptr<TagAdvancedCriterion> uut;
    NodePtr node(new Node(Status::Unknown1, -1, geos::geom::Coordinate(0.0, 0.0), 15.0));

    node->getTags().clear();
    // This is a child of surface=gravel.
    node->getTags().set("surface", "pebblestone");

    // allowChildren is false by default
    uut.reset(new TagAdvancedCriterion("{ \"must\": [ { \"tag\": \"surface=gravel\" } ] }"));
    CPPUNIT_ASSERT(!uut->isSatisfied(node));

    uut.reset(
      new TagAdvancedCriterion(
        "{ \"must\": [ { \"tag\": \"surface=gravel\", \"allowChildren\": \"true\" } ] }"));
    CPPUNIT_ASSERT(uut->isSatisfied(node));

    uut.reset(
      new TagAdvancedCriterion(
        "{ \"must\": [ { \"tag\": \"surface=gravel\", \"allowChildren\": \"false\" } ] }"));
    CPPUNIT_ASSERT(!uut->isSatisfied(node));

    uut.reset(
      new TagAdvancedCriterion(
        "{ \"must\": [ { \"tag\": \"surface=gravel*\", \"allowChildren\": \"true\" } ] }"));
    CPPUNIT_ASSERT(uut->isSatisfied(node));

    uut.reset(
      new TagAdvancedCriterion(
        "{ \"must\": [ { \"tag\": \"surface=grav*\", \"allowChildren\": \"true\" } ] }"));
    CPPUNIT_ASSERT(!uut->isSatisfied(node));

    uut.reset(
      new TagAdvancedCriterion(
        "{ \"must\": [ { \"tag\": \"surface=*\", \"allowChildren\": \"true\" } ] }"));
    CPPUNIT_ASSERT(uut->isSatisfied(node));
  }

  void runAncestorTest()
  {
    //child, parent
    //CPPUNIT_ASSERT_EQUAL(true, uut.isAncestor("highway=primary", "highway=road"));

    boost::shared_ptr<TagAdvancedCriterion> uut;
    NodePtr node(new Node(Status::Unknown1, -1, geos::geom::Coordinate(0.0, 0.0), 15.0));

    node->getTags().clear();
    // This is an ancestor child of highway=primary.
    node->getTags().set("highway", "road");

    // allowAncestors is false by default
    uut.reset(new TagAdvancedCriterion("{ \"must\": [ { \"tag\": \"highway=primary\" } ] }"));
    CPPUNIT_ASSERT(!uut->isSatisfied(node));

    uut.reset(
      new TagAdvancedCriterion(
        "{ \"must\": [ { \"tag\": \"highway=primary\", \"allowAncestors\": \"true\" } ] }"));
    CPPUNIT_ASSERT(uut->isSatisfied(node));

    uut.reset(
      new TagAdvancedCriterion(
        "{ \"must\": [ { \"tag\": \"highway=primary\", \"allowAncestors\": \"false\" } ] }"));
    CPPUNIT_ASSERT(!uut->isSatisfied(node));

    uut.reset(
      new TagAdvancedCriterion(
        "{ \"must\": [ { \"tag\": \"highway=primary*\", \"allowAncestors\": \"true\" } ] }"));
    CPPUNIT_ASSERT(uut->isSatisfied(node));

    uut.reset(
      new TagAdvancedCriterion(
        "{ \"must\": [ { \"tag\": \"highway=prim*\", \"allowAncestors\": \"true\" } ] }"));
    CPPUNIT_ASSERT(!uut->isSatisfied(node));

    uut.reset(
      new TagAdvancedCriterion(
        "{ \"must\": [ { \"tag\": \"highway=*\", \"allowAncestors\": \"true\" } ] }"));
    CPPUNIT_ASSERT(uut->isSatisfied(node));
  }

  void runAssociatedWithTest()
  {
    boost::shared_ptr<TagAdvancedCriterion> uut;
    NodePtr node(new Node(Status::Unknown1, -1, geos::geom::Coordinate(0.0, 0.0), 15.0));

    node->getTags().clear();
    // This is associated with building:part=yes.
    node->getTags().set("roof:height", "15");

    // allowAssociations is false by default
    uut.reset(new TagAdvancedCriterion("{ \"must\": [ { \"tag\": \"building:part=yes\" } ] }"));
    CPPUNIT_ASSERT(!uut->isSatisfied(node));

    uut.reset(
      new TagAdvancedCriterion(
        "{ \"must\": [ { \"tag\": \"building:part=yes\", \"allowAssociations\": \"true\" } ] }"));
    CPPUNIT_ASSERT(uut->isSatisfied(node));

    uut.reset(
      new TagAdvancedCriterion(
        "{ \"must\": [ { \"tag\": \"building:part=yes\", \"allowAssociations\": \"false\" } ] }"));
    CPPUNIT_ASSERT(!uut->isSatisfied(node));

    uut.reset(
      new TagAdvancedCriterion(
        "{ \"must\": [ { \"tag\": \"building:part=yes*\", \"allowAssociations\": \"true\" } ] }"));
    CPPUNIT_ASSERT(uut->isSatisfied(node));

    uut.reset(
      new TagAdvancedCriterion(
        "{ \"must\": [ { \"tag\": \"building:part=y*\", \"allowAssociations\": \"true\" } ] }"));
    CPPUNIT_ASSERT(!uut->isSatisfied(node));

    uut.reset(
      new TagAdvancedCriterion(
        "{ \"must\": [ { \"tag\": \"building:part=*\", \"allowAssociations\": \"true\" } ] }"));
    CPPUNIT_ASSERT(!uut->isSatisfied(node));

    node->getTags().clear();
    // More tags that are associated with building:part=yes.
    node->getTags().set("building:material", "wood");
    node->getTags().set("building_shape", "circular");
    uut.reset(
      new TagAdvancedCriterion(
        "{ \"must\": [ { \"tag\": \"building:part=yes\", \"allowAssociations\": \"true\" } ] }"));
    CPPUNIT_ASSERT(uut->isSatisfied(node));

    node->getTags().clear();
    node->getTags().set("building:material", "*");
    uut.reset(
      new TagAdvancedCriterion(
        "{ \"must\": [ { \"tag\": \"building:part=yes\", \"allowAssociations\": \"true\" } ] }"));
    CPPUNIT_ASSERT(uut->isSatisfied(node));

    node->getTags().clear();
    node->getTags().set("blah", "more_blah");
    uut.reset(
      new TagAdvancedCriterion(
        "{ \"must\": [ { \"tag\": \"building:part=yes\", \"allowAssociations\": \"true\" } ] }"));
    CPPUNIT_ASSERT(!uut->isSatisfied(node));
  }

  void runCategoryTest()
  {
    boost::shared_ptr<TagAdvancedCriterion> uut;
    NodePtr node(new Node(Status::Unknown1, -1, geos::geom::Coordinate(0.0, 0.0), 15.0));

    node->getTags().clear();
    node->getTags().set("highway", "road");

    uut.reset(new TagAdvancedCriterion("{ \"must\": [ { \"category\": \"transportation\" } ] }"));
    CPPUNIT_ASSERT(uut->isSatisfied(node));

    uut.reset(new TagAdvancedCriterion("{ \"must\": [ { \"category\": \"building\" } ] }"));
    CPPUNIT_ASSERT(!uut->isSatisfied(node));
  }

  void runContradictoryFilterTest()
  {
    boost::shared_ptr<TagAdvancedCriterion> uut;
    NodePtr node(new Node(Status::Unknown1, -1, geos::geom::Coordinate(0.0, 0.0), 15.0));

    node->getTags().clear();
    node->getTags().set("amenity", "restaurant");

    uut.reset(
      new TagAdvancedCriterion(
        QString("{ \"must\": [ { \"tag\": \"amenity=restaurant\" } ], ") +
        QString("\"must_not\": [ { \"tag\": \"amenity=restaurant\" } ] }")));
    CPPUNIT_ASSERT(!uut->isSatisfied(node));

    uut.reset(
      new TagAdvancedCriterion(
        QString("{ \"must_not\": [ { \"tag\": \"amenity=restaurant\" } ], ") +
        QString("\"should\": [ { \"tag\": \"amenity=restaurant\" } ] }")));
    CPPUNIT_ASSERT(!uut->isSatisfied(node));
  }

  void runMultiTest()
  {
    // This doesn't test wildcards and other features, so its possible additional multi tests will
    // need to be added for those if bugs are found.

    TagAdvancedCriterion uut(
      QString("{ \"must\": [ { \"tag\": \"amenity=restaurant\" }, { \"tag\": \"poi=yes\" } ], ") +
      QString("\"must_not\": [ { \"tag\": \"tourism=hotel\" }, { \"tag\": \"building=yes\" } ], ") +
      QString("\"should\": [ { \"tag\": \"cuisine=italian\" } ] }"));

    NodePtr node(new Node(Status::Unknown1, -1, geos::geom::Coordinate(0.0, 0.0), 15.0));

    node->getTags().clear();
    node->getTags().set("amenity", "restaurant");
    node->getTags().set("cuisine", "italian");
    node->getTags().set("poi", "yes");
    CPPUNIT_ASSERT(uut.isSatisfied(node));

    node->getTags().clear();
    node->getTags().set("amenity", "restaurant");
    node->getTags().set("poi", "yes");
    CPPUNIT_ASSERT(!uut.isSatisfied(node));

    node->getTags().clear();
    node->getTags().set("cuisine", "italian");
    node->getTags().set("poi", "yes");
    CPPUNIT_ASSERT(!uut.isSatisfied(node));

    node->getTags().clear();
    node->getTags().set("cuisine", "italian");
    node->getTags().set("amenity", "restaurant");
    CPPUNIT_ASSERT(!uut.isSatisfied(node));

    node->getTags().clear();
    node->getTags().set("amenity", "restaurant");
    node->getTags().set("cuisine", "italian");
    node->getTags().set("poi", "yes");
    node->getTags().set("tourism", "hotel");
    CPPUNIT_ASSERT(!uut.isSatisfied(node));

    node->getTags().clear();
    node->getTags().set("amenity", "restaurant");
    node->getTags().set("cuisine", "italian");
    node->getTags().set("poi", "yes");
    node->getTags().set("building", "yes");
    CPPUNIT_ASSERT(!uut.isSatisfied(node));
  }

  void runInvalidFilterTagJsonTest()
  {
    boost::shared_ptr<TagAdvancedCriterion> uut;
    QString exceptionMsg;

    try
    {
      uut.reset(new TagAdvancedCriterion("{ \"must\": [ { \"tag\": \" =arts_centre\" } ] }"));
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT(exceptionMsg.startsWith("Invalid tag filter tag key"));

    try
    {
      uut.reset(new TagAdvancedCriterion("{ \"must\": [ { \"tag\": \"amenity = \" } ] }"));
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT(exceptionMsg.startsWith("Invalid tag filter tag value"));

    try
    {
      uut.reset(new TagAdvancedCriterion("{ \"must\": [ { \"tag\": \" =\" } ] }"));
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT(exceptionMsg.startsWith("Invalid tag filter"));

    try
    {
      uut.reset(new TagAdvancedCriterion("{ \"must\": [ { \"tag\": \"amenity \" } ] }"));
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT(exceptionMsg.startsWith("Invalid tag filter"));

    try
    {
      uut.reset(new TagAdvancedCriterion("{ \"must\": [ { \"tag\": \"*\" } ] }"));
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT(exceptionMsg.startsWith("Invalid tag filter"));
  }

  void runInvalidFilterSimilarityThresholdJsonTest()
  {
    boost::shared_ptr<TagAdvancedCriterion> uut;
    QString exceptionMsg;

    try
    {
      uut.reset(
        new TagAdvancedCriterion(
          "{ \"must\": [ { \"tag\": \"amenity=arts_centre\", \"similarityThreshold\": \"0.0\" } ] }"));
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT(exceptionMsg.startsWith("Invalid tag filter similarity threshold"));

    try
    {
      uut.reset(
        new TagAdvancedCriterion(
          "{ \"must\": [ { \"tag\": \"amenity=arts_centre\", \"similarityThreshold\": \"1.1\" } ] }"));
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT(exceptionMsg.startsWith("Invalid tag filter similarity threshold"));
  }

  void runInvalidFilterFormatJsonTest()
  {
    boost::shared_ptr<TagAdvancedCriterion> uut;
    QString exceptionMsg;

    try
    {
      uut.reset(new TagAdvancedCriterion("{}"));
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    LOG_VART(exceptionMsg);
    CPPUNIT_ASSERT_EQUAL(
      QString("Empty tag filter specified.").toStdString(), exceptionMsg.toStdString());

    try
    {
      uut.reset(new TagAdvancedCriterion("{ \"must\": [ ] }"));
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    LOG_VART(exceptionMsg);
    CPPUNIT_ASSERT_EQUAL(
      QString("Empty tag filter specified.").toStdString(), exceptionMsg.toStdString());

    try
    {
      uut.reset(new TagAdvancedCriterion("{ \"must\": [ {} ] }"));
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    LOG_VART(exceptionMsg);
    CPPUNIT_ASSERT(exceptionMsg.startsWith("Invalid tag filter"));
//    CPPUNIT_ASSERT_EQUAL(
//      QString("If no tag filter is specified a category filter must be specified.")
//        .toStdString(),
//      exceptionMsg.toStdString());

    try
    {
      uut.reset(new TagAdvancedCriterion("{ \"blah\": [ { \"tag\": \"amenity=arts_centre\" } ] }"));
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    LOG_VART(exceptionMsg);
    CPPUNIT_ASSERT_EQUAL(
      QString("Empty tag filter specified.").toStdString(), exceptionMsg.toStdString());
  }

  void runInvalidFilterCategoryJsonTest()
  {
    boost::shared_ptr<TagAdvancedCriterion> uut;
    QString exceptionMsg;

    try
    {
      uut.reset(new TagAdvancedCriterion("{ \"must\": [ { \"category\": \"blah\" } ] }"));
      LOG_VART(uut->getDescription());
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT(exceptionMsg.startsWith("Unknown category"));

    try
    {
      uut.reset(new TagAdvancedCriterion("{ \"must\": [ { \"category\": \"\" } ] }"));
      LOG_VART(uut->getDescription());
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    //CPPUNIT_ASSERT_EQUAL(
      //QString("Empty tag category.").toStdString(), exceptionMsg.toStdString());
    CPPUNIT_ASSERT(exceptionMsg.startsWith("Unknown category"));

    try
    {
      uut.reset(
        new TagAdvancedCriterion(
          "{ \"must\": [ { \"tag\": \"amenity=arts_centre\", \"category\": \"\" } ] }"));
      LOG_VART(uut->getDescription());
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    //CPPUNIT_ASSERT_EQUAL(
      //QString("Empty tag category.").toStdString(), exceptionMsg.toStdString());
    CPPUNIT_ASSERT(exceptionMsg.startsWith("Unknown category"));
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(TagAdvancedCriterionTest, "quick");

}
