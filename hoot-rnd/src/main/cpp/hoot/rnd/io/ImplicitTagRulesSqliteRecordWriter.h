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
#ifndef IMPLICITTAGRULESSQLITERECORDWRITER_H
#define IMPLICITTAGRULESSQLITERECORDWRITER_H

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
class ImplicitTagRulesSqliteRecordWriter : public ImplicitTagRuleWordPartWriter
{

public:

  static std::string className() { return "hoot::ImplicitTagRulesSqliteRecordWriter"; }

  ImplicitTagRulesSqliteRecordWriter();
  ~ImplicitTagRulesSqliteRecordWriter();

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
  QSqlQuery _getWordIdForWordQuery;
  QSqlQuery _getTagIdForTagQuery;

  //TODO: replace with BigMap
  QMap<QString, long> _wordsToWordIds;
  QMap<QString, long> _tagsToTagIds;

  bool _runInMemory;

  long _statusUpdateInterval;

  void _createTables();
  void _prepareQueries();

  long _insertWord(const QString word);
  long _insertTag(const QString kvp);
  void _insertRuleWordPart(const long wordId, const long tagId, const long tagOccurranceCount);
  long _getWordIdForWord(const QString word);
  long _getTagIdForTag(const QString kvp);
};

}

#endif // IMPLICITTAGRULESSQLITERECORDWRITER_H
