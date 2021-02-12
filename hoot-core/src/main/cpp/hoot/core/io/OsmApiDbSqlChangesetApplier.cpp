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
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2016, 2017, 2019, 2020 Maxar (http://www.maxar.com/)
 */
#include "OsmApiDbSqlChangesetApplier.h"

// hoot
#include <hoot/core/geometry/GeometryUtils.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/DbUtils.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/StringUtils.h>

// Qt
#include <QtSql/QSqlDatabase>
#include <QUrl>
#include <QSqlError>
#include <QDateTime>

using namespace geos::geom;

namespace hoot
{

const QString OsmApiDbSqlChangesetApplier::CHANGESET_CREATE_KEY = "changeset-create";
const QString OsmApiDbSqlChangesetApplier::NODE_CREATE_KEY = "node-create";
const QString OsmApiDbSqlChangesetApplier::NODE_MODIFY_KEY = "node-modify";
const QString OsmApiDbSqlChangesetApplier::NODE_DELETE_KEY = "node-delete";
const QString OsmApiDbSqlChangesetApplier::WAY_CREATE_KEY = "way-create";
const QString OsmApiDbSqlChangesetApplier::WAY_MODIFY_KEY = "way-modify";
const QString OsmApiDbSqlChangesetApplier::WAY_DELETE_KEY = "way-delete";
const QString OsmApiDbSqlChangesetApplier::RELATION_CREATE_KEY = "relation-create";
const QString OsmApiDbSqlChangesetApplier::RELATION_MODIFY_KEY = "relation-modify";
const QString OsmApiDbSqlChangesetApplier::RELATION_DELETE_KEY = "relation-delete";
const QString OsmApiDbSqlChangesetApplier::TOTAL_CREATE_KEY = "total-create";
const QString OsmApiDbSqlChangesetApplier::TOTAL_MODIFY_KEY = "total-modify";
const QString OsmApiDbSqlChangesetApplier::TOTAL_DELETE_KEY = "total-delete";

OsmApiDbSqlChangesetApplier::OsmApiDbSqlChangesetApplier(const QUrl targetDatabaseUrl) :
_precision(ConfigOptions().getWriterPrecision())
{
  if (!_db.isSupported(targetDatabaseUrl))
  {
    throw HootException("Unsupported URL: " + targetDatabaseUrl.toString(QUrl::RemoveUserInfo));
  }
  _db.open(targetDatabaseUrl);

  _initChangesetStats();
}

OsmApiDbSqlChangesetApplier::~OsmApiDbSqlChangesetApplier()
{
  _db.close();
}

void OsmApiDbSqlChangesetApplier::_initChangesetStats()
{
  _changesetStats.clear();
}

void OsmApiDbSqlChangesetApplier::write(const QString& sql)
{
  LOG_DEBUG("Executing changeset SQL queries against OSM API database...");
  LOG_VARD(sql.length());

  _db.transaction();

  QString changesetInsertStatement = "";
  QString elementSqlStatements = "";

  const QStringList sqlParts = sql.split(";");
  LOG_VARD(sqlParts.length());

  if (!sqlParts[0].toUpper().startsWith("INSERT INTO CHANGESETS"))
  {
    throw HootException(
      "The first SQL statement in a changeset SQL file must create a changeset. Found: " +
      sqlParts[0].left(25));
  }

  for (int i = 0; i < sqlParts.size(); i++)
  {
    const QString sqlStatement = sqlParts[i];
    LOG_VART(sqlStatement);
    QString changesetStatType;
    if (sqlStatement.toUpper().startsWith("INSERT INTO CHANGESETS"))
    {
      // flush
      if (!changesetInsertStatement.isEmpty())
      {
        if (elementSqlStatements.trimmed().isEmpty())
        {
          throw HootException("No element SQL statements changeset.");
        }

        // had problems here when trying to prepare these queries (should they be?); the changeset
        // writing needs to be done before the element writing, hence the separate queries
        DbUtils::execNoPrepare(_db.getDB(), changesetInsertStatement);
        DbUtils::execNoPrepare(_db.getDB(), elementSqlStatements);

        changesetInsertStatement = "";
        elementSqlStatements = "";
      }

      changesetInsertStatement = sqlStatement + ";";
      changesetStatType = CHANGESET_CREATE_KEY;
    }
    else
    {
      elementSqlStatements += sqlStatement + ";";

      // The sql changeset is made up of one or more sql statements for each changeset operation
      // type. Each operation starts with a comment header (e.g. /* node create 1 */), which can be
      // used to determine its type.
      if (sqlStatement.startsWith("\n/*"))
      {
        const QStringList statementParts = sqlStatement.trimmed().split(" ");
        assert(statementParts.length() >= 3);
        // e.g. "node-create"; note this could be at odds with the string key constants if they are
        // changed...don't see that happening
        changesetStatType = statementParts[2] + "-" + statementParts[1];
        LOG_VART(changesetStatType);
      }
      else if (sqlStatement.contains("UPDATE " + ApiDb::getChangesetsTableName()))
      {
        // some tight coupling here to OsmChangesetSqlFileWriter
        changesetStatType = "";
        _changesetDetailsStr = sqlStatement.split("SET")[1].split("WHERE")[0].trimmed();
        // need to do some extra processing here to convert the coords in the string from ints to
        // decimals
        const QRegExp regex(
          "min_lat=(-*[0-9]+), max_lat=(-*[0-9]+), min_lon=(-*[0-9]+), max_lon=(-*[0-9]+)");
        const int pos = regex.indexIn(_changesetDetailsStr);
        const QStringList captures = regex.capturedTexts();
        if (pos > -1)
        {
          // first capture is the whole string...skip it
          for (int i = 1; i < 5; i++)
          {
            const QString oldCoordStr = captures.at(i);
            const double newCoord = OsmApiDb::fromOsmApiDbCoord(oldCoordStr.toLong());
            _changesetDetailsStr=
              _changesetDetailsStr.replace(
                oldCoordStr, QString::number(newCoord, 'g', _precision));
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
  _changesetStats["total-create"] =
    _changesetStats["node-create"] + _changesetStats["way-create"] +
    _changesetStats["relation-create"];
  _changesetStats["total-modify"] =
    _changesetStats["node-modify"] + _changesetStats["way-modify"] +
    _changesetStats["relation-modify"];
  _changesetStats["total-delete"] =
    _changesetStats["node-delete"] + _changesetStats["way-delete"] +
    _changesetStats["relation-delete"];

  // flush
  if (!changesetInsertStatement.isEmpty())
  {
    if (elementSqlStatements.trimmed().isEmpty())
    {
      throw HootException("No element SQL statements changeset.");
    }

    DbUtils::execNoPrepare(_db.getDB(), changesetInsertStatement);
    DbUtils::execNoPrepare(_db.getDB(), elementSqlStatements);

    changesetInsertStatement = "";
    elementSqlStatements = "";
  }

  _db.commit();

  LOG_DEBUG("Changeset SQL queries execute finished against OSM API database.");
}

void OsmApiDbSqlChangesetApplier::write(QFile& changesetSqlFile)
{
  if (!changesetSqlFile.fileName().endsWith(".osc.sql"))
  {
    throw IllegalArgumentException("Invalid file type: " + changesetSqlFile.fileName());
  }

  if (changesetSqlFile.open(QIODevice::ReadOnly))
  {
    try
    {
      write(changesetSqlFile.readAll());
      changesetSqlFile.close();
    }
    catch (const HootException& e)
    {
      changesetSqlFile.close();
      throw HootException(
        "Error executing query from file: " + changesetSqlFile.fileName() + "; Error: " +
        e.getWhat());
    }
  }
  else
  {
    throw HootException("Unable to open changeset file: " + changesetSqlFile.fileName());
  }
}

QString OsmApiDbSqlChangesetApplier::getChangesetStatsSummary() const
{
  return
    "Changeset(s) Created: " + StringUtils::formatLargeNumber(_changesetStats[CHANGESET_CREATE_KEY]) + "\n" +
    "Changeset Details: " + _changesetDetailsStr + "\n" +
    "Node(s) Created: " + StringUtils::formatLargeNumber(_changesetStats[NODE_CREATE_KEY]) + "\n" +
    "Node(s) Modified: " + StringUtils::formatLargeNumber(_changesetStats[NODE_MODIFY_KEY]) + "\n" +
    "Node(s) Deleted: " + StringUtils::formatLargeNumber(_changesetStats[NODE_DELETE_KEY]) + "\n" +
    "Way(s) Created: " + StringUtils::formatLargeNumber(_changesetStats[WAY_CREATE_KEY]) + "\n" +
    "Way(s) Modified: " + StringUtils::formatLargeNumber(_changesetStats[WAY_MODIFY_KEY]) + "\n" +
    "Way(s) Deleted: " + StringUtils::formatLargeNumber(_changesetStats[WAY_DELETE_KEY]) + "\n" +
    "Relation(s) Created: " + StringUtils::formatLargeNumber(_changesetStats[RELATION_CREATE_KEY]) + "\n" +
    "Relation(s) Modified: " + StringUtils::formatLargeNumber(_changesetStats[RELATION_MODIFY_KEY]) + "\n" +
    "Relation(s) Deleted: " + StringUtils::formatLargeNumber(_changesetStats[RELATION_DELETE_KEY]) + "\n" +
    "Total Creations: " + StringUtils::formatLargeNumber(_changesetStats[TOTAL_CREATE_KEY]) + "\n" +
    "Total Modifications: " + StringUtils::formatLargeNumber(_changesetStats[TOTAL_MODIFY_KEY]) + "\n" +
    "Total Deletions: " + StringUtils::formatLargeNumber(_changesetStats[TOTAL_DELETE_KEY]) + "\n";
}

bool OsmApiDbSqlChangesetApplier::conflictExistsInTarget(const QString& boundsStr,
                                                         const QString& timeStr)
{
  LOG_DEBUG("Checking for OSM API DB conflicts for " << ApiDb::getChangesetsTableName() <<
           " within " << boundsStr << " and created after " << timeStr << "...");

  const Envelope bounds = GeometryUtils::envelopeFromString(boundsStr);
  LOG_VARD(bounds.toString());

  const QDateTime time = QDateTime::fromString(timeStr, OsmApiDb::TIME_FORMAT);
  LOG_VARD(time);
  if (!time.isValid())
  {
    throw HootException(
      "Invalid timestamp: " + time.toString() + ".  Should be of the form " + OsmApiDb::TIME_FORMAT);
  }

  std::shared_ptr<QSqlQuery> changesetItr = _db.getChangesetsCreatedAfterTime(timeStr);
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
      // logging as info instead of error since I couldn't get the log output disabled in the test
      // with the log disabler
      LOG_INFO(
        "Conflict exists at bounds " << changesetBounds.toString() << "for input bounds " <<
        boundsStr << " and input time " << timeStr);
      return true;
    }
  }
  LOG_DEBUG("No conflicts exist for input bounds " << boundsStr << " and input time " << timeStr);
  return false;
}

}
