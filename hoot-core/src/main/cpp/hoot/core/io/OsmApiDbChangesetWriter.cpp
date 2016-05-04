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
 * @copyright Copyright (C) 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "OsmApiDbChangesetWriter.h"

// hoot
#include <hoot/core/util/GeometryUtils.h>

// Qt
#include <QtSql/QSqlDatabase>
#include <QUrl>
#include <QSqlError>

namespace hoot
{

OsmApiDbChangesetWriter::OsmApiDbChangesetWriter(const QUrl targetDatabaseUrl)
{
  if (!_db.isSupported(targetDatabaseUrl))
  {
    throw HootException("Unsupported URL: " + targetDatabaseUrl.toString());
  }
  _db.open(targetDatabaseUrl);
}

OsmApiDbChangesetWriter::~OsmApiDbChangesetWriter()
{
  _db.close();
}

void OsmApiDbChangesetWriter::write(const QString sql)
{
  LOG_INFO("Executing changeset SQL queries against OSM API database...");

  QString changesetInsertStatement;
  QString elementSqlStatements = "";

  const QStringList sqlParts = sql.split(";");
  for (int i = 0; i < sqlParts.size(); i++)
  {
    const QString sqlStatement = sqlParts[i];
    if (i == 0)
    {
      if (!sqlStatement.toUpper().startsWith("INSERT INTO CHANGESETS"))
      {
        throw HootException(
          "The first SQL statement in a changeset SQL file must create a changeset.");
      }
      else
      {
        changesetInsertStatement = sqlStatement + ";";
      }
    }
    else
    {
      elementSqlStatements += sqlStatement + ";";
    }
  }

  if (elementSqlStatements.trimmed().isEmpty())
  {
    throw HootException("No element SQL statements in sql file");
  }

  _db.transaction();
  //had problems here when trying to prepare these queries; the changeset writing needs to be done
  //before the element writing
  _execNoPrepare(changesetInsertStatement);
  _execNoPrepare(elementSqlStatements);
  _db.commit();

  LOG_INFO("Changeset SQL queries execute finished against OSM API database.");
}

void OsmApiDbChangesetWriter::write(QFile& changesetSqlFile)
{
  if (!changesetSqlFile.fileName().endsWith(".osc.sql"))
  {
    throw HootException("Invalid file type: " + changesetSqlFile.fileName());
  }

  if (changesetSqlFile.open(QIODevice::ReadOnly))
  {
    write(changesetSqlFile.readAll());
    changesetSqlFile.close();
  }
  else
  {
    throw HootException("Unable to open changeset file: " + changesetSqlFile.fileName());
  }
}

bool OsmApiDbChangesetWriter::conflictExistsInTarget(const QString bbox,
                                                     const QString timestamp)
{
  //TODO: validate timestamp


  return
    _db.getChangesetsCountWithinBoundsAfterTimestamp(GeometryUtils::envelopeFromString(bbox), timestamp) > 0;
}

void OsmApiDbChangesetWriter::_execNoPrepare(const QString sql)
{
  QSqlQuery q(_db.getDB());
  LOG_VARD(sql);

  if (q.exec(sql) == false)
  {
    throw HootException(QString("Error executing query: %1 (%2)").arg(q.lastError().text()).arg(sql));
  }
  LOG_VARD(q.numRowsAffected());
}

}
