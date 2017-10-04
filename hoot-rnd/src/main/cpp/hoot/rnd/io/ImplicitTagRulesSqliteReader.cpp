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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "ImplicitTagRulesSqliteReader.h"

// hoot
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>

// Qt
#include <QSqlError>
#include <QVariant>
#include <QSet>

namespace hoot
{

ImplicitTagRulesSqliteReader::ImplicitTagRulesSqliteReader()
{
}

ImplicitTagRulesSqliteReader::~ImplicitTagRulesSqliteReader()
{
  close();
}

void ImplicitTagRulesSqliteReader::open(const QString path)
{
  if (!QSqlDatabase::contains(path))
  {
    _db = QSqlDatabase::addDatabase("QSQLITE", path);
    _db.setDatabaseName(path);
    if (!_db.open())
    {
      throw HootException("Error opening DB. " + path);
    }
  }
  else
  {
    _db = QSqlDatabase::database(path);
  }

  if (!_db.isOpen())
  {
    throw HootException("Error DB is not open. " + path);
  }

  _prepareQueries();
}

void ImplicitTagRulesSqliteReader::close()
{
  if (_db.isOpen())
  {
    _db.close();
  }
}

void ImplicitTagRulesSqliteReader::_prepareQueries()
{
  _selectWordIdsForWord = QSqlQuery(_db);
  if (!_selectWordIdsForWord.prepare("SELECT id FROM words WHERE UPPER(word) = UPPER(:word)"))
  {
    throw HootException(
      QString("Error preparing query: %1").arg(_selectWordIdsForWord.lastError().text()));
  }
}

bool ImplicitTagRulesSqliteReader::wordsInvolveMultipleRules(const QSet<QString>& words)
{
  LOG_TRACE("Determining if words: " << words << " involves multiple implicit tag rules...");

  if (words.size() == 0)
  {
    LOG_TRACE("No words specified.");
    return false;
  }

  //can't prepare this one due to variable inputs
  QSqlQuery selectWordIdsForWords(_db);
  //the WHERE IN clause is case sensitive, so or'ing them together instead
  QString queryStr = "SELECT id FROM words WHERE ";
  for (QSet<QString>::const_iterator wordItr = words.begin(); wordItr != words.end(); ++wordItr)
  {
    queryStr += "UPPER(word)='" + (*wordItr).toUpper() + "' OR ";
  }
  queryStr.chop(4);
  LOG_VART(queryStr);
  if (!selectWordIdsForWords.exec(queryStr))
  {
    throw HootException(
      QString("Error executing query: %1").arg(selectWordIdsForWords.lastError().text()));
  }

  //can't prepare this one due to variable inputs
  QSqlQuery uniqueRuleCountForWords(_db);
  queryStr = "SELECT DISTINCT rule_id FROM rules WHERE word_id IN (";
  int wordIdCount = 0;
  while (selectWordIdsForWords.next())
  {
    queryStr += QString::number(selectWordIdsForWords.value(0).toInt()) + ",";
    wordIdCount++;
  }
  queryStr.chop(1);
  queryStr += ")";
  LOG_VART(queryStr);
  if (wordIdCount == 0)
  {
    LOG_TRACE("Found no word IDs for words: " << words);
    return false;
  }
  if (!uniqueRuleCountForWords.exec(queryStr))
  {
    throw HootException(
      QString("Error executing query: %1").arg(uniqueRuleCountForWords.lastError().text()));
  }
  //TODO: make better
  int ruleIdCount = 0;
  while (uniqueRuleCountForWords.next())
  {
    ruleIdCount++;
  }
  LOG_VART(ruleIdCount);
  return ruleIdCount > 1;
}

Tags ImplicitTagRulesSqliteReader::getImplicitTags(const QString word)
{
  LOG_TRACE("Retrieving implicit tags for: " << word << "...");

  _selectWordIdsForWord.bindValue(":word", word);
  if (!_selectWordIdsForWord.exec())
  {
    throw HootException(
      QString("Error executing query: %1").arg(_selectWordIdsForWord.lastError().text()));
  }

  //can't prepare this one due to variable inputs
  QSqlQuery selectTagIdsForWordIds(_db);
  QString query = "SELECT DISTINCT tag_id FROM rules WHERE word_id IN (";
  int numWordIds = 0;
  while (_selectWordIdsForWord.next())
  {
    query += QString::number(_selectWordIdsForWord.value(0).toInt()) + ",";
    numWordIds++;
  }
  query.chop(1);
  query += ")";
  if (numWordIds == 0)
  {
    LOG_TRACE("No associated tag IDs found for word: " << word);
    return Tags();
  }
  LOG_VART(query);
  if (!selectTagIdsForWordIds.exec(query))
  {
    throw HootException(
      QString("Error executing query: %1").arg(selectTagIdsForWordIds.lastError().text()));
  }

  //can't prepare this one due to variable inputs
  QSqlQuery selectTagsForTagIds(_db);
  query = "SELECT kvp FROM tags where id IN (";
  int numTagIds = 0;
  while (selectTagIdsForWordIds.next())
  {
    query += selectTagIdsForWordIds.value(0).toString() + ",";
    numTagIds++;
  }
  query.chop(1);
  query += ")";
  if (numTagIds == 0)
  {
    LOG_TRACE("No associated tags found for word: " << word);
    return Tags();
  }
  LOG_VART(query);
  if (!selectTagsForTagIds.exec(query))
  {
    throw HootException(
      QString("Error executing query: %1").arg(selectTagsForTagIds.lastError().text()));
  }

  Tags tags;
  while (selectTagsForTagIds.next())
  {
    tags.appendValue(selectTagsForTagIds.value(0).toString());
  }
  LOG_VART(tags);
  return tags;
}

}
