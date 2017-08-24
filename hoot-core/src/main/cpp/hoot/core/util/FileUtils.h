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

#ifndef FILEUTILS_H
#define FILEUTILS_H

// Qt
#include <QString>

namespace hoot
{

class FileUtils
{

public:

  /**
   * Delete a directory along with all of its contents.
   * @param dirName Path of directory to remove.
   * @return true on success; false on error.
   */
  static void removeDir(const QString& dirName);

  /**
   * @brief tokenizeOutputFileWithoutDates
   * @param filePath
   * @return
   */
  static QStringList tokenizeOutputFileWithoutDates(const QString filePath);

  /**
   * Reads an entire file to string
   *
   * @param path file path to read from
   * @return a string
   */
  static QString readFully(const QString path);

  /**
   * Writes an entire file to string.  Closes the file on success.
   *
   * @param path file path to write to
   * @param text text to be written
   */
  static void writeFully(const QString path, const QString text);

  /**
   * Converts seconds to DD:MM:SS
   *
   * @param durationInMilliseconds seconds to convert
   * @return a DD:MM:SS string
   */
  static QString secondsToDhms(const qint64 durationInMilliseconds);

  /**
   *
   *
   * @param number
   * @return
   */
  static QString formatPotentiallyLargeNumber(const unsigned long number);
};

}

#endif // FILEUTILS_H
