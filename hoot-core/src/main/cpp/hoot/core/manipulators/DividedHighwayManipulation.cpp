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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "DividedHighwayManipulation.h"

// GEOS
#include <geos/geom/LineString.h>
#include <geos/geom/Point.h>
#include <geos/operation/distance/DistanceOp.h>
using namespace geos::operation::distance;

// Hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/algorithms/LineStringAverager.h>
#include <hoot/core/algorithms/ProbabilityOfMatch.h>
#include <hoot/core/algorithms/WayHeading.h>
#include <hoot/core/algorithms/linearreference/LocationOfPoint.h>
#include <hoot/core/elements/Node.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/ops/RemoveWayOp.h>
#include <hoot/core/util/ElementConverter.h>
#include <hoot/core/visitors/FindWaysVisitor.h>

// Qt
#include <QDebug>

namespace hoot
{

DividedHighwayManipulation::DividedHighwayManipulation(long leftId, long rightId, long midId,
                                                       shared_ptr<const OsmMap> map,
                                                       Meters vectorError)
{
  _left = leftId;
  _right = rightId;
  _mid = midId;
  _impactedElements.insert(ElementId(ElementType::Way, _left));
  _impactedElements.insert(ElementId(ElementType::Way, _right));
  _impactedElements.insert(ElementId(ElementType::Way, _mid));

  _vectorError = vectorError;
  // over-estimate the final score. This could happen from moving nodes around during other merge
  // operations.
  updateEstimate(map);
}

void DividedHighwayManipulation::_addConnector(shared_ptr<OsmMap> map,
                                               long nodeId) const
{
  vector<long> intersectingWays = FindWaysVisitor::findWaysByNode(map, nodeId);
  
  if (intersectingWays.size() == 1)
  {
   // this is the only way connected to the endpoint, nothing to do.
  }
  else
  {
    // determine the angle between the ways
    shared_ptr<Way> mid = map->getWay(_mid);

    long midNodeIndex = mid->getNodeId(0) == nodeId ? 0 : mid->getNodeCount() - 1;

    for (size_t i = 0; i < intersectingWays.size(); i++)
    {
      // if this is not the input way
      if (intersectingWays[i] != mid->getId())
      {
        shared_ptr<Way> other = map->getWay(intersectingWays[i]);

        if (other->getStatus() == mid->getStatus())
        {
          Radians heading1 = WayHeading::calculateHeading(WayLocation(map, mid, midNodeIndex, 0),
            5.0);
          Radians heading2 = WayHeading::calculateHeading(LocationOfPoint::locate(map, other,
            map->getNode(nodeId)->toCoordinate()), 5.0);

          Radians angle = fabs(heading1 - heading2);
          if (angle > M_PI)
          {
            angle = toRadians(360) - angle;
          }

          if (other->getTags()["hoot:stub"] == "true")
          {
            RemoveWayOp::removeWay(map, other->getId());
          }
          // if the angle is less than 45
          else if (angle < toRadians(45))
          {
            // create two stub connectors
            _createStub(map, map->getWay(_left), nodeId);
            _createStub(map, map->getWay(_right), nodeId);
          }
          // if the angle is 45 or greater
          else
          {
            _mergeInbound(map, other, nodeId);
          }
        }
      }
    }
  }
}

void DividedHighwayManipulation::applyManipulation(shared_ptr<OsmMap> wm,
  set<ElementId>& impactedElements, set<ElementId>& newElements) const
{
  shared_ptr<OsmMap> result = wm;

  shared_ptr<Way> mid = result->getWay(_mid);

  _newWays.clear();

  // add connectors to maintain the topology.
  _addConnector(result, mid->getNodeIds()[0]);
  _addConnector(result, mid->getNodeIds()[mid->getNodeCount() - 1]);

  RemoveWayOp::removeWay(result, _mid);

  result->getWay(_left)->setStatus(Status::Conflated);
  result->getWay(_right)->setStatus(Status::Conflated);

  impactedElements = getImpactedElementIds(wm);
  newElements = _newWays;
}

double DividedHighwayManipulation::calculateProbability(shared_ptr<const OsmMap> map) const
{
  shared_ptr<const Way> left = map->getWay(_left);
  shared_ptr<const Way> right = map->getWay(_right);
  shared_ptr<const Way> mid = map->getWay(_mid);

  ElementConverter ec(map);
  // calculate the center line of two ways.
  shared_ptr<LineString> center = LineStringAverager::average(ec.convertToLineString(left),
    ec.convertToLineString(right));

  // calculate standard deviation in meters
  Meters sd1 = left->getCircularError() / 2.0;
  Meters sd2 = right->getCircularError() / 2.0;
  // calculate variance;
  double v1 = sd1 * sd1;
  double v2 = sd2 * sd2;
  // calculate weights for averaging circularError.
  double weight1 = v1 / (v1 + v2);
  double weight2 = v2 / (v1 + v2);

  double midAccuracy = weight1 * weight1 * v1 + weight2 * weight2 * v2;

  return ProbabilityOfMatch::getInstance().distanceScore(map, mid, center, midAccuracy);
}

double DividedHighwayManipulation::calculateScore(shared_ptr<const OsmMap> map) const
{
  assert(isValid(map));

  ElementConverter ec(map);
  double lLength = ec.calculateLength(map->getWay(_left));
  double rLength = ec.calculateLength(map->getWay(_right));

  _p = calculateProbability(map);

  return _p * (lLength + rLength) * 0.8;
}

void DividedHighwayManipulation::_createStub(shared_ptr<OsmMap> map, shared_ptr<Way> oneway,
                                              long nodeId) const
{
  shared_ptr<Node> node = map->getNode(nodeId);
  Coordinate c = node->toCoordinate();

  shared_ptr<Way> mid = map->getWay(_mid);

  shared_ptr<const Node> endNode;
  NodePtr first = map->getNode(oneway->getNodeId(0));
  NodePtr last = map->getNode(oneway->getLastNodeId());
  double d0 = first->toCoordinate().distance(c);
  double d1 = last->toCoordinate().distance(c);

  if (d0 < d1)
  {
    endNode = first;
  }
  else
  {
    endNode = last;
  }

  Status otherUnknown;
  if (oneway->getStatus() == Status::Unknown1)
  {
    otherUnknown = Status::Unknown2;
  }
  else
  {
    otherUnknown = Status::Unknown1;
  }

  shared_ptr<Way> stub(new Way(otherUnknown, map->createNextWayId(),
                               oneway->getRawCircularError()));
  stub->addNode(endNode->getId());
  stub->addNode(nodeId);
  stub->setTags(mid->getTags());
  stub->setTag("hoot:stub", "true");
  map->addWay(stub);
  _newWays.insert(ElementId::way(stub->getId()));
}

double DividedHighwayManipulation::_dotProduct(const Coordinate& c1, const Coordinate& c2) const
{
  return c1.x * c2.x + c1.y * c2.y;
}

bool DividedHighwayManipulation::isValid(shared_ptr<const OsmMap> map) const
{
  bool result = false;
  if (map->containsWay(_left) && map->containsWay(_right) && map->containsWay(_mid))
  {
    result = map->getWay(_left)->isUnknown();
    result &= map->getWay(_right)->isUnknown();
    result &= map->getWay(_mid)->isUnknown();
  }

  return result;
}

const set<ElementId>& DividedHighwayManipulation::getImpactedElementIds(const ConstOsmMapPtr&) const
{
  return _impactedElements;
}

void DividedHighwayManipulation::_mergeInbound(shared_ptr<OsmMap> map,
  shared_ptr<Way> inbound, long nodeId) const
{
  shared_ptr<Way> left = map->getWay(_left);
  shared_ptr<Way> right = map->getWay(_right);
  shared_ptr<Node> node = map->getNode(nodeId);

  // calculate the normalized vector from nodeId to the nearest end point on left.
  size_t inboundNodeIndex;
  Coordinate vi;
  if (inbound->getNodeId(0) == nodeId)
  {
    inboundNodeIndex = 0;
    vi = WayHeading::calculateVector(map->getNode(inbound->getNodeId(0))->toCoordinate(),
                                     map->getNode(inbound->getNodeId(1))->toCoordinate());
  }
  else
  {
    inboundNodeIndex = inbound->getNodeCount() - 1;
    vi = WayHeading::calculateVector(
      map->getNode(inbound->getNodeId(inboundNodeIndex))->toCoordinate(),
      map->getNode(inbound->getNodeId(inboundNodeIndex - 1))->toCoordinate());
  }

  // calculate the normalized vector from nodeId to the nearest end point on left.
  Coordinate vl = _nearestVector(node->toPoint(), ElementConverter(map).convertToLineString(left));
  // calculate the dot product for the left
  double dpl = _dotProduct(vl, vi);

  // calculate the normalized vector from nodeId to the nearest end point on right.
  Coordinate vr = _nearestVector(node->toPoint(), ElementConverter(map).convertToLineString(right));
  // calculate the dot product for the right
  double dpr = _dotProduct(vr, vi);

  Coordinate ci = node->toCoordinate();
  long endNode;

  // if left has the larger dot product
  if (dpl > dpr)
  {
    NodePtr first = map->getNode(left->getNodeId(0));
    NodePtr last = map->getNode(left->getLastNodeId());

    // connect inbound to the left
    if (first->toCoordinate().distance(ci) <
        last->toCoordinate().distance(ci))
    {
      endNode = first->getId();
    }
    else
    {
      endNode = last->getId();
    }
    inbound->replaceNode(nodeId, endNode);
  }
  // if right has the larger dot product
  else
  {
    NodePtr first = map->getNode(right->getNodeId(0));
    NodePtr last = map->getNode(right->getLastNodeId());

    // connect inbound to the right
    if (first->toCoordinate().distance(ci) <
        last->toCoordinate().distance(ci))
    {
      endNode = first->getId();
    }
    else
    {
      endNode = last->getId();
    }
    inbound->replaceNode(nodeId, endNode);
  }

  if (inbound->getNodeCount() == 2 && inbound->getNodeId(0) == inbound->getLastNodeId())
  {
    RemoveWayOp::removeWay(map, inbound->getId());
  }
}

Coordinate DividedHighwayManipulation::_nearestVector(shared_ptr<Geometry> g1,
                                                        shared_ptr<Geometry> g2) const
{
  Coordinate result;

  CoordinateSequence* cs = DistanceOp::closestPoints(g1.get(), g2.get());

  const Coordinate& c1 = cs->getAt(0);
  const Coordinate& c2 = cs->getAt(1);

  result.x = c2.x - c1.x;
  result.y = c2.y - c1.y;

  double mag = result.distance(Coordinate(0, 0));
  result.x /= mag;
  result.y /= mag;

  delete cs;

  return result;
}

QString DividedHighwayManipulation::toString() const
{
  return QString("DividedHighwayManipulation left: %1 right: %2 mid: %3 score: %4 p: %5").
      arg(_left).arg(_right).arg(_mid).arg(_estimate).arg(_p);
}

}
