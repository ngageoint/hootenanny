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

#include "MetadataOp.h"

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
  _mergedImportGeoms.clear();
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
  LOG_INFO( "MetadataOp setConfiguration");
  _pConf = &conf;
}

void MetadataOp::_configure()
{
  ConfigOptions opts = ConfigOptions(*_pConf);
  QStringList indicator = opts.getMetadataDatasetIndicatorTag();
  QStringList tags = opts.getMetadataTags();

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

void MetadataOp::_gatherTargetElements()
{
  for (WayMap::const_iterator it = _allWays.begin(); it != _allWays.end(); ++it)
  {
    if (!_datasetWayPolys.contains(it->second) &&        // ignore the ways providing the dataset
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

}
