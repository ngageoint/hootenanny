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
#include "ImplicitTagRulesSqliteWriter.h"

// hoot
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/DbUtils.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/Factory.h>

// Qt
#include <QSqlError>
#include <QFile>
#include <QStringBuilder>

namespace hoot
{

HOOT_FACTORY_REGISTER(ImplicitTagRulesWriter, ImplicitTagRulesSqliteWriter)

ImplicitTagRulesSqliteWriter::ImplicitTagRulesSqliteWriter()
{
}

ImplicitTagRulesSqliteWriter::~ImplicitTagRulesSqliteWriter()
{
  close();
}

bool ImplicitTagRulesSqliteWriter::isSupported(const QString url)
{
  return url.endsWith(".db", Qt::CaseInsensitive);
}

void ImplicitTagRulesSqliteWriter::open(const QString url)
{
  QFile outputFile(url);
  if (outputFile.exists() && !outputFile.remove())
  {
    throw HootException(QObject::tr("Error removing existing %1 for writing.").arg(url));
  }

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
}

void ImplicitTagRulesSqliteWriter::_createTables()
{
  //TODO: add text indexes on words and tags
  DbUtils::execNoPrepare(_db, "CREATE TABLE words (id INTEGER PRIMARY KEY, word TEXT)");
  DbUtils::execNoPrepare(_db, "CREATE TABLE tags (id INTEGER PRIMARY KEY, kvp TEXT)");
  DbUtils::execNoPrepare(
    _db,
    QString("CREATE TABLE rules (id INTEGER PRIMARY KEY, rule_id INTEGER, word_id INTEGER, ") +
    QString("tag_id INTEGER, FOREIGN KEY(word_id) REFERENCES words(id), ") +
    QString("FOREIGN KEY(tag_id) REFERENCES tags(id))"));
}

void ImplicitTagRulesSqliteWriter::_prepareQueries()
{
  _insertWordQuery = QSqlQuery(_db);
  if (!_insertWordQuery.prepare("INSERT INTO words (word) VALUES(:word)"))
  {
    throw HootException(
      QString("Error preparing query: %1").arg(_insertWordQuery.lastError().text()));
  }

  _insertTagQuery = QSqlQuery(_db);
  if (!_insertTagQuery.prepare("INSERT INTO tags (kvp) VALUES(:kvp)"))
  {
    throw HootException(
      QString("Error preparing query: %1").arg(_insertTagQuery.lastError().text()));
  }

  _insertRuleQuery = QSqlQuery(_db);
  if (!_insertRuleQuery.prepare(
        "INSERT INTO rules (rule_id, word_id, tag_id) VALUES(:ruleId, :wordId, :tagId)"))
  {
    throw HootException(
      QString("Error preparing query: %1").arg(_insertRuleQuery.lastError().text()));
  }

  _getLastWordIdQuery = QSqlQuery(_db);
  if (!_getLastWordIdQuery.prepare("SELECT last_insert_rowid() FROM words"))
  {
    throw HootException(
      QString("Error preparing query: %1").arg(_getLastWordIdQuery.lastError().text()));
  }

  _getLastTagIdQuery = QSqlQuery(_db);
  if (!_getLastTagIdQuery.prepare("SELECT last_insert_rowid() FROM tags"))
  {
    throw HootException(
      QString("Error preparing query: %1").arg(_getLastTagIdQuery.lastError().text()));
  }
}

void ImplicitTagRulesSqliteWriter::write(const ImplicitTagRules& rules)
{
  DbUtils::execNoPrepare(_db, "BEGIN");

  QMap<QString, long> kvpsToTagIds;
  int ruleIdCtr = 1;
  for (ImplicitTagRules::const_iterator rulesItr = rules.begin(); rulesItr != rules.end();
       ++rulesItr)
  {
    ImplicitTagRulePtr rule = *rulesItr;

    //Words per rule are all unique, so insert them every time.
    const QStringList words = rule->getWords();
    QList<long> wordIds;
    for (int i = 0; i < words.size(); i++)
    {
      wordIds.append(_insertWord(words.at(i)));
    }

    //Each tag grouping per rule is unique, but each tag is not.  So, check to see if a tag
    //record has already been inserted each time.
    const Tags tags = rule->getTags();
    QList<long> tagIds;
    for (Tags::const_iterator tagItr = tags.begin(); tagItr != tags.end(); ++tagItr)
    {
      const QString kvp = tagItr.key() % "=" % tagItr.value();
      int tagId = -1;
      if (!kvpsToTagIds.contains(kvp))
      {
        tagId = _insertTag(kvp);
        kvpsToTagIds[kvp] = tagId;
      }
      else
      {
        tagId = kvpsToTagIds[kvp];
      }
      tagIds.append(tagId);
    }

    for (int i = 0; i < wordIds.size(); i++)
    {
      for (int j = 0; j < tagIds.size(); j++)
      {
        _insertRuleRecord(ruleIdCtr, wordIds.at(i), tagIds.at(j));
      }
    }
    ruleIdCtr++;
  }

  DbUtils::execNoPrepare(_db, "COMMIT");
}

void ImplicitTagRulesSqliteWriter::write(const ImplicitTagRulesByWord& /*rules*/)
{
  LOG_DEBUG(
    "The writing of implicit tag rules to Sqlite may only be done when the input is a list of rules.");
}

long ImplicitTagRulesSqliteWriter::_insertWord(const QString word)
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

long ImplicitTagRulesSqliteWriter::_insertTag(const QString kvp)
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

void ImplicitTagRulesSqliteWriter::_insertRuleRecord(const long ruleId, const long wordId,
                                                     const long tagId)
{
  LOG_TRACE(
    "Inserting rule record with ID: " << ruleId << ", word ID: " << wordId << " and tag ID: " <<
    tagId << "...");

  _insertRuleQuery.bindValue(":ruleId", qlonglong(ruleId));
  _insertRuleQuery.bindValue(":wordId", qlonglong(wordId));
  _insertRuleQuery.bindValue(":tagId", qlonglong(tagId));
  if (!_insertRuleQuery.exec())
  {
    QString err = QString("Error executing query: %1 (%2)").arg(_insertRuleQuery.executedQuery()).
        arg(_insertRuleQuery.lastError().text());
    throw HootException(err);
  }
}

void ImplicitTagRulesSqliteWriter::close()
{
  if (_db.open())
  {
    _db.close();
  }
}

}
