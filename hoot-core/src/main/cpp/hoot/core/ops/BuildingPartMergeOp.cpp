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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#include "BuildingPartMergeOp.h"

// Hoot
#include <hoot/core/conflate/polygon/BuildingMerger.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/elements/NodeToWayMap.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/schema/BuildingRelationMemberTagMerger.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/schema/PreserveTypesTagMerger.h>
#include <hoot/core/schema/TagComparator.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/ConfigOptions.h>

// Qt
#include <QThreadPool>
#include <QQueue>
#include <QMutex>

namespace hoot
{

int BuildingPartMergeOp::logWarnCount = 0;

HOOT_FACTORY_REGISTER(OsmMapOperation, BuildingPartMergeOp)

BuildingPartMergeOp::BuildingPartMergeOp(bool preserveTypes) :
_totalBuildingGroupsProcessed(0),
_numBuildingGroupsMerged(0),
_threadCount(1),
_preserveTypes(preserveTypes)
{
}

void BuildingPartMergeOp::setConfiguration(const Settings& conf)
{
  ConfigOptions confOpts = ConfigOptions(conf);

  _threadCount = confOpts.getBuildingPartMergerThreadCount();
  if (_threadCount < 1)
  {
    _threadCount = QThread::idealThreadCount();
  }
  LOG_VARD(_threadCount);
}

void BuildingPartMergeOp::_init(const OsmMapPtr& map)
{
  _buildingPartGroups.clear();
  _map = map;
  _elementToGeometryConverter = std::make_shared<ElementToGeometryConverter>(_map);
  _numAffected = 0;
  _totalBuildingGroupsProcessed = 0;
  _numBuildingGroupsMerged = 0;
}

void BuildingPartMergeOp::apply(OsmMapPtr& map)
{
  _init(map);

  MapProjector::projectToPlanar(map);

  // Before merging, we'll identify all of the building part candidates and send them to threads
  // to be processed into mergeable groups, as some of the logic used is intensive. At this point,
  // treat the map as read only.
  _preProcessBuildingParts();

  // Now, merge the building part groups into buildings.  Changes to the map will be made.
  _mergeBuildingParts();

  // most other operations don't need this index, so we'll clear it out so it isn't actively
  // maintained.
  _map->getIndex().clearRelationIndex();
  _map.reset();
}

QQueue<BuildingPartRelationship> BuildingPartMergeOp::_getBuildingPartWayPreProcessingInput() const
{
  QQueue<BuildingPartRelationship> buildingPartInput;

  const WayMap& ways = _map->getWays();
  LOG_VARD(ways.size());
  int numProcessed = 0;
  for (WayMap::const_iterator it = ways.begin(); it != ways.end(); ++it)
  {
    WayPtr way = it->second;
    std::shared_ptr<geos::geom::Geometry> buildingGeom = _getGeometry(way);
    // If the way wasn't a building, this will be null.
    if (buildingGeom && !buildingGeom->isEmpty())
    {
      const std::vector<long>& intersectIds =
        _map->getIndex().findWays(*buildingGeom->getEnvelopeInternal());
      LOG_VART(intersectIds.size());
      for (std::vector<long>::const_iterator intersection_it = intersectIds.begin(); intersection_it != intersectIds.end();
           ++intersection_it)
      {
        WayPtr neighbor = _map->getWay(*intersection_it);
        // see related note about _buildingCrit in BuildingPartMergeOp::_calculateNeighbors
        if (_buildingCrit.isSatisfied(neighbor))
        {
          buildingPartInput.enqueue(
            BuildingPartRelationship(
              way, buildingGeom, neighbor,
              BuildingPartRelationship::BuildingPartRelationshipType::ContainedWay));
        }
      }

      const std::set<long>& neighborIds = _calculateNeighbors(way, way->getTags());
      LOG_VART(neighborIds.size());
      for (std::set<long>::const_iterator neighbor_it = neighborIds.begin(); neighbor_it != neighborIds.end(); ++neighbor_it)
      {
        WayPtr neighbor = _map->getWay(*neighbor_it);
        // have already checked building status for neighbor in _calculateNeighbors
        buildingPartInput.enqueue(
          BuildingPartRelationship(
            way, buildingGeom, neighbor,
            BuildingPartRelationship::BuildingPartRelationshipType::Neighbor));
      }
    }

    numProcessed++;
    if (numProcessed % 10000 == 0)
    {
      PROGRESS_INFO(
        "\tAdded " << StringUtils::formatLargeNumber(numProcessed) << " of " <<
        StringUtils::formatLargeNumber(ways.size()) << " ways to building part input.");
    }
  }
  LOG_VARD(buildingPartInput.size());

  return buildingPartInput;
}

QQueue<BuildingPartRelationship> BuildingPartMergeOp::_getBuildingPartRelationPreProcessingInput() const
{
  QQueue<BuildingPartRelationship> buildingPartInput;

  const RelationMap& relations = _map->getRelations();
  LOG_VARD(relations.size());
  int numProcessed = 0;
  for (RelationMap::const_iterator it = relations.begin(); it != relations.end(); ++it)
  {
    RelationPtr relation = it->second;

    std::shared_ptr<geos::geom::Geometry> buildingGeom = _getGeometry(relation);
    // If the relation wasn't a building, this will be null.
    if (buildingGeom && !buildingGeom->isEmpty())
    {
      const std::vector<long>& intersectIds =
        _map->getIndex().findWays(*buildingGeom->getEnvelopeInternal());
      LOG_VART(intersectIds.size());
      for (std::vector<long>::const_iterator intersection_it = intersectIds.begin(); intersection_it != intersectIds.end();
           ++intersection_it)
      {
        WayPtr neighbor = _map->getWay(*intersection_it);
        // see related note about _buildingCrit in BuildingPartMergeOp::_calculateNeighbors
        if (_buildingCrit.isSatisfied(neighbor))
        {
          buildingPartInput.enqueue(
            BuildingPartRelationship(
              relation, buildingGeom, neighbor,
              BuildingPartRelationship::BuildingPartRelationshipType::ContainedWay));
        }
      }

      const std::vector<RelationData::Entry>& members = relation->getMembers();
      LOG_VART(members.size());
      for (size_t i = 0; i < members.size(); i++)
      {
        const RelationData::Entry& memberEntry = members[i];
        const ElementId memberElementId = memberEntry.getElementId();
        if (memberElementId.getType() == ElementType::Way)
        {
          WayPtr member = _map->getWay(memberElementId.getId());
          const std::set<long> neighborIds = _calculateNeighbors(member, relation->getTags());
          for (std::set<long>::const_iterator neighbor_it = neighborIds.begin(); neighbor_it != neighborIds.end();
               ++neighbor_it)
          {
            WayPtr neighbor = _map->getWay(*neighbor_it);
            // have already checked building status for neighbor in _calculateNeighbors
            buildingPartInput.enqueue(
              BuildingPartRelationship(
                relation, buildingGeom, neighbor,
                BuildingPartRelationship::BuildingPartRelationshipType::Neighbor));
          }
        }
        else if (members[i].getElementId().getType() == ElementType::Relation)
        {
          if (logWarnCount < Log::getWarnMessageLimit())
          {
            LOG_WARN("\tNot expecting relations of relations: " << relation->toString());
          }
          else if (logWarnCount == Log::getWarnMessageLimit())
          {
            LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
          }
          logWarnCount++;
        }
      }
    }

    numProcessed++;
    if (numProcessed % 10000 == 0)
    {
      PROGRESS_INFO(
        "\tAdded " << StringUtils::formatLargeNumber(numProcessed) << " of " <<
        StringUtils::formatLargeNumber(relations.size()) << " relations to building part input.");
    }
  }
  LOG_VARD(buildingPartInput.size());

  return buildingPartInput;
}

QQueue<BuildingPartRelationship> BuildingPartMergeOp::_getBuildingPartPreProcessingInput() const
{
  // Tried caching all of the geometries beforehand in order to pass them to the task threads to
  // try to not only avoid repeated geometry calcs but also get rid of an extra call to
  // BuildingCriterion::isSatisified (protected by a mutex). However, there were no performance
  // gains in using the cache.

  QQueue<BuildingPartRelationship> buildingPartInput;
  buildingPartInput.append(_getBuildingPartWayPreProcessingInput());
  buildingPartInput.append(_getBuildingPartRelationPreProcessingInput());

  LOG_VARD(buildingPartInput.size());
  return buildingPartInput;
}

void BuildingPartMergeOp::_preProcessBuildingParts()
{
  QQueue<BuildingPartRelationship> buildingPartsInput = _getBuildingPartPreProcessingInput();

  QMutex buildingPartsInputMutex;
  QMutex hootSchemaMutex;
  QMutex buildingPartGroupsOutputMutex;

  QThreadPool threadPool;
  threadPool.setMaxThreadCount(_threadCount);
  LOG_VART(threadPool.maxThreadCount());
  for (int i = 0; i < _threadCount; i++)
  {
    BuildingPartPreMergeCollector* buildingPartCollectTask = new BuildingPartPreMergeCollector();
    buildingPartCollectTask->setBuildingPartsInput(&buildingPartsInput);
    buildingPartCollectTask->setStartingInputSize(buildingPartsInput.size());
    // Passing the groups into the threads as a shared pointer slows down processing by ~60%, so
    // will pass in as a raw pointer.
    buildingPartCollectTask->setBuildingPartGroupsOutput(&_buildingPartGroups);
    buildingPartCollectTask->setMap(_map);
    buildingPartCollectTask->setBuildingPartInputMutex(&buildingPartsInputMutex);
    buildingPartCollectTask->setHootSchemaMutex(&hootSchemaMutex);
    buildingPartCollectTask->setBuildingPartOutputMutex(&buildingPartGroupsOutputMutex);
    threadPool.start(buildingPartCollectTask);
  }
  LOG_VART(threadPool.activeThreadCount());
  LOG_DEBUG("\tLaunched " << _threadCount << " building part pre-processing tasks...");

  const bool allThreadsRemoved = threadPool.waitForDone();
  LOG_VART(allThreadsRemoved);

  LOG_VARD(StringUtils::formatLargeNumber(_buildingPartGroups.size()));
}

void BuildingPartMergeOp::_mergeBuildingParts()
{
  // go through each of the grouped buildings
  const Tgs::DisjointSetMap<ElementPtr>::AllGroups& groups = _buildingPartGroups.getAllGroups();
  for (Tgs::DisjointSetMap<ElementPtr>::AllGroups::const_iterator it = groups.begin();
       it != groups.end(); it++)
  {
    // combine the group of building parts into a relation.
    std::vector<ElementPtr> parts = it->second;
    if (parts.size() > 1)
    {
      BuildingMerger::combineConstituentBuildingsIntoRelation(_map, parts, _preserveTypes);
      _numBuildingGroupsMerged++;
      _numAffected += parts.size();
    }

    _totalBuildingGroupsProcessed++;
    if (_totalBuildingGroupsProcessed % 10000 == 0)
    {
      PROGRESS_INFO(
        "\tMerged " << StringUtils::formatLargeNumber(_numAffected) <<
        " building parts after processing " <<
        StringUtils::formatLargeNumber(_numBuildingGroupsMerged) << " of " <<
        StringUtils::formatLargeNumber(_totalBuildingGroupsProcessed) << " building groups.");
    }
  }
}

std::set<long> BuildingPartMergeOp::_calculateNeighbors(const ConstWayPtr& way, const Tags& tags) const
{
  LOG_VART(way->getElementId());

  std::set<long> neighborIds;

  long lastId = way->getNodeId(0);
  // go through each of its nodes and look for commonality with other ways
  for (size_t i = 1; i < way->getNodeCount(); i++)
  {
    // find all other ways that use this node (neighbors)
    const std::set<long>& ways =
      _map->getIndex().getNodeToWayMap()->getWaysByNode(way->getNodeId(i));

    // go through each of the neighboring ways
    for (std::set<long>::const_iterator it = ways.begin(); it != ways.end(); ++it)
    {
      const long wayId = *it;

      WayPtr neighbor = _map->getWay(wayId);
      // if the neighbor is a building and it also has the two contiguous nodes we're looking at
      if (neighbor != way &&
          // It seems like maybe this should be more strict and check for building parts, not just
          // buildings, but that's not how the code was originally written.
          _buildingCrit.isSatisfied(neighbor) &&
          _hasContiguousNodes(neighbor, way->getNodeId(i), lastId) &&
          _compareTags(tags, neighbor->getTags()))
      {
        // add this to the list of neighbors
        neighborIds.insert(wayId);
      }
    }
    lastId = way->getNodeId(i);
  }

  return neighborIds;
}

bool BuildingPartMergeOp::_compareTags(Tags t1, Tags t2) const
{
  // remove all the building tags that are building:part=yes specific.
  const QSet<QString> buildingPartTagNames =
    BuildingRelationMemberTagMerger::getBuildingPartTagNames();
  for (QSet<QString>::const_iterator it = buildingPartTagNames.begin();
       it != buildingPartTagNames.end(); ++it)
  {
    t1.remove(*it);
    t2.remove(*it);
  }

  const double score = TagComparator::getInstance().compareTags(t1, t2);
  // check for score near 1.0
  return fabs(1.0 - score) < 0.001;
}

bool BuildingPartMergeOp::_hasContiguousNodes(const ConstWayPtr& way, const long node1Id,
                                              const long node2Id)
{
  const std::vector<long>& nodes = way->getNodeIds();
  for (size_t i = 0; i < nodes.size() - 1; i++)
  {
    if ((nodes[i] == node1Id && nodes[i + 1] == node2Id) ||
        (nodes[i] == node2Id && nodes[i + 1] == node1Id))
    {
      return true;
    }
  }
  return false;
}

std::shared_ptr<geos::geom::Geometry> BuildingPartMergeOp::_getGeometry(
  const ConstElementPtr& element) const
{
  // see related note about _buildingCrit in BuildingPartMergeOp::_calculateNeighbors
  if (_buildingCrit.isSatisfied(element))
  {
    switch (element->getElementType().getEnum())
    {
      case ElementType::Way:
        return
          _elementToGeometryConverter->convertToGeometry(
            std::dynamic_pointer_cast<const Way>(element));
      case ElementType::Relation:
        return
          _elementToGeometryConverter->convertToGeometry(
            std::dynamic_pointer_cast<const Relation>(element));
      default:
        throw IllegalArgumentException(
          "Unexpected element type: " + element->getElementType().toString());
    }
  }
  return std::shared_ptr<geos::geom::Geometry>();
}
}
