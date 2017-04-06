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
 * @copyright Copyright (C) 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "RdpWayGeneralizer.h"

// Standard
#include <cmath>

// Hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/elements/Node.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/util/OsmUtils.h>

namespace hoot
{

RdpWayGeneralizer::RdpWayGeneralizer(double epsilon)
{
  setEpsilon(epsilon);
}

RdpWayGeneralizer::RdpWayGeneralizer(boost::shared_ptr<OsmMap> map, double epsilon) :
_map(map)
{
  setEpsilon(epsilon);
}

void RdpWayGeneralizer::generalize(boost::shared_ptr<Way> way)
{
  if (!_map.get())
  {
    throw HootException("No map passed to way generalizer.");
  }

  LOG_VART(way->getNodeIds().size());
  LOG_VART(QVector<long>::fromStdVector(way->getNodeIds()).toList());

  const QList<long> wayNodeIdsBeforeFiltering =
    QVector<long>::fromStdVector(way->getNodeIds()).toList();
  LOG_VART(wayNodeIdsBeforeFiltering.size());
  LOG_VART(wayNodeIdsBeforeFiltering);

  //filter the nodes to be generalized to those in this way and those with no information tags;
  //tried using hoot filters here at first, but it didn't end up making sense
  QList<long> wayNodeIdsAfterFiltering;
  for (QList<long>::const_iterator it = wayNodeIdsBeforeFiltering.begin();
       it != wayNodeIdsBeforeFiltering.end(); it++)
  {
    if (_map->getNode(*it)->getTags().getInformationCount() == 0)
    {
      wayNodeIdsAfterFiltering.append(*it);
    }
  }
  LOG_VART(wayNodeIdsAfterFiltering.size());
  LOG_VART(wayNodeIdsAfterFiltering);

  //get the generalized points
  const QList<boost::shared_ptr<const Node> >& generalizedPoints =
    getGeneralizedPoints(OsmUtils::nodeIdsToNodes(wayNodeIdsAfterFiltering, _map));
  LOG_VART(generalizedPoints.size());
  OsmUtils::printNodes("generalizedPoints", generalizedPoints);

  //replace the current nodes on the way with the reduced collection
  way->setNodes(OsmUtils::nodesToNodeIds(generalizedPoints).toVector().toStdVector());
  LOG_VART(way->getNodeIds().size());
  LOG_VART(QVector<long>::fromStdVector(way->getNodeIds()).toList());
  LOG_VART(_map->getWay(way->getId())->getNodeIds().size());
  LOG_VART(QVector<long>::fromStdVector(_map->getWay(way->getId())->getNodeIds()).toList());
}

QList<boost::shared_ptr<const Node> > RdpWayGeneralizer::getGeneralizedPoints(
  const QList<boost::shared_ptr<const Node> >& wayPoints)
{
  LOG_VART(wayPoints.size());
  if (wayPoints.size() < 3)
  {
    return wayPoints;
  }

  boost::shared_ptr<const Node> firstPoint = wayPoints.at(0);
  LOG_VART(firstPoint->toString());
  boost::shared_ptr<const Node> lastPoint = wayPoints.at(wayPoints.size() - 1);
  LOG_VART(lastPoint->toString());

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
  LOG_VART(largestPerpendicularDistance);
  LOG_VART(indexOfLargestPerpendicularDistance);
  LOG_VART(_epsilon);

  if (largestPerpendicularDistance > _epsilon)
  {
    //split the curve into two parts and recursively reduce the two lines
    const QList<boost::shared_ptr<const Node> > splitLine1 =
      wayPoints.mid(0, indexOfLargestPerpendicularDistance + 1);
    OsmUtils::printNodes("splitLine1", splitLine1);
    const QList<boost::shared_ptr<const Node> > splitLine2 =
      wayPoints.mid(indexOfLargestPerpendicularDistance);
    OsmUtils::printNodes("splitLine2", splitLine2);

    const QList<boost::shared_ptr<const Node> > recursivelySplitLine1 =
      getGeneralizedPoints(splitLine1);
    OsmUtils::printNodes("recursivelySplitLine1", recursivelySplitLine1);
    const QList<boost::shared_ptr<const Node> > recursivelySplitLine2 =
      getGeneralizedPoints(splitLine2);
    OsmUtils::printNodes("recursivelySplitLine2", recursivelySplitLine2);

    //concat r2 to r1 minus the end/start point that will be the same
    QList<boost::shared_ptr<const Node> > combinedReducedLines =
      recursivelySplitLine1.mid(0, recursivelySplitLine1.size() - 1);
    combinedReducedLines.append(recursivelySplitLine2);
    OsmUtils::printNodes("combinedReducedLines", combinedReducedLines);
    return combinedReducedLines;
  }
  else
  {
    //reduce the line by remove all points between the first and last points
    QList<boost::shared_ptr<const Node> > reducedLine;
    reducedLine.append(firstPoint);
    reducedLine.append(lastPoint);
    OsmUtils::printNodes("reducedLine", reducedLine);
    return reducedLine;
  }
}

double RdpWayGeneralizer::_getPerpendicularDistanceBetweenSplitNodeAndImaginaryLine(
  const boost::shared_ptr<const Node> splitPoint, const boost::shared_ptr<const Node> lineToBeReducedStartPoint,
  const boost::shared_ptr<const Node> lineToBeReducedEndPoint) const
{
  LOG_VART(lineToBeReducedStartPoint->getX());
  LOG_VART(lineToBeReducedEndPoint->getX());
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
    LOG_VART(slope);
    const double intercept =
      lineToBeReducedStartPoint->getY() - (slope * lineToBeReducedStartPoint->getX());
    LOG_VART(intercept);
    perpendicularDistance =
      abs(slope * splitPoint->getX() - splitPoint->getY() + intercept) / sqrt(pow(slope, 2) + 1);
  }
  LOG_VART(perpendicularDistance);
  return perpendicularDistance;
}

}
