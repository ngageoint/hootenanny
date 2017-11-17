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
#include <hoot/core/util/ConfigOptions.h>

// Tgs
#include <tgs/System/Time.h>

// Qt
#include <QSqlError>
#include <QVariant>
#include <QSet>

namespace hoot
{

ImplicitTagRulesSqliteReader::ImplicitTagRulesSqliteReader() :
//since we're inserting all objects with cost=1, the cost passed into the constructor
//of QCache behaves as a max size
_tagsCache(ConfigOptions().getPoiImplicitTagRulesReaderMaxTagCacheSize()),
_useTagsCache(ConfigOptions().getPoiImplicitTagRulesReaderUseTagsCache())
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
  _ruleWordPartCountQuery = QSqlQuery(_db);
  if (!_ruleWordPartCountQuery.prepare("SELECT COUNT(*) FROM rules"))
  {
    throw HootException(
      QString("Error preparing _ruleWordPartCountQuery: %1")
        .arg(_ruleWordPartCountQuery.lastError().text()));
  }
}

Tags ImplicitTagRulesSqliteReader::getImplicitTags(const QSet<QString>& words,
                                                   QSet<QString>& matchingWords,
                                                   bool& wordsInvolvedInMultipleRules)
{
  LOG_TRACE("Retrieving implicit tags for words: " << words << "...");

  matchingWords.clear();
  wordsInvolvedInMultipleRules = false;

  if (words.size() == 0)
  {
    LOG_TRACE("No words specified.");
    return Tags();
  }

  if (_useTagsCache)
  {
    //The cache needs to check against a key composed of all of the words, due to possible multiple
    //rule conflicts that can occur when combinations of full names and tokenized names are passed in.
    //Its possible that the exact set of words passed in is in the cache, so check.
    //TODO: handle ';' in words
    QStringList wordsList = words.toList();
    QString wordsKey = wordsList.join(";");
    Tags* cachedTags = _tagsCache[wordsKey];
    if (cachedTags != 0)
    {
      matchingWords = words;
      const QString tagsStr = cachedTags->toString().trimmed().replace("\n", ", ");
      LOG_TRACE("Returning cached tags: " << tagsStr << " for words: " << matchingWords << ".");
      return *cachedTags;
    }
  }

  //can't prepare this one due to variable inputs
  QSqlQuery selectWordIdsForWords(_db);
  //the WHERE IN clause is case sensitive, so OR'ing them together instead
  QString queryStr = "SELECT id, word FROM words WHERE ";
  for (QSet<QString>::const_iterator wordItr = words.begin(); wordItr != words.end(); ++wordItr)
  {
    //queryStr += "UPPER(word)='" + (*wordItr).toUpper() + "' OR ";
    //LIKE is case insensitive by default, so using that instead of '='; using toUpper() with '='
    //for comparisons won't work for unicode chars in SQLite w/o quite a bit of additional setup
    //to link in special unicode libs (I think)
    QString word = *wordItr;
    word = word.replace("'", "''");
    queryStr += "word LIKE '" + word + "' OR ";
  }
  queryStr.chop(4);
  LOG_VART(queryStr);
  if (!selectWordIdsForWords.exec(queryStr))
  {
    throw HootException(
      QString("Error executing query: %1").arg(selectWordIdsForWords.lastError().text()));
  }

  QSet<long> queriedWordIds;
  QSet<QString> queriedWords;
  while (selectWordIdsForWords.next())
  {
    queriedWordIds.insert(selectWordIdsForWords.value(0).toLongLong());
    queriedWords.insert(selectWordIdsForWords.value(1).toString());
  }
  LOG_VART(queriedWordIds);
  LOG_VART(queriedWords);

  if (queriedWordIds.size() == 0)
  {
    return Tags();
  }

  if (_useTagsCache)
  {
    //the words passed in may not have been in the cache previously, if some words were passed in
    //which don't exist in the db.  Now, that those words have been filtered out, let's check
    //the cache again.
    QStringList wordsList = queriedWords.toList();
    QString wordsKey = wordsList.join(";");
    Tags* cachedTags = _tagsCache[wordsKey];
    if (cachedTags != 0)
    {
      matchingWords = queriedWords;
      const QString tagsStr = cachedTags->toString().trimmed().replace("\n", ", ");
      LOG_TRACE("Returning cached tags: " << tagsStr << " for words: " << matchingWords << ".");
      return *cachedTags;
    }
  }

  Tags tags;
  for (QSet<long>::const_iterator wordIdItr = queriedWordIds.begin();
       wordIdItr != queriedWordIds.end(); ++wordIdItr)
  {
    QSqlQuery tagsForWordIds(_db);
    QString queryStr =
      QString("SELECT tags.kvp FROM tags JOIN rules ON rules.tag_id = tags.id") +
      QString(" WHERE rules.word_id = ") + QString::number(*wordIdItr);
    LOG_VART(queryStr);
    if (!tagsForWordIds.exec(queryStr))
    {
      throw HootException(
        QString("Error executing query: %1").arg(tagsForWordIds.lastError().text()));
    }
    Tags tags2;
    while (tagsForWordIds.next())
    {
      tags2.appendValue(tagsForWordIds.value(0).toString());
    }
    LOG_VART(tags2);
    if (tags.isEmpty())
    {
      tags = tags2;
    }
    else if (tags != tags2)
    {
      wordsInvolvedInMultipleRules = true;
      matchingWords = queriedWords;
      LOG_TRACE(
        "Words: " << matchingWords << " involved in multiple rules due to tag sets not matching.");
      return Tags();
    }
  }

  matchingWords = queriedWords;
  if (_useTagsCache)
  {
    QStringList wordsList = matchingWords.toList();
    Tags* tagsToCache(new Tags(tags));
    _tagsCache.insert(wordsList.join(";"), tagsToCache);
  }
  LOG_TRACE("Returning tags: " << tags << " for words: " << matchingWords);
  return tags;
}

long ImplicitTagRulesSqliteReader::getRuleWordPartCount()
{
  LOG_TRACE("Retrieving rule count...");

  if (!_ruleWordPartCountQuery.exec())
  {
    throw HootException(
      QString("Error executing query: %1").arg(_ruleWordPartCountQuery.lastError().text()));
  }

  _ruleWordPartCountQuery.next();
  return _ruleWordPartCountQuery.value(0).toLongLong();
}

}
