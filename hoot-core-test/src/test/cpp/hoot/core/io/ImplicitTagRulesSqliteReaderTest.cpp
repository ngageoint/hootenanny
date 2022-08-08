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
 * @copyright Copyright (C) 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>

#include <hoot/core/io/ImplicitTagRulesSqliteReader.h>

namespace hoot
{

class ImplicitTagRulesSqliteReaderTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(ImplicitTagRulesSqliteReaderTest);
  CPPUNIT_TEST(runTagsTest);
  CPPUNIT_TEST(runMultipleRulesTest);
  CPPUNIT_TEST_SUITE_END();

public:

  ImplicitTagRulesSqliteReaderTest()
    : HootTestFixture("test-files/io/ImplicitTagRulesSqliteReaderTest/",
                      UNUSED_PATH)
  {
  }

  void runTagsTest()
  {
    ImplicitTagRulesSqliteReader reader;
    reader.setAddTopTagOnly(false);
    reader.setAllowWordsInvolvedInMultipleRules(false);
    reader.open(_inputPath + "rules.sqlite");

    QSet<QString> words;
    QSet<QString> wordsInvolved;
    bool wordsInvolvedInMultipleRules = false;
    Tags tags;

    words.insert("Mosque");
    tags = reader.getImplicitTags(words, wordsInvolved, wordsInvolvedInMultipleRules);
    CPPUNIT_ASSERT_EQUAL(2, tags.size());
    CPPUNIT_ASSERT(tags["amenity"] == "place_of_worship");
    CPPUNIT_ASSERT(tags["leisure"] == "park");
    CPPUNIT_ASSERT_EQUAL(1, wordsInvolved.size());
    CPPUNIT_ASSERT(wordsInvolved.contains("Mosque"));
    CPPUNIT_ASSERT(!wordsInvolvedInMultipleRules);
    words.clear();

    words.insert("MOSQUE");
    tags = reader.getImplicitTags(words, wordsInvolved, wordsInvolvedInMultipleRules);
    CPPUNIT_ASSERT_EQUAL(2, tags.size());
    CPPUNIT_ASSERT(tags["amenity"] == "place_of_worship");
    CPPUNIT_ASSERT(tags["leisure"] == "park");
    CPPUNIT_ASSERT_EQUAL(1, wordsInvolved.size());
    CPPUNIT_ASSERT(wordsInvolved.contains("Mosque"));
    CPPUNIT_ASSERT(!wordsInvolvedInMultipleRules);
    words.clear();

    words.insert("mosque");
    tags = reader.getImplicitTags(words, wordsInvolved, wordsInvolvedInMultipleRules);
    CPPUNIT_ASSERT_EQUAL(2, tags.size());
    CPPUNIT_ASSERT(tags["amenity"] == "place_of_worship");
    CPPUNIT_ASSERT(tags["leisure"] == "park");
    CPPUNIT_ASSERT_EQUAL(1, wordsInvolved.size());
    CPPUNIT_ASSERT(wordsInvolved.contains("Mosque"));
    CPPUNIT_ASSERT(!wordsInvolvedInMultipleRules);
    words.clear();

    words.insert("mosque2");
    tags = reader.getImplicitTags(words, wordsInvolved, wordsInvolvedInMultipleRules);
    CPPUNIT_ASSERT_EQUAL(0, tags.size());
    CPPUNIT_ASSERT_EQUAL(0, wordsInvolved.size());
    CPPUNIT_ASSERT(!wordsInvolvedInMultipleRules);
    words.clear();

    words.insert("mosque 3");
    tags = reader.getImplicitTags(words, wordsInvolved, wordsInvolvedInMultipleRules);
    CPPUNIT_ASSERT_EQUAL(0, tags.size());
    CPPUNIT_ASSERT_EQUAL(0, wordsInvolved.size());
    CPPUNIT_ASSERT(!wordsInvolvedInMultipleRules);
    words.clear();

    words.insert("Eid Prayer Ground");
    tags = reader.getImplicitTags(words, wordsInvolved, wordsInvolvedInMultipleRules);
    CPPUNIT_ASSERT_EQUAL(1, tags.size());
    CPPUNIT_ASSERT(tags["amenity"] == "place_of_worship");
    CPPUNIT_ASSERT_EQUAL(1, wordsInvolved.size());
    CPPUNIT_ASSERT(wordsInvolved.contains("Eid Prayer Ground"));
    CPPUNIT_ASSERT(!wordsInvolvedInMultipleRules);
    words.clear();

    words.insert("Mustashfa");
    tags = reader.getImplicitTags(words, wordsInvolved, wordsInvolvedInMultipleRules);
    CPPUNIT_ASSERT_EQUAL(1, tags.size());
    CPPUNIT_ASSERT(tags["amenity"] == "hospital");
    CPPUNIT_ASSERT_EQUAL(1, wordsInvolved.size());
    CPPUNIT_ASSERT(wordsInvolved.contains("Mustashfa"));
    CPPUNIT_ASSERT(!wordsInvolvedInMultipleRules);
    words.clear();

    words.insert("alwhdt");
    tags = reader.getImplicitTags(words, wordsInvolved, wordsInvolvedInMultipleRules);
    CPPUNIT_ASSERT_EQUAL(1, tags.size());
    CPPUNIT_ASSERT(tags["amenity"] == "clinic");
    CPPUNIT_ASSERT_EQUAL(1, wordsInvolved.size());
    CPPUNIT_ASSERT(wordsInvolved.contains("alwhdt"));
    CPPUNIT_ASSERT(!wordsInvolvedInMultipleRules);
    words.clear();

    words.insert("Mustashfa alwhdt");
    tags = reader.getImplicitTags(words, wordsInvolved, wordsInvolvedInMultipleRules);
    CPPUNIT_ASSERT_EQUAL(1, tags.size());
    CPPUNIT_ASSERT(tags["amenity"] == "hospital");
    CPPUNIT_ASSERT_EQUAL(1, wordsInvolved.size());
    CPPUNIT_ASSERT(wordsInvolved.contains("Mustashfa alwhdt"));
    CPPUNIT_ASSERT(!wordsInvolvedInMultipleRules);
    words.clear();

    //the AddImplicitlyDerivedTagsPoiVisitor won't pass a word phrase and its constituent
    //tokens in the same tags request, but if it did, then it should identify the words as
    //belonging to multiple rules
    words.insert("Mustashfa");
    words.insert("alwhdt");
    words.insert("Mustashfa alwhdt");
    tags = reader.getImplicitTags(words, wordsInvolved, wordsInvolvedInMultipleRules);
    CPPUNIT_ASSERT_EQUAL(0, tags.size());
    CPPUNIT_ASSERT_EQUAL(3, wordsInvolved.size());
    CPPUNIT_ASSERT(wordsInvolved.contains("Mustashfa"));
    CPPUNIT_ASSERT(wordsInvolved.contains("alwhdt"));
    CPPUNIT_ASSERT(wordsInvolved.contains("Mustashfa alwhdt"));
    CPPUNIT_ASSERT(wordsInvolvedInMultipleRules);
    words.clear();

    reader.close();
  }

