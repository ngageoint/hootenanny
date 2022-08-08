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
 * @copyright Copyright (C) 2015, 2017, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#include "SampleStats.h"

// Standard
#include <algorithm>
#include <iostream>
#include <limits>
#include <math.h>

using namespace std;

namespace tbs
{

const double zalpha90 = 1.645;

SampleStats::SampleStats(const vector<double>& samples) :
  _samples(samples)
{
  _max = -numeric_limits<double>::max();
  _mean = -numeric_limits<double>::max();
  _median = -numeric_limits<double>::max();
  _min = -numeric_limits<double>::max();
  _sum = -numeric_limits<double>::max();
  _standardDeviation = -numeric_limits<double>::max();
}

double SampleStats::calculateCi90Upper()
{
  return calculateMean() + zalpha90 * calculateUnbiasedStandardDeviation() / sqrt(_samples.size());
}

double SampleStats::calculateCi90Lower()
{
  return calculateMean() - zalpha90 * calculateUnbiasedStandardDeviation() / sqrt(_samples.size());
}

double SampleStats::calculateMax()
{
  if (!_isPopulated(_max))
  {
    for (size_t i = 0; i < _samples.size(); i++)
    {
      _max = max(_samples[i], _max);
    }
  }
  return _max;
}

double SampleStats::calculateMean()
{
  if (!_isPopulated(_mean))
  {
    _mean = calculateSum() / (double)_samples.size();
  }
  return _mean;
}

double SampleStats::calculateMedian()
{
  if (!_isPopulated(_median))
  {
    vector<double> copy = _samples;
    std::sort(copy.begin(), copy.end());
    _q1 = copy[copy.size() / 4];
    _median = copy[copy.size() / 2];
    _q3 = copy[copy.size() * 3 / 4];
  }
  return _median;
}

double SampleStats::calculateMin()
{
  if (!_isPopulated(_min))
  {
    _min = _samples[0];
    for (size_t i = 0; i < _samples.size(); i++)
    {
      _min = min(_samples[i], _min);
    }
  }
  return _min;
}

double SampleStats::calculateUnbiasedStandardDeviation()
{
  if (!_isPopulated(_standardDeviation))
  {
    calculateMean();

    double sumSquares = 0.0;

    for (size_t i = 0; i < _samples.size(); i++)
    {
      double v = _samples[i] - _mean;
      sumSquares += v * v;
    }

    if (_samples.size() <= 1)
    {
      _standardDeviation = 0;
    }
    else
    {
      _standardDeviation = sqrt(sumSquares / (double)(_samples.size() - 1));
    }

    if (int(::qIsNaN(_standardDeviation)))
    {
      std::cout << "sd: " << _standardDeviation << endl;
      std::cout << "samples: " << _samples.size() << endl;
      std::cout << "mean: " << _mean << endl;
    }
  }
  return _standardDeviation;
}

double SampleStats::calculateSum()
{
  if (!_isPopulated(_sum))
  {
    _sum = 0.0;
    for (size_t i = 0; i < _samples.size(); i++)
    {
      _sum += _samples[i];
    }
  }

  return _sum;
}

bool SampleStats::_isPopulated(double v) const
{
  return v != -numeric_limits<double>::max();
}

QString SampleStats::toString()
{
  return QString("mean: %1 sd: %2 min: %3 max: %4 median: %5")
      .arg(calculateMean())
      .arg(calculateUnbiasedStandardDeviation())
      .arg(calculateMin())
      .arg(calculateMax())
      .arg(calculateMedian());
}

}
