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
 * @copyright Copyright (C) 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/rnd/io/ImplicitTagRulesSqliteReader.h>

// Qt
#include <QDir>
#include <QSet>
#include <QString>

namespace hoot
{

class ImplicitTagRulesSqliteReaderTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(ImplicitTagRulesSqliteReaderTest);
  CPPUNIT_TEST(runTagsTest);
  CPPUNIT_TEST(runMultipleRulesTest);
  //TODO:
  //CPPUNIT_TEST(runReadAllTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runTagsTest()
  {
    ImplicitTagRulesSqliteReader reader;
    reader.open("test-files/io/ImplicitTagRulesSqliteReaderTest/rules.sqlite");

    QSet<QString> words;
    words.insert("Mosque");
    QSet<QString> wordsInvolved;
    Tags tags = reader.getImplicitTags(words, wordsInvolved);
    CPPUNIT_ASSERT_EQUAL(2, tags.size());
    CPPUNIT_ASSERT(tags["amenity"] == "place_of_worship");
    CPPUNIT_ASSERT(tags["leisure"] == "park");
    CPPUNIT_ASSERT_EQUAL(1, wordsInvolved.size());
    CPPUNIT_ASSERT(wordsInvolved.contains("Mosque"));
    words.clear();
    wordsInvolved.clear();

    words.insert("MOSQUE");
    tags = reader.getImplicitTags(words, wordsInvolved);
    CPPUNIT_ASSERT_EQUAL(2, tags.size());
    CPPUNIT_ASSERT(tags["amenity"] == "place_of_worship");
    CPPUNIT_ASSERT(tags["leisure"] == "park");
    CPPUNIT_ASSERT_EQUAL(1, wordsInvolved.size());
    CPPUNIT_ASSERT(wordsInvolved.contains("Mosque"));
    words.clear();
    wordsInvolved.clear();

    words.insert("mosque");
    tags = reader.getImplicitTags(words, wordsInvolved);
    CPPUNIT_ASSERT_EQUAL(2, tags.size());
    CPPUNIT_ASSERT(tags["amenity"] == "place_of_worship");
    CPPUNIT_ASSERT(tags["leisure"] == "park");
    CPPUNIT_ASSERT_EQUAL(1, wordsInvolved.size());
    CPPUNIT_ASSERT(wordsInvolved.contains("Mosque"));
    words.clear();
    wordsInvolved.clear();

    words.insert("mosque2");
    tags = reader.getImplicitTags(words, wordsInvolved);
    CPPUNIT_ASSERT_EQUAL(0, tags.size());
    CPPUNIT_ASSERT_EQUAL(0, wordsInvolved.size());
    words.clear();
    wordsInvolved.clear();

    words.insert("mosque 3");
    tags = reader.getImplicitTags(words, wordsInvolved);
    CPPUNIT_ASSERT_EQUAL(0, tags.size());
    CPPUNIT_ASSERT_EQUAL(0, wordsInvolved.size());
    words.clear();
    wordsInvolved.clear();

    words.insert("Eid Prayer Ground");
    tags = reader.getImplicitTags(words, wordsInvolved);
    CPPUNIT_ASSERT_EQUAL(1, tags.size());
    CPPUNIT_ASSERT(tags["amenity"] == "place_of_worship");
    CPPUNIT_ASSERT_EQUAL(1, wordsInvolved.size());
    CPPUNIT_ASSERT(wordsInvolved.contains("Eid Prayer Ground"));
    words.clear();
    wordsInvolved.clear();

    words.insert("Mustashfa");
    tags = reader.getImplicitTags(words, wordsInvolved);
    CPPUNIT_ASSERT_EQUAL(1, tags.size());
    CPPUNIT_ASSERT(tags["amenity"] == "hospital");
    CPPUNIT_ASSERT_EQUAL(1, wordsInvolved.size());
    CPPUNIT_ASSERT(wordsInvolved.contains("Mustashfa"));
    words.clear();
    wordsInvolved.clear();

    words.insert("alwhdt");
    tags = reader.getImplicitTags(words, wordsInvolved);
    CPPUNIT_ASSERT_EQUAL(1, tags.size());
    CPPUNIT_ASSERT(tags["amenity"] == "clinic");
    CPPUNIT_ASSERT_EQUAL(1, wordsInvolved.size());
    CPPUNIT_ASSERT(wordsInvolved.contains("alwhdt"));
    words.clear();
    wordsInvolved.clear();

    words.insert("Mustashfa alwhdt");
    tags = reader.getImplicitTags(words, wordsInvolved);
    CPPUNIT_ASSERT_EQUAL(1, tags.size());
    CPPUNIT_ASSERT(tags["amenity"] == "hospital");
    CPPUNIT_ASSERT_EQUAL(1, wordsInvolved.size());
    CPPUNIT_ASSERT(wordsInvolved.contains("Mustashfa alwhdt"));
    words.clear();
    wordsInvolved.clear();

    words.insert("Mustashfa");
    words.insert("alwhdt");
    words.insert("Mustashfa alwhdt");
    tags = reader.getImplicitTags(words, wordsInvolved);
    CPPUNIT_ASSERT_EQUAL(1, tags.size());
    LOG_VART(tags["amenity"]);
    CPPUNIT_ASSERT(tags["amenity"] == "hospital;clinic");
    CPPUNIT_ASSERT_EQUAL(3, wordsInvolved.size());
    CPPUNIT_ASSERT(wordsInvolved.contains("Mustashfa"));
    CPPUNIT_ASSERT(wordsInvolved.contains("alwhdt"));
    CPPUNIT_ASSERT(wordsInvolved.contains("Mustashfa alwhdt"));
    words.clear();
    wordsInvolved.clear();

    reader.close();
  }

