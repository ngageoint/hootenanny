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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "Histogram.h"

// hoot
#include <tgs/Statistics/Normal.h>
#include <hoot/core/algorithms/WayHeading.h>

// geos
#include <geos/geom/Geometry.h>

// Standard
#include <assert.h>
#include <math.h>

namespace hoot
{

Histogram::Histogram(int bins)
{
  _bins.resize(bins, 0.0);
}

void Histogram::addAngle(Radians theta, double length)
{
  _bins[getBin(theta)] += length;
}

double Histogram::diff(Histogram& other)
{
  assert(_bins.size() == other._bins.size());
  double diff = 0.0;
  for (size_t i = 0; i < _bins.size(); i++)
  {
    diff += fabs(_bins[i] - other._bins[i]);
  }

  return diff / 2.0;
}

size_t Histogram::getBin(Radians theta)
{
//  const double c2pi = 2 * M_PI;
//  // bring theta into 0 - 2pi if it is outside those bounds.
//  theta = theta - c2pi * floor(theta / c2pi);
//  return std::max<size_t>(0,
//    std::min<size_t>(_bins.size() - 1, (theta / c2pi) * _bins.size()));
  while (theta < 0.0)
  {
    theta += 2 * M_PI;
  }
  return (theta / (2 * M_PI)) * _bins.size();

}

Radians Histogram::_getBinAngle(size_t i)
{
  return 2 * M_PI / _bins.size() * i + M_PI / _bins.size();
}

void Histogram::normalize()
{
  double sum = 0.0;
  for (size_t i = 0; i < _bins.size(); i++)
  {
    sum += _bins[i];
  }

  if (sum <= 0.0)
  {
    sum = 1.0;
  }

  for (size_t i = 0; i < _bins.size(); i++)
  {
    _bins[i] /= sum;
  }
}

void Histogram::smooth(Radians sigma)
{
  vector<double> copy(_bins.size(), 0.0);

  for (size_t i = 0; i < copy.size(); ++i)
  {
    for (size_t j = 0; j < _bins.size(); j++)
    {
      double diff = WayHeading::deltaMagnitude(_getBinAngle(i), _getBinAngle(j));
      double weight = Tgs::Normal::normal(diff, sigma);
      copy[i] += _bins[j] * weight;
    }
  }

  _bins = copy;
}

}
