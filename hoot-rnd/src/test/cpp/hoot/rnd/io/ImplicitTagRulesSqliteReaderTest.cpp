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
  CPPUNIT_TEST_SUITE_END();

public:

  void runTagsTest()
  {
    ImplicitTagRulesSqliteReader reader;
    reader.open("test-files/io/ImplicitTagRulesSqliteReaderTest/rules.db");

    Tags tags = reader.getImplicitTags("Mosque");
    CPPUNIT_ASSERT_EQUAL(2, tags.size());
    CPPUNIT_ASSERT(tags["amenity"] == "place_of_worship");
    CPPUNIT_ASSERT(tags["leisure"] == "park");

    tags = reader.getImplicitTags("MOSQUE");
    CPPUNIT_ASSERT_EQUAL(2, tags.size());
    CPPUNIT_ASSERT(tags["amenity"] == "place_of_worship");
    CPPUNIT_ASSERT(tags["leisure"] == "park");

    tags = reader.getImplicitTags("mosque");
    CPPUNIT_ASSERT_EQUAL(2, tags.size());
    CPPUNIT_ASSERT(tags["amenity"] == "place_of_worship");
    CPPUNIT_ASSERT(tags["leisure"] == "park");

    tags = reader.getImplicitTags("Eid Prayer Ground");
    CPPUNIT_ASSERT_EQUAL(1, tags.size());
    CPPUNIT_ASSERT(tags["amenity"] == "place_of_worship");

    tags = reader.getImplicitTags("Mustashfa");
    CPPUNIT_ASSERT_EQUAL(1, tags.size());
    CPPUNIT_ASSERT(tags["amenity"] == "hospital");

    tags = reader.getImplicitTags("alwhdt");
    CPPUNIT_ASSERT_EQUAL(1, tags.size());
    CPPUNIT_ASSERT(tags["amenity"] == "clinic");

    tags = reader.getImplicitTags("Mustashfa alwhdt");
    CPPUNIT_ASSERT_EQUAL(1, tags.size());
    CPPUNIT_ASSERT(tags["amenity"] == "hospital");

    reader.close();
  }

  void runMultipleRulesTest()
  {
    ImplicitTagRulesSqliteReader reader;
    reader.open("test-files/io/ImplicitTagRulesSqliteReaderTest/rules.db");

    QSet<QString> words;

    words.insert("Mosque");
    CPPUNIT_ASSERT(!reader.wordsInvolveMultipleRules(words));
    words.clear();

    words.insert("MOSQUE");
    CPPUNIT_ASSERT(!reader.wordsInvolveMultipleRules(words));
    words.clear();

    words.insert("mosque");
    CPPUNIT_ASSERT(!reader.wordsInvolveMultipleRules(words));
    words.clear();

    words.insert("Mustashfa");
    CPPUNIT_ASSERT(!reader.wordsInvolveMultipleRules(words));
    words.clear();

    words.insert("MUSTASHFA");
    CPPUNIT_ASSERT(!reader.wordsInvolveMultipleRules(words));
    words.clear();

    words.insert("mustashfa");
    CPPUNIT_ASSERT(!reader.wordsInvolveMultipleRules(words));
    words.clear();

    words.insert("mosque");
    words.insert("Mustashfa");
    CPPUNIT_ASSERT(reader.wordsInvolveMultipleRules(words)); //fix
    words.clear();

    words.insert("alwhdt");
    CPPUNIT_ASSERT(!reader.wordsInvolveMultipleRules(words));
    words.clear();

    words.insert("Mustashfa");
    words.insert("alwhdt");
    CPPUNIT_ASSERT(reader.wordsInvolveMultipleRules(words));
    words.clear();

    words.insert("Mustashfa alwhdt");
    CPPUNIT_ASSERT(!reader.wordsInvolveMultipleRules(words));
    words.clear();

    words.insert("Mustashfa alwhdt");
    words.insert("Mustashfa");
    words.insert("alwhdt");
    CPPUNIT_ASSERT(reader.wordsInvolveMultipleRules(words));
    words.clear();

    words.insert("MUSTASHFA ALWHDT");
    words.insert("MUSTASHFA");
    words.insert("ALWHDT");
    CPPUNIT_ASSERT(reader.wordsInvolveMultipleRules(words)); //fix
    words.clear();

    words.insert("mustashfa alwhdt");
    words.insert("mustashfa");
    words.insert("alwhdt");
    CPPUNIT_ASSERT(reader.wordsInvolveMultipleRules(words)); //fix
    words.clear();

    reader.close();
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ImplicitTagRulesSqliteReaderTest, "quick");

}
