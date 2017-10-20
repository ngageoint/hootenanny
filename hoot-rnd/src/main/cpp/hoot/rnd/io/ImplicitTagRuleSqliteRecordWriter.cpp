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
#include "ImplicitTagRuleSqliteRecordWriter.h"

// hoot
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/DbUtils.h>
#include <hoot/core/util/Log.h>

// Qt
#include <QSqlError>
#include <QFile>
#include <QStringBuilder>

namespace hoot
{

ImplicitTagRuleSqliteRecordWriter::ImplicitTagRuleSqliteRecordWriter() :
_currentRuleId(1)
{
}

ImplicitTagRuleSqliteRecordWriter::~ImplicitTagRuleSqliteRecordWriter()
{
  close();
}

void ImplicitTagRuleSqliteRecordWriter::open(const QString url)
{
  QFile outputFile(url);
  if (outputFile.exists() && !outputFile.remove())
  {
    throw HootException(QObject::tr("Error removing existing %1 for writing.").arg(url));
  }
  outputFile.open(QIODevice::WriteOnly);

  if (!QSqlDatabase::contains(url))
  {
    _db = QSqlDatabase::addDatabase("QSQLITE", url);
    _db.setDatabaseName(url);
    if (!_db.open())
    {
      throw HootException("Error opening DB. " + url);
    }
  }
  else
  {
    _db = QSqlDatabase::database(url);
  }

  if (!_db.isOpen())
  {
    throw HootException("Error DB is not open. " + url);
  }
  LOG_DEBUG("Opened: " << url << ".");

  _createTables();
  _prepareQueries();

  _currentRuleId = 1;
}

void ImplicitTagRuleSqliteRecordWriter::_createTables()
{
  DbUtils::execNoPrepare(_db, "CREATE TABLE words (id INTEGER PRIMARY KEY, word TEXT)");
  DbUtils::execNoPrepare(_db, "CREATE TABLE tags (id INTEGER PRIMARY KEY, kvp TEXT)");
  DbUtils::execNoPrepare(
    _db,
    QString("CREATE TABLE rules (id INTEGER PRIMARY KEY, rule_id INTEGER, word_id INTEGER, ") +
    QString("tag_id INTEGER, count INTEGER, FOREIGN KEY(word_id) REFERENCES words(id), ") +
    QString("FOREIGN KEY(tag_id) REFERENCES tags(id))"));
}

void ImplicitTagRuleSqliteRecordWriter::_prepareQueries()
{
  _insertWordQuery = QSqlQuery(_db);
  if (!_insertWordQuery.prepare("INSERT INTO words (word) VALUES(:word)"))
  {
    throw HootException(
      QString("Error preparing _insertWordQuery: %1").arg(_insertWordQuery.lastError().text()));
  }

  _insertTagQuery = QSqlQuery(_db);
  if (!_insertTagQuery.prepare("INSERT INTO tags (kvp) VALUES(:kvp)"))
  {
    throw HootException(
      QString("Error preparing _insertTagQuery: %1").arg(_insertTagQuery.lastError().text()));
  }

  _insertRuleQuery = QSqlQuery(_db);
  if (!_insertRuleQuery.prepare(
        "INSERT INTO rules (rule_id, word_id, tag_id) VALUES(:ruleId, :wordId, :tagId)"))
  {
    throw HootException(
      QString("Error preparing _insertRuleQuery: %1").arg(_insertRuleQuery.lastError().text()));
  }

  _getLastWordIdQuery = QSqlQuery(_db);
  if (!_getLastWordIdQuery.prepare("SELECT last_insert_rowid() FROM words"))
  {
    throw HootException(
      QString("Error preparing _getLastWordIdQuery: %1").arg(_getLastWordIdQuery.lastError().text()));
  }

  _getLastTagIdQuery = QSqlQuery(_db);
  if (!_getLastTagIdQuery.prepare("SELECT last_insert_rowid() FROM tags"))
  {
    throw HootException(
      QString("Error preparing _getLastTagIdQuery: %1").arg(_getLastTagIdQuery.lastError().text()));
  }

  _getWordIdForWord = QSqlQuery(_db);
  if (!_getWordIdForWord.prepare("SELECT id FROM words WHERE word LIKE ':word'"))
  {
    throw HootException(
      QString("Error preparing _getWordIdForWord: %1").arg(_getWordIdForWord.lastError().text()));
  }

  _getTagIdForTag = QSqlQuery(_db);
  if (!_getTagIdForTag.prepare("SELECT id FROM tags WHERE kvp LIKE ':kvp'"))
  {
    throw HootException(
      QString("Error preparing _getTagIdForTag: %1").arg(_getTagIdForTag.lastError().text()));
  }

  _getRuleIdForWordKvp = QSqlQuery(_db);
  if (!_getRuleIdForWordKvp.prepare(
        "SELECT DISTINCT rule_id, count FROM rules WHERE word_id = :wordId AND tag_id = :tagId"))
  {
    throw HootException(
      QString("Error preparing _getRuleIdForWordKvp: %1").arg(_getRuleIdForWordKvp.lastError().text()));
  }
}

long ImplicitTagRuleSqliteRecordWriter::_getWordId(const QString word)
{
  _getWordIdForWord.bindValue(":word", word);
  if (!_getWordIdForWord.exec())
  {
    QString err = QString("Error executing query: %1 (%2)").arg(_getWordIdForWord.executedQuery()).
        arg(_getWordIdForWord.lastError().text());
    throw HootException(err);
  }
  if (!_getWordIdForWord.next())
  {
    return false;
  }
  else
  {
    return _getWordIdForWord.value(0).toLongLong();
  }
}

long ImplicitTagRuleSqliteRecordWriter::_getTagId(const QString kvp)
{
  _getTagIdForTag.bindValue(":kvp", kvp);
  if (!_getTagIdForTag.exec())
  {
    QString err = QString("Error executing query: %1 (%2)").arg(_getTagIdForTag.executedQuery()).
        arg(_getTagIdForTag.lastError().text());
    throw HootException(err);
  }
  if (!_getTagIdForTag.next())
  {
    return false;
  }
  else
  {
    return _getTagIdForTag.value(0).toLongLong();
  }
}

long ImplicitTagRuleSqliteRecordWriter::_getRuleId(const long wordId, const long tagId)
{
  _getRuleIdForWordKvp.bindValue(":wordId", (qlonglong)wordId);
  _getRuleIdForWordKvp.bindValue(":tagId", (qlonglong)tagId);
  if (!_getRuleIdForWordKvp.exec())
  {
    QString err = QString("Error executing query: %1 (%2)").arg(_getRuleIdForWordKvp.executedQuery()).
        arg(_getRuleIdForWordKvp.lastError().text());
    throw HootException(err);
  }
  if (!_getRuleIdForWordKvp.next())
  {
    return false;
  }
  else
  {
    //count = _getRuleIdForWordKvp.value(1).toLongLong();
    return _getRuleIdForWordKvp.value(0).toLongLong();
  }
}

void ImplicitTagRuleSqliteRecordWriter::write(const QString word, const QString kvp,
                                              const long wordTagOccurranceCount)
{
  DbUtils::execNoPrepare(_db, "BEGIN");

  bool wordPreviouslyExisted = false;
  long wordId = _getWordId(word);
  if (wordId == -1)
  {
    wordId = _insertWord(word);
  }
  else
  {
    wordPreviouslyExisted = true;
  }

  bool tagPreviouslyExisted = false;
  long tagId = _getTagId(word);
  if (tagId == -1)
  {
    tagId = _insertTag(kvp);
  }
  else
  {
    tagPreviouslyExisted = true;
  }

  long ruleId = -1;
  long ruleCount = -1;
  if (!wordPreviouslyExisted || !tagPreviouslyExisted)
  {
    ruleId = _currentRuleId;
    _currentRuleId++;
  }
  else
  {
    ruleId = _getRuleId(wordId, tagId);
  }
  if (ruleId == -1)
  {
    ruleId = _currentRuleId;;
    _currentRuleId++;
  }
  else
  {
    assert(ruleCount != -1);
    ruleCount++;
  }
  _insertRuleRecord(ruleId, wordId, tagId, wordTagOccurranceCount);

  DbUtils::execNoPrepare(_db, "COMMIT");

  //TODO: add text indexes on words and tags after writing
}

long ImplicitTagRuleSqliteRecordWriter::_insertWord(const QString word)
{
  LOG_TRACE("Inserting word: " << word << "...");

  _insertWordQuery.bindValue(":word", word);
  if (!_insertWordQuery.exec())
  {
    QString err = QString("Error executing query: %1 (%2)").arg(_insertWordQuery.executedQuery()).
        arg(_insertWordQuery.lastError().text());
    throw HootException(err);
  }

  if (!_getLastWordIdQuery.exec())
  {
    QString err =
      QString("Error executing query: %1 (%2)").arg(_getLastWordIdQuery.executedQuery()).
        arg(_getLastWordIdQuery.lastError().text());
    throw HootException(err);
  }

  bool ok = false;
  long id = -1;
  if (_getLastWordIdQuery.next())
  {
    id = _getLastWordIdQuery.value(0).toLongLong(&ok);
  }
  if (!ok || id == -1)
  {
    throw HootException(
      "Error retrieving new ID " + _getLastWordIdQuery.lastError().text() + " Query: " +
      _getLastWordIdQuery.executedQuery());
  }

  LOG_TRACE("Word: " << word << " inserted with ID: " << id);
  return id;
}

long ImplicitTagRuleSqliteRecordWriter::_insertTag(const QString kvp)
{
  LOG_TRACE("Inserting tag: " << kvp << "...");

  _insertTagQuery.bindValue(":kvp", kvp);
  if (!_insertTagQuery.exec())
  {
    QString err = QString("Error executing query: %1 (%2)").arg(_insertTagQuery.executedQuery()).
        arg(_insertTagQuery.lastError().text());
    throw HootException(err);
  }

  if (!_getLastTagIdQuery.exec())
  {
    QString err =
      QString("Error executing query: %1 (%2)").arg(_getLastTagIdQuery.executedQuery()).
        arg(_getLastTagIdQuery.lastError().text());
    throw HootException(err);
  }

  bool ok = false;
  long id = -1;
  if (_getLastTagIdQuery.next())
  {
    id = _getLastTagIdQuery.value(0).toLongLong(&ok);
  }
  if (!ok || id == -1)
  {
    throw HootException(
      "Error retrieving new ID " + _getLastTagIdQuery.lastError().text() + " Query: " +
      _getLastTagIdQuery.executedQuery());
  }

  LOG_TRACE("Tag: " << kvp << " inserted with ID: " << id);
  return id;
}

void ImplicitTagRuleSqliteRecordWriter::_insertRuleRecord(const long ruleId, const long wordId,
                                                          const long tagId,
                                                          const long wordTagOccurranceCount)
{
  LOG_TRACE(
    "Inserting rule record with ID: " << ruleId << ", word ID: " << wordId << " and tag ID: " <<
    tagId << "...");

  _insertRuleQuery.bindValue(":ruleId", qlonglong(ruleId));
  _insertRuleQuery.bindValue(":wordId", qlonglong(wordId));
  _insertRuleQuery.bindValue(":tagId", qlonglong(tagId));
  _insertRuleQuery.bindValue(":count", qlonglong(wordTagOccurranceCount));
  if (!_insertRuleQuery.exec())
  {
    QString err = QString("Error executing query: %1 (%2)").arg(_insertRuleQuery.executedQuery()).
        arg(_insertRuleQuery.lastError().text());
    throw HootException(err);
  }
}

void ImplicitTagRuleSqliteRecordWriter::close()
{
  if (_db.open())
  {
    _db.close();
  }
}

}
