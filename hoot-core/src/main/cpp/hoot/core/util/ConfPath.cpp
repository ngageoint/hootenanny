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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2021, 2022 Maxar (http://www.maxar.com/)
 */

#include "ConfPath.h"

// hoot
#include <hoot/core/util/Settings.h>

// Qt
#include <QDirIterator>

// Standard
#include <stdlib.h>

using namespace std;

namespace hoot
{

QStringList ConfPath::find(QStringList filters, QString searchDir)
{
  QString hootHome = getHootHome();

  QStringList result;

  result = _find(filters, searchDir);
  result.append(_find(filters, hootHome + "/" + searchDir));

  // remove duplicates. Slow, but effective.
  result = QStringList::fromSet(result.toSet());

  return result;
}

QStringList ConfPath::_find(QStringList filters, QString path)
{
  QDir dir(path);

  QFileInfoList l = dir.entryInfoList(filters, QDir::Files);

  QStringList result;
  for (const auto& path : qAsConst(l))
    result << path.absoluteFilePath();

  return result;
}

QString ConfPath::getHootHome()
{
  if (conf().hasKey("HOOT_HOME"))
    return conf().getString("HOOT_HOME");
  else
    return getenv("HOOT_HOME");
}


QString ConfPath::_subDirSearch(QString baseName, QString searchDir)
{
  // Should we allow following symlinks?
  QDir startDir(searchDir);
  startDir.setFilter(QDir::Files | QDir::NoSymLinks);
  startDir.setNameFilters(QStringList(baseName));

  QDirIterator it(startDir, QDirIterator::Subdirectories);

  // Take the first matching file.
  if (it.hasNext())
    return QFileInfo(it.next()).absoluteFilePath();
  else
    return "";
}


QString ConfPath::search(QString baseName, QString searchDir)
{
  // Try searchDir first
  if (QFileInfo(searchDir + "/" + baseName).isFile())
    return QFileInfo(searchDir + "/" + baseName).absoluteFilePath();

  // Now try HOOT_HOME + / + searchDir
  QString hootHome = getHootHome();

  if (hootHome.isEmpty() == false && QFileInfo(hootHome + "/" + searchDir + "/" + baseName).isFile())
    return QFileInfo(hootHome + "/" + searchDir + "/" + baseName).absoluteFilePath();

  // Now try local dir
  if (QFileInfo(baseName).isFile())
    return QFileInfo(baseName).absoluteFilePath();

  // If we still can't find it, try searching subdirectories.
  // Not sure if this should also have a go with "hootHome/searchDir" as well as "searchDir"

  QString result = "";

  // Now try sub directories with HootHome
  if (hootHome.isEmpty() == false)
  {
    result = _subDirSearch(baseName, hootHome + "/" + searchDir);
    if (result.isEmpty() == false)
      return result;
  }

  // Now try searching just sub directories
  // Note: This may never occur. We _always_ have HOOT_HOME, don't we?
  result = _subDirSearch(baseName, searchDir);
  if (result.isEmpty() == false)
    return result;

  // If we can't find the file, Throw Error
  throw FileNotFoundException("Could not find specified file in any " + searchDir + " directory ("
                              + baseName + ")  HOOT_HOME: " + hootHome);
}

}
