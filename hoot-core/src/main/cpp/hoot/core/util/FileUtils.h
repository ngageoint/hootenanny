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

#ifndef FILEUTILS_H
#define FILEUTILS_H

// Qt
#include <QString>
#include <QDir>

namespace hoot
{

/**
 * General utilities needed when working with files
 */
class FileUtils
{

public:

  /**
   * Delete a directory along with all of its contents.
   *
   * @param dirName Path of directory to remove.
   * @return true on success; false on error.
   */
  static void removeDir(const QString& dirName);

  /**
   * Tokenize a file by line and remove the date from each line
   *
   * @param filePath file to read
   * @return a list of file lines
   */
  static QStringList tokenizeOutputFileWithoutDates(const QString& filePath);

  /**
   * Reads an entire file to string
   *
   * @param path file path to read from
   * @return a string
   */
  static QString readFully(const QString& path);

  /**
   * Writes an entire file to string.  Closes the file on success.
   *
   * @param path file path to write to
   * @param text text to be written
   */
  static void writeFully(const QString& path, const QString& text);

  /**
   * Replaces a set of texts in a file with another set of texts
   *
   * @param path path of the file to modify
   * @param textsToReplace the texts in the file to be replaced
   * @param replacementTexts the texts in the file to replace with
   */
  static void replaceFully(const QString& path, const QStringList& textsToReplace,
                           const QStringList& replacementTexts);

  /**
   * Returns the number of lines in a file
   *
   * @param file file to examine
   * @return a line count
   */
  static long getNumberOfLinesInFile(const QString& file);

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
   * Reads file lines into a string list
   *
   * @param inputPath file to read
   * @return a list of strings
   */
  static QStringList readFileToList(const QString& inputPath);
};

}

#endif // FILEUTILS_H
