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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "FileUtils.h"

// Hoot
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>

// Qt
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QFileInfoList>
#include <QTextStream>

// Std
#include <fstream>
#include <algorithm>

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

QStringList FileUtils::tokenizeOutputFileWithoutDates(const QString& filePath)
{
  QStringList tokens;

  //parse all the string tokens, except those containing dates, which we can't directly compare
  const QRegExp reDate("[12][0-9][0-9][0-9]-[01][0-9]-[0-3][0-9]");
  const QRegExp reTime("[0-2][0-9]:[0-5][0-9]:[0-5][0-9].[0-9][0-9][0-9]");

  QFile file(filePath);
  if (file.open(QIODevice::ReadOnly))
  {
    QTextStream in(&file);
    in.setCodec("UTF-8");
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

QString FileUtils::readFully(const QString& path)
{
  QFile file(path);
  if (file.open(QIODevice::ReadOnly))
  {
    QTextStream stream(&file);
    stream.setCodec("UTF-8");
    const QString text = stream.readAll();
    file.close();
    return text;
  }
  else
  {
    throw HootException("Unable to read file at: " + path);
  }
}

void FileUtils::writeFully(const QString& path, const QString& text)
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
  out.setCodec("UTF-8");
  out << text;
  out.flush();
  outFile.close();
}

void FileUtils::replaceFully(const QString& path, const QString& textToReplace,
                             const QString& replacementText)
{
  QString input = readFully(path);
  input = input.replace(textToReplace, replacementText);
  writeFully(path, input);
}

long FileUtils::getNumberOfLinesInFile(const QString& file)
{
  std::ifstream inFile(file.toStdString().c_str());
  size_t lineCount =
    std::count(std::istreambuf_iterator<char>(inFile), std::istreambuf_iterator<char>(), '\n');
  return lineCount;
}

bool FileUtils::dirContainsFileWithExtension(const QDir& dir, const QString& extension)
{
  const QString ext = extension.toLower().replace(".", "");
  const QStringList files = dir.entryList();
  for (int i = 0; i < files.size(); i++)
  {
    if (files.at(i).toLower().endsWith(ext))
    {
      return true;
    }
  }
  return false;
}

QStringList FileUtils::readFileToList(const QString& inputPath)
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
      if (!line.trimmed().isEmpty() && !line.startsWith("#"))
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
