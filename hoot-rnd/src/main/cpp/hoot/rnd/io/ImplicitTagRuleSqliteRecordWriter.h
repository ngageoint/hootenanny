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
#ifndef IMPLICITTAGRULESQLITERECORDWRITER_H
#define IMPLICITTAGRULESQLITERECORDWRITER_H

// Qt
#include <QString>
#include <QMap>
#include <QtSql/QSqlDatabase>
#include <QSqlQuery>
#include <QSet>

namespace hoot
{

/**
 * Writes implicit tag rules ot a Sqlite database
 */
class ImplicitTagRuleSqliteRecordWriter
{

public:

  ImplicitTagRuleSqliteRecordWriter();
  ~ImplicitTagRuleSqliteRecordWriter();

  void open(const QString url);

  void write(const QString word, const QString kvp);

  void close();

private:

  QSqlDatabase _db;

  QSqlQuery _insertRuleQuery;
  QSqlQuery _insertWordQuery;
  QSqlQuery _insertTagQuery;
  QSqlQuery _getLastWordIdQuery;
  QSqlQuery _getLastTagIdQuery;
  QSqlQuery _getWordIdForWord;
  QSqlQuery _getTagIdForTag;
  QSqlQuery _getRuleIdForWordKvp;
  QSqlQuery _updateRuleTagCountQuery;

  long _currentRuleId;

  void _createTables();
  void _prepareQueries();

  long _insertWord(const QString word);
  long _insertTag(const QString kvp);
  void _insertRuleRecord(const long ruleId, const long wordId, const long tagId,
                         const long ruleCount);
  long _getWordId(const QString word);
  long _getTagId(const QString kvp);
  long _getRuleId(const QString word, const QString kvp, long& count);
  void _updateRuleTagCount(const long ruleId, const long wordId, const long tagId,
                           const long count);
};

}

#endif // IMPLICITTAGRULESQLITERECORDWRITER_H
