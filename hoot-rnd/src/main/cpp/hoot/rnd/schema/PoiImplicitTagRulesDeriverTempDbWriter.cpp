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
#include "PoiImplicitTagRulesDeriverTempDbWriter.h"

// hoot
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/DbUtils.h>
#include <hoot/core/util/Log.h>

// Qt
#include <QSqlError>

namespace hoot
{

PoiImplicitTagRulesDeriverTempDbWriter::PoiImplicitTagRulesDeriverTempDbWriter()
{
  _createTempTables();
  _prepareQueries();
}

PoiImplicitTagRulesDeriverTempDbWriter::~PoiImplicitTagRulesDeriverTempDbWriter()
{
  close();
}

void PoiImplicitTagRulesDeriverTempDbWriter::close()
{
  if (_db.open())
  {
    _db.close();
  }
  if (_tempDbFile)
  {
    _tempDbFile->close();
  }
}

void PoiImplicitTagRulesDeriverTempDbWriter::_createTempTables()
{
  _tempDbFile.reset(
    new QTemporaryFile(
      ConfigOptions().getApidbBulkInserterTempFileDir() +
      "/poi-implicit-tag-rules-deriver-temp-XXXXXX.sqlite"));
  _tempDbFile->setAutoRemove(false); //for debugging only
  if (!_tempDbFile->open())
  {
    throw HootException(
      QObject::tr("Error opening %1 for writing.").arg(_tempDbFile->fileName()));
  }
  LOG_DEBUG("Opened db temp file: " << _tempDbFile->fileName());

  _db = QSqlDatabase::addDatabase("QSQLITE", _tempDbFile->fileName());
  _db.setDatabaseName(_tempDbFile->fileName());
  if (!_db.open())
  {
    throw HootException("Error opening DB. " + _tempDbFile->fileName());
  }
  LOG_DEBUG("Opened: " << _tempDbFile->fileName() << ".");

  DbUtils::execNoPrepare(_db, "CREATE TABLE words (id INTEGER PRIMARY KEY, word TEXT)");
  DbUtils::execNoPrepare(_db, "CREATE TABLE tag_keys (id INTEGER PRIMARY KEY, tag_key TEXT)");
  DbUtils::execNoPrepare(
    _db,
    QString("CREATE TABLE word_tag_keys (id INTEGER PRIMARY KEY, word_id INTEGER, ") +
    QString("tag_key_id INTEGER, tag_count INTEGER, FOREIGN KEY(word_id) REFERENCES words(id), ") +
    QString("FOREIGN KEY(tag_key_id) REFERENCES tag_keys(id))"));
}

void PoiImplicitTagRulesDeriverTempDbWriter::_prepareQueries()
{
  _insertWordQuery = QSqlQuery(_db);
  if (!_insertWordQuery.prepare("INSERT INTO words (word) VALUES(:word)"))
  {
    throw HootException(
      QString("Error preparing _insertWordQuery: %1").arg(_insertWordQuery.lastError().text()));
  }

  _insertTagKeyQuery = QSqlQuery(_db);
  if (!_insertTagKeyQuery.prepare("INSERT INTO tag_keys (tag_key) VALUES(:tagKey)"))
  {
    throw HootException(
      QString("Error preparing _insertTagKeyQuery: %1").arg(_insertTagKeyQuery.lastError().text()));
  }

  _insertWordTagKeyQuery = QSqlQuery(_db);
  if (!_insertWordTagKeyQuery.prepare(
        "INSERT INTO word_tag_keys (word_id, tag_key_id, tag_count) VALUES(:wordId, :tagKeyId, :tagCount)"))
  {
    throw HootException(
      QString("Error preparing _insertWordTagKeyQuery: %1").arg(_insertWordTagKeyQuery.lastError().text()));
  }

  _getWordQuery = QSqlQuery(_db);
  if (!_getWordQuery.prepare("SELECT word FROM words WHERE word LIKE :word"))
  {
    throw HootException(
      QString("Error preparing _getWordQuery: %1").arg(_getWordQuery.lastError().text()));
  }

  _getWordTagKeyCountQuery = QSqlQuery(_db);
  if (!_getWordTagKeyCountQuery.prepare(
        "SELECT tag_count FROM word_tag_keys WHERE word_id = :wordId AND tag_key_id = :tagKeyId"))
  {
    throw HootException(
      QString("Error preparing _getWordTagKeyCountQuery: %1").arg(_getWordTagKeyCountQuery.lastError().text()));
  }

  _getWordIdForWordQuery = QSqlQuery(_db);
  if (!_getWordIdForWordQuery.prepare("SELECT id FROM words WHERE word LIKE :word"))
  {
    throw HootException(
      QString("Error preparing _getWordIdForWordQuery: %1")
        .arg(_getWordIdForWordQuery.lastError().text()));
  }

  _getTagKeyIdForTagKeyQuery = QSqlQuery(_db);
  if (!_getTagKeyIdForTagKeyQuery.prepare("SELECT id FROM tag_keys WHERE tag_key = :tagKey"))
  {
    throw HootException(
      QString("Error preparing _getTagIdForTagQuery: %1")
        .arg(_getTagKeyIdForTagKeyQuery.lastError().text()));
  }

  _getLastWordIdQuery = QSqlQuery(_db);
  if (!_getLastWordIdQuery.prepare("SELECT last_insert_rowid() FROM words"))
  {
    throw HootException(
      QString("Error preparing _getLastWordIdQuery: %1").arg(_getLastWordIdQuery.lastError().text()));
  }

  _getLastTagKeyIdQuery = QSqlQuery(_db);
  if (!_getLastTagKeyIdQuery.prepare("SELECT last_insert_rowid() FROM tag_keys"))
  {
    throw HootException(
      QString("Error preparing _getLastTagKeyIdQuery: %1").arg(_getLastTagKeyIdQuery.lastError().text()));
  }
}

long PoiImplicitTagRulesDeriverTempDbWriter::insertWord(const QString word)
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
  LOG_VART(id);
  LOG_VART(ok);
  if (!ok || id == -1)
  {
    throw HootException(
      "Error retrieving new ID " + _getLastWordIdQuery.lastError().text() + " Query: " +
      _getLastWordIdQuery.executedQuery());
  }

