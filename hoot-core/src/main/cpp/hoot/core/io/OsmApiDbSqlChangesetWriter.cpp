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
#include "OsmApiDbSqlChangesetWriter.h"

// hoot
#include <hoot/core/util/GeometryUtils.h>

// Qt
#include <QtSql/QSqlDatabase>
#include <QUrl>
#include <QSqlError>
#include <QDateTime>

namespace hoot
{

OsmApiDbSqlChangesetWriter::OsmApiDbSqlChangesetWriter(const QUrl targetDatabaseUrl)
{
  if (!_db.isSupported(targetDatabaseUrl))
  {
    throw HootException("Unsupported URL: " + targetDatabaseUrl.toString());
  }
  _db.open(targetDatabaseUrl);

  _initChangesetStats();
}

OsmApiDbSqlChangesetWriter::~OsmApiDbSqlChangesetWriter()
{
  _db.close();
}

void OsmApiDbSqlChangesetWriter::_initChangesetStats()
{
  _changesetStats.clear();
}

void OsmApiDbSqlChangesetWriter::write(const QString sql)
{
  LOG_INFO("Executing changeset SQL queries against OSM API database...");

  _db.transaction();

  QString changesetInsertStatement = "";
  QString elementSqlStatements = "";

  const QStringList sqlParts = sql.split(";");

  if (!sqlParts[0].toUpper().startsWith("INSERT INTO CHANGESETS"))
  {
    throw HootException(
      "The first SQL statement in a changeset SQL file must create a changeset.");
  }

  for (int i = 0; i < sqlParts.size(); i++)
  {
    const QString sqlStatement = sqlParts[i];
    //LOG_VARD(sqlStatement);
    QString changesetStatType;
    if (sqlStatement.toUpper().startsWith("INSERT INTO CHANGESETS"))
    {
      //flush
      if (!changesetInsertStatement.isEmpty())
      {
        if (elementSqlStatements.trimmed().isEmpty())
        {
          throw HootException("No element SQL statements changeset.");
        }

        //had problems here when trying to prepare these queries (should they be?); the changeset writing
        //needs to be done before the element writing, hence the separate queries
        _execNoPrepare(changesetInsertStatement);
        _execNoPrepare(elementSqlStatements);

        changesetInsertStatement = "";
        elementSqlStatements = "";
      }

      changesetInsertStatement = sqlStatement + ";";
      changesetStatType = "changeset-create";
    }
    else
    {
      elementSqlStatements += sqlStatement + ";";

      //The sql changeset is made up of one or more sql statements for each changeset operation type.
      //Each operation starts with a comment header (e.g. /* node create 1 */), which can be used
      //to determine its type.
      if (sqlStatement.startsWith("\n/*"))
      {
        const QStringList statementParts = sqlStatement.trimmed().split(" ");
        assert(statementParts.length() >= 3);
        changesetStatType = statementParts[2] + "-" + statementParts[1]; //e.g. "node-create"
        //LOG_VARD(changesetStatType);
      }
      else if (sqlStatement.contains("UPDATE changesets"))
      {
        //some tight coupling here to OsmChangesetSqlFileWriter
        changesetStatType = "";
        _changesetDetailsStr = sqlStatement.split("SET")[1].split("WHERE")[0].trimmed();
        //need to do some extra processing here to convert the coords in the string from ints to
        //decimals
        const QRegExp regex(
          "min_lat=(-*[0-9]+), max_lat=(-*[0-9]+), min_lon=(-*[0-9]+), max_lon=(-*[0-9]+)");
        const int pos = regex.indexIn(_changesetDetailsStr);
        const QStringList captures = regex.capturedTexts();
        if (pos > -1)
        {
          //first capture is the whole string...skip it
          for (int i = 1; i < 5; i++)
          {
            const QString oldCoordStr = captures.at(i);
            const double newCoord = OsmApiDb::fromOsmApiDbCoord(oldCoordStr.toLong());
            _changesetDetailsStr=
              _changesetDetailsStr.replace(
                oldCoordStr, QString::number(newCoord, ConfigOptions().getWriterPrecision()));
          }
        }
      }
    }
    if (_changesetStats.contains(changesetStatType))
    {
      _changesetStats[changesetStatType] = _changesetStats[changesetStatType] + 1;
    }
    else
    {
      _changesetStats[changesetStatType] = 1;
    }
  }

  //flush
  if (!changesetInsertStatement.isEmpty())
  {
    if (elementSqlStatements.trimmed().isEmpty())
    {
      throw HootException("No element SQL statements changeset.");
    }

    _execNoPrepare(changesetInsertStatement);
    _execNoPrepare(elementSqlStatements);

    changesetInsertStatement = "";
    elementSqlStatements = "";
  }

  _db.commit();

  LOG_INFO("Changeset SQL queries execute finished against OSM API database.");
}

void OsmApiDbSqlChangesetWriter::write(QFile& changesetSqlFile)
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

QString OsmApiDbSqlChangesetWriter::getChangesetStats() const
{
  //LOG_VARD(_changesetDetailsStr);
  return
    "Changeset(s) Created: " + QString::number(_changesetStats["changeset-create"]) + "\n" +
    "Changeset Details: " + _changesetDetailsStr + "\n" +
    "Node(s) Created: " + QString::number(_changesetStats["node-create"]) + "\n" +
    "Node(s) Modified: " + QString::number(_changesetStats["node-modify"]) + "\n" +
    "Node(s) Deleted: " + QString::number(_changesetStats["node-delete"]) + "\n" +
    "Way(s) Created: " + QString::number(_changesetStats["way-create"]) + "\n" +
    "Way(s) Modified: " + QString::number(_changesetStats["way-modify"]) + "\n" +
    "Way(s) Deleted: " + QString::number(_changesetStats["way-delete"]) + "\n" +
    "Relation(s) Created: " + QString::number(_changesetStats["relation-create"]) + "\n" +
    "Relation(s) Modified: " + QString::number(_changesetStats["relation-modify"]) + "\n" +
    "Relation(s) Deleted: " + QString::number(_changesetStats["relation-delete"]) + "\n";
}

bool OsmApiDbSqlChangesetWriter::conflictExistsInTarget(const QString boundsStr, const QString timeStr)
{
  LOG_INFO("Checking for OSM API DB conflicts for changesets within " << boundsStr << " and " <<
           "created after " << timeStr << "...");

  const Envelope bounds = GeometryUtils::envelopeFromConfigString(boundsStr);
  LOG_VARD(bounds.toString());

  const QDateTime time = QDateTime::fromString(timeStr, OsmApiDb::TIME_FORMAT);
  LOG_VARD(time);
  if (!time.isValid())
  {
    throw HootException(
      "Invalid timestamp: " + time.toString() + ".  Should be of the form " + OsmApiDb::TIME_FORMAT);
  }

  shared_ptr<QSqlQuery> changesetItr = _db.getChangesetsCreatedAfterTime(timeStr);
  while (changesetItr->next())
  {
    LOG_VARD(changesetItr->value(0).toLongLong());
    LOG_VARD(changesetItr->value(1).toLongLong());
    LOG_VARD(changesetItr->value(2).toLongLong());
    LOG_VARD(changesetItr->value(3).toLongLong());
    Envelope changesetBounds(
      changesetItr->value(0).toLongLong() / (double)ApiDb::COORDINATE_SCALE,
      changesetItr->value(1).toLongLong() / (double)ApiDb::COORDINATE_SCALE,
      changesetItr->value(2).toLongLong() / (double)ApiDb::COORDINATE_SCALE,
      changesetItr->value(3).toLongLong() / (double)ApiDb::COORDINATE_SCALE);
    LOG_VARD(changesetBounds.toString());
    if (changesetBounds.intersects(bounds))
    {
      //logging as info instead of error since I couldn't get the log output disabled in the test
      //with the log disabler
      LOG_INFO(
        "Conflict exists at bounds " << changesetBounds.toString() << "for input bounds " <<
        boundsStr << " and input time " << timeStr);
      return true;
    }
  }
  LOG_INFO("No conflicts exist for input bounds " << boundsStr << " and input time " << timeStr);
  return false;
}

void OsmApiDbSqlChangesetWriter::_execNoPrepare(const QString sql)
{
  QSqlQuery q(_db.getDB());
  LOG_VARD(sql);
  if (q.exec(sql) == false)
  {
    throw HootException(
      QString("Error executing query: %1 (%2)").arg(q.lastError().text()).arg(sql));
  }
  LOG_VARD(q.numRowsAffected());
}

}
