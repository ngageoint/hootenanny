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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2021 Maxar (http://www.maxar.com/)
 */
#include "Histogram.h"

// hoot
#include <tgs/Statistics/Normal.h>
#include <hoot/core/algorithms/WayHeading.h>

// geos
#include <geos/geom/Geometry.h>

// hoot
#include <hoot/core/util/Log.h>

// Qt
#include <QStringList>

// Standard
#include <assert.h>
#include <math.h>

// Tgs
#include <tgs/Statistics/Normal.h>

using namespace std;

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

Radians Histogram::getBinCenter(size_t bin) const
{
  assert(bin < _bins.size());

  Radians binSize = 2.0 * M_PI / (double)_bins.size();
  return bin * binSize + binSize / 2.0;
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
  vector<double> old = _bins;

  // This is quite inefficient and can be reworked to cache the normal curve and reuse it as needed.
  for (size_t i = 0; i < _bins.size(); ++i)
  {
    _bins[i] = 0.0;
    Radians center = getBinCenter(i);
    for (size_t j = 0; j < old.size(); ++j)
    {
      Radians rawDiff = fabs(getBinCenter(j) - center);
      Radians diff = std::min(rawDiff, 2 * M_PI - rawDiff);
      _bins[i] += old[j] * Tgs::Normal::normal(diff, sigma);
    }
  }
}

QString Histogram::toString() const
{
  QStringList l;
  for (size_t i = 0; i < _bins.size(); ++i)
  {
    l <<
      QString("%1°: %2")
        .arg(QString::number(toDegrees(getBinCenter(i)), 'g', 6))
        .arg(QString::number(_bins[i], 'g', 6));
  }
  return l.join(", ");
}

QString Histogram::printPositiveBins() const
{
  QStringList l;
  for (size_t i = 0; i < _bins.size(); ++i)
  {
    if (_bins[i] > 0.0)
    {
      l <<
        QString("%1°: %2")
          .arg(QString::number(toDegrees(getBinCenter(i)), 'g', 6))
          .arg(QString::number(_bins[i], 'g', 6));
    }
  }
  return l.join(", ");
}

}
