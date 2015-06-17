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
 * @copyright Copyright (C) 2012, 2013 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "FileUtils.h"

// Qt
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QFileInfoList>

// Hoot
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>

namespace hoot
{

void FileUtils::removeDir(const QString& dirName)
{
  QDir dir(dirName);

  if (dir.exists("."))
  {
    Q_FOREACH(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System |
      QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
    {
      QString path = info.absoluteFilePath();
      if (info.isDir())
      {
        removeDir(path);
      }
      else
      {
        if (QFile::remove(path) == false)
        {
          throw HootException(QString("Failed to remove %1").arg(dirName));
        }
      }

    }
    if (QDir().rmdir(dirName) == false)
    {
      throw HootException(QString("Failed to remove %1").arg(dirName));
    }
  }
}

}
