 
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

#include "CollapsePolyGeoModifierAction.h"

// geos
#include <geos/geom/Polygon.h>

// Hoot
#include <hoot/core/elements/ElementConverter.h>
#include <hoot/core/ops/RemoveNodeOp.h>
#include <hoot/core/util/Factory.h>

using namespace  geos::geom;

namespace hoot
{

HOOT_FACTORY_REGISTER(GeometryModifierAction, CollapsePolyGeoModifierAction)

const QString CollapsePolyGeoModifierAction::MAX_AREA_PARAM = "max_area_in_m";

bool CollapsePolyGeoModifierAction::process( const ElementPtr& pElement, OsmMap* pMap )
{
  // only process closed area ways
  if( pElement->getElementType() != ElementType::Way ) return false;
  const WayPtr& pWay = boost::dynamic_pointer_cast<Way>(pElement);
  if( !pWay->isClosedArea() ) return false;

  OsmMapPtr mapPtr = pMap->shared_from_this();
  ElementConverter elementConverter(mapPtr);

  shared_ptr<Polygon> pPoly = elementConverter.convertToPolygon(pWay);

  if( pPoly->getArea() < _area )
  {
    Coordinate centroid;
    if( pPoly->getCentroid(centroid) == false )
    {
      // throwing a HootException might be too harsh
      LOG_ERROR( "Collapse polygon modifier could not calculate centroid for element id " + pElement->getId() );
      return false;
    }

    NodePtr pNode( new Node(Status::Unknown1, pMap->createNextNodeId(), centroid) );

    // copy tags from original way to node
    pNode->setTags(pWay->getTags());

    std::vector<long> nodeIds = pWay->getNodeIds();

    // replace original way with node
    pMap->replace(pWay, pNode);

    // remove unused nodes of previous way
    for( std::vector<long>::iterator it = nodeIds.begin(); it != nodeIds.end(); it++ )
    {
      RemoveNodeOp removeOp( *it, true, false, true );
      removeOp.apply(mapPtr);
    }

    return true;
  }

  return false;
}

void CollapsePolyGeoModifierAction::parseArguments(const QHash<QString, QString>& arguments)
{
  _area = DEFAULT_AREA;

  if( arguments.keys().contains(MAX_AREA_PARAM) )
  {
    double area = arguments[MAX_AREA_PARAM].toDouble();
    if( area > 0 )
    {
      _area = area;
    }
  }
}

}
