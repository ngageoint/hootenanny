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
 * @copyright Copyright (C) 2015, 2017, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef WORDWEIGHTDICTIONARY_H
#define WORDWEIGHTDICTIONARY_H

// Qt
#include <QString>

namespace hoot
{

/**
 * @brief The WordWeightDictionary class returns the weight of a given word.
 */
class WordWeightDictionary
{
public:

  WordWeightDictionary() = default;
  virtual ~WordWeightDictionary() = default;

  /**
   * @brief getMinWeight
   * @return The minimum valid weight that will be returned. This will always be > 0.
   */
  virtual double getMinWeight() const = 0;

  /**
   * @brief getWeight
   * @param word
   * @return The weight of a given word. If the word isn't found 0 is returned.
   */
  virtual double getWeight(const QString& word) const = 0;
};

using WordWeightDictionaryPtr = std::shared_ptr<WordWeightDictionary>;

}

#endif // WORDWEIGHTDICTIONARY_H
