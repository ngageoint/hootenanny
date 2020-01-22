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

#include "OsmUtils.h"

// Hoot
#include <hoot/core/elements/Node.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/criterion/PoiCriterion.h>
#include <hoot/core/criterion/BuildingCriterion.h>
#include <hoot/core/criterion/NonBuildingAreaCriterion.h>
#include <hoot/core/criterion/poi-polygon/PoiPolygonPoiCriterion.h>
#include <hoot/core/criterion/poi-polygon/PoiPolygonPolyCriterion.h>
#include <hoot/core/criterion/OneWayCriterion.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/elements/NodeToWayMap.h>
#include <hoot/core/algorithms/WayDiscretizer.h>
#include <hoot/core/algorithms/Distance.h>
#include <hoot/core/criterion/ChainCriterion.h>
#include <hoot/core/ops/CopyMapSubsetOp.h>
#include <hoot/core/criterion/AttributeValueCriterion.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/criterion/PointCriterion.h>
#include <hoot/core/visitors/UniqueElementIdVisitor.h>
#include <hoot/core/criterion/IdTagMatchesId.h>
#include <hoot/core/elements/ElementConverter.h>
#include <hoot/core/util/Factory.h>

// Qt
#include <QDateTime>
#include <QRegExp>
#include <QStringBuilder>

// Std
#include <float.h>

// GEOS
#include <geos/util/TopologyException.h>

using namespace geos::geom;
using namespace std;

