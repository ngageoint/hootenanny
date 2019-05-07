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

class PolyClusterGeoModifierAction : public GeometryModifierAction
{
public:
  static std::string className() { return "hoot::PolyClusterGeoModifierAction"; }

  virtual QString getCommandName() const override { return "poly_cluster"; }
  virtual QList<QString> getParameterNames() const override { return QList<QString> { }; }

  virtual void parseArguments(const QHash<QString, QString>& arguments) override;
  virtual void processStart(boost::shared_ptr<OsmMap>& ) override;
  virtual bool processElement(const ElementPtr& pElement, OsmMap* pMap) override;
  virtual void processFinalize(boost::shared_ptr<OsmMap>& pMap) override;

private:
  const int MAX_PROCESSED_NODES_PER_POLY = 1000;
  double _distance = 20;
  double _alpha = _distance;
  double _distanceSquared;

  boost::shared_ptr<OsmMap> _pMap;
  QList<WayPtr> _ways;
  QList<boost::shared_ptr<geos::geom::Polygon>> _polys;

  // cluster generation data
  int _clusterIndex;
  QList<QList<long>> _clusters;
  QList<long> _processedPolys;
  QHash<long, CoordinateExt> _coordinateByNodeIx;
  QHash<long, boost::shared_ptr<Polygon>> _polyByWayId;
  boost::shared_ptr<ClosePointHash> _pClosePointHash;

  void _clearProcessData();
  void _createWayPolygons();
  void _generateClusters();
  void _recursePolygons(const boost::shared_ptr<geos::geom::Polygon>& poly);
  void _createClusterPolygons();

  void _createDebugConvexHull();
};

}
#endif // POLYCLUSTERGEOMODIFIERACTION_H
