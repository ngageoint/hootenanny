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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#include "MarkForReviewMergerCreator.h"

// hoot
#include <hoot/core/conflate/matching/Match.h>
#include <hoot/core/conflate/matching/MatchClassification.h>
#include <hoot/core/conflate/matching/MatchThreshold.h>
#include <hoot/core/conflate/merging/MarkForReviewMerger.h>

using namespace std;

namespace hoot
{

bool MarkForReviewMergerCreator::createMergers(const MatchSet& matches,
                                               vector<MergerPtr>& mergers) const
{
  LOG_TRACE("Creating mergers with " << className() << "...");

  bool result = false;

  set<pair<ElementId, ElementId>> eids;

  int reviewCount = 0;

  QStringList matchStrings;
  QStringList reviewType;
  double score = 0;
  // go through all the matches
  for (MatchSet::const_iterator it = matches.begin(); it != matches.end(); ++it)
  {
    ConstMatchPtr match = *it;
    LOG_VART(match->toString());
    MatchType type = match->getType();
    LOG_VART(type);
    if (type == MatchType::Review)
    {
      set<pair<ElementId, ElementId>> s = match->getMatchPairs();
      eids.insert(s.begin(), s.end());
      matchStrings.append(match->explain());
      score = max<double>(match->getClassification().getReviewP(), score);
      reviewCount++;
      LOG_VART(reviewType);
      LOG_VART(match->getName());
      LOG_VART(score);
      if (reviewType.contains(match->getName()) == false)
      {
        reviewType.append(match->getName());
      }
    }
  }

  if (reviewCount > 0 && reviewCount != int(matches.size()))
  {
    throw HootException("Expected the whole set to be review matches.");
  }

  // only add the mark for review merger if there are elements to merge.
  if (!eids.empty())
  {
    mergers.push_back(
      std::make_shared<MarkForReviewMerger>(
        eids, matchStrings.join(","), reviewType.join(";"), score));
    result = true;
  }
  else
  {
    LOG_TRACE("No elements to merge.");
  }

  return result;
}

vector<CreatorDescription> MarkForReviewMergerCreator::getAllCreators() const
{
  // don't provide this as an option in the UI. This will be added automatically in the right place.
  // TODO: How do we get a description back for this one?
  return vector<CreatorDescription>();
}

bool MarkForReviewMergerCreator::isConflicting(
  const ConstOsmMapPtr&, ConstMatchPtr, ConstMatchPtr, const QHash<QString, ConstMatchPtr>&) const
{
  return false;
}

}
