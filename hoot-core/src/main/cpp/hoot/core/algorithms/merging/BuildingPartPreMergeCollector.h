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
 * todo
 */
struct BuildingPartDescription
{
  enum BuildingPartRelationType
  {
    ContainedWay = 1,
    Neighbor
  };

  BuildingPartDescription(ElementPtr part, WayPtr neighbor, BuildingPartRelationType relationType,
                          boost::shared_ptr<geos::geom::Geometry> partGeom) :
  _part(part),
  _neighbor(neighbor),
  _relationType(relationType),
  _partGeom(partGeom)
  {
  }

  //
  ElementPtr _part;
  //
  WayPtr _neighbor;
  //
  BuildingPartRelationType _relationType;
  //
  boost::shared_ptr<geos::geom::Geometry> _partGeom;
};

/**
 * todo
 */
class BuildingPartPreMergeCollector : public QRunnable
{

public:

  BuildingPartPreMergeCollector();

  /**
   * todo
   */
  void run() override;

  void setBuildingPartsInput(QQueue<BuildingPartDescription>* queue)
  { _buildingPartsInput = queue; }
  void setBuildingPartGroupsOutput(Tgs::DisjointSetMap<ElementPtr>* groups)
  { _buildingPartGroupsOutput = groups; }

  void setMap(ConstOsmMapPtr map);

  void setBuildingPartInputMutex(QMutex* mutex) { _buildingPartInputMutex = mutex; }
  void setHootSchemaMutex(QMutex* mutex) { _hootSchemaMutex = mutex; }
  void setBuildingPartOutputMutex(QMutex* mutex) { _buildingPartOutputMutex = mutex; }

private:

  //
  QQueue<BuildingPartDescription>* _buildingPartsInput;
  // Accessing this as a shared pointer slows down processing by ~60%, so will keep it as a raw
  // pointer.
  Tgs::DisjointSetMap<ElementPtr>* _buildingPartGroupsOutput;

  ConstOsmMapPtr _map;

  boost::shared_ptr<ElementConverter> _elementConverter;

  //
  QMutex* _buildingPartInputMutex;
  //
  QMutex* _hootSchemaMutex;
  //
  QMutex* _buildingPartOutputMutex;

  QString _id;
  int _numGeometriesCleaned;
  int _numBuildingPartsProcessed;

  // Don't pass these shared pointers around as refs, as it will disrupt the ref counting and wreak
  // havoc in the threads.

  /*
   * todo
   */
  boost::shared_ptr<geos::geom::Geometry> _getGeometry(ConstElementPtr element);

  /*
   * todo
   */
  void _addNeighborsToGroup(const BuildingPartDescription& buildingPartDesc);

  /*
   * todo
   */
  void _addContainedWayToGroup(boost::shared_ptr<geos::geom::Geometry> buildingPartGeom,
                               WayPtr neighbor, ElementPtr buildingPart);

  /*
   * todo
   */
  void _addBuildingPartGroup(WayPtr building, ElementPtr buildingPart);
};

}

#endif // BUILDING_PART_PRE_MERGE_COLLECTOR_H
