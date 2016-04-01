#ifndef __MAP_REPROJECTOR_H__
#define __MAP_REPROJECTOR_H__


// Boost
#include <boost/shared_ptr.hpp>

// GDAL
#include <ogr_spatialref.h>

// GEOS
#include <geos/geom/Coordinate.h>
#include <geos/geom/Geometry.h>

// Hoot
namespace hoot {
  class OsmMap;
}

namespace hoot
{
  using namespace boost;
  using namespace geos::geom;

class MapProjector
{
public:

  /**
   * Given a bounding box in WGS84, create a reasonable planar projection for that region. The
   * units are in meters.
   */
  static shared_ptr<OGRSpatialReference> createPlanarProjection(const OGREnvelope& env);

  static void reproject(shared_ptr<OsmMap> map,
                        shared_ptr<OGRSpatialReference> ref);

  static bool isGeographic(const shared_ptr<const OsmMap>& map);

  /**
   * Reprojects the specified geometry from srs1 to srs2. The coordinates of g will change, but
   * nothing else will be modified.
   */
  static void reproject(const shared_ptr<Geometry>& g, const shared_ptr<OGRSpatialReference>& srs1,
    const shared_ptr<OGRSpatialReference>& srs2);

  /**
   * Picks the center of the orthographic projection as the center of the map bounds. Should be
   * a reasonable projection for small areas. Units are in meters.
   */
  static void reprojectToOrthographic(shared_ptr<OsmMap> map);

  static void reprojectToOrthographic(shared_ptr<OsmMap> map, const OGREnvelope& env);

  /**
   * Uses createPlanarProjection to create a planar projection and then reprojects the given map.
   * Uses the envelope of the map to determine the projection.
   */
  static void reprojectToPlanar(shared_ptr<OsmMap> map);

  /**
   * Uses createPlanarProjection to create a planar projection and then reprojects the given map.
   * Uses the provided envelope to determine the projection.
   */
  static void reprojectToPlanar(shared_ptr<OsmMap> map, const OGREnvelope& env);

  static void reprojectToWgs84(shared_ptr<OsmMap> map);

  /**
   * Very slow convenience function.
   */
  static Coordinate reprojectFromWgs84(const Coordinate& c,
                                       shared_ptr<OGRSpatialReference> srs);

  /**
   * Very slow convenience function.
   */
  static Coordinate reproject(const Coordinate& c, shared_ptr<OGRSpatialReference> srs1,
                              shared_ptr<OGRSpatialReference> srs2);
};

}

#endif
