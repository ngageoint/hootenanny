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

#include "DualWaySplitter.h"

// GEOS
#include <geos/geom/PrecisionModel.h>
#include <geos/geom/Geometry.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/LineString.h>
#include <geos/geom/Point.h>
#include <geos/operation/buffer/BufferParameters.h>
#include <geos/operation/buffer/BufferBuilder.h>
using namespace geos::geom;
using namespace geos::operation::buffer;

// Hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/algorithms/linearreference/LocationOfPoint.h>
#include <hoot/core/algorithms/Distance.h>
#include <hoot/core/algorithms/WayHeading.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/filters/DistanceNodeCriterion.h>
#include <hoot/core/filters/NotCriterion.h>
#include <hoot/core/filters/ChainCriterion.h>
#include <hoot/core/filters/TagCriterion.h>
#include <hoot/core/util/ElementConverter.h>
#include <hoot/core/util/GeometryUtils.h>
#include <hoot/core/Factory.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/ops/RemoveWayOp.h>
#include <hoot/core/visitors/FindWaysVisitor.h>
#include <hoot/core/visitors/FindNodesVisitor.h>


// Qt
#include <QDebug>

// Standard
#include <limits>
#include <typeinfo>

// TGS
#include <tgs/StreamUtils.h>
using namespace Tgs;

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, DualWaySplitter)

DualWaySplitter::DualWaySplitter()
{
  if (ConfigOptions().getDualWaySplitterDrivingSideDefaultValue().toLower() == "left")
  {
    _drivingSide = DualWaySplitter::Left;
    LOG_INFO("Assuming drives on left.");
  }
  else
  {
    _drivingSide = DualWaySplitter::Right;
    LOG_INFO("Assuming drives on right.");
  }
  _defaultSplitSize = ConfigOptions().getDualWaySplitterSplitSizeDefaultValue();
}

DualWaySplitter::DualWaySplitter(shared_ptr<const OsmMap> map, DrivingSide drivingSide,
  Meters defaultSplitSize)
{
  _map = map;
  _drivingSide = drivingSide;
  _defaultSplitSize = defaultSplitSize;
}

shared_ptr<Way> DualWaySplitter::_createOneWay(shared_ptr<const Way> w, Meters bufferSize,
                                               bool left)
{
  shared_ptr<const LineString> ls = ElementConverter(_result).convertToLineString(w);

  BufferParameters bp(8, BufferParameters::CAP_FLAT, BufferParameters::JOIN_ROUND,
                      bufferSize * 2);

  BufferBuilder bb(bp);
  auto_ptr<Geometry> g(bb.bufferLineSingleSided(ls.get(), bufferSize, left));
  const LineString* newLs = dynamic_cast<const LineString*>(g.get());

  shared_ptr<Way> result(new Way(w->getStatus(), _result->createNextWayId(),
    w->getRawCircularError()));

  // This sometimes happens if the buffer builder returns a multilinestring. See #2275
  if (newLs == 0)
  {
    /// @todo MultiLineString not handled properly See r2275
    LOG_WARN("Inappropriate handling of geometry.");
    auto_ptr<Point> p(GeometryFactory::getDefaultInstance()->createPoint(ls->getCoordinateN(0)));

    auto_ptr<Geometry> unioned(ls->Union(p.get()));
    auto_ptr<Geometry> cleaned(GeometryUtils::validateGeometry(ls.get()));
    auto_ptr<Geometry> buffered(ls->buffer(0));
    LOG_WARN("input geometry: " << ls->toString());
    LOG_WARN("unioned geometry: " << unioned->toString());
    LOG_WARN("cleaned geometry: " << cleaned->toString());
    LOG_WARN("buffered geometry: " << buffered->toString());
    LOG_WARN("output geometry: " << g->toString());

    LOG_WARN("Adding original line back in to keep things moving.");
    const CoordinateSequence* cs = ls->getCoordinatesRO();
    for (size_t i = 0; i < cs->getSize(); i++)
    {
      shared_ptr<Node> n(new Node(w->getStatus(), _result->createNextNodeId(), cs->getAt(i),
        w->getCircularError()));
      _result->addNode(n);
      result->addNode(n->getId());
    }
  }
  else
  {
    const CoordinateSequence* cs = newLs->getCoordinatesRO();
    for (size_t i = 0; i < cs->getSize(); i++)
    {
      shared_ptr<Node> n(new Node(w->getStatus(), _result->createNextNodeId(), cs->getAt(i),
        w->getCircularError()));
      _result->addNode(n);
      result->addNode(n->getId());
    }
  }

  return result;
}

