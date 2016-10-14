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
#include "CustomPoiMerger.h"

// hoot
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/ops/RemoveElementOp.h>
#include <hoot/core/schema/TagComparator.h>

namespace hoot
{

CustomPoiMerger::CustomPoiMerger(const set<pair<ElementId, ElementId> > &pairs) : _pairs(pairs)
{
}

void CustomPoiMerger::apply(const OsmMapPtr& map, vector< pair<ElementId, ElementId> >& replaced)
  const
{
  set<ElementId> pois = getImpactedElementIds();

  // find the node with the best circular error.
  ElementId bestEid;
  Meters bestAcc = numeric_limits<Meters>::max();
  for (set<ElementId>::iterator it = pois.begin(); it != pois.end(); ++it)
  {
    ElementId eid = *it;

    const ConstNodePtr& n = map->getNode(eid.getId());
    if (n->getCircularError() < bestAcc || (n->getCircularError() == bestAcc && n->getStatus() == Status::Unknown1))
    {
      bestAcc = n->getCircularError();
      bestEid = eid;
    }
  }

  // the best circular error node becomes the geometry for the merger.
  const NodePtr& n = map->getNode(bestEid.getId());

  // grab the tags out of the best node.
  Tags tags = n->getTags();
  for (set<ElementId>::iterator it = pois.begin(); it != pois.end(); ++it)
  {
    ElementId eid = *it;

    // if this isn't the best node
    if (eid != bestEid)
    {
      // record that this ndoe is getting replaced.
      replaced.push_back(pair<ElementId, ElementId>(eid, bestEid));

      // average all the tags together.
      tags = TagComparator::getInstance().generalize(tags, map->getNode(eid.getId())->getTags());

      // if the POI has no parents, then simply remove it.
      if (map->getIndex().getParents(eid).size() == 0)
      {
        RemoveElementOp::removeElement(map, eid);
      }
      // if the POI has parents, then remove all tags on the node.
      else
      {
        map->getNode(eid.getId())->getTags().clear();
      }
    }
  }

  n->setTags(tags);
}

set<ElementId> CustomPoiMerger::getImpactedElementIds() const
{
  set<ElementId> result;

  for (set< pair<ElementId, ElementId> >::iterator it = _pairs.begin(); it != _pairs.end(); ++it)
  {
    result.insert(it->first);
    result.insert(it->second);
  }

  return result;
}

bool CustomPoiMerger::isValid(const ConstOsmMapPtr& map) const
{
  bool result = true;
  for (set< pair<ElementId, ElementId> >::iterator it = _pairs.begin();
    result && it != _pairs.end(); ++it)
  {
    result = result &&  map->containsElement(it->first) && map->containsElement(it->second);
    result = result && it->first.getType() == ElementType::Node;
    result = result && it->second.getType() == ElementType::Node;
  }

  return result;
}

void CustomPoiMerger::replace(ElementId oldEid, ElementId newEid)
{
  set< pair<ElementId, ElementId> > tmp = _pairs;
  _pairs.clear();

  for (set< pair<ElementId, ElementId> >::iterator it = tmp.begin(); it != tmp.end(); ++it)
  {
    pair<ElementId, ElementId> old;

    if (old.first == oldEid)
    {
      old.first = newEid;
    }
    if (old.second == oldEid)
    {
      old.second == newEid;
    }

    _pairs.insert(old);
  }
}


}
