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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef MATCHTHRESHOLD_H
#define MATCHTHRESHOLD_H

// hoot
#include <hoot/core/conflate/matching/Match.h>
#include <hoot/core/conflate/matching/MatchType.h>

// Standard
#include <vector>

namespace hoot
{

class MatchClassification;

/**
 * This class is re-entrant, but not thread safe.
 */
class MatchThreshold
{

public:

  /**
   * Constructor
   *
   * @param matchThreshold the score threshold at which a match object is considered a match
   * @param missThreshold the score threshold at which a match object is considered a miss
   * @param reviewThreshold the score threshold at which a match object is considered a review
   * @param validateRange if true, the range (0.0, 1.0] will be honored. For conflate usage we
   * generally want to honor that range for thresholds. In some instances, though, we don't want to.
   * e.g. match feature extraction or Multiary Conflation
   */
  MatchThreshold(double matchThreshold = 0.5, double missThreshold = 0.5,
                 double reviewThreshold = 1.0, bool validateRange = true);

  double getMatchThreshold() const { return _matchThreshold; }

  double getMissThreshold() const { return _missThreshold; }

  double getReviewThreshold() const { return _reviewThreshold; }

  MatchType getType(const Match& m) const { return getType(m.getClassification()); }
  MatchType getType(const MatchClassification& mc) const;

  /**
   * Returns human readable information about the match type
   *
   * @param mc match classification
   * @return match type information
   */
  QString getTypeDetail(const MatchClassification& mc) const;

  QString toString() const;

private:

  double _matchThreshold, _missThreshold, _reviewThreshold;

};

using MatchThresholdPtr = std::shared_ptr<MatchThreshold>;
using ConstMatchThresholdPtr = std::shared_ptr<const MatchThreshold>;

}

#endif // MATCHTHRESHOLD_H
