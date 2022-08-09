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
#include <hoot/core/criterion/ReviewScoreCriterion.h>
#include <hoot/core/elements/Relation.h>
#include <hoot/core/schema/MetadataTags.h>

namespace hoot
{

class ReviewScoreCriterionTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(ReviewScoreCriterionTest);
  CPPUNIT_TEST(runBasicTest);
  CPPUNIT_TEST(runInvertTest);
  CPPUNIT_TEST(invalidThresholdTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runBasicTest()
  {
    ReviewScoreCriterion uut;
    uut.setMinScoreThreshold(0.2);
    uut.setMaxScoreThreshold(0.8);
    uut.setInvertThresholding(false);

    // just creating a barebones, empty review relation...all we need for these tests
    RelationPtr reviewRelation = std::make_shared<Relation>(Status::Conflated, 1);
    reviewRelation->getTags().set(MetadataTags::HootReviewNeeds(), true);

    reviewRelation->getTags().set(MetadataTags::HootReviewScore(), "0.6");
    CPPUNIT_ASSERT(uut.isSatisfied(reviewRelation));

    reviewRelation->getTags().set(MetadataTags::HootReviewScore(), "0.9");
    CPPUNIT_ASSERT(!uut.isSatisfied(reviewRelation));

    // check boundaries

    ReviewScoreCriterion uut2;
    uut2.setMinScoreThreshold(0.0);
    uut2.setMaxScoreThreshold(1.0);
    uut2.setInvertThresholding(false);

    reviewRelation->getTags().set(MetadataTags::HootReviewScore(), "0.0");
    CPPUNIT_ASSERT(uut2.isSatisfied(reviewRelation));

    reviewRelation->getTags().set(MetadataTags::HootReviewScore(), "1.0");
    CPPUNIT_ASSERT(uut2.isSatisfied(reviewRelation));
  }

  void runInvertTest()
  {
    ReviewScoreCriterion uut;
    uut.setMinScoreThreshold(0.2);
    uut.setMaxScoreThreshold(0.8);
    uut.setInvertThresholding(true);

    RelationPtr reviewRelation = std::make_shared<Relation>(Status::Conflated, 1);
    reviewRelation->getTags().set(MetadataTags::HootReviewNeeds(), true);

    reviewRelation->getTags().set(MetadataTags::HootReviewScore(), "0.1");
    CPPUNIT_ASSERT(uut.isSatisfied(reviewRelation));

    reviewRelation->getTags().set(MetadataTags::HootReviewScore(), "0.9");
    CPPUNIT_ASSERT(uut.isSatisfied(reviewRelation));

    reviewRelation->getTags().set(MetadataTags::HootReviewScore(), "0.6");
    CPPUNIT_ASSERT(!uut.isSatisfied(reviewRelation));

    reviewRelation->getTags().set(MetadataTags::HootReviewScore(), "0.2");
    CPPUNIT_ASSERT(!uut.isSatisfied(reviewRelation));

    reviewRelation->getTags().set(MetadataTags::HootReviewScore(), "0.8");
    CPPUNIT_ASSERT(!uut.isSatisfied(reviewRelation));

    // check boundaries

    ReviewScoreCriterion uut2;
    uut2.setMinScoreThreshold(0.0);
    uut2.setMaxScoreThreshold(1.0);
    uut2.setInvertThresholding(true);

    reviewRelation->getTags().set(MetadataTags::HootReviewScore(), "0.0");
    CPPUNIT_ASSERT(!uut2.isSatisfied(reviewRelation));

    reviewRelation->getTags().set(MetadataTags::HootReviewScore(), "0.1");
    CPPUNIT_ASSERT(!uut2.isSatisfied(reviewRelation));

    reviewRelation->getTags().set(MetadataTags::HootReviewScore(), "1.0");
    CPPUNIT_ASSERT(!uut2.isSatisfied(reviewRelation));
  }

  void invalidThresholdTest()
  {
    ReviewScoreCriterion uut;
    QString exceptionMsg;

    try
    {
      uut.setMinScoreThreshold(-0.01);
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT(exceptionMsg.contains("Review score threshold outside of allowed range"));

    try
    {
      uut.setMinScoreThreshold(1.1);
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT(exceptionMsg.contains("Review score threshold outside of allowed range"));
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ReviewScoreCriterionTest, "quick");

}
