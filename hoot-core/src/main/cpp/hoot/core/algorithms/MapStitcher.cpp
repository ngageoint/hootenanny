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
 * @copyright Copyright (C) 2021 Maxar (http://www.maxar.com/)
 */

#include "MapStitcher.h"

//  Hoot
#include <hoot/core/algorithms/Distance.h>
#include <hoot/core/algorithms/WayHeading.h>
#include <hoot/core/algorithms/WayJoinerBasic.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/elements/NodeToWayMap.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/ops/RemoveNodeByEid.h>
#include <hoot/core/schema/TagComparator.h>
#include <hoot/core/schema/TagMergerFactory.h>
#include <hoot/core/visitors/CalculateMapBoundsVisitor.h>

//  Standard
#include <cmath>

//  Geos
#include <geos/geom/Envelope.h>

using namespace geos::geom;
using namespace std;

namespace hoot
{

MapStitcher::MapStitcher(const OsmMapPtr& base_map)
  : _base_map(base_map),
    _stitch_buffer_size(ConfigOptions().getMapStitcherThreshold() > 0 ?
                        ConfigOptions().getMapStitcherThreshold() :
                        ConfigOptions::getMapStitcherThresholdDefaultValue()),
    _node_buffer_threshold(ConfigOptions().getDuplicateNodeRemoverDistanceThreshold() > 0 ?
                           ConfigOptions().getDuplicateNodeRemoverDistanceThreshold() :
                           ConfigOptions::getDuplicateNodeRemoverDistanceThresholdDefaultValue()),
    _cph(_node_buffer_threshold)
{
}

void MapStitcher::stitchMaps(const OsmMapPtr& base_map, const OsmMapPtr& source_map)
{
  MapStitcher stitcher(base_map);
  stitcher.stitchMap(source_map);
}

void MapStitcher::stitchMap(const OsmMapPtr& map)
{
  //  Get the map envelopes
  OGREnvelope projectionEnvelope = CalculateMapBoundsVisitor::getBounds(_base_map);
  projectionEnvelope.Merge(CalculateMapBoundsVisitor::getBounds(map));
  //  Reproject the maps
  MapProjector::projectToPlanar(_base_map, projectionEnvelope);
  MapProjector::projectToPlanar(map, projectionEnvelope);

  //  Get the map envelopes
  Envelope baseEnvelope = CalculateMapBoundsVisitor::getGeosBounds(_base_map);
  Envelope mapEnvelope = CalculateMapBoundsVisitor::getGeosBounds(map);
  //  Expand the envelopes
  baseEnvelope.expandBy(_stitch_buffer_size, _stitch_buffer_size);
  mapEnvelope.expandBy(_stitch_buffer_size, _stitch_buffer_size);

  //  Make sure that the bounds touch and calculate the stitching area envelope
  Envelope stitchEnvelope;
  if (baseEnvelope.intersection(mapEnvelope, stitchEnvelope))
  {
    const NodeMap& baseNodes = _base_map->getNodes();
    const NodeMap& mapNodes = map->getNodes();
    OsmMapPtr buffer = std::make_shared<OsmMap>();

    //  Iterate all of the base nodes and record their ID if they fit in the bounds of the stitch envelope
    for (const auto &it : baseNodes)
    {
      if (stitchEnvelope.contains(it.second->getX(), it.second->getY()))
      {
        //  Track the base nodes in the stitch zone
        _base_node_ids.insert(it.first);
        //  Add stitch nodes into a close point hash
        _cph.addPoint(it.second->getX(), it.second->getY(), it.first);
      }
    }
    //  Iterate all of the secondary map nodes and record their ID if they fit in the bounds of the stitch envelope
    for (const auto& it : mapNodes)
    {
      if (stitchEnvelope.contains(it.second->getX(), it.second->getY()))
        _map_node_ids.insert(it.first);
    }
    //  Iterate all of the ways in the map, copying them to the base and stitching them if needed
    const WayMap& mapWays = map->getWays();
    for (const auto& it : mapWays)
    {
      WayPtr clonedWay = std::dynamic_pointer_cast<Way>(it.second->clone());
      //  Closed areas require a different stitching strategy
      if (!clonedWay->isClosedArea())
      {
        bool firstNode = _map_node_ids.find(clonedWay->getFirstNodeId()) != _map_node_ids.end();
        bool lastNode = _map_node_ids.find(clonedWay->getLastNodeId()) != _map_node_ids.end();
        //  Ways that don't have endpoints in the buffer don't need to be stitched, just copied to the base
        //  Copy the way over to the base map or stitch it with another way
        if (!firstNode && !lastNode)
          _copyWayToMap(map, _base_map, clonedWay);
        else
          _stitchWay(map, _base_map, clonedWay);
      }
      else  //  Try to stitch the poly if it is on the border
        _stitchPoly(map, _base_map, clonedWay);
    }
    //  Copy any POI nodes that haven't been copied over yet
    std::shared_ptr<NodeToWayMap> nodeToWayMap = map->getIndex().getNodeToWayMap();
    for (const auto& it : mapNodes)
    {
      if (nodeToWayMap->find(it.first) == nodeToWayMap->end())
        _copyNodeToMap(_base_map, it.second->cloneSp());
    }
    //  Copy any relations over now that the ways and nodes have been copied
    const RelationMap& mapRelations = map->getRelations();
    for (const auto& it : mapRelations)
    {
      if (_updated_relation_ids.find(it.first) == _updated_relation_ids.end())
        _copyRelationToMap(map, _base_map, std::dynamic_pointer_cast<Relation>(it.second->clone()));
    }
    //  Join any ways that crossed over the boundary and crossed back
    _joinWayPairs();
    //  Reproject maps back to WGS84
    MapProjector::projectToWgs84(_base_map);
    MapProjector::projectToWgs84(map);
  }
  else
  {
    LOG_ERROR("Maps are not adjacent, cannot stitch maps at boundary edges.");
  }
}

Radians MapStitcher::_getWayEndpointHeading(const OsmMapPtr& map,
                                            const WayPtr& way,
                                            long node_id,
                                            bool towards_node_id) const
{
  NodePtr p1;
  NodePtr p2;
  //  Get the two nodes (beginning or end) to calculate the heading
  if (way->getFirstNodeId() == node_id)
  {
    //  Calculate the heading towards node_id
    long second = way->getNodeId(1);
    p1 = map->getNode(second);
    p2 = map->getNode(node_id);
  }
  else if (way->getLastNodeId() == node_id)
  {
    //  Calculate the heading towards node_id
    long first = way->getNodeId(static_cast<int>(way->getNodeCount() - 2));
    p1 = map->getNode(first);
    p2 = map->getNode(node_id);
  }
  else
    return std::numeric_limits<Radians>::min();
  //  Reverse the direction of the heading calculation if required
  if (!towards_node_id)
    p1.swap(p2);
  //  Calculate the heading
  return WayHeading::calculateHeading(geos::geom::Coordinate(p1->getX(), p1->getY()), geos::geom::Coordinate(p2->getX(), p2->getY()));
}

long MapStitcher::_copyNodeToMap(const OsmMapPtr& dest_map, const NodePtr& node)
{
  long nodeId = node->getId();
  long id = nodeId;
  const NodeMap& baseNodes = dest_map->getNodes();
  //  Check if this node has already been copied over
  if (_updated_node_ids.find(id) != _updated_node_ids.end())
    return _updated_node_ids[id];
  //  Don't copy the node over to the base map if it has already been copied
  if (baseNodes.find(id) != baseNodes.end())
  {
    //  Check the distance between this node and the one with the same ID
    if (id > 0 && Distance::euclidean(*node.get(), *dest_map->getNode(id).get()) < _node_buffer_threshold)
      return id;
    //  Get a new ID for this node and update the way
    id = dest_map->createNextNodeId();
    //  Update the node ID
    node->setId(id);
  }
  //  Record the change to the node ID
  _updated_node_ids[nodeId] = id;
  //  Copy the node straight across
  dest_map->addNode(node);
  //  Return the node ID (updated or otherwise)
  return id;
}


void MapStitcher::_copyWayToMap(const OsmMapPtr& source_map,
                                const OsmMapPtr& dest_map,
                                const WayPtr& way)
{
  const NodeMap& mapNodes = source_map->getNodes();
  const WayMap& baseWays = dest_map->getWays();
  long way_id = way->getId();
  //  Check for a way with the same ID in the base map
  if (baseWays.find(way_id) == baseWays.end())
  {
    //  Not found so it can be copied straight across, next check for nodes
    const vector<long>& waynodes = way->getNodeIds();
    vector<long> waynodeUpdates;
    for (auto nodeId : waynodes)
    {
      long id = nodeId;
      //  If the nodes are the "same" in both maps, don't copy it
      NodePtr n = mapNodes.find(nodeId)->second;
      NodePtr baseNode = dest_map->getNode(nodeId);
      if (n && baseNode && baseNode->coordsMatch(*n)) //  Set the node ID for the way
        waynodeUpdates.push_back(id);
      else if (_updated_node_ids.find(nodeId) == _updated_node_ids.end())
      {
        NodePtr clonedNode = n->cloneSp();
        id = _copyNodeToMap(dest_map, clonedNode);
        //  Set the node ID for the way
        waynodeUpdates.push_back(id);
      }
    }
    _updated_way_ids[way_id] = way_id;
    way->setNodes(waynodeUpdates);
    //  Finally add the way to the base map
    dest_map->addWay(way);
  }
  else
  {
    //  Way with the same ID is found in the destination map
    //  Negative IDs can be replaced when copied when they are duplicated
    if (way_id < 0)
    {
      //  Get a new way ID from the base map
      long id = dest_map->createNextWayId();
      way->setId(id);
      //  Work on the nodes now
      const vector<long>& waynodes = way->getNodeIds();
      vector<long> waynodeUpdates;
      for (auto nodeId : waynodes)
      {
        long new_id = nodeId;
        //  If the nodes are the "same" in both maps, don't copy it
        NodePtr n = mapNodes.find(nodeId)->second;
        NodePtr baseNode = dest_map->getNode(nodeId);
        if (n && baseNode && baseNode->coordsMatch(*n)) //  Set the node ID for the way
          waynodeUpdates.push_back(new_id);
        else if (_updated_node_ids.find(nodeId) == _updated_node_ids.end())
        {
          NodePtr clonedNode = n->cloneSp();
          new_id = _copyNodeToMap(dest_map, clonedNode);
          //  Set the node ID for the way
          waynodeUpdates.push_back(new_id);
        }
      }
      _updated_way_ids[way_id] = id;
      way->setNodes(waynodeUpdates);
      //  Finally add the way to the base map
      dest_map->addWay(way);
    }
    else  //  Positive IDs should be stitched if possible
    {
      //  Get the way in the destination map to check if it can be stitched
      WayPtr dest_way = dest_map->getWay(way->getId());
      _mergeWays(source_map, way, dest_map, dest_way);
    }
  }
}

void MapStitcher::_copyRelationToMap(const OsmMapPtr& source_map, const OsmMapPtr& dest_map, const RelationPtr& relation)
{
  //  Make sure that this relation hasn't been copied before
  if (_updated_relation_ids.find(relation->getId()) == _updated_relation_ids.end())
  {
    //  Iterate all of the members, copy the relations and update the other IDs
    const std::vector<RelationData::Entry>& members = relation->getMembers();
    std::vector<RelationData::Entry> new_members;
    for (const auto &it : members)
    {
      ElementId id = it.getElementId();
      QString role = it.getRole();
      switch (id.getType().getEnum())
      {
      case ElementType::Node:
        //  Update the Node ID if necessary
        if (_updated_node_ids.find(id.getId()) == _updated_node_ids.end())
          new_members.emplace_back(role, id);
        else
          new_members.emplace_back(role, ElementId(ElementType::Node, _updated_node_ids[id.getId()]));
        break;
      case ElementType::Way:
        //  Update the Way ID if necessary
        if (_updated_way_ids.find(id.getId()) == _updated_way_ids.end())
          new_members.emplace_back(role, id);
        else
          new_members.emplace_back(role, ElementId(ElementType::Way, _updated_way_ids[id.getId()]));
        break;
      case ElementType::Relation:
        //  Copy any sub-relations over to the destination if they haven't been copied already
        if (_updated_relation_ids.find(id.getId()) == _updated_relation_ids.end())
        {
          RelationPtr r = std::dynamic_pointer_cast<Relation>(source_map->getRelation(id.getId())->clone());
          _copyRelationToMap(source_map, dest_map, r);
        }
        new_members.emplace_back(role, ElementId(ElementType::Relation, _updated_relation_ids[id.getId()]));
        break;
      default:
        LOG_ERROR("Unknown element type (" << id.getType().getEnum() << ")");
        return;
      }
    }
    relation->setMembers(new_members);
    //  Update the ID of the relation if necessary
    long id = relation->getId();
    long new_id = id;
    const RelationMap& relations = dest_map->getRelations();
    //  Found a positive, duplicate relation ID, try to merge them
    if (relation->getId() > 0 && relations.find(id) != relations.end())
    {
      //  Merge them and return, if the merge was successful, quit
      if (_mergeRelations(relation, relations.find(id)->second))
        return;
      //  The relations couldn't be merged so copy it over with a new negative ID
      new_id = dest_map->createNextRelationId();
      relation->setId(new_id);
    }
    else if (relation->getId() < 0)
    {
      //  Relation ID is negative, can be updated, get a new ID from the destination map
      new_id = dest_map->createNextRelationId();
      relation->setId(new_id);
    }
    //  Record the updated relation ID
    _updated_relation_ids[id] = new_id;
    //  Copy the new relation into the destination map
    dest_map->addRelation(relation);
  }
}

void MapStitcher::_stitchWay(const OsmMapPtr& source_map,
                             const OsmMapPtr& dest_map,
                             const WayPtr& way)
{
  bool copy_way = false;
  bool already_stitched = false;
  //  Try stitching the first way endpoint
  if (_map_node_ids.find(way->getFirstNodeId()) != _map_node_ids.end())
  {
    WayPtr dest_way = _findStitchPointWay(source_map, dest_map, way, true);
    if (dest_way != nullptr)
    {
      _mergeWays(source_map, way, dest_map, dest_way);
      already_stitched = true;
    }
    else
      copy_way = true;
  }
  //  Try stitching the last way endpoint
  if (_map_node_ids.find(way->getLastNodeId()) != _map_node_ids.end())
  {
    WayPtr dest_way = _findStitchPointWay(source_map, dest_map, way, false);
    if (dest_way != nullptr)
    {
      //  If the other end is already stitched, join the dest way on to the updated, stitched way
      if (already_stitched)
        _way_join_pairs.push_back(std::pair<long, long>(_updated_way_ids[way->getId()], dest_way->getId()));
      else
        _mergeWays(source_map, way, dest_map, dest_way);
    }
    else
      copy_way = true;
  }
  //  Copy the way since it wasn't stitched
  if (copy_way)
    _copyWayToMap(source_map, dest_map, way);
}

void MapStitcher::_stitchPoly(const OsmMapPtr& source_map,
                              const OsmMapPtr& dest_map,
                              const WayPtr& poly)
{
  LOG_WARN("Copying polygon way to stitched map.")
  _copyWayToMap(source_map, dest_map, poly);
}

WayPtr MapStitcher::_findStitchPointWay(const OsmMapPtr& source_map,
                                        const OsmMapPtr& dest_map,
                                        const WayPtr& way,
                                        bool first)
{
  //  Get the stitch point in the source to find in the destination
  NodePtr stitchPoint = source_map->getNode(first ? way->getFirstNodeId() : way->getLastNodeId());
  return _findStitchPointWay(source_map, dest_map, way, stitchPoint);
}

WayPtr MapStitcher::_findStitchPointWay(const OsmMapPtr& source_map,
                                        const OsmMapPtr& dest_map,
                                        const WayPtr& way,
                                        const NodePtr& node)
{
  WayPtr result;
  //  Use a fake node ID of 0 in the close point hash so that it won't affect any other node ID in the base map
  const int fakeNodeId = 0;
  //  Update the base close point hash with the stitch point node
  _cph.updatePoint(node->getX(), node->getY(), fakeNodeId);
  //  Get all of the points near by from the close point hash
  vector<long> matches = _cph.getMatchesFor(fakeNodeId);
  //  There should only be one match (plus the ID), if any
  if (matches.size() > 2)
  {
    LOG_WARN("Multiple stitch points found, unable to stitch way (" << way->getId() << ") at node (" << node->getId() << ")");
    return result;
  }
  else if (matches.size() == 2) //  Found one point in the base to merge with for stitching
  {
    //  Get the non-fake ID
    long baseNodeId = matches[0] != fakeNodeId ? matches[0] : matches[1];

    LOG_DEBUG("Stitching ways at base node (" << baseNodeId << ") and secondary node (" << node->getId() << ")");

    NodePtr baseNode = dest_map->getNode(baseNodeId);
    //  Find all of the ways where this node is a member
    std::shared_ptr<NodeToWayMap> nodeToWayMap = dest_map->getIndex().getNodeToWayMap();
    const set<long>& possibleWays = nodeToWayMap->getWaysByNode(baseNodeId);
    //  Find the best fit for the way to stitch
    double maxScore = 0;
    Radians minDelta = M_PI * 2;
    for (auto wayId : possibleWays)
    {
      double score = 0;
      //  Score each way
      WayPtr w = dest_map->getWay(wayId);
      //  The ways have the same, positive way ID is a match
      if (w->getId() == way->getId() && w->getId() > 0)
      {
        result = w;
        break;
      }
      //  Check the heading of both way endpoints
      Radians heading1 = _getWayEndpointHeading(dest_map, w, baseNodeId, true);
      Radians heading2 = _getWayEndpointHeading(source_map, way, node->getId(), false);
      Radians delta = WayHeading::deltaMagnitude(heading1, heading2);
      if (delta < M_PI_4 / 4.0)
        score += 1.0;
      else if (delta < M_PI_4 / 2.0)
        score += 0.9;
      else if (delta < M_PI_4)
        score += 0.75;
      else if (delta < M_PI_2)
        score += 0.5;
      //  Check the tag values
      const Tags& baseTags = w->getTags();
      const Tags& secondaryTags = way->getTags();
      double scoreValue = 0.0;
      double weightValue = 0.0;
      TagComparator::getInstance().compareNames(baseTags, secondaryTags, scoreValue, weightValue);
      score += scoreValue;
      score += TagComparator::getInstance().compareTags(baseTags,secondaryTags);
      //  Save the best match score
      if ((score == maxScore && delta < minDelta) || score > maxScore)
      {
        maxScore = score;
        minDelta = delta;
        result = w;
      }
    }
  }
  return result;
}

void MapStitcher::_mergeWays(const OsmMapPtr& source_map,
                             const WayPtr& source_way,
                             const OsmMapPtr& dest_map,
                             const WayPtr& dest_way)
{
  vector<long> new_nodes;
  const vector<long>& source_nodes = source_way->getNodeIds();
  const vector<long>& dest_nodes = dest_way->getNodeIds();
  bool append = true;
  bool reverse = false;
  bool remove_overlap = false;
  int source_way_endpoint = -1;
  int dest_way_endpoint = -1;
  //  Find the endpoints that match
  _findWayEndpoints(source_map, source_way, dest_map, dest_way, source_way_endpoint, dest_way_endpoint);
  //  Prepend or append the source ways
  if (source_way_endpoint == -1 || dest_way_endpoint == -1)
  {
    LOG_ERROR("Merge ways couldn't find way matching way endpoints.");
    return;
  }
  else if (source_way_endpoint == 0 && dest_way_endpoint == 0)
  {
    append = false;
    reverse = true;
    remove_overlap = true;
  }
  else if (source_way_endpoint != 0 && dest_way_endpoint == 0)
  {
    append = false;
    reverse = false;
    remove_overlap = true;
  }
  else if (source_way_endpoint == 0 && dest_way_endpoint != 0)
  {
    append = true;
    reverse = false;
    remove_overlap = true;
  }
  else if (source_way_endpoint != 0 && dest_way_endpoint != 0)
  {
    append = true;
    reverse = true;
    remove_overlap = true;
  }
  //  Preallocate the size of the vector
  new_nodes.resize(source_nodes.size() + dest_nodes.size() - (remove_overlap ? 1 : 0));
  size_t position = 0;
  //  Source node index for renumbering
  size_t index = 0;
  //  Prepend the source nodes
  if (!append)
  {
    //  Copy the nodes over
    std::copy(source_nodes.begin(), source_nodes.end(), new_nodes.begin());
    position += source_nodes.size();
    //  Reverse the order if necessary
    if (reverse)
    {
      std::reverse(new_nodes.begin(), new_nodes.begin() + source_nodes.size());
      index = source_nodes.size() - 1 - (remove_overlap ? 1 : 0);
    }
    //  Remove the last node ID if they overlap
    if (remove_overlap)
      position--;
  }
  //  Copy all of the destination nodes into the vector
  std::copy(dest_nodes.begin(), dest_nodes.end(), new_nodes.begin() + position);
  //  Append the source nodes
  if (append)
  {
    //  Forward or backward, removing the first or last if there is overlap
    if (!reverse)
    {
      //  Append source nodes on to the end of the destination nodes
      std::copy(source_nodes.begin() + (remove_overlap ? 1 : 0), source_nodes.end(), new_nodes.begin() + dest_nodes.size());
      index = dest_nodes.size();
    }
    else
    {
      //  Append source nodes in reverse order on to the end of the destination nodes
      std::reverse_copy(source_nodes.begin(), source_nodes.begin() + (remove_overlap ? source_nodes.size() - 1 : source_nodes.size()), new_nodes.begin() + dest_nodes.size());
      index = new_nodes.size() - 1;
    }
  }
  //  Copy the nodes from the source to the destination, iterate with iterators here
  for (auto it = source_nodes.begin(); it != source_nodes.end(); ++it)
  {
    long node_id = *it;
    //  Don't copy over a node that overlaps
    if (remove_overlap)
    {
      //  Prepended, non-reversed, skip the last node
      //  Prepended, reversed, skip the first node
      if ((!append && !reverse && it + 1 == source_nodes.end()) || (!append && reverse && it == source_nodes.begin()))
        continue;
      //  Appended, non-reversed, skip the first node
      //  Appended, reversed, skip the last node
      if ((append && !reverse && it == source_nodes.begin()) || (append && reverse && it + 1 == source_nodes.end()))
        continue;
    }
    //  Copy the node from the source map to the destination map
    _copyNodeToMap(dest_map, source_map->getNode(node_id)->cloneSp());
    //  Update the way nodes with the new node ID
    if (_updated_node_ids.find(node_id) != _updated_node_ids.end())
      new_nodes[index] = _updated_node_ids[node_id];
    //  Reverse the index if necessary
    reverse ? index-- : index++;
  }
  //  Set the nodes
  dest_way->setNodes(new_nodes);
  //  Merge the way tags
  const Tags& dest_tags = dest_way->getTags();
  const Tags& source_tags = source_way->getTags();
  dest_way->setTags(TagMergerFactory::mergeTags(dest_tags, source_tags, ElementType::Way));
  //  Update the way ID
  _updated_way_ids[source_way->getId()] = dest_way->getId();
}

bool MapStitcher::_mergeRelations(const RelationPtr& source_relation, const RelationPtr& dest_relation) const
{
  //  Both relations should have positive, matching IDs so that they can be merged
  if (source_relation && dest_relation &&
      source_relation->getId() == dest_relation->getId() &&
      source_relation->getId() > 0)
  {
    //  Iterate all of the source members and add them if they aren't duplicates
    const vector<RelationData::Entry>& source_members = source_relation->getMembers();
    for (const auto& member : source_members)
    {
      if (!dest_relation->contains(member.getElementId()))
        dest_relation->addElement(member.getRole(), member.getElementId());
    }
    return true;
  }
  return false;
}

void MapStitcher::_joinWayPairs() const
{
  WayJoinerBasic joiner;
  for (auto it : _way_join_pairs)
  {
    WayPtr way1 = _base_map->getWay(it.first);
    WayPtr way2 = _base_map->getWay(it.second);
    int way1_join_point = -1;
    int way2_join_point = -1;
    //  Find the endpoints to join the ways at
    _findWayEndpoints(_base_map, way1, _base_map, way2, way1_join_point, way2_join_point);
    //  Setup the two ways to be joined (must have joining nodes)
    long node_id = way2->getNodeId(way2_join_point);
    vector<long> way1_nodes = way1->getNodeIds();
    long node_to_delete_id = way1_nodes[way1_join_point];
    way1_nodes[way1_join_point] = node_id;
    way1->setNodes(way1_nodes);
    //  Join the two ways
    joiner.joinWay(_base_map, way1, way2);
    //  Delete the old node join point
    RemoveNodeByEid::removeNode(_base_map, node_to_delete_id, true);
  }
}

void MapStitcher::_findWayEndpoints(const OsmMapPtr& source_map, const WayPtr& source_way,
                                    const OsmMapPtr& dest_map, const WayPtr& dest_way,
                                    int& source_way_node_pos, int& dest_way_node_pos) const
{
  NodePtr source_first_node = source_map->getNode(source_way->getFirstNodeId());
  NodePtr source_last_node = source_map->getNode(source_way->getLastNodeId());
  NodePtr dest_first_node = dest_map->getNode(dest_way->getFirstNodeId());
  NodePtr dest_last_node = dest_map->getNode(dest_way->getLastNodeId());
  //  Check which of the endpoints are matches
  if (source_way->getFirstNodeId() == dest_way->getFirstNodeId() ||
      (source_first_node != nullptr && dest_first_node != nullptr &&
       Distance::euclidean(*source_first_node.get(), *dest_first_node.get()) < _node_buffer_threshold))
  {
    //  First nodes in both ways are matches
    source_way_node_pos = 0;
    dest_way_node_pos = 0;
  }
  else if (source_way->getLastNodeId() == dest_way->getFirstNodeId() ||
           (source_last_node != nullptr && dest_first_node != nullptr &&
            Distance::euclidean(*source_last_node.get(), *dest_first_node.get()) < _node_buffer_threshold))
  {
    //  Last source node matches first destination node
    source_way_node_pos = static_cast<int>(source_way->getNodeCount() - 1);
    dest_way_node_pos = 0;
  }
  else if (source_way->getFirstNodeId() == dest_way->getLastNodeId() ||
           (source_first_node != nullptr && dest_last_node != nullptr &&
            Distance::euclidean(*source_first_node.get(), *dest_last_node.get()) < _node_buffer_threshold))
  {
    //  First source node matches last destination node
    source_way_node_pos = 0;
    dest_way_node_pos = static_cast<int>(dest_way->getNodeCount() - 1);
  }
  else if (source_way->getLastNodeId() == dest_way->getLastNodeId() ||
           (source_last_node != nullptr && dest_last_node != nullptr &&
            Distance::euclidean(*source_last_node.get(), *dest_last_node.get()) < _node_buffer_threshold))
  {
    //  Last nodes in both ways are matches
    source_way_node_pos = static_cast<int>(source_way->getNodeCount() - 1);
    dest_way_node_pos = static_cast<int>(dest_way->getNodeCount() - 1);
  }
}


}