  void runMultipleRulesTest()
  {
    ImplicitTagRulesSqliteReader reader;
    reader.setMaxQueryResultsSize(3);
    reader.open("test-files/io/ImplicitTagRulesSqliteReaderTest/rules.sqlite");

    QSet<QString> words;
    QSet<QString> ruleWordsInvolved;

    words.insert("Mosque");
    CPPUNIT_ASSERT(!reader.wordsInvolveMultipleRules(words, ruleWordsInvolved));
    CPPUNIT_ASSERT_EQUAL(0, ruleWordsInvolved.size());
    words.clear();
    ruleWordsInvolved.clear();

    words.insert("MOSQUE");
    CPPUNIT_ASSERT(!reader.wordsInvolveMultipleRules(words, ruleWordsInvolved));
    CPPUNIT_ASSERT_EQUAL(0, ruleWordsInvolved.size());
    words.clear();
    ruleWordsInvolved.clear();

    words.insert("mosque");
    CPPUNIT_ASSERT(!reader.wordsInvolveMultipleRules(words, ruleWordsInvolved));
    CPPUNIT_ASSERT_EQUAL(0, ruleWordsInvolved.size());
    words.clear();
    ruleWordsInvolved.clear();

    words.insert("Mustashfa");
    CPPUNIT_ASSERT(!reader.wordsInvolveMultipleRules(words, ruleWordsInvolved));
    CPPUNIT_ASSERT_EQUAL(0, ruleWordsInvolved.size());
    words.clear();
    ruleWordsInvolved.clear();

    words.insert("MUSTASHFA");
    CPPUNIT_ASSERT(!reader.wordsInvolveMultipleRules(words, ruleWordsInvolved));
    CPPUNIT_ASSERT_EQUAL(0, ruleWordsInvolved.size());
    words.clear();
    ruleWordsInvolved.clear();

    words.insert("mustashfa");
    CPPUNIT_ASSERT(!reader.wordsInvolveMultipleRules(words, ruleWordsInvolved));
    CPPUNIT_ASSERT_EQUAL(0, ruleWordsInvolved.size());
    words.clear();
    ruleWordsInvolved.clear();

    words.insert("mosque");
    words.insert("Mustashfa");
    CPPUNIT_ASSERT(reader.wordsInvolveMultipleRules(words, ruleWordsInvolved));
    CPPUNIT_ASSERT_EQUAL(2, ruleWordsInvolved.size());
    CPPUNIT_ASSERT(ruleWordsInvolved.contains("Mustashfa"));
    CPPUNIT_ASSERT(ruleWordsInvolved.contains("Mosque"));
    words.clear();
    ruleWordsInvolved.clear();

    words.insert("mosque2");
    words.insert("Mustashfa");
    CPPUNIT_ASSERT(!reader.wordsInvolveMultipleRules(words, ruleWordsInvolved));
    CPPUNIT_ASSERT_EQUAL(0, ruleWordsInvolved.size());
    words.clear();
    ruleWordsInvolved.clear();

    words.insert("mosque 3");
    words.insert("Mustashfa");
    CPPUNIT_ASSERT(!reader.wordsInvolveMultipleRules(words, ruleWordsInvolved));
    CPPUNIT_ASSERT_EQUAL(0, ruleWordsInvolved.size());
    words.clear();
    ruleWordsInvolved.clear();

    words.insert("alwhdt");
    CPPUNIT_ASSERT(!reader.wordsInvolveMultipleRules(words, ruleWordsInvolved));
    CPPUNIT_ASSERT_EQUAL(0, ruleWordsInvolved.size());
    words.clear();
    ruleWordsInvolved.clear();

    words.insert("Mustashfa");
    words.insert("alwhdt");
    CPPUNIT_ASSERT(reader.wordsInvolveMultipleRules(words, ruleWordsInvolved));
    CPPUNIT_ASSERT_EQUAL(2, ruleWordsInvolved.size());
    CPPUNIT_ASSERT(ruleWordsInvolved.contains("Mustashfa"));
    CPPUNIT_ASSERT(ruleWordsInvolved.contains("alwhdt"));
    words.clear();
    ruleWordsInvolved.clear();

    words.insert("Mustashfa alwhdt");
    CPPUNIT_ASSERT(!reader.wordsInvolveMultipleRules(words, ruleWordsInvolved));
    CPPUNIT_ASSERT_EQUAL(0, ruleWordsInvolved.size());
    words.clear();
    ruleWordsInvolved.clear();

    words.insert("Mustashfa alwhdt");
    words.insert("Mustashfa");
    words.insert("alwhdt");
    CPPUNIT_ASSERT(reader.wordsInvolveMultipleRules(words, ruleWordsInvolved));
    CPPUNIT_ASSERT_EQUAL(3, ruleWordsInvolved.size());
    CPPUNIT_ASSERT(ruleWordsInvolved.contains("Mustashfa"));
    CPPUNIT_ASSERT(ruleWordsInvolved.contains("alwhdt"));
    CPPUNIT_ASSERT(ruleWordsInvolved.contains("Mustashfa alwhdt"));
    words.clear();
    ruleWordsInvolved.clear();

    words.insert("MUSTASHFA ALWHDT");
    words.insert("MUSTASHFA");
    words.insert("ALWHDT");
    CPPUNIT_ASSERT(reader.wordsInvolveMultipleRules(words, ruleWordsInvolved));
    CPPUNIT_ASSERT_EQUAL(3, ruleWordsInvolved.size());
    CPPUNIT_ASSERT(ruleWordsInvolved.contains("Mustashfa"));
    CPPUNIT_ASSERT(ruleWordsInvolved.contains("alwhdt"));
    CPPUNIT_ASSERT(ruleWordsInvolved.contains("Mustashfa alwhdt"));
    words.clear();
    ruleWordsInvolved.clear();

    words.insert("mustashfa alwhdt");
    words.insert("mustashfa");
    words.insert("alwhdt");
    CPPUNIT_ASSERT(reader.wordsInvolveMultipleRules(words, ruleWordsInvolved));
    CPPUNIT_ASSERT_EQUAL(3, ruleWordsInvolved.size());
    CPPUNIT_ASSERT(ruleWordsInvolved.contains("Mustashfa"));
    CPPUNIT_ASSERT(ruleWordsInvolved.contains("alwhdt"));
    CPPUNIT_ASSERT(ruleWordsInvolved.contains("Mustashfa alwhdt"));
    words.clear();
    ruleWordsInvolved.clear();

    reader.close();
  }

  void runReadAllTest()
  {
    ImplicitTagRulesSqliteReader reader;
    reader.setMaxQueryResultsSize(3);
    reader.open("test-files/io/ImplicitTagRulesSqliteReaderTest/rules.sqlite");
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ImplicitTagRulesSqliteReaderTest, "quick");

}
