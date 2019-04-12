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
#include <hoot/core/elements/Element.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/info/OperationStatusInfo.h>
#include <hoot/core/criterion/BuildingCriterion.h>
#include <hoot/core/elements/ElementConverter.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/elements/NodeToWayMap.h>
#include <hoot/core/schema/TagComparator.h>

// TGS
#include <tgs/DisjointSet/DisjointSetMap.h>

// Qt
#include <QHash>
#include <QRunnable>
#include <QQueue>
#include <QMutex>

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

class BuildingPartDescription
{

public:

  BuildingPartDescription(ElementPtr part, long neighborId, QString relationType,
                          boost::shared_ptr<geos::geom::Geometry> partGeom) :
  _part(part),
  _neighborId(neighborId),
  _relationType(relationType),
  _partGeom(partGeom)
  {
  }

  ElementPtr _part;
  long _neighborId;
  QString _relationType;
  boost::shared_ptr<geos::geom::Geometry> _partGeom;
};

class BuildingPartProcessor : public QRunnable
{

public:

  BuildingPartProcessor();
  ~BuildingPartProcessor();

  void run() override;

  void setBuildingPartTagNames(std::set<QString> tagNames)
  { _buildingPartTagNames = tagNames; }
  void setBuildingPartQueue(QQueue<BuildingPartDescription>* queue) { _buildingPartQueue = queue; }
  void setBuildingPartGroups(Tgs::DisjointSetMap<ElementPtr>* groups)
  { _buildingPartGroups = groups; }

  void setMap(OsmMapPtr map);

  void setBuildingPartGroupMutex(QMutex* mutex) { _buildingPartGroupMutex = mutex; }
  void setSchemaMutex(QMutex* mutex) { _schemaMutex = mutex; }
  void setBuildingPartQueueMutex(QMutex* mutex) { _buildingPartQueueMutex = mutex; }

private:

  QQueue<BuildingPartDescription>* _buildingPartQueue;

  QMutex* _buildingPartGroupMutex;
  QMutex* _schemaMutex;
  QMutex* _buildingPartQueueMutex;

  OsmMapPtr _map;

  Tgs::DisjointSetMap<ElementPtr>* _buildingPartGroups;

  std::set<QString> _buildingPartTagNames;
  boost::shared_ptr<ElementConverter> _elementConverter;
  BuildingCriterion _buildingCrit;
  boost::shared_ptr<NodeToWayMap> _n2w;

  QString _id;
  int _numGeometriesCleaned;
  int _numProcessed;

  void _addNeighborsToGroup(BuildingPartDescription buildingPart);
  void _addContainedWayToGroup(boost::shared_ptr<geos::geom::Geometry> g, const long wayId,
                               ElementPtr part);
  boost::shared_ptr<geos::geom::Geometry> _getGeometry(ElementPtr element,
                                                       const bool checkForBuilding = true);
  bool _isBuilding(ElementPtr element) const;
};

class Relation;
class OsmSchema;

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
 * 4/10/19 - See notes for this class in the mainline.  This is a less stable multithreaded version
 * but does not suffer from an unbalanced number of relation members as the 3061-2 version does.
 * Due to its instability, no actual performance testing has occurred for this yet.
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

  RelationPtr combineParts(const OsmMapPtr &map, const std::vector<ElementPtr>& parts);

  virtual QString getDescription() const override
  { return "Merges individual building parts into a single building"; }

  virtual QString getInitStatusMessage() const { return "Merging building parts..."; }

  virtual QString getCompletedStatusMessage() const
  {
    return "Merged " + StringUtils::formatLargeNumber(_numAffected) + " building parts.";
  }

private:

  /// Used to keep track of which elements make up a building.
  Tgs::DisjointSetMap<ElementPtr> _ds;
  OsmMapPtr _map;
  std::set<QString> _buildingPartTagNames;
  BuildingCriterion _buildingCrit;
  boost::shared_ptr<ElementConverter> _elementConverter;

  int _threadCount;

  void _processBuildingParts();
  void _mergeBuildingParts();

  QQueue<BuildingPartDescription> _getBuildingPartQueue();

  void _initBuildingPartTagNames();

  boost::shared_ptr<geos::geom::Geometry> _getGeometry(
    const ElementPtr& element, const bool checkForBuilding = true);
  bool _hasContiguousNodes(const WayPtr& w, long n1, long n2);
  bool _compareTags(Tags t1, Tags te);
  std::set<long> _calculateNeighbors(const WayPtr& w, const Tags& tags);
  bool _isBuilding(const ElementPtr& element) const;
};

}

#endif // BUILDINGPARTMERGEOP_H
