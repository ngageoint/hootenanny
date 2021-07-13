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
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#include "PolyClusterGeoModifierAction.h"

// Hoot
#include <hoot/core/algorithms/alpha-shape/AlphaShape.h>
#include <hoot/core/geometry/CoordinateExt.h>
#include <hoot/core/geometry/ElementToGeometryConverter.h>
#include <hoot/core/geometry/GeometryToElementConverter.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/ops/RemoveNodeByEid.h>
#include <hoot/core/ops/RemoveWayByEid.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/visitors/WorstCircularErrorVisitor.h>

// Geos
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/LineString.h>
#include <geos/operation/union/UnaryUnionOp.h>

// std
#include <algorithm>

using namespace std;
using namespace geos::geom;

namespace hoot
{

HOOT_FACTORY_REGISTER(GeometryModifierAction, PolyClusterGeoModifierAction)

const QString PolyClusterGeoModifierAction::DISTANCE_PARAM = "distance";
const QString PolyClusterGeoModifierAction::ALPHA_PARAM = "alpha";
const QString PolyClusterGeoModifierAction::REMOVE_POLYS_PARAM = "remove_polys";
const QString PolyClusterGeoModifierAction::CHECK_INTERSECTIONS_PARAM = "check_intersections";
const QString PolyClusterGeoModifierAction::CLUSTER_TAG_LIST_PARAM = "cluster_tag_list";

void PolyClusterGeoModifierAction::parseArguments(const QHash<QString, QString>& arguments)
{
  _distance = DEFAULT_DISTANCE;
  _alpha = DEFAULT_ALPHA;
  _removePolys = DEFAULT_REMOVE_POLYS;
  _checkIntersections = DEFAULT_CHECK_INTERSECTIONS;
  _clusterTagList = DEFAULT_CLUSTER_TAG_LIST;
  _clusterTags.clear();

  if (arguments.keys().contains(DISTANCE_PARAM))
  {
    _distance = arguments[DISTANCE_PARAM].toDouble();
  }

  if (arguments.keys().contains(ALPHA_PARAM))
  {
    _alpha = arguments[ALPHA_PARAM].toDouble();
  }

  if (arguments.keys().contains(REMOVE_POLYS_PARAM))
  {
    _removePolys = arguments[REMOVE_POLYS_PARAM].toLower() == "true";
  }

  if (arguments.keys().contains(CHECK_INTERSECTIONS_PARAM))
  {
    _checkIntersections = arguments[CHECK_INTERSECTIONS_PARAM].toLower() == "true";
  }

  if (arguments.keys().contains(CLUSTER_TAG_LIST_PARAM))
  {
    _clusterTagList = arguments[CLUSTER_TAG_LIST_PARAM];
  }

  QStringList tagList = _clusterTagList.split(";");

  foreach (QString tag, tagList)
  {
    QStringList keyValues = tag.split("=");
    if (keyValues.length() >= 2)
    {
      _clusterTags[keyValues[0]] = keyValues[1];
    }
  }
}

void PolyClusterGeoModifierAction::processStart(OsmMapPtr& /* pMap */)
{
  _ways.clear();  
}

bool PolyClusterGeoModifierAction::processElement(const ElementPtr& pElement, OsmMap* /* pMap */)
{
  // only process closed area ways
  if (pElement->getElementType() != ElementType::Way) return false;
  const WayPtr& pWay = std::dynamic_pointer_cast<Way>(pElement);
  if (!pWay->isClosedArea()) return false;

  // store for use in processFinalize
  _ways.push_back(pWay);
  return true;
}

void PolyClusterGeoModifierAction::processFinalize(OsmMapPtr& pMap)
{
  LOG_DEBUG("poly_cluster: finalizing " << _ways.length() << " ways");

  LOG_DEBUG("arguments:");
  LOG_VARD(_distance);
  LOG_VARD(_alpha);
  LOG_VARD(_removePolys);
  LOG_VARD(_checkIntersections);

  _pMap = pMap;

  // make sure we work from a fresh data set
  _clearProcessData();

  // generate geos::geom::Polygons for source buildings
  _createWayPolygons();

  // generate clusters from building polys
  _generateClusters();

  // create cluster representations on the map
  _createClusterPolygons();

  // debug info
  LOG_DEBUG("Generated " << _clusters.length() << " clusters.");

  // show clusters for debug
  foreach (QList<long> cluster, _clusters)
  {
    LOG_TRACE("Cluster way ids:");
    foreach (long id, cluster)
    {
      LOG_TRACE(id);
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

void PolyClusterGeoModifierAction::_createWayPolygons()
{
  ElementToGeometryConverter ElementToGeometryConverter(_pMap);

  // create a polygon from each building/way
  _polys.clear();

  foreach (WayPtr pWay, _ways)
  {
    if (!pWay)
      continue;
    std::shared_ptr<Polygon> pPoly = ElementToGeometryConverter.convertToPolygon(pWay);
    long wayId = pWay->getId();
    // set id as user data
    pPoly->setUserData((void*)wayId);
    _polyByWayId[wayId] = pPoly;
    _polys.push_back(pPoly);
  }
}

void PolyClusterGeoModifierAction::_generateClusters()
{   
  _distanceSquared = _distance * _distance;

  // build the ClosePointHash
  _pClosePointHash = std::make_shared<ClosePointHash>(_distance);

  // gather coordinates and build lookups
  foreach (std::shared_ptr<Polygon> poly, _polys)
  {
    long wayId = (long)poly->getUserData();
    const CoordinateSequence* pCoords = poly->getCoordinates();
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
  foreach (std::shared_ptr<Polygon> poly, _polys)
  {
    long wayId = (long)poly->getUserData();
    if (_processedPolys.contains(wayId)) continue;

    _clusters.push_back(QList<long>());
    _clusterIndex = _clusters.length()-1;

    _recursePolygons(poly);
  }
}

void PolyClusterGeoModifierAction::_recursePolygons(const std::shared_ptr<Polygon>& poly)
{
  long thisWayId = (long)poly->getUserData();
  assert(_processedPolys.contains(thisWayId) == false);

  // add this poly to current cluster and mark as processed
  _clusters[_clusterIndex].push_back(thisWayId);
  _processedPolys.push_back(thisWayId);

  // go through each node and find its matches
  int coordCount = min((int)poly->getNumPoints(), MAX_PROCESSED_NODES_PER_POLY-1);

  // create an envelope if we need it for intersection check
  std::shared_ptr<Envelope> pEnvelope;

  if (_checkIntersections)
  {
    WayPtr pWay = _pMap->getWay(thisWayId);
    pEnvelope = std::shared_ptr<Envelope>(pWay->getEnvelope(_pMap));
  }

  for (int i = 0; i < coordCount; i++)
  {
    long thisNodeIndex = thisWayId * MAX_PROCESSED_NODES_PER_POLY + i;
    CoordinateExt thisCoord = _coordinateByNodeIx[thisNodeIndex];
    vector<long> matches = _pClosePointHash->getMatchesFor(thisNodeIndex);

    foreach (long otherNodeIndex, matches)
    {
      long otherWayId = otherNodeIndex / MAX_PROCESSED_NODES_PER_POLY;
      CoordinateExt otherCoord = _coordinateByNodeIx[otherNodeIndex];

      // if the match is from another poly and within distance, process it
      if (otherWayId != thisWayId &&
         !_processedPolys.contains(otherWayId) &&
         (thisCoord-otherCoord).lengthSquared() <= _distanceSquared )
      {
        bool allow = true;

        if (_checkIntersections)
        {
          vector<long> wayIdsToCheck = _pMap->getIndex().findWays(*pEnvelope);

          // this probably takes quite a while, but here it is as an option
          for (long wayIdToCheck : wayIdsToCheck)
          {
            // don't check against polys in the cluster
            if (!_polyByWayId.contains(wayIdToCheck))
            {
              WayPtr pWayToCheck = _pMap->getWay(wayIdToCheck);

              long wayToCheckNodeCount = pWayToCheck->getNodeCount();
              vector<long> wayToCheckNodeIds = pWayToCheck->getNodeIds();

              for (int checkNodeIx = 0; checkNodeIx < wayToCheckNodeCount-1; checkNodeIx++)
              {
                long i1Id = wayToCheckNodeIds[checkNodeIx];
                long i2Id = wayToCheckNodeIds[checkNodeIx+1];

                CoordinateExt interP1(_pMap->getNode(i1Id)->toCoordinate());
                CoordinateExt interP2(_pMap->getNode(i2Id)->toCoordinate());

                std::shared_ptr<CoordinateExt> pIntersectionPoint = CoordinateExt::lineSegementsIntersect(thisCoord, otherCoord, interP1, interP2);

                if (pIntersectionPoint)
                {
                  allow = false;
                  break;
                }
              }

              if (!allow) break;
            }
          }
        }

        if (allow)
        {
          _recursePolygons(_polyByWayId[otherWayId]);
        }
      }
    }
  }
}

void PolyClusterGeoModifierAction::_createClusterPolygons()
{
  foreach (QList<long> cluster, _clusters)
  {
    // create alpha shape for each cluster
    AlphaShape alphashape(_alpha);

    // put all nodes of all buildings into an alpha shape
    std::vector<std::pair<double, double>> points;

    foreach (long wayId, cluster)
    {
      const CoordinateSequence* pCoords = _polyByWayId[wayId]->getCoordinates();

      CoordinateExt last;
      bool hasLast = false;

      for (size_t i = 0; i < pCoords->size(); i++)
      {
        CoordinateExt c = pCoords->getAt(i);
        std::pair<double, double> point(c.x, c.y);
        points.push_back(point);

        if (hasLast)
        {
          // if the this and the last point are further apart than distance,
          // split it up in distance or smaller length points
          CoordinateExt delta = last - c;
          double len = delta.length();
          double testDist = _alpha / 2.0;

          if (len > testDist)
          {
            double factor = testDist / len;
            int count = (int)(1.0 / factor);

            for (int p = 1; p <= count; p++)
            {
              double inc = (double)p * factor;
              CoordinateExt vInc = delta * inc;
              CoordinateExt add = c + vInc;
              std::pair<double, double> addPoint(add.x, add.y);
              points.push_back(addPoint);
            }
          }
        }

        last = c;
        hasLast = true;
      }
    }

    alphashape.insert(points);

    // generate geometry
    std::shared_ptr<Geometry> pAlphaGeom = alphashape.toGeometry();

    // combine polys from this cluster with AlphaShape
    vector<Geometry*> geomvect;
    geomvect.push_back(pAlphaGeom.get());

    foreach (long wayId, cluster)
    {
      geomvect.push_back(_polyByWayId[wayId].get());
    }

    const MultiPolygon *mp = GeometryFactory::getDefaultInstance()->createMultiPolygon(&geomvect);
    //std::shared_ptr<MultiPolygon> mp = std::shared_ptr<MultiPolygon>(GeometryFactory::getDefaultInstance()->createMultiPolygon(&geomvect));
    std::shared_ptr<Geometry> pCombinedPoly = geos::operation::geounion::UnaryUnionOp::Union(*mp);

    // create a new element with cluster representation
    GeometryToElementConverter gc(_pMap);
    std::shared_ptr<Element> pElem = gc.convertGeometryToElement(
          pCombinedPoly.get(),
          Status::Unknown1,
          WorstCircularErrorVisitor::getWorstCircularError(_pMap));

    // add desired cluster tags
    QHashIterator<QString, QString> tagIterator(_clusterTags);

    Tags tags = pElem->getTags();
    while (tagIterator.hasNext())
    {
      tagIterator.next();
      tags[tagIterator.key()] = tagIterator.value();
    }

    pElem->setTags(tags);
    _pMap->addElement(pElem);

    // remove cluster polys
    if (_removePolys)
    {
      // find all nodes from polys in this cluster
      std::vector<long> nodeIds;

      foreach (WayPtr pWay, _ways)
      {
        const std::vector<long> wayNodes = pWay->getNodeIds();
        nodeIds.insert(nodeIds.end(), wayNodes.begin(), wayNodes.end());
      }

      // remove polys
      foreach (long wayId, cluster)
      {
        RemoveWayByEid::removeWayFully(_pMap, wayId);
        _polyByWayId[wayId];
      }

      // remove nodes
      foreach (long nodeId, nodeIds)
      {
        RemoveNodeByEid removeOp(nodeId, true, false, true);
        removeOp.apply(_pMap);
      }
    }
  }
}

}
