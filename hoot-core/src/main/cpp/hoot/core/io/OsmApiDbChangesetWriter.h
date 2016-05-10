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
#ifndef OSMAPIDBCHANGESETWRITER_H
#define OSMAPIDBCHANGESETWRITER_H

#include "OsmApiDb.h"

namespace hoot
{

/**
 * Writes OSM changesets from a .osc.sql file to an OSM API database
 */
class OsmApiDbChangesetWriter
{

public:

  OsmApiDbChangesetWriter(const QUrl targetDatabaseUrl);

  ~OsmApiDbChangesetWriter();

  /**
   * Executes changeset SQL against an OSM API database.
   *
   * @param changesetSqlFile The file containing the change SQL to be executed.
   */
  void write(QFile& changesetSqlFile);

  /**
   * Executes changeset SQL against an OSM API database.
   *
   * @param sql The SQL to be executed.
   */
  void write(const QString sql);

  /**
   * Detects conflicts in the target OSM API database by examining its changesets and bounds and
   * time input parameters.
   *
   * @param boundsStr a bounds string of the form minx,maxx,miny,maxy for which to search
   * within the target database for changesets
   * @param timeStr a time string of the format specified by the OsmApiDb::TIME_FORMAT constant;
   * only searches for changesets created after the time
   * @return
   */
  bool conflictExistsInTarget(const QString boundsStr, const QString timeStr);

private:

  void _execNoPrepare(const QString sql);

  OsmApiDb _db;
};

}

#endif // OSMAPIDBCHANGESETWRITER_H
