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
#include "BuildingMerger.h"

// hoot
#include <hoot/core/filters/BaseFilter.h>
#include <hoot/core/ops/BuildingPartMergeOp.h>
#include <hoot/core/ops/RecursiveElementRemover.h>
#include <hoot/core/ops/ReplaceElementOp.h>
#include <hoot/core/schema/TagComparator.h>
#include <hoot/core/schema/TagMergerFactory.h>
#include <hoot/core/schema/OverwriteTagMerger.h>
#include <hoot/core/visitors/CountNodesVisitor.h>
#include <hoot/core/conflate/ReviewMarker.h>

namespace hoot
{

class DeletableBuildingPart : public BaseFilter
{
public:
  DeletableBuildingPart() : BaseFilter(Filter::FilterMatches) {}

  bool isMatch(const Element &e) const
  {
    bool result = false;

    if (e.getElementType() == ElementType::Node && e.getTags().getInformationCount() == 0)
    {
      result = true;
    }
    else if (e.getElementType() != ElementType::Node)
    {
      if (OsmSchema::getInstance().isBuilding(e.getTags(), e.getElementType()) ||
          OsmSchema::getInstance().isBuildingPart(e.getTags(), e.getElementType()))
      {
        result = true;
      }
    }

    return result;
  }

  virtual ElementCriterion* clone() { return new DeletableBuildingPart(); }

};

BuildingMerger::BuildingMerger(const set< pair<ElementId, ElementId> >& pairs) :
  _pairs(pairs)
{
}

void BuildingMerger::apply(const OsmMapPtr& map,
  vector< pair<ElementId, ElementId> >& replaced) const
{
  //check if it is many to many
  set<ElementId> firstPairs;
  set<ElementId> secondPairs;
  set<ElementId> combined;
  set< pair<ElementId, ElementId> >::iterator sit = _pairs.begin();
  while (sit != _pairs.end())
  {
    firstPairs.insert(sit->first);
    secondPairs.insert(sit->second);
    combined.insert(sit->first);
    combined.insert(sit->second);
    sit++;
  }
  if (firstPairs.size() > 1 && secondPairs.size() > 1) //it is many to many
  {
    QString note = "Merging multiple buildings from each data source is error prone and requires a human eye.";
    ReviewMarker::mark(map, combined, note, "Building", 1);
  }
  else
  {
    // use node count as a surrogate for complexity of the geometry.
    CountNodesVisitor count1;
    shared_ptr<Element> e1 = _buildBuilding1(map);
    e1->visitRo(*map, count1);

    CountNodesVisitor count2;
    shared_ptr<Element> e2 = _buildBuilding2(map);
    e2->visitRo(*map, count2);

    shared_ptr<Element> keeper;
    shared_ptr<Element> scrap;
    // keep the more complex building geometry.
    if (count1.getCount() >= count2.getCount())
    {
      keeper = e1;
      scrap = e2;
    }
    else
    {
      keeper = e2;
      scrap = e1;
    }

    // use the default tag merging mechanism
    Tags newTags = TagMergerFactory::mergeTags(e1->getTags(), e2->getTags(), ElementType::Way);

    QStringList ref1;
    e1->getTags().readValues("REF1", ref1);
    QStringList ref2;
    e2->getTags().readValues("REF2", ref2);

    ref1.sort();
    ref2.sort();

    if (ref1.size() != 0 || ref2.size() != 0)
    {
      if (ref1 == ref2)
      {
        newTags["hoot:building:match"] = "true";
      }
      else
      {
        newTags["hoot:building:match"] = "false";
      }
    }

    keeper->setTags(newTags);
    keeper->setStatus(Status::Conflated);

    // remove the duplicate element.
    DeletableBuildingPart filter;
    ReplaceElementOp(scrap->getElementId(), keeper->getElementId()).apply(map);
    RecursiveElementRemover(scrap->getElementId(), &filter).apply(map);
    scrap->getTags().clear();

    set< pair<ElementId, ElementId> > replacedSet;
    for (set< pair<ElementId, ElementId> >::const_iterator it = _pairs.begin();
      it != _pairs.end(); ++it)
    {
      // if we replaced the second group of buildings
      if (it->second != keeper->getElementId())
      {
        replacedSet.insert(pair<ElementId, ElementId>(it->second, keeper->getElementId()));
      }
      if (it->first != keeper->getElementId())
      {
        replacedSet.insert(pair<ElementId, ElementId>(it->first, keeper->getElementId()));
      }
    }
    replaced.insert(replaced.end(), replacedSet.begin(), replacedSet.end());
  }
}

shared_ptr<Element> BuildingMerger::buildBuilding(const OsmMapPtr& map, const set<ElementId>& eid)
{
  assert(eid.size() > 0);

  if (eid.size() == 1)
  {
    return map->getElement(*eid.begin());
  }
  else
  {
    vector< shared_ptr<Element> > parts;
    vector< ElementId > toRemove;
    parts.reserve(eid.size());
    for (set<ElementId>::const_iterator it = eid.begin(); it != eid.end(); ++it)
    {
      shared_ptr<Element> e = map->getElement(*it);
      bool isBuilding = false;
      if (e && e->getElementType() == ElementType::Relation)
      {
        shared_ptr<Relation> r = dynamic_pointer_cast<Relation>(e);
        if (r->getType() == "building")
        {
          isBuilding = true;

          // This is odd. Originally I had a const reference to the result, but that was causing
          // an obscure segfault. I changed it to a copy and everything is happy. I don't know
          // when/where the reference would be changing, but I also don't think this will be
          // a significant optimization issue.
          vector<RelationData::Entry> m = r->getMembers();
          for (size_t i = 0; i < m.size(); ++i)
          {
            if (m[i].getRole() == "part")
            {
              shared_ptr<Element> em = map->getElement(m[i].getElementId());
              // push any non-conflicing tags in the parent relation down into the building part.
              em->setTags(OverwriteTagMerger().mergeTags(em->getTags(), r->getTags(),
                em->getElementType()));
              parts.push_back(em);
            }
          }

          toRemove.push_back(r->getElementId());
        }
      }

      if (!isBuilding)
      {
        parts.push_back(e);
      }
    }

    shared_ptr<Element> result = BuildingPartMergeOp().combineParts(map, parts);

    DeletableBuildingPart filter;

    // likely create a filter that only matches buildings and building parts and pass that to the
    for (size_t i = 0; i < toRemove.size(); i++)
    {
      if (map->containsElement(toRemove[i]))
      {
        ElementPtr willRemove = map->getElement(toRemove[i]);
        ReplaceElementOp(toRemove[i], result->getElementId()).apply(map);
        RecursiveElementRemover(toRemove[i], &filter).apply(map);
        // just in case it wasn't removed (e.g. part of another relation)
        willRemove->getTags().clear();
      }
    }

    return result;
  }
}

shared_ptr<Element> BuildingMerger::_buildBuilding1(const OsmMapPtr& map) const
{
  set<ElementId> e;

  for (set< pair<ElementId, ElementId> >::const_iterator it = _pairs.begin();
    it != _pairs.end(); ++it)
  {
    e.insert(it->first);
  }

  return buildBuilding(map, e);
}

shared_ptr<Element> BuildingMerger::_buildBuilding2(const OsmMapPtr& map) const
{
  set<ElementId> e;

  for (set< pair<ElementId, ElementId> >::const_iterator it = _pairs.begin();
    it != _pairs.end(); ++it)
  {
    e.insert(it->second);
  }

  return buildBuilding(map, e);
}

QString BuildingMerger::toString() const
{
  QString s = hoot::toString(getPairs());
  return QString("BuildingMerger %1").arg(s);
}

}
