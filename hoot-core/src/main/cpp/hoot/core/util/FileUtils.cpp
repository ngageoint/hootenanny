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
 * @copyright Copyright (C) 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "FileUtils.h"

// Qt
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QFileInfoList>
#include <QTextStream>

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

QStringList FileUtils::tokenizeOutputFileWithoutDates(const QString filePath)
{
  QStringList tokens;

  //parse all the string tokens, except those containing dates, which we can't directly compare
  const QRegExp reDate("[12][0-9][0-9][0-9]-[01][0-9]-[0-3][0-9]");
  const QRegExp reTime("[0-2][0-9]:[0-5][0-9]:[0-5][0-9].[0-9][0-9][0-9]");

  QFile file(filePath);
  if (file.open(QIODevice::ReadOnly))
  {
    QTextStream in(&file);
    while (!in.atEnd())
    {
      QString line = in.readLine();
      line = line.remove(reDate);
      line = line.remove(reTime);
      tokens << line;
    }
    file.close();
  }
  else
  {
    throw HootException("Unable to open file " + filePath + ".");
  }

  return tokens;
}

QString FileUtils::readFully(const QString path)
{
  QFile file(path);
  if (file.open(QIODevice::ReadOnly))
  {
    return file.readAll();
  }
  else
  {
    throw HootException("Unable to read file at: " + path);
  }
}

void FileUtils::writeFully(const QString path, const QString text)
{
  QFile outFile(path);
  if (outFile.exists() && !outFile.remove())
  {
    throw HootException("Unable to remove file: " + path);
  }
  if (!outFile.open(QFile::WriteOnly | QFile::Text))
  {
    throw HootException("Error opening file: " + path);
  }
  QTextStream out(&outFile);
  out << text;
  out.flush();
  outFile.close();
}

QString FileUtils::secondsToDhms(const qint64 durationInMilliseconds)
{
  //TODO: move to utility class
  QString res;
  int duration = (int)(durationInMilliseconds / 1000);
  const int seconds = (int)(duration % 60);
  duration /= 60;
  const int minutes = (int)(duration % 60);
  duration /= 60;
  const int hours = (int)(duration % 24);
  const int days = (int)(duration / 24);
  if ((hours == 0) && (days == 0))
  {
    return res.sprintf("%02d:%02d", minutes, seconds);
  }
  if (days == 0)
  {
    return res.sprintf("%02d:%02d:%02d", hours, minutes, seconds);
  }
  return res.sprintf("%dd%02d:%02d:%02d", days, hours, minutes, seconds);
}

QString FileUtils::formatPotentiallyLargeNumber(const unsigned long number)
{
  //I want to see comma separators...probably a better way to handle this...will go with this for
  //now.
  const QLocale& cLocale = QLocale::c();
  QString ss = cLocale.toString((qulonglong)number);
  ss.replace(cLocale.groupSeparator(), ',');
  return ss;
}

}
