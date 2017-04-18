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

#ifndef WORDCOUNTWRITER_H
#define WORDCOUNTWRITER_H

// Qt
#include <QVector>
#include <QString>

class QSqlDatabase;

namespace hoot
{

class WordCount;

class WordCountWriter
{
public:

  static QString totalWordCount() { return QString("Total word count:"); }

  /**
   * Create two files that contain a dictionary of words and their frequency. The frequent words
   * are designed to be read entirely into RAM and the .all words are intended to be randomly
   * accessed as needed.
   *
   * The first line of each file contains where ### is the total number of words that were counted.
   * This will be the same number in both files.
   *
   * Total word count:\t###\n
   *
   * The .all file is sorted by word. The .frequent file is sorted by count, descending. Both files
   * are structured as UTF-8 with one line per word/count combo. The word/count is tab delimited and
   * ends with a \n. E.g.
   *
   * Total word count:\t3\n
   * bar\t2\n
   * foo\t1\n
   *
   * @param basePath Base path for writing data. A .frequent and .all file will be created.
   * @param maxFrequent Maximum size of frequent words in bytes.
   */
  WordCountWriter(long maxFrequent) : _maxFrequent(maxFrequent) {}

  virtual ~WordCountWriter() {}

  void write(QString basePath, QVector<WordCount> words);

private:

  long _maxFrequent;

  void _writeFile(QString path, long totalCount, QVector<WordCount> words, long maxSize);
};

}

#endif // WORDCOUNTWRITER_H
