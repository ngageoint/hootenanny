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
 * @copyright Copyright (C) 2015, 2021, 2022 Maxar (http://www.maxar.com/)
 */

#ifndef AFFINETRANSFORM_H
#define AFFINETRANSFORM_H


//Std Includes
#include <algorithm>
#include <sstream>

#include "../../Utilities/BoundingBox.h"
#include "../../Utilities/Exception.h"
#include "../../Utilities/Point2.h"

namespace tbs
{

/**
 * @brief The AffineTransform class
 *
 * Converts values between raster coordinates and geospatial coordinates as defined in GDAL
 *
 * Basic structure
 * AT[0] = X origin coodinate (geospatial units)
 * AT[1] = X cell width (geospatial units / pixel)
 * AT[2] = 0
 * AT[3] = Y origin coodinate (geospatial units)
 * AT[4] = 0
 * AT[5] = Y cell height (geospatial units / pixel)
 *
 * Typical usage is the origin is the upper left corner as minimum X and maximum Y and
 * the Y cell width is then negative
 */
class AffineTransform
{
public:
  /**
   * @brief Constructor
   */
  AffineTransform();

  /**
   * @brief Constructor
   * @param originX the X origin coordinate (geospatial units)
   * @param originY the Y origin coordinate (geospatial units)
   * @param cellWidth the cell width (projection units/pixel)
   * @param cellHeight the cell height (projection units/pixel)
   */
  AffineTransform(double originX, double originY, double cellWidth, double cellHeight);

  /**
   * @brief convertGeoToGrid
   *
   * The values are clamped to the integer values and 1 is subtracted from the max
   * values so the max grid value is the maximum pixel coordinate in the array
   *
   * For example an array with an origin of (0,0) and 10 pixel wide will have an output X bounds of (0,9)
   *
   * @param geoBounds the source geospatial bounding box (projection units)
   * @return the converted grid bounding box (pixels)
   */
  BoundingBox<double> convertGeoToGrid(BoundingBox<double> geoBounds);

  /**
   * @brief convertGeoToGrid
   * @param geoPoint the source geospatial coordinate (projection units)
   * @return the converted grid point (pixels)
   */
  Point2<double> convertGeoToGrid(Point2<double> geoPoint);

  /**
   * @brief Converts a grid point to a geospatial point
   * @param gridPoint the source grid point (pixels)
   * @return the converted geospatial point (projection units)
   */
  Point2<double> convertGridToGeo(Point2<double> gridPoint);

  /**
   * @brief Converts a grid point to a geospatial point
   * @param gridPoint the source grid point (pixels)
   * @return the converted geospatial point (projection units)
   */
  Point2<double> convertGridToGeo(Point2<double> gridPoint);
private:
  double _transform[6]; 
 
};

}

#endif
