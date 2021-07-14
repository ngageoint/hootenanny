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
 * @copyright Copyright (C) 2015, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef SQLITEWORDWEIGHTDICTIONARY_H
#define SQLITEWORDWEIGHTDICTIONARY_H

// hoot
#include <hoot/core/algorithms/string/WordWeightDictionary.h>
#include <hoot/core/io/WordCountReader.h>

// Qt
#include <QRegExp>

// Tgs
#include <tgs/HashMap.h>

namespace hoot
{

class SqliteWordWeightDictionary : public WordWeightDictionary
{
public:

  SqliteWordWeightDictionary(const QString& filePath);
  ~SqliteWordWeightDictionary() = default;

  double getMinWeight() const override { return 1.0 / (double)_count; }
  double getWeight(const QString& word) const override;

private:

  // some white box testing.
  friend class TextFileWordWeightDictionaryTest;

  // tried a few different structures all running in release mode on yubyub:
  // typedef                  RSS       Elapsed time loading
  // QHash<QString, int>      1.265GB   18.4sec
  // QHash<QByteArray, int>   1.332GB   19.8sec
  // QHash<QString, long>     1.265GB   18.3sec
  // QMap<QString, long>      1.130GB   24.1sec
  // std::map<QString, long>  1.272GB   22.0sec
  // ** Using this one, fast, low memory and I don't have to worry about int overflow **
  // HashMap<QString, long>   1.069GB   19.4sec
  // HashMap<QString, int>    1.069GB   19.2sec
  using WeightHash = HashMap<QString, long>;
  mutable WeightHash _weights;
  long _count;
  QRegExp _nonWord;
  WordCountReader _reader;
};

}

#endif // SQLITEWORDWEIGHTDICTIONARY_H