double DualWaySplitter::_dotProduct(const Coordinate& c1, const Coordinate& c2) const
{
  return c1.x * c2.x + c1.y * c2.y;
}

long DualWaySplitter::_nearestNode(long nid, shared_ptr<const Way> w)
{
  shared_ptr<Node> src = _result->getNode(nid);
  const vector<long>& nids = w->getNodeIds();
  Meters best = std::numeric_limits<double>::max();
  long bestNid = -1;
  for (size_t i = 0; i < nids.size(); i++)
  {
    shared_ptr<Node> n = _result->getNode(nids[i]);
    Meters d = Distance::euclidean(src->toCoordinate(), n->toCoordinate());
    if (d < best)
    {
      best = d;
      bestNid = n->getId();
    }
  }

  return bestNid;
}

Coordinate DualWaySplitter::_normalizedVector(long nid1, long nid2)
{
  Coordinate c1 = _result->getNode(nid1)->toCoordinate();
  Coordinate c2 = _result->getNode(nid2)->toCoordinate();

  Coordinate result;

  result.x = c2.x - c1.x;
  result.y = c2.y - c1.y;

  double mag = sqrt(result.x * result.x + result.y * result.y);
  result.x /= mag;
  result.y /= mag;

  return result;
}

bool DualWaySplitter::_onRight(long intersectionId, shared_ptr<Way> inbound, long leftNn,
                               long rightNn)
{
  // calculate the normalized vector from nodeId to the nearest end point on left.
  size_t inboundNodeIndex;
  Coordinate vi;
  if (inbound->getNodeId(0) == intersectionId)
  {
    inboundNodeIndex = 0;
    vi = WayHeading::calculateVector(_result->getNode(inbound->getNodeId(0))->toCoordinate(),
                                     _result->getNode(inbound->getNodeId(1))->toCoordinate());
  }
  else
  {
    inboundNodeIndex = inbound->getNodeCount() - 1;
    vi = WayHeading::calculateVector(
      _result->getNode(inbound->getNodeId(inboundNodeIndex))->toCoordinate(),
      _result->getNode(inbound->getNodeId(inboundNodeIndex - 1))->toCoordinate());
  }

  // calculate the normalized vector from intersectionId to the nearest end point on left.
  Coordinate vl = _normalizedVector(intersectionId, leftNn);
  // calculate the dot product for the left
  double dpl = _dotProduct(vl, vi);


  // calculate the normalized vector from intersectionId to the nearest end point on right.
  Coordinate vr = _normalizedVector(intersectionId, rightNn);
  // calculate the dot product for the right
  double dpr = _dotProduct(vr, vi);

  // if left has the larger dot product
  if (dpl > dpr)
  {
    return false;
  }
  // if right has the larger dot product
  else
  {
    return true;
  }
}

shared_ptr<OsmMap> DualWaySplitter::splitAll(shared_ptr<const OsmMap> map, DrivingSide drivingSide,
                                             Meters defaultSplitSize)
{
  DualWaySplitter dws(map, drivingSide, defaultSplitSize);
  return dws.splitAll();
}

shared_ptr<OsmMap> DualWaySplitter::splitAll()
{
  LOG_INFO("Splitting divided ways...");

  shared_ptr<OsmMap> result(new OsmMap(_map));
  _result = result;

  TagCriterion tagCrit("divider", "yes");
  LOG_DEBUG("  filtering...");

  vector<long> wayIds = FindWaysVisitor::findWays(_result, &tagCrit);
  LOG_DEBUG("  filtered.");

  for (size_t i = 0; i < wayIds.size(); i++)
  {
    if (Log::getInstance().isInfoEnabled())
    {
      cout << "  splitting " << i << " / " << wayIds.size() << "\r";
      cout.flush();
    }
    _splitWay(wayIds[i]);
  }

  if (Log::getInstance().isInfoEnabled())
  {
    cout << endl;
  }

  _result.reset();
  return result;
}

