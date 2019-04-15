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
#ifndef BUILDINGPARTMERGEOP_H
#define BUILDINGPARTMERGEOP_H

// Hoot
#include <hoot/core/ops/OsmMapOperation.h>
#include <hoot/core/io/Serializable.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/info/OperationStatusInfo.h>
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
  struct hash<boost::shared_ptr<hoot::Element>>
  {
    size_t
    operator()(const boost::shared_ptr<hoot::Element>& k) const
    { return (size_t)(k.get()); }
  };

}

namespace hoot
{

/**
 * UFD Data frequently has buildings mapped out as individual parts where each part has a different
 * height. While they may represent a single building they're presented in the Shapefile as
 * individual rows.
 *
 * I would like to maintain the richness available in the data they've collected (gabled roofs,
 * heights on sections of buildings, etc). This is very applicable to the 3D mapping being done
 * in OSM. [1]
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
 * TODO: updated notes for mt changes
 */
class BuildingPartMergeOp : public OsmMapOperation, public Serializable, public OperationStatusInfo,
  public Configurable
{
public:

  static std::string className() { return "hoot::BuildingPartMergeOp"; }

  static unsigned int logWarnCount;

  BuildingPartMergeOp();

  virtual void apply(OsmMapPtr& map) override;

  virtual void setConfiguration(const Settings& conf);

  virtual std::string getClassName() const { return className(); }

  virtual void readObject(QDataStream& /*is*/) {}
  virtual void writeObject(QDataStream& /*os*/) const {}

  /**
   * todo
   *
   * @param map
   * @param parts
   * @return
   */
  RelationPtr combineBuildingParts(const OsmMapPtr& map, std::vector<ElementPtr>& parts);

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

private:

  // used to keep track of which elements make up a building
  Tgs::DisjointSetMap<ElementPtr> _buildingPartGroups;

  OsmMapPtr _map;

  //
  std::set<QString> _buildingPartTagNames;
  BuildingCriterion _buildingCrit;
  boost::shared_ptr<ElementConverter> _elementConverter;

  int _totalBuildingGroupsProcessed;
  int _numBuildingGroupsMerged;

  int _threadCount;

  /*
   * todo
   */
  void _preProcessBuildingParts();

  /*
   * todo
   */
  void _mergeBuildingParts();

  /*
   * todo
   */
  QQueue<BuildingPartDescription> _getBuildingPartPreProcessingInput();
  QQueue<BuildingPartDescription> _getBuildingPartWayPreProcessingInput();
  QQueue<BuildingPartDescription> _getBuildingPartRelationPreProcessingInput();

  /*
   * todo
   */
  void _initBuildingPartTagNames();

  /*
   * todo
   */
  boost::shared_ptr<geos::geom::Geometry> _getGeometry(const ElementPtr& element) const;

  /*
   * todo
   */
  static bool _hasContiguousNodes(const WayPtr& way, const long node1Id, const long node2Id);

  /*
   * todo
   */
  bool _compareTags(Tags t1, Tags te);

  /*
   * todo
   */
  std::set<long> _calculateNeighbors(const WayPtr& way, const Tags& tags);
};

}

#endif // BUILDINGPARTMERGEOP_H
