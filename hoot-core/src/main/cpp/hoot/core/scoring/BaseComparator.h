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
 * @copyright Copyright (C) 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef BASE_COMPARATOR_H
#define BASE_COMPARATOR_H

// GDAL Includes
#include <ogrsf_frmts.h>

// GEOS
#include <geos/geom/Coordinate.h>

// Hoot
#include <hoot/core/util/Units.h>
#include <hoot/core/util/OpenCv.h>

// Qt
#include <QRgb>
#include <QString>

// TGS
#include <tgs/SharedPtr.h>
#include <tgs/RasterOps/Image.hpp>

namespace hoot
{

class OsmMap;

/**
 * Compares two OSM maps for consistency of vector location and density. The resulting score is a
 * value from 0 to 1 where 1 is an exact match and 0 is completely different.
 */
class BaseComparator
{
public:
  /**
   * Takes two maps for comparison as input
   */
  BaseComparator(boost::shared_ptr<OsmMap> map1, boost::shared_ptr<OsmMap> map2);

  virtual ~BaseComparator() {}

  void setPixelSize(double pixelSize) { _pixelSize = pixelSize; }

  virtual double compareMaps() = 0;

protected:

  int _width, _height;
  boost::shared_ptr<OsmMap> _map1, _map2;
  boost::shared_ptr<OsmMap> _mapP1, _mapP2;
  Meters _pixelSize;
  OGREnvelope _worldBounds;
  OGREnvelope _projectedBounds;
  Meters _sigma;

  double _calculateError(const cv::Mat& image1, const cv::Mat& image2);

  geos::geom::Coordinate _findNearestPointOnFeature(boost::shared_ptr<OsmMap> map, geos::geom::Coordinate c);

  virtual void _init(boost::shared_ptr<OsmMap> map1, boost::shared_ptr<OsmMap> map2);

  void _saveImage(cv::Mat& image, QString path, double max = 0.0, bool gradient = true);

  void _updateBounds();

  void _calculateColor(double v, double max, QRgb& c);

  /**
   * Calculates rings on 10min intervals.
   */
  void _calculateRingColor(double v, double max, QRgb& c);
};

}

#endif // BASE_COMPARATOR_H
