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

#include "OsmMap.h"

// GEOS
#include <geos/geom/LineString.h>

// Hoot
#include <hoot/core/algorithms/rubber-sheet/RubberSheet.h>
#include <hoot/core/conflate/IdSwap.h>
#include <hoot/core/elements/ConstOsmMapConsumer.h>
#include <hoot/core/elements/ElementComparer.h>
#include <hoot/core/elements/ElementId.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/elements/Node.h>
#include <hoot/core/elements/NodeToWayMap.h>
#include <hoot/core/elements/OsmMapListener.h>
#include <hoot/core/geometry/GeometryUtils.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/ops/RemoveElementByEid.h>
#include <hoot/core/ops/RemoveNodeByEid.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/SignalCatcher.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/util/Validate.h>
#include <hoot/core/visitors/ConstElementVisitor.h>

// Qt
#include <QTextStream>

using namespace std;

namespace hoot
{

std::shared_ptr<OGRSpatialReference> OsmMap::_wgs84;

OsmMap::OsmMap()
  : _index(std::make_shared<OsmMapIndex>(*this)),
    _idSwap(std::make_shared<IdSwap>()),
    _enableProgressLogging(true)
{
  if (!_wgs84)
    _wgs84 = MapProjector::createWgs84Projection();
  _srs = _wgs84;

  setIdGenerator(IdGenerator::getInstance());
  _initCounters();
}

OsmMap::OsmMap(const ConstOsmMapPtr& map)
  : _enableProgressLogging(true)
{
  _copy(map);
  _initCounters();
}

OsmMap::OsmMap(const OsmMapPtr& map)
  : _enableProgressLogging(true)
{
  _copy(map);
  _initCounters();
}

OsmMap::OsmMap(const std::shared_ptr<OGRSpatialReference>& srs)
  : _srs(srs),
    _index(std::make_shared<OsmMapIndex>(*this)),
    _idSwap(std::make_shared<IdSwap>()),
    _enableProgressLogging(true)
{
  setIdGenerator(IdGenerator::getInstance());
  _initCounters();
}

void OsmMap::_initCounters()
{
  _numNodesAppended = 0;
  _numWaysAppended = 0;
  _numRelationsAppended = 0;
  _numNodesSkippedForAppending = 0;
  _numWaysSkippedForAppending = 0;
  _numRelationsSkippedForAppending = 0;
}

void OsmMap::append(const ConstOsmMapPtr& appendFromMap, const bool throwOutDupes)
{
  if (this == appendFromMap.get())
    throw HootException("Can't append to the same map.");
  if (!getProjection()->IsSame(appendFromMap->getProjection().get()))
  {
    char* wkt1 = nullptr;
    getProjection()->exportToPrettyWkt(&wkt1);
    QString proj1 = QString::fromLatin1(wkt1);
    CPLFree(wkt1);
    char* wkt2 = nullptr;
    appendFromMap->getProjection()->exportToPrettyWkt(&wkt2);
    QString proj2 = QString::fromLatin1(wkt2);
    CPLFree(wkt2);
    throw HootException("Incompatible maps.  Map being appended to has projection:\n" + proj1 +
                        "\nMap being appended from has projection:\n" + proj2);
  }

  LOG_DEBUG("Appending maps...");

  _initCounters();
  _srs = appendFromMap->getProjection();
  // These indexes need to be force initialized before appending.
  _index->getNodeToWayMap();
  _index->getElementToRelationMap();
  ElementComparer elementComparer;

  // The append order must be nodes, ways, and then relations. If not, the map indexes won't update
  // properly.

  NodeMap::const_iterator itn = appendFromMap->_nodes.begin();
  while (itn != appendFromMap->_nodes.end())
  {
    bool appendElement = true;
    NodePtr node = itn->second;
    if (containsElement(ElementId(node->getElementId())))
    {
      // If they have the same ID but aren't considered to be identical elements, throw an error.
      // Otherwise, we'll just skip adding the identical element, since we already have it.
      // throwOutDupes being enabled lets us skip it whether the two are identical or not.
      ElementPtr existingElement = getElement(node->getElementId());
      if (!throwOutDupes && !elementComparer.isSame(node, existingElement))
      {
        const QString msg =
          QString("Map already contains %1; existing element: %2; attempting to replace with element: %3")
            .arg(node->getElementId().toString(),
                 getElement(node->getElementId())->toString(),
                 node->toString());
        throw HootException(msg);
      }
      else
      {
        LOG_TRACE("Skipping appending same element: " << node->getElementId());
        appendElement = false;
      }
    }

    if (appendElement)
    {
      NodePtr n = std::make_shared<Node>(*node);
      LOG_TRACE("Appending: " << n->getElementId() << "...");
      addNode(n);
      _numNodesAppended++;
    }
    else
      _numNodesSkippedForAppending++;

    ++itn;
  }

  WayMap::const_iterator it = appendFromMap->_ways.begin();
  while (it != appendFromMap->_ways.end())
  {
    bool appendElement = true;
    WayPtr way = it->second;
    if (containsElement(ElementId(way->getElementId())))
    {
      ElementPtr existingElement = getElement(way->getElementId());
      if (!throwOutDupes && !elementComparer.isSame(way, existingElement))
      {
        const QString msg =
          QString("Map already contains %1; existing element: %2; attempting to replace with element: %3")
          .arg(way->getElementId().toString(),
               getElement(way->getElementId())->toString(),
               way->toString());
        throw HootException(msg);
      }
      else
      {
        LOG_TRACE("Skipping appending same element: " << way->getElementId());
        appendElement = false;
      }
    }

    if (appendElement)
    {
      WayPtr w = std::make_shared<Way>(*way);
      LOG_TRACE("Appending: " << w->getElementId() << "...");
      addWay(w);
      _numWaysAppended++;
    }
    else
      _numWaysSkippedForAppending++;

    ++it;
  }

  const RelationMap& allRelations = appendFromMap->getRelations();
  for (auto r_it = allRelations.begin(); r_it != allRelations.end(); ++r_it)
  {
    bool appendElement = true;
    RelationPtr relation = r_it->second;
    if (containsElement(ElementId(relation->getElementId())))
    {
      ElementPtr existingElement = getElement(relation->getElementId());
      if (!throwOutDupes && !elementComparer.isSame(relation, existingElement))
      {
        const QString msg =
          QString("Map already contains %1; existing element: %2; attempting to replace with element: %3")
          .arg(relation->getElementId().toString(),
               getElement(relation->getElementId())->toString(),
               relation->toString());
        throw HootException(msg);
      }
      else
      {
        LOG_TRACE("Skipping appending same element: " << relation->getElementId());
        appendElement = false;
      }
    }

    if (appendElement)
    {
      RelationPtr r = std::make_shared<Relation>(*relation);
      LOG_TRACE("Appending: " << r->getElementId() << "...");
      addRelation(r);
      _numRelationsAppended++;
    }
    else
      _numRelationsSkippedForAppending++;
  }

  for (const auto& l : appendFromMap->getListeners())
    _listeners.push_back(l->clone());

  LOG_VARD(_numNodesAppended);
  LOG_VARD(_numNodesSkippedForAppending);
  LOG_VARD(_numWaysAppended);
  LOG_VARD(_numWaysSkippedForAppending);
  LOG_VARD(_numRelationsAppended);
  LOG_VARD(_numRelationsSkippedForAppending);
}

void OsmMap::addElement(const std::shared_ptr<Element>& e)
{
  switch (e->getElementType().getEnum())
  {
  case ElementType::Node:
    addNode(std::dynamic_pointer_cast<Node>(e));
    break;
  case ElementType::Way:
    addWay(std::dynamic_pointer_cast<Way>(e));
    break;
  case ElementType::Relation:
    addRelation(std::dynamic_pointer_cast<Relation>(e));
    break;
  default:
    throw HootException(QString("Unexpected element type: %1").arg(e->getElementType().toString()));
  }
}

void OsmMap::addNode(const NodePtr& n)
{
  _idGen->ensureNodeBounds(n->getId());
  _nodes[n->getId()] = n;
  n->registerListener(_index.get());
  _index->addNode(n);
}

void OsmMap::addNodes(const std::vector<NodePtr>& nodes)
{
  if (!nodes.empty())
  {
    long minId = nodes[0]->getId();
    long maxId = minId;

    // this seemed like a clever optimization. However, this impacts the BigPertyCmd.sh test b/c
    // it modifies the order in which the elements are written to the output. Which presumably (?)
    // impacts the ID when reading the file with re-numbering. Sad.
    // TODO: The BigPertyCmd.sh test no longer exists, so maybe try it again.
//    size_t minBuckets = _nodes.size() + nodes.size() * 1.1;
//    if (_nodes.bucket_count() < minBuckets)
//    {
//      _nodes.resize(minBuckets);
//    }

    for (const auto& node : nodes)
    {
      long id = node->getId();
      _nodes[id] = node;
      node->registerListener(_index.get());
      _index->addNode(node);
      maxId = std::max(id, maxId);
      minId = std::min(id, minId);
    }

    _idGen->ensureNodeBounds(maxId);
    _idGen->ensureNodeBounds(minId);
  }
}

void OsmMap::addRelation(const RelationPtr& r)
{
  VALIDATE(validate());
  _idGen->ensureRelationBounds(r->getId());
  _relations[r->getId()] = r;
  r->registerListener(_index.get());
  _index->addRelation(r);
  VALIDATE(validate());
}

void OsmMap::addWay(const WayPtr& w)
{
  LOG_TRACE("Adding: " << w->getElementId());
  _idGen->ensureWayBounds(w->getId());
  _ways[w->getId()] = w;
  w->registerListener(_index.get());
  _index->addWay(w);
}

void OsmMap::clear()
{
  _srs = MapProjector::createWgs84Projection();

  _nodes.clear();
  _ways.clear();
  _relations.clear();

  _index->reset();
  _listeners.clear();

  _roundabouts.clear();
  _idSwap.reset();
}

bool OsmMap::containsElement(const ElementId& eid) const
{
  return containsElement(eid.getType(), eid.getId());
}

bool OsmMap::containsElement(ElementType type, long id) const
{
  switch (type.getEnum())
  {
  case ElementType::Node:
    return containsNode(id);
  case ElementType::Way:
    return containsWay(id);
  case ElementType::Relation:
    return containsRelation(id);
  default:
    throw HootException(QString("Unexpected element type: %1").arg(type.toString()));
  }
}

bool OsmMap::containsElement(const std::shared_ptr<const Element>& e) const
{
  return containsElement(e->getElementType(), e->getId());
}

void OsmMap::_copy(const ConstOsmMapPtr& from)
{
  _idGen = from->_idGen;
  _index = std::make_shared<OsmMapIndex>(*this);
  _srs = from->getProjection();
  _roundabouts = from->getRoundabouts();
  _idSwap = from->getIdSwap();
  _name = from->getName();
  _cachedRubberSheet = from->_cachedRubberSheet;

  //  No need to add relations/ways/nodes to the index, b/c the index is created in a lazy fashion.
  const RelationMap& allRelations = from->getRelations();
  for (auto it = allRelations.begin(); it != allRelations.end(); ++it)
  {
    RelationPtr r = std::make_shared<Relation>(*(it->second));
    r->registerListener(_index.get());
    _relations[it->first] = r;
  }

  for (auto it = from->_ways.begin(); it != from->_ways.end(); ++it)
  {
    WayPtr w = std::make_shared<Way>(*(it->second));
    w->registerListener(_index.get());
    _ways[it->first] = w;
  }

  for (auto itn = from->_nodes.begin(); itn != from->_nodes.end(); ++itn)
    _nodes[itn->first] = std::make_shared<Node>(*itn->second);

  for (const auto& l : from->getListeners())
    _listeners.push_back(l->clone());
}

ConstElementPtr OsmMap::getElement(const ElementId& eid) const
{
  return getElement(eid.getType(), eid.getId());
}

ElementPtr OsmMap::getElement(const ElementId& eid)
{
  return getElement(eid.getType(), eid.getId());
}

ConstElementPtr OsmMap::getElement(ElementType type, long id) const
{
  switch (type.getEnum())
  {
  case ElementType::Node:
    return getNode(id);
  case ElementType::Way:
    return getWay(id);
  case ElementType::Relation:
    return getRelation(id);
  default:
    throw HootException(QString("Unexpected element type: %1").arg(type.toString()));
  }
}

ElementPtr OsmMap::getElement(ElementType type, long id)
{
  switch (type.getEnum())
  {
  case ElementType::Node:
    return getNode(id);
  case ElementType::Way:
    return getWay(id);
  case ElementType::Relation:
    return getRelation(id);
  default:
    throw HootException(QString("Unexpected element type: %1").arg(type.toString()));
  }
}

size_t OsmMap::getElementCount() const
{
  return getNodes().size() + getWays().size() + getRelations().size();
}

set<ElementId> OsmMap::getParents(ElementId eid) const
{
  return getIndex().getParents(eid);
}

bool OsmMap::_listContainsNode(const QList<ElementPtr> l) const
{
  for (const auto& element: l)
  {
    if (element->getElementType() == ElementType::Node)
      return true;
  }
  return false;
}

void OsmMap::replace(const std::shared_ptr<const Element>& from, const std::shared_ptr<Element>& to)
{
  LOG_TRACE("Replacing: " << from->getElementId() << " with: " << to->getElementId() << "...");

  const std::shared_ptr<NodeToWayMap>& n2w = getIndex().getNodeToWayMap();

  // Do some error checking before we add the new element.
  if (from->getElementType() == ElementType::Node &&
      to->getElementType() != ElementType::Node &&
      !n2w->getWaysByNode(from->getId()).empty())
  {
    throw HootException("Trying to replace a node with a non-node when the node is part of a way.");
  }

  if (from->getElementType() == ElementType::Node && to->getElementType() == ElementType::Node)
    replaceNode(from->getId(), to->getId());
  else
  {
    if (!containsElement(to))
      addElement(to);

    // create a copy of the set b/c we may modify it with replace commands.
    const set<long> rids = getIndex().getElementToRelationMap()->getRelationByElement(from.get());
    for (auto relation_id : rids)
    {
      const RelationPtr& r = getRelation(relation_id);
      r->replaceElement(from, to);
    }

    RemoveElementByEid::removeElementNoCheck(shared_from_this(), from->getElementId());
  }
}

void OsmMap::replace(const std::shared_ptr<const Element>& from, const QList<ElementPtr>& to)
{ 
  const std::shared_ptr<NodeToWayMap>& n2w = getIndex().getNodeToWayMap();

  // Do some error checking before we add the new element.
  if (from->getElementType() == ElementType::Node &&
      (_listContainsNode(to) == false || to.size() > 1) &&
      !n2w->getWaysByNode(from->getId()).empty())
  {
    throw IllegalArgumentException(
      "Trying to replace a node with multiple nodes or a non-node when the node is part of a way.");
  }

  if (from->getElementType() == ElementType::Node && to.size() == 1 &&
      to[0]->getElementType() == ElementType::Node)
  {
    replaceNode(from->getId(), to[0]->getId());
  }
  else
  {
    QList<long> elem;
    for (const auto& element : to)
    {
      elem.append(element->getId());
      if (!containsElement(element))
        addElement(element);
    }

    // Create a copy of the set, b/c we may modify it with replace commands.
    const set<long> rids = getIndex().getElementToRelationMap()->getRelationByElement(from.get());
    for (auto relation_id : rids)
    {
      const RelationPtr& r = getRelation(relation_id);
      r->replaceElement(from, to);
    }

    //  Don't remove the element if it is being replaced by itself
    if (!elem.contains(from->getId()))
      RemoveElementByEid::removeElementNoCheck(shared_from_this(), from->getElementId());
  }
}

void OsmMap::replaceNode(long oldId, long newId)
{  
  //  nothing to do
  if (oldId == newId)
    return;

  LOG_TRACE("Replacing node: " << oldId << " with: " << newId << "...");

  for (size_t i = 0; i < _listeners.size(); i++)
    _listeners[i]->replaceNodePre(oldId, newId);

  const std::shared_ptr<NodeToWayMap>& n2w = getIndex().getNodeToWayMap();

  // get a copy of the ways so our changes below don't modify the working set.
  const set<long> ways = n2w->getWaysByNode(oldId);

  VALIDATE(getIndex().getNodeToWayMap()->validate(*this));

  for (auto way_id : ways)
  {
    const WayPtr& w = getWay(way_id);
    LOG_VART(w->getElementId());

#   ifdef DEBUG
    if (w.get() == nullptr)
    {
      LOG_WARN("Way for way id: " << way_id << " is null.");
    }
#   endif

    w->replaceNode(oldId, newId);
  }

  // Replace node in any relations it exists in
  _replaceNodeInRelations(oldId, newId);

  if (containsNode(oldId))
    RemoveNodeByEid::removeNodeNoCheck(shared_from_this(), oldId);

  VALIDATE(getIndex().getNodeToWayMap()->validate(*this));
}

void OsmMap::setProjection(const std::shared_ptr<OGRSpatialReference>& srs)
{
  _srs = srs;
  _index->reset();
}

bool OsmMap::validate(bool strict) const
{
  bool result = true;
  result &= getIndex().getNodeToWayMap()->validate(*this);

  const WayMap& allWays = (*this).getWays();
  for (auto it = allWays.begin(); it != allWays.end(); ++it)
  {
    const ConstWayPtr& way = it->second;

    const vector<long>& nids = way->getNodeIds();
    vector<long> missingNodes;
    bool badWay = false;
    for (auto node_id : nids)
    {
      if (containsNode(node_id) == false)
      {
        badWay = true;
        result = false;
        missingNodes.push_back(node_id);
      }
    }

    if (badWay)
    {
      LOG_WARN("Way failed validation. Way: " << way->toString());
      LOG_WARN("  missing nodes: " << missingNodes);
    }
  }

  const RelationMap& allRelations = getRelations();
  for (auto it = allRelations.begin(); it != allRelations.end(); ++it)
  {
    const ConstRelationPtr& relation = it->second;

    const vector<RelationData::Entry>& members = relation->getMembers();
    vector<RelationData::Entry> missingElements;
    bool badRelation = false;
    for (const auto& member : members)
    {
      if (containsElement(member.getElementId()) == false)
      {
        badRelation = true;
        result = false;
        missingElements.push_back(member);
      }
    }

    if (badRelation)
    {
      LOG_WARN("Relation failed validation. Relation: " << relation->toString());
      LOG_WARN("  missing members: " << missingElements);
    }
  }

  if (strict)
    result &= _index->validate();

  if (strict && result == false)
    throw HootException("OsmMap validation failed. See log for details.");

  if (result == false)
  {
    LOG_WARN("OsmMap is invalid.");
  }

  return result;
}

void OsmMap::visitRo(ConstElementVisitor& visitor) const
{
  visitNodesRo(visitor);
  visitWaysRo(visitor);
  visitRelationsRo(visitor);
}

void OsmMap::visitNodesRo(ConstElementVisitor& visitor) const
{
  ConstOsmMapConsumer* consumer = dynamic_cast<ConstOsmMapConsumer*>(&visitor);
  if (consumer != nullptr)
    consumer->setOsmMap(this);

  // Make a copy so we can iterate through even if there are changes.
  const NodeMap& allNodes = getNodes();
  int numVisited = 0;
  const int taskStatusUpdateInterval = ConfigOptions().getTaskStatusUpdateInterval();
  for (auto it = allNodes.begin(); it != allNodes.end(); ++it)
  {
    if (containsNode(it->first))
    {
      ConstNodePtr node = std::dynamic_pointer_cast<const Node>(it->second);
      if (node)
        visitor.visit(node);
    }

    numVisited++;
    if (_enableProgressLogging && numVisited % (taskStatusUpdateInterval * 10) == 0)
    {
      PROGRESS_INFO(
        "\tProcessed " << StringUtils::formatLargeNumber(numVisited) << " of " <<
        StringUtils::formatLargeNumber(allNodes.size()) << " nodes.");
    }
  }
}

void OsmMap::visitWaysRo(ConstElementVisitor& visitor) const
{
  ConstOsmMapConsumer* consumer = dynamic_cast<ConstOsmMapConsumer*>(&visitor);
  if (consumer != nullptr)
    consumer->setOsmMap(this);

  // make a copy so we can iterate through even if there are changes.
  const WayMap& allWays = getWays();
  int numVisited = 0;
  const int taskStatusUpdateInterval = ConfigOptions().getTaskStatusUpdateInterval();
  for (auto it = allWays.begin(); it != allWays.end(); ++it)
  {
    if (containsWay(it->first))
    {
      ConstWayPtr way = std::dynamic_pointer_cast<const Way>(it->second);
      if (way)
        visitor.visit(way);
    }

    numVisited++;
    if (_enableProgressLogging && numVisited % (taskStatusUpdateInterval * 10) == 0)
    {
      PROGRESS_INFO(
        "\tProcessed " << StringUtils::formatLargeNumber(numVisited) << " of " <<
        StringUtils::formatLargeNumber(allWays.size()) << " ways.");
    }
  }
}

void OsmMap::visitRelationsRo(ConstElementVisitor& visitor) const
{
  ConstOsmMapConsumer* consumer = dynamic_cast<ConstOsmMapConsumer*>(&visitor);
  if (consumer != nullptr)
    consumer->setOsmMap(this);

  // make a copy so we can iterate through even if there are changes.
  const RelationMap& allRelations = getRelations();
  int numVisited = 0;
  const int taskStatusUpdateInterval = ConfigOptions().getTaskStatusUpdateInterval();
  for (auto it = allRelations.begin(); it != allRelations.end(); ++it)
  {
    if (containsRelation(it->first))
    {
      ConstRelationPtr relation = std::dynamic_pointer_cast<const Relation>(it->second);
      if (relation)
        visitor.visit(relation);
    }

    numVisited++;
    if (_enableProgressLogging && numVisited % (taskStatusUpdateInterval * 10) == 0)
    {
      PROGRESS_INFO(
        "\tProcessed " << StringUtils::formatLargeNumber(numVisited) << " of " <<
        StringUtils::formatLargeNumber(allRelations.size()) << " relations.");
    }
  }
}

void OsmMap::visitRw(ConstElementVisitor& visitor)
{
  OsmMapConsumer* consumer = dynamic_cast<OsmMapConsumer*>(&visitor);
  if (consumer != nullptr)
    consumer->setOsmMap(this);

  //  Calling visitNodesRw() here causes many unit test failures because of the difference between
  //  ConstNodePtr and NodePtr calls to ElementVisitor::visit(), ways and relations aren't affected

  // make a copy so we can iterate through even if there are changes.
  const NodeMap allNodes = getNodes();
  int numVisited = 0;
  const int taskStatusUpdateInterval = ConfigOptions().getTaskStatusUpdateInterval();
  for (auto it = allNodes.begin(); it != allNodes.end(); ++it)
  {
    if (containsNode(it->first))
    {
      ConstNodePtr node = std::dynamic_pointer_cast<const Node>(it->second);
      if (node)
        visitor.visit(node);
    }

    numVisited++;
    if (_enableProgressLogging && numVisited % (taskStatusUpdateInterval * 10) == 0)
    {
      PROGRESS_INFO(
        "\tProcessed " << StringUtils::formatLargeNumber(numVisited) << " of " <<
        StringUtils::formatLargeNumber(allNodes.size()) << " nodes.");
    }
  }
  visitWaysRw(visitor);
  visitRelationsRw(visitor);
}

void OsmMap::visitRw(ElementVisitor& visitor)
{
  visitNodesRw(visitor);
  visitWaysRw(visitor);
  visitRelationsRw(visitor);
}

void OsmMap::visitNodesRw(ElementVisitor& visitor)
{
  OsmMapConsumer* consumer = dynamic_cast<OsmMapConsumer*>(&visitor);
  if (consumer != nullptr)
    consumer->setOsmMap(this);

  // make a copy so we can iterate through even if there are changes.
  const NodeMap allNodes = getNodes();
  int numVisited = 0;
  const int taskStatusUpdateInterval = ConfigOptions().getTaskStatusUpdateInterval();
  for (auto it = allNodes.begin(); it != allNodes.end(); ++it)
  {
    if (containsNode(it->first))
    {
      NodePtr node = std::dynamic_pointer_cast<Node>(it->second);
      if (node)
        visitor.visit(node);
    }

    numVisited++;
    if (_enableProgressLogging && numVisited % (taskStatusUpdateInterval * 10) == 0)
    {
      PROGRESS_INFO(
        "\tProcessed " << StringUtils::formatLargeNumber(numVisited) << " of " <<
        StringUtils::formatLargeNumber(allNodes.size()) << " nodes.");
    }
  }
}

void OsmMap::visitWaysRw(ElementVisitor& visitor)
{
  OsmMapConsumer* consumer = dynamic_cast<OsmMapConsumer*>(&visitor);
  if (consumer != nullptr)
    consumer->setOsmMap(this);

  //  make a copy so we can iterate through even if there are changes.
  const WayMap allWays = getWays();
  int numVisited = 0;
  const int taskStatusUpdateInterval = ConfigOptions().getTaskStatusUpdateInterval();
  for (auto it = allWays.begin(); it != allWays.end(); ++it)
  {
    if (containsWay(it->first))
    {
      WayPtr way = std::dynamic_pointer_cast<Way>(it->second);
      if (way)
        visitor.visit(way);
    }

    numVisited++;
    if (_enableProgressLogging && numVisited % (taskStatusUpdateInterval * 10) == 0)
    {
      PROGRESS_INFO(
        "\tProcessed " << StringUtils::formatLargeNumber(numVisited) << " of " <<
        StringUtils::formatLargeNumber(allWays.size()) << " ways.");
    }
  }
}

void OsmMap::visitRelationsRw(ElementVisitor& visitor)
{
  OsmMapConsumer* consumer = dynamic_cast<OsmMapConsumer*>(&visitor);
  if (consumer != nullptr)
    consumer->setOsmMap(this);

  // make a copy so we can iterate through even if there are changes.
  const RelationMap allRs = getRelations();
  int numVisited = 0;
  const int taskStatusUpdateInterval = ConfigOptions().getTaskStatusUpdateInterval();
  for (auto it = allRs.begin(); it != allRs.end(); ++it)
  {
    if (containsRelation(it->first))
    {
      RelationPtr relation = std::dynamic_pointer_cast<Relation>(it->second);
      if (relation)
        visitor.visit(relation);
    }

    numVisited++;
    if (_enableProgressLogging && numVisited % (taskStatusUpdateInterval * 10) == 0)
    {
      PROGRESS_INFO(
        "\tProcessed " << StringUtils::formatLargeNumber(numVisited) << " of " <<
        StringUtils::formatLargeNumber(allRs.size()) << " relations.");
    }
  }
}

void OsmMap::_replaceNodeInRelations(long oldId, long newId)
{
  const ElementId oldNodeId = ElementId::node(oldId);

  LOG_TRACE("Replace node in relations: replace " << oldId << " with " << newId );

  NodeMap::iterator it;

  // Make sure both nodes exist; calling getNode on non-existent IDs causes failed assert

  it = _nodes.find(oldId);
  if (it == _nodes.end())
  {
    LOG_TRACE("Tried to replace a non-existent node " << oldId );
    return;
  }

  it = _nodes.find(newId);
  if ( it == _nodes.end() )
  {
    LOG_TRACE("Replacement node " << newId << "does not exist");
    return;
  }

  ConstElementPtr oldNode = getNode(oldId);
  ConstElementPtr newNode = getNode(newId);
  //  Use the index to get all relations containing the old node ID
  const std::shared_ptr<ElementToRelationMap>& mapping = getIndex().getElementToRelationMap();
  const set<long>& relation_map = mapping->getRelationByElement(oldNodeId);
  vector<long> relations(relation_map.begin(), relation_map.end());
  //  Only iterate the relations that contain the old node ID
  for (auto relation_id: relations)
  {
    RelationPtr currRelation = getRelation(relation_id);
    if (!currRelation || !currRelation->contains(oldNodeId))
      continue;
    LOG_TRACE("Trying to replace node " << oldNode->getId() << " with node " <<
              newNode->getId() << " in relation " << currRelation->getId());
    currRelation->replaceElement(oldNode, newNode);
  }
}

QString OsmMap::getSource() const
{
  QString buffer;
  QTextStream ts(&buffer);
  bool first = true;
  for (const auto& source : _sources)
  {
    if (first)
      first = false;
    else
      ts << ";";
    ts << source;
  }
  return ts.readAll();
}

void OsmMap::appendSource(const QString& url)
{
  QStringList urls = url.split(";");
  for (const auto& url : urls)
  {
    QUrl src(url);
    QString source;
    if (src.scheme() == "")
      source = QFileInfo(src.toString()).fileName();
    else if (src.isLocalFile())
      source = QFileInfo(src.toLocalFile()).fileName();
    else if (src.scheme().toLower() == MetadataTags::HootApiDbScheme() ||
             src.scheme().toLower() == MetadataTags::OsmApiDbScheme())
      source = src.scheme() + ":" + src.path().split("/").last();
    else
      source = src.toDisplayString();
    _sources.insert(source);
  }
}

void OsmMap::replaceSource(const QString &url)
{
  _sources.clear();
  appendSource(url);
}

QSet<long> OsmMap::getNodeIds() const
{
  QSet<long> ids;
  for (auto it = _nodes.begin(); it != _nodes.end(); ++it)
    ids.insert(it->first);
  return ids;
}

QSet<long> OsmMap::getWayIds() const
{
  QSet<long> ids;
  for (auto it = _ways.begin(); it != _ways.end(); ++it)
    ids.insert(it->first);
  return ids;
}

QSet<long> OsmMap::getRelationIds() const
{
  QSet<long> ids;
  for (auto it = _relations.begin(); it != _relations.end(); ++it)
    ids.insert(it->first);
  return ids;
}

void OsmMap::resetIterator()
{
  _currentNodeItr = getNodes().begin();
}

void OsmMap::_next()
{
  if (_currentElementId.isNull())
  {
    if (getNodes().size() > 0)
      _currentNodeItr = getNodes().begin();
    else if (getWays().size() > 0)
      _currentWayItr = getWays().begin();
    else if (getRelations().size() > 0)
      _currentRelationItr = getRelations().begin();
  }

  if (getNodes().size() > 0 && _currentNodeItr != getNodes().end())
  {
    const ElementId nodeId = ElementId(ElementType::Node, _currentNodeItr->first);
    LOG_VART(nodeId);
    _addElement(getNode(nodeId.getId()));
    _currentElementId = nodeId;
    _currentNodeItr++;
    if (_currentNodeItr == getNodes().end())
      _currentWayItr = getWays().begin();
  }
  else if (getWays().size() > 0 && _currentWayItr != getWays().end())
  {
    const ElementId wayId = ElementId(ElementType::Way, _currentWayItr->first);
    LOG_VART(wayId);
    _addElement(getWay(wayId.getId()));
    _currentElementId = wayId;
    _currentWayItr++;
    if (_currentWayItr == getWays().end())
      _currentRelationItr = getRelations().begin();
  }
  else if (getRelations().size() > 0 && _currentRelationItr != getRelations().end())
  {
    const ElementId relationId = ElementId(ElementType::Relation, _currentRelationItr->first);
    LOG_VART(relationId);
    _addElement(getRelation(relationId.getId()));
    _currentElementId = relationId;
    _currentRelationItr++;
  }
}

}
