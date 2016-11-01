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
 * @copyright Copyright (C) 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "BBox.h"

//std includes
#include <math.h>
#include <stdlib.h>
#include <limits>

//hoot includes
#include <hoot/core/util/HootException.h>

namespace hoot
{

BBox::BBox(vector<double> min, vector<double> max)
{
  _min = min;
  _max = max;
  _check();
}

BBox::BBox(Envelope envelope)
{
  _min.reserve(2);
  _max.reserve(2);

  _min.push_back(envelope.getMinX());
  _min.push_back(envelope.getMinY());
  _max.push_back(envelope.getMaxX());
  _max.push_back(envelope.getMaxY());
}

BBox::~BBox()
{
  _min.clear();
  _max.clear();
}

bool BBox::in(vector<double> p)
{
  if (p.size() < _min.size() || p.size() < _max.size())
  {
    throw HootException("Input vector size is less than min or max size.");
  }

  bool result = true;
  for (uint i = 0; i < _min.size(); i++)
  {
    result = result && (p[i] >= _min[i]) && (p[i] <= _max[i]);
  }
  return result;
}

bool BBox::in(BBox container)
{
  bool result = true;
  for (uint i = 0; i < _min.size(); i++)
  {
    result = result && (_min[i] >= container.getMin()[i]);
    result = result && (_max[i] <= container.getMax()[i]);
  }

  return result;
}

bool BBox::intersects(BBox b)
{
  bool result = true;
  for (uint i = 0; i < _min.size(); i++)
  {
      result = result && (b.getMin()[i] <= getMax()[i]);
      result = result && (b.getMax()[i] >= getMin()[i]);
  }

  return result;
}

double BBox::manhattanDistance(BBox b)
{
  if (intersects(b))
  {
    return 0.0;
  }

  double d = numeric_limits<double>::max();
  for (uint i = 0; i < _min.size(); i++)
  {
    d = min(d, fabs(_min[i] - b.getMin()[i]));
    d = min(d, fabs(_min[i] - b.getMax()[i]));
    d = min(d, fabs(_max[i] - b.getMin()[i]));
    d = min(d, fabs(_max[i] - b.getMax()[i]));
  }
  return d;
}

QString BBox::toString()
{
  QString result = "{ ";
  for (uint i = 0; i < _min.size(); i++)
  {
    result += QString::number(_min[i]) + " : " + QString::number(_max[i]) + " ";
  }
  result += "}";
  return result;
}

double BBox::getWidth(int d)
{
  if (d > (int)_min.size() || d > (int)_max.size())
  {
    throw HootException("Index is greater than min or max size.");
  }
  return _max[d] - _min[d];
}

void BBox::_check()
{
  for (uint i = 0; i < _min.size(); i++)
  {
    if (_min[i] > _max[i])
    {
      throw HootException("Min is greater than max. " + toString());
    }
  }
}

}

