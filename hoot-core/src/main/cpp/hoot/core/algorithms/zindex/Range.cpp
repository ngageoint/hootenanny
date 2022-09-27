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
 * @copyright Copyright (C) 2016, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#include "Range.h"

//  std
#include <stdio.h>

namespace hoot
{

Range::Range(long int min, long int max)
{
  set(min,max);
}

bool Range::hashCode() const
{
  int result = (int) (_max ^ (_max >> 32));
  result = (31 * result) + (int) (_min ^ (_min >> 32));
  return result;
}

bool Range::in(long int l) const
{
  return (l >= _min) && (l <= _max);
}

bool Range::isValid() const
{
  return (_min >= 0) && (_max >= 0);
}

void Range::setInvalid()
{
  _min = -1;
  _max = -1;
}

QString Range::toString() const
{
  return "[" + QString::number(_min) + " : " + QString::number(_max) + "]";
}

void Range::set(long int min, long int max)
{
  if (min > max)
    throw HootException("min is greater than max: " + QString::number(min) + " " +  QString::number(max));
  _min = min;
  _max = max;
}

}

