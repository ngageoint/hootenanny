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

#include "GeometryModifierVisitor.h"

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>

using namespace geos::geom;
using namespace std;
using namespace boost;

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, GeometryModifierVisitor)

GeometryModifierVisitor::GeometryModifierVisitor()
{
}

void GeometryModifierVisitor::visit(const ElementPtr& pElement)
{
  _numProcessed++;

  // only process Ways
  if( pElement->getElementType() != ElementType::Way ) return;

  Tags& tags = pElement->getTags();

  // for now use a hardocded filter to get a basic framework going
  if( tags.find("aeroway") == tags.end() ) return;

  if( tags["aeroway"] == "runway")
  {
    extrude( boost::dynamic_pointer_cast<Way>(pElement));

    tags["Test"] = "GeometryModifierVisitor";
    _numAffected++;
  }
}

double getLength( const Coordinate& c )
{
  return sqrt(c.x * c.x + c.y * c.y);
}

void normalize( Coordinate& c )
{
  double len = getLength(c);
  if( len == 0 ) return;
  c.x /= len;
  c.y /= len;
}

void GeometryModifierVisitor::extrude(const WayPtr& pWay)
{
  size_t nodeCount = pWay->getNodeCount();

  // too small, nothing to do
  if( nodeCount < 2 ) return;

  // create poly way and add it to map
  WayPtr pPoly( new Way(Status::Unknown1, _pMap->createNextWayId(), -1) );
  pPoly->setTag("name", "GeneratedPoly");
  _pMap->addElement(pPoly);

  // build poly by extruding existing nodes
  const vector<long> nodeIds = pWay->getNodeIds();
  //bool isLoop = pWay->isSimpleLoop();

  assert(nodeCount == nodeIds.size());

  // create coordinate array with two opposing perpendicular points on each side
  // of original way points plus one more entry for closing the polygon
  vector<Coordinate> polyPositions(nodeCount * 2 + 1);

  for (size_t i = 0; i < nodeCount; i++)
  {
    long currId = nodeIds[i];
    long prevId = (i > 0) ? nodeIds[i-1] : currId;
    long nextId = (i < nodeCount-1) ? nodeIds[i+1] : currId;

    const NodePtr pCurrNode = _pMap->getNode(currId);
    const NodePtr pPrevNode = _pMap->getNode(prevId);
    const NodePtr pNextNode = _pMap->getNode(nextId);

    Coordinate currCoor( pCurrNode->toCoordinate() );
    Coordinate prevCoor( pPrevNode->toCoordinate() );
    Coordinate nextCoor( pNextNode->toCoordinate() );

    Coordinate vector( nextCoor.x - prevCoor.x, nextCoor.y - prevCoor.y );
    Coordinate perp( vector.y, -vector.x );
    normalize(perp);

    const double width = .00005;

    perp.x *= width;
    perp.y *= width;

    polyPositions[i] = Coordinate( currCoor.x + perp.x, currCoor.y + perp.y );
    perp.x = -perp.x; perp.y = -perp.y;
    polyPositions[nodeCount*2-1-i] = Coordinate( currCoor.x + perp.x, currCoor.y + perp.y );
  }

  // close poly
  polyPositions[nodeCount*2] = polyPositions[0];

  // add nodes to polygon element
  for( vector<Coordinate>::iterator it = polyPositions.begin(); it != polyPositions.end(); it++)
  {
      long nodeId =_pMap->createNextNodeId();
      NodePtr pNode( new Node(Status::Unknown1, nodeId, *it));
      _pMap->addElement(pNode);
      pPoly->addNode(nodeId);
  }

  LOG_VAR(nodeCount);
}

} // namespace hoot
