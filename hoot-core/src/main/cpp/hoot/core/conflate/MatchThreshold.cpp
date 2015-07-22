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
#include "MatchThreshold.h"

// hoot
#include <hoot/core/conflate/Match.h>
#include <hoot/core/util/Log.h>

namespace hoot
{

MatchThreshold::MatchThreshold(double matchThreshold, double missThreshold, double reviewThreshold)
{
  _matchThreshold = matchThreshold;
  _missThreshold = missThreshold;
  _reviewThreshold = reviewThreshold;
}

MatchType MatchThreshold::getType(const MatchClassification& mc) const
{
  if (mc.getReviewP() >= _reviewThreshold)
  {
    return MatchType::Review;
  }
  else if (mc.getMatchP() >= _matchThreshold && mc.getMissP() >= _missThreshold)
  {
    return MatchType::Review;
  }
  else if (mc.getMatchP() >= _matchThreshold)
  {
    return MatchType::Match;
  }
  else if (mc.getMissP() >= _missThreshold)
  {
    return MatchType::Miss;
  }
  else
  {
    return MatchType::Review;
  }
}

QString MatchThreshold::toString() const
{
  return QString("Thresholds; Match: %1 Miss: %2 Review: %3").arg(_matchThreshold).
      arg(_missThreshold).arg(_reviewThreshold);
}

}
