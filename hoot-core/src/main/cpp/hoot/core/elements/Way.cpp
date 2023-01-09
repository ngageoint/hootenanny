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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */

#include "Way.h"

// Hoot
#include <hoot/core/elements/Node.h>
#include <hoot/core/geometry/ElementToGeometryConverter.h>
#include <hoot/core/geometry/GeometryUtils.h>
#include <hoot/core/util/CollectionUtils.h>
#include <hoot/core/visitors/ConstElementVisitor.h>

// Geos
#include <geos/geom/CoordinateSequenceFactory.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/LinearRing.h>
#include <geos/geom/LineString.h>
#include <geos/geom/Polygon.h>

// Tgs
#include <tgs/StreamUtils.h>

using namespace geos::geom;
using namespace std;

//  Enable way debugging, checking for duplicated node IDs
//#define WAY_DEBUGGING

namespace hoot
{

Way::Way(Status s, long id, Meters circularError, long changeset, long version,
         OsmTimestamp timestamp, QString user, long uid, bool visible, long pid)
  : Element(s)
{
  _wayData = std::make_shared<WayData>(id, changeset, version, timestamp, user, uid, visible, pid);
  _wayData->setCircularError(circularError);
}

Way::Way(const Way& from)
  : Element(from.getStatus()),
    _wayData(std::make_shared<WayData>(*from._wayData.get()))
{
}

void Way::addNode(long id)
{
  _preGeometryChange();
  _makeWritable();
  _wayData->addNode(id);
  _postGeometryChange();

#ifdef WAY_DEBUGGING
  // for debugging only; SLOW
  if (_nodeIdsAreDuplicated(getNodeIds()))
  {
    LOG_VARE(getNodeIds());
    throw IllegalArgumentException("Duplicate way node IDs: addNode");
  }
#endif
}

void Way::insertNode(long index, long id)
{
  _preGeometryChange();
  _makeWritable();
  _wayData->insertNode(index, id);
  _postGeometryChange();

#ifdef WAY_DEBUGGING
  // for debugging only; SLOW
  if (_nodeIdsAreDuplicated(getNodeIds()))
  {
    LOG_VARE(getNodeIds());
    throw IllegalArgumentException("Duplicate way node IDs: insertNode");
  }
#endif
}

void Way::addNodes(const vector<long>& ids)
{
  addNodes(ids.begin(), ids.end());

#ifdef WAY_DEBUGGING
  // for debugging only; SLOW
  if (_nodeIdsAreDuplicated(getNodeIds()))
  {
    LOG_VARE(getNodeIds());
    throw IllegalArgumentException("Duplicate way node IDs: addNodes");
  }
#endif
}

bool Way::_nodeIdsAreDuplicated(const vector<long>& ids) const
{
  if (ids.size() == 1)
    return false;

  LOG_VART(ids);

  QList<long> idsCopy;
  for (size_t i = 0; i < ids.size(); i++)
  {
    const long id = ids[i];
    LOG_VART(id);
    LOG_VART(i);
    LOG_VART(idsCopy.lastIndexOf(id));
    if (!idsCopy.contains(id))
      idsCopy.append(id);
    else if (i == ids.size() - 1 && idsCopy.lastIndexOf(id) != 0)
      return true;
    else if (i != ids.size() - 1)
      return true;
  }
  return false;
}

void Way::clear()
{
  _preGeometryChange();
  _makeWritable();
  _wayData->clear();
  _postGeometryChange();
}

bool Way::containsNodeId(long nid) const
{
  const std::vector<long>& nids = getNodeIds();
  for (auto node_id : nids)
  {
    if (node_id == nid)
      return true;
  }
  return false;
}

void Way::visitRo(const ElementProvider& map, ConstElementVisitor& filter, const bool recursive) const
{
  filter.visit(map.getWay(getId()));

  if (recursive)
  {
    const std::vector<long>& nids = getNodeIds();
    for (auto node_id : nids)
    {
      if (map.containsNode(node_id))
        filter.visit(map.getNode(node_id));
    }
  }
}

void Way::visitRw(ElementProvider& map, ConstElementVisitor& filter, const bool recursive)
{
  visitRo(map, filter, recursive);
}

const Envelope& Way::getApproximateEnvelope(const std::shared_ptr<const ElementProvider>& ep) const
{
  bool goodNodes = true;
  if (ep.get())
  {
    const std::vector<long>& ids = _wayData->getNodeIds();
    for (auto node_id : ids)
    {
      if (ep->containsElement(ElementId::node(node_id)) == false)
        goodNodes = false;
      else
      {
        std::shared_ptr<const Element> e = ep->getElement(ElementId::node(node_id));
        ConstNodePtr n = std::dynamic_pointer_cast<const Node>(e);
        assert(n.get());
        _cachedEnvelope.expandToInclude(n->getX(), n->getY());
      }
    }
  }
  else
    goodNodes = false;

  if (goodNodes)
    return getEnvelopeInternal(ep);
  else
    return _cachedEnvelope;
}

const Envelope& Way::getEnvelopeInternal(const std::shared_ptr<const ElementProvider>& ep) const
{
  _cachedEnvelope.init();

  const std::vector<long>& ids = _wayData->getNodeIds();
  for (auto node_id : ids)
  {
    std::shared_ptr<const Element> e = ep->getElement(ElementId::node(node_id));
    ConstNodePtr n = std::dynamic_pointer_cast<const Node>(e);
    if (n)
      _cachedEnvelope.expandToInclude(n->getX(), n->getY());
  }
  return _cachedEnvelope;
}

void Way::setNodes(const vector<long>& newNodes)
{
  _preGeometryChange();
  _makeWritable();

  _wayData->getNodeIds() = newNodes;

  _postGeometryChange();

#ifdef WAY_DEBUGGING
  // for debugging only; SLOW
  if (_nodeIdsAreDuplicated(getNodeIds()))
  {
    LOG_VARE(getNodeIds());
    throw IllegalArgumentException("Duplicate way node IDs: setNodes");
  }
#endif
}

int Way::getNodeIndex(long nodeId) const
{
  const std::vector<long>& ids = _wayData->getNodeIds();

  for (size_t i = 0; i < ids.size(); i++)
  {
    if (ids[i] == nodeId)
      return static_cast<int>(i);
  }
  assert(false);
  return -1;
}

bool Way::hasNode(long nodeId) const
{
  const std::vector<long>& ids = _wayData->getNodeIds();
  for (auto node_id : ids)
  {
    if (node_id == nodeId)
      return true;
  }

  return false;
}

bool Way::isSimpleLoop() const
{
  if (getNodeCount() < 2)
    return false;
  return (getFirstNodeId() == getLastNodeId());
}

bool Way::isValidPolygon() const
{
  size_t nc = getNodeCount();
  // the way must have either 0 or 4 points to be considered a valid polygon by GEOS
  // or 3 points and first != last
  return (nc == 0 || nc >= 4 || (nc == 3 && getNodeId(0) != getLastNodeId()));
}

void Way::_makeWritable()
{
  // Make sure we're the only ones referencing the way data.
  if (_wayData.use_count() > 1)
    _wayData = std::make_shared<WayData>(*_wayData);
}

void Way::removeNode(long id) const
{
  LOG_TRACE("Removing node: " << id << " in way: " << getId() << "...");

  std::vector<long>& nodes = _wayData->getNodeIds();
  size_t newCount = 0;
  // copy the array in place and remove the unwanted nodes.
  for (auto node_id : nodes)
  {
    if (node_id != id)
    {
      nodes[newCount] = node_id;
      newCount++;
    }
  }
  nodes.resize(newCount);
}

bool Way::isEndNode(const long nodeId) const
{
  return nodeId == getFirstNodeId() || nodeId == getLastNodeId();
}

void Way::replaceNode(long oldId, long newId)
{
  // Note: Noticed that duplicate way nodes were being introduced here when called by
  // MergeNearbyNodes. Initially made some attempts in here to clean that up, but it introduced
  // unwanted logic and runtime complexity. Ultimately since the problem was only seen with
  // MergeNearbyNodes, it was updated to remove the duplicated way nodes at the end of its run and
  // this logic was left alone. To validate this behavior, the argument can be made that this method
  // allows you to replace whatever is desired, but its the caller's responsbility to make sure the
  // result of the replacement is valid. If we ever decide we want to make this more foolproof for
  // the caller, then a different approach should be taken similar to the changes attempted before.

  if (oldId == newId)
    return;

  LOG_TRACE("Replacing node: " << oldId << " with: " << newId << " in way: " << getId() << "...");

  const vector<long>& ids = getNodeIds();

  vector<size_t> indices;
  for (size_t i = 0; i < ids.size(); i++)
  {
    if (ids[i] == oldId)
      indices.emplace_back(i);
  }

  if (!indices.empty())
  {
    LOG_TRACE("IDs before replacement: " << getNodeIds());

    // for debugging only; see _nodeIdsAreDuplicated call below
    //const vector<long> oldIdsCopy = ids;

    _preGeometryChange();
    _makeWritable();

    vector<long>& newIds = _wayData->getNodeIds();
    LOG_TRACE("Replacement IDs: " << newIds);
    for (auto index : indices)
      newIds[index] = newId;

    _postGeometryChange();

    LOG_TRACE("IDs after replacement: " << getNodeIds());

#ifdef WAY_DEBUGGING
    // for debugging only; SLOW
    if (_nodeIdsAreDuplicated(getNodeIds()))
    {
      LOG_ERROR(
        "Attempting to replace node: " << oldId << " with: " << newId << " in way: " << getId() <<
        "...");
      LOG_ERROR("Original IDs: " << oldIdsCopy);
      LOG_ERROR("New IDs: " << getNodeIds());
      throw IllegalArgumentException("Duplicate way node IDs: replaceNode");
    }
#endif
  }
}

void Way::reverseOrder()
{
  _preGeometryChange();
  _makeWritable();

  std::vector<long> oldOrder = getNodeIds();
  std::vector<long>& nodes = _wayData->getNodeIds();
  for (size_t i = 0; i < oldOrder.size(); i++)
    nodes[i] = oldOrder[oldOrder.size() - i - 1];

  _postGeometryChange();
}

QString Way::toString() const
{
  stringstream ss(stringstream::out);
  ss << "way(" << getId() << ")" << endl;
  ss << "nodes: ";
  Tgs::operator << (ss, getNodeIds());
  ss << endl;
  ss << "tags: " << getTags().toString().toStdString();
  ss << "cached envelope: " << GeometryUtils::toMinMaxString(_cachedEnvelope).toStdString() << endl;
  ss << "status: " << getStatusString().toStdString() << endl;
  ss << "version: " << getVersion() << endl;
  ss << "visible: " << getVisible() << endl;
  if (hasCircularError())
    ss << "circular error: " << getCircularError() << endl;
  if (hasPid())
    ss << "parent id: (" << getPid() << ")" << endl;
  return QString::fromStdString(ss.str());
}

bool Way::isClosedArea() const
{
  return getNodeCount() > 3 && getFirstNodeId() == getLastNodeId();
}

long Way::getPid(const ConstWayPtr& p, const ConstWayPtr& c)
{
  if (!p && !c)
    return WayData::PID_EMPTY;
  else
    return Way::getPid(p->getPid(), c->getPid());
}

long Way::getPid(long p, long c)
{
  if (p != WayData::PID_EMPTY)
    return p;
  else if (c != WayData::PID_EMPTY)
    return c;
  else
    return WayData::PID_EMPTY;
}

bool Way::hasSameNodes(const Way& other) const
{
  return getNodeIds() == other.getNodeIds();
}

QSet<long> Way::sharedNodeIds(const Way& other) const
{
  QSet<long> nodes = QVector<long>::fromStdVector(getNodeIds()).toList().toSet();
  QSet<long> otherNodes = QVector<long>::fromStdVector(other.getNodeIds()).toList().toSet();
  return nodes.intersect(otherNodes);
}

bool Way::hasSharedNode(const Way& other) const
{
  return !sharedNodeIds(other).empty();
}

bool Way::hasSharedEndNode(const Way& other) const
{
  const std::vector<long>& nodeIds1 = getNodeIds();
  const std::vector<long>& nodeIds2 = other.getNodeIds();
  const long firstNodeId = nodeIds1.at(0);
  const long lastNodeId = nodeIds1.at(nodeIds1.size() - 1);
  const long otherFirstNodeId = nodeIds2.at(0);
  const long otherLastNodeId = nodeIds2.at(nodeIds2.size() - 1);
  return firstNodeId == otherFirstNodeId || firstNodeId == otherLastNodeId ||
         lastNodeId == otherFirstNodeId || lastNodeId == otherLastNodeId;
}

}
