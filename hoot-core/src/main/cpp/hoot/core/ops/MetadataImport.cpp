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

#include "MetadataImport.h"

// Hoot
#include <hoot/core/geometry/ElementToGeometryConverter.h>
#include <hoot/core/util/Factory.h>

using namespace geos::geom;
using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, MetadataImport)

void MetadataImport::_apply()
{
  // find all dataset ways and create Polygon object for each
  _findDatasetWays();

  // merge polygons as much as possible
  _mergePolygonsWithMatchingMetadata();

  // gather all potential target elements for metadata tags and
  // create node location lookup
  _gatherProcessElements();

  // apply tags to all elements contained in the _mergedImportGeoms
  _importMetadataToElements();

  // delete dataset ways
  for (const auto& pDataset : _datasetWayPolys.keys())
    _removeDatasetWay(pDataset);
}

void MetadataImport::_findDatasetWays()
{
  QString indiKey = _datasetIndicator.first;
  QString indiVal = _datasetIndicator.second;
  ElementToGeometryConverter ElementToGeometryConverter(_pMap);

  for (auto it = _allWays.begin(); it != _allWays.end(); ++it)
  {
    const WayPtr pWay = it->second;
    const Tags& tags = pWay->getTags();

    if (tags.contains(indiKey) && tags[indiKey]==indiVal)
    {
      LOG_TRACE( "Found dataset indicator in way " << pWay->getId());

      // store dataset way and its polygon geometry
      _datasetWayPolys[pWay] = ElementToGeometryConverter.convertToPolygon(pWay);
    }
  }
}

void MetadataImport::_mergePolygonsWithMatchingMetadata()
{
  for (const auto& pCheckWay : _datasetWayPolys.keys())
  {
    bool matched = false;

    // check if current way matches any existing merged polys
    for (const auto& pMergedWay : _mergedGeoms.keys())
    {
      if (_areMetadataTagsEqual(pCheckWay, pMergedWay))
      {
        // merge polygon with existing polygon
        _mergedGeoms[pMergedWay] = shared_ptr<Geometry>(_mergedGeoms[pMergedWay]->Union(_datasetWayPolys[pCheckWay].get()));
        matched = true;
        break;
      }
    }

    if (!matched)
    {
      // create new polygon entry
      _mergedGeoms[pCheckWay] = _datasetWayPolys[pCheckWay];
    }
  }
}

void MetadataImport::_importMetadataToElements()
{
  for (int ie = 0; ie < _elementsToProcess.length(); ie++)
  {
    if (_applyToElement(_elementsToProcess[ie]))
    {
      // remove successfuly processed element
      _elementsToProcess.removeAt(ie);
      ie--;
    }
  }
}

bool MetadataImport::_areMetadataTagsEqual(ElementPtr p1, ElementPtr p2) const
{
  Tags t1 = p1->getTags();
  Tags t2 = p2->getTags();

  for (const auto& tag : _tags.keys())
  {
    bool p1hasTag = t1.contains(tag);
    bool p2hasTag = t2.contains(tag);

    if (p1hasTag && p2hasTag)
    {
      // fail with mismatching tag values
      if (t1[tag] != t2[tag])
        return false;
    }
    else if (p1hasTag != p2hasTag)  // fail if one has the tag but not the other
      return false;
  }
  return true;
}

bool MetadataImport::_applyToElement( ElementPtr pElement )
{
  WayPtr assignedDataset = _assignToDataset( pElement );

  if (assignedDataset)
  {
    Tags srcTags = assignedDataset->getTags();
    Tags destTags = pElement->getTags();

    // finally copy over the tags
    for (const auto& tag : _tags.keys())
    {
      if (srcTags.contains(tag))
        destTags[tag] = srcTags[tag];
      else
        destTags[tag] = _tags[tag];
    }

    LOG_TRACE( "Copied metadata tags from way id " << assignedDataset->getId() << " to element id " << pElement->getId());

    pElement->setTags(destTags);
    _numAffected++;
    return true;
  }

  return false;
}

}
