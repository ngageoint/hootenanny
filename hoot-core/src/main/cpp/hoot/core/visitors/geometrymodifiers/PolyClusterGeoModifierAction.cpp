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
 * @copyright Copyright (C) 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "PolyClusterGeoModifierAction.h"

// Hoot
#include <hoot/core/elements/ElementConverter.h>
#include <hoot/core/ops/RemoveNodeOp.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/CoordinateExt.h>


// Geos
#include <geos/geom/GeometryFactory.h>

// std
#include <algorithm>

using namespace std;
using namespace geos::geom;

namespace hoot
{

HOOT_FACTORY_REGISTER(GeometryModifierAction, PolyClusterGeoModifierAction)

void PolyClusterGeoModifierAction::parseArguments(const QHash<QString, QString>&)
{
}

void PolyClusterGeoModifierAction::processStart(boost::shared_ptr<OsmMap>& )
{
  _ways.clear();  
}

bool PolyClusterGeoModifierAction::processElement( const ElementPtr& pElement, OsmMap* )
{
  // only process closed area ways
  if (pElement->getElementType() != ElementType::Way) return false;
  const WayPtr& pWay = boost::dynamic_pointer_cast<Way>(pElement);
  if (!pWay->isClosedArea()) return false;

  // store for use in processFinalize
  _ways.push_back(pWay);
  return true;
}

void PolyClusterGeoModifierAction::processFinalize(boost::shared_ptr<OsmMap>& pMap)
{
  LOG_INFO( "finalizing " << _ways.length() << " ways");

  _clearProcessData();

  ElementConverter elementConverter(pMap);

  // create a polygon from each building/way
  _polys.clear();

  QHash<long,boost::shared_ptr<Polygon>> polyLookup;

  foreach (WayPtr pWay, _ways)
  {
    boost::shared_ptr<Polygon> pPoly = elementConverter.convertToPolygon(pWay);
    long wayId = pWay->getId();
    // set id as user data
    pPoly->setUserData((void*)wayId);
    polyLookup[wayId] = pPoly;
    _polys.push_back(pPoly);
  }

  // get clusters
  _generateClusters();

  LOG_INFO( "Generated " << _clusters.length() << " clusters.");

  // draw debug way around each cluster
  foreach (QList<long> cluster, _clusters)
  {
    boost::shared_ptr<Geometry> pCombinedPoly = boost::shared_ptr<Polygon>(GeometryFactory::getDefaultInstance()->createPolygon());

    foreach (long wayId, cluster)
    {
      pCombinedPoly = boost::shared_ptr<Geometry>(pCombinedPoly->Union(polyLookup[wayId].get()));
    }

    // create hull
    Geometry* pHull = pCombinedPoly->convexHull();
    CoordinateSequence* pHullCoords = pHull->getCoordinates();

    WayPtr pDebugWay( new Way(Status::Unknown1, pMap->createNextWayId()));
    pDebugWay->getTags()["Test"] = "Debug";
    pMap->addElement(pDebugWay);

    for (size_t i = 0; i < pHullCoords->size(); i++)
    {
      Coordinate pos = pHullCoords->getAt(i);
      NodePtr pNode( new Node(Status::Unknown1, pMap->createNextNodeId(), pos) );
      pDebugWay->addNode(pNode->getId());
      pMap->addElement(pNode);
    }
  }  

  // cleanup
  _clearProcessData();
}

void PolyClusterGeoModifierAction::_clearProcessData()
{
  _processedPolys.clear();
  _clusters.clear();
  _coordinateByNodeIx.clear();
  _polyByWayId.clear();
  _pClosePointHash.reset();
}

void PolyClusterGeoModifierAction::_generateClusters()
{   
  _distanceSquared = _distance * _distance;

  // build the ClosePointHash
  _pClosePointHash = boost::shared_ptr<ClosePointHash>(new ClosePointHash(_distance));

  // gather coordinates and build lookups
  foreach (boost::shared_ptr<Polygon> poly, _polys)
  {
    long wayId = (long)poly->getUserData();
    _polyByWayId[wayId] = poly;
    CoordinateSequence* pCoords = poly->getCoordinates();
    int coordCount = min((int)pCoords->size(), MAX_PROCESSED_NODES_PER_POLY-1);

    for (int i = 0; i < coordCount; i++)
    {
      CoordinateExt c = pCoords->getAt(i);
      // The 'node index' calculated here is used just for processing here with the ClosePointHash
      // and based on the way id. It is not related to the actual node id.
      long nodeIndex = wayId * MAX_PROCESSED_NODES_PER_POLY + i;
      _pClosePointHash->addPoint(c.x, c.y, nodeIndex);
      _coordinateByNodeIx[nodeIndex] = c;
    }
  }

  // recursively build clusters
  foreach (boost::shared_ptr<Polygon> poly, _polys)
  {
    long wayId = (long)poly->getUserData();
    if( _processedPolys.contains(wayId) ) continue;

    _clusters.push_back(QList<long>());
    _clusterIndex = _clusters.length()-1;

    _recursePolygons(poly);
  }

  // show clusters for debug
  foreach (QList<long> cluster, _clusters)
  {
    LOG_INFO("Cluster");
    foreach (long id, cluster)
    {
      LOG_INFO(id);
    }
  }
}

void PolyClusterGeoModifierAction::_recursePolygons(const boost::shared_ptr<Polygon>& poly)
{
  long thisWayId = (long)poly->getUserData();
  assert(_processedPolys.contains(thisWayId) == false);

  // add this poly to current cluster and mark as processed
  _clusters[_clusterIndex].push_back(thisWayId);
  _processedPolys.push_back(thisWayId);

  // go through each node and find its matches
  int coordCount = min((int)poly->getNumPoints(), MAX_PROCESSED_NODES_PER_POLY-1);

  for (int i = 0; i < coordCount; i++)
  {
    long thisNodeIndex = thisWayId * MAX_PROCESSED_NODES_PER_POLY + i;
    CoordinateExt thisCoord = _coordinateByNodeIx[thisNodeIndex];
    vector<long> matches = _pClosePointHash->getMatchesFor(thisNodeIndex);

    foreach (long otherNodeIndex, matches)
    {
      long otherWayId = otherNodeIndex / MAX_PROCESSED_NODES_PER_POLY;

      // if the match is from another poly and within distance, process it
      if (otherWayId != thisWayId &&
         !_processedPolys.contains(otherWayId) &&
         (thisCoord-_coordinateByNodeIx[otherNodeIndex]).lengthSquared() <= _distanceSquared )
      {
        _recursePolygons(_polyByWayId[otherWayId]);
      }
    }
  }
}

}
