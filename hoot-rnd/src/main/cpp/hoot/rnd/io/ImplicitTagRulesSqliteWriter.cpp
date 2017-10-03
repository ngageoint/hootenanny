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

// Qt
#include <QSqlError>
#include <QFile>

namespace hoot
{

ImplicitTagRulesSqliteWriter::ImplicitTagRulesSqliteWriter() :
_currentRuleId(1)
{
}

ImplicitTagRulesSqliteWriter::~ImplicitTagRulesSqliteWriter()
{
  close();
}

void ImplicitTagRulesSqliteWriter::open(const QString output)
{
  QFile outputFile(output);
  if (outputFile.exists() && !outputFile.remove())
  {
    throw HootException(QObject::tr("Error removing existing %1 for writing.").arg(output));
  }

  _db = QSqlDatabase::addDatabase("QSQLITE");
  _db.setDatabaseName(output);
  if (!_db.open())
  {
    throw HootException("Error opening database: " + output);
  }
  LOG_DEBUG("Opened: " << output << ".");

  _createTables();
  _prepareQueries();
}

void ImplicitTagRulesSqliteWriter::_createTables()
{
  DbUtils::execNoPrepare(_db, "CREATE TABLE words (id INTEGER PRIMARY KEY, word TEXT)");
  DbUtils::execNoPrepare(_db, "CREATE TABLE tags (id INTEGER PRIMARY KEY, kvp TEXT)");
  DbUtils::execNoPrepare(
    _db,
    QString("CREATE TABLE rules (id INTEGER PRIMARY KEY, rule_id INTEGER, word_id INTEGER, ") +
    QString("tag_id INTEGER, count INT, FOREIGN KEY(word_id) REFERENCES words(id), ") +
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

  _selectTagIdForKvpQuery = QSqlQuery(_db);
  if (!_selectTagIdForKvpQuery.prepare("SELECT id FROM tags WHERE kvp = :kvp"))
  {
    throw HootException(
      QString("Error preparing query: %1").arg(_selectTagIdForKvpQuery.lastError().text()));
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

  _numTagsForRuleQuery = QSqlQuery(_db);
  if (!_numTagsForRuleQuery.prepare("SELECT tag_id FROM rules where rule_id = :ruleId"))
  {
    throw HootException(
      QString("Error preparing query: %1").arg(_getLastTagIdQuery.lastError().text()));
  }
}

bool caseInsensitiveLessThan2(const QString s1, const QString s2)
{
  return s1.toLower() < s2.toLower();
}

void ImplicitTagRulesSqliteWriter::write(QMap<QString, QMap<QString, long> > rules)
{
  DbUtils::execNoPrepare(_db, "BEGIN");

  //sort rules alphabetically by word case insensitively (QMap sorts them case sensitively by
  //default)
  QStringList words = rules.keys();
  qSort(words.begin(), words.end(), caseInsensitiveLessThan2);

  for (int i = 0; i < words.size(); i++)
  {
    const QString word = words.at(i);
    //always insert word (incoming words are all unique)
    const int wordId = _insertWord(word);

    //kvps auto-sorted alphabetically by tag
    QMap<QString, long> kvps = rules[word];

    //query for the set of rule id's associated with all of the tags
    const QSet<int> ruleIds = _selectRuleIdsContainingAllTags(kvps.keys().toSet());
    int ruleId = -1;
    if (ruleIds.size() == 0)
    {
      //if the rule set is empty insert multiple rule records with a new rule id associated with the
      //word for each tag
      ruleId = _currentRuleId;
      _currentRuleId++;
    }
    else if (ruleIds.size() == 1)
    {
      //if the rule set size = 1 insert multiple rule records with the existing rule id associated
      //with the word for each tag
      ruleId = ruleIds.toList().at(0);
    }
    else
    {
      //should never be more than one rule with the same tag set
      assert(false);
    }

    for (QMap<QString, long>::const_iterator kvpItr = kvps.begin(); kvpItr != kvps.end(); ++kvpItr)
    {
      const QString kvp = kvpItr.key();

      //if tag doesn't exist, insert tag
      int tagId = _selectTagIdForKvp(kvp);
      if (tagId == -1)
      {
        tagId = _insertTag(kvp);
      }

      _insertRuleRecord(ruleId, wordId, tagId);
    }
  }

  DbUtils::execNoPrepare(_db, "COMMIT");
}

int ImplicitTagRulesSqliteWriter::_selectTagIdForKvp(const QString kvp)
{
  //TODO: make this simpler with a join or having clause?

  LOG_TRACE("Selecting tag ID for: " << kvp << "...");
  _selectTagIdForKvpQuery.bindValue(":kvp", kvp);
  if (!_selectTagIdForKvpQuery.exec())
  {
    QString err =
      QString("Error executing query: %1 (%2)").arg(_selectTagIdForKvpQuery.executedQuery()).
        arg(_selectTagIdForKvpQuery.lastError().text());
    throw HootException(err);
  }

  bool ok = false;
  int id = -1;
  if (_selectTagIdForKvpQuery.next())
  {
    id = _selectTagIdForKvpQuery.value(0).toInt(&ok);
  }
  if (!ok || id == -1)
  {
    LOG_TRACE("No tag ID found for kvp: " << kvp);
    return -1;
  }
  LOG_TRACE("Selected tag ID: " << id << " for " << kvp);
  return id;
}

int ImplicitTagRulesSqliteWriter::_insertWord(const QString word)
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
  int id = -1;
  if (_getLastWordIdQuery.next())
  {
    id = _getLastWordIdQuery.value(0).toInt(&ok);
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

int ImplicitTagRulesSqliteWriter::_insertTag(const QString kvp)
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
  int id = -1;
  if (_getLastTagIdQuery.next())
  {
    id = _getLastTagIdQuery.value(0).toInt(&ok);
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

void ImplicitTagRulesSqliteWriter::_insertRuleRecord(const int ruleId, const int wordId,
                                                     const int tagId)
{
  LOG_TRACE(
    "Inserting rule record with ID: " << ruleId << ", word ID: " << wordId << " and tag ID: " <<
    tagId << "...");

  _insertRuleQuery.bindValue(":ruleId", ruleId);
  _insertRuleQuery.bindValue(":wordId", wordId);
  _insertRuleQuery.bindValue(":tagId", tagId);
  if (!_insertRuleQuery.exec())
  {
    QString err = QString("Error executing query: %1 (%2)").arg(_insertRuleQuery.executedQuery()).
        arg(_insertRuleQuery.lastError().text());
    throw HootException(err);
  }
}

QSet<int> ImplicitTagRulesSqliteWriter::_selectTagIdsForKvps(const QSet<QString>& kvps)
{
  LOG_TRACE("Selecting tag IDs for kvps: " << kvps);

  //can't prepare this one due to variable inputs
  QSqlQuery selectTagIdsForTagsQuery = QSqlQuery(_db);
  QString queryStr = "SELECT id FROM tags WHERE kvp IN (";
  for (QSet<QString>::const_iterator kvpItr = kvps.begin(); kvpItr != kvps.end(); ++kvpItr)
  {
    queryStr += "'" + *kvpItr + "',";
  }
  queryStr.chop(1);
  queryStr += ")";
  LOG_VART(queryStr);

  if (!selectTagIdsForTagsQuery.exec(queryStr))
  {
    throw HootException(
      QString("Error preparing query: %1").arg(selectTagIdsForTagsQuery.lastError().text()));
  }

  QSet<int> tagIds;
  while (selectTagIdsForTagsQuery.next())
  {
    tagIds.insert(selectTagIdsForTagsQuery.value(0).toInt());
  }
  LOG_TRACE("Selected tag IDs: " << tagIds << " for kvps: " << kvps);
  return tagIds;
}

int ImplicitTagRulesSqliteWriter::_getNumTagsForRule(const int ruleId)
{
  _numTagsForRuleQuery.bindValue(":ruleId", ruleId);
  if (!_numTagsForRuleQuery.exec())
  {
    QString err =
      QString("Error executing query: %1 (%2)").arg(_numTagsForRuleQuery.executedQuery()).
        arg(_numTagsForRuleQuery.lastError().text());
    throw HootException(err);
  }
  //TODO: make better
  int tagCtr = 0;
  while (_numTagsForRuleQuery.next())
  {
    tagCtr++;
  }
  return tagCtr;
}

QSet<int> ImplicitTagRulesSqliteWriter::_selectRuleIdsContainingAllTags(const QSet<QString>& kvps)
{
  LOG_TRACE("Selecting rules containing all kvps: " << kvps);

  if (kvps.size() == 0)
  {
    LOG_TRACE("Empty set of kvps.");
    return QSet<int>();
  }

  const QSet<int> tagIds = _selectTagIdsForKvps(kvps);
  if (tagIds.size() == 0 || tagIds.size() != kvps.size())
  {
    LOG_TRACE("No rules found containing all kvps: " << kvps);
    return QSet<int>();
  }

  //There is probably a better way to do this...this returns all rules which have any of the
  //specified tags.  This narrows down the rules but obviously isn't complete.
  QSqlQuery selectRuleIdsForTagsQuery = QSqlQuery(_db);
  QString queryStr = "SELECT rule_id, tag_id FROM rules WHERE tag_id IN (";
  for (QSet<int>::const_iterator tagIdItr = tagIds.begin(); tagIdItr != tagIds.end(); ++tagIdItr)
  {
    queryStr += QString::number(*tagIdItr) + ",";
  }
  queryStr.chop(1);
  //This doesn't work
  //queryStr +=
    //") GROUP BY rule_id HAVING COUNT(DISTINCT tag_id) = " + QString::number(kvps.size());
  queryStr += ")";
  LOG_VART(queryStr);

  //can't prepare this one due to variable inputs
  if (!selectRuleIdsForTagsQuery.exec(queryStr))
  {
    throw HootException(
      QString("Error preparing query: %1").arg(selectRuleIdsForTagsQuery.lastError().text()));
  }

  //Now, filter out all rules whose number of tags doesn't match the input tag set.
  QMap<int, QSet<int> > ruleIdsToTagIds;
  while (selectRuleIdsForTagsQuery.next())
  {
    const int ruleId = selectRuleIdsForTagsQuery.value(0).toInt();
    LOG_VART(ruleId);
    const int tagId = selectRuleIdsForTagsQuery.value(1).toInt();
    LOG_VART(tagId);
    const int numTagsForRule = _getNumTagsForRule(ruleId);
    LOG_VART(numTagsForRule);
    LOG_VART(kvps.size());
    if (numTagsForRule == kvps.size())
    {
      if (!ruleIdsToTagIds.contains(ruleId))
      {
        ruleIdsToTagIds[ruleId] = QSet<int>();
      }
      ruleIdsToTagIds[ruleId].insert(tagId);
    }
  }
  LOG_VART(ruleIdsToTagIds);

  //Now, filter down to just the rules that have a tag set that matches the input tags set.
  QSet<int> ruleIds;
  for (QMap<int, QSet<int> >::const_iterator ruleItr = ruleIdsToTagIds.begin();
       ruleItr != ruleIdsToTagIds.end(); ++ruleItr)
  {
    QSet<int> queriedTagIds = ruleItr.value();
    LOG_VART(ruleItr.key());
    LOG_VART(queriedTagIds);
    LOG_VART(tagIds);
    if (queriedTagIds == tagIds)
    {
      ruleIds.insert(ruleItr.key());
    }
  }

  LOG_TRACE("Selected rules: " << ruleIds << " containing all kvps: " << kvps);
  return ruleIds;
}

void ImplicitTagRulesSqliteWriter::close()
{
  if (_db.open())
  {
    _db.close();
    _currentRuleId = 1;
  }
}

}
