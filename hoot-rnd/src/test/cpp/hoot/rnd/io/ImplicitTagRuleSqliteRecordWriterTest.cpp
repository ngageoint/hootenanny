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
#include <hoot/rnd/io/ImplicitTagRuleSqliteRecordWriter.h>
#include <hoot/rnd/schema/ImplicitTagRule.h>

// Qt
#include <QDir>

namespace hoot
{

class ImplicitTagRuleSqliteRecordWriterTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(ImplicitTagRuleSqliteRecordWriterTest);
  CPPUNIT_TEST(runWriteTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runWriteTest()
  {
    const QString outputDir = "test-output/io/ImplicitTagRuleSqliteRecordWriter";
    const QString outputFile = outputDir + "/rules-out.sqlite";
    QDir().mkpath(outputDir);

    ImplicitTagRules rules;

    //rule #1
    rules.append(ImplicitTagRulePtr(new ImplicitTagRule()));
    rules.back()->getWords().insert(QString::fromUtf8("Mosque")); //word #1
    rules.back()->getTags().appendValue("amenity=place_of_worship"); //tag #1
    rules.back()->getTags().appendValue("leisure=park"); //tag #2

    //rule #2
    rules.append(ImplicitTagRulePtr(new ImplicitTagRule()));
    rules.back()->getWords().insert(QString::fromUtf8("Masjid")); //word #2
    rules.back()->getTags().appendValue("amenity=place_of_worship"); //tag #1
    rules.back()->getTags().appendValue("tourism=hotel"); //tag #3

    //rule #3
    rules.append(ImplicitTagRulePtr(new ImplicitTagRule()));
    rules.back()->getWords().insert(QString::fromUtf8("Sidi Muhammad")); //word #3
    rules.back()->getTags().appendValue("amenity=grave_yard"); //tag #4

    //rule #4
    rules.append(ImplicitTagRulePtr(new ImplicitTagRule()));
    rules.back()->getWords().insert(QString::fromUtf8("Eid Prayer Ground")); //word #4
    rules.back()->getWords().insert(QString::fromUtf8("Eid Prayer Ground 2")); //word #5
    rules.back()->getTags().appendValue("amenity=place_of_worship"); //tag #1

    //rule #5
    rules.append(ImplicitTagRulePtr(new ImplicitTagRule()));
    rules.back()->getWords().insert(QString::fromUtf8("Mustashfa")); //word #6
    rules.back()->getWords().insert(QString::fromUtf8("Mustashfa alwhdt")); //word #7
    rules.back()->getTags().appendValue("amenity=hospital"); //tag #5

    //rule #6
    rules.append(ImplicitTagRulePtr(new ImplicitTagRule()));
    rules.back()->getWords().insert(QString::fromUtf8("alwhdt")); //word #8
    rules.back()->getTags().appendValue("amenity=clinic"); //tag #6

    ImplicitTagRuleSqliteRecordWriter writer;
    writer.open(outputFile);
    for (ImplicitTagRules::const_iterator ruleItr = rules.begin(); ruleItr != rules.end();
         ++ruleItr)
    {
      const ImplicitTagRulePtr rule = *ruleItr;
      const QSet<QString> words = rule->getWords();
      const Tags tags = rule->getTags();
      for (QSet<QString>::const_iterator wordItr = words.begin(); wordItr != words.end();
           ++wordItr)
      {
        const QString word = *wordItr;
        for (Tags::const_iterator tagItr = tags.begin(); tagItr != tags.end();
             ++tagItr)
        {
          const QString kvp = tagItr.key() + "=" + tagItr.value();
          writer.write(word, kvp);
        }
      }
    }
    writer.close();

    _openDb(outputFile);

    QSqlQuery query(_db);

//    for debugging
//
//    query.exec("SELECT * FROM rules ORDER BY rule_id");
//    LOG_TRACE("------------------------------");
//    while (query.next())
//    {
//      LOG_VART(query.value(0).toInt());
//      LOG_VART(query.value(1).toInt());
//      LOG_VART(query.value(2).toInt());
//    }
//    query.clear();

    query.exec("SELECT COUNT(*) FROM words");
    query.next();
    const int wordCount = query.value(0).toInt();
    CPPUNIT_ASSERT_EQUAL(8, wordCount);
    query.clear();

    query.exec("SELECT COUNT(*) FROM tags");
    query.next();
    const int tagCount = query.value(0).toInt();
    CPPUNIT_ASSERT_EQUAL(6, tagCount);
    query.clear();

    query.exec("SELECT COUNT(*) FROM rules");
    query.next();
    const int ruleRowCount = query.value(0).toInt();
    CPPUNIT_ASSERT_EQUAL(10, ruleRowCount);
    query.clear();

    query.exec("SELECT DISTINCT rule_id FROM rules");
    int ruleCtr = 0;
    while (query.next())
    {
      ruleCtr++;
    }
    CPPUNIT_ASSERT_EQUAL(6, ruleCtr);
    query.clear();

    query.prepare("SELECT COUNT(*) FROM words where word = :word");
    query.bindValue(":word", "Mosque");
    query.exec();
    query.next();
    const int word1Count = query.value(0).toInt();
    CPPUNIT_ASSERT_EQUAL(1, word1Count);
    query.clear();

    query.prepare("SELECT COUNT(*) FROM tags where kvp = :kvp");
    query.bindValue(":kvp", "amenity=place_of_worship");
    query.exec();
    query.next();
    const int tag1Count = query.value(0).toInt();
    CPPUNIT_ASSERT_EQUAL(1, tag1Count);
    query.clear();

    QSqlQuery ruleCountQuery(_db);
    ruleCountQuery.prepare("SELECT COUNT(*) FROM rules where rule_id = :ruleId");
    QSqlQuery wordIdQuery(_db);
    wordIdQuery.prepare("SELECT DISTINCT word_id FROM rules where rule_id = :ruleId");
    QSqlQuery tagIdQuery(_db);
    tagIdQuery.prepare("SELECT DISTINCT tag_id FROM rules where rule_id = :ruleId");
    for (int i = 1; i < 7; i++)
    {
      ruleCountQuery.bindValue(":ruleId", i);
      ruleCountQuery.exec();
      ruleCountQuery.next();
      const int ruleRowCount = ruleCountQuery.value(0).toInt();
      switch (i)
      {
        case 1:
          CPPUNIT_ASSERT_EQUAL(2, ruleRowCount);
          break;

        case 2:
          CPPUNIT_ASSERT_EQUAL(2, ruleRowCount);
          break;

        case 3:
          CPPUNIT_ASSERT_EQUAL(1, ruleRowCount);
          break;

        case 4:
          CPPUNIT_ASSERT_EQUAL(2, ruleRowCount);
          break;

        case 5:
          CPPUNIT_ASSERT_EQUAL(2, ruleRowCount);
          break;

        case 6:
          CPPUNIT_ASSERT_EQUAL(1, ruleRowCount);
          break;

        default:
          assert(false);
      }

      wordIdQuery.bindValue(":ruleId", i);
      wordIdQuery.exec();
      int wordCtr = 0;
      while (wordIdQuery.next())
      {
        wordCtr++;
      }
      switch (i)
      {
        case 1:
          CPPUNIT_ASSERT_EQUAL(1, wordCtr);
          break;

        case 2:
          CPPUNIT_ASSERT_EQUAL(1, wordCtr);
          break;

        case 3:
          CPPUNIT_ASSERT_EQUAL(1, wordCtr);
          break;

        case 4:
          CPPUNIT_ASSERT_EQUAL(2, wordCtr);
          break;

        case 5:
          CPPUNIT_ASSERT_EQUAL(2, wordCtr);
          break;

        case 6:
          CPPUNIT_ASSERT_EQUAL(1, wordCtr);
          break;

        default:
          assert(false);
      }

      tagIdQuery.bindValue(":ruleId", i);
      tagIdQuery.exec();
      int tagCtr = 0;
      while (tagIdQuery.next())
      {
        tagCtr++;
      }
      switch (i)
      {
        case 1:
          CPPUNIT_ASSERT_EQUAL(2, tagCtr);
          break;

        case 2:
          CPPUNIT_ASSERT_EQUAL(2, tagCtr);
          break;

        case 3:
          CPPUNIT_ASSERT_EQUAL(1, tagCtr);
          break;

        case 4:
          CPPUNIT_ASSERT_EQUAL(1, tagCtr);
          break;

        case 5:
          CPPUNIT_ASSERT_EQUAL(1, tagCtr);
          break;

        case 6:
          CPPUNIT_ASSERT_EQUAL(1, tagCtr);
          break;

        default:
          assert(false);
      }
    }

    _db.close();
  }

private:

  QSqlDatabase _db;

  void _openDb( const QString path)
  {
    if (!QSqlDatabase::contains(path))
    {
      _db = QSqlDatabase::addDatabase("QSQLITE", path);
      _db.setDatabaseName(path);
    }
    else
    {
      _db = QSqlDatabase::database(path);
    }

    if (_db.isOpen())
    {
      return;
    }

    if (!_db.open())
    {
      throw HootException("Error opening database: " + path);
    }
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ImplicitTagRuleSqliteRecordWriterTest, "quick");

}
