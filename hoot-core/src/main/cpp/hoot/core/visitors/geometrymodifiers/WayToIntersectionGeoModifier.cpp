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

#include "WayToIntersectionGeoModifier.h"

// geos
#include <geos/geom/Envelope.h>

// Hoot
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/util/CoordinateExt.h>
#include <hoot/core/util/Factory.h>

using namespace geos::geom;
using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(GeometryModifierAction, WayToIntersectionGeoModifier)

bool WayToIntersectionGeoModifier::process( const ElementPtr& pElement, OsmMap* pMap )
{
  // only process ways
  if( pElement->getElementType() != ElementType::Way ) return false;
  const WayPtr pWay = boost::dynamic_pointer_cast<Way>(pElement);

  // find envelope of nodes
  boost::shared_ptr<Envelope> pEnv( pWay->getEnvelope(pMap->shared_from_this()) );

  // find intersecting ways
  vector<long> intersectIds = pMap->getIndex().findWays(*pEnv);

  long currNodeId = pWay->getId();
  long myNodeCount = pWay->getNodeCount();
  vector<long> myNodeIds = pWay->getNodeIds();

  for( vector<long>::iterator it = intersectIds.begin(); it != intersectIds.end(); it++ )
  {
    WayPtr pInters = pMap->getWay(*it);

    long interNodeCount = pInters->getNodeCount();
    vector<long> interNodeIds = pInters->getNodeIds();

    for( int myNodeIx = 0; myNodeIx < myNodeCount-1; myNodeIx++ )
    {
      long p1Id = myNodeIds[myNodeIx];
      long p2Id = myNodeIds[myNodeIx+1];
      CoordinateExt myP1( pMap->getNode(p1Id)->toCoordinate());
      CoordinateExt myP2( pMap->getNode(p2Id)->toCoordinate());

      for( int interNodeIx = 0; interNodeIx < interNodeCount-1; interNodeIx++ )
      {
        long i1Id = interNodeIds[interNodeIx];
        long i2Id = interNodeIds[interNodeIx+1];

        // don't check if the segments share any nodes
        if( i1Id == p1Id || i1Id == p2Id || i2Id == p1Id || i2Id == p2Id ) continue;

        CoordinateExt interP1( pMap->getNode(i1Id)->toCoordinate());
        CoordinateExt interP2( pMap->getNode(i2Id)->toCoordinate());

        boost::shared_ptr<CoordinateExt> pInters = CoordinateExt::lineSegementsIntersect(myP1, myP2, interP1, interP2);

        if( pInters )
        {
          LOG_INFO("      INTERS: " << pInters->x)
          NodePtr pNode( new Node(Status::Unknown1, pMap->createNextNodeId(), *pInters) );
          pNode->getTags()["TEST"]="INTERSECTION";
          pMap->addNode(pNode);

          // todo: do something with the original bridge
        }
      }
    }
  }

  return true;
}

}
