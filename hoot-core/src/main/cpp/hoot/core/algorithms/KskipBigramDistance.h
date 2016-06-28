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
 * @copyright Copyright (C) 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef KSKIPBIGRAMDISTANCE_H
#define KSKIPBIGRAMDISTANCE_H

//  Hoot
#include <hoot/core/algorithms/StringDistance.h>
#include <hoot/core/util/HootException.h>

//  Qt
#include <QString>
#include <QStringList>

//  Standard
#include <vector>

namespace hoot
{

typedef QStringList BigramType;

/** Work based on:
 *  A Closer Look at Skip-gram Modelling
 *  David Guthrie, Ben Allison, Wei Liu, Louise Guthrie, Yorick Wilks
 *  http://homepages.inf.ed.ac.uk/ballison/pdf/lrec_skipgrams.pdf
 */
class KskipBigramDistance : public StringDistance
{
public:

  KskipBigramDistance(unsigned k = 3);

  static string className() { return "hoot::KskipBigramDistance"; }

  virtual ~KskipBigramDistance() {}

  void setK(unsigned k);

  /**
   * Returns a value from 1 (very similar) to 0 (very dissimilar) describing the distance between
   * two strings based on their k-skip bi-grams
   */
  virtual double compare(const QString& s1, const QString& s2) const { return score(s1, s2); }

  BigramType getBigrams(const QString& str) const;

  double score(const QString& s1, const QString& s2) const;

  QString toString() const { return QString("%1-skip bi-gram").arg(_k); }

private:
  /**
   * @brief _k is a value 'k' >= 2 for k-skip bigrams
   */
  unsigned _k;
};

}
#endif // KSKIPBIGRAMDISTANCE_H
