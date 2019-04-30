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
  double distance = 15;
  ClosePointHash cph(distance);

  foreach (boost::shared_ptr<Polygon> poly, geoms)
  {
    Point *p = poly->getCentroid();
    cph.addPoint(p->getX(), p->getY(), (long)poly->getUserData());
  }

  QList<QList<long>> clusters;

  cph.resetIterator();

  while (cph.next())
  {
    LOG_INFO("Matches");
    const vector<long>& matches = cph.getMatch();

    // see if we already have a cluster with any entry of this match list
    QList<int> clusterIndices;
    foreach (long id, matches)
    {
      LOG_INFO(id);

      for (int i = 0; i < clusters.length(); i++)
      {
        if (clusters[i].contains(id))
        {
          if (!clusterIndices.contains(i)) clusterIndices.push_back(i);
        }
      }
    }

    int clusterIndexCount = clusterIndices.length();
    int clusterIndex;

    if (clusterIndexCount == 0)
    {
      LOG_INFO("make new cluster");
      clusters.push_back(QList<long>());
      clusterIndex = clusters.length()-1;
    }
    else if (clusterIndexCount == 1)
    {
      clusterIndex = clusterIndices[0];
    }
    else
    {
      // merge clusters
      sort(clusterIndices.begin(), clusterIndices.end());
      int destIx = clusterIndices[0];

      for (int i = clusterIndexCount-1; i > 0; i-- )
      {
        int sourceIx = clusterIndices[i];

        foreach (long id, clusters[sourceIx])
        {
          if (!clusters[destIx].contains(id)) clusters[destIx].push_back(id);
        }

        clusters.removeAt(sourceIx);
      }

      clusterIndex = destIx;
    }

    foreach (long id, matches)
    {
      if (!clusters[clusterIndex].contains(id)) clusters[clusterIndex].push_back(id);
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
