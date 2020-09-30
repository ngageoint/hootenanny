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
 * @copyright Copyright (C) 2015, 2017, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef DBUTILS_H
#define DBUTILS_H

// Qt
#include <QString>
#include <QtSql/QSqlDatabase>

namespace hoot
{

/**
 * Class meant for hoot-agnostic database common utility functions
 */
class DbUtils
{
public:

  /**
   * Executes the provided SQL statement without calling prepare. This is handy when creating
   * constraints, tables, etc.
   *
   * @param database the database to execute the query against
   * @param sql SQL to execute.
   * @return the executed query
   */
  static QSqlQuery execNoPrepare(const QSqlDatabase& database, const QString& sql);

  /**
   * Get the number of rows in the table
   *
   * @param database the database to execute the query against
   * @param tableName name of the table to determine the row count for
   * @return number of rows
   */
  static long getRowCount(const QSqlDatabase& database, const QString& tableName);

  static QStringList getConstraintsForTable(const QSqlDatabase& database, const QString& tableName);

  /**
   * Disables all existing database constraints on the specified table
   *
   * @param database database to modify
   * @param tableName table to modify
   */
  static void disableTableConstraints(const QSqlDatabase& database, const QString& tableName);

  /**
   * Enables all existing database constraints on the specified table
   *
   * @param database database to modify
   * @param tableName table to modify
   */
  static void enableTableConstraints(const QSqlDatabase& database, const QString& tableName);

  /**
   * Returns the version of the Postgres database used
   *
   * @return a database version string
   */
  static QString getPostgresDbVersion(const QSqlDatabase& database);

  /**
   * Identifies database URLs as used in Hootenanny
   *
   * @param url the string to examine
   * @return true if the input string is a database URL; false otherwise
   */
  static bool isDbUrl(const QString& url);

private:

  static void _modifyTableConstraints(const QSqlDatabase& database, const QString& tableName,
                                      const bool disable = true);

};

}

#endif // DBUTILS_H