void DualWaySplitter::_addConnector(long /*nodeId*/)
{
//  vector<long> intersectingWays = _result->findWayByNode(nodeId);
//
//  // If this isn't the only way connected to the endpoint.
//  if (intersectingWays.size() > 1)
//  {
//    // get the index of nodeId in _working
//    long workingNodeIndex = _working->getNodeId(0) == nodeId ? 0 : _working->getNodeCount() - 1;
//
//    for (size_t i = 0; i < intersectingWays.size(); i++)
//    {
//      // if this is not the input way
//      if (intersectingWays[i] != _working->getId())
//      {
//        shared_ptr<Way> other = _result->getWay(intersectingWays[i]);
//
//        if (other->getStatus() == _working->getStatus())
//        {
//          Radians heading1 = WayHeading::calculateHeading(WayLocation(_working, midNodeIndex, 0), 5.0);
//          Radians heading2 = WayHeading::calculateHeading(LocationOfPoint::locate(other,
//            _result->getNode(nodeId)->toCoordinate()), 5.0);
//
//          Radians angle = fabs(heading1 - heading2);
//          if (angle > M_PI)
//          {
//            angle = toRadians(360) - angle;
//          }
//
//          if (other->getTags()["hoot:stub"] == "true")
//          {
//            map->removeWay(other);
//          }
//          // if the angle is less than 45
//          else if (angle < toRadians(45))
//          {
//            // create two stub connectors
//            _createStub(map, map->getWay(_left), nodeId);
//            _createStub(map, map->getWay(_right), nodeId);
//          }
//          // if the angle is 45 or greater
//          else
//          {
//            _mergeInbound(map, other, nodeId);
//          }
//        }
//      }
//    }
//  }
}

void DualWaySplitter::_createStub(shared_ptr<Way> /*dividedWay*/, long /*centerNodeId*/,
                                  long /*edgeNodeId*/)
{
//  shared_ptr<Node> node = map->getNode(nodeId);
//  Coordinate c = node->toCoordinate();
//
//  shared_ptr<Way> mid = map->getWay(_mid);
//
//  shared_ptr<const Node> endNode;
//  double d0 = oneway->getNodeN(0)->toCoordinate().distance(c);
//  double d1 = oneway->getLastNode()->toCoordinate().distance(c);
//
//  if (d0 < d1)
//  {
//    endNode = oneway->getNodeN(0);
//  }
//  else
//  {
//    endNode = oneway->getLastNode();
//  }
//
//  Status otherUnknown;
//  if (oneway->getStatus() == Unknown1)
//  {
//    otherUnknown = Unknown2;
//  }
//  else
//  {
//    otherUnknown = Unknown1;
//  }
//
//  shared_ptr<Way> stub(new Way(otherUnknown, OsmMap::createNextWayId(),
//                               oneway->getAccuracy()));
//  stub->addNode(endNode->getId());
//  stub->addNode(nodeId);
//  stub->setTags(_way->getTags());
//  stub->setTag("hoot:stub", "true");
//  map->addWay(stub);
}

void DualWaySplitter::_fixLanes(shared_ptr<Way> w)
{
  QString lanesStr = w->getTags()["lanes"];

  bool ok;
  int lanes = lanesStr.toInt(&ok);

  if (ok)
  {
    if (lanes <= 1)
    {
      w->setTag("lanes", lanesStr);
    }
    else if (lanes % 2 == 0)
    {
      w->setTag("lanes", QString("%1").arg(lanes / 2));
    }
    else
    {
      w->setTag("lanes", QString("%1").arg(std::max(1, lanes / 2)));
      w->getTags().addNote(QString("lane count may be %1 or %2").arg(lanes / 2).arg(lanes / 2 + 1));
    }
  }
}

