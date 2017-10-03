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
#ifndef IMPLICITTAGRULESSQLITEWRITER_H
#define IMPLICITTAGRULESSQLITEWRITER_H

// Qt
#include <QString>
#include <QMap>
#include <QtSql/QSqlDatabase>
#include <QSqlQuery>
#include <QSet>

namespace hoot
{

/**
 *
 */
class ImplicitTagRulesSqliteWriter
{

public:

  ImplicitTagRulesSqliteWriter();
  ~ImplicitTagRulesSqliteWriter();

  void open(const QString input);

  void write(QMap<QString, QMap<QString, long> > rules);

  void close();

private:

  QSqlDatabase _db;

  QSqlQuery _insertRuleQuery;
  QSqlQuery _insertWordQuery;
  QSqlQuery _insertTagQuery;
  QSqlQuery _selectTagIdForKvpQuery;
  QSqlQuery _getLastWordIdQuery;
  QSqlQuery _getLastTagIdQuery;
  QSqlQuery _numTagsForRuleQuery;

  int _currentRuleId;

  void _createTables();
  void _prepareQueries();

  int _insertWord(const QString word);
  int _insertTag(const QString kvp);
  void _insertRuleRecord(const int ruleId, const int wordId, const int tagId);
  int _selectTagIdForKvp(const QString kvp);
  QSet<int> _selectRuleIdsContainingAllTags(const QSet<QString>& kvps);
  QSet<int> _selectTagIdsForKvps(const QSet<QString>& kvps);
  int _getNumTagsForRule(const int ruleId);
};

}

#endif // IMPLICITTAGRULESSQLITEWRITER_H
