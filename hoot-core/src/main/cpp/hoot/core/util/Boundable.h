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
 * @copyright Copyright (C) 2015, 2019, 2020, 2021, 2023 Maxar (http://www.maxar.com/)
 */
#ifndef BOUNDABLE_H
#define BOUNDABLE_H

// Hoot
#include <hoot/core/geometry/GeometryUtils.h>

namespace hoot
{

/**
 * A base class for setting a geospatial boundary on an object (used to be a base class but doesn't
 * seem to be causing any problems in inheritors due to MI). The exact meaning of setting the bounds
 * is context dependent. In the case of OsmMapOperation, for example, it limits the scope of the
 * operation. It is important to note that even if the rest of the operation in an implementation is
 * not in WGS84, the bounds itself will still be in WGS84.
 *
 * Currently from a config string we're only supporting Envelope and Polygon geometries (see
 * GeometryUtils::boundsFromString), but that could be expanded. For the implementations that
 * require the bounds to be a rectangular envelope, custom error checking should be added to their
 * configuration parsing to ensure an envelope string is passed in.
 */
class Boundable
{
public:

  Boundable() = default;
  virtual ~Boundable() = default;

  /**
   * Sets the bounds from a geometry in WGS84
   */
  virtual void setBounds(std::shared_ptr<geos::geom::Geometry> bounds) { _bounds = bounds; }

  /**
   * Sets the bounds from an envelope in WGS84
   */
  virtual void setBounds(const geos::geom::Envelope& bounds)
  {
    setBounds(GeometryUtils::envelopeToPolygon(bounds));
  }

  /**
   * @brief loadBounds - Attempts to load bounds from string then from file
   * @param options - Configuration options
   * @param forceEnvelopeBounds - Force the function to return a bounding envelope even if the
   *   geometry is a polygon
   * @return geometry of the bounds or nullptr
   */
  static std::shared_ptr<geos::geom::Geometry> loadBounds(const ConfigOptions& options, bool forceEnvelopeBounds = false);
  static std::shared_ptr<geos::geom::Geometry> loadBounds(bool forceEnvelopeBounds = false)
  {
    return loadBounds(ConfigOptions(), forceEnvelopeBounds);
  }

  /**
   * @brief loadBoundsString - Attempts to load bounds from string
   * @param options - Configuration options
   * @param forceEnvelopeBounds - Force the function to return a bounding envelope even if the
   *   geometry is a polygon
   * @return geometry of the bounds or nullptr
   */
  static std::shared_ptr<geos::geom::Geometry> loadBoundsString(const ConfigOptions& options, bool forceEnvelopeBounds = false);
  static std::shared_ptr<geos::geom::Geometry> loadBoundsString(bool forceEnvelopeBounds = false)
  {
    return loadBoundsString(ConfigOptions(), forceEnvelopeBounds);
  }

  /**
   * @brief loadBoundsFile - Attempts to load bounds from file
   * @param options - Configuration options
   * @param forceEnvelopeBounds - Force the function to return a bounding envelope even if the
   *   geometry is a polygon
   * @return geometry of the bounds or nullptr
   */
  static std::shared_ptr<geos::geom::Geometry> loadBoundsFile(const ConfigOptions& options, bool forceEnvelopeBounds = false);
  static std::shared_ptr<geos::geom::Geometry> loadBoundsFile(bool forceEnvelopeBounds = false)
  {
    return loadBoundsFile(ConfigOptions(), forceEnvelopeBounds);
  }

  /**
   * @brief loadCropBounds - Attempts to load crop bounds from string
   * @param options - Configuration options
   * @param forceEnvelopeBounds - Force the function to return a bounding envelope even if the
   *   geometry is a polygon
   * @return geometry of the bounds or nullptr
   */
  static std::shared_ptr<geos::geom::Geometry> loadCropBounds(const ConfigOptions& options, bool forceEnvelopeBounds = false);
  static std::shared_ptr<geos::geom::Geometry> loadCropBounds(bool forceEnvelopeBounds = false)
  {
    return loadCropBounds(ConfigOptions(), forceEnvelopeBounds);
  }

  /**
   * @brief loadCropBounds - Attempts to load crop bounds from string
   * @param options - Configuration options
   * @param forceEnvelopeBounds - Force the function to return a bounding envelope even if the
   *   geometry is a polygon
   * @return geometry of the bounds or nullptr
   */
  static std::shared_ptr<geos::geom::Geometry> loadInBoundsCriterionBounds(const ConfigOptions& options, bool forceEnvelopeBounds = false);
  static std::shared_ptr<geos::geom::Geometry> loadInBoundsCriterionBounds(bool forceEnvelopeBounds = false)
  {
    return loadInBoundsCriterionBounds(ConfigOptions(), forceEnvelopeBounds);
  }

protected:
  /** Bounding envelope or polygon */
  std::shared_ptr<geos::geom::Geometry> _bounds;

private:
  /** Convert string to geometry, used by loadBoundsString, loadCropBounds, and loadInBoundsCriterionBounds functions */
  static std::shared_ptr<geos::geom::Geometry> loadBoundsByString(const QString& bounds, bool forceEnvelopeBounds);
};

}

#endif // BOUNDABLE_H