namespace hoot
{

int OsmUtils::_badGeomCount = 0;

void OsmUtils::printNodes(const QString& nodeCollectionName,
                          const QList<std::shared_ptr<const Node>>& nodes)
{
  if (Log::getInstance().getLevel() == Log::Trace)
  {
    LOG_DEBUG(nodeCollectionName);
    LOG_VARD(nodes.size());
    for (QList<std::shared_ptr<const Node>>::const_iterator it = nodes.begin();
         it != nodes.end(); ++it)
    {
      std::shared_ptr<const Node> node = *it;
      LOG_VARD(node->toString());
    }
  }
}

QList<long> OsmUtils::nodesToNodeIds(const QList<std::shared_ptr<const Node>>& nodes)
{
  QList<long> nodeIds;
  for (QList<std::shared_ptr<const Node>>::const_iterator it = nodes.constBegin();
       it != nodes.constEnd(); ++it)
  {
    std::shared_ptr<const Node> node = *it;
    nodeIds.append(node->getElementId().getId());
  }
  return nodeIds;
}

std::vector<long> OsmUtils::nodesToNodeIds(const std::vector<std::shared_ptr<const Node>>& nodes)
{
  std::vector<long> nodeIds;
  for (std::vector<std::shared_ptr<const Node>>::const_iterator it = nodes.begin();
       it != nodes.end(); ++it)
  {
    std::shared_ptr<const Node> node = *it;
    if (node)
    {
      nodeIds.push_back(node->getElementId().getId());
    }
  }
  return nodeIds;
}

QList<std::shared_ptr<const Node>> OsmUtils::nodeIdsToNodes(
  const QList<long>& nodeIds, const std::shared_ptr<const OsmMap>& map)
{
  QList<std::shared_ptr<const Node>> nodes;
  for (QList<long>::const_iterator it = nodeIds.constBegin(); it != nodeIds.constEnd(); ++it)
  {
    nodes.append(std::dynamic_pointer_cast<const Node>(map->getElement(ElementType::Node, *it)));
  }
  return nodes;
}

std::vector<std::shared_ptr<const Node>> OsmUtils::nodeIdsToNodes(
  const std::vector<long>& nodeIds, const std::shared_ptr<const OsmMap>& map)
{
  std::vector<std::shared_ptr<const Node>> nodes;
  for (std::vector<long>::const_iterator it = nodeIds.begin(); it != nodeIds.end(); ++it)
  {
    nodes.push_back(std::dynamic_pointer_cast<const Node>(map->getElement(ElementType::Node, *it)));
  }
  return nodes;
}

bool OsmUtils::nodeCoordsMatch(std::vector<std::shared_ptr<const Node>> nodes1,
                               std::vector<std::shared_ptr<const Node>> nodes2)
{
  if (nodes1.size() != nodes2.size())
  {
    return false;
  }

  for (size_t i = 0; i < nodes1.size(); i++)
  {
    ConstNodePtr node1 = nodes1[i];
    ConstNodePtr node2 = nodes2[i];

    if (!node1 || !node2)
    {
      return false;
    }

    if (!node1->coordsMatch(*node2))
    {
      return false;
    }
  }

  return true;
}

QString OsmUtils::nodeCoordsToString(const std::vector<ConstNodePtr>& nodes)
{
  QString str;
  const int comparisonSensitivity = ConfigOptions().getNodeComparisonCoordinateSensitivity();
  for (size_t i = 0; i < nodes.size(); i++)
  {
    ConstNodePtr node = nodes[i];
    if (node)
    {
      str +=
        "ID: " + QString::number(node->getId()) + ", X: " +
        QString::number(node->getX(), 'f', comparisonSensitivity) + ", Y: " +
        QString::number(node->getY(), 'f', comparisonSensitivity) + "; ";
    }
    else
    {
      str += "null coord; ";
    }
  }
  str.chop(2);
  return str;
}

bool OsmUtils::nodeCoordsMatch(const ConstWayPtr& way1, const ConstWayPtr& way2,
                               const ConstOsmMapPtr& map)
{
  return
    nodeCoordsMatch(
      nodeIdsToNodes(way1->getNodeIds(), map), nodeIdsToNodes(way2->getNodeIds(), map));
}

Coordinate OsmUtils::nodeToCoord(const std::shared_ptr<const Node>& node)
{
  return Coordinate(node->getX(), node->getY());
}

QString OsmUtils::toTimeString(quint64 timestamp)
{
  // convert time in seconds since epoch into timestamp string
  QDateTime dt;
  dt.setTimeSpec(Qt::UTC);
  dt.setMSecsSinceEpoch(timestamp*1000);
  return dt.toString("yyyy-MM-ddThh:mm:ssZ");
}

quint64 OsmUtils::fromTimeString(QString timestamp)
{
  //2016-05-04T22:07:19Z
  QRegExp timestampRegex("\\d{4}-\\d{2}-\\d{2}T\\d{2}:\\d{2}:\\d{2}Z*");
  if (!timestampRegex.exactMatch(timestamp))
  {
    throw IllegalArgumentException("Invalid timestamp string: " + timestamp);
  }

  struct tm t;
  strptime(timestamp.toStdString().c_str(), "%Y-%m-%dT%H:%M:%SZ", &t);

  // calc time in seconds since epoch
  return (quint64)(t.tm_sec + t.tm_min*60 + t.tm_hour*3600 + t.tm_yday*86400 +
    (t.tm_year-70)*31536000 + ((t.tm_year-69)/4)*86400 -
    ((t.tm_year-1)/100)*86400 + ((t.tm_year+299)/400)*86400);
}

QString OsmUtils::currentTimeAsString()
{
  return QDateTime::currentDateTime().toString("yyyy-MM-ddThh:mm:ssZ");
}

QString OsmUtils::getRelationDetailedString(const ConstRelationPtr& relation,
                                            const ConstOsmMapPtr& map)
{
  return relation->toString() + getRelationMembersDetailedString(relation, map);
}

QString OsmUtils::getRelationMembersDetailedString(const ConstRelationPtr& relation,
                                                   const ConstOsmMapPtr& map)
{
  QString str = "\nMember Detail:\n\n";
  const std::vector<RelationData::Entry> relationMembers = relation->getMembers();
  LOG_VART(relationMembers.size());
  for (size_t i = 0; i < relationMembers.size(); i++)
  {
    str += "Member #" + QString::number(i + 1) + ":\n\n";
    ConstElementPtr member = map->getElement(relationMembers[i].getElementId());
    if (!member)
    {
      throw HootException(
        "Unable to retrieve relation member: " + relationMembers[i].getElementId().toString() +
        ". Skipping adding it to output...");
    }
    LOG_VART(member->getElementId());
    str += member->toString() + "\n\n";
  }
  return str;
}

QString OsmUtils::getWayNodesDetailedString(const ConstWayPtr& way, const ConstOsmMapPtr& map)
{
  return nodeCoordsToString(nodeIdsToNodes(way->getNodeIds(), map));
}

long OsmUtils::getFirstWayIdFromRelation(const ConstRelationPtr& relation, const OsmMapPtr& map)
{
  const std::vector<RelationData::Entry>& relationMembers = relation->getMembers();
  QSet<long> wayMemberIds;
  for (size_t i = 0; i < relationMembers.size(); i++)
  {
    ConstElementPtr member = map->getElement(relationMembers[i].getElementId());
    if (member->getElementType() == ElementType::Way)
    {
      wayMemberIds.insert(member->getId());
    }
  }
  LOG_VART(wayMemberIds);
  if (wayMemberIds.size() > 0)
  {
    return wayMemberIds.toList().at(0);
  }
  else
  {
    return 0;
  }
}

QString OsmUtils::getElementDetailString(const ConstElementPtr& element, const ConstOsmMapPtr& map)
{
  QString str;
  str += element->toString() + "\n";
  if (element->getElementType() == ElementType::Way)
  {
    ConstWayPtr way = std::dynamic_pointer_cast<const Way>(element);
    str += OsmUtils::getWayNodesDetailedString(way, map) + "\n";
  }
  else if (element->getElementType() == ElementType::Relation)
  {
    ConstRelationPtr relation = std::dynamic_pointer_cast<const Relation>(element);
    str += OsmUtils::getRelationMembersDetailedString(relation, map) + "\n";
  }
  return str;
}

bool OsmUtils::oneWayConflictExists(const ConstElementPtr& element1,
                                    const ConstElementPtr& element2)
{
  // Technically, this should also take into account reverse one ways and check direction.  Since
  // we have a map pre-op standardizing all the ways to not be reversed, not worrying about it for
  // now.
  OneWayCriterion isAOneWayStreet;
  return
    (isAOneWayStreet.isSatisfied(element1) && explicitlyNotAOneWayStreet(element2)) ||
    (isAOneWayStreet.isSatisfied(element2) && explicitlyNotAOneWayStreet(element1));
}

bool OsmUtils::explicitlyNotAOneWayStreet(const ConstElementPtr& element)
{
  // TODO: use Tags::isFalse here instead
  return element->getTags().get("oneway") == "no";
}

bool OsmUtils::nameConflictExists(const ConstElementPtr& element1, const ConstElementPtr& element2)
{
  return
    element1->getTags().hasName() && element2->getTags().hasName() &&
      !Tags::haveMatchingName(element1->getTags(), element2->getTags());
}

bool OsmUtils::nonGenericHighwayConflictExists(const ConstElementPtr& element1,
                                               const ConstElementPtr& element2)
{
  const QString element1HighwayVal = element1->getTags().get("highway");
  const QString element2HighwayVal = element2->getTags().get("highway");
  return
    element1HighwayVal != "road" && element2HighwayVal != "road" &&
    element1HighwayVal != element2HighwayVal;
}

set<long> OsmUtils::getContainingWayIdsByNodeId(const long nodeId, const ConstOsmMapPtr& map,
                                                const ElementCriterionPtr& wayCriterion)
{
  LOG_VART(nodeId);
  set<long> containingWayIds;

  const set<long>& idsOfWaysContainingNode =
    map->getIndex().getNodeToWayMap()->getWaysByNode(nodeId);
  LOG_VART(idsOfWaysContainingNode);
  for (set<long>::const_iterator containingWaysItr = idsOfWaysContainingNode.begin();
       containingWaysItr != idsOfWaysContainingNode.end(); ++containingWaysItr)
  {
    const long containingWayId = *containingWaysItr;
    LOG_VART(containingWayId);
    LOG_VART(map->getWay(containingWayId));

    if (wayCriterion)
    {
      LOG_VART(typeid(*wayCriterion).name());
      std::shared_ptr<ChainCriterion> chainCrit =
        std::dynamic_pointer_cast<ChainCriterion>(wayCriterion);
      if (chainCrit)
      {
        LOG_VART(chainCrit->toString());
      }
      LOG_VART(wayCriterion->isSatisfied(map->getWay(containingWayId)));
    }

    if (!wayCriterion || wayCriterion->isSatisfied(map->getWay(containingWayId)))
    {
      containingWayIds.insert(containingWayId);
    }
  }

  LOG_VART(containingWayIds);
  return containingWayIds;
}

bool OsmUtils::endWayNodeIsCloserToNodeThanStart(const ConstNodePtr& node, const ConstWayPtr& way,
                                                 const ConstOsmMapPtr& map)
{
  if (way->isFirstLastNodeIdentical())
  {
    return false;
  }
  const double distanceToStartNode =
    Distance::euclidean(node->toCoordinate(), map->getNode(way->getFirstNodeId())->toCoordinate());
  const double distanceToEndNode =
    Distance::euclidean(node->toCoordinate(), map->getNode(way->getLastNodeId())->toCoordinate());
  return distanceToEndNode < distanceToStartNode;
}

Coordinate OsmUtils::closestWayCoordToNode(
  const ConstNodePtr& node, const ConstWayPtr& way, double& distance,
  const double discretizationSpacing, const ConstOsmMapPtr& map)
{
  // split the way up into coords
  vector<Coordinate> discretizedWayCoords;
  WayDiscretizer wayDiscretizer(map, way);
  wayDiscretizer.discretize(discretizationSpacing, discretizedWayCoords);
  // add the first and last coords in (one or both could already be there, but it won't hurt if
  // they're duplicated)
  discretizedWayCoords.insert(
    discretizedWayCoords.begin(), map->getNode(way->getFirstNodeId())->toCoordinate());
  discretizedWayCoords.push_back(map->getNode(way->getLastNodeId())->toCoordinate());
  // determine which end of the way is closer to our input node (to handle ways looping back on
  // themselves)
  if (endWayNodeIsCloserToNodeThanStart(node, way, map))
  {
    std::reverse(discretizedWayCoords.begin(), discretizedWayCoords.end());
  }
  LOG_VART(discretizedWayCoords);

  // find the closest coord to the input node
  double shortestDistance = DBL_MAX;
  double lastDistance = DBL_MAX;
  Coordinate closestWayCoordToNode;
  for (size_t i = 0; i < discretizedWayCoords.size(); i++)
  {
    const Coordinate wayCoord = discretizedWayCoords[i];
    const double distanceBetweenNodeAndWayCoord = wayCoord.distance(node->toCoordinate());
    // Since we're going in node order and started at the closest end of the way, if we start
    // seeing larger distances, then we're done.
    if (distanceBetweenNodeAndWayCoord > lastDistance)
    {
      break;
    }
    if (distanceBetweenNodeAndWayCoord < shortestDistance)
    {
      closestWayCoordToNode = wayCoord;
      shortestDistance = distanceBetweenNodeAndWayCoord;
    }
  }
  distance = shortestDistance;

  LOG_VART(distance);
  LOG_VART(closestWayCoordToNode);

  return closestWayCoordToNode;
}

long OsmUtils::closestWayNodeIdToNode(const ConstNodePtr& node, const ConstWayPtr& way,
                                      const ConstOsmMapPtr& map)
{
  double shortestDistance = DBL_MAX;
  long closestWayNodeId = 0;

  const vector<long>& wayNodeIds = way->getNodeIds();
  for (size_t i = 0; i < wayNodeIds.size(); i++)
  {
    ConstNodePtr wayNode = map->getNode(wayNodeIds[i]);
    const double distanceFromNodeToWayNode =
      Distance::euclidean(node->toCoordinate(), wayNode->toCoordinate());
    if (distanceFromNodeToWayNode < shortestDistance)
    {
      shortestDistance = distanceFromNodeToWayNode;
      closestWayNodeId = wayNode->getId();
    }
  }
  LOG_VART(shortestDistance);

  LOG_VART(closestWayNodeId);
  return closestWayNodeId;
}

bool OsmUtils::nodesAreContainedInTheSameWay(const long nodeId1, const long nodeId2,
                                             const ConstOsmMapPtr& map)
{
  const std::set<long>& waysContainingNode1 =
    map->getIndex().getNodeToWayMap()->getWaysByNode(nodeId1);
  LOG_VART(waysContainingNode1);

  const std::set<long>& waysContainingNode2 =
    map->getIndex().getNodeToWayMap()->getWaysByNode(nodeId2);
  LOG_VART(waysContainingNode2);

  std::set<long> commonNodesBetweenWayGroups;
  std::set_intersection(
    waysContainingNode1.begin(), waysContainingNode1.end(),
    waysContainingNode2.begin(), waysContainingNode2.end(),
    std::inserter(commonNodesBetweenWayGroups, commonNodesBetweenWayGroups.begin()));
  LOG_VART(commonNodesBetweenWayGroups);

  return commonNodesBetweenWayGroups.size() != 0;
}

OsmMapPtr OsmUtils::getMapSubset(const ConstOsmMapPtr& map, const ElementCriterionPtr& filter)
{
  CopyMapSubsetOp wayCopier(map, filter);
  OsmMapPtr output(new OsmMap());
  wayCopier.apply(output);
  return output;
}

bool OsmUtils::elementContainedByAnyRelation(const ElementId& elementId, const ConstOsmMapPtr& map)
{
  return map->getIndex().getElementToRelationMap()->getRelationByElement(elementId).size() > 0;
}

bool OsmUtils::nodeContainedByAnyWay(const long nodeId, const ConstOsmMapPtr& map)
{
  return map->getIndex().getNodeToWayMap()->getWaysByNode(nodeId).size() > 0;
}

bool OsmUtils::nodeContainedByAnyWay(const long nodeId, const std::set<long> wayIds,
                                     const ConstOsmMapPtr& map)
{
  std::set<long> waysContainingNode = map->getIndex().getNodeToWayMap()->getWaysByNode(nodeId);
  std::set<long> commonWayIds;
  std::set_intersection(
    waysContainingNode.begin(), waysContainingNode.end(), wayIds.begin(), wayIds.end(),
    std::inserter(commonWayIds, commonWayIds.begin()));
  return commonWayIds.size() > 0;
}

bool OsmUtils::nodeContainedByMoreThanOneWay(const long nodeId, const ConstOsmMapPtr& map)
{
  return map->getIndex().getNodeToWayMap()->getWaysByNode(nodeId).size() > 1;
}

bool OsmUtils::isChild(const ElementId& elementId, const ConstOsmMapPtr& map)
{
  if (elementContainedByAnyRelation(elementId, map))
  {
    return true;
  }
  if (elementId.getType() == ElementType::Node && nodeContainedByAnyWay(elementId.getId(), map))
  {
    return true;
  }
  return false;
}

bool OsmUtils::allElementIdsPositive(const ConstOsmMapPtr& map)
{
  std::shared_ptr<AttributeValueCriterion> attrCrit(
    new AttributeValueCriterion(
      ElementAttributeType(ElementAttributeType::Id), 1, NumericComparisonType::LessThan));
  return
    (int)FilteredVisitor::getStat(
      attrCrit, std::shared_ptr<ElementCountVisitor>(new ElementCountVisitor()), map) == 0;
}

bool OsmUtils::allElementIdsNegative(const ConstOsmMapPtr& map)
{
  std::shared_ptr<AttributeValueCriterion> attrCrit(
    new AttributeValueCriterion(
      ElementAttributeType(ElementAttributeType::Id), -1, NumericComparisonType::GreaterThan));
  return
    (int)FilteredVisitor::getStat(
      attrCrit, std::shared_ptr<ElementCountVisitor>(new ElementCountVisitor()), map) == 0;
}

bool OsmUtils::allIdTagsMatchIds(const ConstOsmMapPtr& map)
{
  std::shared_ptr<IdTagMatchesId> idCrit(new IdTagMatchesId());
  return
    (int)FilteredVisitor::getStat(
      idCrit, std::shared_ptr<ElementCountVisitor>(new ElementCountVisitor()), map) ==
    (int)map->size();
}

int OsmUtils::versionLessThanOneCount(const OsmMapPtr& map)
{
  std::shared_ptr<AttributeValueCriterion> attrCrit(
    new AttributeValueCriterion(
      ElementAttributeType(ElementAttributeType::Version), 1, NumericComparisonType::LessThan));
  return
    (int)FilteredVisitor::getStat(
      attrCrit, std::shared_ptr<ElementCountVisitor>(new ElementCountVisitor()), map);
}

bool OsmUtils::checkVersionLessThanOneCountAndLogWarning(const OsmMapPtr& map)
{
  const int numberOfRefElementsWithVersionLessThan1 = OsmUtils::versionLessThanOneCount(map);
  if (numberOfRefElementsWithVersionLessThan1 > 0)
  {
    LOG_WARN(
      StringUtils::formatLargeNumber(numberOfRefElementsWithVersionLessThan1) << " features in " <<
      "the reference map have a version less than one. This could lead to difficulties when " <<
      "applying the resulting changeset back to an authoritative data store. Are the versions " <<
      "on the features being populated correctly?");
    return true;
  }
  return false;
}

std::set<ElementId> OsmUtils::getIdsOfElementsWithVersionLessThanOne(const OsmMapPtr& map)
{
  AttributeValueCriterion attrCrit(
    ElementAttributeType(ElementAttributeType::Version), 1, NumericComparisonType::LessThan);
  UniqueElementIdVisitor idSetVis;
  FilteredVisitor filteredVis(attrCrit, idSetVis);
  map->visitRo(filteredVis);
  return idSetVis.getElementSet();
}

bool OsmUtils::mapIsPointsOnly(const OsmMapPtr& map)
{
  std::shared_ptr<PointCriterion> pointCrit(new PointCriterion());
  pointCrit->setOsmMap(map.get());
  return
    (int)FilteredVisitor::getStat(
      pointCrit, ElementVisitorPtr(new ElementCountVisitor()), map) ==
    (int)map->getElementCount();
}

bool OsmUtils::allElementsHaveAnyTagKey(const QStringList& tagKeys,
                                        const std::vector<ElementPtr>& elements)
{
  for (std::vector<ElementPtr>::const_iterator it = elements.begin(); it != elements.end(); ++it)
  {
    ElementPtr element = *it;
    bool elementHasTagKey = false;
    for (int i = 0; i < tagKeys.size(); i++)
    {
      if (element->getTags().contains(tagKeys.at(i)))
      {
        elementHasTagKey = true;
        break;
      }
    }
    if (!elementHasTagKey)
    {
      return false;
    }
  }
  return true;
}

bool OsmUtils::allElementsHaveAnyKvp(const QStringList& kvps,
                                     const std::vector<ElementPtr>& elements)
{

  for (std::vector<ElementPtr>::const_iterator it = elements.begin(); it != elements.end(); ++it)
  {
    ElementPtr element = *it;
    bool elementHasKvp = false;
    for (int i = 0; i < kvps.size(); i++)
    {
      const QString kvp = kvps.at(i);
      const QStringList kvpParts = kvp.split("=");
      if (kvpParts.size() != 2)
      {
        throw IllegalArgumentException("Invalid kvp: " + kvp);
      }
      const QString key = kvpParts[0];
      const QString val = kvpParts[1];
      if (element->getTags()[key] == val)
      {
        elementHasKvp = true;
        break;
      }
    }
    if (!elementHasKvp)
    {
      return false;
    }
  }
  return true;
}

bool OsmUtils::anyElementsHaveAnyTagKey(const QStringList& tagKeys,
                                        const std::vector<ElementPtr>& elements)
{
  for (std::vector<ElementPtr>::const_iterator it = elements.begin(); it != elements.end(); ++it)
  {
    ElementPtr element = *it;
    for (int i = 0; i < tagKeys.size(); i++)
    {
      if (element->getTags().contains(tagKeys.at(i)))
      {
        return true;
      }
    }
  }
  return false;
}

bool OsmUtils::anyElementsHaveAnyKvp(const QStringList& kvps,
                                     const std::vector<ElementPtr>& elements)
{

  for (std::vector<ElementPtr>::const_iterator it = elements.begin(); it != elements.end(); ++it)
  {
    ElementPtr element = *it;
    for (int i = 0; i < kvps.size(); i++)
    {
      const QString kvp = kvps.at(i);
      const QStringList kvpParts = kvp.split("=");
      if (kvpParts.size() != 2)
      {
        throw IllegalArgumentException("Invalid kvp: " + kvp);
      }
      const QString key = kvpParts[0];
      const QString val = kvpParts[1];
      if (element->getTags()[key] == val)
      {
        return true;
      }
    }
  }
  return false;
}

bool OsmUtils::allElementsHaveAnyTagKey(const QStringList& tagKeys,
                                        const std::set<ElementId>& elementIds, OsmMapPtr& map)
{
  std::vector<ElementPtr> elements;
  for (std::set<ElementId>::const_iterator it = elementIds.begin(); it != elementIds.end(); ++it)
  {
    elements.push_back(map->getElement(*it));
  }
  return allElementsHaveAnyTagKey(tagKeys, elements);
}

bool OsmUtils::allElementsHaveAnyKvp(const QStringList& kvps,
                                     const std::set<ElementId>& elementIds, OsmMapPtr& map)
{
  std::vector<ElementPtr> elements;
  for (std::set<ElementId>::const_iterator it = elementIds.begin(); it != elementIds.end(); ++it)
  {
    elements.push_back(map->getElement(*it));
  }
  return allElementsHaveAnyKvp(kvps, elements);
}

bool OsmUtils::anyElementsHaveAnyTagKey(const QStringList& tagKeys,
                                        const std::set<ElementId>& elementIds, OsmMapPtr& map)
{
  std::vector<ElementPtr> elements;
  for (std::set<ElementId>::const_iterator it = elementIds.begin(); it != elementIds.end(); ++it)
  {
    elements.push_back(map->getElement(*it));
  }
  return anyElementsHaveAnyTagKey(tagKeys, elements);
}

bool OsmUtils::anyElementsHaveAnyKvp(const QStringList& kvps,
                                     const std::set<ElementId>& elementIds, OsmMapPtr& map)
{
  std::vector<ElementPtr> elements;
  for (std::set<ElementId>::const_iterator it = elementIds.begin(); it != elementIds.end(); ++it)
  {
    elements.push_back(map->getElement(*it));
  }
  return anyElementsHaveAnyKvp(kvps, elements);
}

std::shared_ptr<geos::geom::Geometry> OsmUtils::_getGeometry(
  const ConstElementPtr& element, ConstOsmMapPtr map)
{
  if (!element)
  {
    throw IllegalArgumentException("The input element is null.");
  }

  std::shared_ptr<geos::geom::Geometry> newGeom;
  QString errorMsg =
    "Feature passed to OsmUtils caused topology exception on conversion to a geometry: ";
  try
  {
    newGeom = ElementConverter(map).convertToGeometry(element);
  }
  catch (const geos::util::TopologyException& e)
  {
    if (_badGeomCount <= Log::getWarnMessageLimit())
    {
      LOG_TRACE(errorMsg << element->toString() << "\n" << e.what());
      _badGeomCount++;
    }
  }
  catch (const HootException& e)
  {
    if (_badGeomCount <= Log::getWarnMessageLimit())
    {
      LOG_TRACE(errorMsg << element->toString() << "\n" << e.what());
      _badGeomCount++;
    }
  }
  if (newGeom.get() &&
      QString::fromStdString(newGeom->toString()).toUpper().contains("EMPTY"))
  {
    if (_badGeomCount <= Log::getWarnMessageLimit())
    {
      LOG_TRACE("Invalid element passed: " << newGeom->toString());
      _badGeomCount++;
    }
    newGeom.reset();
  }
  return newGeom;
}

bool OsmUtils::elementContains(const ConstElementPtr& containingElement,
                               const ConstElementPtr& containedElement, ConstOsmMapPtr map)
{
  if (!containingElement || !containedElement)
  {
    throw IllegalArgumentException("One of the input elements is null.");
  }
  if (containingElement->getElementType() != ElementType::Way &&
      containingElement->getElementType() != ElementType::Relation)
  {
    throw IllegalArgumentException("One of the input elements is of the wrong type.");
  }
  LOG_VART(containedElement->getElementId());
  LOG_VART(containingElement->getElementId());

  std::shared_ptr<geos::geom::Geometry> containingElementGeom =
    _getGeometry(containingElement, map);
  std::shared_ptr<geos::geom::Geometry> containedElementGeom = _getGeometry(containedElement, map);
  bool contains = false;
  if (containingElementGeom && containedElementGeom)
  {
    contains = containingElementGeom->contains(containedElementGeom.get());
    LOG_TRACE(
      "Calculated contains: " << contains << " for containing element: " <<
      containingElement->getElementId() <<
      " and contained element: " << containedElement->getElementId() << ".");
  }
  else
  {
    LOG_TRACE(
      "Unable to calculate contains for containing element: " <<
      containingElement->getElementId() << " and contained element: " <<
      containedElement->getElementId() << ".");
  }
  return contains;
}

bool OsmUtils::containsMember(const ConstElementPtr& parent, const ElementId& memberId)
{
  if (!parent ||
      (parent->getElementType() != ElementType::Way &&
       parent->getElementType() != ElementType::Relation))
  {
    throw IllegalArgumentException("The parent element is null or of the wrong element type.");
  }
  if (parent->getElementType() != ElementType::Way && memberId.getType() != ElementType::Node)
  {
    throw IllegalArgumentException("The inputs are of the wrong element type.");
  }
  if (parent->getElementType() != ElementType::Relation &&
      memberId.getType() == ElementType::Unknown)
  {
    throw IllegalArgumentException("The inputs are of the wrong element type.");
  }

  bool containsMember = false;
  if (parent->getElementType() == ElementType::Way)
  {
    containsMember =
      (std::dynamic_pointer_cast<const Way>(parent))->containsNodeId(memberId.getId());
  }
  else
  {
    containsMember =
      (std::dynamic_pointer_cast<const Relation>(parent))->contains(memberId);
  }
  return containsMember;
}

bool OsmUtils::elementsIntersect(const ConstElementPtr& element1, const ConstElementPtr& element2,
                                 ConstOsmMapPtr map)
{
  if (!element1 || !element2)
  {
    throw IllegalArgumentException("One of the input elements is null.");
  }

  std::shared_ptr<geos::geom::Geometry> geom1 = _getGeometry(element1, map);
  std::shared_ptr<geos::geom::Geometry> geom2 = _getGeometry(element2, map);
  bool intersects = false;
  if (geom1 && geom2)
  {
    intersects = geom1->intersects(geom2.get());
  }
  else
  {
    LOG_TRACE(
      "Unable to calculate intersects for: " << element1->getElementId() <<
      " and: " << element2->getElementId() << ".");
  }
  return intersects;
}

double OsmUtils::getDistance(const ConstElementPtr& element1, const ConstElementPtr& element2,
                             ConstOsmMapPtr map)
{
  if (!element1 || !element2)
  {
    throw IllegalArgumentException("One of the input elements is null.");
  }
  LOG_VART(element1->getElementId());
  LOG_VART(element2->getElementId());

  double distance = -1.0;

  std::shared_ptr<geos::geom::Geometry> element1Geom = _getGeometry(element1, map);
  std::shared_ptr<geos::geom::Geometry> element2Geom = _getGeometry(element2, map);
  if (element1Geom && element2Geom)
  {
    distance = element1Geom->distance(element2Geom.get());
    LOG_TRACE(
      "Calculated distance: " << distance << " for: " << element1->getElementId() <<
      " and: " << element2->getElementId() << ".");
  }
  else
  {
    LOG_TRACE(
      "Unable to calculate distance for: " << element1->getElementId() <<
      " and: " << element2->getElementId() << ".");
  }

  return distance;
}

double OsmUtils::getArea(const ConstElementPtr& element, ConstOsmMapPtr map)
{
  if (!element)
  {
    throw IllegalArgumentException("The input element is null.");
  }

  std::shared_ptr<geos::geom::Geometry> geom = _getGeometry(element, map);
  double area = -1.0;
  if (geom)
  {
    area = geom->getArea();
  }
  else
  {
    LOG_TRACE("Unable to calculate area for: " << element->getElementId() << ".");
  }
  return area;
}

bool OsmUtils::hasCriterion(const ConstElementPtr& element, const QString& criterionClassName)
{
  if (!element || criterionClassName.trimmed().isEmpty())
  {
    throw IllegalArgumentException(
      "The input element is null or the criterion class name is empty.");
  }

  return _getCrit(criterionClassName)->isSatisfied(element);
}

ElementCriterionPtr OsmUtils::_getCrit(const QString& criterionClassName)
{
  if (criterionClassName.trimmed().isEmpty())
  {
    throw IllegalArgumentException("The criterion class name is empty.");
  }

  ElementCriterionPtr crit =
    ElementCriterionPtr(
      Factory::getInstance().constructObject<ElementCriterion>(criterionClassName));
  if (!crit)
  {
    throw IllegalArgumentException(
      "Invalid criterion passed to PoiPolygonInfoCache::hasCriterion: " + criterionClassName);
  }
  return crit;
}

}
