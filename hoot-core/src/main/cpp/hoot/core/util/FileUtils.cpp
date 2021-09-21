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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#include "FileUtils.h"

// Hoot
#include <hoot/core/util/StringUtils.h>

// Qt
#include <QFileInfoList>
#include <QHostAddress>
#include <QRegularExpression>
#include <QTextStream>
#include <QUrl>

// Std
#include <algorithm>
#include <fstream>
#include <thread>

namespace hoot
{

bool FileUtils::makeDir(const QString& path)
{
  //  Don't make it if it exists
  if (QDir().exists(path))
    return true;
  //  Try to make the path 'retry' times waiting 'duration' microseconds in between tries
  const int retry = 3;
  //  100 msec should be enough to wait between tries
  const unsigned int duration = 100;
  for (int i = 0; i < retry; i++)
  {
    if (QDir().mkpath(path))
      return true;
    std::this_thread::sleep_for(std::chrono::milliseconds(duration));
  }
  //  Report failure
  LOG_ERROR(QString("Couldn't create output directory: %1").arg(path).toStdString());
  return false;
}

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
  QFileInfo fi(outFile);
  QDir dir = fi.dir();
  //  Attempt to create the directory if it doesn't exist
  if (!dir.exists())
    makeDir(dir.absolutePath());
  //  Delete the previous file
  if (outFile.exists() && !outFile.remove())
    throw HootException("Unable to remove file: " + path);
  //  Open to write
  if (!outFile.open(QFile::WriteOnly | QFile::Text))
    throw HootException("Error opening file: " + path);
  //  Write a UTF-8 file
  QTextStream out(&outFile);
  out.setCodec("UTF-8");
  out << text;
  out.flush();
  outFile.close();
}

long FileUtils::getNumberOfLinesInFile(const QString& file)
{
  std::ifstream inFile(file.toStdString().c_str());
  //  Count the number of new lines in the file
  size_t lineCount =
    std::count(std::istreambuf_iterator<char>(inFile), std::istreambuf_iterator<char>(), '\n');
  //  If the file doesn't end in a new line, then add one to the total
  inFile.seekg(-1, std::ios_base::end);
  char last;
  inFile.get(last);
  if (last != '\n')
    lineCount++;
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

QStringList FileUtils::readFileToLines(const QString& inputPath, const bool toLowerCase)
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
        if (toLowerCase)
        {
          outputList.append(line.toLower());
        }
        else
        {
          outputList.append(line);
        }
      }
    }
    inputFile.close();
  }
  LOG_VART(outputList);
  return outputList;
}

QString FileUtils::toLogFormat(QString url, int characters)
{
  //  Remove username/password from URLs
  QUrl tempUrl(url);
  url = tempUrl.toString(QUrl::RemoveUserInfo);
  tempUrl.setUrl(url);
  //  Remove port from URLs
  url = tempUrl.toString(QUrl::RemovePort);
  tempUrl.setUrl(url);
  //  Remove IP addresses from URLs
  QHostAddress host(tempUrl.host());
  if (!host.isNull())
    url.replace(tempUrl.host(), "<host-ip>");
  //  Truncate the URL if desired
  return url.right((characters >= 0) ? characters : url.length());
}

QString FileUtils::toLogFormat(QStringList urls, int characters)
{
  QStringList files;
  //  Format each url individually
  for (int i = 0; i < urls.size(); ++i)
    files.push_back(FileUtils::toLogFormat(urls[i]));
  //  Make a comma separated list before truncating if desired
  QString result = files.join(", ");
  return result.right((characters >= 0) ? characters : result.length());
}

bool FileUtils::anyAreDirs(const QStringList& paths)
{
  if (paths.empty())
    return false;

  for (int i = 0; i < paths.size(); i++)
  {
    if (QFileInfo(paths.at(i)).isDir())
      return true;
  }
  return false;
}

}
