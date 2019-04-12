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
_numBuildingPartsProcessed(0)
{
}

void BuildingPartPreMergeCollector::setMap(OsmMapPtr map)
{
  _map = map;
  _elementConverter.reset(new ElementConverter(_map));
}

void BuildingPartPreMergeCollector::run()
{
  _id = QUuid::createUuid().toString();
  LOG_TRACE("Starting thread: " << _id << "...");

  while (!_buildingPartsInput->empty())
  {
    _buildingPartInputMutex->lock();
    BuildingPartDescription buildingPartDesc = _buildingPartsInput->dequeue();
    _buildingPartInputMutex->unlock();

    LOG_VART(buildingPartDesc._part->getElementId());
    LOG_VART(buildingPartDesc._neighbor->getElementId());
    LOG_VART(buildingPartDesc._relationType);

    _addNeighborsToGroup(buildingPartDesc);

    _numBuildingPartsProcessed++;
    LOG_VART(_buildingPartsInput->size());
  }

  LOG_VART(_id);
  LOG_VART(_buildingPartGroupsOutput->size());
  LOG_VART(_numBuildingPartsProcessed);
  LOG_VART(_numGeometriesCleaned);
}

void BuildingPartPreMergeCollector::_addNeighborsToGroup(
  const BuildingPartDescription buildingPartDesc)
{
  switch (buildingPartDesc._relationType)
  {
    case BuildingPartDescription::BuildingPartRelationType::ContainedWay:
      assert(buildingPartDesc._partGeom);
      _addContainedWayToGroup(
        buildingPartDesc._partGeom, buildingPartDesc._neighbor, buildingPartDesc._part);
      break;

    case BuildingPartDescription::BuildingPartRelationType::Neighbor:
      // add these two buildings to a set
      _addBuildingPartGroup(buildingPartDesc._neighbor, buildingPartDesc._part);
      break;

    default:
      throw IllegalArgumentException(
        "Unknown building part description relation type: " + buildingPartDesc._relationType);
  }
}

void BuildingPartPreMergeCollector::_addBuildingPartGroup(WayPtr building, ElementPtr buildingPart)
{
  _buildingPartOutputMutex->lock();
  _buildingPartGroupsOutput->joinT(building, buildingPart);
  LOG_VART(_buildingPartGroupsOutput->size());
  _buildingPartOutputMutex->unlock();
}

void BuildingPartPreMergeCollector::_addContainedWayToGroup(
  boost::shared_ptr<geos::geom::Geometry> buildingPartGeom, WayPtr neighbor,
  ElementPtr buildingPart)
{
  boost::shared_ptr<geos::geom::Geometry> candidateGeom = _getGeometry(neighbor);
  assert(candidateGeom);
  // if this is another building part totally contained by this building
  bool contains = false;
  try
  {
    contains = buildingPartGeom->contains(candidateGeom.get());
  }
  catch (const geos::util::TopologyException&)
  {
    LOG_TRACE("cleaning...");
    boost::shared_ptr<geos::geom::Geometry> cleanCandidate(
      GeometryUtils::validateGeometry(candidateGeom.get()));
    boost::shared_ptr<geos::geom::Geometry> cleanBuildingPart(
      GeometryUtils::validateGeometry(buildingPartGeom.get()));
    contains = cleanBuildingPart->contains(cleanCandidate.get());
    _numGeometriesCleaned++;
   }
   LOG_VART(contains);

  if (contains)
  {
    _addBuildingPartGroup(neighbor, buildingPart);
  }
}

boost::shared_ptr<geos::geom::Geometry> BuildingPartPreMergeCollector::_getGeometry(
  ElementPtr element)
{
  boost::shared_ptr<geos::geom::Geometry> geom;
  switch (element->getElementType().getEnum())
  {
    case ElementType::Way:
      // TODO: We could avoid even having to use this mutex by passing in the way geoms...
      _hootSchemaMutex->lock();
      geom = _elementConverter->convertToGeometry(boost::dynamic_pointer_cast<Way>(element));
      _hootSchemaMutex->unlock();
      break;

    case ElementType::Relation:
      geom = _elementConverter->convertToGeometry(boost::dynamic_pointer_cast<Relation>(element));
      break;

    default:
      throw IllegalArgumentException(
        "Unexpected element type: " + element->getElementType().toString());
  }
  return geom;
}

}
