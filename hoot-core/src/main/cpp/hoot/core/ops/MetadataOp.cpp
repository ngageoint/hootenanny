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
 * @copyright Copyright (C) 2019 Maxar (http://www.maxar.com/)
 */

#include "MetadataOp.h"

// Hoot
#include <hoot/core/ops/RemoveNodeByEid.h>
#include <hoot/core/ops/RemoveWayByEid.h>

// geos
#include <geos/geom/GeometryFactory.h>

using namespace geos::geom;
using namespace std;

namespace hoot
{

void MetadataOp::apply(std::shared_ptr<OsmMap>& pMap)
{
  _pMap = pMap;

  _elementsToProcess.clear();
  _datasetWayPolys.clear();
  _mergedGeoms.clear();
  _nodeLocations.clear();
  _numAffected = 0;

  _configure();

  // don't process anything without a dataset indicator
  if (_datasetIndicator.first.length() == 0) return;

  _allWays = _pMap->getWays();
  _allNodes = _pMap->getNodes();
  _allRels = _pMap->getRelations();

  _numProcessed = _allWays.size() + _allNodes.size() + _allRels.size();

  _apply();
}

void MetadataOp::setConfiguration(const Settings& conf)
{
  _pConf = &conf;
}

void MetadataOp::_configure()
{
  ConfigOptions opts = ConfigOptions(*_pConf);
  QStringList indicator = opts.getMetadataDatasetIndicatorTag();
  QStringList tags = opts.getMetadataTags();
  _gridCellSize = opts.getMetadataGridCellSize();

  if (indicator.length() > 1)
  {
    _datasetIndicator.first = indicator[0];
    _datasetIndicator.second = indicator[1];
  }

  for (int i = 0; i < tags.length(); i+=2)
  {
    QString key = tags[i];
    QString value = (i < tags.length()-1) ? tags[i+1] : "";
    _tags[key] = value;
  }
}

void MetadataOp::_gatherProcessElements()
{
  int elementCount = 0;
  int nodeCount = 0;
  int nodesInDatasets = 0;

  for (WayMap::const_iterator it = _allWays.begin(); it != _allWays.end(); ++it)
  {
    if (!_datasetWayPolys.contains(it->second) &&        // ignore the ways providing the dataset
        it->second->getTags().hasInformationTag())
    {
      _elementsToProcess.push_back(it->second);
      elementCount++;
    }
  }

  for (RelationMap::const_iterator it = _allRels.begin(); it != _allRels.end(); ++it)
  {
    if (it->second->getTags().hasInformationTag())
    {
      _elementsToProcess.push_back(it->second);
      elementCount++;
    }
  }

  for (NodeMap::const_iterator it = _allNodes.begin(); it != _allNodes.end(); ++it)
  {
    NodePtr pNode = it->second;

    if (pNode->getTags().hasInformationTag())
    {
      _elementsToProcess.push_back(pNode);
      elementCount++;
    }

    // determine all node locations for assigning elements to datasets
    shared_ptr<Point> pPoint = shared_ptr<Point>(
          GeometryFactory::getDefaultInstance()->createPoint(
            Coordinate(pNode->getX(),pNode->getY())));

    nodeCount++;

    for (shared_ptr<Geometry> geom : _mergedGeoms)
    {
      if (geom->contains(pPoint.get()))
      {
        _nodeLocations[pNode->getId()] = geom;
        nodesInDatasets++;
        break;
      }
    }
  }

  LOG_TRACE( "Non-debug elements: " << elementCount);
  LOG_TRACE( nodeCount << " nodes of which " << nodesInDatasets << " are inside datasets" );
}

WayPtr MetadataOp::_assignToDataset( ElementPtr pElement )
{
  // This function determines which dataset an element is assigned to. Currently it counts the
  // number of nodes of an element and assigns it the dataset that contain the largest number.
  // However in the future this may need to be more specific, for example picking the largest
  // area for closed ways or the longest distance for open ones.

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

      if (pMember)
      {
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

  WayPtr datasetWayWithMostNodes;
  int nodeCount = 0;

  foreach (WayPtr pWay, _mergedGeoms.keys())
  {
    shared_ptr<Geometry> pGeom =_mergedGeoms[pWay];
    if (nodeCountPerGeom[pGeom] > nodeCount)
    {
      datasetWayWithMostNodes = pWay;
      nodeCount = nodeCountPerGeom[pGeom];
    }
  }

  return datasetWayWithMostNodes;
}

void MetadataOp::_removeDatasetWay(WayPtr pDataset)
{
  // store way nodes for deletion
  vector<long> nodes = pDataset->getNodeIds();

  // remove the way
  RemoveWayByEid::removeWayFully(_pMap,pDataset->getId());

  for (long node: nodes)
  {
    RemoveNodeByEid::removeNodeFully(_pMap,node);
  }
}

}
