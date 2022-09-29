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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */

#ifndef LEVENSHTEINDISTANCE_H
#define LEVENSHTEINDISTANCE_H

// Hoot
#include <hoot/core/algorithms/string/StringDistance.h>
#include <hoot/core/util/Configurable.h>

namespace hoot
{

class LevenshteinDistance : public StringDistance, public Configurable
{
public:

  static QString className() { return "LevenshteinDistance"; }

  /**
   * @param alph See the levenshtein.distance.alpha configuration option for an explanation.
   */
  LevenshteinDistance(double alpha = -1);
  ~LevenshteinDistance() = default;

  double compare(const QString& s1, const QString& s2) const override;

  static double score(const char* s1, const char* s2);
  static double score(const QString& s1, const QString& s2);
  template<class T> static
  double score(const T& s1, const T& s2)
  {
    size_t size = std::max(s1.size(), s2.size());
    unsigned int d = distance(s1, s2);
    return 1.0 - (double)d / (double)size;
  }

  static unsigned int distance(const QString& s1, const QString& s2);
  static unsigned int distance(const char* s1, const char* s2);
  template<class T> static
  unsigned int distance(const T& s1, const T& s2)
  {
    const size_t len1 = s1.size(), len2 = s2.size();
    std::vector<unsigned int> col(len2+1), prevCol(len2+1);

    for (unsigned int i = 0; i < prevCol.size(); i++)
      prevCol[i] = i;
    for (unsigned int i = 0; i < len1; i++)
    {
      col[0] = i+1;
      for (unsigned int j = 0; j < len2; j++)
        col[j + 1] = std::min(std::min(1 + col[j], 1 + prevCol[1 + j]), prevCol[j] + (s1[i] == s2[j] ? 0 : 1));
      col.swap(prevCol);
    }
    return prevCol[len2];
  }

  int operator() (const QString& s1, const QString& s2) const
  {
    return distance(s1, s2);
  }

  void setConfiguration(const Settings& conf) override;

  QString toString() const override { return QString("Levenshtein %1").arg(_alpha); }
  QString getDescription() const override
  { return "Returns a string comparison score derived using Levenshtein Distance"; }
  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }

  void setAlpha(double alpha);

private:

  double _alpha;
};

}

#endif // LEVENSHTEINDISTANCE_H
