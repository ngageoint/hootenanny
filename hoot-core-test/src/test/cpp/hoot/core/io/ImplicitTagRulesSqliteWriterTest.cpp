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
 * @copyright Copyright (C) 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>

#include <hoot/core/io/ImplicitTagRulesSqliteWriter.h>

namespace hoot
{

class ImplicitTagRulesSqliteWriterTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(ImplicitTagRulesSqliteWriterTest);
  CPPUNIT_TEST(runWriteTest);
  CPPUNIT_TEST_SUITE_END();

public:

  ImplicitTagRulesSqliteWriterTest()
    : HootTestFixture("test-files/io/ImplicitTagRulesSqliteWriterTest/",
                      "test-output/io/ImplicitTagRulesSqliteWriterTest/")
  {
  }

  void runWriteTest()
  {
    const QString outputFile = _outputPath + "rules-out.sqlite";

    ImplicitTagRulesSqliteWriter writer;
    writer.open(outputFile);
    writer.write(_inputPath + "ruleWordParts");
    writer.close();

    _openDb(outputFile);

    QSqlQuery query(_db);

//    for debugging
//
//    LOG_TRACE("Querying words...");
//    query.exec("SELECT * FROM words ORDER BY id");
//    LOG_TRACE("------------------------------");
//    while (query.next())
//    {
//      LOG_VART(query.value(0).toInt());
//      LOG_VART(query.value(1).toString());
//    }
//    query.clear();
//    LOG_TRACE("Querying tags...");
//    query.exec("SELECT * FROM tags ORDER BY id");
//    LOG_TRACE("------------------------------");
//    while (query.next())
//    {
//      LOG_VART(query.value(0).toInt());
//      LOG_VART(query.value(1).toString());
//    }
//    query.clear();
//    LOG_TRACE("Querying rules...");
//    query.exec("SELECT * FROM rules ORDER BY word_id");
//    LOG_TRACE("------------------------------");
//    while (query.next())
//    {
//      LOG_VART(query.value(0).toInt());
//      LOG_VART(query.value(1).toInt());
//      LOG_VART(query.value(2).toInt());
//    }
//    query.clear();
//    return;

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

    query.prepare("SELECT COUNT(*) FROM words WHERE word = :word");
    query.bindValue(":word", "Mosque");
    query.exec();
    query.next();
    const int word1Count = query.value(0).toInt();
    CPPUNIT_ASSERT_EQUAL(1, word1Count);
    query.clear();

    query.prepare("SELECT COUNT(*) FROM tags WHERE kvp = :kvp");
    query.bindValue(":kvp", "amenity=place_of_worship");
    query.exec();
    query.next();
    const int tag1Count = query.value(0).toInt();
    CPPUNIT_ASSERT_EQUAL(1, tag1Count);
    query.clear();

    QSqlQuery wordIdQuery(_db);
    wordIdQuery.prepare("SELECT COUNT(*) FROM rules WHERE word_id = :wordId");
    for (int i = 1; i < 9; i++)
    {
      wordIdQuery.bindValue(":wordId", i);
      wordIdQuery.exec();
      wordIdQuery.next();
      int wordCount = wordIdQuery.value(0).toInt();
      switch (i)
      {
        case 1:
          CPPUNIT_ASSERT_EQUAL(1, wordCount);
          break;

        case 2:
          CPPUNIT_ASSERT_EQUAL(1, wordCount);
          break;

        case 3:
          CPPUNIT_ASSERT_EQUAL(1, wordCount);
          break;

        case 4:
          CPPUNIT_ASSERT_EQUAL(2, wordCount);
          break;

        case 5:
          CPPUNIT_ASSERT_EQUAL(2, wordCount);
          break;

        case 6:
          CPPUNIT_ASSERT_EQUAL(1, wordCount);
          break;

        case 7:
          CPPUNIT_ASSERT_EQUAL(1, wordCount);
          break;

        case 8:
          CPPUNIT_ASSERT_EQUAL(1, wordCount);
          break;

        default:
          assert(false);
      }
    }

    QSqlQuery tagIdQuery(_db);
    tagIdQuery.prepare("SELECT COUNT(*) FROM rules WHERE tag_id = :tagId");
    for (int i = 1; i < 7; i++)
    {
      tagIdQuery.bindValue(":tagId", i);
      tagIdQuery.exec();
      tagIdQuery.next();
      int tagCount = tagIdQuery.value(0).toInt();
      switch (i)
      {
        case 1:
          CPPUNIT_ASSERT_EQUAL(1, tagCount);
          break;

        case 2:
          CPPUNIT_ASSERT_EQUAL(4, tagCount);
          break;

        case 3:
          CPPUNIT_ASSERT_EQUAL(1, tagCount);
          break;

        case 4:
          CPPUNIT_ASSERT_EQUAL(1, tagCount);
          break;

        case 5:
          CPPUNIT_ASSERT_EQUAL(2, tagCount);
          break;

        case 6:
          CPPUNIT_ASSERT_EQUAL(1, tagCount);
          break;

        default:
          assert(false);
      }
    }

    _db.close();
  }

private:

  QSqlDatabase _db;

  void _openDb(const QString& path)
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

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ImplicitTagRulesSqliteWriterTest, "quick");

}
