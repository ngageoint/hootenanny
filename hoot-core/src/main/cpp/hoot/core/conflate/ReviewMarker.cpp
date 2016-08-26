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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "ReviewMarker.h"

#include <hoot/core/util/Log.h>
#include <hoot/core/ops/RemoveElementOp.h>

// Tgs
#include <tgs/RStarTree/HilbertCurve.h>

namespace hoot
{

QString ReviewMarker::_complexGeometryType = "Bad Geometry";
QString ReviewMarker::_revieweeKey = "reviewee";
QString ReviewMarker::_reviewUuidKey = "hoot:review:uuid";
QString ReviewMarker::_reviewScoreKey = "hoot:review:score";
QString ReviewMarker::reviewSortOrderKey = "hoot:review:sort_order";
QString ReviewMarker::_reviewNeedsKey = "hoot:review:needs";
QString ReviewMarker::_reviewNoteKey = "hoot:review:note";
QString ReviewMarker::_reviewTypeKey = "hoot:review:type";
QString ReviewMarker::_reviewChoicesKey = "hoot:review:choices";
QString ReviewMarker::reviewMemberCountKey = "hoot:review:members";


ReviewMarker::ReviewMarker()
{
}

set<ElementId> ReviewMarker::getReviewElements(const ConstOsmMapPtr &map, ReviewUid uid)
{
  set<ElementId> result;

  ConstRelationPtr r = map->getRelation(uid.getId());

  if (r)
  {
    const vector<RelationData::Entry>& entries = r->getMembers();

    for (size_t i = 0; i < entries.size(); i++)
    {
      result.insert(entries[i].getElementId());
    }
  }

  return result;
}

set<ElementId> ReviewMarker::_getReviewRelations(const ConstOsmMapPtr &map, ElementId eid)
{
  set<ElementId> result = map->getParents(eid);

  for (set<ElementId>::iterator it = result.begin(); it != result.end();)
  {
    set<ElementId>::iterator current = it++;
    ElementId p = *current;
    if (p.getType() != ElementType::Relation ||
        map->getRelation(p.getId())->getType() != Relation::REVIEW)
    {
      result.erase(current);
    }
  }

  return result;
}

QString ReviewMarker::getReviewType(const ConstOsmMapPtr &map, ReviewUid uid)
{
  assert(isReviewUid(map, uid));

  ConstRelationPtr r = map->getRelation(uid.getId());

  return r->getTags()[_reviewTypeKey];
}

set<ReviewMarker::ReviewUid> ReviewMarker::getReviewUids(const ConstOsmMapPtr &map)
{
  set<ElementId> result;

  const RelationMap& relations = map->getRelationMap();
  for (RelationMap::const_iterator it = relations.begin(); it != relations.end(); ++it)
  {
    shared_ptr<Relation> relation = it->second;
    if (relation->getElementType() == ElementType::Relation || relation->getType() == Relation::REVIEW)
    {
      result.insert(relation->getElementId());
    }
  }

  return result;
}

set<ReviewMarker::ReviewUid> ReviewMarker::getReviewUids(const ConstOsmMapPtr &map,
  ConstElementPtr e1)
{
  return _getReviewRelations(map, e1->getElementId());
}

bool ReviewMarker::isNeedsReview(const ConstOsmMapPtr &map, ConstElementPtr e1)
{
  // get all the review relations for e1
  set<ElementId> review1 = _getReviewRelations(map, e1->getElementId());

  // if there are more than one relations in the intersection, return true.
  return review1.size() >= 1;
}

bool ReviewMarker::isNeedsReview(const ConstOsmMapPtr &map, ConstElementPtr e1, ConstElementPtr e2)
{
  // get all the review relations for e1
  set<ElementId> review1 = _getReviewRelations(map, e1->getElementId());
  // get all the review relations for e2
  set<ElementId> review2 = _getReviewRelations(map, e2->getElementId());

  // intersect the relations
  set<ElementId> intersection;
  set_intersection(review1.begin(), review1.end(), review2.begin(), review2.end(),
    std::inserter(intersection, intersection.begin()));

  // if there are more than one relations in the intersection, return true.
  return intersection.size() >= 1;
}

bool ReviewMarker::isReviewUid(const ConstOsmMapPtr &map, ReviewUid uid)
{
  bool result = false;

  if (uid.getType() == ElementType::Relation)
  {
    ConstRelationPtr r = map->getRelation(uid.getId());

    if (r->getTags().isTrue(_reviewNeedsKey))
    {
      result = true;
    }
  }

  return result;
}

void ReviewMarker::mark(const OsmMapPtr &map, const ElementPtr& e1, const ElementPtr& e2,
  const QString& note, const QString &reviewType, double score, vector<QString> choices)
{
  if (note.isEmpty())
  {
    //LOG_VARD(e1->toString());
    //LOG_VARD(e2->toString());
    throw IllegalArgumentException("You must specify a review note.");
  }

  RelationPtr r(new Relation(Status::Conflated, map->createNextRelationId(), 0, Relation::REVIEW));
  r->getTags().set(_reviewNeedsKey, true);
  r->getTags().appendValueIfUnique(_reviewNoteKey, note);
  r->getTags().appendValueIfUnique(_reviewTypeKey, reviewType);
  r->getTags().set(_reviewScoreKey, score);
  r->addElement(_revieweeKey, e1->getElementId());
  r->addElement(_revieweeKey, e2->getElementId());
  r->getTags().set(reviewMemberCountKey, (int)r->getMembers().size());
  r->setCircularError(-1);


  //LOG_VARD(r->getId());
  //LOG_VARD(e1->getElementId());
  //LOG_VARD(e2->getElementId());

  for (unsigned int i = 0; i < choices.size(); i++)
  {
    r->getTags()[_reviewChoicesKey + ":" + QString::number(i+1)] = choices[i];
  }

  map->addElement(r);
}

void ReviewMarker::mark(const OsmMapPtr &map, set<ElementId> ids, const QString& note,
   const QString& reviewType, double score, vector<QString> choices)
{
  if (note.isEmpty())
  {
    throw IllegalArgumentException("You must specify a review note.");
  }

  RelationPtr r(new Relation(Status::Conflated, map->createNextRelationId(), 0, Relation::REVIEW));
  r->getTags().set(_reviewNeedsKey, true);
  r->getTags().appendValueIfUnique(_reviewNoteKey, note);
  r->getTags().appendValueIfUnique(_reviewTypeKey, reviewType);
  r->getTags().set(_reviewScoreKey, score);
  set<ElementId>::iterator it = ids.begin();
  while (it != ids.end())
  {
    ElementId id = *it;
    r->addElement(_revieweeKey, id);
    it++;
  }
  r->getTags().set(reviewMemberCountKey, (int)r->getMembers().size());
  r->setCircularError(-1);

  //LOG_VARD(r->getId());
  //LOG_VARD(ids);

  for (unsigned int i = 0; i < choices.size(); i++)
  {
    r->getTags()[_reviewChoicesKey + ":" + QString::number(i+1)] = choices[i];
  }

  map->addElement(r);

}

void ReviewMarker::mark(const OsmMapPtr& map, const ElementPtr& e, const QString& note,
  const QString &reviewType, double score, vector<QString> choices)
{
  if (note.isEmpty())
  {
    //LOG_VARD(e->toString())
    throw IllegalArgumentException("You must specify a review note.");
  }

  RelationPtr r(new Relation(Status::Conflated, map->createNextRelationId(), 0, Relation::REVIEW));
  r->getTags().set(_reviewNeedsKey, true);
  r->getTags().appendValueIfUnique(_reviewNoteKey, note);
  r->getTags().appendValueIfUnique(_reviewTypeKey, reviewType);
  r->getTags().set(_reviewScoreKey, score);
  r->addElement(_revieweeKey, e->getElementId());
  r->getTags().set(reviewMemberCountKey, (int)r->getMembers().size());
  r->setCircularError(-1);

  //LOG_VARD(r->getId());
  //LOG_VARD(e->getElementId());

  for (unsigned int i = 0; i < choices.size(); i++)
  {
    r->getTags()[_reviewChoicesKey + ":" + QString::number(i+1)] = choices[i];
  }

  map->addElement(r);
}

void ReviewMarker::removeElement(const OsmMapPtr& map, ElementId eid)
{
  RemoveElementOp::removeElement(map, eid);
}

}
