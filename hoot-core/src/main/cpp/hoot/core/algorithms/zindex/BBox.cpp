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
 * @copyright Copyright (C) 2016, 2017, 2021 Maxar (http://www.maxar.com/)
 */
#include "BBox.h"

//std includes
#include <limits>
#include <math.h>
#include <stdlib.h>

//hoot includes


using namespace geos::geom;
using namespace std;

namespace hoot
{

BBox::BBox(const vector<double>& min, const vector<double>& max)
  : _min(min),
    _max(max)
{
  _check();
}

BBox::BBox(const Envelope& envelope)
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

bool BBox::in(const vector<double>& p) const
{
  if (p.size() < _min.size() || p.size() < _max.size())
    throw HootException("Input vector size is less than min or max size.");

  bool result = true;
  for (uint i = 0; i < _min.size(); i++)
    result = result && (p[i] >= _min[i]) && (p[i] <= _max[i]);

  return result;
}

bool BBox::intersects(const BBox& b) const
{
  bool result = true;
  for (uint i = 0; i < _min.size(); i++)
  {
      result = result && (b.getMin()[i] <= getMax()[i]);
      result = result && (b.getMax()[i] >= getMin()[i]);
  }

  return result;
}

QString BBox::toString() const
{
  QString result = "{ ";
  for (uint i = 0; i < _min.size(); i++)
    result += QString::number(_min[i]) + " : " + QString::number(_max[i]) + " ";
  result += "}";
  return result;
}

double BBox::getWidth(int d) const
{
  if (d > (int)_min.size() || d > (int)_max.size())
    throw HootException("Index is greater than min or max size.");
  return _max[d] - _min[d];
}

void BBox::_check() const
{
  for (uint i = 0; i < _min.size(); i++)
  {
    if (_min[i] > _max[i])
      throw HootException("Min is greater than max. " + toString());
  }
}

}

