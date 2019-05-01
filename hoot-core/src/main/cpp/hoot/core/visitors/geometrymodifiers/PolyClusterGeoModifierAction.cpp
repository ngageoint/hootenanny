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
#include <hoot/core/index/ClosePointHash.h>

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

  ElementConverter elementConverter(pMap);

  // create a polygon from each building/way
  QList<boost::shared_ptr<Polygon>> geoms;
  QHash<long,boost::shared_ptr<Polygon>> polyLookup;

  foreach (WayPtr pWay, _ways)
  {
    boost::shared_ptr<Polygon> pPoly = elementConverter.convertToPolygon(pWay);
    long wayId = pWay->getId();
    // set id as user data
    pPoly->setUserData((void*)wayId);
    polyLookup[wayId] = pPoly;
    geoms.push_back(pPoly);
  }

  // get clusters
  QList<QList<long>> clusters = _generateClusters(geoms);

  // draw debug way around each cluster
  foreach (QList<long> cluster, clusters)
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
}

QList<QList<long>> PolyClusterGeoModifierAction::_generateClusters(const QList<boost::shared_ptr<Polygon>>& geoms)
{
  const int MAX_PROCESSED_NODES_PER_POLY = 100;
  double distance = 17;
  double distanceSquared = distance * distance;

  // build the ClosePointHash
  ClosePointHash cph(distance);
  QHash<long, CoordinateExt> coordinateByNodeIx;

  foreach (boost::shared_ptr<Polygon> poly, geoms)
  {
    long wayId = (long)poly->getUserData();
    CoordinateSequence* pCoords = poly->getCoordinates();
    int coordCount = min((int)pCoords->size(), MAX_PROCESSED_NODES_PER_POLY-1);

    for (int i = 0; i < coordCount; i++)
    {
      CoordinateExt c = pCoords->getAt(i);
      // The 'node index' calculated here is used just for processing here with the ClosePointHash
      // and based on the way id. It is not related to the actual node id.
      long nodeIndex = wayId * MAX_PROCESSED_NODES_PER_POLY + i;
      cph.addPoint(c.x, c.y, nodeIndex);
      coordinateByNodeIx[nodeIndex] = c;
    }
  }

  QList<QList<long>> clusters;
  QHash<long,int> clusterIndexByWayId;

  // find matches by building, checking each individual building node
  foreach (boost::shared_ptr<Polygon> poly, geoms)
  {
    long thisWayId = (long)poly->getUserData();
    CoordinateSequence* pCoords = poly->getCoordinates();
    int coordCount = min((int)pCoords->size(), MAX_PROCESSED_NODES_PER_POLY-1);

    // create new cluster if needed
    int clusterIndex = -1;

    if(!clusterIndexByWayId.contains(thisWayId))
    {
      clusters.push_back(QList<long>());
      clusterIndex = clusters.length()-1;
      clusterIndexByWayId[thisWayId] = clusterIndex;
      // enter this building in new cluster
      clusters[clusterIndexByWayId[thisWayId]].push_back(thisWayId);
    }
    else
    {
      clusterIndex = clusterIndexByWayId[thisWayId];
    }

    for (int i = 0; i < coordCount; i++)
    {
      long thisNodeIndex = thisWayId * MAX_PROCESSED_NODES_PER_POLY + i;
      CoordinateExt thisCoord = coordinateByNodeIx[thisNodeIndex];
      vector<long> matches = cph.getMatchesFor(thisNodeIndex);

      foreach (long otherNodeIndex, matches)
      {
        long otherWayId = otherNodeIndex / MAX_PROCESSED_NODES_PER_POLY;

        if (otherWayId == thisWayId || (thisCoord-coordinateByNodeIx[otherNodeIndex]).lengthSquared() <= distanceSquared )
        {
          if (!clusters[clusterIndex].contains(otherWayId)) clusters[clusterIndex].push_back(otherWayId);
          if (!clusterIndexByWayId.contains(otherWayId)) clusterIndexByWayId[otherWayId] = clusterIndex;
        }
      }
    }
  }

  foreach (QList<long> cluster, clusters)
  {
    LOG_INFO("Cluster");
    foreach (long id, cluster)
    {
      LOG_INFO(id);
    }
  }

  return clusters;
}

}
