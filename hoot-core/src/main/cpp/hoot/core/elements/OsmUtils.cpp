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

// Qt
#include <QDateTime>
#include <QRegExp>

#include <float.h>

using namespace geos::geom;
using namespace std;

namespace hoot
{

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

const QList<long> OsmUtils::nodesToNodeIds(const QList<std::shared_ptr<const Node>>& nodes)
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

QList<std::shared_ptr<const Node>> OsmUtils::nodeIdsToNodes(const QList<long>& nodeIds,
                                                            const std::shared_ptr<const OsmMap>& map)
{
  QList<std::shared_ptr<const Node>> nodes;
  for (QList<long>::const_iterator it = nodeIds.constBegin(); it != nodeIds.constEnd(); ++it)
  {
    nodes.append(std::dynamic_pointer_cast<const Node>(map->getElement(ElementType::Node, *it)));
  }
  return nodes;
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

QString OsmUtils::getRelationDetailedString(const ConstRelationPtr& relation, const ConstOsmMapPtr& map)
{
  return relation->toString() + getRelationMembersDetailedString(relation, map);
}

QString OsmUtils::getRelationMembersDetailedString(const ConstRelationPtr& relation,
                                                   const ConstOsmMapPtr& map)
{
  QString str = "\nMember Detail:\n\n";
  const std::vector<RelationData::Entry> relationMembers = relation->getMembers();
  for (size_t i = 0; i < relationMembers.size(); i++)
  {
    str += "Member #" + QString::number(i + 1) + ":\n\n";
    ConstElementPtr member = map->getElement(relationMembers[i].getElementId());
    str += member->toString() + "\n\n";
  }
  return str;
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

void OsmUtils::logElementDetail(const ConstElementPtr& element, const ConstOsmMapPtr& map,
                                const Log::WarningLevel& logLevel, const QString& message)
{
  if (Log::getInstance().getLevel() <= logLevel)
  {
    LOG_VAR(message);
    LOG_VAR(element);
    if (element->getElementType() == ElementType::Relation)
    {
      ConstRelationPtr relation = std::dynamic_pointer_cast<const Relation>(element);
      LOG_VAR(OsmUtils::getRelationMembersDetailedString(relation, map));
    }
  }
}

bool OsmUtils::oneWayConflictExists(const ConstElementPtr& element1, const ConstElementPtr& element2)
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

bool OsmUtils::nonGenericHighwayConflictExists(const ConstElementPtr& element1, const ConstElementPtr& element2)
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
  set<long> containingWayIds;

  const set<long>& idsOfWaysContainingNode =
    map->getIndex().getNodeToWayMap()->getWaysByNode(nodeId);
  for (set<long>::const_iterator containingWaysItr = idsOfWaysContainingNode.begin();
       containingWaysItr != idsOfWaysContainingNode.end(); ++containingWaysItr)
  {
    const long containingWayId = *containingWaysItr;;
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
    ConstNodePtr wayNode = map->getNode(wayNodeIds[i]);;
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

bool OsmUtils::nodesAreContainedByTheSameWay(const long nodeId1, const long nodeId2,
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

}
