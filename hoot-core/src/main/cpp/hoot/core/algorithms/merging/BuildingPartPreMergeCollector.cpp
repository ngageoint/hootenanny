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
#include "BuildingPartPreMergeCollector.h"

// geos
#include <geos/util/TopologyException.h>

// Hoot
#include <hoot/core/util/GeometryUtils.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/index/OsmMapIndex.h>

// Qt
#include <QUuid>

namespace hoot
{

BuildingPartPreMergeCollector::BuildingPartPreMergeCollector() :
_numGeometriesCleaned(0),
_numProcessed(0)
{
}

void BuildingPartPreMergeCollector::setMap(OsmMapPtr map)
{
  _map = map;
  // TODO: necessary?
  const std::vector<long> intersectIds = _map->getIndex().findWays(geos::geom::Envelope());
  LOG_VART(intersectIds.size());
  const boost::shared_ptr<ElementToRelationMap> e2r = _map->getIndex().getElementToRelationMap();
  LOG_VART(e2r->size());
  _n2w = _map->getIndex().getNodeToWayMap();
  _elementConverter.reset(new ElementConverter(_map));
}

void BuildingPartPreMergeCollector::run()
{
  _id = QUuid::createUuid().toString();
  LOG_TRACE("Starting thread: " << _id << "...");

  while (!_buildingPartQueue->empty())
  {
    _buildingPartQueueMutex->lock();
    BuildingPartDescription buildingPart = _buildingPartQueue->dequeue();
    _buildingPartQueueMutex->unlock();

    if (buildingPart._part) // TODO: necessary?
    {
      LOG_VART(buildingPart._part->getElementId());
      LOG_VART(buildingPart._neighborId);
      LOG_VART(buildingPart._relationType);

      _addNeighborsToGroup(buildingPart);

      _numProcessed++;
    }
    LOG_VART(_buildingPartQueue->size());
  }

  LOG_VART(_id);
  LOG_VART(_buildingPartGroups->size());
  LOG_VART(_numProcessed);
  LOG_VART(_numGeometriesCleaned);
}

void BuildingPartPreMergeCollector::_addNeighborsToGroup(const BuildingPartDescription buildingPart)
{
  if (buildingPart._relationType == "containedWay")
  {
    if (buildingPart._partGeom) // TODO: necessary?
    {
      _addContainedWayToGroup(
        buildingPart._partGeom, buildingPart._neighborId, buildingPart._part);
    }
  }
  else
  {
    // add these two buildings to a set
    WayPtr neighbor = _map->getWay(buildingPart._neighborId);
    _addBuildingPartGroup(neighbor, buildingPart._part);
  }
}

void BuildingPartPreMergeCollector::_addBuildingPartGroup(WayPtr building, ElementPtr buildingPart)
{
  _buildingPartGroupMutex->lock();
  _buildingPartGroups->joinT(building, buildingPart);
  LOG_VART(_buildingPartGroups->size());
  _buildingPartGroupMutex->unlock();
}

void BuildingPartPreMergeCollector::_addContainedWayToGroup(
  boost::shared_ptr<geos::geom::Geometry> g, const long wayId, ElementPtr part)
{
  WayPtr candidate = _map->getWay(wayId);

  boost::shared_ptr<geos::geom::Geometry> cg = _getGeometry(candidate);
  // if this is another building part totally contained by this building
  if (cg)
  {
    bool contains = false;
    try
    {
      contains = g->contains(cg.get());
    }
    catch (const geos::util::TopologyException&)
    {
      LOG_TRACE("cleaning...");
      boost::shared_ptr<geos::geom::Geometry> cleanCandidate(
        GeometryUtils::validateGeometry(cg.get()));
      boost::shared_ptr<geos::geom::Geometry> cleanG(GeometryUtils::validateGeometry(g.get()));
      contains = cleanG->contains(cleanCandidate.get());
      _numGeometriesCleaned++;
     }
     LOG_VART(contains);

    if (contains)
    {
      _addBuildingPartGroup(candidate, part);
    }
  }
}

boost::shared_ptr<geos::geom::Geometry> BuildingPartPreMergeCollector::_getGeometry(
  ElementPtr element, const bool checkForBuilding)
{
  boost::shared_ptr<geos::geom::Geometry> g;
  if (!checkForBuilding || _isBuilding(element))
  {
    if (element->getElementType() == ElementType::Relation)
    {
      g = _elementConverter->convertToGeometry(boost::dynamic_pointer_cast<Relation>(element));
    }
    else
    {
      _schemaMutex->lock();
      g = _elementConverter->convertToGeometry(boost::dynamic_pointer_cast<Way>(element));
      _schemaMutex->unlock();
    }
  }
  return g;
}

bool BuildingPartPreMergeCollector::_isBuilding(ElementPtr element) const
{
  _schemaMutex->lock();
  const bool isBuilding = _buildingCrit.isSatisfied(element);
  _schemaMutex->unlock();
  return isBuilding;
}

}
