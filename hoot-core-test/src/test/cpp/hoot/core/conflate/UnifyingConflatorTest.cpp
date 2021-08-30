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
 * @copyright Copyright (C) 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/conflate/UnifyingConflator.h>
#include <hoot/core/conflate/matching/MatchClassification.h>
#include <hoot/core/conflate/matching/MatchThreshold.h>

namespace hoot
{

/**
 * @brief The UnifyingConflatorTest class tests UnifyingConflator.
 *
 * The vast majority of UnifyingConflator is tested via command line tests, and this test class is
 * mainly meant for testing smaller, very specific pieces of functionality
 */
class UnifyingConflatorTest : public HootTestFixture
{
    CPPUNIT_TEST_SUITE(UnifyingConflatorTest);
    CPPUNIT_TEST(runConflateScoreTagsTest);
    CPPUNIT_TEST_SUITE_END();

public:

  UnifyingConflatorTest()
  {
  }

  void runConflateScoreTagsTest()
  {
    MatchClassification matchClassification;
    QStringList conflateScoreTagsFilter;
    NodePtr node = TestUtils::createNode();
    MatchThresholdPtr matchThreshold = std::make_shared<MatchThreshold>();
    UnifyingConflator uut(matchThreshold);

    // Create an element involved in a match and set the filter to Match.
    matchClassification.setMatch();
    conflateScoreTagsFilter.clear();
    conflateScoreTagsFilter.append("Match");
    uut.setConflateScoreTagsFilter(conflateScoreTagsFilter);
    node->getTags().clear();
    uut._addConflateScoreTags(node, matchClassification, *matchThreshold);
    // Verify the element had score tags added to it.
    LOG_VART(node->getTags());
    CPPUNIT_ASSERT(_haveScoreTags(node->getTags()));

    // Create an element involved in a match and set the filter to Review.
    matchClassification.setMatch();
    conflateScoreTagsFilter.clear();
    conflateScoreTagsFilter.append("Review");
    uut.setConflateScoreTagsFilter(conflateScoreTagsFilter);
    node->getTags().clear();
    uut._addConflateScoreTags(node, matchClassification, *matchThreshold);
    // Verify the element did not have score tags added to it.
    CPPUNIT_ASSERT(!_haveScoreTags(node->getTags()));

    // Create an element involved in a review and set the filter to Match/Review.
    matchClassification.setReview();
    conflateScoreTagsFilter.clear();
    conflateScoreTagsFilter.append("Match");
    conflateScoreTagsFilter.append("Review");
    uut.setConflateScoreTagsFilter(conflateScoreTagsFilter);
    node->getTags().clear();
    uut._addConflateScoreTags(node, matchClassification, *matchThreshold);
    // Verify the element had score tags added to it.
    CPPUNIT_ASSERT(_haveScoreTags(node->getTags()));

    // Create an element involved in a review and set the filter to Match/Miss.
    matchClassification.setReview();
    conflateScoreTagsFilter.clear();
    conflateScoreTagsFilter.append("Match");
    conflateScoreTagsFilter.append("Miss");
    uut.setConflateScoreTagsFilter(conflateScoreTagsFilter);
    node->getTags().clear();
    uut._addConflateScoreTags(node, matchClassification, *matchThreshold);
    // Verify the element did not have score tags added to it.
    CPPUNIT_ASSERT(!_haveScoreTags(node->getTags()));
  }

private:

  bool _haveScoreTags(const Tags& tags) const
  {
    // Think its basically ok if conflate score tags get added over time and they don't also get
    // added here. Just using this set as a basic indicator that any score tags got added.
    // hoot:score:uuid gets added in a method not being tested by this so is ommitted here.
    return
      tags.contains(MetadataTags::HootScoreMatch()) &&
      tags.contains(MetadataTags::HootScoreMiss()) &&
      tags.contains(MetadataTags::HootScoreReview()) &&
      tags.contains(MetadataTags::HootScoreClassification()) &&
      tags.contains(MetadataTags::HootScoreDetail()) &&
      tags.contains(MetadataTags::HootScoreMatchThreshold()) &&
      tags.contains(MetadataTags::HootScoreMissThreshold()) &&
      tags.contains(MetadataTags::HootScoreReviewThreshold());
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(UnifyingConflatorTest, "quick");

}
