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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef BUILDINGPARTMERGEOP_H
#define BUILDINGPARTMERGEOP_H

// Hoot
#include <hoot/core/ops/OsmMapOperation.h>
#include <hoot/core/io/Serializable.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/criterion/BuildingCriterion.h>
#include <hoot/core/elements/ElementConverter.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/algorithms/merging/BuildingPartPreMergeCollector.h>

// TGS
#include <tgs/DisjointSet/DisjointSetMap.h>

// geos
#include <geos/geom/Geometry.h>

namespace __gnu_cxx
{

template<>
  struct hash<std::shared_ptr<hoot::Element>>
  {
    size_t
    operator()(const std::shared_ptr<hoot::Element>& k) const
    { return (size_t)(k.get()); }
  };

}

namespace hoot
{

/**
 * UFD Data frequently has buildings mapped out as individual parts where each part has a different
 * height. While they may represent a single building they're presented in the Shapefile as
 * individual rows. We would like to maintain the richness available in the data they've collected
 * (gabled roofs, heights on sections of buildings, etc). This is very applicable to the 3D mapping
 * being done in OSM. [1]
 *
 * This class implicitly merges the individual building parts into a single part. If two buildings
 * share two or more contiguous nodes and the relevant tags match then the parts will be merged
 * into a single building. In some cases this may not be desirable (think row houses in DC without
 * addresses), but it should yield reasonable results on a case by case basis.
 *
 * When multiple parts are merged the result is a building relation. All other elements are left
 * untouched.
 *
 * It is assumed that the data has been cleaned and redundent nodes and invalid ways have been
 * removed. It is also assumed that all buildings form closed polygons.
 *
 * 1. http://wiki.openstreetmap.org/wiki/OSM-3D
 *
 * This class has been updated to process building parts in parallel. The bottleneck in the original
 * merging logic had mostly to do with the geometry calls to GEOS to check for building part
 * containment within buildings, and to a lesser degree, the inserts into the disjoint set map for
 * the building part groups. This class now collects all building parts beforehand and sends them
 * off to threads for parallel processing. As of 3/15/19, this resulted in a ~78% performance
 * increase when processing ~500K UFD buildings. The parallelization must occur at the building
 * part level, and not at the building level, because a single building relation may have far more
 * building parts than others, which would cause an imbalance with paralleization made at the
 * building level.
 *
 * TODO: Should this class be renamed since it merges not only building:part now but also multipoly
 * buildings? Or should we change it only to be more strict and only merge building:part features
 * (if that's possible; or maybe its already doing that)?
 */
class BuildingPartMergeOp : public OsmMapOperation, public Serializable, public Configurable
{
public:

  static std::string className() { return "hoot::BuildingPartMergeOp"; }

  static int logWarnCount;

  BuildingPartMergeOp(bool preserveTypes = false);

  virtual void apply(OsmMapPtr& map) override;

  virtual void setConfiguration(const Settings& conf);

  virtual std::string getClassName() const { return className(); }

  virtual void readObject(QDataStream& /*is*/) {}
  virtual void writeObject(QDataStream& /*os*/) const {}

  virtual QString getDescription() const override
  { return "Merges individual building parts into a single building"; }

  virtual QString getInitStatusMessage() const { return "Merging building parts..."; }

  virtual QString getCompletedStatusMessage() const
  {
    return
      "Merged " + StringUtils::formatLargeNumber(_numAffected) +
      " building parts from " +
      StringUtils::formatLargeNumber(_numBuildingGroupsMerged) + " valid building groups / " +
      StringUtils::formatLargeNumber(_totalBuildingGroupsProcessed) + " total.";
  }

  void setThreadCount(int count) { _threadCount = count; }

  int getTotalBuildingGroupsProcessed() const { return _totalBuildingGroupsProcessed; }
  int getNumBuildingGroupsMerged() const { return _numBuildingGroupsMerged; }
  void setPreserveTypes(bool preserve) { _preserveTypes = preserve; }

private:

  // used to keep track of which elements make up a building
  Tgs::DisjointSetMap<ElementPtr> _buildingPartGroups;

  OsmMapPtr _map;

  BuildingCriterion _buildingCrit;
  std::shared_ptr<ElementConverter> _elementConverter;

  int _totalBuildingGroupsProcessed;
  int _numBuildingGroupsMerged;

  int _threadCount;

  // if true, building part type tags will be preserved in the combined building output
  bool _preserveTypes;

  void _init(OsmMapPtr& map);

  std::shared_ptr<geos::geom::Geometry> _getGeometry(const ConstElementPtr& element) const;

  /*
   * Collects building parts and passes them off for parallel processing
   */
  void _preProcessBuildingParts();

  /*
   * Groups contained and neighboring building part with the buildings containing them
   */
  QQueue<BuildingPartRelationship> _getBuildingPartPreProcessingInput();
  QQueue<BuildingPartRelationship> _getBuildingPartWayPreProcessingInput();
  QQueue<BuildingPartRelationship> _getBuildingPartRelationPreProcessingInput();

  /*
   * Merges building parts grouped by the parallel processing
   */
  void _mergeBuildingParts();

  /*
   * Determines neighboring building parts for a building
   */
  std::set<long> _calculateNeighbors(const ConstWayPtr& way, const Tags& tags);

  static bool _hasContiguousNodes(const ConstWayPtr& way, const long node1Id, const long node2Id);

  /*
   * Returns a similarity decision by scoring the non-building part tags between two tag sets
   */
  bool _compareTags(Tags t1, Tags te);
};

}

#endif // BUILDINGPARTMERGEOP_H
