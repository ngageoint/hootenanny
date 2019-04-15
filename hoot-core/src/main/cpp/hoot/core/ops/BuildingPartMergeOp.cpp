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

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/visitors/WorstCircularErrorVisitor.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/schema/TagComparator.h>
#include <hoot/core/elements/InMemoryElementSorter.h>
#include <hoot/core/elements/NodeToWayMap.h>

// Qt
#include <QThreadPool>
#include <QQueue>
#include <QMutex>

namespace hoot
{

unsigned int BuildingPartMergeOp::logWarnCount = 0;

HOOT_FACTORY_REGISTER(OsmMapOperation, BuildingPartMergeOp)

BuildingPartMergeOp::BuildingPartMergeOp() :
_totalBuildingGroupsProcessed(0),
_numBuildingGroupsMerged(0),
_threadCount(1)
{
  _initBuildingPartTagNames();
}

void BuildingPartMergeOp::_initBuildingPartTagNames()
{
  const std::vector<SchemaVertex>& buildingPartTags =
    OsmSchema::getInstance().getAssociatedTagsAsVertices(MetadataTags::BuildingPart() + "=yes");
  for (size_t i = 0; i < buildingPartTags.size(); i++)
  {
    _buildingPartTagNames.insert(buildingPartTags[i].name.split("=")[0]);
  }
  _buildingPartTagNames.insert(MetadataTags::BuildingPart());
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

void BuildingPartMergeOp::_init(OsmMapPtr& map)
{
  _buildingPartGroups.clear();
  _map = map;
  _elementConverter.reset(new ElementConverter(_map));
  _numAffected = 0;
  _totalBuildingGroupsProcessed = 0;
  _numBuildingGroupsMerged = 0;
}

void BuildingPartMergeOp::apply(OsmMapPtr& map)
{
  _init(map);

  MapProjector::projectToPlanar(map);

  // treat the map as read only while we determine building parts
  _preProcessBuildingParts();

  // time to start making changes to the map
  _mergeBuildingParts();

  // most other operations don't need this index, so we'll clear it out so it isn't actively
  // maintained.
  _map->getIndex().clearRelationIndex();
  _map.reset();
}

QQueue<BuildingPartDescription> BuildingPartMergeOp::_getBuildingPartWayPreProcessingInput()
{
  QQueue<BuildingPartDescription> buildingPartInput;

  const WayMap& ways = _map->getWays();
  LOG_VARD(ways.size());
  for (WayMap::const_iterator it = ways.begin(); it != ways.end(); ++it)
  {
    WayPtr way = it->second;
    boost::shared_ptr<geos::geom::Geometry> geom = _getGeometry(way);
    if (geom)
    {
      const std::vector<long>& intersectIds =
        _map->getIndex().findWays(*geom->getEnvelopeInternal());
      LOG_VART(intersectIds.size());
      for (std::vector<long>::const_iterator it = intersectIds.begin(); it != intersectIds.end();
           ++it)
      {
        WayPtr neighbor = _map->getWay(*it);
        if (_buildingCrit.isSatisfied(neighbor))
        {
          buildingPartInput.enqueue(
            BuildingPartDescription(
              way, neighbor, BuildingPartDescription::BuildingPartRelationType::ContainedWay,
              geom));
        }
      }

      const std::set<long>& neighborIds = _calculateNeighbors(way, way->getTags());
      LOG_VART(neighborIds.size());
      for (std::set<long>::const_iterator it = neighborIds.begin(); it != neighborIds.end(); ++it)
      {
        WayPtr neighbor = _map->getWay(*it);
        // have already checked building status in _calculateNeighbors
        buildingPartInput.enqueue(
          BuildingPartDescription(
            way, neighbor, BuildingPartDescription::BuildingPartRelationType::Neighbor, geom));
      }
    }
  }
  LOG_VARD(buildingPartInput.size());

  return buildingPartInput;
}

QQueue<BuildingPartDescription> BuildingPartMergeOp::_getBuildingPartRelationPreProcessingInput()
{
  QQueue<BuildingPartDescription> buildingPartInput;

  const RelationMap& relations = _map->getRelations();
  LOG_VARD(relations.size());
  for (RelationMap::const_iterator it = relations.begin(); it != relations.end(); ++it)
  {
    RelationPtr relation = it->second;

    boost::shared_ptr<geos::geom::Geometry> geom = _getGeometry(relation);
    if (geom)
    {
      const std::vector<long>& intersectIds =
        _map->getIndex().findWays(*geom->getEnvelopeInternal());
      LOG_VART(intersectIds.size());
      for (std::vector<long>::const_iterator it = intersectIds.begin(); it != intersectIds.end();
           ++it)
      {
        WayPtr neighbor = _map->getWay(*it);
        if (_buildingCrit.isSatisfied(neighbor))
        {
          buildingPartInput.enqueue(
            BuildingPartDescription(
              relation, neighbor, BuildingPartDescription::BuildingPartRelationType::ContainedWay,
              geom));
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
          for (std::set<long>::const_iterator it = neighborIds.begin(); it != neighborIds.end();
               ++it)
          {
            WayPtr neighbor = _map->getWay(*it);
            // have already checked building status in _calculateNeighbors
            buildingPartInput.enqueue(
              BuildingPartDescription(
                relation, neighbor, BuildingPartDescription::BuildingPartRelationType::Neighbor,
                geom));
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
  }
  LOG_VARD(buildingPartInput.size());

  return buildingPartInput;
}

QQueue<BuildingPartDescription> BuildingPartMergeOp::_getBuildingPartPreProcessingInput()
{
  LOG_INFO("\tCreating building part pre-processing input...");

  // Tried caching all of the geometries and in order to pass them to the task threads to not only
  // avoid repeated geometry calcs but also get rid of a call to BuildingCriterion::isSatisified on
  // the threads, which required an extra mutex. However, 1) there were no performance gains in
  // using the cache 2) access to the cache caused thread instability as to which I couldn't
  // determine the cause.

  QQueue<BuildingPartDescription> buildingPartInput;
  buildingPartInput.append(_getBuildingPartWayPreProcessingInput());
  buildingPartInput.append(_getBuildingPartRelationPreProcessingInput());

  LOG_VARD(buildingPartInput.size());
  return buildingPartInput;
}

void BuildingPartMergeOp::_preProcessBuildingParts()
{
  QQueue<BuildingPartDescription> buildingPartsInput = _getBuildingPartPreProcessingInput();

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
  LOG_INFO("\tLaunched " << _threadCount << " building part pre-processing tasks...");

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
      combineBuildingParts(_map, parts);

      _numBuildingGroupsMerged++;
      _numAffected += parts.size();
    }

    _totalBuildingGroupsProcessed++;
    if (_totalBuildingGroupsProcessed % 100 == 0)
    {
      PROGRESS_INFO(
        "\tMerged " << StringUtils::formatLargeNumber(_numAffected) <<
        " building parts after processing " <<
        StringUtils::formatLargeNumber(_numBuildingGroupsMerged) << " / " <<
        StringUtils::formatLargeNumber(_totalBuildingGroupsProcessed) << " building groups.");
    }
  }
}

std::set<long> BuildingPartMergeOp::_calculateNeighbors(const WayPtr& way, const Tags& tags)
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
      if (neighbor != way && _buildingCrit.isSatisfied(neighbor) &&
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

bool BuildingPartMergeOp::_compareTags(Tags t1, Tags t2)
{
  // remove all the building tags that are building:part=yes specific.
  for (std::set<QString>::const_iterator it = _buildingPartTagNames.begin();
       it != _buildingPartTagNames.end(); ++it)
  {
    t1.remove(*it);
    t2.remove(*it);
  }

  const double score = TagComparator::getInstance().compareTags(t1, t2);
  // check for score near 1.0
  return fabs(1.0 - score) < 0.001;
}

bool BuildingPartMergeOp::_hasContiguousNodes(const WayPtr& way, const long node1Id,
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

boost::shared_ptr<geos::geom::Geometry> BuildingPartMergeOp::_getGeometry(
  const ElementPtr& element) const
{
  if (_buildingCrit.isSatisfied(element))
  {
    switch (element->getElementType().getEnum())
    {
      case ElementType::Way:
        return _elementConverter->convertToGeometry(boost::dynamic_pointer_cast<Way>(element));
      case ElementType::Relation:
        return _elementConverter->convertToGeometry(boost::dynamic_pointer_cast<Relation>(element));
      default:
        throw IllegalArgumentException(
          "Unexpected element type: " + element->getElementType().toString());
    }
  }
  return boost::shared_ptr<geos::geom::Geometry>();
}

RelationPtr BuildingPartMergeOp::combineBuildingParts(const OsmMapPtr& map,
                                                      std::vector<ElementPtr>& parts)
{
  if (parts.size() == 0)
  {
    throw IllegalArgumentException(
      "No building parts passed to BuildingPartMergeOp::combineParts.");
  }
  // This is primarily in place to support testable output.
  InMemoryElementSorter::sort(parts);

  RelationPtr building(
    new Relation(
      parts[0]->getStatus(), map->createNextRelationId(),
      WorstCircularErrorVisitor::getWorstCircularError(parts), MetadataTags::RelationBuilding()));

  OsmSchema& schema = OsmSchema::getInstance();
  Tags& tags = building->getTags();

  for (size_t i = 0; i < parts.size(); i++)
  {
    building->addElement(MetadataTags::RolePart(), parts[i]);

    Tags partTags = parts[i]->getTags();

    Tags tagsCopy = tags;
    Tags names;
    TagComparator::getInstance().mergeNames(tagsCopy, partTags, names);
    tags.set(names);

    // go through all the tags.
    for (Tags::const_iterator it = partTags.begin(); it != partTags.end(); ++it)
    {
      // ignore all keys that are building:part specific.
      if (_buildingPartTagNames.find(it.key()) == _buildingPartTagNames.end())
      {
        // if the tag isn't already in the relation
        if (tags.contains(it.key()) == false)
        {
          tags[it.key()] = it.value();
        }
        // if this is an arbitrary text value, then concatenate the values.
        else if (schema.isTextTag(it.key()))
        {
          tags.appendValueIfUnique(it.key(), it.value());
        }
        // if the tag is in the relation and the tags differ.
        else if (tags[it.key()] != it.value())
        {
          tags[it.key()] = "";
        }
      }
    }
  }

  // go through all the keys that were consistent for each of the parts and move them into the
  // relation.
  Tags tagsCopy = tags;
  for (Tags::const_iterator it = tagsCopy.begin(); it != tagsCopy.end(); ++it)
  {
    // if the value is empty, then the tag isn't needed, or it wasn't consistent between multiple
    // parts.
    if (it.value() == "")
    {
      tags.remove(it.key());
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

  if (tags.contains("building") == false)
  {
    tags["building"] = "yes";
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

}
