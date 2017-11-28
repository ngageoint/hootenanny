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
_useTagsCache(ConfigOptions().getPoiImplicitTagRulesReaderUseTagsCache()),
_firstRoundTagsCacheHits(0),
_secondRoundTagsCacheHits(0),
_addTopTagOnly(ConfigOptions().getPoiImplicitTagRulesAddTopTagOnly()),
_allowWordsInvolvedInMultipleRules(
  ConfigOptions().getPoiImplicitTagRulesAllowWordsInvolvedInMultipleRules())
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

  _tagsForWordIds = QSqlQuery(_db);
  if (_addTopTagOnly)
  {
//    if (!_tagsForWordIds.prepare(
//         QString("SELECT tags.kvp FROM tags JOIN rules ON rules.tag_id = tags.id ") +
//         QString("WHERE rules.word_id = :wordId ORDER BY rules.tag_count DESC LIMIT 1")))
    if (!_tagsForWordIds.prepare(
         QString("SELECT tags.kvp FROM tags JOIN rules ON rules.tag_id = tags.id ") +
         QString("WHERE rules.word_id = :wordId ORDER BY rules.tag_count DESC")))
    {
      throw HootException(
        QString("Error preparing _topTagForWordIds: %1")
          .arg(_tagsForWordIds.lastError().text()));
    }
  }
  else
  {
    if (!_tagsForWordIds.prepare(
         QString("SELECT tags.kvp FROM tags JOIN rules ON rules.tag_id = tags.id ") +
         QString("WHERE rules.word_id = :wordId")))
    {
      throw HootException(
        QString("Error preparing _tagsForWordIds: %1")
          .arg(_tagsForWordIds.lastError().text()));
    }
  }

  _tagCountsForWordIds = QSqlQuery(_db);
  if (!_tagCountsForWordIds.prepare(
       QString("SELECT rules.tag_count FROM rules JOIN tags on rules.tag_id = tags.id ") +
       QString("WHERE rules.word_id = :wordId ") +
       QString("ORDER BY rules.tag_count DESC LIMIT 1")))
  {
    throw HootException(
      QString("Error preparing _tagCountsForWordIds: %1")
        .arg(_tagCountsForWordIds.lastError().text()));
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
      LOG_TRACE("Found cached tags.");
      matchingWords = words;
      Tags tagsToReturn;
      if (!cachedTags->contains("wordsInvolvedInMultipleRules"))
      {
        tagsToReturn = *cachedTags;
      }
      else
      {
        wordsInvolvedInMultipleRules = true;
        LOG_TRACE("Cached tags involved in multiple rules.");
        LOG_VART(matchingWords);
      }
      const QString tagsStr = tagsToReturn.toString().trimmed().replace("\n", ", ");
      LOG_TRACE("Returning cached tags: " << tagsStr << " for words: " << matchingWords << ".");
      _firstRoundTagsCacheHits++;
      return tagsToReturn;
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
      QString("Error executing query: %1; Error: %2")
        .arg(selectWordIdsForWords.lastQuery())
        .arg(selectWordIdsForWords.lastError().text()));
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
    if (_useTagsCache)
    {
      //cache empty set of tags
      QStringList wordsList = words.toList();
      Tags* tagsToCache(new Tags());
      _tagsCache.insert(wordsList.join(";"), tagsToCache);
    }
    return Tags();
  }

  if (_useTagsCache)
  {
    //The words passed in may not have been in the cache previously, if some words were passed in
    //that didn't exist in the db.  Now that those words have been filtered out of the input, let's
    //check the cache again.
    QStringList wordsList = queriedWords.toList();
    QString wordsKey = wordsList.join(";");
    Tags* cachedTags = _tagsCache[wordsKey];
    if (cachedTags != 0)
    {
      LOG_TRACE("Found cached tags.");
      matchingWords = queriedWords;
      Tags tagsToReturn;
      if (!cachedTags->contains("wordsInvolvedInMultipleRules"))
      {
        tagsToReturn = *cachedTags;
      }
      else
      {
        wordsInvolvedInMultipleRules = true;
        LOG_TRACE("Cached tags involved in multiple rules.");
        LOG_VART(matchingWords);
      }
      const QString tagsStr = tagsToReturn.toString().trimmed().replace("\n", ", ");
      LOG_TRACE("Returning cached tags: " << tagsStr << " for words: " << matchingWords << ".");
      _secondRoundTagsCacheHits++;
      return tagsToReturn;
    }
  }

  if (_allowWordsInvolvedInMultipleRules)
  {
    long wordIdWithHighestTagOccurrenceCount = -1;
    long highestTagOccurrenceCount = -1;
    for (QSet<long>::const_iterator wordIdItr = queriedWordIds.begin();
         wordIdItr != queriedWordIds.end(); ++wordIdItr)
    {
      const long wordId = *wordIdItr;
      _tagCountsForWordIds.bindValue(":wordId", (qlonglong)wordId);
      LOG_VART(_tagCountsForWordIds.lastQuery());
      if (!_tagCountsForWordIds.exec())
      {
        throw HootException(
          QString("Error executing query: %1; Error: %2")
            .arg(_tagCountsForWordIds.lastQuery())
            .arg(_tagCountsForWordIds.lastError().text()));
      }
      while (_tagCountsForWordIds.next()) //only one record should be returned
      {
        const long tagCount = _tagCountsForWordIds.value(0).toLongLong();
        if (tagCount > highestTagOccurrenceCount)
        {
          highestTagOccurrenceCount = tagCount;
          wordIdWithHighestTagOccurrenceCount = wordId;
        }
      }
    }
    LOG_VART(highestTagOccurrenceCount);
    LOG_VART(wordIdWithHighestTagOccurrenceCount);

    queriedWordIds.clear();
    if (wordIdWithHighestTagOccurrenceCount != -1)
    {
      queriedWordIds.insert(wordIdWithHighestTagOccurrenceCount);
      LOG_VART(queriedWordIds);
    }
    else
    {
      if (_useTagsCache)
      {
        //cache empty set of tags
        QStringList wordsList = words.toList();
        Tags* tagsToCache(new Tags());
        _tagsCache.insert(wordsList.join(";"), tagsToCache);
      }
      return Tags();
    }
  }

  Tags tags;
  for (QSet<long>::const_iterator wordIdItr = queriedWordIds.begin();
       wordIdItr != queriedWordIds.end(); ++wordIdItr)
  {
    _tagsForWordIds.bindValue(":wordId", (qlonglong)*wordIdItr);
    LOG_VART(_tagsForWordIds.lastQuery());
    if (!_tagsForWordIds.exec())
    {
      throw HootException(
        QString("Error executing query: %1; Error: %2")
          .arg(_tagsForWordIds.lastQuery())
          .arg(_tagsForWordIds.lastError().text()));
    }
    Tags tags2;
    while (_tagsForWordIds.next())
    {
      const QString kvp = _tagsForWordIds.value(0).toString();
      if (!_customRules.getTagIgnoreList().contains(kvp) &&
          (!_addTopTagOnly || (_addTopTagOnly && tags2.isEmpty())))
      {
        tags2.appendValue(kvp);
      }
    }
    LOG_VART(tags);
    LOG_VART(tags2);
    if (tags.isEmpty())
    {
      tags = tags2;
    }
    else if (!tags2.isEmpty() && tags != tags2)
    {
      wordsInvolvedInMultipleRules = true;
      matchingWords = queriedWords;
      if (_useTagsCache)
      {
        //bit of a hack...cache a single tag "wordsInvolvedInMultipleRules=true"; that tells us to
        //return an empty set of tags and set wordsInvolvedInMultipleRules = true on output
        QStringList wordsList = matchingWords.toList();
        Tags* tagsToCache(new Tags());
        tagsToCache->insert("wordsInvolvedInMultipleRules", "true");
        _tagsCache.insert(wordsList.join(";"), tagsToCache);
        if (matchingWords != words)
        {
          //also cache the originally passed in words
          wordsList = words.toList();
          Tags* tagsToCache(new Tags());
          tagsToCache->insert("wordsInvolvedInMultipleRules", "true");
          _tagsCache.insert(wordsList.join(";"), tagsToCache);
        }
      }
      LOG_TRACE(
        "Words: " << matchingWords << " involved in multiple rules due to tag sets not matching.");
      LOG_VART(tags);
      LOG_VART(tags2);
      return Tags();
    }
  }

  //don't return more than one tag that have the same value; arbitrarily pick the first one;
  //e.g. amenity=hospital and building=hospital; this behavior could be captured in the rules
  //deriver instead
  QStringList tagValues;
  QStringList tagKeysWithDuplicatedValues;
  for (Tags::const_iterator tagItr = tags.begin(); tagItr != tags.end(); ++tagItr)
  {
    const QString tagValue = tagItr.value();
    if (!tagValues.contains(tagValue))
    {
      tagValues.append(tagValue);
    }
    else
    {
      tagKeysWithDuplicatedValues.append(tagItr.key());
    }
  }
  LOG_VART(tagKeysWithDuplicatedValues);
  for (int i = 0; i < tagKeysWithDuplicatedValues.size(); i++)
  {
    const QString tagKey = tagKeysWithDuplicatedValues[i];
    tags.remove(tagKey);
    LOG_TRACE("Removed tag key: " << tagKey << " due to value duplication.");
  }
  if (tags.size() == 0)
  {
    if (_useTagsCache)
    {
      //cache empty set of tags
      QStringList wordsList = matchingWords.toList();
      Tags* tagsToCache(new Tags());
      _tagsCache.insert(wordsList.join(";"), tagsToCache);
      if (matchingWords != words)
      {
        //also cache the originally passed in words
        wordsList = words.toList();
        Tags* tagsToCache(new Tags());
        _tagsCache.insert(wordsList.join(";"), tagsToCache);
      }
    }
    return Tags();
  }

  matchingWords = queriedWords;
  if (_useTagsCache)
  {
    QStringList wordsList = matchingWords.toList();
    Tags* tagsToCache(new Tags(tags));
    _tagsCache.insert(wordsList.join(";"), tagsToCache);
    if (matchingWords != words)
    {
      //also cache the originally passed in words
      wordsList = words.toList();
      Tags* tagsToCache(new Tags());
      _tagsCache.insert(wordsList.join(";"), tagsToCache);
    }
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
