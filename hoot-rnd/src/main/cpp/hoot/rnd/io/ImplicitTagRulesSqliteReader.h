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

// Qt
#include <QString>
#include <QtSql/QSqlDatabase>
#include <QSqlQuery>

namespace  hoot
{

/**
 * Reads implicit tag rules from a Sqlite database
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
  void open(const QString url);

  /**
   * Closes a Sqlite implicit tag rules database
   */
  void close();

  /**
   * Determines whether members of a set of words (either single words or word phrases) belong
   * to multiple implicit tag rules
   *
   * @param words collection of words to determine membership for
   * @param matchingRuleWords if any of the input words are involved in multiple rules, the
   * actual collection of words that are involved with the multiple rules; an empty collection
   * if the input words were not involved in multiple rules
   * @return true if the input words are involved in multiple implicit tag rules; false otherwise
   */
  bool wordsInvolveMultipleRules(const QSet<QString>& words, QSet<QString>& matchingRuleWords);

  /**
   * Returns a set of tags derived implicitly given a set of words (either single words or word
   * phrases)
   *
   * @param words collection of words to retrieve implicit tags for
   * @param matchingWords if any of the input words are associated with an implicit tag rule,
   * the actual collection of words that were involved with a rule; an empty collection if none
   * of the input words were associated with a rule
   * @return
   */
  Tags getImplicitTags(const QSet<QString>& words, QSet<QString>& matchingWords);

  /**
   * Retrieves total number of implicit tag rules in the database
   *
   * @return total rule count
   */
  long getRuleCount();

private:

  QString _path;
  QSqlDatabase _db;

  QSqlQuery _ruleCountQuery;
  long _currentRuleId;

  void _prepareQueries();
};

}

#endif // IMPLICITTAGRULESSQLITEREADER_H
