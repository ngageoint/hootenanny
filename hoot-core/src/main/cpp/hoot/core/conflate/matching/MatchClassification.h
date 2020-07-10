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
#ifndef MATCHCLASSIFICATION_H
#define MATCHCLASSIFICATION_H

// Qt
#include <QString>

namespace hoot
{

/**
 * The match classification object represents the classification of a match. A match may be in
 * one of three states:
 *
 *  - match - we're confident the two elements represent the same feature
 *  - miss - we're confident the two elements do not represent the same feature
 *  - review - we're confident the data is ambiguous or wrong and it requires human review
 *
 * Of course, there are different levels to each of those states. This class represents those
 * varying levels of confidence.
 *
 * @sa MatchThreshold
 */
class MatchClassification
{
public:

  MatchClassification();
  MatchClassification(double match, double miss, double review);

  /**
   * Reset the values to all zeros. This isn't valid, but it is a known state.
   */
  void clear() { _match = 0.0; _miss = 0.0; _review = 0.0; }

  double getMatchP() const { return _match; }
  double getMissP() const { return _miss; }
  double getReviewP() const { return _review; }

  /**
   * Returns true if all Ps are non-negative and sum approximately to 1.
   */
  bool isValid() const;

  /**
   * Makes sure all values are >= 0. If the sum of Ps is > 0 then they're normalized to sum to 1.
   * If the values are completely out of wack then review is set.
   */
  void normalize();

  void setMatch() { _miss = 0; _match = 1; _review = 0; }
  void setMatchP(double match) { _match = match; }

  void setMiss() { _miss = 1; _match = 0; _review = 0; }
  void setMissP(double miss) { _miss = miss; }

  void setReview() { _miss = 0; _match = 0; _review = 1; }
  void setReviewP(double review) { _review = review; }

  QString toString() const
  {
    return QString("match: %1 miss: %2 review: %3").arg(_match).arg(_miss).arg(_review);
  }

private:

  double _match;
  double _miss;
  double _review;
};

}

#endif // MATCHCLASSIFICATION_H