  LOG_TRACE("Word: " << word << " inserted with ID: " << id);
  return id;
}

long PoiImplicitTagRulesDeriverTempDbWriter::insertTagKey(const QString tagKey)
{
  LOG_TRACE("Inserting tag key: " << tagKey << "...");

  _insertTagKeyQuery.bindValue(":tagKey", tagKey);
  if (!_insertTagKeyQuery.exec())
  {
    QString err = QString("Error executing query: %1 (%2)").arg(_insertTagKeyQuery.executedQuery()).
        arg(_insertTagKeyQuery.lastError().text());
    throw HootException(err);
  }

  if (!_getLastTagKeyIdQuery.exec())
  {
    QString err =
      QString("Error executing query: %1 (%2)").arg(_getLastTagKeyIdQuery.executedQuery()).
        arg(_getLastTagKeyIdQuery.lastError().text());
    throw HootException(err);
  }
  bool ok = false;
  long id = -1;
  if (_getLastTagKeyIdQuery.next())
  {
    id = _getLastTagKeyIdQuery.value(0).toLongLong(&ok);
  }
  if (!ok || id == -1)
  {
    throw HootException(
      "Error retrieving new ID " + _getLastTagKeyIdQuery.lastError().text() + " Query: " +
      _getLastTagKeyIdQuery.executedQuery());
  }

  LOG_TRACE("Tag key: " << tagKey << " inserted with ID: " << id);
  return id;
}

