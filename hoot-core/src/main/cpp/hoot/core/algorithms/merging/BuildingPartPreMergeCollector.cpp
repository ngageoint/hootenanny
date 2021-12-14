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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#include "BuildingPartPreMergeCollector.h"

// geos
#include <geos/util/TopologyException.h>

// Hoot
#include <hoot/core/geometry/GeometryUtils.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/util/StringUtils.h>

// Qt
#include <QUuid>

namespace hoot
{

BuildingPartPreMergeCollector::BuildingPartPreMergeCollector(std::mutex& inputMutex, std::mutex& outputMutex, std::mutex& schemaMutex)
  : _buildingPartsInput(nullptr),
    _startingInputSize(0),
    _buildingPartInputMutex(inputMutex),
    _buildingPartGroupsOutput(nullptr),
    _buildingPartOutputMutex(outputMutex),
    _hootSchemaMutex(schemaMutex),
    _numGeometriesCleaned(0),
    _numBuildingPartsProcessed(0)
{
}

void BuildingPartPreMergeCollector::setMap(ConstOsmMapPtr map)
{
  _map = map;
  _elementToGeomeryConverter = std::make_shared<ElementToGeometryConverter>(_map);
}

void BuildingPartPreMergeCollector::run()
{
  _id = QUuid::createUuid().toString();
  LOG_TRACE("Starting thread: " << _id << "...");

  while (!_buildingPartsInput->empty())
  {
    std::unique_lock<std::mutex> building_input_lock(_buildingPartInputMutex);
    BuildingPartRelationship buildingPartRelationship = _buildingPartsInput->dequeue();
    if (_buildingPartsInput->size() % 10000 == 0)
    {
      PROGRESS_INFO(
        "\tProcessed " <<
        StringUtils::formatLargeNumber(_startingInputSize - _buildingPartsInput->size()) <<
        " / " << StringUtils::formatLargeNumber(_startingInputSize) << " building parts.");
    }
    building_input_lock.unlock();

    _processBuildingPart(buildingPartRelationship);

    _numBuildingPartsProcessed++;
    LOG_VART(_buildingPartsInput->size());
  }

  LOG_VART(_id);
  LOG_VART(_buildingPartGroupsOutput->size());
  LOG_VART(_numBuildingPartsProcessed);
  LOG_VART(_numGeometriesCleaned);
}

void BuildingPartPreMergeCollector::_processBuildingPart(const BuildingPartRelationship& buildingPartRelationship)
{
  switch (buildingPartRelationship.relationshipType)
  {
  case BuildingPartRelationship::BuildingPartRelationshipType::ContainedWay:
    assert(buildingPartRelationship.buildingGeom);
    _addContainedBuildingPartToGroup(buildingPartRelationship.building,
                                     buildingPartRelationship.buildingGeom,
                                     buildingPartRelationship.buildingPartNeighbor);
    break;
  case BuildingPartRelationship::BuildingPartRelationshipType::Neighbor:
    // add these two buildings to a set
    _groupBuildingParts(buildingPartRelationship.building, buildingPartRelationship.buildingPartNeighbor);
    break;
  default:
    throw IllegalArgumentException(QString("Unknown building part description relation type: ") +
      buildingPartRelationship.relationshipType);
  }
}

void BuildingPartPreMergeCollector::_groupBuildingParts(ElementPtr building, WayPtr buildingPart)
{
  std::lock_guard<std::mutex> outputLock(_buildingPartOutputMutex);
  _buildingPartGroupsOutput->joinT(buildingPart, building);
  LOG_VART(_buildingPartGroupsOutput->size());
}

void BuildingPartPreMergeCollector::_addContainedBuildingPartToGroup(ElementPtr building,
                                                                     std::shared_ptr<geos::geom::Geometry> buildingGeom,
                                                                     WayPtr buildingPartNeighbor)
{
  std::shared_ptr<geos::geom::Geometry> buildingPartMatchCandidateGeom = _getGeometry(buildingPartNeighbor);
  assert(buildingPartMatchCandidateGeom);
  if (buildingPartMatchCandidateGeom->isEmpty())
    return;
  // if this is another building part totally contained by this building
  bool contains = false;
  try
  {
    contains = buildingGeom->contains(buildingPartMatchCandidateGeom.get());
  }
  catch (const geos::util::TopologyException&)
  {
    // Something is wrong with the geometry, so let's try cleaning it.
    LOG_TRACE("cleaning...");
    std::shared_ptr<geos::geom::Geometry> cleanMatchCandidate(
      GeometryUtils::validateGeometry(buildingPartMatchCandidateGeom.get()));
    std::shared_ptr<geos::geom::Geometry> cleanBuilding(
      GeometryUtils::validateGeometry(buildingGeom.get()));
    contains = cleanBuilding->contains(cleanMatchCandidate.get());
    _numGeometriesCleaned++;
   }
   LOG_VART(contains);

  if (contains)
    _groupBuildingParts(building, buildingPartNeighbor);
}

std::shared_ptr<geos::geom::Geometry> BuildingPartPreMergeCollector::_getGeometry(ConstElementPtr element)
{
  std::shared_ptr<geos::geom::Geometry> geom;
  switch (element->getElementType().getEnum())
  {
  case ElementType::Way:
  {
    // We could avoid having to use this mutex by passing in the precomputed element geoms,
    // but that was causing stability issues as noted in
    // BuildingPartMergeOp::_getBuildingPartPreProcessingInput.
    std::lock_guard<std::mutex> schema_lock(_hootSchemaMutex);
    geom = _elementToGeomeryConverter->convertToGeometry(std::dynamic_pointer_cast<const Way>(element));
    break;
  }
  case ElementType::Relation:
  {
    // Interestingly enough, conversion to a relation doesn't make any calls to OsmSchema and,
    // therefore, don't require a mutex lock.  Its not inconceivable that fact could change at
    // some point and then a mutex would have to be added here.
    geom = _elementToGeomeryConverter->convertToGeometry(std::dynamic_pointer_cast<const Relation>(element));
    break;
  }
  default:
    throw IllegalArgumentException("Unexpected element type: " + element->getElementType().toString());
  }
  return geom;
}

}
