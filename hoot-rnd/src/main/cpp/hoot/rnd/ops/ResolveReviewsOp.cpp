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
 * @copyright Copyright (C) 2021 DigitalGlobe (http://www.digitalglobe.com/)
 * @copyright Copyright (C) 2021 Maxar (http://www.maxar.com/)
 */
#include "ResolveReviewsOp.h"

// hoot
#include <hoot/core/conflate/matching/MatchClassification.h>
#include <hoot/core/conflate/matching/MatchFactory.h>
#include <hoot/core/conflate/merging/MergerFactory.h>
#include <hoot/core/conflate/review/ReviewMarker.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/ops/CopyMapSubsetOp.h>
#include <hoot/core/ops/RemoveElementByEid.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>

// Standard
#include <set>

using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, ResolveReviewsOp)

ResolveReviewsOp::ResolveReviewsOp()
  : _taskStatusUpdateInterval(1000),
    _type(ResolveType::ResolveReviews)
{
}

void ResolveReviewsOp::setConfiguration(const Settings& conf)
{
  ConfigOptions opts(conf);
  _taskStatusUpdateInterval = opts.getTaskStatusUpdateInterval();
  _type = _resolveString(opts.getResolveReviewType());
}

void ResolveReviewsOp::apply(std::shared_ptr<OsmMap>& map)
{
  _numAffected = 0;
  //  Bail out when keeping reviews
  if (_type == KeepReviews)
  {
    LOG_INFO("ResolveReviews operator keeping reviews");
    return;
  }
  //  Go through all the relations to get reviews
  const RelationMap& relations = map->getRelations();
  RelationMap reviews;
  for (RelationMap::const_iterator it = relations.begin(); it != relations.end(); ++it)
  {
    ElementId eid = ElementId::relation(it->first);
    if (ReviewMarker::isReviewUid(map, eid))
      reviews[eid.getId()] = dynamic_pointer_cast<Relation>(map->getElement(eid));
  }
  //  The number affected is the same as the total number
  _numProcessed = _numAffected = reviews.size();

  //  Remove all of the review relations
  if (_type == RemoveReviews)
  {
    LOG_INFO("ResolveReviews operator removing reviews");
    //  Remove all of the review relations one by one
    for (RelationMap::iterator it = reviews.begin(); it != reviews.end(); ++it)
      RemoveElementByEid(ElementId::relation(it->first)).apply(map);
  }
  else
  {
    LOG_INFO("ResolveReviews operator resolving reviews");
    //  Iterate all reviews and resolve them
    for (RelationMap::iterator it = reviews.begin(); it != reviews.end(); ++it)
    {
      ElementId rid = ElementId::relation(it->first);
      set<ElementId> elements = ReviewMarker::getReviewElements(map, rid);
      if (elements.size() < 2)
      {
        //  Less than two elements, this review can be removed
        RemoveElementByEid(rid).apply(map);
      }
      else
      {
        //  Check if any elements are part of multiple reviews
        bool multipleReviews = false;
        for (set<ElementId>::iterator it2 = elements.begin(); it2 != elements.end(); ++it2)
        {
          if (ReviewMarker::getReviewUids(map, *it2).size() > 1)
            multipleReviews = true;
        }
        std::vector<ElementId> eids(elements.begin(), elements.end());
        //  Handle reviews differently if an element is involved in multiple reviews
        if (!multipleReviews)
          _resolveReview(map, rid, eids[0], eids[1]);
        else
          _resolveMultipleReviews(map, rid, eids[0], eids[1]);
      }
    }
  }
}

void ResolveReviewsOp::_resolveReview(
  const std::shared_ptr<OsmMap>& map, const ElementId& relation_id, const ElementId& eid1,
  const ElementId& eid2)
{
  MatchPtr match = _getCachedMatch(map, relation_id, eid1, eid2);
  //  Resolve the match review
  _resolveMatchReview(match, map, relation_id, eid1, eid2);
}

void ResolveReviewsOp::_resolveMultipleReviews(
  const std::shared_ptr<OsmMap>& map, const ElementId& relation_id, const ElementId& eid1,
  const ElementId& eid2)
{
  LOG_TRACE(
    "Resolving multiple reviews for review: " << relation_id << ", elements: " <<
    eid1 << ", " << eid2 << "...");

  //  Get the initial match
  MatchPtr match = _getCachedMatch(map, relation_id, eid1, eid2);
  if (!match)
  {
    //  No match object was found, resolve the review manually
    _resolveManualReview(map, relation_id, eid1, eid2);
    return;
  }
  double score = match->getScore();
  std::vector<ElementId> element_ids = { eid1, eid2 };
  //  Iterate both sides of the relation looking for the best result
  for (std::vector<ElementId>::iterator it = element_ids.begin(); it != element_ids.end(); ++it)
  {
    //  Iterate all of the reviews involving this element ID looking for a score higher than the current score
    std::set<ReviewMarker::ReviewUid> reviews_eid = ReviewMarker::getReviewUids(map, *it);
    for (std::set<ReviewMarker::ReviewUid>::iterator rit = reviews_eid.begin(); rit != reviews_eid.end(); ++rit)
    {
      ReviewMarker::ReviewUid rid = *rit;
      //  Ignore the current review
      if (relation_id == rid)
        continue;
      std::set<ElementId> elements = ReviewMarker::getReviewElements(map, rid);
      //  Ignore incomplete reviews
      if (elements.size() != 2)
        continue;
      //  Get the match object for the other review item
      std::vector<ElementId> ids(elements.begin(), elements.end());
      MatchPtr match2 = _getCachedMatch(map, relation_id, ids[0], ids[1]);
      //  Check if there is a better review score in another review
      if (match2 && match2->getScore() > score)
      {
        ReviewMarker::removeElement(map, relation_id);
        return;
      }
    }
  }
  //  This is the best match between the two elements, resolve the review
  _resolveMatchReview(match, map, relation_id, eid1, eid2);
}

void ResolveReviewsOp::_resolveMatchReview(const std::shared_ptr<Match>& match,
                                           const std::shared_ptr<OsmMap>& map,
                                           const ElementId& relation_id,
                                           const ElementId& eid1,
                                           const ElementId& eid2) const
{
  LOG_TRACE(
    "Resolving match review: " << relation_id << ", for match: " << match << ", elements: " <<
    eid1 << ", " << eid2 << "...");

  //  Only resolve valid matches
  if (match)
  {
    //  Removing all other distractions, is this a match, miss, or review?
    MatchType t = match->getType();
    if (t == MatchType::Match || t == MatchType::Review)
    {
      MatchSet matches;
      matches.insert(match);
      std::vector<MergerPtr> mergers;
      //  Matches and reviews are going to both be merged here
      set<pair<ElementId, ElementId>> eids;
      eids.emplace(eid1, eid2);
      MergerFactory::getInstance().createMergers(map, matches, mergers);

      for (std::vector<MergerPtr>::iterator it = mergers.begin(); it != mergers.end(); ++it)
      {
        std::vector<std::pair<ElementId, ElementId>> replaced;
        MergerPtr merger = *it;
        merger->apply(map, replaced);
      }
    }
    //  Once the match is merged, delete the review relation or after removing all other distractions
    ReviewMarker::removeElement(map, relation_id);
  }
  else
  {
    //  Manually resolve the review relation
    _resolveManualReview(map, relation_id, eid1, eid2);
  }
}

void ResolveReviewsOp::_resolveManualReview(
  const std::shared_ptr<OsmMap>& map, const ElementId& relation_id, const ElementId& eid1,
  const ElementId& eid2) const
{
  LOG_TRACE(
    "Manually resolving review: " << relation_id << ", elements: " << eid1 << ", " << eid2 << "...");

  ElementPtr element1 = map->getElement(eid1);
  ElementPtr element2 = map->getElement(eid2);
  //  Remove UNKNOWN2
  if (element1->getStatus() == Status::Unknown2)
  {
    ReviewMarker::removeElement(map, eid1);
  }

  if (element2->getStatus() == Status::Unknown2)
  {
    ReviewMarker::removeElement(map, eid2);
  }
  //  Remove the review relation
  ReviewMarker::removeElement(map, relation_id);
}

MatchPtr ResolveReviewsOp::_getCachedMatch(
  const std::shared_ptr<OsmMap>& map, const ElementId& relation_id, const ElementId& eid1,
  const ElementId& eid2)
{
  MatchPtr match;
  //  Check for a previously cached match
  if (_matchCache.find(relation_id.getId()) != _matchCache.end())
    match = _matchCache[relation_id.getId()];
  else
  {
    //  Copy elements to a temporary map for matching
    OsmMapPtr copy_score = std::make_shared<OsmMap>();
    CopyMapSubsetOp(map, eid1, eid2).apply(copy_score);
    copy_score->getElement(eid1)->setStatus(Status::Unknown1);
    copy_score->getElement(eid2)->setStatus(Status::Unknown2);
    //  Create the match
    match = MatchFactory::getInstance().createMatch(map, eid1, eid2);
    _matchCache[relation_id.getId()] = match;
  }
  return match;
}

ResolveReviewsOp::ResolveType ResolveReviewsOp::_resolveString(const QString& type) const
{
  if (type == "resolve")
    return ResolveType::ResolveReviews;
  else if (type == "remove")
    return ResolveType::RemoveReviews;
  else  //  type == "keep"
    return ResolveType::KeepReviews;
}

}
