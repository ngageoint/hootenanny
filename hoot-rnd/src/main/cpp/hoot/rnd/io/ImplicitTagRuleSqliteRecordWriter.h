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

// Hoot
#include <hoot/rnd/io/ImplicitTagRuleWordPartWriter.h>

// Qt
#include <QString>
#include <QMap>
#include <QtSql/QSqlDatabase>
#include <QSqlQuery>

namespace hoot
{

/**
 * Writes implicit tag rules ot a Sqlite database
 */
class ImplicitTagRuleSqliteRecordWriter : public ImplicitTagRuleWordPartWriter
{

public:

  static std::string className() { return "hoot::ImplicitTagRuleSqliteRecordWriter"; }

  ImplicitTagRuleSqliteRecordWriter();
  ~ImplicitTagRuleSqliteRecordWriter();

  /**
   * @see ImplicitTagRuleWordPartWriter
   */
  virtual bool isSupported(const QString outputUrl);

  /**
   * @see ImplicitTagRuleWordPartWriter
   */
  virtual void open(const QString outputUrl);

  /**
   * @see ImplicitTagRuleWordPartWriter
   */
  virtual void write(const QString inputUrl, const long totalParts = -1);

  /**
   * @see ImplicitTagRuleWordPartWriter
   */
  virtual void close();

private:

  QSqlDatabase _db;

  QSqlQuery _insertRuleQuery;
  QSqlQuery _insertWordQuery;
  QSqlQuery _insertTagQuery;
  QSqlQuery _getLastWordIdQuery;
  QSqlQuery _getLastTagIdQuery;

  long _currentRuleId;
  QMap<QString, long> _wordsToWordIds;
  QMap<QString, long> _tagsToTagIds;
  QMap<QString, long> _wordIdTagIdsToRuleIds;

  void _createTables();
  void _prepareQueries();

  long _insertWord(const QString word);
  long _insertTag(const QString kvp);
  void _insertRuleRecord(const long ruleId, const long wordId, const long tagId,
                         const long wordTagOccurranceCount);
};

}

#endif // IMPLICITTAGRULESQLITERECORDWRITER_H
