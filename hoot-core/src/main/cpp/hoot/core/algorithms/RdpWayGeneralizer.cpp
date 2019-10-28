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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "RdpWayGeneralizer.h"

// Standard
#include <cmath>

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/elements/Node.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/elements/OsmUtils.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/ops/RemoveNodeByEid.h>
#include <hoot/core/elements/OsmUtils.h>

using namespace std;

namespace hoot
{

RdpWayGeneralizer::RdpWayGeneralizer(double epsilon) :
_removeNodesSharedByWays(false)
{
  setEpsilon(epsilon);
}

void RdpWayGeneralizer::setEpsilon(double epsilon)
{
  if (epsilon <= 0.0)
  {
    throw HootException("Invalid epsilon value: " + QString::number(epsilon));
  }
  _epsilon = epsilon;
  LOG_VART(_epsilon);
}

int RdpWayGeneralizer::generalize(const std::shared_ptr<Way>& way)
{
  if (!_map)
  {
    throw IllegalArgumentException("No map passed to way generalizer.");
  }

  LOG_TRACE("Attempting to generalize: " << way->getElementId() << "...");

  const QList<long> wayNodeIdsBeforeFiltering =
    QVector<long>::fromStdVector(way->getNodeIds()).toList();
  LOG_VART(wayNodeIdsBeforeFiltering);

  //filter the nodes to be generalized to those in this way and those with no information tags;
  //tried using hoot filters here at first, but it didn't end up making sense
  QList<long> wayNodeIdsAfterFiltering;
  for (QList<long>::const_iterator it = wayNodeIdsBeforeFiltering.begin();
       it != wayNodeIdsBeforeFiltering.end(); ++it)
  {
    if (_map->getNode(*it)->getTags().getInformationCount() == 0)
    {
      wayNodeIdsAfterFiltering.append(*it);
    }
  }
  LOG_VART(wayNodeIdsAfterFiltering);

  //get the generalized points
  const QList<std::shared_ptr<const Node>>& generalizedPoints =
    _getGeneralizedPoints(OsmUtils::nodeIdsToNodes(wayNodeIdsAfterFiltering, _map));
  OsmUtils::printNodes("generalizedPoints", generalizedPoints);
  QList<long> wayNodeIdsAfterGeneralization = OsmUtils::nodesToNodeIds(generalizedPoints);

  // The nodes we're going to remove is the difference between the node ids before and after
  // generalization.
  QSet<long> nodeIdsToRemove =
    wayNodeIdsBeforeFiltering.toSet().subtract(wayNodeIdsAfterGeneralization.toSet());
  LOG_VART(nodeIdsToRemove);

  // If _removeNodesSharedByWays=true, we don't want to remove any nodes shared between ways. So,
  // let's come up with a new removal list that excludes those nodes.
  QSet<long> modifiedNodeIdsToRemove;
  for (QSet<long>::const_iterator it = nodeIdsToRemove.begin(); it != nodeIdsToRemove.end(); ++it)
  {
    const long nodeId = *it;
    LOG_VART(nodeId);
    LOG_VART(_removeNodesSharedByWays);
    LOG_VART(OsmUtils::nodeContainedByMoreThanOneWay(nodeId, _map));
    if (_removeNodesSharedByWays || !OsmUtils::nodeContainedByMoreThanOneWay(nodeId, _map))
    {
      modifiedNodeIdsToRemove.insert(nodeId);
    }
  }
  LOG_VART(modifiedNodeIdsToRemove);

  // Now take the difference between nodes removed during generalization and any shared ones we
  // don't want to remove to get the list of nodes we tried to remove but aren't allowed to.
  const QSet<long> originalNodeIdsToRemove = nodeIdsToRemove;
  const QSet<long> nodeIdsNotAllowedToBeRemoved = nodeIdsToRemove.subtract(modifiedNodeIdsToRemove);
  LOG_VART(nodeIdsNotAllowedToBeRemoved);

  QList<long> updatedWayNodeIds = wayNodeIdsAfterGeneralization;
  if (nodeIdsNotAllowedToBeRemoved.size() > 0)
  {
    // If there were any nodes we removed during generalization but aren't allowed to remove, we
    // need to add those back in here using the pre-generalization node set. We could have tried to
    // suppress their removal during generalization but that's not straight-forward to do in
    // that recursive algorithm.
    updatedWayNodeIds =
      _getUpdatedWayNodeIdsForThoseNotAllowedToBeRemoved(
        nodeIdsNotAllowedToBeRemoved, wayNodeIdsAfterFiltering, wayNodeIdsAfterGeneralization);
  }

  // replace the current nodes on the way with the reduced collection
  way->setNodes(updatedWayNodeIds.toVector().toStdVector());

  // remove any of the unused, recently orphaned way nodes from the map as well (we could also do
  // this with SuperfluousNodeRemover, but that might be a little more intrusive)
  int nodesRemoved = 0;
  LOG_VART(originalNodeIdsToRemove);
  for (QSet<long>::const_iterator it = originalNodeIdsToRemove.begin();
       it != originalNodeIdsToRemove.end(); ++it)
  {
    const long nodeId = *it;
    LOG_VART(nodeId);
    LOG_VART(_removeNodesSharedByWays);
    LOG_VART(nodeIdsNotAllowedToBeRemoved.contains(nodeId));
    if (_removeNodesSharedByWays || !nodeIdsNotAllowedToBeRemoved.contains(nodeId))
    {
      RemoveNodeByEid::removeNode(_map, nodeId, true);
      nodesRemoved++;
    }
  }

  LOG_VART(nodesRemoved);
  return nodesRemoved;
}

QList<long> RdpWayGeneralizer::_getUpdatedWayNodeIdsForThoseNotAllowedToBeRemoved(
  const QSet<long>& nodeIdsNotAllowedToBeRemoved, const QList<long>& nodeIdsBeforeGeneralization,
  const QList<long>& generalizedNodeIds)
{
  LOG_VART(nodeIdsBeforeGeneralization);
  LOG_VART(generalizedNodeIds);

  QList<long> newNodeIds = generalizedNodeIds;

  for (QSet<long>::const_iterator it = nodeIdsNotAllowedToBeRemoved.begin();
       it != nodeIdsNotAllowedToBeRemoved.end(); ++it)
  {
    const int nodeIdToAddBack = *it;
    LOG_VART(nodeIdToAddBack);
    const int originalIndex = nodeIdsBeforeGeneralization.indexOf(nodeIdToAddBack);
    LOG_VART(originalIndex);

    QList<long> originalBefore;
    for (int i = 0; i < originalIndex; i++)
    {
      originalBefore.append(nodeIdsBeforeGeneralization.at(i));
    }
    LOG_VART(originalBefore);

    QList<long> originalAfter;
    for (int i = originalIndex + 1; i < nodeIdsBeforeGeneralization.size(); i++)
    {
      originalAfter.append(nodeIdsBeforeGeneralization.at(i));
    }
    LOG_VART(originalAfter);

    int closestOriginalBeforeIndex = -1;
    for (int i = originalBefore.size() - 1; i >= 0; i--)
    {
      if (generalizedNodeIds.contains(originalBefore.at(i)))
      {
        closestOriginalBeforeIndex = i;
        break;
      }
    }
    if (closestOriginalBeforeIndex == -1)
    {
      closestOriginalBeforeIndex = 0;
    }
    LOG_VART(closestOriginalBeforeIndex);

    int closestOriginalAfterIndex = -1;
    for (int i = 0; i < originalAfter.size(); i++)
    {
      if (generalizedNodeIds.contains(originalAfter.at(i)))
      {
        closestOriginalAfterIndex = i;
        break;
      }
    }
    if (closestOriginalAfterIndex == -1)
    {
      closestOriginalAfterIndex = generalizedNodeIds.size() - 1;
    }
    LOG_VART(closestOriginalAfterIndex);

    assert(closestOriginalBeforeIndex != closestOriginalAfterIndex);
    assert(closestOriginalBeforeIndex < closestOriginalAfterIndex);
    assert((closestOriginalAfterIndex - closestOriginalBeforeIndex) >= 2);
    const int newNodeInsertIndex = closestOriginalBeforeIndex + 1;
    LOG_VART(newNodeInsertIndex);
    newNodeIds.insert(newNodeInsertIndex, nodeIdToAddBack);
  }

  LOG_VART(nodeIdsBeforeGeneralization);
  LOG_VART(generalizedNodeIds);
  LOG_VART(newNodeIds);
  return newNodeIds;
}

QList<std::shared_ptr<const Node>> RdpWayGeneralizer::_getGeneralizedPoints(
  const QList<std::shared_ptr<const Node>>& wayPoints)
{
  //LOG_VART(wayPoints.size());
  if (wayPoints.size() < 3)
  {
    return wayPoints;
  }

  std::shared_ptr<const Node> firstPoint = wayPoints.at(0);
  //LOG_VART(firstPoint->toString());
  std::shared_ptr<const Node> lastPoint = wayPoints.at(wayPoints.size() - 1);
  //LOG_VART(lastPoint->toString());

  int indexOfLargestPerpendicularDistance = -1;
  double largestPerpendicularDistance = 0;
  for (int i = 1; i < wayPoints.size() - 1; i++)
  {
    const double perpendicularDistance =
      _getPerpendicularDistanceBetweenSplitNodeAndImaginaryLine(
        wayPoints.at(i), firstPoint, lastPoint);
    if (perpendicularDistance > largestPerpendicularDistance)
    {
      largestPerpendicularDistance = perpendicularDistance;
      indexOfLargestPerpendicularDistance = i;
    }
  }
  //LOG_VART(largestPerpendicularDistance);
  //LOG_VART(indexOfLargestPerpendicularDistance);
  //LOG_VART(_epsilon);

  if (largestPerpendicularDistance > _epsilon)
  {
    //split the curve into two parts and recursively reduce the two lines
    const QList<std::shared_ptr<const Node>> splitLine1 =
      wayPoints.mid(0, indexOfLargestPerpendicularDistance + 1);
    OsmUtils::printNodes("splitLine1", splitLine1);
    const QList<std::shared_ptr<const Node>> splitLine2 =
      wayPoints.mid(indexOfLargestPerpendicularDistance);
    OsmUtils::printNodes("splitLine2", splitLine2);

    const QList<std::shared_ptr<const Node>> recursivelySplitLine1 =
      _getGeneralizedPoints(splitLine1);
    OsmUtils::printNodes("recursivelySplitLine1", recursivelySplitLine1);
    const QList<std::shared_ptr<const Node>> recursivelySplitLine2 =
      _getGeneralizedPoints(splitLine2);
    OsmUtils::printNodes("recursivelySplitLine2", recursivelySplitLine2);

    //concat r2 to r1 minus the end/start point that will be the same
    QList<std::shared_ptr<const Node>> combinedReducedLines =
      recursivelySplitLine1.mid(0, recursivelySplitLine1.size() - 1);
    combinedReducedLines.append(recursivelySplitLine2);
    OsmUtils::printNodes("combinedReducedLines", combinedReducedLines);
    return combinedReducedLines;
  }
  else
  {
    //reduce the line by remove all points between the first and last points
    QList<std::shared_ptr<const Node>> reducedLine;
    reducedLine.append(firstPoint);
    reducedLine.append(lastPoint);
    OsmUtils::printNodes("reducedLine", reducedLine);
    return reducedLine;
  }
}

double RdpWayGeneralizer::_getPerpendicularDistanceBetweenSplitNodeAndImaginaryLine(
  const std::shared_ptr<const Node> splitPoint,
  const std::shared_ptr<const Node> lineToBeReducedStartPoint,
  const std::shared_ptr<const Node> lineToBeReducedEndPoint) const
{
  //LOG_VART(lineToBeReducedStartPoint->getX());
  //LOG_VART(lineToBeReducedEndPoint->getX());
  double perpendicularDistance;
  if (lineToBeReducedStartPoint->getX() == lineToBeReducedEndPoint->getX())
  {
    perpendicularDistance = abs(splitPoint->getX() - lineToBeReducedStartPoint->getX());
  }
  else
  {
    const double slope =
      (lineToBeReducedEndPoint->getY() - lineToBeReducedStartPoint->getY()) /
      (lineToBeReducedEndPoint->getX() - lineToBeReducedStartPoint->getX());
    //LOG_VART(slope);
    const double intercept =
      lineToBeReducedStartPoint->getY() - (slope * lineToBeReducedStartPoint->getX());
    //LOG_VART(intercept);
    perpendicularDistance =
      abs(slope * splitPoint->getX() - splitPoint->getY() + intercept) / sqrt(pow(slope, 2) + 1);
  }
  //LOG_VART(perpendicularDistance);
  return perpendicularDistance;
}

}
