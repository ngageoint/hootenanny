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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef TRANSLATESTRINGDISTANCE_H
#define TRANSLATESTRINGDISTANCE_H

// hoot
#include <hoot/core/algorithms/StringDistanceConsumer.h>
#include <hoot/core/util/Configurable.h>

// tgs
#include <tgs/SharedPtr.h>

namespace hoot
{

/**
 * First attempts to translate/transliterate the words and phrases in each input string. Multiple
 * variations on the translation may be derived in which case the best matching translation will
 * be used.
 */
class TranslateStringDistance : public StringDistance, public StringDistanceConsumer,
  public Configurable
{
public:
  static std::string className() { return "hoot::TranslateStringDistance"; }

  TranslateStringDistance(StringDistance* d);
  TranslateStringDistance() {}

  virtual ~TranslateStringDistance() {}

  virtual void setStringDistance(const StringDistancePtr &sd) { _d = sd; }

  /**
   * Returns a value from 1 (very similar) to 0 (very dissimilar) describing the distance between
   * two strings.
   */
  virtual double compare(const QString& s1, const QString& s2) const;

  virtual void setConfiguration(const Settings& conf);

  virtual QString toString() const { return "Translate " + _d->toString(); }

private:
  StringDistancePtr _d;
  bool _tokenize;
};

}

#endif // TRANSLATESTRINGDISTANCE_H
