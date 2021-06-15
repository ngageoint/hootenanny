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

#include "BaseComparator.h"

// GEOS
#include <geos/geom/LineString.h>
#include <geos/geom/Point.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/operation/distance/DistanceOp.h>

// Hoot
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/geometry/ElementToGeometryConverter.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/util/OpenCv.h>
#include <hoot/core/visitors/CalculateMapBoundsVisitor.h>

// Qt
#include <QDebug>
#include <QImage>
#include <QPainter>

using namespace geos::geom;
using namespace geos::operation::distance;

namespace hoot
{

BaseComparator::BaseComparator(
  const std::shared_ptr<OsmMap>& map1, const std::shared_ptr<OsmMap>& map2)
{
  _init(map1, map2);
}

void BaseComparator::_calculateColor(double v, double max, QRgb& c) const
{
  if (v >= 0.0)
  {
    if (v == 0.0)
    {
      c = qRgb(50, 50, 50);
    }
    if (v < max / 3.0)
    {
      int r = v / (max / 3.0) * 255.0;
      c = qRgb(r, 0, 0);
    }
    else if (v < max * 2.0 / 3.0)
    {
      int r = (v - (max / 3.0)) / (max / 3.0) * 255.0;
      c = qRgb(255, r, 0);
    }
    else if (v <= max)
    {
      int r = (v - (max * 2.0 / 3.0)) / (max / 3.0) * 255.0;
      c = qRgb(255, 255, r);
    }
    else
    {
      c = qRgb(255, 255, 255);
    }
  }
  else
  {
    c = qRgba(90, 20, 20, 0);
  }
}

double BaseComparator::_calculateError(const cv::Mat& image1, const cv::Mat& image2) const
{
  double errorSum = 0.0;
  double image1Sum = 0.0;
  double image2Sum = 0.0;
  const float* image1Data = image1.ptr<float>(0);
  const float* image2Data = image2.ptr<float>(0);
  size_t size = (image1.dataend - image1.datastart) / sizeof(float);
  for (size_t i = 0; i < size; i++)
  {
    errorSum += fabs(image1Data[i] - image2Data[i]);
    image1Sum += image1Data[i];
    image2Sum += image2Data[i];
  }

  return errorSum / (image1Sum + image2Sum);
}

void BaseComparator::_calculateRingColor(double v, double, QRgb& c) const
{
  double m = v / 60.0;
  if (v >= 0.0)
  {
    if (m >= 0.0 && m <= 10.0)
    {
      c = qRgb(255, 0, 255);
    }
    if (m > 10.0 && m <= 20.0)
    {
      c = qRgb(0, 0, 255);
    }
    else if (m > 20.0 && m <= 30.0)
    {
      c = qRgb(0, 255, 0);
    }
    else if (m > 30.0 && m <= 40.0)
    {
      c = qRgb(255, 255, 0);
    }
    else if (m > 40.0 && m <= 50.0)
    {
      c = qRgb(255, 0, 0);
    }
  }
  else
  {
    c = qRgba(90, 20, 20, 0);
  }
}

Coordinate BaseComparator::_findNearestPointOnFeature(
  const std::shared_ptr<OsmMap>& map, const Coordinate& c)
{
  LOG_TRACE("Finding nearest point to: " << c << "...");

  Coordinate result;

  // find the nearest feature
  long wId = map->getIndex().findNearestWay(c);
  WayPtr w = map->getWay(wId);

  // find the nearest point on that feature.
  std::shared_ptr<Point> p(GeometryFactory::getDefaultInstance()->createPoint(c));
  std::shared_ptr<LineString> ls = ElementToGeometryConverter(map).convertToLineString(w);
  std::unique_ptr<CoordinateSequence> cs = DistanceOp::nearestPoints(p.get(), ls.get());

  cs->getAt(0, result);

  return result;
}

void BaseComparator::_init(const std::shared_ptr<OsmMap>& map1, const std::shared_ptr<OsmMap>& map2)
{
  _map1 = map1;
  _map2 = map2;
  _pixelSize = 10.0;
  _sigma = 10.0;

  // determine the world bounds by looking at all the nodes.
  _worldBounds = OGREnvelope();
  _worldBounds.Merge(CalculateMapBoundsVisitor::getBounds(_map1));
  _worldBounds.Merge(CalculateMapBoundsVisitor::getBounds(_map2));

  _mapP1.reset(new OsmMap(_map1));
  MapProjector::projectToOrthographic(_mapP1, _worldBounds);
  _mapP2.reset(new OsmMap(_map2));
  MapProjector::projectToOrthographic(_mapP2, _worldBounds);
}

void BaseComparator::_saveImage(cv::Mat& image, QString path, double max, bool gradient) const
{
  if (max <= 0.0)
  {
    for (int y = 0; y < _height; y++)
    {
      float* row = image.ptr<float>(y);
      for (int x = 0; x < _width; x++)
      {
        max = std::max((double)row[x], max);
      }
    }
  }

  QImage qImage(_width, _height, QImage::Format_ARGB32);

  QRgb rgb = 0;
  if (max > 0.0)
  {
    for (int y = 0; y < _height; y++)
    {
      float* row = image.ptr<float>(y);
      for (int x = 0; x < _width; x++)
      {
        if (gradient)
        {
          _calculateColor(row[x], max, rgb);
        }
        else
        {
          _calculateRingColor(row[x], max, rgb);
        }
        qImage.setPixel(x, y, rgb);
      }
    }
  }

  qImage.save(path);
}

void BaseComparator::_updateBounds()
{
  _projectedBounds = OGREnvelope();
  _projectedBounds.Merge(CalculateMapBoundsVisitor::getBounds(_mapP1));
  _projectedBounds.Merge(CalculateMapBoundsVisitor::getBounds(_mapP2));
  _projectedBounds.MinX -= _sigma * 2;
  _projectedBounds.MinY -= _sigma * 2;
  _projectedBounds.MaxX += _sigma * 2;
  _projectedBounds.MaxY += _sigma * 2;

  // round the image width and height up
  _width = ceil((_projectedBounds.MaxX - _projectedBounds.MinX) / _pixelSize);
  _height = ceil((_projectedBounds.MaxY - _projectedBounds.MinY) / _pixelSize);

  // adjust the world bounds accordingly.
  _projectedBounds.MaxX = _projectedBounds.MinX + _width * _pixelSize;
  _projectedBounds.MaxY = _projectedBounds.MinY + _height * _pixelSize;
}

}
