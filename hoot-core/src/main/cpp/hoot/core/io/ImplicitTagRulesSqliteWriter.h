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
 * @copyright Copyright (C) 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
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
 * Writes implicit tag rules to a Sqlite implicit tag rules database
 */
class ImplicitTagRulesSqliteWriter
{

public:

  ImplicitTagRulesSqliteWriter();
  ~ImplicitTagRulesSqliteWriter();

  /**
   * Determines if the output location is a writable format
   *
   * @param outputUrl output database location
   * @return true if the output location is supported; false otherwise
   */
  bool isSupported(const QString& outputUrl);

  /**
   * Opens the output rules database
   *
   * @param outputUrl output rules database
   */
  void open(const QString& outputUrl);

  /**
   * Writes rules from the input file to the output database
   *
   * @param inputUrl raw implicit tag rules input file
   */
  void write(const QString& inputUrl);

  /**
   * Close the writer
   */
  void close();

private:

  QSqlDatabase _db;

  QSqlQuery _insertRuleQuery;
  QSqlQuery _insertWordQuery;
  QSqlQuery _insertTagQuery;
  //returns the ID of the last word inserted
  QSqlQuery _getLastWordIdQuery;
  //returns the ID of the last tag inserted
  QSqlQuery _getLastTagIdQuery;

  //cache word strings to their database ids
  QHash<QString, long> _wordsToWordIds;
  //cache tag key=value strings to their database ids
  QHash<QString, long> _tagsToTagIds;
  QSet<QString> _words;

  long _statusUpdateInterval;

  void _createTables();
  void _prepareQueries();

  long _insertWord(const QString& word);
  long _insertTag(const QString& kvp);
  void _insertRule(const long wordId, const long tagId, const long tagOccurrenceCount);
  void _createIndexes();
};

}

#endif // IMPLICITTAGRULESSQLITEWRITER_H
