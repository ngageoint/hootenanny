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
#include <hoot/core/util/GeometryUtils.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/visitors/WorstCircularErrorVisitor.h>
#include <hoot/core/schema/OsmSchema.h>

// Qt
#include <QThreadPool>
#include <QUuid>

namespace hoot
{

RelationBuildingPartProcessor::RelationBuildingPartProcessor() :
_numGeometryCacheHits(0),
_numGeometriesCleaned(0),
_numProcessed(0)
{
}

void RelationBuildingPartProcessor::setMap(OsmMapPtr map)
{
  _map = map;
  _n2w = _map->getIndex().getNodeToWayMap();
  _elementConverter.reset(new ElementConverter(_map));
}

void RelationBuildingPartProcessor::run()
{
  _id = QUuid::createUuid().toString();
  LOG_INFO("Starting thread: " << _id << "...");
  while (!_buildingPartQueue->empty())
  {
    _buildingPartQueueMutex->lock();
    ElementPtr buildingPart = _buildingPartQueue->dequeue();
    _buildingPartQueueMutex->unlock();
    if (buildingPart)
    {
      LOG_VARD(buildingPart->getElementId());
      if (buildingPart->getElementType() == ElementType::Relation)
      {
        _addNeighborsToGroup(boost::dynamic_pointer_cast<Relation>(buildingPart));
      }
      else
      {
        WayPtr wayBuildingPart = boost::dynamic_pointer_cast<Way>(buildingPart);
        LOG_VART(wayBuildingPart == 0);
        _addNeighborsToGroup(wayBuildingPart);
        boost::shared_ptr<geos::geom::Geometry> g = _getWayGeometry(wayBuildingPart, false);
        if (g)
        {
          _addContainedWaysToGroup(*g, wayBuildingPart);
        }
      }
      _numProcessed++;
    }
    LOG_VART(_buildingPartQueue->size());
  }
  LOG_VAR(_id);
  LOG_VAR(_buildingPartGroups->size());
  LOG_VAR(_numProcessed);
  LOG_VAR(_numGeometryCacheHits);
  LOG_VAR(_numGeometriesCleaned);
}

bool RelationBuildingPartProcessor::_compareTags(Tags t1, Tags t2)
{
  // remove all the building tags that are building:part=yes specific.
  for (std::set<QString>::const_iterator it = _buildingPartTagNames.begin();
       it != _buildingPartTagNames.end(); ++it)
  {
    t1.remove(*it);
    t2.remove(*it);
  }

  _schemaMutex->lock();
  const double score = TagComparator::getInstance().compareTags(t1, t2);
  _schemaMutex->unlock();
  // check for score near 1.0
  return fabs(1.0 - score) < 0.001;
}

bool RelationBuildingPartProcessor::_hasContiguousNodes(const WayPtr& w, long n1, long n2)
{
  const std::vector<long> nodes = w->getNodeIds();
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

std::set<long> RelationBuildingPartProcessor::_calculateNeighbors(const WayPtr& w,
                                                                  const Tags& tags)
{
  LOG_VARD(w->getElementId());

  std::set<long> neighborIds;

  long lastId = w->getNodeId(0);
  // go through each of its nodes and look for commonality with other ways
  for (size_t i = 1; i < w->getNodeCount(); i++)
  {
    // find all other ways that use this node (neighbors)
    const std::set<long> ways = _n2w->getWaysByNode(w->getNodeId(i));

    // go through each of the neighboring ways
    for (std::set<long>::const_iterator it = ways.begin(); it != ways.end(); ++it)
    {
      const long wayId = *it;
      WayPtr neighbor = _map->getWay(wayId);
      LOG_VART(neighbor == 0);
      // if the neighbor is a building and it also has the two contiguous nodes we're looking at
      if (neighbor != w && _isBuilding(neighbor) &&
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

void RelationBuildingPartProcessor::_addNeighborsToGroup(const WayPtr& w)
{
  LOG_VART(w->getElementId());
  const std::set<long> neighborIds = _calculateNeighbors(w, w->getTags());
  LOG_VARD(neighborIds.size());
  // go through each of the neighboring ways
  for (std::set<long>::const_iterator it = neighborIds.begin(); it != neighborIds.end(); ++it)
  {
    WayPtr neighbor = _map->getWay(*it);
    LOG_VAR(neighbor == 0);
    // add these two buildings to a set
    _buildingPartGroupMutex->lock();
    _buildingPartGroups->joinT(neighbor, w);
    LOG_VARD(_buildingPartGroups->size());
    _buildingPartGroupMutex->unlock();
  }
}

void RelationBuildingPartProcessor::_addNeighborsToGroup(const RelationPtr& r)
{
  LOG_VART(r->getElementId());

  _schemaMutex->lock(); // TODO: necessary?
  boost::shared_ptr<geos::geom::Geometry> relationGeom = _elementConverter->convertToGeometry(r);
  _schemaMutex->unlock();
  _addContainedWaysToGroup(*relationGeom, r);

  const std::vector<RelationData::Entry> members = r->getMembers();
  LOG_VARD(members.size());
  for (size_t i = 0; i < members.size(); i++)
  {
    const RelationData::Entry memberEntry = members[i];
    const ElementId memberElementId = memberEntry.getElementId();
    if (memberElementId.getType() == ElementType::Way)
    {
      WayPtr member = _map->getWay(memberElementId.getId());
      const std::set<long> neighborIds = _calculateNeighbors(member, r->getTags());
      for (std::set<long>::const_iterator it = neighborIds.begin(); it != neighborIds.end(); ++it)
      {
        WayPtr neighbor = _map->getWay(*it);
        // add these two buildings to a set
        _buildingPartGroupMutex->lock();
        _buildingPartGroups->joinT(neighbor, r);
        LOG_VARD(_buildingPartGroups->size());
        _buildingPartGroupMutex->unlock();
      }
    }
  }
}

boost::shared_ptr<geos::geom::Geometry> RelationBuildingPartProcessor::_getWayGeometry(
  const WayPtr& way, const bool checkForBuilding)
{
  boost::shared_ptr<geos::geom::Geometry> g;
  if (!checkForBuilding || _isBuilding(way))
  {
    _schemaMutex->lock();
    g = _elementConverter->convertToGeometry(way);
    _schemaMutex->unlock();
  }
  return g;
}

bool RelationBuildingPartProcessor::_isBuilding(const ElementPtr& element) const
{
  _schemaMutex->lock();
  const bool isBuilding = _buildingCrit.isSatisfied(element);
  _schemaMutex->unlock();
  return isBuilding;
}

void RelationBuildingPartProcessor::_addContainedWaysToGroup(const geos::geom::Geometry& g,
                                                             const ElementPtr& neighbor)
{
  LOG_VARD(neighbor->getElementId());

  // merge with buildings that are contained by this polygon
  _mapIndexMutex->lock();
  const std::vector<long> intersectIds = _map->getIndex().findWays(*g.getEnvelopeInternal());
  _mapIndexMutex->unlock();
  LOG_VARD(intersectIds.size());
  for (std::vector<long>::const_iterator it = intersectIds.begin(); it != intersectIds.end(); ++it)
  {
    const long candidateWayId = *it;
    LOG_VARD(candidateWayId);

    WayPtr candidate = _map->getWay(candidateWayId);
    LOG_VART(candidate == 0);

    boost::shared_ptr<geos::geom::Geometry> cg = _getWayGeometry(candidate);
    LOG_VART(cg == 0);
    // if this is another building part totally contained by this building
    if (cg)
    {
      bool contains = false;
      try
      {
        contains = g.contains(cg.get());
        LOG_VART(contains);
      }
      catch (const geos::util::TopologyException&)
      {
        LOG_DEBUG("cleaning...");
        _geomUtilsMutex->lock();
        boost::shared_ptr<geos::geom::Geometry> cleanCandidate(
          GeometryUtils::validateGeometry(cg.get()));
        boost::shared_ptr<geos::geom::Geometry> cleanG(GeometryUtils::validateGeometry(&g));
        _geomUtilsMutex->unlock();
        contains = cleanG->contains(cleanCandidate.get());
        LOG_VART(contains);
        _numGeometriesCleaned++;
      }
      LOG_VART(contains);

      if (contains)
      {
        _buildingPartGroupMutex->lock();
        _buildingPartGroups->joinT(candidate, neighbor);
        LOG_VARD(_buildingPartGroups->size());
        _buildingPartGroupMutex->unlock();
      }
    }
  }
}

unsigned int BuildingPartMergeOp::logWarnCount = 0;

HOOT_FACTORY_REGISTER(OsmMapOperation, BuildingPartMergeOp)

BuildingPartMergeOp::BuildingPartMergeOp() :
_numGeometriesCleaned(0),
_numGeometryCacheHits(0),
_totalContainedBuildingsAdded(0),
_totalContainedWaysProcessed(0)
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
  if (_threadCount < -2)  //TODO: temp hack
  {
    throw IllegalArgumentException(
      "Invalid BuildingPartMergerOp thread count: " + QString::number(_threadCount));
  }
  else if (_threadCount < 1)
  {
    _threadCount = QThread::idealThreadCount();
  }
  LOG_VARD(_threadCount);
}

QQueue<ElementPtr> BuildingPartMergeOp::_getBuildingPartQueue()
{
  QQueue<ElementPtr> buildingPartQueue;

  const RelationMap& relations = _map->getRelations();
  LOG_VAR(relations.size());
  for (RelationMap::const_iterator it = relations.begin(); it != relations.end(); ++it)
  {
    const RelationPtr& r = it->second;
    if (_buildingCrit.isSatisfied(r))
    {
      buildingPartQueue.enqueue(r);
    }
  }

  const WayMap& ways = _map->getWays();
  for (WayMap::const_iterator it = ways.begin(); it != ways.end(); ++it)
  {
    const WayPtr& w = it->second;
    if (_buildingCrit.isSatisfied(w))
    {
      //buildingPartQueue.enqueue(w);   // TODO: re-enable
    }
  }

  return buildingPartQueue;
}

void BuildingPartMergeOp::_processBuildingParts()
{
  QQueue<ElementPtr> buildingPartQueue = _getBuildingPartQueue();
  LOG_VAR(buildingPartQueue.size());

  QMutex buildingPartGroupMutex(QMutex::Recursive);
  QMutex schemaMutex(QMutex::Recursive);
  QMutex mapIndexMutex(QMutex::Recursive);
  QMutex geomUtilsMutex(QMutex::Recursive);
  QMutex buildingPartQueueMutex(QMutex::Recursive);

  QThreadPool threadPool;
  threadPool.setMaxThreadCount(_threadCount);
  LOG_VARD(threadPool.maxThreadCount());
  LOG_INFO("Launching " << _threadCount << " processing tasks...");
  for (int i = 0; i < _threadCount; i++)
  {
    RelationBuildingPartProcessor* buildingPartTask = new RelationBuildingPartProcessor();

    buildingPartTask->setBuildingPartTagNames(_buildingPartTagNames);
    buildingPartTask->setBuildingPartQueue(&buildingPartQueue);
    buildingPartTask->setBuildingPartGroupMutex(&buildingPartGroupMutex);
    buildingPartTask->setSchemaMutex(&schemaMutex);
    buildingPartTask->setMapIndexMutex(&mapIndexMutex);
    buildingPartTask->setGeomUtilsMutex(&geomUtilsMutex);
    buildingPartTask->setBuildingPartQueueMutex(&buildingPartQueueMutex);
    buildingPartTask->setMap(_map); // TODO: send copy?
    buildingPartTask->setBuildingPartGroups(&_ds);

    threadPool.start(buildingPartTask);
  }
  LOG_VARD(threadPool.activeThreadCount());
  const bool allThreadsRemoved = threadPool.waitForDone();
  LOG_VARD(allThreadsRemoved);
}

void BuildingPartMergeOp::_addContainedWaysToGroup(const geos::geom::Geometry& g,
                                                   const ElementPtr& neighbor)
{ 
  // merge with buildings that are contained by this polygon

  const std::vector<long> intersectIds = _map->getIndex().findWays(*g.getEnvelopeInternal());
  int totalProcessed = 0;
  int buildingsAdded = 0;
  for (std::vector<long>::const_iterator it = intersectIds.begin(); it != intersectIds.end(); ++it)
  {
    const long candidateWayId = *it;
    const WayPtr& candidate = _map->getWay(candidateWayId);

    boost::shared_ptr<geos::geom::Geometry> cg = _getWayGeometry(candidate);
    // if this is another building part totally contained by this building
    if (cg)
    {
      bool contains = false;
      try
      {
        contains = g.contains(cg.get());
      }
      catch (const geos::util::TopologyException&)
      {
        boost::shared_ptr<geos::geom::Geometry> cleanCandidate(
          GeometryUtils::validateGeometry(cg.get()));
        boost::shared_ptr<geos::geom::Geometry> cleanG(GeometryUtils::validateGeometry(&g));
        _wayGeometryCache[candidateWayId] = cleanCandidate;
        contains = cleanG->contains(cleanCandidate.get());
        _numGeometriesCleaned++;
      }

      if (contains)
      {
        _ds.joinT(candidate, neighbor);
        buildingsAdded++;
        _totalContainedBuildingsAdded++;
      }
    }

    _totalContainedWaysProcessed++;
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
  const std::set<long> neighborIds = _calculateNeighbors(w, w->getTags());
  // go through each of the neighboring ways
  for (std::set<long>::const_iterator it = neighborIds.begin(); it != neighborIds.end(); ++it)
  {
    WayPtr neighbor = _map->getWay(*it);
    // add these two buildings to a set
    _ds.joinT(neighbor, w);
  }
}

void BuildingPartMergeOp::_addNeighborsToGroup(const RelationPtr& r)
{
  _addContainedWaysToGroup(*(_elementConverter->convertToGeometry(r)), r);

  const std::vector<RelationData::Entry>& members = r->getMembers();
  int totalProcessed = 0;
  int buildingsAdded = 0;
  for (size_t i = 0; i < members.size(); i++)
  {
    const RelationData::Entry memberEntry = members[i];
    const ElementId memberElementId = memberEntry.getElementId();
    if (memberElementId.getType() == ElementType::Way)
    {
      const WayPtr& member = _map->getWay(memberElementId.getId());
      const std::set<long> neighborIds = _calculateNeighbors(member, r->getTags());
      for (std::set<long>::const_iterator it = neighborIds.begin(); it != neighborIds.end(); ++it)
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

boost::shared_ptr<geos::geom::Geometry> BuildingPartMergeOp::_getWayGeometry(
  const WayPtr& way, const bool checkForBuilding)
{
  boost::shared_ptr<geos::geom::Geometry> g;
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
      boost::shared_ptr<geos::geom::Geometry> g = _getWayGeometry(w, false);
      if (g)
      {
        _addContainedWaysToGroup(*g, w);
        _numAffected++;
      }
    }

    totalProcessed++;
    if (totalProcessed % 1000 == 0)
    {
//      PROGRESS_INFO(
//        "\tProcessed " << StringUtils::formatLargeNumber(totalProcessed) << " / " <<
//        StringUtils::formatLargeNumber(ways.size()) <<
//        " ways for building part merging.");
    }
  }
  LOG_DEBUG("\tProcessed " << StringUtils::formatLargeNumber(totalProcessed) << " ways.");
}

void BuildingPartMergeOp::_processRelations()
{
  int totalProcessed = 0;
  // go through all the relations
  const RelationMap& relations = _map->getRelations();
  LOG_VAR(relations.size());
  int numBuildings = 0;
  for (RelationMap::const_iterator it = relations.begin(); it != relations.end(); ++it)
  {
    const RelationPtr& r = it->second;
    // add the relation to a building group if appropriate
    if (_buildingCrit.isSatisfied(r))
    {
      _addNeighborsToGroup(r);
      _numAffected++;
      numBuildings++;
    }

    totalProcessed++;
    if (totalProcessed % 10 == 0)
    {
//      PROGRESS_INFO(
//        "\tProcessed " << StringUtils::formatLargeNumber(totalProcessed) << " / " <<
//        StringUtils::formatLargeNumber(relations.size()) <<
//        " relations for building part merging.");
    }
  }
  LOG_VAR(numBuildings);
  LOG_DEBUG(
    "Added " << StringUtils::formatLargeNumber(_totalContainedBuildingsAdded) << " / " <<
    StringUtils::formatLargeNumber(_totalContainedWaysProcessed) <<
    " contained ways as buildings.");
  LOG_DEBUG("\tProcessed " << StringUtils::formatLargeNumber(totalProcessed) << " relations.");
}

void BuildingPartMergeOp::_mergeBuildingParts()
{
  // go through each of the grouped buildings
  int totalBuildingGroupsProcessed = 0;
  int numBuildingGroupsMerged = 0;
  int numBuildingPartsMerged = 0;
  const Tgs::DisjointSetMap<ElementPtr>::AllGroups& groups = _ds.getAllGroups();
  for (Tgs::DisjointSetMap<ElementPtr>::AllGroups::const_iterator it = groups.begin();
       it != groups.end(); it++)
  {
    // combine the group of building parts into a relation.
    const std::vector<ElementPtr>& parts = it->second;
    if (parts.size() > 1)
    {
      _combineParts(parts);
      numBuildingGroupsMerged++;
      numBuildingPartsMerged += parts.size();
    }

    totalBuildingGroupsProcessed++;
    if (totalBuildingGroupsProcessed % 100 == 0)
    {
      PROGRESS_INFO(
        "\tMerged " << StringUtils::formatLargeNumber(numBuildingPartsMerged) <<
        " building parts after processing " <<
        StringUtils::formatLargeNumber(numBuildingGroupsMerged) << " / " <<
        StringUtils::formatLargeNumber(totalBuildingGroupsProcessed) << " building groups.");
    }
  }
  LOG_DEBUG("\tMerged " << numBuildingPartsMerged << " building parts.");
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

  if (_threadCount == -2)   // TODO: remove
  {
    _processWays();
    LOG_VAR(StringUtils::formatLargeNumber(_ds.size()));
    _processRelations();
  }
  else
  {
    _processWays(); //TODO: remove
    _processBuildingParts();
  }
  LOG_VAR(StringUtils::formatLargeNumber(_ds.size()));

//  LOG_DEBUG(
//    "\tCleaned " << StringUtils::formatLargeNumber(_numGeometriesCleaned) <<
//    " total building geometries.");
//  LOG_DEBUG("\tGeometry cache hits: " << StringUtils::formatLargeNumber(_numGeometryCacheHits));

  ////
  /// Time to start making changes to the map.
  ////
  _mergeBuildingParts();

  // most other operations don't need this index, so we'll clear it out so it isn't actively
  // maintained.
  _map->getIndex().clearRelationIndex();
  _map.reset();
}

std::set<long> BuildingPartMergeOp::_calculateNeighbors(const WayPtr& w, const Tags& tags)
{
  std::set<long> neighborIds;

  const NodeToWayMap& n2w = *_map->getIndex().getNodeToWayMap();
  long lastId = w->getNodeId(0);
  // go through each of its nodes and look for commonality with other ways
  for (size_t i = 1; i < w->getNodeCount(); i++)
  {
    // find all other ways that use this node (neighbors)
    const std::set<long>& ways = n2w.getWaysByNode(w->getNodeId(i));

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

void BuildingPartMergeOp::_combineParts(const std::vector<ElementPtr>& parts)
{
  combineParts(_map, parts);
}

RelationPtr BuildingPartMergeOp::combineParts(const OsmMapPtr& map,
                                              const std::vector<ElementPtr>& parts)
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
