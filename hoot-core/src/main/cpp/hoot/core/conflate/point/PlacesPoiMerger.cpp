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
#include "PlacesPoiMerger.h"

// hoot
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/ops/RemoveElementOp.h>
#include <hoot/core/schema/TagMergerFactory.h>
#include <hoot/core/util/Settings.h>

namespace hoot
{

PlacesPoiMerger::PlacesPoiMerger(const set<pair<ElementId, ElementId> > &pairs) : _pairs(pairs)
{
  setConfiguration(conf());
}

void PlacesPoiMerger::apply(const OsmMapPtr& map, vector< pair<ElementId, ElementId> >& replaced)
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

      // use the default tag merging mechanism
      tags = _merger->mergeTags(tags, map->getNode(eid.getId())->getTags(), ElementType::Node);

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
  n->setStatus(Status::Conflated);
}

void PlacesPoiMerger::setConfiguration(const Settings& conf)
{
  if (conf.hasKey(placesTagMerger()))
  {
    _merger = &TagMergerFactory::getInstance().getMerger(ConfigOptions(conf).getPlacesTagMerger());
  }
  else
  {
    _merger = &TagMergerFactory::getInstance().getDefault();
  }
}

}
