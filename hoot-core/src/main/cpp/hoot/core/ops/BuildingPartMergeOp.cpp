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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "BuildingPartMergeOp.h"

// geos
#include <geos/util/TopologyException.h>

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/elements/NodeToWayMap.h>
#include <hoot/core/ops/BuildingOutlineUpdateOp.h>
#include <hoot/core/schema/TagComparator.h>
#include <hoot/core/util/GeometryUtils.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/visitors/WorstCircularErrorVisitor.h>

using namespace geos::geom;
using namespace std;
using namespace Tgs;

namespace hoot
{

unsigned int BuildingPartMergeOp::logWarnCount = 0;

HOOT_FACTORY_REGISTER(OsmMapOperation, BuildingPartMergeOp)

BuildingPartMergeOp::BuildingPartMergeOp() :
_numGeometriesCleaned(0),
_numGeometryCacheHits(0)
{
  vector<SchemaVertex> buildingPartTags =
    OsmSchema::getInstance().getAssociatedTagsAsVertices(MetadataTags::BuildingPart() + "=yes");
  for (size_t i = 0; i < buildingPartTags.size(); i++)
  {
    _buildingPartTagNames.insert(buildingPartTags[i].name.split("=")[0]);
  }
  _buildingPartTagNames.insert(MetadataTags::BuildingPart());
}

void BuildingPartMergeOp::_addContainedWaysToGroup(const Geometry& g,
  const boost::shared_ptr<Element>& neighbor)
{
  // merge with buildings that are contained by this polygon

  const vector<long> intersectIds = _map->getIndex().findWays(*g.getEnvelopeInternal());
  int totalProcessed = 0;
  int buildingsAdded = 0;
  for (vector<long>::const_iterator it = intersectIds.begin(); it != intersectIds.end(); ++it)
  {
    const long candidateWayId = *it;
    const WayPtr& candidate = _map->getWay(candidateWayId);

    boost::shared_ptr<Geometry> cg = _getWayGeometry(candidate);

    // if this is another building part totally contained by this building
    if (cg.get())
    {
      bool contains = false;
      try
      {
        contains = g.contains(cg.get());
      }
      catch (const geos::util::TopologyException&)
      {
        boost::shared_ptr<Geometry> cleanCandidate(GeometryUtils::validateGeometry(cg.get()));
        boost::shared_ptr<Geometry> cleanG(GeometryUtils::validateGeometry(&g));
        _wayGeometryCache[candidateWayId] = cleanCandidate;
        contains = cleanG->contains(cleanCandidate.get());
        _numGeometriesCleaned++;
      }

      if (contains)
      {
        _ds.joinT(candidate, neighbor);
        buildingsAdded++;
      }
    }

    totalProcessed++;
    if (totalProcessed % 1000 == 0)
    {
      PROGRESS_DEBUG(
        "\tProcessed " << StringUtils::formatLargeNumber(totalProcessed) << " / " <<
        StringUtils::formatLargeNumber(intersectIds.size()) <<
        " intersecting buildings for: " << neighbor->getElementId());
    }
  }
}

void BuildingPartMergeOp::_addNeighborsToGroup(const WayPtr& w)
{
  const set<long> neighborIds = _calculateNeighbors(w, w->getTags());
  // go through each of the neighboring ways
  for (set<long>::const_iterator it = neighborIds.begin(); it != neighborIds.end(); ++it)
  {
    WayPtr neighbor = _map->getWay(*it);
    // add these two buildings to a set
    _ds.joinT(neighbor, w);
  }
}

void BuildingPartMergeOp::_addNeighborsToGroup(const RelationPtr& r)
{
  _addContainedWaysToGroup(*(_elementConverter->convertToGeometry(r)), r);

  const vector<RelationData::Entry>& members = r->getMembers();
  int totalProcessed = 0;
  int buildingsAdded = 0;
  for (size_t i = 0; i < members.size(); i++)
  {
    const RelationData::Entry memberEntry = members[i];
    const ElementId memberElementId = memberEntry.getElementId();
    if (memberElementId.getType() == ElementType::Way)
    {
      const WayPtr& member = _map->getWay(memberElementId.getId());
      const set<long> neighborIds = _calculateNeighbors(member, r->getTags());
      for (set<long>::const_iterator it = neighborIds.begin(); it != neighborIds.end(); ++it)
      {
        WayPtr neighbor = _map->getWay(*it);
        // add these two buildings to a set
        _ds.joinT(neighbor, r);
        buildingsAdded++;
      }
    }
    else if (memberElementId.getType() == ElementType::Relation)
    {
      if (logWarnCount < Log::getWarnMessageLimit())
      {
        LOG_WARN("Not expecting relations with relation members: " << r->toString());
      }
      else if (logWarnCount == Log::getWarnMessageLimit())
      {
        LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
      }
      logWarnCount++;
    }

    totalProcessed++;
  }
}

boost::shared_ptr<Geometry> BuildingPartMergeOp::_getWayGeometry(const WayPtr& way,
                                                                 const bool checkForBuilding)
{
  boost::shared_ptr<Geometry> g;
  if (_wayGeometryCache.contains(way->getId()))
  {
    g = _wayGeometryCache[way->getId()];
    _numGeometryCacheHits++;
  }
  else if (!checkForBuilding || _buildingCrit.isSatisfied(way))
  {
    g = _elementConverter->convertToGeometry(way);
    _wayGeometryCache[way->getId()] = g;
  }
  return g;
}

void BuildingPartMergeOp::_processWays()
{
  int totalProcessed = 0;
  // go through all the ways
  const WayMap& ways = _map->getWays();
  for (WayMap::const_iterator it = ways.begin(); it != ways.end(); ++it)
  {
    const WayPtr& w = it->second;
    // add the way to a building group, if the way is part of a building
    if (_buildingCrit.isSatisfied(w))
    {
      _addNeighborsToGroup(w);

      // Cache the way geometries now, as that will benefit us when we later process way relation
      // members.
      boost::shared_ptr<Geometry> g = _getWayGeometry(w);
      if (g)
      {
        _addContainedWaysToGroup(*g, w);
        _numAffected++;
      }
    }

    totalProcessed++;
    if (totalProcessed % 1000 == 0)
    {
      PROGRESS_INFO(
        "\tProcessed " << StringUtils::formatLargeNumber(totalProcessed) << " / " <<
        StringUtils::formatLargeNumber(ways.size()) <<
        " ways for building part merging.");
    }
  }
  LOG_DEBUG("\tProcessed " << StringUtils::formatLargeNumber(totalProcessed) << " ways.");
}

void BuildingPartMergeOp::_processRelations()
{
  int totalProcessed = 0;
  // go through all the relations
  const RelationMap& relations = _map->getRelations();
  for (RelationMap::const_iterator it = relations.begin(); it != relations.end(); ++it)
  {
    const RelationPtr& r = it->second;
    // add the relation to a building group if appropriate
    if (_buildingCrit.isSatisfied(r))
    {
      _addNeighborsToGroup(r);
      _numAffected++;
    }

    totalProcessed++;
    if (totalProcessed % 10 == 0)
    {
      PROGRESS_INFO(
        "\tProcessed " << StringUtils::formatLargeNumber(totalProcessed) << " / " <<
        StringUtils::formatLargeNumber(relations.size()) <<
        " relations for building part merging.");
    }
  }
  LOG_DEBUG("\tProcessed " << StringUtils::formatLargeNumber(totalProcessed) << " relations.");
}

void BuildingPartMergeOp::_mergeBuildingParts()
{
  // go through each of the grouped buildings
  int totalProcessed = 0;
  int numBuildingsMerged = 0;
  const DisjointSetMap<boost::shared_ptr<Element>>::AllGroups& groups = _ds.getAllGroups();
  for (DisjointSetMap<boost::shared_ptr<Element>>::AllGroups::const_iterator it = groups.begin();
       it != groups.end(); it++)
  {
    // combine the group of building parts into a relation.
    const vector<boost::shared_ptr<Element>>& parts = it->second;
    if (parts.size() > 1)
    {
      _combineParts(parts);
      numBuildingsMerged += parts.size();
    }

    totalProcessed += parts.size();
    if (totalProcessed % 100 == 0)
    {
      PROGRESS_INFO(
        "\tMerged " << StringUtils::formatLargeNumber(numBuildingsMerged) << " / " <<
        StringUtils::formatLargeNumber(totalProcessed) << " building parts.");
    }
  }
  LOG_DEBUG("\tMerged " << numBuildingsMerged << " buildings.");
}

void BuildingPartMergeOp::apply(OsmMapPtr& map)
{
  MapProjector::projectToPlanar(map);

  ////
  /// treat the map as read only while we determine building parts.
  ////
  _ds.clear();
  _map = map;
  _elementConverter.reset(new ElementConverter(_map));
  _wayGeometryCache.clear();

  _processWays();
  _processRelations();

  LOG_DEBUG(
    "\tCleaned " << StringUtils::formatLargeNumber(_numGeometriesCleaned) <<
    " total building geometries.");
  LOG_DEBUG("\tGeometry cache hits: " << StringUtils::formatLargeNumber(_numGeometryCacheHits));

  ////
  /// Time to start making changes to the map.
  ////
  _mergeBuildingParts();

  // most other operations don't need this index, so we'll clear it out so it isn't actively
  // maintained.
  _map->getIndex().clearRelationIndex();
  _map.reset();
}

set<long> BuildingPartMergeOp::_calculateNeighbors(const WayPtr& w, const Tags& tags)
{
  set<long> neighborIds;

  const NodeToWayMap& n2w = *_map->getIndex().getNodeToWayMap();
  long lastId = w->getNodeId(0);
  // go through each of its nodes and look for commonality with other ways
  for (size_t i = 1; i < w->getNodeCount(); i++)
  {
    // find all other ways that use this node (neighbors)
    const set<long>& ways = n2w.getWaysByNode(w->getNodeId(i));

    // go through each of the neighboring ways
    for (set<long>::const_iterator it = ways.begin(); it != ways.end(); ++it)
    {
      const long wayId = *it;
      WayPtr neighbor = _map->getWay(wayId);
      // if the neighbor is a building and it also has the two contiguous nodes we're looking at
      if (neighbor != w && _buildingCrit.isSatisfied(neighbor) &&
          _hasContiguousNodes(neighbor, w->getNodeId(i), lastId) &&
          _compareTags(tags, neighbor->getTags()))
      {
        // add this to the list of neighbors
        neighborIds.insert(wayId);
      }
    }
    lastId = w->getNodeId(i);
  }

  return neighborIds;
}

void BuildingPartMergeOp::_combineParts(const std::vector<boost::shared_ptr<Element>>& parts)
{
  combineParts(_map, parts);
}

RelationPtr BuildingPartMergeOp::combineParts(const OsmMapPtr& map,
  const vector<boost::shared_ptr<Element>>& parts)
{
  if (parts.size() == 0)
  {
    throw IllegalArgumentException(
      "No building parts passed to BuildingPartMergeOp::combineParts.");
  }

  RelationPtr building(
    new Relation(
      parts[0]->getStatus(), map->createNextRelationId(),
      WorstCircularErrorVisitor::getWorstCircularError(parts), MetadataTags::RelationBuilding()));

  OsmSchema& schema = OsmSchema::getInstance();
  Tags& t = building->getTags();

  for (size_t i = 0; i < parts.size(); i++)
  {
    building->addElement(MetadataTags::RolePart(), parts[i]);

    Tags pt = parts[i]->getTags();

    Tags tCopy = t;
    Tags names;
    TagComparator::getInstance().mergeNames(tCopy, pt, names);
    t.set(names);

    // go through all the tags.
    for (Tags::const_iterator it = pt.begin(); it != pt.end(); ++it)
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
  for (Tags::const_iterator it = tCopy.begin(); it != tCopy.end(); ++it)
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
    parts[i]->getTags()[MetadataTags::BuildingPart()] = "yes";
  }

  map->addRelation(building);
  return building;
}

bool BuildingPartMergeOp::_compareTags(Tags t1, Tags t2)
{
  // remove all the building tags that are building:part=yes specific.
  for (set<QString>::const_iterator it = _buildingPartTagNames.begin();
       it != _buildingPartTagNames.end(); ++it)
  {
    t1.remove(*it);
    t2.remove(*it);
  }

  const double score = TagComparator::getInstance().compareTags(t1, t2);
  // check for score near 1.0
  return fabs(1.0 - score) < 0.001;
}

bool BuildingPartMergeOp::_hasContiguousNodes(const WayPtr& w, long n1, long n2)
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

}
