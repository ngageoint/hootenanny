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
#include "ZValue.h"

//Std Includes
#include <math.h>

//Hoot Includes
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>

namespace hoot
{

ZValue::ZValue(int dimensions, int depth, vector<double> min, vector<double> max)
{
  _depth = depth;
  _dimensions = dimensions;
  _min = min;
  _max = max;
  _range = (1 << _depth) - 1;
  _b.reserve(dimensions);
  for (int i = 0; i < dimensions; i++)
  {
    _b.push_back(0);;
  }
}

ZValue::~ZValue()
{
  _b.clear();
  _min.clear();
  _max.clear();
}

long int ZValue::calculate(vector<double> point)
{
  for (int i = 0; i < _dimensions; i++)
  {
    _b[i] = calculateComponent(point[i], i);
  }
  return calculate(_b);
}

long int ZValue::calculate(vector<long int> point)
{
  long int bitRead = 1 << (_depth - 1);
  long int result = 0;
  for (int depth = 0; depth < _depth; depth++)
  {
    // reverse the order so it looks like a "z" and makes it consistent
    // with the Wikipedia definition.
    for (int i = _dimensions - 1; i >= 0; i--)
    {
      long int bit = ((point[i] & bitRead) != 0) ? 1 : 0;
      result = (result << 1) | bit;
    }
    bitRead = bitRead >> 1;
  }
  return result;
}

long int ZValue::calculateComponent(double v, int d)
{
  if (d >= (int)_min.size() || d >= (int)_max.size())
  {
    throw HootException("Input vector size is greater than min or max size.");
  }
  return round(((v - _min[d]) / (_max[d] - _min[d])) * _range);
}

void ZValue::decompose(long int v, vector<long int>& point)
{
  for (uint i = 0; i < point.size(); i++)
  {
    point[i] = 0;
  }

  long bitRead = 1 << ((_depth * _dimensions) - 1);
  for (int depth = 0; depth < _depth; depth++)
  {
    // reverse the order so it looks like a "z" and makes it consistent
    // with the Wikipedia definition.
    for (int i = _dimensions - 1; i >= 0; i--)
    {
      long bit = ((v & bitRead) != 0) ? 1 : 0;
      point[i] = (point[i] << 1) | bit;
      bitRead = bitRead >> 1;
    }
  }
}

}