void DualWaySplitter::_reconnectEnd(long centerNodeId, shared_ptr<Way> edge)
{
  Coordinate centerNodeC = _result->getNode(centerNodeId)->toCoordinate();
  // determine which end of edge we're operating on
  Coordinate edgeEnd;
  long edgeEndId;

  ConstNodePtr first = _result->getNode(edge->getNodeId(0));
  ConstNodePtr last = _result->getNode(edge->getLastNodeId());
  if (first->toCoordinate().distance(centerNodeC) <
      last->toCoordinate().distance(centerNodeC))
  {
    edgeEnd = first->toCoordinate();
    edgeEndId = first->getId();
  }
  else
  {
    edgeEnd = last->toCoordinate();
    edgeEndId = last->getId();
  }

  // find all the nodes that are about the right distance from centerNodeId
  // Find nodes that are > _splitSize*.99 away, but less than _splitSize*1.01
  shared_ptr<DistanceNodeCriterion> outerCrit(new DistanceNodeCriterion(centerNodeC, _splitSize*1.01));
  shared_ptr<NotCriterion> notInnerCrit(new NotCriterion(new DistanceNodeCriterion(centerNodeC, _splitSize * .99)));
  ChainCriterion chainCrit(outerCrit, notInnerCrit);

  vector<long> nids = FindNodesVisitor::findNodes(_result,
                                                  &chainCrit,
                                                  centerNodeC,
                                                  _splitSize * 1.02);

  Radians bestAngle = std::numeric_limits<Radians>::max();
  long bestNid = numeric_limits<long>::max();

  Radians edgeAngle = WayHeading::calculateHeading(centerNodeC, edgeEnd);

  // find the smallest angle from edgeEnd to any of the candidate nodes
  for (size_t i = 0; i < nids.size(); i++)
  {
    if (edgeEndId != nids[i])
    {
      Coordinate c = _result->getNode(nids[i])->toCoordinate();
      Radians thisAngle = WayHeading::calculateHeading(centerNodeC, c);

      Radians deltaMagnitude = WayHeading::deltaMagnitude(edgeAngle, thisAngle);
      if (deltaMagnitude < bestAngle)
      {
        bestAngle = deltaMagnitude;
        bestNid = nids[i];
      }
    }
  }

  // if the smallest angle is less than 45 deg, then connect them.
  if (toDegrees(bestAngle) < 45)
  {
    edge->replaceNode(edgeEndId, bestNid);
  }
}

void DualWaySplitter::_splitIntersectingWays(long nid)
{
  std::vector<long> wids = FindWaysVisitor::findWaysByNode(_result, nid);

  if (wids.size() == 1)
  {
    return;
  }

  long leftNn = _nearestNode(nid, _left);
  long rightNn = _nearestNode(nid, _right);

  for (size_t i = 0; i < wids.size(); i++)
  {
    if (wids[i] != _working->getId())
    {
      shared_ptr<Way> inbound = _result->getWay(wids[i]);
      if (_onRight(nid, inbound, leftNn, rightNn))
      {
        inbound->replaceNode(nid, rightNn);
      }
      else
      {
        inbound->replaceNode(nid, leftNn);
      }
    }
  }
}

void DualWaySplitter::_splitWay(long wid)
{
  _working = _map->getWay(wid);

  if (ElementConverter(_result).convertToLineString(_working)->getLength() <= 0.0)
  {
    return;
  }

  // determine the split size
  _splitSize = _defaultSplitSize / 2.0;
  if (_working->getTags().contains("divider:width"))
  {
    _splitSize = _working->getTags().readMeters("divider:width") / 2.0;
  }

  // Create the two new ways.
  _left = _createOneWay(_working, _splitSize, true);
  _right = _createOneWay(_working, _splitSize, false);

  // set the tags
  _left->setTags(_working->getTags());
  _right->setTags(_working->getTags());

  // remove the old divider tags.
  _left->removeTag("divider");
  _left->removeTag("divider:width");
  _right->removeTag("divider");
  _right->removeTag("divider:width");

  _fixLanes(_left);
  _fixLanes(_right);

  if (_drivingSide == Right)
  {
    _left->reverseOrder();
    _right->reverseOrder();
  }

  _left->setTag("oneway", "yes");
  _right->setTag("oneway", "yes");

  // go through each node on the old way
  const vector<long>& nids = _working->getNodeIds();
  for (size_t i = 0; i < nids.size(); i++)
  {
    // If there are intersecting ways, split the way into two smaller ways
    _splitIntersectingWays(nids[i]);
  }

  _reconnectEnd(nids[0], _left);
  _reconnectEnd(nids[0], _right);
  _reconnectEnd(nids[nids.size() - 1], _left);
  _reconnectEnd(nids[nids.size() - 1], _right);

  RemoveWayOp::removeWay(_result, wid);

  // add the results to the map
  _result->addWay(_left);
  _result->addWay(_right);
}

void DualWaySplitter::apply(shared_ptr<OsmMap>& map)
{
  map = splitAll(map, _drivingSide, _defaultSplitSize);
}

}
