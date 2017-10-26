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

bool ImplicitTagRuleSqliteRecordWriter::isSupported(const QString outputUrl)
{
  return outputUrl.endsWith(".db", Qt::CaseInsensitive);
}

void ImplicitTagRuleSqliteRecordWriter::open(const QString outputUrl)
{
  QFile outputFile(outputUrl);
  if (outputFile.exists() && !outputFile.remove())
  {
    throw HootException(QObject::tr("Error removing existing %1 for writing.").arg(outputUrl));
  }
  outputFile.open(QIODevice::WriteOnly);

  if (!QSqlDatabase::contains(outputUrl))
  {
    _db = QSqlDatabase::addDatabase("QSQLITE", outputUrl);
    _db.setDatabaseName(outputUrl);
    if (!_db.open())
    {
      throw HootException("Error opening DB. " + outputUrl);
    }
  }
  else
  {
    _db = QSqlDatabase::database(outputUrl);
  }

  if (!_db.isOpen())
  {
    throw HootException("Error DB is not open. " + outputUrl);
  }
  LOG_DEBUG("Opened: " << outputUrl << ".");

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
}

void ImplicitTagRuleSqliteRecordWriter::write(const QString inputUrl, const long /*totalParts*/)
{
  DbUtils::execNoPrepare(_db, "BEGIN");

  //The input is assumed sorted by word, then by kvp.
  QFile inputFile(inputUrl);
  if (!inputFile.open(QIODevice::ReadOnly))
  {
    throw HootException(QObject::tr("Error opening %1 for reading.").arg(inputUrl));
  }

  while (!inputFile.atEnd())
  {
    const QString line = QString::fromUtf8(inputFile.readLine().constData());
    const QStringList lineParts = line.split("\t");
    const long wordTagOccurranceCount = lineParts[0].toLong();
    const QString word = lineParts[1];
    const QString kvp = lineParts[2];

    long wordId = -1;
    if (_wordsToWordIds.contains(word))
    {
      wordId = _wordsToWordIds[word];
    }
    else
    {
      wordId = _insertWord(word);
      _wordsToWordIds[word] = wordId;
    }

    long tagId = -1;
    if (_tagsToTagIds.contains(kvp))
    {
      tagId = _tagsToTagIds[kvp];
    }
    else
    {
      tagId = _insertTag(kvp);
      _tagsToTagIds[kvp] = tagId;
    }

    long ruleId = -1;
    const QString wordIdTagId = QString::number(wordId) % ";" % QString::number(tagId);
    if (_wordIdTagIdsToRuleIds.contains(wordIdTagId))
    {
      ruleId = _wordIdTagIdsToRuleIds[wordIdTagId];
    }
    else
    {
      ruleId = _currentRuleId;
      _currentRuleId++;
      _wordIdTagIdsToRuleIds[wordIdTagId] = ruleId;
    }

    _insertRuleRecord(ruleId, wordId, tagId, wordTagOccurranceCount);
  }
  inputFile.close();

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
