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
#include <QHash>
#include <QtSql/QSqlDatabase>
#include <QSqlQuery>
#include <QSet>

namespace hoot
{

/**
 * Writes implicit tag rules ot a Sqlite database
 */
class ImplicitTagRulesSqliteWriter
{

public:

  ImplicitTagRulesSqliteWriter();
  ~ImplicitTagRulesSqliteWriter();

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
  virtual void write(const QString inputUrl);

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

  QHash<QString, long> _wordsToWordIds;
  QHash<QString, long> _tagsToTagIds;
  QSet<QString> _words;

  long _statusUpdateInterval;

  void _createTables();
  void _prepareQueries();

  long _insertWord(const QString word);
  long _insertTag(const QString kvp);
  void _insertRuleWordPart(const long wordId, const long tagId, const long tagOccurrenceCount);
  void _createIndexes();
};

}

#endif // IMPLICITTAGRULESSQLITEWRITER_H
