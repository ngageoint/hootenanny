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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#include "ImplicitTagRulesSqliteReader.h"

// hoot
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>

// Qt
#include <QSet>
#include <QSqlError>
#include <QTextStream>
#include <QVariant>

namespace hoot
{

ImplicitTagRulesSqliteReader::ImplicitTagRulesSqliteReader() :
_firstRoundTagsCacheHits(0),
_secondRoundTagsCacheHits(0),
_addTopTagOnly(true),
_allowWordsInvolvedInMultipleRules(false),
//Since we're inserting all objects with cost = 1, the cost passed into the constructor
//of QCache behaves as a max size.
_tagsCache(ConfigOptions().getImplicitTaggingMaxCacheSize())
{
}

ImplicitTagRulesSqliteReader::~ImplicitTagRulesSqliteReader()
{
  close();
}

void ImplicitTagRulesSqliteReader::open(const QString& url)
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

Tags ImplicitTagRulesSqliteReader::getImplicitTags(const QSet<QString>& words,
                                                   QSet<QString>& matchingWords,
                                                   bool& wordsInvolvedInMultipleRules)
{
  //This method can probably be sped up by combining some queries, but the sizes of the databases
  //being used so far have been small enough that performance has not yet been an issue.

  if (words.empty())
  {
    LOG_TRACE("No words specified.");
    return Tags();
  }

  LOG_TRACE("Retrieving implicit tags for words: " << words << "...");

  matchingWords.clear();
  wordsInvolvedInMultipleRules = false;
  Tags cachedTags;

  //The cache needs to check against a key composed of all of the words, due to possible multiple
  //rule conflicts that can occur when combinations of full names and tokenized names are passed in.
  //Its possible that the exact set of words passed in is in the cache, so check for that now.
  cachedTags = _checkCachedTags(words, matchingWords, wordsInvolvedInMultipleRules);
  if (!cachedTags.isEmpty())
  {
    _firstRoundTagsCacheHits++;
    return cachedTags;
  }

  //get ids for only the input words that actually exist in the database
  QSet<long> queriedWordIds;
  QSet<QString> queriedWords;
  _queryWords(words, queriedWordIds, queriedWords);
  if (queriedWordIds.empty())
  {
    //cache empty set of tags
    _cacheTags(words, Tags());
    return Tags();
  }

  //The words passed in may not have been in the cache previously, if some words were passed in
  //that didn't exist in the db.  Now that those words have been filtered out of the input, let's
  //check the cache again.
  cachedTags = _checkCachedTags(queriedWords, matchingWords, wordsInvolvedInMultipleRules);
  if (!cachedTags.isEmpty())
  {
    _secondRoundTagsCacheHits++;
    return cachedTags;
  }

  if (_allowWordsInvolvedInMultipleRules)
  {
    //special log if we're allowing tags to be returned when input words are involved in multiple
    //implicit tag rules
    _modifyWordIdsForMultipleRules(queriedWordIds);
    if (queriedWordIds.isEmpty())
    {
      //cache empty set of tags
      _cacheTags(words, Tags());
      return Tags();
    }
  }

  //get all the tags associated with the words
  Tags tags =
    _getTagsForWords(queriedWordIds, queriedWords, words, matchingWords,
                     wordsInvolvedInMultipleRules);
  if (tags.empty())
  {
    return tags;
  }
  //Don't return more than one tag that have the same value.  Arbitrarily pick the first one.
  //e.g. amenity=hospital and building=hospital.  This behavior could be captured in the rules
  //deriver instead, as well as improved overall.
  _removeTagsWithDuplicatedValues(tags);
  if (tags.empty())
  {
    //cache empty set of tags
    _cacheTags(matchingWords, Tags());
    if (matchingWords != words)
    {
      //also cache the originally passed in words
      _cacheTags(words, Tags());
    }
    return Tags();
  }

  //record the actual words involved in the rule to be returned to the caller
  matchingWords = queriedWords;
  _cacheTags(matchingWords, tags);
  if (matchingWords != words)
  {
    //also cache the originally passed in words
    _cacheTags(words, tags);
  }
  LOG_TRACE("Returning tags: " << tags << " for words: " << matchingWords);
  return tags;
}

long ImplicitTagRulesSqliteReader::getRuleCount()
{
  LOG_TRACE("Retrieving rule count...");

  if (!_ruleCountQuery.exec())
  {
    throw HootException(
      QString("Error executing query: %1").arg(_ruleCountQuery.lastError().text()));
  }

  _ruleCountQuery.next();
  return _ruleCountQuery.value(0).toLongLong();
}

QString ImplicitTagRulesSqliteReader::getStats()
{
  LOG_DEBUG("Printing stats...");

  if (!_ruleCountQuery.exec())
  {
    throw HootException(
      QString("Error executing query: %1").arg(_ruleCountQuery.lastError().text()));
  }
  _ruleCountQuery.next();
  const long ruleCount = _ruleCountQuery.value(0).toLongLong();

  if (!_tagCountQuery.exec())
  {
    throw HootException(
      QString("Error executing query: %1").arg(_tagCountQuery.lastError().text()));
  }
  _tagCountQuery.next();
  const long tagCount = _tagCountQuery.value(0).toLongLong();

  if (!_wordCountQuery.exec())
  {
    throw HootException(
      QString("Error executing query: %1").arg(_wordCountQuery.lastError().text()));
  }
  _wordCountQuery.next();
  const long wordCount = _wordCountQuery.value(0).toLongLong();

  QString buffer;
  QTextStream ts(&buffer);
  ts.setCodec("UTF-8");
  ts << "Implicit tag rules database summary:" << endl;
  ts << "\tWord count: " << wordCount << endl;
  ts << "\tTag count: " << tagCount << endl;
  ts << "\tRule count: " << ruleCount << endl;
  return ts.readAll();
}

void ImplicitTagRulesSqliteReader::_prepareQueries()
{
  _ruleCountQuery = QSqlQuery(_db);
  if (!_ruleCountQuery.prepare("SELECT COUNT(*) FROM rules"))
  {
    throw HootException(
      QString("Error preparing _ruleCountQuery: %1")
        .arg(_ruleCountQuery.lastError().text()));
  }

  _tagCountQuery = QSqlQuery(_db);
  if (!_tagCountQuery.prepare("SELECT COUNT(*) FROM tags"))
  {
    throw HootException(
      QString("Error preparing _tagCountQuery: %1")
        .arg(_tagCountQuery.lastError().text()));
  }

  _wordCountQuery = QSqlQuery(_db);
  if (!_wordCountQuery.prepare("SELECT COUNT(*) FROM words"))
  {
    throw HootException(
      QString("Error preparing _wordCountQuery: %1")
        .arg(_wordCountQuery.lastError().text()));
  }

  _tagsForWordIdsQuery = QSqlQuery(_db);
  if (_addTopTagOnly)
  {
    if (!_tagsForWordIdsQuery.prepare(
         QString("SELECT tags.kvp FROM tags JOIN rules ON rules.tag_id = tags.id ") +
         QString("WHERE rules.word_id = :wordId ORDER BY rules.tag_count DESC")))
    {
      throw HootException(
        QString("Error preparing _topTagForWordIdsQuery: %1")
          .arg(_tagsForWordIdsQuery.lastError().text()));
    }
  }
  else
  {
    if (!_tagsForWordIdsQuery.prepare(
         QString("SELECT tags.kvp FROM tags JOIN rules ON rules.tag_id = tags.id ") +
         QString("WHERE rules.word_id = :wordId")))
    {
      throw HootException(
        QString("Error preparing _tagsForWordIdQuerys: %1")
          .arg(_tagsForWordIdsQuery.lastError().text()));
    }
  }

  _tagCountsForWordIdsQuery = QSqlQuery(_db);
  if (!_tagCountsForWordIdsQuery.prepare(
       QString("SELECT rules.tag_count FROM rules JOIN tags on rules.tag_id = tags.id ") +
       QString("WHERE rules.word_id = :wordId ") +
       QString("ORDER BY rules.tag_count DESC LIMIT 1")))
  {
    throw HootException(
      QString("Error preparing _tagCountsForWordIdsQuery: %1")
        .arg(_tagCountsForWordIdsQuery.lastError().text()));
  }
}

Tags ImplicitTagRulesSqliteReader::_checkCachedTags(const QSet<QString>& words,
                                                    QSet<QString>& matchingWords,
                                                    bool& wordsInvolvedInMultipleRules)
{
  const QStringList wordsList = words.toList();
  const QString wordsKey = wordsList.join(";");
  const Tags* cachedTags = _tagsCache[wordsKey];
  if (cachedTags != nullptr)
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
    return tagsToReturn;
  }
  return Tags();
}

