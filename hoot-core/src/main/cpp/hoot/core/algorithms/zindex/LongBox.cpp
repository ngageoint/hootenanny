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
#include "LongBox.h"


//std includes
#include <math.h>

//hoot includes
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>

namespace hoot
{

LongBox::LongBox(vector<long int> min, vector<long int> max)
{
  setMin(min);
  setMax(max);
}

shared_ptr<LongBox> LongBox::copy()
{
  shared_ptr<LongBox> box = shared_ptr<LongBox>(new LongBox(getMin(), getMax()));
  return box;
}

LongBox::~LongBox()
{
  _min.clear();
  _max.clear();
}

long int LongBox::calculateVolume()
{
  long result = 1;
  for (uint i = 0; i < getMin().size(); i++)
  {
    result *= getWidth(i);
  }
  return result;
}

bool LongBox::edgeOverlaps(LongBox b)
{
  bool result = false;
  for (uint i = 0; i < getMin().size(); i++)
  {
    result = result || (b.getMin()[i] == getMax()[i]) || (b.getMin()[i] == b.getMin()[i]) || (b.getMax()[i] == b.getMax()[i]);
  }
  return result;
}

LongBox LongBox::expand(int size)
{
  vector<long int> min = getMin();
  vector<long int> max = getMax();
  for (uint i = 0; i < getMin().size(); i++)
  {
    min[i] -= size;
    max[i] += size;
  }

  return LongBox(min, max);
}

bool LongBox::in(vector<long int> p)
{
  if (p.size() < _min.size() || p.size() < _max.size())
  {
    throw HootException("Input vector size is less than min or max size.");
  }

  bool result = true;
  for (uint i = 0; i < getMin().size(); i++)
  {
    result = result && (p[i] >= getMin()[i]) && (p[i] <= getMax()[i]);
  }
  return result;
}

bool LongBox::intersects(LongBox b)
{
  bool result = true;
  for (uint i = 0; i < getMin().size(); i++)
  {
    result = result && (b.getMin()[i] <= getMax()[i]);
    result = result && (b.getMax()[i] >= getMin()[i]);
  }

  return result;
}

QString LongBox::toString()
{
  QString result = "{ ";
  for (uint i = 0; i < _min.size(); i++)
  {
    result += "( " + QString::number(getMin()[i]) + " : " + QString::number(getMax()[i]) + ") ";
  }
  result += "}";
  return result;
}

long LongBox::getWidth(int d)
{
  if (d > (int)getMin().size() || d > (int)getMax().size())
  {
    throw HootException("Index is greater than min or max size.");
  }
  return getMax()[d] - getMin()[d] + 1;
}

}
