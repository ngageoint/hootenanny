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
 * @copyright Copyright (C) 2015, 2021 Maxar (http://www.maxar.com/)
 */
#include "AffineTransform.h"

//Std Includes
#include <typeinfo>

namespace tbs
{

AffineTransform::AffineTransform()
{
  try
  {
    for(int i = 0; i < 6; i++)_transform[i] = 0.0;
  }
  catch(const std::exception& e)
  {
    std::stringstream ss;
    ss <<  typeid(this).name() << "::" << __FUNCTION__ << " Line(" << __LINE__ <<
      ") -->" << std::endl << e.what();
    throw Exception(ss.str());
  }
}

AffineTransform::AffineTransform(double originX, double originY, double cellWidth,
  double cellHeight)
{
  try
  {
    _transform[0] = originX;
    _transform[1] = cellWidth;
    _transform[2] = 0.0;
    _transform[3] = originY;
    _transform[4] = 0.0;
    _transform[5] = cellHeight;
  }
  catch(const std::exception& e)
  {
    std::stringstream ss;
    ss <<  typeid(this).name() << "::" << __FUNCTION__ << " Line(" << __LINE__ <<
      ") -->" << std::endl << e.what();
    throw Exception(ss.str());
  }
}

BoundingBox<double> AffineTransform::convertGeoToGrid(BoundingBox<double> geoBounds)
{
  try
  {
    BoundingBox<double> gridBounds(2);

    std::vector<Point2<double> > cornerPoints(4);

    //NW
    cornerPoints[0] = Point2<double>(geoBounds.getMinimumValue(DIM_X),
      geoBounds.getMaximumValue(DIM_Y));

    //NE
    cornerPoints[1] = Point2<double>(geoBounds.getMaximumValue(DIM_X),
      geoBounds.getMaximumValue(DIM_Y));

    //SE
    cornerPoints[2] = Point2<double>(geoBounds.getMaximumValue(DIM_X),
      geoBounds.getMinimumValue(DIM_Y));

    //SW
    cornerPoints[3] = Point2<double>(geoBounds.getMinimumValue(DIM_X),
      geoBounds.getMinimumValue(DIM_Y));

    for(unsigned int i = 0; i < cornerPoints.size(); i++)
    {
      Point2<double> geoPoint = convertGeoToGrid(cornerPoints[i]);
      gridBounds.updateRange(DIM_X, geoPoint.x);
      gridBounds.updateRange(DIM_Y, geoPoint.y);
    }

    gridBounds.clampToInteger();

    gridBounds.updateRange(DIM_X, gridBounds.getMaximumValue(DIM_X) - 1);
    gridBounds.updateRange(DIM_Y, gridBounds.getMaximumValue(DIM_Y) - 1);

    return gridBounds;
  }
  catch(const std::exception& e)
  {
    std::stringstream ss;
    ss <<  typeid(this).name() << "::" << __FUNCTION__ << " Line(" << __LINE__ <<
      ") -->" << std::endl << e.what();
    throw Exception(ss.str());
  }
}

Point2<double> AffineTransform::convertGeoToGrid(Point2<double> geoPoint)
{
  try
  {
    Point2<double> gridPoint;
    gridPoint.x = (geoPoint.x - _transform[0]) / _transform[1];
    gridPoint.y = (geoPoint.y - _transform[3]) / _transform[5];
    return gridPoint;
  }
  catch(const std::exception& e)
  {
    std::stringstream ss;
    ss <<  typeid(this).name() << "::" << __FUNCTION__ << " Line(" << __LINE__ <<
      ") -->" << std::endl << e.what();
    throw Exception(ss.str());
  }
}

Point2<double> AffineTransform::convertGridToGeo(Point2<double> gridPoint)
{
  try
  {
    Point2<double> geoPoint;
    geoPoint.x = _transform[0] + gridPoint.x * _transform[1];
    geoPoint.y = _transform[3] + gridPoint.y * _transform[5];
    return geoPoint;
  }
  catch(const std::exception& e)
  {
    std::stringstream ss;
    ss <<  typeid(this).name() << "::" << __FUNCTION__ << " Line(" << __LINE__ <<
      ") -->" << std::endl << e.what();
    throw Exception(ss.str());
  }
}

}
