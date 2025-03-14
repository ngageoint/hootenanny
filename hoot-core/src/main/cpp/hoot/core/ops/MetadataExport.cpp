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
 * @copyright Copyright (C) 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */

#include "MetadataExport.h"

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/geometry/ElementToGeometryConverter.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/visitors/CalculateMapBoundsVisitor.h>

using namespace geos::geom;
using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, MetadataExport)

void MetadataExport::_apply()
{
  _modifiedDatasets.clear();

  // create cells
  _createCells();

  // gather all potential source elements for metadata tags and create node location lookup
  _gatherProcessElements();

  // apply tags from elements to the dataset
  _exportMetadataFromElements();

  // remove datasets which contain no elements
  for (const auto& pDataset : _mergedGeoms.keys())
  {
    if (!_modifiedDatasets.contains(pDataset))
      _removeDatasetWay(pDataset);
  }
}

void MetadataExport::_createCells()
{
  const geos::geom::Envelope bounds = CalculateMapBoundsVisitor::getGeosBounds(_pMap);

  ElementToGeometryConverter ElementToGeometryConverter(_pMap);

  double minX = bounds.getMinX();
  double maxX = bounds.getMaxX();
  double minY = bounds.getMinY();
  double maxY = bounds.getMaxY();

  double gridX = _gridCellSize;
  double gridY = _gridCellSize;

  if (_gridCellSize == 0)
  {
    gridX = maxX - minX;
    gridY = maxY - minY;
  }
  else
  {
    // align the bounds with the grid spacing
    minX = floor(minX / gridX) * gridX;
    maxX = ceil(maxX / gridX) * gridX - gridX / 2;
    minY = floor(minY / gridY) * gridY;
    maxY = ceil(maxY / gridY) * gridY - gridY / 2;
  }

  for (double x = minX; x < maxX; x += gridX)
  {
    for (double y = minY; y < maxY; y += gridY)
    {
      WayPtr pGrid = std::make_shared<Way>(Status::Unknown1, _pMap->createNextWayId(), -1);

      // create grid poly
      long startId = _addNodeToPoly(x ,y, pGrid);
      _addNodeToPoly(x + gridX, y, pGrid);
      _addNodeToPoly(x + gridX, y + gridY, pGrid);
      _addNodeToPoly(x, y + gridY, pGrid);
      pGrid->addNode(startId);

      // mark as dataset and add to map
      pGrid->setTag(_datasetIndicator.first, _datasetIndicator.second);
      _pMap->addElement(pGrid);

      // convert to poly for assigning nodes based on location
      _datasetWayPolys[pGrid] = ElementToGeometryConverter.convertToPolygon(pGrid);

      // add these to the _mergedGeoms list (merging is for import only but we still need this
      // proper node location assignment)
      _mergedGeoms[pGrid] = _datasetWayPolys[pGrid];

      LOG_TRACE( "Added grid element " << pGrid->getId());
    }
  }
}

void MetadataExport::_exportMetadataFromElements()
{
  for (const auto& element : qAsConst(_elementsToProcess))
  {
    WayPtr assignedDataset = _assignToDataset(element);
    if (assignedDataset)
    {
      //  Copy the destination tags to update and replace
      Tags destTags = assignedDataset->getTags();
      const Tags& srcTags = element->getTags();
      // assign the tags we find
      for (const auto& tag : _tags.keys())
      {
        if (srcTags.contains(tag))
        {
          if (!destTags.contains(tag))  // assign src tag as new tag
            destTags[tag] = srcTags[tag];
          else
          {
            // attach src tag if not already assigned
            QStringList assignedDestTags = destTags[tag].split(";");
            QStringList newTags = srcTags[tag].split(";");

            for (const auto& newTag : qAsConst(newTags))
            {
              if (!assignedDestTags.contains(newTag))
                destTags[tag] += ";" + newTag;
            }
          }
        }
      }
      assignedDataset->setTags(destTags);
      if (!_modifiedDatasets.contains(assignedDataset))
        _modifiedDatasets.push_back(assignedDataset);
    }
  }

  // make sure all tags are set, if not, assign the default
  for (const auto& pDataset : _mergedGeoms.keys())
  {
    //  Copy destination tags to update and replace
    Tags destTags = pDataset->getTags();
    for (const auto& tag : _tags.keys())
    {
      if (!destTags.contains(tag))
        destTags[tag] = _tags[tag];
    }
    pDataset->setTags(destTags);
  }
  _numAffected = _modifiedDatasets.length();
}

long MetadataExport::_addNodeToPoly(double x, double y, const WayPtr& pPoly) const
{
  long nodeId = _pMap->createNextNodeId();
  _pMap->addElement(std::make_shared<Node>(Status::Unknown1, nodeId, Coordinate(x, y)));
  pPoly->addNode(nodeId);
  return nodeId;
}

}
