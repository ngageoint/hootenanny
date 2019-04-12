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
  if (_threadCount < 1 || QThread::idealThreadCount() < _threadCount)
  {
    _threadCount = QThread::idealThreadCount();
  }
  LOG_VARD(_threadCount);
}

QQueue<BuildingPartDescription> BuildingPartMergeOp::_getBuildingPartPreProcessingInput()
{
  LOG_INFO("\tCreating building part pre-processing input...");

  QQueue<BuildingPartDescription> buildingPartInput;

  // Tried caching all of the geometries and passed them to the task threads to avoid repeated
  // geometry calcs, but there were no performance gains.

  const WayMap& ways = _map->getWays();
  LOG_VARD(ways.size());
  for (WayMap::const_iterator it = ways.begin(); it != ways.end(); ++it)
  {
    WayPtr way = it->second;
    if (_buildingCrit.isSatisfied(way))
    {
      boost::shared_ptr<geos::geom::Geometry> geom = _getGeometry(way, false);
      const std::vector<long>& intersectIds =
        _map->getIndex().findWays(*geom->getEnvelopeInternal());
      LOG_VART(intersectIds.size());
      for (std::vector<long>::const_iterator it = intersectIds.begin(); it != intersectIds.end();
           ++it)
      {
        buildingPartInput.enqueue(BuildingPartDescription(way, *it, "containedWay", geom));
      }

      const std::set<long>& neighborIds = _calculateNeighbors(way, way->getTags());
      LOG_VART(neighborIds.size());
      for (std::set<long>::const_iterator it = neighborIds.begin(); it != neighborIds.end(); ++it)
      {
        buildingPartInput.enqueue(BuildingPartDescription(way, *it, "neighbor", geom));
      }
    }
  }

  const RelationMap& relations = _map->getRelations();
  LOG_VARD(relations.size());
  for (RelationMap::const_iterator it = relations.begin(); it != relations.end(); ++it)
  {
    RelationPtr relation = it->second;
    if (_buildingCrit.isSatisfied(relation))
    {
      boost::shared_ptr<geos::geom::Geometry> geom = _getGeometry(relation, true);
      const std::vector<long>& intersectIds =
        _map->getIndex().findWays(*geom->getEnvelopeInternal());
      LOG_VART(intersectIds.size());
      for (std::vector<long>::const_iterator it = intersectIds.begin(); it != intersectIds.end();
           ++it)
      {
        buildingPartInput.enqueue(BuildingPartDescription(relation, *it, "containedWay", geom));
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
            buildingPartInput.enqueue(BuildingPartDescription(relation, *it, "neighbor", geom));
          }
        }
      }
    }
  }

  LOG_VARD(buildingPartInput.size());
  return buildingPartInput;
}

void BuildingPartMergeOp::_preProcessBuildingParts()
{
  QQueue<BuildingPartDescription> buildingPartQueue = _getBuildingPartPreProcessingInput();

  QMutex buildingPartsInputMutex;
  QMutex hootSchemaMutex;
  QMutex buildingPartGroupsOutputMutex;

  QThreadPool threadPool;
  threadPool.setMaxThreadCount(_threadCount);
  LOG_VART(threadPool.maxThreadCount());
  for (int i = 0; i < _threadCount; i++)
  {
    BuildingPartPreMergeCollector* buildingPartCollectTask = new BuildingPartPreMergeCollector();
    buildingPartCollectTask->setBuildingPartTagNames(_buildingPartTagNames);
    buildingPartCollectTask->setBuildingPartsInput(&buildingPartQueue);
    buildingPartCollectTask->setBuildingPartOutputMutex(&buildingPartGroupsOutputMutex);
    buildingPartCollectTask->setHootSchemaMutex(&hootSchemaMutex);
    buildingPartCollectTask->setBuildingPartInputMutex(&buildingPartsInputMutex);
    buildingPartCollectTask->setMap(_map);
    buildingPartCollectTask->setBuildingPartGroupsOutput(&_ds);
    threadPool.start(buildingPartCollectTask);
  }
  LOG_VART(threadPool.activeThreadCount());
  LOG_INFO("\tLaunched " << _threadCount << " building part pre-processing tasks...");

  const bool allThreadsRemoved = threadPool.waitForDone();
  LOG_VART(allThreadsRemoved);

  LOG_VARD(StringUtils::formatLargeNumber(_ds.size()));
}

void BuildingPartMergeOp::_mergeBuildingParts()
{
  // go through each of the grouped buildings
  const Tgs::DisjointSetMap<ElementPtr>::AllGroups& groups = _ds.getAllGroups();
  for (Tgs::DisjointSetMap<ElementPtr>::AllGroups::const_iterator it = groups.begin();
       it != groups.end(); it++)
  {
    // combine the group of building parts into a relation.
    std::vector<ElementPtr> parts = it->second;
    if (parts.size() > 1)
    {
      combineParts(_map, parts);

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
//  LOG_INFO(
//    "\tMerged " << StringUtils::formatLargeNumber(_numAffected) <<
//    " building parts after processing " <<
//    StringUtils::formatLargeNumber(numBuildingGroupsMerged) << " / " <<
//    StringUtils::formatLargeNumber(totalBuildingGroupsProcessed) << " building groups.");
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

bool BuildingPartMergeOp::_hasContiguousNodes(const WayPtr& way, const long n1, const long n2)
{
  const std::vector<long> nodes = way->getNodeIds();
  for (size_t i = 0; i < nodes.size() - 1; i++)
  {
    if ((nodes[i] == n1 && nodes[i + 1] == n2) || (nodes[i] == n2 && nodes[i + 1] == n1))
    {
      return true;
    }
  }
  return false;
}

boost::shared_ptr<geos::geom::Geometry> BuildingPartMergeOp::_getGeometry(
  const ElementPtr& element, const bool checkForBuilding) const
{
  boost::shared_ptr<geos::geom::Geometry> geom;
  if (!checkForBuilding || _buildingCrit.isSatisfied(element))
  {
    if (element->getElementType() == ElementType::Relation)
    {
      geom = _elementConverter->convertToGeometry(boost::dynamic_pointer_cast<Relation>(element));
    }
    else
    {
      geom = _elementConverter->convertToGeometry(boost::dynamic_pointer_cast<Way>(element));
    }
  }
  return geom;
}

void BuildingPartMergeOp::apply(OsmMapPtr& map)
{
  _ds.clear();
  _map = map;
  _elementConverter.reset(new ElementConverter(_map));
  _numAffected = 0;
  _totalBuildingGroupsProcessed = 0;
  _numBuildingGroupsMerged = 0;

  MapProjector::projectToPlanar(map);

  // treat the map as read only while we determine building parts
  _preProcessBuildingParts();

  // time to start making changes to the map
  _mergeBuildingParts();

  // most other operations don't need this index, so we'll clear it out so it isn't actively
  // maintained.
  // TODO: re-enable
  //_map->getIndex().clearRelationIndex();
  //_map.reset();
}

RelationPtr BuildingPartMergeOp::combineParts(const OsmMapPtr& map, std::vector<ElementPtr>& parts)
{
  if (parts.size() == 0)
  {
    throw IllegalArgumentException(
      "No building parts passed to BuildingPartMergeOp::combineParts.");
  }
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
