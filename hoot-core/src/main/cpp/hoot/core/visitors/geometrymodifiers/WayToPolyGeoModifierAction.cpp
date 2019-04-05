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

#include "WayToPolyGeoModifierAction.h"
#include "CoordinateExt.h"

// Hoot
#include <hoot/core/util/Factory.h>

//#include <vector>

using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(GeometryModifierAction, WayToPolyGeoModifierAction)

bool WayToPolyGeoModifierAction::process(const ElementPtr& pElement, OsmMap* pMap) const
{
  // only process Ways
  if( pElement->getElementType() != ElementType::Way ) return false;

  Tags& tags = pElement->getTags();

  // for now use a hardocded filter to get a basic framework going
  if( tags.find("aeroway") == tags.end() ) return false;
  double width = (tags["aeroway"] == "runway") ? 10 : 5;

  // todo: use actual filter and arguments here

  const WayPtr& pWay = boost::dynamic_pointer_cast<Way>(pElement);

  size_t nodeCount = pWay->getNodeCount();

  // too small, nothing to do
  if( nodeCount < 2 ) return false;

  // create poly way and add it to map
  WayPtr pPoly( new Way(Status::Unknown1, pMap->createNextWayId(), -1) );

  // build poly by 'extruding' existing nodes
  const vector<long> nodeIds = pWay->getNodeIds();

  assert(nodeCount == nodeIds.size());

  // create coordinate array with two opposing perpendicular points on each side
  // of original way points plus one more entry for closing the polygon
  vector<Coordinate> polyPositions(nodeCount * 2 + 1);

  for (size_t i = 0; i < nodeCount; i++)
  {
    long currId = nodeIds[i];
    long prevId = (i > 0) ? nodeIds[i-1] : currId;
    long nextId = (i < nodeCount-1) ? nodeIds[i+1] : currId;

    const NodePtr pCurrNode = pMap->getNode(currId);
    const NodePtr pPrevNode = pMap->getNode(prevId);
    const NodePtr pNextNode = pMap->getNode(nextId);

    CoordinateExt currCoor( pCurrNode->toCoordinate() );
    CoordinateExt prevCoor( pPrevNode->toCoordinate() );
    CoordinateExt nextCoor( pNextNode->toCoordinate() );

    // find perpendicular vector to vector between previous and next point
    CoordinateExt c1 = currCoor - prevCoor;
    CoordinateExt c2 = nextCoor - currCoor;
    c1.normalize();
    c2.normalize();
    CoordinateExt vector = c1+c2;
    CoordinateExt perp( vector.y, -vector.x );
    perp.normalize();

    perp.x *= width;
    perp.y *= width;

    polyPositions[i] = currCoor + perp;
    perp.x = -perp.x; perp.y = -perp.y;
    polyPositions[nodeCount*2-1-i] = Coordinate( currCoor.x + perp.x, currCoor.y + perp.y );
  }

  // close poly
  polyPositions[nodeCount*2] = polyPositions[0];

  // add nodes to polygon element
  for( vector<Coordinate>::iterator it = polyPositions.begin(); it != polyPositions.end(); it++)
  {
      long nodeId =pMap->createNextNodeId();
      NodePtr pNode( new Node(Status::Unknown1, nodeId, *it));
      pMap->addElement(pNode);
      pPoly->addNode(nodeId);
  }

  // copy tags from original way to poly way
  pPoly->setTags(pWay->getTags());

  // replace original way with poly
  pMap->replace(pWay,pPoly);

  return true;
}

}
