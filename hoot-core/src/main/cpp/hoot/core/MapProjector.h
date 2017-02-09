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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef __MAP_PROJECTOR_H__
#define __MAP_PROJECTOR_H__


// Boost
#include <boost/shared_ptr.hpp>

// GDAL
#include <ogr_spatialref.h>

// GEOS
#include <geos/geom/Coordinate.h>
#include <geos/geom/CoordinateFilter.h>
#include <geos/geom/Geometry.h>

// Hoot
#include <hoot/core/Units.h>
#include <hoot/core/elements/ElementProvider.h>

// Qt
#include <QString>

// Standard
#include <vector>

namespace hoot
{
class OsmMap;

  using namespace boost;
  using namespace geos::geom;
  using namespace std;

class MapProjector
{
public:

  static std::string className() { return "hoot::MapProjector"; }

  static unsigned int logWarnCount;

  /**
   * Given a bounding box in WGS84, create a reasonable planar projection for that region. The
   * units are in meters.
   */
  static shared_ptr<OGRSpatialReference> createAeacProjection(const OGREnvelope& env);

  static shared_ptr<OGRSpatialReference> createOrthographic(const OGREnvelope& env);

  /**
   * Given a bounding box in WGS84, create a reasonable planar projection for that region. The
   * units are in meters.
   */
  static shared_ptr<OGRSpatialReference> createSinusoidalProjection(const OGREnvelope& env);

  static shared_ptr<OGRSpatialReference> createWgs84Projection();

  static void project(shared_ptr<OsmMap> map, shared_ptr<OGRSpatialReference> ref);

  /**
   * Returns a vector of all candidate planar projections for a given envelope.
   */
  vector< shared_ptr<OGRSpatialReference> > createAllPlanarProjections(const OGREnvelope& env);

  /**
   * Using a predefined set of projections this method evaluates each one of them for both distance
   * and angular error over the given envelope. The best projection is returned.
   *
   * If no projections are found that meet both thresholds and warnOnFail == false then an error
   * is thrown. Otherwise, the best projection is returned.
   *
   * Best is an internal heuristic that is subject to change.
   */
  shared_ptr<OGRSpatialReference> createPlanarProjection(const OGREnvelope& env,
    Radians maxAngleError = toRadians(2.0), Meters maxDistanceError = 10.0,
    Meters testDistance = 1000.0, bool warnOnFail = true);

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
  static void project(const shared_ptr<Geometry>& g, const shared_ptr<OGRSpatialReference>& srs1,
    const shared_ptr<OGRSpatialReference>& srs2);

  static void projectToAeac(shared_ptr<OsmMap> map);

  /**
   * Picks the center of the orthographic projection as the center of the map bounds. Should be
   * a reasonable projection for small areas. Units are in meters.
   */
  static void projectToOrthographic(shared_ptr<OsmMap> map);

  static void projectToOrthographic(shared_ptr<OsmMap> map, const OGREnvelope& env);

  /**
   * Uses createPlanarProjection to create a planar projection and then reprojects the given map.
   * Uses the envelope of the map to determine the projection.
   */
  static void projectToPlanar(shared_ptr<OsmMap> map);

  /**
   * Uses createPlanarProjection to create a planar projection and then reprojects the given map.
   * Uses the provided envelope to determine the projection.
   */
  static void projectToPlanar(shared_ptr<OsmMap> map, const OGREnvelope& env);

  static void projectToWgs84(shared_ptr<OsmMap> map);

  /**
   * Very slow convenience function.
   */
  static Coordinate projectFromWgs84(const Coordinate& c,
                                       shared_ptr<OGRSpatialReference> srs);

  /**
   * Very slow convenience function.
   */
  static Coordinate project(const Coordinate& c, shared_ptr<OGRSpatialReference> srs1,
                              shared_ptr<OGRSpatialReference> srs2);

  static QString toWkt(shared_ptr<OGRSpatialReference> srs) { return toWkt(srs.get()); }
  static QString toWkt(OGRSpatialReference* srs);

private:

  struct PlanarTestResult {
    size_t i;
    Meters distanceError;
    Radians angleError;
    double score;
  };

  MapProjector() {}

  static shared_ptr<MapProjector> _theInstance;

  static bool _angleLessThan(const PlanarTestResult& p1, const PlanarTestResult& p2);

  Radians _calculateAngle(Coordinate p1, Coordinate p2, Coordinate p3);

  static bool _distanceLessThan(const PlanarTestResult& p1, const PlanarTestResult& p2);

  bool _evaluateProjection(const OGREnvelope& env, shared_ptr<OGRSpatialReference> srs,
    Meters testDistance, Meters& maxDistanceError, Radians& maxAngleError);

  size_t _findBestResult(vector<PlanarTestResult>& results);

  size_t _findBestScore(vector<PlanarTestResult>& results);

  static bool _scoreLessThan(const PlanarTestResult& p1, const PlanarTestResult& p2);
};

class ReprojectCoordinateFilter : public CoordinateFilter
{
public:

  ReprojectCoordinateFilter(OGRCoordinateTransformation* t);

  virtual void filter_rw(Coordinate* c) const;

  void project(Coordinate* c) const;

private:
  OGRCoordinateTransformation* _transform;
};

}

#endif
