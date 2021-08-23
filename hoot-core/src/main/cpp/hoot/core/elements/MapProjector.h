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
#ifndef __MAP_PROJECTOR_H__
#define __MAP_PROJECTOR_H__

// GDAL
#include <ogr_core.h>

// GEOS
#include <geos/geom/Coordinate.h>
#include <geos/geom/CoordinateFilter.h>
#include <geos/geom/Geometry.h>

// Hoot
#include <hoot/core/elements/ElementProvider.h>
#include <hoot/core/util/Units.h>

// Qt


// Standard
#include <memory>
#include <vector>

class OGRCoordinateTransformation;
class OGRSpatialReference;

namespace hoot
{

class OsmMap;

/**
 * (Singleton)
 *
 * Note that OGRSpatialReference does not always play nicely with make_shared at GDAL 3.2.3.
 */
class MapProjector
{
public:

  static QString className() { return "hoot::MapProjector"; }

  static int logWarnCount;

  /**
   * Given a bounding box in WGS84, create a reasonable planar projection for that region. The
   * units are in meters.
   */
  static std::shared_ptr<OGRSpatialReference> createAeacProjection(const OGREnvelope& env);

  /**
   * Create an orthographic projection centered around env.
   */
  static std::shared_ptr<OGRSpatialReference> createOrthographic(const OGREnvelope& env);
  static std::shared_ptr<OGRSpatialReference> createOrthographic(double x, double y);

  /**
   * Given a bounding box in WGS84, create a reasonable planar projection for that region. The
   * units are in meters.
   */
  static std::shared_ptr<OGRSpatialReference> createSinusoidalProjection(const OGREnvelope& env);

  static std::shared_ptr<OGRSpatialReference> createWgs84Projection();

  static void project(const std::shared_ptr<OsmMap>& map,
                      const std::shared_ptr<OGRSpatialReference>& ref);

  /**
   * Returns a vector of all candidate planar projections for a given envelope.
   */
  std::vector<std::shared_ptr<OGRSpatialReference>> createAllPlanarProjections(
    const OGREnvelope& env) const;

  /**
   * Using a predefined set of projections this method evaluates each one of them for both distance
   * and angular error over the given envelope. The best projection is returned.
   *
   * If no projections are found that meet both thresholds and warnOnFail == false then an error
   * is thrown. Otherwise, the best projection is returned.
   *
   * Best is an internal heuristic that is subject to change.
   */
  std::shared_ptr<OGRSpatialReference> createPlanarProjection(const OGREnvelope& env,
    Radians maxAngleError = toRadians(2.0), Meters maxDistanceError = 10.0,
    Meters testDistance = 1000.0, bool warnOnFail = true) const;

  static MapProjector& getInstance();

  static bool isGeographic(const ConstElementProviderPtr& provider );

  /**
   * Returns true if the projection is planar. This method simply assumes that if the projection
   * is not geographic it is planar.
   */
  static bool isPlanar(const ConstElementProviderPtr& provider ) { return !isGeographic(provider); }

  /**
   * Reprojects the specified geometry from srs1 to srs2. The coordinates of g will change, but
   * nothing else will be modified.
   */
  static void project(const std::shared_ptr<geos::geom::Geometry>& g,
                      const std::shared_ptr<OGRSpatialReference>& srs1,
                      const std::shared_ptr<OGRSpatialReference>& srs2);

  static void projectToAeac(const std::shared_ptr<OsmMap>& map);

  /**
   * Picks the center of the orthographic projection as the center of the map bounds. Should be
   * a reasonable projection for small areas. Units are in meters.
   */
  static void projectToOrthographic(const std::shared_ptr<OsmMap>& map);

  static void projectToOrthographic(const std::shared_ptr<OsmMap>& map, const OGREnvelope& env);

  /**
   * Uses createPlanarProjection to create a planar projection and then reprojects the given map.
   * Uses the envelope of the map to determine the projection.
   */
  static void projectToPlanar(const std::shared_ptr<OsmMap>& map);

  /**
   * Uses createPlanarProjection to create a planar projection and then reprojects the given map.
   * Uses the provided envelope to determine the projection.
   */
  static void projectToPlanar(const std::shared_ptr<OsmMap>& map, const OGREnvelope& env);

  static void projectToWgs84(const std::shared_ptr<OsmMap>& map);

  /**
   * Very slow convenience function.
   */
  static geos::geom::Coordinate projectFromWgs84(const geos::geom::Coordinate& c,
                                                 const std::shared_ptr<OGRSpatialReference>& srs);

  /**
   * Very slow convenience function.
   */
  static geos::geom::Coordinate project(const geos::geom::Coordinate& c,
                                        const std::shared_ptr<OGRSpatialReference>& srs1,
                                        const std::shared_ptr<OGRSpatialReference>& srs2);

  static QString toWkt(const std::shared_ptr<OGRSpatialReference>& srs) { return toWkt(srs.get()); }
  static QString toWkt(const OGRSpatialReference* srs);

private:

  struct PlanarTestResult
  {
    size_t i;
    Meters distanceError;
    Radians angleError;
    double score;

    PlanarTestResult()
      : i(-1),
        distanceError(0.0),
        angleError(0.0),
        score(0.0)
    {
    }

    QString toString() const
    {
      return
        "Distance Error: " + QString::number(distanceError) + ", Angle Error: " +
        QString::number(angleError) + ", Score: " + QString::number(score);
    }
  };

  static std::shared_ptr<MapProjector> _theInstance;

  /** Default constructor/destructor */
  MapProjector() = default;
  ~MapProjector() = default;
  /** Delete copy constructor and assignment operator */
  MapProjector(const MapProjector&) = delete;
  MapProjector& operator=(const MapProjector&) = delete;

  static bool _angleLessThan(const PlanarTestResult& p1, const PlanarTestResult& p2);

  Radians _calculateAngle(geos::geom::Coordinate p1, geos::geom::Coordinate p2,
                          geos::geom::Coordinate p3) const;

  static bool _distanceLessThan(const PlanarTestResult& p1, const PlanarTestResult& p2);

  bool _evaluateProjection(
    const OGREnvelope& env, const std::shared_ptr<OGRSpatialReference>& srs, Meters testDistance,
    Meters& maxDistanceError, Radians& maxAngleError) const;

  size_t _findBestScore(const std::vector<PlanarTestResult>& results) const;

  static bool _scoreLessThan(const PlanarTestResult& p1, const PlanarTestResult& p2);
};

class ReprojectCoordinateFilter : public geos::geom::CoordinateFilter
{
public:

  ReprojectCoordinateFilter(OGRCoordinateTransformation* t);

  void filter_rw(geos::geom::Coordinate* c) const override;

  void project(geos::geom::Coordinate* c) const;

private:

  OGRCoordinateTransformation* _transform;
};

}

#endif
