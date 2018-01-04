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
 * @copyright Copyright (C) 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef IMPLICITTAGRULESSQLITEREADER_H
#define IMPLICITTAGRULESSQLITEREADER_H

// Hoot
#include <hoot/core/elements/Tags.h>
#include <hoot/rnd/schema/ImplicitTagCustomRules.h>
#include <hoot/core/util/Configurable.h>

// Qt
#include <QString>
#include <QtSql/QSqlDatabase>
#include <QSqlQuery>
#include <QCache>

namespace  hoot
{

/**
 * Reads implicit tag rules from a Sqlite database
 */
class ImplicitTagRulesSqliteReader : public Configurable
{
public:

  ImplicitTagRulesSqliteReader();
  ~ImplicitTagRulesSqliteReader();

  /**
   * Opens a Sqlite implicit tag rules database
   *
   * @param url location of the database to open
   */
  void open(const QString url);

  /**
   * Closes a Sqlite implicit tag rules database
   */
  void close();

  virtual void setConfiguration(const Settings& conf);

  /**
   *
   *
   * @param words a collection of words for which to retrieve implicitly derived tags
   * @param matchingWords a collection of words found in the implicit tag database
   * @param wordsInvolvedInMultipleRules true if the collection of words are associated with more
   * than one set of tags
   * @return a set of implicitly derived tags if they exists for given inputs and the input words
   * are not associated with more than one set of tags; an empty tag set otherwise
   */
  Tags getImplicitTags(const QSet<QString>& words, QSet<QString>& matchingWords,
                       bool& wordsInvolvedInMultipleRules);

  /**
   * Retrieves total number of word/tag associations in the database
   *
   * @return a rule word part count
   */
  long getRuleWordPartCount();

  int getTagsCacheSize() const { return _tagsCache.size(); }
  long getFirstRoundTagsCacheHits() const { return _firstRoundTagsCacheHits; }
  long getSecondRoundTagsCacheHits() const { return _secondRoundTagsCacheHits; }

  void setCustomRules(const ImplicitTagCustomRules& rules) { _customRules = rules; }
  void setAddTopTagOnly(bool add) { _addTopTagOnly = add; }
  void setAllowWordsInvolvedInMultipleRules(bool allow)
  { _allowWordsInvolvedInMultipleRules = allow; }

private:

  QString _path;
  QSqlDatabase _db;

  QSqlQuery _ruleWordPartCountQuery;
  QSqlQuery _tagsForWordIds;
  QSqlQuery _tagCountsForWordIds;

  QCache<QString, Tags> _tagsCache;
  long _firstRoundTagsCacheHits;
  long _secondRoundTagsCacheHits;
  bool _addTopTagOnly;
  bool _allowWordsInvolvedInMultipleRules;

  ImplicitTagCustomRules _customRules;

  void _prepareQueries();
};

}

#endif // IMPLICITTAGRULESSQLITEREADER_H
