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
 * @copyright Copyright (C) 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef POLYCLUSTERGEOMODIFIERACTION_H
#define POLYCLUSTERGEOMODIFIERACTION_H

#include "GeometryModifierAction.h"

// Hoot
#include <hoot/core/util/CoordinateExt.h>
#include <hoot/core/index/ClosePointHash.h>

// Geos
#include <geos/geom/Polygon.h>

namespace hoot
{

/*
 * Replaces a cluster of polygons with a single polygon.
 * The purpose of this class is to simplify maps for printing by replacing a large amount of
 * buildings with a single built-up area polygon feature.
 *
 * Parameters:
 * - distance:
 *    Polygons are considered to be in the same cluster if any of their nodes are within 'distance'.
 * - alpha:
 *    The polygon replacing the cluster is an alpha-shape created with the polygon nodes and using
 *    this `alpha` value.
 * - remove_polys:
 *    If 'remove_polys' is true, the original polygons and all their nodes are removed from the map.
 * - check_intersections:
 *    If 'check_intersections' is true and there is an intersection encountered between two nodes
 *    which are within 'distance', the nodes are NOT considered to be within 'distance'.
 * - cluster_tag_list:
 *    The cluster polygon is created with the tags of the 'cluster_tag_list'.
 */
class PolyClusterGeoModifierAction : public GeometryModifierAction
{
public:

  static const QString DISTANCE_PARAM;
  static const QString ALPHA_PARAM;
  static const QString REMOVE_POLYS_PARAM;
  static const QString CHECK_INTERSECTIONS_PARAM;
  static const QString CLUSTER_TAG_LIST_PARAM;

  static std::string className() { return "hoot::PolyClusterGeoModifierAction"; }

  virtual QString getCommandName() const override { return "poly_cluster"; }
  virtual QList<QString> getParameterNames() const override { return QList<QString>
    { DISTANCE_PARAM, ALPHA_PARAM, REMOVE_POLYS_PARAM, CHECK_INTERSECTIONS_PARAM, CLUSTER_TAG_LIST_PARAM }; }

  virtual void parseArguments(const QHash<QString, QString>& arguments) override;
  virtual void processStart(OsmMapPtr& pMap) override;
  virtual bool processElement(const ElementPtr& pElement, OsmMap* pMap) override;
  virtual void processFinalize(OsmMapPtr& pMap) override;

  virtual std::string getClassName() const { return className(); }

private:

  const double DEFAULT_DISTANCE = 20;
  const double DEFAULT_ALPHA = 20;
  const bool DEFAULT_REMOVE_POLYS = true;
  const bool DEFAULT_CHECK_INTERSECTIONS = false;
  const QString DEFAULT_CLUSTER_TAG_LIST = QString("landuse=residential");

  const int MAX_PROCESSED_NODES_PER_POLY = 1000;

  double _distance = DEFAULT_DISTANCE;
  double _alpha = DEFAULT_ALPHA;
  bool _removePolys = DEFAULT_REMOVE_POLYS;
  bool _checkIntersections = DEFAULT_CHECK_INTERSECTIONS;
  QString _clusterTagList = DEFAULT_CLUSTER_TAG_LIST;

  std::shared_ptr<OsmMap> _pMap;
  QList<WayPtr> _ways;
  QList<std::shared_ptr<geos::geom::Polygon>> _polys;

  // cluster generation data
  double _distanceSquared;
  int _clusterIndex;
  QList<QList<long>> _clusters;
  QList<long> _processedPolys;
  QHash<long, CoordinateExt> _coordinateByNodeIx;
  QHash<long, std::shared_ptr<Polygon>> _polyByWayId;
  std::shared_ptr<ClosePointHash> _pClosePointHash;
  QHash<QString,QString> _clusterTags;

  void _clearProcessData();
  void _createWayPolygons();
  void _generateClusters();
  void _recursePolygons(const std::shared_ptr<geos::geom::Polygon>& poly);
  void _createClusterPolygons();
};

}
#endif // POLYCLUSTERGEOMODIFIERACTION_H
