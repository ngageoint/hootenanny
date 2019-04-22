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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef BUILDING_PART_PRE_MERGE_COLLECTOR_H
#define BUILDING_PART_PRE_MERGE_COLLECTOR_H

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/elements/ElementConverter.h>
#include <hoot/core/util/StringUtils.h>

// TGS
#include <tgs/DisjointSet/DisjointSetMap.h>

// Qt
#include <QRunnable>
#include <QQueue>
#include <QMutex>

// geos
#include <geos/geom/Geometry.h>

namespace hoot
{

/**
 * Contains information to group a building part either as a contained way or a neighboring part
 */
struct BuildingPartRelationship
{
  enum BuildingPartRelationshipType
  {
    // This is a building that we need to check for containment within another building before
    // adding to the group.
    ContainedWay = 1,
    // This is a building neighbor that we can add to the group with no further checks.
    Neighbor
  };

  BuildingPartRelationship(ElementPtr building, boost::shared_ptr<geos::geom::Geometry> buildingGeom,
                           WayPtr buildingPartNeighbor,
                           BuildingPartRelationshipType relationshipType)
    : building(building),
      buildingGeom(buildingGeom),
      buildingPartNeighbor(buildingPartNeighbor),
      relationshipType(relationshipType)
  {
  }

  // building containing building parts
  ElementPtr building;
  // the geometry of building
  boost::shared_ptr<geos::geom::Geometry> buildingGeom;
  // a neighboring buildin part to building
  WayPtr buildingPartNeighbor;
  // the type of relationship between building and buildingPartNeighbor
  BuildingPartRelationshipType relationshipType;
};

/**
 * Building part merge pre-processing thread
 */
class BuildingPartPreMergeCollector : public QRunnable
{

public:

  BuildingPartPreMergeCollector();

  /**
   * Takes building part input from an input queue and adds groups it with a disjoint set as
   * appropriate
   *
   * @see QRunnable
   */
  void run() override;

  void setBuildingPartsInput(QQueue<BuildingPartRelationship>* queue)
  { _buildingPartsInput = queue; }
  void setBuildingPartGroupsOutput(Tgs::DisjointSetMap<ElementPtr>* groups)
  { _buildingPartGroupsOutput = groups; }

  void setMap(ConstOsmMapPtr map);

  void setBuildingPartInputMutex(QMutex* mutex) { _buildingPartInputMutex = mutex; }
  void setStartingInputSize(int size) { _startingInputSize = size; }
  void setHootSchemaMutex(QMutex* mutex) { _hootSchemaMutex = mutex; }
  void setBuildingPartOutputMutex(QMutex* mutex) { _buildingPartOutputMutex = mutex; }

private:

  // input data to be grouped
  QQueue<BuildingPartRelationship>* _buildingPartsInput;
  int _startingInputSize;
  // Output building part data that can be merged; accessing this as a shared pointer slows down
  // processing by ~60%, so will keep it as a raw pointer.
  Tgs::DisjointSetMap<ElementPtr>* _buildingPartGroupsOutput;

  ConstOsmMapPtr _map;

  boost::shared_ptr<ElementConverter> _elementConverter;

  // protects the input data
  QMutex* _buildingPartInputMutex;
  // protects access to OsmSchema, which is not thread safe
  QMutex* _hootSchemaMutex;
  // protects the output data
  QMutex* _buildingPartOutputMutex;

  QString _id;
  int _numGeometriesCleaned;
  int _numBuildingPartsProcessed;

  // Don't pass the shared pointers used by these methods around as refs, as it will disrupt the
  // ref counting and wreak havoc in the threads.

  boost::shared_ptr<geos::geom::Geometry> _getGeometry(ConstElementPtr element);

  void _processBuildingPart(const BuildingPartRelationship& buildingPartRelationship);
  void _addContainedBuildingPartToGroup(ElementPtr building,
                                        boost::shared_ptr<geos::geom::Geometry> buildingGeom,
                                        WayPtr buildingPartNeighbor);

  /*
   * groups a building part with a building so they can later be merged together
   */
  void _groupBuildingParts(ElementPtr building, WayPtr buildingPart);
};

}

#endif // BUILDING_PART_PRE_MERGE_COLLECTOR_H
