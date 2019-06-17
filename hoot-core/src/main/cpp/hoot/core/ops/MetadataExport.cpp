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

#include "MetadataExport.h"

// Hoot
#include <hoot/core/elements/ElementConverter.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/visitors/CalculateMapBoundsVisitor.h>

// geos
#include <geos/geom/Coordinate.h>

using namespace geos::geom;
using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, MetadataExport)

void MetadataExport::_apply()
{
  // create cells
  _createCells();

  // gather all potential target elements for metadata tags and
  // create node location lookup
  _gatherTargetElements();

}

void MetadataExport::_createCells()
{
  const geos::geom::Envelope bounds = CalculateMapBoundsVisitor::getGeosBounds(_pMap);

  ElementConverter elementConverter(_pMap);

  //  LOG_INFO(bounds.getMinX());
  //  LOG_INFO(bounds.getMaxX());
  //  LOG_INFO(bounds.getMinY());
  //  LOG_INFO(bounds.getMaxY());

  double minX = bounds.getMinX();
  double maxX = bounds.getMaxX();
  double minY = bounds.getMinY();
  double maxY = bounds.getMaxY();

  minX = floor(minX / _cellSize) * _cellSize;
  maxX = ceil(maxX / _cellSize) * _cellSize - _cellSize / 2;
  minY = floor(minY / _cellSize) * _cellSize;
  maxY = ceil(maxY / _cellSize) * _cellSize - _cellSize / 2;

  for (double x = minX; x < maxX; x += _cellSize)
  {
    for (double y = minY; y < maxY; y += _cellSize)
    {
      WayPtr pGrid(new Way(Status::Unknown1, _pMap->createNextWayId(), -1));

      // create grid poly
      long startId = _addNodeToPoly(x ,y, pGrid);
      _addNodeToPoly(x + _cellSize, y, pGrid);
      _addNodeToPoly(x + _cellSize, y + _cellSize, pGrid);
      _addNodeToPoly(x, y + _cellSize, pGrid);
      pGrid->addNode(startId);
      _pMap->addElement(pGrid);

      _datasetWayPolys[pGrid] = elementConverter.convertToPolygon(pGrid);

      LOG_INFO( "Added grid element " << pGrid->getId());
    }
  }
}

long MetadataExport::_addNodeToPoly(double x, double y, WayPtr& pPoly)
{
  long nodeId = _pMap->createNextNodeId();
  NodePtr pNode(new Node(Status::Unknown1, nodeId, Coordinate(x,y)));
  _pMap->addElement(pNode);
  pPoly->addNode(nodeId);
  return nodeId;
}

}
