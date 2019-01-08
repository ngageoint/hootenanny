/*
 * This file is part of Hootenanny.
 *
 * Hootenanny is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * --------------------------------------------------------------------
 *
 * The following copyright notices are generated automatically. If you
 * have a new notice to add, please use the format:
 * " * @copyright Copyright ..."
 * This will properly maintain the copyright information. DigitalGlobe
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2015, 2016, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/criterion/TagCriterion2.h>

namespace hoot
{

class TagCriterion2Test : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(TagCriterion2Test);
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
//  CPPUNIT_TEST(runMultiTest);
  //TODO: error tests
  CPPUNIT_TEST_SUITE_END();

public:

  void runMustTest()
  {
    const QString filter =
      "{ \"must\": [ { \"filter\": \"amenity=restaurant\" }, { \"filter\": \"poi=yes\" } ] }";
    TagCriterion2 uut(filter);

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
    const QString filter =
      "{ \"should\": [ { \"filter\": \"amenity=restaurant\" }, { \"filter\": \"poi=yes\" } ] }";
    TagCriterion2 uut(filter);

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
    const QString filter =
      "{ \"must_not\": [ { \"filter\": \"amenity=restaurant\" }, { \"filter\": \"poi=yes\" } ] }";
    TagCriterion2 uut(filter);

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
    const QString filter = "{ \"should\": [ { \"filter\": \"*=restaurant\" } ] }";
    TagCriterion2 uut(filter);

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
    const QString filter = "{ \"should\": [ { \"filter\": \"amenity*=restaurant\" } ] }";
    TagCriterion2 uut(filter);

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
    const QString filter = "{ \"should\": [ { \"filter\": \"*amenity=restaurant\" } ] }";
    TagCriterion2 uut(filter);

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
    const QString filter = "{ \"should\": [ { \"filter\": \"*amenity*=restaurant\" } ] }";
    TagCriterion2 uut(filter);

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
    const QString filter = "{ \"should\": [ { \"filter\": \"amenity=*\" } ] }";
    TagCriterion2 uut(filter);

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
    const QString filter = "{ \"should\": [ { \"filter\": \"amenity=water*\" } ] }";
    TagCriterion2 uut(filter);

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
    const QString filter = "{ \"should\": [ { \"filter\": \"amenity=*water\" } ] }";
    TagCriterion2 uut(filter);

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
    const QString filter = "{ \"should\": [ { \"filter\": \"amenity=*water*\" } ] }";
    TagCriterion2 uut(filter);

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
    boost::shared_ptr<TagCriterion2> uut;
    QString filter;
    NodePtr node(new Node(Status::Unknown1, -1, geos::geom::Coordinate(0.0, 0.0), 15.0));

    node->getTags().clear();
    // This is an alias for amenity=charging_station as defined in the hoot schema.
    node->getTags().set("amenity", "ev_charging");

    // allowAliases is false by default
    filter =
      "{ \"must\": [ { \"filter\": \"amenity=charging_station\" } ] }";
    uut.reset(new TagCriterion2(filter));
    CPPUNIT_ASSERT(!uut->isSatisfied(node));

    filter =
      "{ \"must\": [ { \"filter\": \"amenity=charging_station\", \"allowAliases\": \"true\" } ] }";
    uut.reset(new TagCriterion2(filter));
    CPPUNIT_ASSERT(uut->isSatisfied(node));

    filter =
      "{ \"must\": [ { \"filter\": \"amenity=charging_station\", \"allowAliases\": \"false\" } ] }";
    uut.reset(new TagCriterion2(filter));
    CPPUNIT_ASSERT(!uut->isSatisfied(node));

    filter =
      "{ \"must\": [ { \"filter\": \"amenity=charging_station*\", \"allowAliases\": \"true\" } ] }";
    uut.reset(new TagCriterion2(filter));
    CPPUNIT_ASSERT(uut->isSatisfied(node));

    filter =
      "{ \"must\": [ { \"filter\": \"amenity=charging*\", \"allowAliases\": \"true\" } ] }";
    uut.reset(new TagCriterion2(filter));
    CPPUNIT_ASSERT(!uut->isSatisfied(node));

    filter =
      "{ \"must\": [ { \"filter\": \"amenity=*\", \"allowAliases\": \"true\" } ] }";
    uut.reset(new TagCriterion2(filter));
    CPPUNIT_ASSERT(uut->isSatisfied(node));
  }

  void runSimilarityTest()
  {
    boost::shared_ptr<TagCriterion2> uut;
    QString filter;
    NodePtr node(new Node(Status::Unknown1, -1, geos::geom::Coordinate(0.0, 0.0), 15.0));

    node->getTags().clear();
    // amenity=community_centre has a similarity score of 0.7 with amenity=arts_centre as defined
    // in the hoot schema.
    node->getTags().set("amenity", "community_centre");

    // similarityThreshold is disabled by default
    filter =
      "{ \"must\": [ { \"filter\": \"amenity=arts_centre\" } ] }";
    uut.reset(new TagCriterion2(filter));
    CPPUNIT_ASSERT(!uut->isSatisfied(node));

    // a similarity score of -1.0 is the same as not using similarity scoring at all
    filter =
      "{ \"must\": [ { \"filter\": \"amenity=arts_centre\", \"similarityThreshold\": \"-1.0\" } ] }";
    uut.reset(new TagCriterion2(filter));
    CPPUNIT_ASSERT(!uut->isSatisfied(node));

    filter =
      "{ \"must\": [ { \"filter\": \"amenity=arts_centre\", \"similarityThreshold\": \"0.7\" } ] }";
    uut.reset(new TagCriterion2(filter));
    CPPUNIT_ASSERT(uut->isSatisfied(node));

    filter =
      "{ \"must\": [ { \"filter\": \"amenity=arts_centre\", \"similarityThreshold\": \"0.6\" } ] }";
    uut.reset(new TagCriterion2(filter));
    CPPUNIT_ASSERT(uut->isSatisfied(node));

    filter =
      "{ \"must\": [ { \"filter\": \"amenity=arts_centre\", \"similarityThreshold\": \"0.8\" } ] }";
    uut.reset(new TagCriterion2(filter));
    CPPUNIT_ASSERT(!uut->isSatisfied(node));
  }

  void runMultiTest()
  {

  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(TagCriterion2Test, "quick");

}
