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
 * @copyright Copyright (C) 2015, 2019, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#include "KernelEstimationInterpolator.h"

// Qt
#include <QDataStream>

// Standard
#include <math.h>
#include <sstream>
#include <vector>

// Tgs
#include <tgs/Optimization/NelderMead.h>
#include <tgs/RandomForest/DataFrame.h>
#include <tgs/RStarTree/HilbertRTree.h>
#include <tgs/RStarTree/KnnIteratorNd.h>
#include <tgs/Statistics/Normal.h>

namespace Tgs
{
using namespace std;

KernelEstimationInterpolator::KernelEstimationInterpolator(double sigma)
  : _sigma(sigma),
    _stopDelta(1.0)
{
}

class OptimizeFunction : public NelderMead::Function
{
public:

  OptimizeFunction(KernelEstimationInterpolator& kei) : _kei(kei) { }

  double f(Vector v) override
  {
    _kei.setSigma(std::max(0.1, v[0]));
    return -_kei.estimateError();
  }

private:
  KernelEstimationInterpolator& _kei;
};

void KernelEstimationInterpolator::_buildModel()
{
  const DataFrame& df = *_df;

  _index.reset();

  if (_sigma < 0)
  {
    // calculate the standard deviation in x
    double mean = 0;
    unsigned int n = df.getNumDataVectors();
    for (unsigned int i = 0; i < n; ++i)
      mean += df.getDataVector(i)[_indColumns[0]];
    mean /= df.getNumDataVectors();

    double sumDiff = 0;
    for (unsigned int i = 0; i < n; ++i)
    {
      double v = df.getDataVector(i)[_indColumns[0]];
      sumDiff += (v - mean) * (v - mean);
    }

    double sdx = sqrt(1.0 / static_cast<double>(n - 1) * sumDiff);

    // calculate a reasonable starting point w/ silverman's rule of thumb. Put a minimum at 1m to
    // prevent some edge conditions.
    double silvermans = max(1.0, 1.06 * sdx * pow(n, -.2));

    NelderMead optimizer(1, std::make_shared<OptimizeFunction>(*this), _stopDelta);
    Vector result;
    result.prepare(1);

    // silverman's rule of thumb tends to over estimate and we're faster at evaluating smaller sigma
    // so start with two smallish values to seed nelder-mead.
    _sigma = silvermans * 0.6;
    result[0] = _sigma;
    optimizer.step(result, -estimateError());

    _sigma = silvermans * 0.2;
    result[0] = _sigma;
    optimizer.step(result, -estimateError());

    int iterations = 0;
    while (optimizer.done() == false && iterations <= _maxAllowedPerLoopOptimizationIterations)
    {
      double e = -estimateError();
      result = optimizer.step(result, e);
      iterations++;
      _sigma = result[0];
    }
    if (iterations > _iterations)
      _iterations = iterations;
  }
}

double KernelEstimationInterpolator::_estimateError(unsigned int index) const
{
  const DataFrame& df = *_df;

  vector<double> predicted(_depColumns.size(), 0.0);
  const vector<double>& uut = df.getDataVector(index);
  vector<double> simplePoint(_indColumns.size());
  for (size_t i = 0; i < _indColumns.size(); i++)
    simplePoint[i] = uut[_indColumns[i]];

  double n0 = Normal::normal(0, _sigma);

  KnnIteratorNd it(_getIndex(), simplePoint);
  double wSum = 0.0;
  int iterations = 0;
  while (it.next() && it.getDistance() < _sigma * 3.0 &&
         iterations <= _maxAllowedPerLoopOptimizationIterations)
  {
    unsigned int i = it.getId();
    if (i == index)
      continue;
    const vector<double>& record = df.getDataVector(i);

    // figure out the distance between point and this data vector
    double d = 0;
    for (auto col : _indColumns)
    {
      double v = uut[col] - record[col];
      d += v * v;
    }
    d = sqrt(d);
    if (d / _sigma < 3.0)
    {
      // calculate the weight of this sample.
      double w = Normal::normal(d, _sigma) / n0;
      wSum += w;

      assert(w <= 1.000001);

      // calculate the contribution to the predicted value.
      for (size_t j = 0; j < predicted.size(); j++)
        predicted[j] += (record[_depColumns[j]] * w);
    }

    iterations++;
  }
  // do less rubber sheeting as we get far away from tie points.
  wSum = std::max(wSum, n0);

  double result = 0.0;
  for (size_t j = 0; j < predicted.size(); j++)
  {
    double diff = uut[_depColumns[j]] - (predicted[j] / wSum);
    result += diff * diff;
  }
  result = sqrt(result);

  return result;
}

const vector<double>& KernelEstimationInterpolator::interpolate(const vector<double>& point) const
{
  const DataFrame& df = *_df;

  _result.resize(_depColumns.size());
  for (size_t i = 0; i < _result.size(); i++)
    _result[i] = 0.0;

  vector<double> simplePoint(_indColumns.size());
  for (size_t i = 0; i < _indColumns.size(); i++)
    simplePoint[i] = point[_indColumns[i]];

  double n0 = Normal::normal(0, _sigma);

  KnnIteratorNd it(_getIndex(), simplePoint);
  double wSum = 0.0;
  int iterations = 0;
  while (it.next() && it.getDistance() < _sigma * 3.0 &&
         iterations <= _maxAllowedPerLoopOptimizationIterations)
  {
    unsigned int i = it.getId();
    const vector<double>& record = df.getDataVector(i);

    // figure out the distance between point and this data vector
    double d = 0;
    for (auto col : _indColumns)
    {
      double v = point[col] - record[col];
      d += v * v;
    }
    d = sqrt(d);
    if (d / _sigma < 3.0)
    {
      // calculate the weight of this sample.
      double w = Normal::normal(d, _sigma) / n0;
      wSum += w;

      assert(w <= 1.000001);

      // calculate the contribution to the predicted value.
      for (size_t j = 0; j < _result.size(); j++)
        _result[j] += (record[_depColumns[j]] * w);
    }

    iterations++;
  }
  // do less rubber sheeting as we get far away from tie points.
  wSum = std::max(wSum, n0);

  for (size_t j = 0; j < _result.size(); j++)
    _result[j] /= wSum;

  return _result;
}

void KernelEstimationInterpolator::_readInterpolator(QIODevice& is)
{
  QDataStream ds(&is);

  quint32 version;
  ds >> version;

  if (version != 0x0)
    throw Exception("Unexpected version.");

  ds >> _sigma;
  ds >> _stopDelta;
}

QString KernelEstimationInterpolator::toString() const
{
  stringstream ss;
  ss << "Kernel estimation, sigma: " << _sigma;
  return QString::fromStdString(ss.str());
}

void KernelEstimationInterpolator::_writeInterpolator(QIODevice& os) const
{
  QDataStream ds(&os);

  // write a version.
  ds << (quint32)0x0;
  ds << _sigma;
  ds << _stopDelta;
}

}
