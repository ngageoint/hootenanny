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
 * @copyright Copyright (C) 2015, 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "BuildingPartMergeOp.h"

// geos
#include <geos/geom/Polygon.h>
#include <geos/util/TopologyException.h>

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/conflate/NodeToWayMap.h>
#include <hoot/core/ops/BuildingOutlineUpdateOp.h>
#include <hoot/core/schema/TagComparator.h>
#include <hoot/core/util/ElementConverter.h>
#include <hoot/core/util/GeometryUtils.h>
#include <hoot/core/schema/OsmSchema.h>

// tgs
#include <tgs/StreamUtils.h>
#include <tgs/System/Time.h>

using namespace Tgs;

namespace hoot
{

unsigned int BuildingPartMergeOp::logWarnCount = 0;

HOOT_FACTORY_REGISTER(OsmMapOperation, BuildingPartMergeOp)

BuildingPartMergeOp::BuildingPartMergeOp()
{
  vector<SchemaVertex> buildingPartTags =
      OsmSchema::getInstance().getAssociatedTags("building:part=yes");
  for (size_t i = 0; i < buildingPartTags.size(); i++)
  {
    _buildingPartTagNames.insert(buildingPartTags[i].name.split("=")[0]);
  }
  _buildingPartTagNames.insert("building:part");
}

void BuildingPartMergeOp::_addContainedWaysToGroup(const Geometry& g,
  const boost::shared_ptr<Element>& neighbor)
{
  // merge with buildings that are contained by this polygon
  vector<long> intersectIds = _map->getIndex().findWays(*g.getEnvelopeInternal());
  for (size_t i = 0; i < intersectIds.size(); i++)
  {
    const boost::shared_ptr<Way>& candidate = _map->getWay(intersectIds[i]);
    // if this is another building part totally contained by this building
    if (_isBuildingPart(candidate))
    {
      bool contains = false;
      try
      {
        boost::shared_ptr<Geometry> cg = ElementConverter(_map).convertToGeometry(candidate);
        contains = g.contains(cg.get());
      }
      catch (geos::util::TopologyException& e)
      {
        boost::shared_ptr<Geometry> cg = ElementConverter(_map).convertToGeometry(candidate);
        auto_ptr<Geometry> cleanCandidate(GeometryUtils::validateGeometry(cg.get()));
        auto_ptr<Geometry> cleanG(GeometryUtils::validateGeometry(&g));
        contains = cleanG->contains(cleanCandidate.get());
      }

      if (contains)
      {
        _ds.joinT(candidate, neighbor);
      }
    }
  }
}

void BuildingPartMergeOp::_addNeighborsToGroup(const boost::shared_ptr<Way>& w)
{
  set<long> neighborIds = _calculateNeighbors(w, w->getTags());
  // go through each of the neighboring ways.
  for (set<long>::const_iterator it = neighborIds.begin(); it != neighborIds.end(); it++)
  {
    boost::shared_ptr<Way> neighbor = _map->getWay(*it);
    // add these two buildings to a set.
    _ds.joinT(neighbor, w);
  }
}

void BuildingPartMergeOp::_addNeighborsToGroup(const boost::shared_ptr<Relation>& r)
{
  boost::shared_ptr<Geometry> mp;
  mp = ElementConverter(_map).convertToGeometry(r);
  _addContainedWaysToGroup(*mp, r);

  const vector<RelationData::Entry>& members = r->getMembers();

  for (size_t i = 0; i < members.size(); i++)
  {
    if (members[i].getElementId().getType() == ElementType::Way)
    {
      const boost::shared_ptr<Way>& member = _map->getWay(members[i].getElementId().getId());

      set<long> neighborIds = _calculateNeighbors(member, r->getTags());
      // got through each of the neighboring ways.
      for (set<long>::const_iterator it = neighborIds.begin(); it != neighborIds.end(); it++)
      {
        boost::shared_ptr<Way> neighbor = _map->getWay(*it);
        // add these two buildings to a set.
        _ds.joinT(neighbor, r);
      }
    }
    if (members[i].getElementId().getType() == ElementType::Relation)
    {
      if (logWarnCount < ConfigOptions().getLogWarnMessageLimit())
      {
        LOG_WARN("Not expecting relations of relations: " << r->toString());
      }
      else if (logWarnCount == ConfigOptions().getLogWarnMessageLimit())
      {
        LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
      }
      logWarnCount++;
    }
  }
}

void BuildingPartMergeOp::apply(boost::shared_ptr<OsmMap>& map)
{
  MapProjector::projectToPlanar(map);
  ////
  // treat the map as read only while we determine building parts.
  ////
  _ds.clear();
  _map = map;

  size_t i;
  i = 0;
  // go through all the ways
  const WayMap& ways = map->getWays();
  for (WayMap::const_iterator it = ways.begin(); it != ways.end(); it++)
  {
    if (Log::getInstance().getLevel() <= Log::Info && i % 1000 == 0)
    {
      cout << "Ways: " << i << " / " << ways.size() << "        \r";
      cout << flush;
    }
    const boost::shared_ptr<Way>& w = it->second;
    // add the way to a building group if appropriate
    // if the way is part of a building
    if (_isBuildingPart(w))
    {
      _addNeighborsToGroup(w);
      boost::shared_ptr<Geometry> g = ElementConverter(_map).convertToGeometry(w);
      _addContainedWaysToGroup(*g, w);
    }
    i++;
  }
  if (Log::getInstance().getLevel() <= Log::Info)
  {
    cout << "Ways: " << ways.size() << " / " << ways.size() << "        \r";
    cout << endl << flush;
  }

  i = 0;
  // go through all the relations
  const RelationMap& relations = map->getRelations();
  for (RelationMap::const_iterator it = relations.begin(); it != relations.end(); it++)
  {
    if (Log::getInstance().getLevel() <= Log::Info /* && i % 100 == 0 */)
    {
      cout << "Relations: " << i << " / " << relations.size() << "        \r";
      cout << flush;
    }
    const boost::shared_ptr<Relation>& r = it->second;
    // add the relation to a building group if appropriate
    if (_isBuildingPart(r))
    {
      _addNeighborsToGroup(r);
    }
    i++;
  }
  if (Log::getInstance().getLevel() <= Log::Info)
  {
    cout << "Relations: " << relations.size() << " / " << relations.size() << "        \r";
    cout << endl << flush;
  }

  ////
  // Time to start making changes to the map.
  ////

  // go through each of the grouped buildings
  i = 0;
  const DisjointSetMap< boost::shared_ptr<Element> >::AllGroups& groups = _ds.getAllGroups();
  for (DisjointSetMap< boost::shared_ptr<Element> >::AllGroups::const_iterator it = groups.begin();
       it != groups.end(); it++)
  {
    if (Log::getInstance().getLevel() <= Log::Info && i % 1000 == 0)
    {
      cout << "Combining Parts: " << i << " / " << groups.size() << "        \r";
      cout << flush;
    }
    // combine the group of building parts into a relation.
    const vector< boost::shared_ptr<Element> >& parts = it->second;
    if (parts.size() > 1)
    {
      _combineParts(parts);
    }
    i++;
  }
  if (Log::getInstance().getLevel() <= Log::Info)
  {
    cout << "Combining Parts: " << groups.size() << " / " << groups.size() << "        \r";
    cout << endl << flush;
  }

  // most other operations don't need this index, so we'll clear it out so it isn't actively
  // maintainted.
  _map->getIndex().clearRelationIndex();
  _map.reset();
}

set<long> BuildingPartMergeOp::_calculateNeighbors(const boost::shared_ptr<Way>& w, const Tags& tags)
{
  set<long> neighborIds;
  const NodeToWayMap& n2w = *_map->getIndex().getNodeToWayMap();
  long lastId = w->getNodeId(0);
  // go through each of its nodes and look for commonality with other ways.
  for (size_t i = 1; i < w->getNodeCount(); i++)
  {
    // find all other ways that use this node (neighbors).
    const set<long>& ways = n2w.getWaysByNode(w->getNodeId(i));

    // go through each of the neighboring ways.
    for (set<long>::const_iterator it = ways.begin(); it != ways.end(); it++)
    {
      boost::shared_ptr<Way> neighbor = _map->getWay(*it);
      // if the neighbor is a building and it also has the two contiguos nodes we're looking at
      if (neighbor != w &&
          _isBuildingPart(neighbor) &&
          _hasContiguousNodes(neighbor, w->getNodeId(i), lastId) &&
          _compareTags(tags, neighbor->getTags()))
      {
        // add this to the list of neighbors
        neighborIds.insert(*it);
      }
    }
    lastId = w->getNodeId(i);
  }

  return neighborIds;
}

boost::shared_ptr<Relation> BuildingPartMergeOp::combineParts(const OsmMapPtr& map,
  const vector< boost::shared_ptr<Element> >& parts)
{
  LOG_VART(parts.size());
  assert(parts.size() > 0);

  boost::shared_ptr<Relation> building(
    new Relation(
      parts[0]->getStatus(),
      map->createNextRelationId(),
      -1,
      "building"));

  OsmSchema& schema = OsmSchema::getInstance();
  Tags& t = building->getTags();

  for (size_t i = 0; i < parts.size(); i++)
  {
    building->addElement("part", parts[i]);

    Tags pt = parts[i]->getTags();

    Tags tCopy = t;
    Tags names;
    TagComparator::getInstance().mergeNames(tCopy, pt, names);
    t.set(names);

    // go through all the tags.
    for (Tags::const_iterator it = pt.begin(); it != pt.end(); it++)
    {
      // ignore all keys that are building:part specific.
      if (_buildingPartTagNames.find(it.key()) == _buildingPartTagNames.end())
      {
        // if the tag isn't already in the relation
        if (t.contains(it.key()) == false)
        {
          t[it.key()] = it.value();
        }
        // if this is an arbitrary text value, then concatenate the values.
        else if (schema.isTextTag(it.key()))
        {
          t.appendValueIfUnique(it.key(), it.value());
        }
        // if the tag is in the relation and the tags differ.
        else if (t[it.key()] != it.value())
        {
          t[it.key()] = "";
        }
      }
    }
  }

  // go through all the keys that were consistent for each of the parts and move them into the
  // relation.
  Tags tCopy = t;
  for (Tags::const_iterator it = tCopy.begin(); it != tCopy.end(); it++)
  {
    // if the value is empty, then the tag isn't needed, or it wasn't consistent between multiple
    // parts.
    if (it.value() == "")
    {
      t.remove(it.key());
    }
    // if the tag isn't empty, remove it from each of the parts.
    else
    {
      for (size_t i = 0; i < parts.size(); i++)
      {
        parts[i]->getTags().remove(it.key());
      }
    }
  }

  if (t.contains("building") == false)
  {
    t["building"] = "yes";
  }

  // replace the building tag with building:part tags.
  for (size_t i = 0; i < parts.size(); i++)
  {
    parts[i]->getTags().remove("building");
    parts[i]->getTags()["building:part"] = "yes";
  }

  map->addRelation(building);
  return building;
}

bool BuildingPartMergeOp::_compareTags(Tags t1, Tags t2)
{
  // remove all the building tags that are building:part=yes specific.
  for (set<QString>::const_iterator it = _buildingPartTagNames.begin();
    it != _buildingPartTagNames.end(); it++)
  {
    t1.remove(*it);
    t2.remove(*it);
  }

  double score = TagComparator::getInstance().compareTags(t1, t2);
  // check for score near 1.0
  return fabs(1.0 - score) < 0.001;
}

bool BuildingPartMergeOp::_hasContiguousNodes(const boost::shared_ptr<Way>& w, long n1, long n2)
{
  const std::vector<long>& nodes = w->getNodeIds();

  for (size_t i = 0; i < nodes.size() - 1; i++)
  {
    if ((nodes[i] == n1 && nodes[i + 1] == n2) ||
        (nodes[i] == n2 && nodes[i + 1] == n1))
    {
      return true;
    }
  }
  return false;
}

bool BuildingPartMergeOp::_isBuildingPart(const boost::shared_ptr<Way>& w)
{
  bool result = false;
  if (OsmSchema::getInstance().isBuilding(w->getTags(), w->getElementType()))
  {
    result = true;
  }
  return result;
}

bool BuildingPartMergeOp::_isBuildingPart(const boost::shared_ptr<Relation>& r)
{
  bool result = false;
  if (OsmSchema::getInstance().isBuilding(r->getTags(), r->getElementType()))
  {
    result = true;
  }
  return result;
}

}
