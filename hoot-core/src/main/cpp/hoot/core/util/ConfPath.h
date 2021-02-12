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
 * @copyright Copyright (C) 2015 Maxar (http://www.maxar.com/)
 */

#ifndef CONFPATH_H
#define CONFPATH_H

// Qt
#include <QString>

namespace hoot
{

/**
 * A convenience class for searching the path for configuration files.
 *
 * Searches in this order:
 *  - Local directory
 *  - Local directory with "/conf"
 *  - $HOOT_HOME/conf/
 */
class ConfPath
{
public:
  ConfPath();

  /**
   * @brief Find all the files in the conf path that meet the requirements in filters.
   * @param filters - as specified in QDir::setNameFilters
   * @param searchDir - directory to search in the conf path
   * @return List of the absolute paths to all the files found.
   */
  static QStringList find(QStringList filters, QString searchDir = "conf");

  static QString getHootHome();

  /**
   * Searches the configuration path for the given configuration file name. Returns an absolute
   * path to the appropriate configuration file or throws an exception if it couldn't be found.
   */
  static QString search(QString baseName, QString searchDir = "conf");

private:
  static QStringList _find(QStringList filters, QString path);
  static QString _subDirSearch(QString baseName, QString searchDir);

};

}

#endif // CONFPATH_H
