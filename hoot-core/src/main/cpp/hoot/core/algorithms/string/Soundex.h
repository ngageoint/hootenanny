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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef SOUNDEX_H
#define SOUNDEX_H

// hoot
#include <hoot/core/algorithms/string/StringDistance.h>

// Qt
#include <QString>

// standard
#include <string>

namespace hoot
{

/**
 * Implements Soundex as described on Wikipedia on 5/16/2013 [1].
 *
 * 1. http://en.wikipedia.org/wiki/Soundex
 */
class Soundex : public StringDistance
{
public:

  static QString className() { return "hoot::Soundex"; }

  Soundex() = default;
  virtual ~Soundex() = default;

  virtual double compare(const QString& s1, const QString& s2) const override;

  /**
   * Compares word1 & word2 by encoding the strings and counting each mismatched character as an
   * error. The result is 0 for no similarity and 4 is a high similarity.
   */
  static int compareSoundex(QString word1, QString word2);

  /**
   * Similar to above, but it compares two soundex encoded strings.
   */
  static int compareEncoded(QString encoded1, QString encoded2);

  /**
   * Encodes the given word as a soundex string.
   */
  static QString encode(QString s);

  /**
   * Returns the soundex digit that represents the specified character. If there is no match then
   * an empty string is returned.
   */
  static QString toDigit(QChar c);

  QString toString() const override { return "Soundex"; }

  virtual QString getName() const override { return className(); }

  virtual QString getDescription() const override
  { return "Returns a string comparison score based on the Soundex algorithm"; }
};

}

#endif // SOUNDEX_H
