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
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2019, 2021 Maxar (http://www.maxar.com/)
 */

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
  CPPUNIT_TEST(runCaseSensitivityTest);
  CPPUNIT_TEST(runMultiTest);
  CPPUNIT_TEST(runFileTest);
  CPPUNIT_TEST(runInvalidFilterTagJsonTest);
  CPPUNIT_TEST(runInvalidFilterSimilarityThresholdJsonTest);
  CPPUNIT_TEST(runInvalidFilterFormatJsonTest);
  CPPUNIT_TEST(runInvalidFilterCategoryJsonTest);
  CPPUNIT_TEST_SUITE_END();

public:

  TagAdvancedCriterionTest()
    : HootTestFixture("test-files/criterion/",
                      UNUSED_PATH)
  {
  }

  /**
   * A note about certain types of "aux matching", including alias, child, ancestor, and associated
   * with: combining string kvp wildcard filters with these type of options is not possible (but may
   * work in certain cases).  See the comments in runAliasTest for more detail.  The same comments
   * apply to tests for the aforementioned aux matching types.
   */
  void runMustTest()
  {
    // element must satisfy all parts of the filter

    TagAdvancedCriterion uut(
      "{ \"must\": [ { \"tag\": \"amenity=restaurant\" }, { \"tag\": \"poi=yes\" } ] }");

    NodePtr node =
      std::make_shared<Node>(Status::Unknown1, -1, geos::geom::Coordinate(0.0, 0.0), 15.0);

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
    // element needs only to satisfy any one part of the filter

    TagAdvancedCriterion uut(
      "{ \"should\": [ { \"tag\": \"amenity=restaurant\" }, { \"tag\": \"poi=yes\" } ] }");

    NodePtr node =
      std::make_shared<Node>(Status::Unknown1, -1, geos::geom::Coordinate(0.0, 0.0), 15.0);

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
    // element cannot satisfy any parts of the filter

    TagAdvancedCriterion uut(
      "{ \"must_not\": [ { \"tag\": \"amenity=restaurant\" }, { \"tag\": \"poi=yes\" } ] }");

    NodePtr node =
      std::make_shared<Node>(Status::Unknown1, -1, geos::geom::Coordinate(0.0, 0.0), 15.0);

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

    NodePtr node =
      std::make_shared<Node>(Status::Unknown1, -1, geos::geom::Coordinate(0.0, 0.0), 15.0);

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

    NodePtr node =
      std::make_shared<Node>(Status::Unknown1, -1, geos::geom::Coordinate(0.0, 0.0), 15.0);

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

    NodePtr node =
      std::make_shared<Node>(Status::Unknown1, -1, geos::geom::Coordinate(0.0, 0.0), 15.0);

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

    NodePtr node =
      std::make_shared<Node>(Status::Unknown1, -1, geos::geom::Coordinate(0.0, 0.0), 15.0);

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

    NodePtr node =
      std::make_shared<Node>(Status::Unknown1, -1, geos::geom::Coordinate(0.0, 0.0), 15.0);

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

    NodePtr node =
      std::make_shared<Node>(Status::Unknown1, -1, geos::geom::Coordinate(0.0, 0.0), 15.0);

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

    NodePtr node =
      std::make_shared<Node>(Status::Unknown1, -1, geos::geom::Coordinate(0.0, 0.0), 15.0);

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

    NodePtr node =
      std::make_shared<Node>(Status::Unknown1, -1, geos::geom::Coordinate(0.0, 0.0), 15.0);

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

  void runSimilarityTest()
  {
    std::shared_ptr<TagAdvancedCriterion> uut;
    NodePtr node =
      std::make_shared<Node>(Status::Unknown1, -1, geos::geom::Coordinate(0.0, 0.0), 15.0);

    node->getTags().clear();
    // amenity=community_centre has a similarity score of 0.7 with amenity=arts_centre as defined
    // in the hoot schema.
    node->getTags().set("amenity", "community_centre");

    // similarityThreshold is disabled by default
    uut =
      std::make_shared<TagAdvancedCriterion>(
        "{ \"must\": [ { \"tag\": \"amenity=arts_centre\" } ] }");
    CPPUNIT_ASSERT(!uut->isSatisfied(node));

    // a similarity score of -1.0 is the same as not using similarity scoring at all
    uut =
      std::make_shared<TagAdvancedCriterion>(
        "{ \"must\": [ { \"tag\": \"amenity=arts_centre\", \"similarityThreshold\": \"-1.0\" } ] }");
    CPPUNIT_ASSERT(!uut->isSatisfied(node));

    // input is at the threshold
    uut =
      std::make_shared<TagAdvancedCriterion>(
        "{ \"must\": [ { \"tag\": \"amenity=arts_centre\", \"similarityThreshold\": \"0.7\" } ] }");
    CPPUNIT_ASSERT(uut->isSatisfied(node));

    // input exceeds the threshold
    uut =
      std::make_shared<TagAdvancedCriterion>(
        "{ \"must\": [ { \"tag\": \"amenity=arts_centre\", \"similarityThreshold\": \"0.6\" } ] }");
    CPPUNIT_ASSERT(uut->isSatisfied(node));

    //input is below the threshold
    uut =
      std::make_shared<TagAdvancedCriterion>(
        "{ \"must\": [ { \"tag\": \"amenity=arts_centre\", \"similarityThreshold\": \"0.8\" } ] }");
    CPPUNIT_ASSERT(!uut->isSatisfied(node));
  }

  void runAliasTest()
  {
    std::shared_ptr<TagAdvancedCriterion> uut;
    NodePtr node =
      std::make_shared<Node>(Status::Unknown1, -1, geos::geom::Coordinate(0.0, 0.0), 15.0);

    node->getTags().clear();
    // amenity=ev_charging is an alias for amenity=charging_station, as defined in the hoot schema.
    node->getTags().set("amenity", "ev_charging");

    // allowAliases is false by default
    uut =
      std::make_shared<TagAdvancedCriterion>(
        "{ \"must\": [ { \"tag\": \"amenity=charging_station\" } ] }");
    CPPUNIT_ASSERT(!uut->isSatisfied(node));

    uut =
      std::make_shared<TagAdvancedCriterion>(
        "{ \"must\": [ { \"tag\": \"amenity=charging_station\", \"allowAliases\": \"true\" } ] }");
    CPPUNIT_ASSERT(uut->isSatisfied(node));

    uut =
      std::make_shared<TagAdvancedCriterion>(
        "{ \"must\": [ { \"tag\": \"amenity=charging_station\", \"allowAliases\": \"false\" } ] }");
    CPPUNIT_ASSERT(!uut->isSatisfied(node));

    // we don't allow partial wildcards for this type of aux matching and the wildcard part gets
    // automatically dropped, so this still matches
    uut =
      std::make_shared<TagAdvancedCriterion>(
        "{ \"must\": [ { \"tag\": \"amenity=charging_station*\", \"allowAliases\": \"true\" } ] }");
    CPPUNIT_ASSERT(uut->isSatisfied(node));

    // we don't allow partial wildcards for this type of aux matching, so this won't match
    uut =
      std::make_shared<TagAdvancedCriterion>(
        "{ \"must\": [ { \"tag\": \"amenity=charging*\", \"allowAliases\": \"true\" } ] }");
    CPPUNIT_ASSERT(!uut->isSatisfied(node));

    // full wildcards will still work for this type of aux matching, so this matches
    uut =
      std::make_shared<TagAdvancedCriterion>(
        "{ \"must\": [ { \"tag\": \"amenity=*\", \"allowAliases\": \"true\" } ] }");
    CPPUNIT_ASSERT(uut->isSatisfied(node));
  }

  void runChildTest()
  {
    std::shared_ptr<TagAdvancedCriterion> uut;
    NodePtr node =
      std::make_shared<Node>(Status::Unknown1, -1, geos::geom::Coordinate(0.0, 0.0), 15.0);

    node->getTags().clear();
    // surface=pebblestone is a child of surface=gravel.
    node->getTags().set("surface", "pebblestone");

    // allowChildren is false by default
    uut =
      std::make_shared<TagAdvancedCriterion>("{ \"must\": [ { \"tag\": \"surface=gravel\" } ] }");
    CPPUNIT_ASSERT(!uut->isSatisfied(node));

    uut =
      std::make_shared<TagAdvancedCriterion>(
        "{ \"must\": [ { \"tag\": \"surface=gravel\", \"allowChildren\": \"true\" } ] }");
    CPPUNIT_ASSERT(uut->isSatisfied(node));

    uut =
      std::make_shared<TagAdvancedCriterion>(
        "{ \"must\": [ { \"tag\": \"surface=gravel\", \"allowChildren\": \"false\" } ] }");
    CPPUNIT_ASSERT(!uut->isSatisfied(node));

    // see wildcard notes in runAliasTest

    uut =
      std::make_shared<TagAdvancedCriterion>(
        "{ \"must\": [ { \"tag\": \"surface=gravel*\", \"allowChildren\": \"true\" } ] }");
    CPPUNIT_ASSERT(uut->isSatisfied(node));

    uut =
      std::make_shared<TagAdvancedCriterion>(
        "{ \"must\": [ { \"tag\": \"surface=grav*\", \"allowChildren\": \"true\" } ] }");
    CPPUNIT_ASSERT(!uut->isSatisfied(node));

    uut =
      std::make_shared<TagAdvancedCriterion>(
        "{ \"must\": [ { \"tag\": \"surface=*\", \"allowChildren\": \"true\" } ] }");
    CPPUNIT_ASSERT(uut->isSatisfied(node));
  }

  void runAncestorTest()
  {
    std::shared_ptr<TagAdvancedCriterion> uut;
    NodePtr node =
      std::make_shared<Node>(Status::Unknown1, -1, geos::geom::Coordinate(0.0, 0.0), 15.0);

    node->getTags().clear();
    // highway=road is an ancestor child of highway=primary.
    node->getTags().set("highway", "road");

    // allowAncestors is false by default
    uut =
      std::make_shared<TagAdvancedCriterion>("{ \"must\": [ { \"tag\": \"highway=primary\" } ] }");
    CPPUNIT_ASSERT(!uut->isSatisfied(node));

    uut =
      std::make_shared<TagAdvancedCriterion>(
        "{ \"must\": [ { \"tag\": \"highway=primary\", \"allowAncestors\": \"true\" } ] }");
    CPPUNIT_ASSERT(uut->isSatisfied(node));

    uut =
      std::make_shared<TagAdvancedCriterion>(
        "{ \"must\": [ { \"tag\": \"highway=primary\", \"allowAncestors\": \"false\" } ] }");
    CPPUNIT_ASSERT(!uut->isSatisfied(node));

    // see wildcard notes in runAliasTest

    uut =
      std::make_shared<TagAdvancedCriterion>(
        "{ \"must\": [ { \"tag\": \"highway=primary*\", \"allowAncestors\": \"true\" } ] }");
    CPPUNIT_ASSERT(uut->isSatisfied(node));

    uut =
      std::make_shared<TagAdvancedCriterion>(
        "{ \"must\": [ { \"tag\": \"highway=prim*\", \"allowAncestors\": \"true\" } ] }");
    CPPUNIT_ASSERT(!uut->isSatisfied(node));

    uut =
      std::make_shared<TagAdvancedCriterion>(
        "{ \"must\": [ { \"tag\": \"highway=*\", \"allowAncestors\": \"true\" } ] }");
    CPPUNIT_ASSERT(uut->isSatisfied(node));
  }

  void runAssociatedWithTest()
  {
    std::shared_ptr<TagAdvancedCriterion> uut;
    NodePtr node =
      std::make_shared<Node>(Status::Unknown1, -1, geos::geom::Coordinate(0.0, 0.0), 15.0);

    node->getTags().clear();
    // roof:height is associated with building:part=yes.  The value used is irrelevant.
    node->getTags().set("roof:height", "15");

    // allowAssociations is false by default
    uut =
      std::make_shared<TagAdvancedCriterion>(
        "{ \"must\": [ { \"tag\": \"building:part=yes\" } ] }");
    CPPUNIT_ASSERT(!uut->isSatisfied(node));

    uut =
      std::make_shared<TagAdvancedCriterion>(
        "{ \"must\": [ { \"tag\": \"building:part=yes\", \"allowAssociations\": \"true\" } ] }");
    CPPUNIT_ASSERT(uut->isSatisfied(node));

    uut =
      std::make_shared<TagAdvancedCriterion>(
        "{ \"must\": [ { \"tag\": \"building:part=yes\", \"allowAssociations\": \"false\" } ] }");
    CPPUNIT_ASSERT(!uut->isSatisfied(node));

    // see wildcard notes in runAliasTest

    uut =
      std::make_shared<TagAdvancedCriterion>(
        "{ \"must\": [ { \"tag\": \"building:part=yes*\", \"allowAssociations\": \"true\" } ] }");
    CPPUNIT_ASSERT(uut->isSatisfied(node));

    uut =
      std::make_shared<TagAdvancedCriterion>(
        "{ \"must\": [ { \"tag\": \"building:part=y*\", \"allowAssociations\": \"true\" } ] }");
    CPPUNIT_ASSERT(!uut->isSatisfied(node));

    // associations are slightly different from the other aux match types in that they don't support
    // full wildcards, so this won't match
    uut =
      std::make_shared<TagAdvancedCriterion>(
        "{ \"must\": [ { \"tag\": \"building:part=*\", \"allowAssociations\": \"true\" } ] }");
    CPPUNIT_ASSERT(!uut->isSatisfied(node));

    node->getTags().clear();
    // More tags that are associated with building:part=yes.
    node->getTags().set("building:material", "wood");
    node->getTags().set("building_shape", "circular");
    uut =
      std::make_shared<TagAdvancedCriterion>(
        "{ \"must\": [ { \"tag\": \"building:part=yes\", \"allowAssociations\": \"true\" } ] }");
    CPPUNIT_ASSERT(uut->isSatisfied(node));

    node->getTags().clear();
    node->getTags().set("building:material", "*");
    uut =
      std::make_shared<TagAdvancedCriterion>(
        "{ \"must\": [ { \"tag\": \"building:part=yes\", \"allowAssociations\": \"true\" } ] }");
    CPPUNIT_ASSERT(uut->isSatisfied(node));

    node->getTags().clear();
    node->getTags().set("blah", "more_blah");
    uut =
      std::make_shared<TagAdvancedCriterion>(
        "{ \"must\": [ { \"tag\": \"building:part=yes\", \"allowAssociations\": \"true\" } ] }");
    CPPUNIT_ASSERT(!uut->isSatisfied(node));
  }

  void runCategoryTest()
  {
    std::shared_ptr<TagAdvancedCriterion> uut;
    NodePtr node =
      std::make_shared<Node>(Status::Unknown1, -1, geos::geom::Coordinate(0.0, 0.0), 15.0);

    node->getTags().clear();
    node->getTags().set("highway", "road");

    uut =
      std::make_shared<TagAdvancedCriterion>(
        "{ \"must\": [ { \"category\": \"transportation\" } ] }");
    CPPUNIT_ASSERT(uut->isSatisfied(node));

    uut =
      std::make_shared<TagAdvancedCriterion>("{ \"must\": [ { \"category\": \"building\" } ] }");
    CPPUNIT_ASSERT(!uut->isSatisfied(node));
  }

  void runContradictoryFilterTest()
  {
    std::shared_ptr<TagAdvancedCriterion> uut;
    NodePtr node =
      std::make_shared<Node>(Status::Unknown1, -1, geos::geom::Coordinate(0.0, 0.0), 15.0);

    node->getTags().clear();
    node->getTags().set("amenity", "restaurant");

    // If a filter contradicts itself, it should always fail to pass the element.

    uut =
      std::make_shared<TagAdvancedCriterion>(
        QString("{ \"must\": [ { \"tag\": \"amenity=restaurant\" } ], ") +
        QString("\"must_not\": [ { \"tag\": \"amenity=restaurant\" } ] }"));
    CPPUNIT_ASSERT(!uut->isSatisfied(node));

    uut =
      std::make_shared<TagAdvancedCriterion>(
        QString("{ \"must_not\": [ { \"tag\": \"amenity=restaurant\" } ], ") +
        QString("\"should\": [ { \"tag\": \"amenity=restaurant\" } ] }"));
    CPPUNIT_ASSERT(!uut->isSatisfied(node));
  }

  void runCaseSensitivityTest()
  {
    std::shared_ptr<TagAdvancedCriterion> uut;
    NodePtr node =
      std::make_shared<Node>(Status::Unknown1, -1, geos::geom::Coordinate(0.0, 0.0), 15.0);

    node->getTags().clear();
    node->getTags().set("name", "Starbucks");

    // Case sensitivity is set to false by default.

    uut =
      std::make_shared<TagAdvancedCriterion>(
        QString("{ \"must\": [ { \"tag\": \"name=starbucks\" } ] }"));
    CPPUNIT_ASSERT(uut->isSatisfied(node));

    uut =
      std::make_shared<TagAdvancedCriterion>(
        QString("{ \"must\": [ { \"tag\": \"name=Starbucks\" } ] }"));
    CPPUNIT_ASSERT(uut->isSatisfied(node));
  }

  void runMultiTest()
  {
    // Tests combinations of filters.  This doesn't test wildcards and other features, so its
    // possible additional multi tests will need to be added for those if bugs are found.

    TagAdvancedCriterion uut(
      QString("{ \"must\": [ { \"tag\": \"amenity=restaurant\" }, { \"tag\": \"poi=yes\" } ], ") +
      QString("\"must_not\": [ { \"tag\": \"tourism=hotel\" }, { \"tag\": \"building=yes\" } ], ") +
      QString("\"should\": [ { \"tag\": \"cuisine=italian\" } ] }"));

    NodePtr node =
      std::make_shared<Node>(Status::Unknown1, -1, geos::geom::Coordinate(0.0, 0.0), 15.0);

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
    node->getTags().set(MetadataTags::Building(), "yes");
    CPPUNIT_ASSERT(!uut.isSatisfied(node));
  }

  void runFileTest()
  {
    TagAdvancedCriterion uut(_inputPath + "TagAdvancedCriterionTest.json");
    NodePtr node =
      std::make_shared<Node>(Status::Unknown1, -1, geos::geom::Coordinate(0.0, 0.0), 15.0);

    node->getTags().clear();
    node->getTags().set("amenity", "restaurant");
    CPPUNIT_ASSERT(uut.isSatisfied(node));
  }

  void runInvalidFilterTagJsonTest()
  {
    std::shared_ptr<TagAdvancedCriterion> uut;
    QString exceptionMsg;

    try
    {
      uut =
        std::make_shared<TagAdvancedCriterion>("{ \"must\": [ { \"tag\": \" =arts_centre\" } ] }");
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT(exceptionMsg.startsWith("Invalid tag filter tag key"));

    try
    {
      uut = std::make_shared<TagAdvancedCriterion>("{ \"must\": [ { \"tag\": \"amenity = \" } ] }");
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT(exceptionMsg.startsWith("Invalid tag filter tag value"));

    try
    {
      uut = std::make_shared<TagAdvancedCriterion>("{ \"must\": [ { \"tag\": \" =\" } ] }");
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT(exceptionMsg.startsWith("Invalid tag filter"));

    try
    {
      uut = std::make_shared<TagAdvancedCriterion>("{ \"must\": [ { \"tag\": \"amenity \" } ] }");
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT(exceptionMsg.startsWith("Invalid tag filter"));

    // don't allow kvp full wildcards; a tag filter must always be filtering kvps in some way, or
    // the filter won't actually do any filtering
    try
    {
      uut = std::make_shared<TagAdvancedCriterion>("{ \"must\": [ { \"tag\": \"*\" } ] }");
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT(exceptionMsg.startsWith("Invalid tag filter"));
  }

  void runInvalidFilterSimilarityThresholdJsonTest()
  {
    std::shared_ptr<TagAdvancedCriterion> uut;
    QString exceptionMsg;

    // Thresholds must be a positive number <= 1.0.

    try
    {
      uut =
        std::make_shared<TagAdvancedCriterion>(
          "{ \"must\": [ { \"tag\": \"amenity=arts_centre\", \"similarityThreshold\": \"0.0\" } ] }");
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT(exceptionMsg.startsWith("Invalid tag filter similarity threshold"));

    try
    {
      uut =
        std::make_shared<TagAdvancedCriterion>(
          "{ \"must\": [ { \"tag\": \"amenity=arts_centre\", \"similarityThreshold\": \"1.1\" } ] }");
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT(exceptionMsg.startsWith("Invalid tag filter similarity threshold"));
  }

  void runInvalidFilterFormatJsonTest()
  {
    std::shared_ptr<TagAdvancedCriterion> uut;
    QString exceptionMsg;

    try
    {
      uut = std::make_shared<TagAdvancedCriterion>("{}");
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
      uut = std::make_shared<TagAdvancedCriterion>("{ \"must\": [ ] }");
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
      uut = std::make_shared<TagAdvancedCriterion>("{ \"must\": [ {} ] }");
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    LOG_VART(exceptionMsg);
    // arguably this could be "Empty tag filter specified." instead
    CPPUNIT_ASSERT(exceptionMsg.startsWith("Invalid tag filter"));

    try
    {
      uut =
        std::make_shared<TagAdvancedCriterion>(
          "{ \"blah\": [ { \"tag\": \"amenity=arts_centre\" } ] }");
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    LOG_VART(exceptionMsg);
    // arguably this could return "invalid" or something more descriptive instead
    CPPUNIT_ASSERT_EQUAL(
      QString("Empty tag filter specified.").toStdString(), exceptionMsg.toStdString());
  }

  void runInvalidFilterCategoryJsonTest()
  {
    std::shared_ptr<TagAdvancedCriterion> uut;
    QString exceptionMsg;

    try
    {
      uut = std::make_shared<TagAdvancedCriterion>("{ \"must\": [ { \"category\": \"blah\" } ] }");
      LOG_VART(uut->getDescription());
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT(exceptionMsg.startsWith("Unknown category"));

    // Empty categories are allowed in hoot, but make no sense in the context of a tag filter.

    try
    {
      uut = std::make_shared<TagAdvancedCriterion>("{ \"must\": [ { \"category\": \"\" } ] }");
      LOG_VART(uut->getDescription());
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT(exceptionMsg.startsWith("Unknown category"));

    try
    {
      uut =
        std::make_shared<TagAdvancedCriterion>(
          "{ \"must\": [ { \"tag\": \"amenity=arts_centre\", \"category\": \"\" } ] }");
      LOG_VART(uut->getDescription());
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT(exceptionMsg.startsWith("Unknown category"));
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(TagAdvancedCriterionTest, "quick");

}