void PoiImplicitTagRulesDeriverTempDbWriter::insertWordTagKey(const long wordId, const long tagKeyId,
                                                              const long tagCount)
{
  LOG_TRACE(
    "Inserting word id: " << wordId << ", tag key id: " << tagKeyId << ", tag count: " <<
    tagCount << "...");

  _insertWordTagKeyQuery.bindValue(":wordId", (qlonglong)wordId);
  _insertWordTagKeyQuery.bindValue(":tagKeyId", (qlonglong)tagKeyId);
  _insertWordTagKeyQuery.bindValue(":tagCount", (qlonglong)tagCount);
  if (!_insertWordTagKeyQuery.exec())
  {
    QString err = QString("Error executing query: %1 (%2)").arg(_insertTagKeyQuery.executedQuery()).
        arg(_insertTagKeyQuery.lastError().text());
    throw HootException(err);
  }
}

QString PoiImplicitTagRulesDeriverTempDbWriter::getWord(const QString word)
{
  LOG_TRACE("Retrieving word: " << word << "...");

  _getWordQuery.bindValue(":word", word);
  if (!_getWordQuery.exec())
  {
    QString err = QString("Error executing query: %1 (%2)").arg(_getWordQuery.executedQuery()).
        arg(_getWordQuery.lastError().text());
    throw HootException(err);
  }
  //bool ok = false;
  QString returnWord;
  if (_getWordQuery.next())
  {
    returnWord = _getWordQuery.value(0).toString();
  }
  return returnWord;
}

long PoiImplicitTagRulesDeriverTempDbWriter::getWordTagKeyCount(const long wordId,
                                                                const long tagKeyId)
{
  LOG_TRACE(
    "Retrieving tag count for wordId: " << wordId << " and tag key id: " << tagKeyId << "...");

  _getWordTagKeyCountQuery.bindValue(":wordId", (qlonglong)wordId);
  _getWordTagKeyCountQuery.bindValue(":tagKeyId", (qlonglong)tagKeyId);
  if (!_getWordTagKeyCountQuery.exec())
  {
    QString err = QString("Error executing query: %1 (%2)").arg(_getWordTagKeyCountQuery.executedQuery()).
        arg(_getWordTagKeyCountQuery.lastError().text());
    throw HootException(err);
  }
  //bool ok = false;
  long tagCount = -1;
  if (_getWordTagKeyCountQuery.next())
  {
    tagCount = _getWordTagKeyCountQuery.value(0).toLongLong();
  }
  return tagCount;
}

long PoiImplicitTagRulesDeriverTempDbWriter::getWordIdForWord(const QString word)
{
  LOG_TRACE("Retrieving word ID for word: " << word << "...");

  _getWordIdForWordQuery.bindValue(":word", word);
  if (!_getWordIdForWordQuery.exec())
  {
    QString err =
      QString("Error executing query: %1 (%2)")
        .arg(_getWordIdForWordQuery.executedQuery())
        .arg(_getWordIdForWordQuery.lastError().text());
    throw HootException(err);
  }
  //bool ok = false;
  long id = -1;
  if (_getWordIdForWordQuery.next())
  {
    id = _getWordIdForWordQuery.value(0).toLongLong(/*&ok*/);
  }
  return id;
}

long PoiImplicitTagRulesDeriverTempDbWriter::getTagKeyIdForTagKey(const QString tagKey)
{
  LOG_TRACE("Retrieving tag key ID for tag key: " << tagKey << "...");

  _getTagKeyIdForTagKeyQuery.bindValue(":tagKey", tagKey);
  if (!_getTagKeyIdForTagKeyQuery.exec())
  {
    QString err =
      QString("Error executing query: %1 (%2)")
        .arg(_getTagKeyIdForTagKeyQuery.executedQuery())
        .arg(_getTagKeyIdForTagKeyQuery.lastError().text());
    throw HootException(err);
  }
  //bool ok = false;
  long id = -1;
  if (_getTagKeyIdForTagKeyQuery.next())
  {
    id = _getTagKeyIdForTagKeyQuery.value(0).toLongLong(/*&ok*/);
  }
  return id;
}

}