void ImplicitTagRulesSqliteReader::_queryWords(const QSet<QString>& words,
                                               QSet<long>& queriedWordIds,
                                               QSet<QString>& queriedWords) const
{
  //can't prepare this query due to the variable inputs
  QSqlQuery selectWordIdsForWords(_db);
  //the WHERE IN clause is case sensitive, so OR'ing the words together with LIKE instead
  //(no wildcards)
  QString queryStr = "SELECT id, word FROM words WHERE ";
  for (QSet<QString>::const_iterator wordItr = words.begin(); wordItr != words.end(); ++wordItr)
  {
    //LIKE is case insensitive by default, so using that instead of '='; using toUpper() with '='
    //for comparisons won't work for unicode chars in SQLite w/o quite a bit of additional setup
    //to link in special unicode libs (I think)
    QString word = *wordItr;
    //escape '
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

  while (selectWordIdsForWords.next())
  {
    queriedWordIds.insert(selectWordIdsForWords.value(0).toLongLong());
    queriedWords.insert(selectWordIdsForWords.value(1).toString());
  }
  LOG_VART(queriedWordIds);
  LOG_VART(queriedWords);
}

void ImplicitTagRulesSqliteReader::_cacheTags(const QSet<QString>& words, const Tags& tags)
{
  QStringList wordsList = words.toList();
  // The cache takes ownership of this object.
  Tags* tagsToCache = new Tags(tags);
  _tagsCache.insert(wordsList.join(";"), tagsToCache);
}

void ImplicitTagRulesSqliteReader::_modifyWordIdsForMultipleRules(QSet<long>& queriedWordIds)
{
  // I'm not sure this is doing what's intended.  Since we're not allowing multiple rule
  // involvement by default, its not a problem for now.

  long wordIdWithHighestTagOccurrenceCount = -1;
  long highestTagOccurrenceCount = -1;
  for (QSet<long>::const_iterator wordIdItr = queriedWordIds.begin();
       wordIdItr != queriedWordIds.end(); ++wordIdItr)
  {
    const long wordId = *wordIdItr;
    _tagCountsForWordIdsQuery.bindValue(":wordId", (qlonglong)wordId);
    LOG_VART(_tagCountsForWordIdsQuery.lastQuery());
    if (!_tagCountsForWordIdsQuery.exec())
    {
      throw HootException(
        QString("Error executing query: %1; Error: %2")
          .arg(_tagCountsForWordIdsQuery.lastQuery())
          .arg(_tagCountsForWordIdsQuery.lastError().text()));
    }
    while (_tagCountsForWordIdsQuery.next()) //only one record should be returned
    {
      const long tagCount = _tagCountsForWordIdsQuery.value(0).toLongLong();
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
}

Tags ImplicitTagRulesSqliteReader::_getTagsForWords(const QSet<long>& queriedWordIds,
                                                    const QSet<QString>& queriedWords,
                                                    const QSet<QString>& inputWords,
                                                    QSet<QString>& matchingWords,
                                                    bool& wordsInvolvedInMultipleRules)
{
  Tags tags;
  for (QSet<long>::const_iterator wordIdItr = queriedWordIds.begin();
       wordIdItr != queriedWordIds.end(); ++wordIdItr)
  {
    _tagsForWordIdsQuery.bindValue(":wordId", (qlonglong)*wordIdItr);
    LOG_VART(_tagsForWordIdsQuery.lastQuery());
    if (!_tagsForWordIdsQuery.exec())
    {
      throw HootException(
        QString("Error executing query: %1; Error: %2")
          .arg(_tagsForWordIdsQuery.lastQuery())
          .arg(_tagsForWordIdsQuery.lastError().text()));
    }

    Tags tags2;
    while (_tagsForWordIdsQuery.next())
    {
      const QString kvp = _tagsForWordIdsQuery.value(0).toString();
      //';' denotes multiple tags for the same word; currently this only occurs in the custom rules
      //file.  It would also work for any entires in the rules database, but we don't write multiple
      //tags for the same rules during derivation yet.
      const QStringList kvps = kvp.split(";");
      if (!_addTopTagOnly || (_addTopTagOnly && tags2.isEmpty()))
      {
        for (int i = 0; i < kvps.size(); i++)
        {
          tags2.appendValue(kvps.at(i));
        }
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

      //bit of a hack...cache a single tag "wordsInvolvedInMultipleRules=true"; that tells us to
      //return an empty set of tags and set wordsInvolvedInMultipleRules = true on output
      Tags tagsToCache;
      tagsToCache.insert("wordsInvolvedInMultipleRules", "true");
      _cacheTags(matchingWords, tagsToCache);
      if (matchingWords != inputWords)
      {
        //also cache the originally passed in words
        Tags moreTagsToCache;
        moreTagsToCache.insert("wordsInvolvedInMultipleRules", "true");
        _cacheTags(inputWords, moreTagsToCache);
      }
      LOG_TRACE(
        "Words: " << matchingWords << " involved in multiple rules due to tag sets not matching.");
      LOG_VART(tags);
      LOG_VART(tags2);
      return Tags();
    }
  }
  return tags;
}

void ImplicitTagRulesSqliteReader::_removeTagsWithDuplicatedValues(Tags& tags) const
{
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
}

}
