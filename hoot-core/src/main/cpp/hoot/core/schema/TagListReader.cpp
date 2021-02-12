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
 * @copyright Copyright (C) 2017, 2018, 2019 Maxar (http://www.maxar.com/)
 */
#include "TagListReader.h"

// hoot
#include <hoot/core/util/Log.h>
#include <hoot/core/util/HootException.h>

// Qt
#include <QFile>

namespace hoot
{

QStringList TagListReader::readList(const QString& inputPath, const bool keysOnly)
{  
  LOG_VARD(inputPath);
  QStringList outputList;
  if (!inputPath.trimmed().isEmpty())
  {
    QFile inputFile(inputPath);
    if (!inputFile.open(QIODevice::ReadOnly))
    {
      throw HootException(QObject::tr("Error opening %1 for writing.").arg(inputFile.fileName()));
    }
    while (!inputFile.atEnd())
    {
      const QString line = QString::fromUtf8(inputFile.readLine().constData()).trimmed();
      if (!line.trimmed().isEmpty() && !line.startsWith("#") && (keysOnly || line.contains("=")))
      {
        outputList.append(line.toLower());
      }
    }
    inputFile.close();
  }
  LOG_VART(outputList);
  return outputList;
}

}
