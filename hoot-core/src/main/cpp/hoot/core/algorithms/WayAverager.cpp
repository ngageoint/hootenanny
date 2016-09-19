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

#include "WayAverager.h"

// GEOS
#include <geos/geom/CoordinateSequence.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/LineString.h>
#include <geos/geom/Point.h>
#include <geos/operation/distance/DistanceOp.h>
using namespace geos::operation::distance;

// Hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/algorithms/DirectionFinder.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/schema/TagComparator.h>
#include <hoot/core/schema/TagMergerFactory.h>
#include <hoot/core/util/ElementConverter.h>
#include <hoot/core/ops/RemoveWayOp.h>

namespace hoot
{

WayAverager::WayAverager(OsmMapPtr map, shared_ptr<Way> w1, shared_ptr<Way> w2) :
    _map(*map)
{
  if (w1->getStatus() == Status::Unknown2 && w2->getStatus() == Status::Unknown1)
  {
    _w2 = w1;
    _w1 = w2;
  }
  else
  {
    _w1 = w1;
    _w2 = w2;
  }
}

shared_ptr<Way> WayAverager::average()
{
  _sumMovement1 = 0.0;
  _sumMovement2 = 0.0;
  _moveCount1 = 0;
  _moveCount2 = 0;
  _maxMovement1 = 0.0;
  _maxMovement2 = 0.0;

  if (DirectionFinder::isSimilarDirection(_map.shared_from_this(), _w1, _w2) == false)
  {
    if (OsmSchema::getInstance().isOneWay(*_w1) == true)
    {
      _w2->reverseOrder();
    }
    else
    {
      _w1->reverseOrder();
    }
  }

  shared_ptr<const LineString> ls1 = ElementConverter(_map.shared_from_this()).
      convertToLineString(_w1);
  shared_ptr<const LineString> ls2 = ElementConverter(_map.shared_from_this()).
      convertToLineString(_w2);

  // All of the fancy stats here are compliments of Mike Porter.

  // calculate standard deviation in meters
  Meters sd1 = _w1->getCircularError() / 2.0;
  Meters sd2 = _w2->getCircularError() / 2.0;
  // calculate variance;
  double v1 = sd1 * sd1;
  double v2 = sd2 * sd2;
  // calculate weights for averaging.
  double weight1 = 1 - v1 / (v1 + v2);
  double weight2 = 1 - v2 / (v1 + v2);

  Meters newAcc = 2.0 * sqrt(weight1 * weight1 * v1 + weight2 * weight2 * v2);

  shared_ptr<Way> result(new Way(_w1->getStatus(), _map.createNextWayId(),
                                 newAcc));

  _map.addWay(result);

  result->addNode(_merge(_w1->getNodeIds()[0], weight1, _w2->getNodeIds()[0], weight2));

  // we're getting the vectors affter the above merge because the merge will change node ids.
  const std::vector<long>& ns1 = _w1->getNodeIds();
  const std::vector<long>& ns2 = _w2->getNodeIds();

  size_t i1 = 1;
  size_t i2 = 1;

  // while there is more than one point available in each line
  while (i1 < ns1.size() - 1 || i2 < ns2.size() - 1)
  {
    // if we're all out of cs1 points
    if (i1 == ns1.size() - 1)
    {
      result->addNode(_moveToLine(ns2[i2++], weight2, ls1.get(), weight1, 2));
    }
    // if we're all out of cs2 points
    else if (i2 == ns2.size() - 1)
    {
      result->addNode(_moveToLine(ns1[i1++], weight1, ls2.get(), weight2, 1));
    }
    else
    {
      const Coordinate& last = _map.getNode(result->getLastNodeId())->toCoordinate();

      const Coordinate& nc1 = _moveToLineAsCoordinate(ns1[i1], weight1, ls2.get(), weight2);
      const Coordinate& nc2 = _moveToLineAsCoordinate(ns2[i2], weight2, ls1.get(), weight1);

      if (nc1.distance(last) < nc2.distance(last))
      {
        result->addNode(_moveToLine(ns1[i1++], weight1, ls2.get(), weight2, 1));
      }
      else
      {
        result->addNode(_moveToLine(ns2[i2++], weight2, ls1.get(), weight1, 2));
      }
    }
  }

  // merge the last two nodes and move to the average location
  result->addNode(_merge(ns1[i1], weight1, ns2[i2], weight2));

  // use the default tag merging mechanism
  Tags tags = TagMergerFactory::mergeTags(_w1->getTags(), _w2->getTags(), ElementType::Way);

  result->setTags(tags);

  RemoveWayOp::removeWay(_map.shared_from_this(), _w1->getId());
  RemoveWayOp::removeWay(_map.shared_from_this(), _w2->getId());

  _meanMovement1 = _sumMovement1 / (double)_moveCount1;
  _meanMovement2 = _sumMovement2 / (double)_moveCount2;

  return result;
}

shared_ptr<Way> WayAverager::average(OsmMapPtr map, shared_ptr<Way> w1, shared_ptr<Way> w2)
{
  WayAverager wa(map, w1, w2);
  return wa.average();
}

long WayAverager::_merge(long ni1, double weight1, long ni2, double weight2)
{
  shared_ptr<Node> n1 = _map.getNode(ni1);
  shared_ptr<Node> n2 = _map.getNode(ni2);

  Meters d = n1->toCoordinate().distance(n2->toCoordinate());

  _sumMovement1 += d * weight2;
  _moveCount1++;
  _maxMovement2 = max(_maxMovement2, d * weight2);

  _sumMovement2 += d * weight1;
  _moveCount2++;
  _maxMovement2 = max(_maxMovement2, d * weight1);

  shared_ptr<Node> node(new Node(Status::Conflated, _map.createNextNodeId(),
                                 n1->getX() * weight1 + n2->getX() * weight2,
                                 n1->getY() * weight1 + n2->getY() * weight2,
                                 std::min(n1->getCircularError(), n2->getCircularError())));

  _map.addNode(node);
  // replace all instances of n1/n2 with node.
  _map.replaceNode(n1->getId(), node->getId());
  _map.replaceNode(n2->getId(), node->getId());

  return node->getId();
}

long WayAverager::_moveToLine(long ni, double nWeight, const LineString* ls, double lWeight,
  int w1OrW2)
{
  shared_ptr<Node> n = _map.getNode(ni);
  Coordinate c = _moveToLineAsCoordinate(ni, nWeight, ls, lWeight);

  Meters d = c.distance(n->toCoordinate());
  if (w1OrW2 == 1)
  {
    _sumMovement1 += d;
    _moveCount1++;
    _maxMovement1 = max(_maxMovement1, d);
  }
  else
  {
    _sumMovement2 += d;
    _moveCount2++;
    _maxMovement2 = max(_maxMovement2, d);
  }


  n->setX(c.x);
  n->setY(c.y);

  return ni;
}

Coordinate WayAverager::_moveToLineAsCoordinate(long ni, double nWeight, const LineString* ls,
                                                double lWeight)
{
  shared_ptr<Node> n = _map.getNode(ni);
  Point* point(GeometryFactory::getDefaultInstance()->createPoint(n->toCoordinate()));

  // find the two closest points
  CoordinateSequence* cs = DistanceOp::closestPoints(point, const_cast<LineString*>(ls));

  Coordinate result = Coordinate(cs->getAt(0).x * nWeight + cs->getAt(1).x * lWeight,
                                 cs->getAt(0).y * nWeight + cs->getAt(1).y * lWeight);

  delete cs;
  delete point;

  return result;
}

}
