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
 * @copyright Copyright (C) 2014, 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "ReviewMarker.h"

#include <hoot/core/util/Log.h>

namespace hoot
{

ReviewMarker::ReviewMarker()
{
}

set<ElementId> ReviewMarker::_getReviewRelations(const ConstOsmMapPtr &map, ElementId eid) const
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

void ReviewMarker::mark(const OsmMapPtr &map, ElementPtr& e1, ElementPtr& e2, const QString& note,
  double score)
{
  RelationPtr r(new Relation(Status::Conflated, map->createNextRelationId(), 0, Relation::REVIEW));
  r->getTags().set(reviewNeedsKey(), true);
  if (note.isEmpty())
  {
    throw IllegalArgumentException("You must specify a review note.");
  }
  r->getTags().appendValueIfUnique(reviewNoteKey(), note);
  r->getTags().set(reviewScoreKey(), score);
  r->addElement(revieweeKey(), e1->getElementId());
  r->addElement(revieweeKey(), e2->getElementId());
  r->setCircularError(-1);
  map->addElement(r);
}

void ReviewMarker::mark(const OsmMapPtr& map, ElementPtr& e, const QString& note, double score)
{
  RelationPtr r(new Relation(Status::Conflated, map->createNextRelationId(), 0, Relation::REVIEW));
  r->getTags().set(reviewNeedsKey(), true);
  if (note.isEmpty())
  {
    throw IllegalArgumentException("You must specify a review note.");
  }
  r->getTags().appendValueIfUnique(reviewNoteKey(), note);
  r->getTags().set(reviewScoreKey(), score);
  r->addElement(revieweeKey(), e->getElementId());
  r->setCircularError(-1);
  map->addElement(r);
}

}