  void runMultipleRulesTest()
  {
    ImplicitTagRulesSqliteReader reader;
    reader.setAddTopTagOnly(false);
    reader.setAllowWordsInvolvedInMultipleRules(false);
    reader.open(_inputPath + "rules.sqlite");

    QSet<QString> words;
    QSet<QString> ruleWordsInvolved;
    bool wordsInvolvedInMultipleRules = false;
    Tags tags;

    words.insert("Mosque");
    tags = reader.getImplicitTags(words, ruleWordsInvolved, wordsInvolvedInMultipleRules);
    CPPUNIT_ASSERT_EQUAL(1, ruleWordsInvolved.size());
    CPPUNIT_ASSERT(ruleWordsInvolved.contains("Mosque"));
    CPPUNIT_ASSERT(!wordsInvolvedInMultipleRules);
    CPPUNIT_ASSERT(!tags.isEmpty());
    words.clear();

    words.insert("MOSQUE");
    tags = reader.getImplicitTags(words, ruleWordsInvolved, wordsInvolvedInMultipleRules);
    CPPUNIT_ASSERT_EQUAL(1, ruleWordsInvolved.size());
    CPPUNIT_ASSERT(ruleWordsInvolved.contains("Mosque"));
    CPPUNIT_ASSERT(!wordsInvolvedInMultipleRules);
    CPPUNIT_ASSERT(!tags.isEmpty());
    words.clear();

    words.insert("mosque");
    tags = reader.getImplicitTags(words, ruleWordsInvolved, wordsInvolvedInMultipleRules);
    CPPUNIT_ASSERT_EQUAL(1, ruleWordsInvolved.size());
    CPPUNIT_ASSERT(ruleWordsInvolved.contains("Mosque"));
    CPPUNIT_ASSERT(!wordsInvolvedInMultipleRules);
    CPPUNIT_ASSERT(!tags.isEmpty());
    words.clear();

    words.insert("Mustashfa");
    tags = reader.getImplicitTags(words, ruleWordsInvolved, wordsInvolvedInMultipleRules);
    CPPUNIT_ASSERT_EQUAL(1, ruleWordsInvolved.size());
    CPPUNIT_ASSERT(ruleWordsInvolved.contains("Mustashfa"));
    CPPUNIT_ASSERT(!wordsInvolvedInMultipleRules);
    CPPUNIT_ASSERT(!tags.isEmpty());
    words.clear();

    words.insert("MUSTASHFA");
    tags = reader.getImplicitTags(words, ruleWordsInvolved, wordsInvolvedInMultipleRules);
    CPPUNIT_ASSERT_EQUAL(1, ruleWordsInvolved.size());
    CPPUNIT_ASSERT(ruleWordsInvolved.contains("Mustashfa"));
    CPPUNIT_ASSERT(!wordsInvolvedInMultipleRules);
    CPPUNIT_ASSERT(!tags.isEmpty());
    words.clear();

    words.insert("mustashfa");
    tags = reader.getImplicitTags(words, ruleWordsInvolved, wordsInvolvedInMultipleRules);
    CPPUNIT_ASSERT_EQUAL(1, ruleWordsInvolved.size());
    CPPUNIT_ASSERT(ruleWordsInvolved.contains("Mustashfa"));
    CPPUNIT_ASSERT(!wordsInvolvedInMultipleRules);
    CPPUNIT_ASSERT(!tags.isEmpty());
    words.clear();

    words.insert("mosque");
    words.insert("Mustashfa");
    tags = reader.getImplicitTags(words, ruleWordsInvolved, wordsInvolvedInMultipleRules);
    CPPUNIT_ASSERT_EQUAL(2, ruleWordsInvolved.size());
    CPPUNIT_ASSERT(ruleWordsInvolved.contains("Mustashfa"));
    CPPUNIT_ASSERT(ruleWordsInvolved.contains("Mosque"));
    CPPUNIT_ASSERT(wordsInvolvedInMultipleRules);
    CPPUNIT_ASSERT(tags.isEmpty());
    words.clear();

    words.insert("mosque2");
    words.insert("Mustashfa");
    tags = reader.getImplicitTags(words, ruleWordsInvolved, wordsInvolvedInMultipleRules);
    CPPUNIT_ASSERT_EQUAL(1, ruleWordsInvolved.size());
    CPPUNIT_ASSERT(ruleWordsInvolved.contains("Mustashfa"));
    CPPUNIT_ASSERT(!wordsInvolvedInMultipleRules);
    CPPUNIT_ASSERT(!tags.isEmpty());
    words.clear();

    words.insert("mosque 3");
    words.insert("Mustashfa");
    tags = reader.getImplicitTags(words, ruleWordsInvolved, wordsInvolvedInMultipleRules);
    CPPUNIT_ASSERT_EQUAL(1, ruleWordsInvolved.size());
    CPPUNIT_ASSERT(ruleWordsInvolved.contains("Mustashfa"));
    CPPUNIT_ASSERT(!wordsInvolvedInMultipleRules);
    CPPUNIT_ASSERT(!tags.isEmpty());
    words.clear();

    words.insert("alwhdt");
    tags = reader.getImplicitTags(words, ruleWordsInvolved, wordsInvolvedInMultipleRules);
    CPPUNIT_ASSERT_EQUAL(1, ruleWordsInvolved.size());
    CPPUNIT_ASSERT(ruleWordsInvolved.contains("alwhdt"));
    CPPUNIT_ASSERT(!wordsInvolvedInMultipleRules);
    CPPUNIT_ASSERT(!tags.isEmpty());
    words.clear();

    words.insert("Mustashfa");
    words.insert("alwhdt");
    tags = reader.getImplicitTags(words, ruleWordsInvolved, wordsInvolvedInMultipleRules);
    CPPUNIT_ASSERT_EQUAL(2, ruleWordsInvolved.size());
    CPPUNIT_ASSERT(ruleWordsInvolved.contains("Mustashfa"));
    CPPUNIT_ASSERT(ruleWordsInvolved.contains("alwhdt"));
    CPPUNIT_ASSERT(wordsInvolvedInMultipleRules);
    CPPUNIT_ASSERT(tags.isEmpty());
    words.clear();

    words.insert("Mustashfa alwhdt");
    tags = reader.getImplicitTags(words, ruleWordsInvolved, wordsInvolvedInMultipleRules);
    CPPUNIT_ASSERT_EQUAL(1, ruleWordsInvolved.size());
    CPPUNIT_ASSERT(ruleWordsInvolved.contains("Mustashfa alwhdt"));
    CPPUNIT_ASSERT(!wordsInvolvedInMultipleRules);
    CPPUNIT_ASSERT(!tags.isEmpty());
    words.clear();

    words.insert("Mustashfa alwhdt");
    words.insert("Mustashfa");
    words.insert("alwhdt");
    tags = reader.getImplicitTags(words, ruleWordsInvolved, wordsInvolvedInMultipleRules);
    CPPUNIT_ASSERT_EQUAL(3, ruleWordsInvolved.size());
    CPPUNIT_ASSERT(ruleWordsInvolved.contains("Mustashfa"));
    CPPUNIT_ASSERT(ruleWordsInvolved.contains("alwhdt"));
    CPPUNIT_ASSERT(ruleWordsInvolved.contains("Mustashfa alwhdt"));
    CPPUNIT_ASSERT(wordsInvolvedInMultipleRules);
    CPPUNIT_ASSERT(tags.isEmpty());
    words.clear();

    words.insert("MUSTASHFA ALWHDT");
    words.insert("MUSTASHFA");
    words.insert("ALWHDT");
    tags = reader.getImplicitTags(words, ruleWordsInvolved, wordsInvolvedInMultipleRules);
    CPPUNIT_ASSERT_EQUAL(3, ruleWordsInvolved.size());
    CPPUNIT_ASSERT(ruleWordsInvolved.contains("Mustashfa"));
    CPPUNIT_ASSERT(ruleWordsInvolved.contains("alwhdt"));
    CPPUNIT_ASSERT(ruleWordsInvolved.contains("Mustashfa alwhdt"));
    CPPUNIT_ASSERT(wordsInvolvedInMultipleRules);
    CPPUNIT_ASSERT(tags.isEmpty());
    words.clear();

    words.insert("mustashfa alwhdt");
    words.insert("mustashfa");
    words.insert("alwhdt");
    tags = reader.getImplicitTags(words, ruleWordsInvolved, wordsInvolvedInMultipleRules);
    CPPUNIT_ASSERT_EQUAL(3, ruleWordsInvolved.size());
    CPPUNIT_ASSERT(ruleWordsInvolved.contains("Mustashfa"));
    CPPUNIT_ASSERT(ruleWordsInvolved.contains("alwhdt"));
    CPPUNIT_ASSERT(ruleWordsInvolved.contains("Mustashfa alwhdt"));
    CPPUNIT_ASSERT(wordsInvolvedInMultipleRules);
    CPPUNIT_ASSERT(tags.isEmpty());
    words.clear();

    reader.close();
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ImplicitTagRulesSqliteReaderTest, "quick");

}
