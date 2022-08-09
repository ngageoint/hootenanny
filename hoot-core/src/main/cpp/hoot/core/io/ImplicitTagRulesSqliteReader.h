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

#ifndef IMPLICITTAGRULESSQLITEREADER_H
#define IMPLICITTAGRULESSQLITEREADER_H

// Hoot
#include <hoot/core/elements/Tags.h>

// Qt
#include <QtSql/QSqlDatabase>
#include <QSqlQuery>
#include <QCache>

namespace  hoot
{

/**
 * Reads implicit tag rules from a Sqlite implicit tag rules database
 */
class ImplicitTagRulesSqliteReader
{
public:

  ImplicitTagRulesSqliteReader();
  ~ImplicitTagRulesSqliteReader();

  /**
   * Opens a Sqlite implicit tag rules database
   *
   * @param url location of the database to open
   */
  void open(const QString& url);

  /**
   * Closes a Sqlite implicit tag rules database
   */
  void close();

  /**
   * Given a set of input words, returns a set of implicitly derived tags associated with those
   * words
   *
   * It also populates the words that matched (matchingWords) and determines if the
   * input words were involved in multiple tag rules (wordsInvolvedInMultipleRule). Words involved
   * in multiple tag rules will result in an empty tag set being returned if
   * _allowWordsInvolvedInMultipleRules is set to true.
   *
   * @param words a collection of words for which to retrieve implicitly derived tags
   * @param matchingWords a collection of words found in the implicit tag database; populated by
   * this method
   * @param wordsInvolvedInMultipleRules true if the collection of words are associated with more
   * than one set of tags; populated by this method
   * @return a set of implicitly derived tags, if they exist, for the given input words; an empty
   * tag set otherwise
   */
  Tags getImplicitTags(
    const QSet<QString>& words, QSet<QString>& matchingWords, bool& wordsInvolvedInMultipleRules);

  /**
   * Return a string containing relevant info about the rule database
   */
  QString getStats();

  /**
   * Retrieves the total number implicit tag rules (word/tag associations) in the database
   *
   * @return a rule count
   */
  long getRuleCount();

  int getTagsCacheSize() const { return _tagsCache.size(); }
  long getFirstRoundTagsCacheHits() const { return _firstRoundTagsCacheHits; }
  long getSecondRoundTagsCacheHits() const { return _secondRoundTagsCacheHits; }

  void setAddTopTagOnly(bool add) { _addTopTagOnly = add; }
  void setAllowWordsInvolvedInMultipleRules(bool allow)
  { _allowWordsInvolvedInMultipleRules = allow; }

private:

  //implicit tag rules db
  QSqlDatabase _db;

  QSqlQuery _ruleCountQuery;
  QSqlQuery _tagCountQuery;
  QSqlQuery _wordCountQuery;
  //query to return all tags associated with the specified word ids
  QSqlQuery _tagsForWordIdsQuery;
  //query to return the tag occurrence associated with the specified word ids
  QSqlQuery _tagCountsForWordIdsQuery;

  //number of times tags were found in the first tag cache query
  long _firstRoundTagsCacheHits;
  //number of times tags were found in the second tag cache query
  long _secondRoundTagsCacheHits;

  //If true, only the tag with the highest word/tag occurrence count will be returned
  //(default = true).  Setting this to true may avoids some false positive type tags from being
  //applied.  More experimentation may need to be done to see if disabling this setting can yield
  //better overall tagging results.
  bool _addTopTagOnly;
  //If true, any input words associated with multiple rules in the database will result in all
  //associated tags being returned (default = false).  If false, no type tags will be returned in
  //this situation and only a single tag indicating the multiple rule involvement will be applied.
  //More experimentation may need to be done to see if disabling this setting can yield better
  //overall tagging results.
  bool _allowWordsInvolvedInMultipleRules;

  //keep a simple cache associating words with implicit tags to speed things up a bit
  QCache<QString, Tags> _tagsCache;

  void _cacheTags(const QSet<QString>& words, const Tags& tags);
  Tags _checkCachedTags(const QSet<QString>& words, QSet<QString>& matchingWords,
                        bool& wordsInvolvedInMultipleRules);

  void _prepareQueries();
  /*
   * Check to see which input words exist
   */
  void _queryWords(const QSet<QString>& words, QSet<long>& queriedWordIds,
                   QSet<QString>& queriedWords) const;
  /*
   * Gets tags associated with only the inputs words that actually exist
   */
  Tags _getTagsForWords(const QSet<long>& queriedWordIds, const QSet<QString>& queriedWords,
                        const QSet<QString>& inputWords, QSet<QString>& matchingWords,
                        bool& wordsInvolvedInMultipleRules);
  void _removeTagsWithDuplicatedValues(Tags& tags) const;
  void _modifyWordIdsForMultipleRules(QSet<long>& queriedWordIds);
};

}

#endif // IMPLICITTAGRULESSQLITEREADER_H
