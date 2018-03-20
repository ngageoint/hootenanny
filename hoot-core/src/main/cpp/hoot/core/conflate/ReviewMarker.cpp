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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "ReviewMarker.h"

#include <hoot/core/ops/RemoveElementOp.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/MetadataTags.h>

// Tgs
#include <tgs/RStarTree/HilbertCurve.h>

using namespace std;

namespace hoot
{

QString ReviewMarker::_complexGeometryType = "Bad Geometry";

ReviewMarker::ReviewMarker() :
_addReviewTagsToFeatures(ConfigOptions().getAddReviewTagsToFeatures())
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
        map->getRelation(p.getId())->getType() != MetadataTags::RelationReview())
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

  return r->getTags()[MetadataTags::HootReviewType()];
}

set<ReviewMarker::ReviewUid> ReviewMarker::getReviewUids(const ConstOsmMapPtr &map)
{
  set<ElementId> result;

  const RelationMap& relations = map->getRelations();
  for (RelationMap::const_iterator it = relations.begin(); it != relations.end(); ++it)
  {
    RelationPtr relation = it->second;
    if (relation->getElementType() == ElementType::Relation || relation->getType() == MetadataTags::RelationReview())
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

bool ReviewMarker::isReview(ConstElementPtr e)
{
  bool result = false;

  if (e->getElementType() == ElementType::Relation)
  {
    if (e->getTags().isTrue(MetadataTags::HootReviewNeeds()))
    {
      result = true;
    }
  }

  return result;
}

bool ReviewMarker::isReviewUid(const ConstOsmMapPtr &map, ReviewUid uid)
{
  return isReview(map->getElement(uid));
}

void ReviewMarker::mark(const OsmMapPtr &map, const ElementPtr& e1, const ElementPtr& e2,
  const QString& note, const QString &reviewType, double score, vector<QString> choices)
{
  LOG_TRACE("Marking review...");

  LOG_VART(reviewType);
  LOG_VART(note);

  if (note.isEmpty())
  {
    LOG_VART(e1->toString());
    LOG_VART(e2->toString());
    throw IllegalArgumentException("You must specify a review note.");
  }

  RelationPtr r(
    new Relation(
      Status::Conflated, map->createNextRelationId(), 0, MetadataTags::RelationReview()));
  r->getTags().set(MetadataTags::HootReviewNeeds(), true);
  if (_addReviewTagsToFeatures)
  {
    e1->getTags().set(MetadataTags::HootReviewNeeds(), true);
    e2->getTags().set(MetadataTags::HootReviewNeeds(), true);
  }
  r->getTags().appendValueIfUnique(MetadataTags::HootReviewType(), reviewType);
  if (ConfigOptions().getWriterIncludeConflateReviewDetailTags())
  {
    r->getTags().appendValueIfUnique(MetadataTags::HootReviewNote(), note.simplified());
    r->getTags().set(MetadataTags::HootReviewScore(), score);
  }
  r->addElement(MetadataTags::RoleReviewee(), e1->getElementId());
  r->addElement(MetadataTags::RoleReviewee(), e2->getElementId());
  r->getTags().set(MetadataTags::HootReviewMembers(), (int)r->getMembers().size());
  r->setCircularError(-1);

  LOG_VART(r->getId());
  LOG_VART(e1->getElementId());
  LOG_VART(e2->getElementId());

  for (unsigned int i = 0; i < choices.size(); i++)
  {
    r->getTags()[MetadataTags::HootReviewChoices() + ":" + QString::number(i+1)] = choices[i];
  }

  map->addElement(r);
}

void ReviewMarker::mark(const OsmMapPtr &map, set<ElementId> ids, const QString& note,
   const QString& reviewType, double score, vector<QString> choices)
{
  LOG_TRACE("Marking review...");

  LOG_VART(reviewType);
  LOG_VART(note);

  if (note.isEmpty())
  {
    throw IllegalArgumentException("You must specify a review note.");
  }

  RelationPtr r(
    new Relation(
      Status::Conflated, map->createNextRelationId(), 0, MetadataTags::RelationReview()));
  r->getTags().set(MetadataTags::HootReviewNeeds(), true);
  if (_addReviewTagsToFeatures)
  {
    for (set<ElementId>::iterator itr = ids.begin(); itr != ids.end(); ++itr)
    {
      map->getElement(*itr)->getTags().set(MetadataTags::HootReviewNeeds(), true);
    }
  }
  r->getTags().appendValueIfUnique(MetadataTags::HootReviewType(), reviewType);
  if (ConfigOptions().getWriterIncludeConflateReviewDetailTags())
  {
    r->getTags().appendValueIfUnique(MetadataTags::HootReviewNote(), note.simplified());
    r->getTags().set(MetadataTags::HootReviewScore(), score);
  }
  for (set<ElementId>::iterator it = ids.begin(); it != ids.end(); ++it)
  {
    ElementId id = *it;
    r->addElement(MetadataTags::RoleReviewee(), id);
  }
  r->getTags().set(MetadataTags::HootReviewMembers(), (int)r->getMembers().size());
  r->setCircularError(-1);

  LOG_VART(r->getId());
  LOG_VART(ids);

  for (unsigned int i = 0; i < choices.size(); i++)
  {
    r->getTags()[MetadataTags::HootReviewChoices() + ":" + QString::number(i+1)] = choices[i];
  }

  map->addElement(r);
}

void ReviewMarker::mark(const OsmMapPtr& map, const ElementPtr& e, const QString& note,
  const QString &reviewType, double score, vector<QString> choices)
{
  LOG_TRACE("Marking review with note: " << note);

  LOG_VART(reviewType);
  LOG_VART(note);

  if (note.isEmpty())
  {
    LOG_VART(e->toString())
    throw IllegalArgumentException("You must specify a review note.");
  }

  RelationPtr r(
    new Relation(
      Status::Conflated, map->createNextRelationId(), 0, MetadataTags::RelationReview()));
  r->getTags().set(MetadataTags::HootReviewNeeds(), true);
  if (_addReviewTagsToFeatures)
  {
    e->getTags().set(MetadataTags::HootReviewNeeds(), true);
  }
  r->getTags().appendValueIfUnique(MetadataTags::HootReviewType(), reviewType);
  if (ConfigOptions().getWriterIncludeConflateReviewDetailTags())
  {
    r->getTags().appendValueIfUnique(MetadataTags::HootReviewNote(), note.simplified());
    r->getTags().set(MetadataTags::HootReviewScore(), score);
  }
  r->addElement(MetadataTags::RoleReviewee(), e->getElementId());
  r->getTags().set(MetadataTags::HootReviewMembers(), (int)r->getMembers().size());
  r->setCircularError(-1);

  LOG_VART(r->getId());
  LOG_VART(e->getElementId());

  for (unsigned int i = 0; i < choices.size(); i++)
  {
    r->getTags()[MetadataTags::HootReviewChoices() + ":" + QString::number(i+1)] = choices[i];
  }

  map->addElement(r);
}

void ReviewMarker::removeElement(const OsmMapPtr& map, ElementId eid)
{
  LOG_TRACE("Removing review element: " << eid);
  RemoveElementOp::removeElement(map, eid);
}

}
