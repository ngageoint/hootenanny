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
#ifndef POIIMPLICITTAGRULESDERIVERTEMPDBWRITER_H
#define POIIMPLICITTAGRULESDERIVERTEMPDBWRITER_H

// Hoot


// Qt
#include <QString>
//#include <QMap>
#include <QtSql/QSqlDatabase>
#include <QSqlQuery>
#include <QTemporaryFile>

namespace hoot
{

/**
 *
 */
class PoiImplicitTagRulesDeriverTempDbWriter
{

public:

  PoiImplicitTagRulesDeriverTempDbWriter();
  ~PoiImplicitTagRulesDeriverTempDbWriter();

  long insertWord(const QString word);
  long insertTagKey(const QString tagKey);
  void insertWordTagKey(const long wordId, const long tagKeyId, const long tagCount);
  QString getWord(const QString word);
  long getWordTagKeyCount(const long wordId, const long tagKeyId);
  long getWordIdForWord(const QString word);
  long getTagKeyIdForTagKey(const QString tagKey);

  void close();

private:

  boost::shared_ptr<QTemporaryFile> _tempDbFile;
  QSqlDatabase _db;
  QSqlQuery _insertWordQuery;
  QSqlQuery _insertTagKeyQuery;
  QSqlQuery _insertWordTagKeyQuery;
  QSqlQuery _getWordQuery;
  QSqlQuery _getWordTagKeyCountQuery;
  QSqlQuery _getWordIdForWordQuery;
  QSqlQuery _getTagKeyIdForTagKeyQuery;
  QSqlQuery _getLastWordIdQuery;
  QSqlQuery _getLastTagKeyIdQuery;

  void _createTempTables();
  void _prepareQueries();
};

}

#endif // POIIMPLICITTAGRULESDERIVERTEMPDBWRITER_H
