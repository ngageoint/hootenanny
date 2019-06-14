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

#include "MetadataImport.h"

// Hoot
#include <hoot/core/elements/ElementConverter.h>


#include <hoot/core/util/Factory.h>

// geos
#include <geos/geom/GeometryFactory.h>


using namespace geos::geom;
using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, MetadataImport)

void MetadataImport::_apply()
{
  LOG_INFO( "IMPORTING METADATA" );

  // don't process anything without a dataset indicator
  if (_datasetIndicator.first.length() == 0) return;

  _allWays = _pMap->getWays();
  _allNodes = _pMap->getNodes();
  _allRels = _pMap->getRelations();

  // find all dataset ways and create Polygon object for each
  _findDatasetWays();

  // merge polygons as much as possible
  _mergePolygonsWithMatchingMetadata();

  // gather all potential target elements for metadata tags and
  // create node location lookup
  _gatherTargetElements();

  // apply tags to all elements contained in the _mergedImportGeoms
  _importMetadataToElements();
}

void MetadataImport::_findDatasetWays()
{
  QString indiKey = _datasetIndicator.first;
  QString indiVal = _datasetIndicator.second;
  ElementConverter elementConverter(_pMap);

  for (WayMap::const_iterator it = _allWays.begin(); it != _allWays.end(); ++it)
  {
    const WayPtr pWay = it->second;
    const Tags& tags = pWay->getTags();

    if (tags.contains(indiKey) && tags[indiKey]==indiVal)
    {
      LOG_INFO( "Found dataset indicator in way " << pWay->getId());

      _datasetWays.push_back(pWay);
      _datasetPolys.push_back(elementConverter.convertToPolygon(pWay));
    }
  }
}

void MetadataImport::_mergePolygonsWithMatchingMetadata()
{
  for (int ds = 0; ds < _datasetWays.length(); ds++)
  {
    bool matched = false;

    // check if current way matches any existing merged polys
    for (int im = 0; im < _mergedImportWaysRep.length(); im++)
    {
      if (_areMetadataTagsEqual(_datasetWays[ds], _mergedImportWaysRep[im]))
      {
        // merge polygon with existing polygon
        _mergedImportGeoms[im] =
            shared_ptr<Geometry>(_mergedImportGeoms[im]->Union(_datasetPolys[ds].get()));
        matched = true;
        break;
      }
    }

    if (!matched)
    {
      // create new polygon entry
      _mergedImportWaysRep.push_back(_datasetWays[ds]);
      _mergedImportGeoms.push_back(_datasetPolys[ds]);
    }
  }
}

void MetadataImport::_gatherTargetElements()
{
  for (WayMap::const_iterator it = _allWays.begin(); it != _allWays.end(); ++it)
  {
    if (!_datasetWays.contains(it->second) &&        // ignore the ways providing the dataset
        it->second->getTags().hasInformationTag())
    {
      _elementsToProcess.push_back(it->second);
    }
  }

  for (RelationMap::const_iterator it = _allRels.begin(); it != _allRels.end(); ++it)
  {
    if (it->second->getTags().hasInformationTag())
      _elementsToProcess.push_back(it->second);
  }

  for (NodeMap::const_iterator it = _allNodes.begin(); it != _allNodes.end(); ++it)
  {
    NodePtr pNode = it->second;

    if (pNode->getTags().hasInformationTag())
      _elementsToProcess.push_back(pNode);

    // determine all node locations for assigning elements to datasets
    shared_ptr<Point> pPoint = shared_ptr<Point>(
          GeometryFactory::getDefaultInstance()->createPoint(
            Coordinate(pNode->getX(),pNode->getY())));

    for (shared_ptr<Geometry> geom : _mergedImportGeoms)
    {
      if (geom->contains(pPoint.get()))
      {
        _nodeLocations[pNode->getId()] = geom;
      }
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

bool MetadataImport::_areMetadataTagsEqual(ElementPtr p1, ElementPtr p2)
{
  Tags t1 = p1->getTags();
  Tags t2 = p2->getTags();

  for (QString tag : _tags.keys())
  {
    bool p1hasTag = t1.contains(tag);
    bool p2hasTag = t2.contains(tag);

    if (p1hasTag && p2hasTag)
    {
      // fail with mismatching tag values
      if (t1[tag] != t2[tag]) return false;
    }
    else if (p1hasTag != p2hasTag)
    {
      // fail if one has the tag but not the other
      return false;
    }
  }

  return true;
}

bool MetadataImport::_applyToElement( ElementPtr pElement )
{
  // check if element is inside poly
  Way* pw = dynamic_cast<Way*>(pElement.get());
  Node* pn = dynamic_cast<Node*>(pElement.get());
  Relation* pr = dynamic_cast<Relation*>(pElement.get());

  vector<long> elementNodes;

  if (pn)
  {
    elementNodes.push_back(pn->getId());
  }

  if (pw)
  {
     vector<long> nodes = pw->getNodeIds();
     elementNodes.insert(elementNodes.end(), nodes.begin(), nodes.end());
  }

  if (pr)
  {
    for (RelationData::Entry entry : pr->getMembers())
    {
      ElementPtr pMember =_pMap->getElement(entry.getElementId());

      switch(pMember->getElementType().getEnum())
      {
        case ElementType::Way:
        {
          const WayPtr& pWay = std::dynamic_pointer_cast<Way>(pMember);
          vector<long> nodes = pWay->getNodeIds();
          elementNodes.insert(elementNodes.end(), nodes.begin(), nodes.end());
          break;
        }

        case ElementType::Node:
        {
          const NodePtr& pNode = std::dynamic_pointer_cast<Node>(pMember);
          elementNodes.push_back(pNode->getId());
          break;
        }

        default:
          break;
      }
    }
  }

  // count number of nodes per dataset
  QMap<shared_ptr<Geometry>,int> nodeCountPerGeom;

  for (long nodeId : elementNodes)
  {
    if (_nodeLocations.contains(nodeId))
    {
      shared_ptr<Geometry> geom = _nodeLocations[nodeId];

      if (nodeCountPerGeom.contains(geom) )
      {
        nodeCountPerGeom[geom]++;
      }
      else
      {
        nodeCountPerGeom[geom] = 1;
      }
    }
  }

  shared_ptr<Geometry> geomWithMostNodes;
  int nodeCount = 0;

  foreach (shared_ptr<Geometry> geom, _mergedImportGeoms)
  {
    if (nodeCountPerGeom[geom] > nodeCount)
    {
      geomWithMostNodes = geom;
      nodeCount = nodeCountPerGeom[geom];
    }
  }

  if (geomWithMostNodes)
  {
    int index = _mergedImportGeoms.indexOf(geomWithMostNodes);
    WayPtr pTagSource = _mergedImportWaysRep[index];

    Tags srcTags = pTagSource->getTags();
    Tags destTags = pElement->getTags();

    // finally copy over the tags
    for (QString tag : _tags.keys())
    {
      if (srcTags.contains(tag))
      {
        destTags[tag] = srcTags[tag];
      }
      else
      {
        destTags[tag] = _tags[tag];
      }
    }

    pElement->setTags(destTags);
    return true;
  }

  return false;
}

}
