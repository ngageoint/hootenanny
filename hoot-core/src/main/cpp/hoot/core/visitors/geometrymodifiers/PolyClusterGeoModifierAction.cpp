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

  _ways.push_back(pWay);
  return true;
}

void PolyClusterGeoModifierAction::processFinalize(boost::shared_ptr<OsmMap>& pMap)
{
  LOG_INFO( "finalizing " << _ways.length() << " ways");

  boost::shared_ptr<Geometry> pCombinedPoly = boost::shared_ptr<Polygon>(GeometryFactory::getDefaultInstance()->createPolygon());
  ElementConverter elementConverter(pMap);

  QList<shared_ptr<Polygon>> geoms;

  foreach (WayPtr pWay, _ways)
  {
    shared_ptr<Polygon> pPoly = elementConverter.convertToPolygon(pWay);
    pCombinedPoly = boost::shared_ptr<Geometry>(pCombinedPoly->Union(pPoly.get()));

    geoms.push_back(pPoly);

  }

  _generateClusters(geoms);






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

void PolyClusterGeoModifierAction::_generateClusters(QList<shared_ptr<Polygon>>& geoms)
{
  foreach (shared_ptr<Polygon> inner, geoms)
  {
    foreach (shared_ptr<Polygon> outer, geoms)
    {
      if (inner != outer)
      {

      }
    }
  }

}

}
