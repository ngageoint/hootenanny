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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#include "RemoveDuplicateReviewsOp.h"

// hoot
#include <hoot/core/conflate/matching/MatchClassification.h>
#include <hoot/core/conflate/matching/MatchFactory.h>
#include <hoot/core/conflate/review/ReviewMarker.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/ops/CopyMapSubsetOp.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>

using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, RemoveDuplicateReviewsOp)

RemoveDuplicateReviewsOp::RemoveDuplicateReviewsOp() :
_taskStatusUpdateInterval(ConfigOptions().getTaskStatusUpdateInterval())
{
}

void RemoveDuplicateReviewsOp::apply(std::shared_ptr<OsmMap>& map)
{
  _numAffected = 0;
  _map = map;

  // go through all the relations to get duplicate reviews
  LOG_DEBUG("Retrieving duplicate reviews...");

  int totalMembersToReview = 0;
  const RelationMap& relations = map->getRelations();
  QMap<set<ElementId>, QList<ReviewMarker::ReviewUid>> membersToReview;
  for (RelationMap::const_iterator it = relations.begin(); it != relations.end(); ++it)
  {
    ElementId eid = ElementId::relation(it->first);
    if (ReviewMarker::isReviewUid(map, eid))
    {
      membersToReview[ReviewMarker::getReviewElements(map, eid)].append(eid);
      totalMembersToReview++;
    }
  }
  LOG_VARD(membersToReview.size());
  LOG_VARD(totalMembersToReview);
  LOG_VART(membersToReview);

  // loop through duplicate reviews
  LOG_DEBUG("Removing duplicate reviews...");

  ReviewMarker reviewMarker;
  QMap<set<ElementId>, QList<ReviewMarker::ReviewUid>>::iterator it = membersToReview.begin();
  while (it != membersToReview.end())
  {
    set<ElementId> eids = it.key();

    QList<ReviewMarker::ReviewUid> duplicateReviews = it.value();

    LOG_VART(eids.size());
    LOG_VART(duplicateReviews.size());

    // Only remove reviews and process if there is more than one review. See discussion here:
    // https://github.com/ngageoint/hootenanny/issues/81#issuecomment-162980656
    if (eids.size() == 2 && duplicateReviews.size() > 1)
    {
      for (int i = 0; i < duplicateReviews.size(); i++)
      {
        ReviewMarker::removeElement(map, duplicateReviews[i]);
        _numAffected++;
      }

      ElementId beid = *eids.begin();
      ElementId eeid = *eids.rbegin();

      std::shared_ptr<OsmMap> copy(new OsmMap());
      CopyMapSubsetOp(map, beid, eeid).apply(copy);
      copy->getElement(beid)->setStatus(Status::Unknown1);
      copy->getElement(eeid)->setStatus(Status::Unknown2);
      LOG_VART(copy->size());

      MatchPtr match = MatchFactory::getInstance().createMatch(copy, beid, eeid);
      if (match)
      {
        LOG_VART(match->getType());
      }
      if (match && match->getType() != MatchType::Miss)
      {
        QString explain = match->explain();
        if (match->getType() == MatchType::Match)
        {
          if (explain.isEmpty())
          {
            explain = "Multiple overlapping high confidence reviews";
          }
          else
          {
            explain = "Multiple overlapping high confidence reviews: " + explain;
          }
        }
        reviewMarker.mark(
          map, map->getElement(beid), map->getElement(eeid), explain, match->getName(),
          match->getClassification().getReviewP());
      }
    }
    ++it;
    _numProcessed += duplicateReviews.size();

    if (_numProcessed % (_taskStatusUpdateInterval /  10) == 0)
    {
      PROGRESS_INFO(
        "Processed " << StringUtils::formatLargeNumber(_numProcessed) <<
        " members with reviews / " << StringUtils::formatLargeNumber(totalMembersToReview) <<
        " total members.");
    }
  }
}

}
