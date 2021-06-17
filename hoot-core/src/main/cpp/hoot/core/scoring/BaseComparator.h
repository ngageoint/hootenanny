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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef BASE_COMPARATOR_H
#define BASE_COMPARATOR_H

// GDAL Includes
#include <ogrsf_frmts.h>

// GEOS
#include <geos/geom/Coordinate.h>

// Hoot
#include <hoot/core/util/OpenCv.h>
#include <hoot/core/util/Units.h>

// Qt
#include <QRgb>
#include <QString>

// TGS
#include <tgs/RasterOps/Image.hpp>

// Standard
#include <memory>

namespace hoot
{

class OsmMap;

/**
 * Base class for map comparators which determine a particular similarity score for two maps
 */
class BaseComparator
{
public:

  /**
   * Takes two maps for comparison as input
   */
  BaseComparator(const std::shared_ptr<OsmMap>& map1, const std::shared_ptr<OsmMap>& map2);
  virtual ~BaseComparator() = default;

  virtual double compareMaps() = 0;

  void setPixelSize(double pixelSize) { _pixelSize = pixelSize; }

protected:

  int _width, _height;

  std::shared_ptr<OsmMap> _map1, _map2;
  std::shared_ptr<OsmMap> _mapP1, _mapP2;

  Meters _pixelSize;

  OGREnvelope _worldBounds;
  OGREnvelope _projectedBounds;

  Meters _sigma;

  int _taskStatusUpdateInterval;

  virtual void _init(const std::shared_ptr<OsmMap>& map1, const std::shared_ptr<OsmMap>& map2);
  void _updateBounds();

  double _calculateError(const cv::Mat& image1, const cv::Mat& image2) const;

  geos::geom::Coordinate _findNearestPointOnFeature(
    const std::shared_ptr<OsmMap>& map, const geos::geom::Coordinate& c);

  void _calculateColor(double v, double max, QRgb& c) const;
  /**
   * Calculates rings on 10min intervals.
   */
  void _calculateRingColor(double v, double max, QRgb& c) const;

  void _saveImage(cv::Mat& image, QString path, double max = 0.0, bool gradient = true) const;
};

}

#endif // BASE_COMPARATOR_H
