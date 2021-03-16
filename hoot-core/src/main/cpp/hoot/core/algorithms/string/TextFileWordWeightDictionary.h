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
 * @copyright Copyright (C) 2015, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef TEXTFILEWORDWEIGHTDICTIONARY_H
#define TEXTFILEWORDWEIGHTDICTIONARY_H

// hoot
#include <hoot/core/algorithms/string/WordWeightDictionary.h>

// Qt
#include <QRegExp>

// Tgs
#include <tgs/HashMap.h>

namespace hoot
{

/**
 * @brief The TextFileWordWeightDictionary class
 *
 * Performs a lazy read of a sorted file where the format is:
 *
 * Total word count: 787892387
 *
 * Total word count: ######
 * <word><tab><count>
 *
 * E.g.
 *
 * Total word count: 3
 * foo  1
 * bar  2
 *
 * (imagine I actually have tabs between the words and numbers).
 *
 * The weight returned is the count / total word count. If there are no instances of the word then
 * a zero is returned.
 */
class TextFileWordWeightDictionary : public WordWeightDictionary
{
public:
  TextFileWordWeightDictionary(const QString& filePath);
  virtual ~TextFileWordWeightDictionary() = default;

  virtual double getMinWeight() const override { return 1.0 / (double)_count; }

  virtual double getWeight(const QString& word) const override;

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
  WeightHash _weights;
  long _count;
  QRegExp _nonWord;

  void _loadFile(const QString& path);
};

}

#endif // TEXTFILEWORDWEIGHTDICTIONARY_H
