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

void ImplicitTagRulesSqliteReader::open(const QString url)
{
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
  _ruleCountQuery = QSqlQuery(_db);
  if (!_ruleCountQuery.prepare("SELECT COUNT(*) FROM rules"))
  {
    throw HootException(
      QString("Error preparing query: %1").arg(_ruleCountQuery.lastError().text()));
  }
}

bool ImplicitTagRulesSqliteReader::wordsInvolveMultipleRules(const QSet<QString>& words,
                                                             QSet<QString>& matchingRuleWords)
{
  LOG_TRACE("Determining if words: " << words << " involve multiple implicit tag rules...");

  if (words.size() == 0)
  {
    LOG_TRACE("No words specified.");
    return false;
  }

  //can't prepare this one due to variable inputs
  QSqlQuery selectWordIdsForWords(_db);
  //the WHERE IN clause is case sensitive, so or'ing them together instead
  QString queryStr = "SELECT id, word FROM words WHERE ";
  for (QSet<QString>::const_iterator wordItr = words.begin(); wordItr != words.end(); ++wordItr)
  {
    //queryStr += "UPPER(word)='" + (*wordItr).toUpper() + "' OR ";
    //LIKE is case insensitive by default, so using that instead of '='; using toUpper() with '='
    //for comparisons won't work for unicode chars in SQLite w/o quite a bit of additional setup
    //to link in special unicode libs (I think)
    queryStr += "word LIKE '" + *wordItr + "' OR ";
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
  queryStr = "SELECT DISTINCT rule_id, word_id FROM rules WHERE word_id IN (";
  int wordIdCount = 0;
  while (selectWordIdsForWords.next())
  {
    const QString word = selectWordIdsForWords.value(1).toString();
    LOG_VART(word);
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
  QSet<int> matchingRuleWordIds;
  QSet<int> ruleIds;
  while (uniqueRuleCountForWords.next())
  {
    const int ruleId = uniqueRuleCountForWords.value(0).toInt();
    LOG_VART(ruleId);
    matchingRuleWordIds.insert(uniqueRuleCountForWords.value(1).toInt());
    if (!ruleIds.contains(ruleId))
    {
      ruleIds.insert(ruleId);
      ruleIdCount++;
    }
  }
  LOG_VART(ruleIdCount);
  const bool wordsInvolveMultipleRules = ruleIdCount > 1;
  LOG_VART(wordsInvolveMultipleRules);

  if (wordsInvolveMultipleRules)
  {
    //can't prepare this one due to variable inputs
    QSqlQuery selectWordForWordIds(_db);
    queryStr = "SELECT word FROM words WHERE id IN (";
    for (QSet<int>::const_iterator wordIdItr = matchingRuleWordIds.begin();
         wordIdItr != matchingRuleWordIds.end(); ++wordIdItr)
    {
      queryStr += QString::number(*wordIdItr) + ",";
    }
    queryStr.chop(1);
    queryStr += ")";
    LOG_VART(queryStr);
    if (!selectWordForWordIds.exec(queryStr))
    {
      throw HootException(
        QString("Error executing query: %1").arg(selectWordForWordIds.lastError().text()));
    }
    while (selectWordForWordIds.next())
    {
      matchingRuleWords.insert(selectWordForWordIds.value(0).toString());
    }
    LOG_VART(matchingRuleWords);
  }

  return wordsInvolveMultipleRules;
}

int ImplicitTagRulesSqliteReader::getRuleCount()
{
  LOG_TRACE("Retrieving rule count...");

  if (!_ruleCountQuery.exec())
  {
    throw HootException(
      QString("Error executing query: %1").arg(_ruleCountQuery.lastError().text()));
  }

  _ruleCountQuery.next();
  return _ruleCountQuery.value(0).toInt();
}

Tags ImplicitTagRulesSqliteReader::getImplicitTags(const QSet<QString>& words,
                                                   QSet<QString>& matchingWords)
{
  LOG_TRACE("Retrieving implicit tags for words: " << words << "...");

  //can't prepare this one due to variable inputs
  QSqlQuery selectWordIdsForWords = QSqlQuery(_db);
  //the WHERE IN clause is case sensitive, so or'ing them together instead
  QString queryStr = "SELECT id, word FROM words WHERE ";
  for (QSet<QString>::const_iterator wordItr = words.begin(); wordItr != words.end(); ++wordItr)
  {
    //queryStr += "UPPER(word)='" + (*wordItr).toUpper() + "' OR ";
    //see comment about use of LIKE in wordsInvolveMultipleRules
    queryStr += "word LIKE '" + *wordItr + "' OR ";
  }
  queryStr.chop(4);
  LOG_VART(queryStr);
  if (!selectWordIdsForWords.exec(queryStr))
  {
    throw HootException(
      QString("Error executing query: %1").arg(selectWordIdsForWords.lastError().text()));
  }

  //can't prepare this one due to variable inputs
  QSqlQuery selectTagIdsForWordIds(_db);
  QString query = "SELECT DISTINCT tag_id FROM rules WHERE word_id IN (";
  int numWordIds = 0;
  while (selectWordIdsForWords.next())
  {
    const QString word = selectWordIdsForWords.value(1).toString();
    LOG_VART(word);
    query += QString::number(selectWordIdsForWords.value(0).toInt()) + ",";
    matchingWords.insert(word);
    numWordIds++;
  }
  query.chop(1);
  query += ")";
  LOG_VART(numWordIds);
  if (numWordIds == 0)
  {
    LOG_TRACE("No associated tag IDs found for words: " << words);
    matchingWords.clear();
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
    LOG_TRACE("No associated tags found for words: " << words);
    matchingWords.clear();
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
