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
  std::vector<SchemaVertex> buildingPartTags =
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

QQueue<BuildingPartDescription> BuildingPartMergeOp::_getBuildingPartQueue()
{
  LOG_INFO("\tCreating building part queue...");

  QQueue<BuildingPartDescription> buildingPartQueue;

  const WayMap ways = _map->getWays();
  LOG_VARD(ways.size());
  for (WayMap::const_iterator it = ways.begin(); it != ways.end(); ++it)
  {
    const WayPtr w = it->second;
    if (_buildingCrit.isSatisfied(w))
    {
      boost::shared_ptr<geos::geom::Geometry> g = _getGeometry(w, false);
      const std::vector<long> intersectIds = _map->getIndex().findWays(*g->getEnvelopeInternal());
      LOG_VART(intersectIds.size());
      for (std::vector<long>::const_iterator it = intersectIds.begin(); it != intersectIds.end();
           ++it)
      {
        buildingPartQueue.enqueue(BuildingPartDescription(w, *it, "containedWay", g));
      }

      const std::set<long> neighborIds = _calculateNeighbors(w, w->getTags());
      LOG_VART(neighborIds.size());
      for (std::set<long>::const_iterator it = neighborIds.begin(); it != neighborIds.end(); ++it)
      {
        buildingPartQueue.enqueue(BuildingPartDescription(w, *it, "neighbor", g));
      }
    }
  }

  const RelationMap relations = _map->getRelations();
  LOG_VARD(relations.size());
  for (RelationMap::const_iterator it = relations.begin(); it != relations.end(); ++it)
  {
    const RelationPtr r = it->second;
    if (_buildingCrit.isSatisfied(r))
    {
      boost::shared_ptr<geos::geom::Geometry> g = _getGeometry(r, true);
      const std::vector<long> intersectIds = _map->getIndex().findWays(*g->getEnvelopeInternal());
      LOG_VART(intersectIds.size());
      for (std::vector<long>::const_iterator it = intersectIds.begin(); it != intersectIds.end();
           ++it)
      {
        buildingPartQueue.enqueue(BuildingPartDescription(r, *it, "containedWay", g));
      }

      const std::vector<RelationData::Entry> members = r->getMembers();
      LOG_VART(members.size());
      for (size_t i = 0; i < members.size(); i++)
      {
        const RelationData::Entry memberEntry = members[i];
        const ElementId memberElementId = memberEntry.getElementId();
        if (memberElementId.getType() == ElementType::Way)
        {
          WayPtr member = _map->getWay(memberElementId.getId());
          const std::set<long> neighborIds = _calculateNeighbors(member, r->getTags());
          for (std::set<long>::const_iterator it = neighborIds.begin(); it != neighborIds.end();
               ++it)
          {
            buildingPartQueue.enqueue(BuildingPartDescription(r, *it, "neighbor", g));
          }
        }
      }
    }
  }

  LOG_VARD(buildingPartQueue.size());
  return buildingPartQueue;
}

std::set<long> BuildingPartMergeOp::_calculateNeighbors(const WayPtr& w, const Tags& tags)
{
  LOG_VART(w->getElementId());

  std::set<long> neighborIds;

  long lastId = w->getNodeId(0);
  // go through each of its nodes and look for commonality with other ways
  for (size_t i = 1; i < w->getNodeCount(); i++)
  {
    // find all other ways that use this node (neighbors)
    const std::set<long> ways = _map->getIndex().getNodeToWayMap()->getWaysByNode(w->getNodeId(i));

    // go through each of the neighboring ways
    for (std::set<long>::const_iterator it = ways.begin(); it != ways.end(); ++it)
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

bool BuildingPartMergeOp::_hasContiguousNodes(const WayPtr& w, long n1, long n2)
{
  const std::vector<long> nodes = w->getNodeIds();
  for (size_t i = 0; i < nodes.size() - 1; i++)
  {
    if ((nodes[i] == n1 && nodes[i + 1] == n2) || (nodes[i] == n2 && nodes[i + 1] == n1))
    {
      return true;
    }
  }
  return false;
}

void BuildingPartMergeOp::_preProcessBuildingParts()
{
  QQueue<BuildingPartDescription> buildingPartQueue = _getBuildingPartQueue();

  QMutex buildingPartsInputMutex;
  QMutex hootSchemaMutex;
  QMutex buildingPartGroupsOutputMutex;

  QThreadPool threadPool;
  threadPool.setMaxThreadCount(_threadCount);
  LOG_VART(threadPool.maxThreadCount());
  for (int i = 0; i < _threadCount; i++)
  {
    BuildingPartPreMergeCollector* buildingPartTask = new BuildingPartPreMergeCollector();
    buildingPartTask->setBuildingPartTagNames(_buildingPartTagNames);
    buildingPartTask->setBuildingPartsInput(&buildingPartQueue);
    buildingPartTask->setBuildingPartOutputMutex(&buildingPartGroupsOutputMutex);
    buildingPartTask->setHootSchemaMutex(&hootSchemaMutex);
    buildingPartTask->setBuildingPartInputMutex(&buildingPartsInputMutex);
    buildingPartTask->setMap(_map);
    buildingPartTask->setBuildingPartGroupsOutput(&_ds);
    threadPool.start(buildingPartTask);
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

boost::shared_ptr<geos::geom::Geometry> BuildingPartMergeOp::_getGeometry(
  const ElementPtr& element, const bool checkForBuilding)
{
  boost::shared_ptr<geos::geom::Geometry> g;
  if (!checkForBuilding || _buildingCrit.isSatisfied(element))
  {
    if (element->getElementType() == ElementType::Relation)
    {
      g = _elementConverter->convertToGeometry(boost::dynamic_pointer_cast<Relation>(element));
    }
    else
    {
      g = _elementConverter->convertToGeometry(boost::dynamic_pointer_cast<Way>(element));
    }
  }
  return g;
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

RelationPtr BuildingPartMergeOp::combineParts(const OsmMapPtr& map,
                                              std::vector<ElementPtr>& parts)
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

}
