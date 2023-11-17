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

#ifndef FILEUTILS_H
#define FILEUTILS_H

//  Qt
#include <QDir>
#include <QString>

namespace hoot
{

/**
 * General utilities needed when working with files
 */
class FileUtils
{

public:

  /**
   * Creates a folder path using QDir::mkpath in a more thread-safe way
   * @param path relative or absolute path to create (think `mkdir -p`)
   * @return true if successful
   */
  static bool makeDir(const QString& path);
  /**
   * Delete a directory along with all of its contents.
   *
   * @param dirName Path of directory to remove.
   * @return true on success; false on error.
   */
  static void removeDir(const QString& dirName);
  /**
   * Determines if a directory contains a file with the specified extension
   *
   * @param dir directory to examine
   * @param extension file extension to search for
   * @return returns true if the input directory contains at least one file with the specified
   * extension; false otherwise
   */
  static bool dirContainsFileWithExtension(const QDir& dir, const QString& extension);
  /**
   * Determines if any path in a collection of paths are directories
   *
   * @param paths paths to examine
   * @return true if any input path is a directory; false otherwise
   */
  static bool anyAreDirs(const QStringList& paths);

  /**
   * Reads an entire file to string
   *
   * @param path file path to read from
   * @return a string
   */
  static QString readFully(const QString& path);
  /**
   * Reads file lines into a string list
   *
   * @param inputPath file to read
   * @param toLowerCase if true, converts each line to lower case
   * @return a list of strings
   */
  static QStringList readFileToLines(const QString& inputPath, const bool toLowerCase = false);
  /**
   * Returns the number of lines in a file
   *
   * @param file file to examine
   * @return a line count
   */
  static long getNumberOfLinesInFile(const QString& file);

  /**
   * Writes an entire file to string.  Closes the file on success.
   *
   * @param path file path to write to
   * @param text text to be written
   */
  static void writeFully(const QString& path, const QString& text);
  static void writeFully(const QString& path, const QByteArray& text);

  /**
   * Tokenize a file by line and remove the date from each line
   *
   * @param filePath file to read
   * @return a list of file lines
   */
  static QStringList tokenizeOutputFileWithoutDates(const QString& filePath);

  /**
   * Convert a URL (file path, database URL, or other resource location) to a loggable format
   * Removes usernames, passwords, ip addresses, etc. and truncates the filename if needed
   *
   * @param url - file URL to inspect (passed by copy to be modified)
   * @param characters - number of right-hand characters to preserve
   * @return modified URL ready to output in logs
   */
  static QString toLogFormat(QString url, int characters = -1);
  static QString toLogFormat(QStringList urls, int characters = -1);
};

}

#endif // FILEUTILS_H
