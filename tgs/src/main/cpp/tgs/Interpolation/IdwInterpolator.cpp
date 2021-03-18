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
 * @copyright Copyright (C) 2015, 2019, 2021 Maxar (http://www.maxar.com/)
 */
#include "IdwInterpolator.h"

// Qt
#include <QDataStream>

// Standard
#include <math.h>
#include <sstream>
#include <vector>

// Tgs
#include <tgs/Optimization/NelderMead.h>
#include <tgs/RandomForest/DataFrame.h>
#include <tgs/RStarTree/KnnIteratorNd.h>
#include <tgs/RStarTree/MemoryPageStore.h>
#include <tgs/Statistics/Normal.h>

namespace Tgs
{
using namespace std;

IdwInterpolator::IdwInterpolator(double p)
{
  _p = p;
  _stopDelta = 0.1;
}

class IdwOptimizeFunction : public NelderMead::Function
{
public:

  IdwOptimizeFunction(IdwInterpolator& idw) : _idw(idw)
  {

  }

  virtual double f(Vector v)
  {
    // a value less than zero isn't meaningful.
    double p = std::max(0.0, v[0]);
    _idw.setP(p);
    return -_idw.estimateError();
  }

private:

  IdwInterpolator& _idw;
};

void IdwInterpolator::_buildModel()
{
  _index.reset();

  if (_p < 0.0)
  {
    NelderMead optimizer(1, new IdwOptimizeFunction(*this), _stopDelta);
    Vector result;
    result.prepare(1);

    _p = 1.0;
    result[0] = _p;
    optimizer.step(result, -estimateError());

    _p = 4.0;
    result[0] = _p;
    optimizer.step(result, -estimateError());

    int iterations = 0;
    while (optimizer.done() == false && iterations <= _maxAllowedPerLoopOptimizationIterations)
    {
      double e = -estimateError();
      //cout << "error: " << e << " count: " << iterations << endl;
      result = optimizer.step(result, e);
      iterations++;
      _p = result[0];
    }
    if (iterations > _iterations)
    {
      _iterations = iterations;
    }
  }
}

double IdwInterpolator::_calculateWeight(double d) const
{
  return 1.0 / pow(d, _p);
}

double IdwInterpolator::_estimateError(unsigned int index) const
{
  const DataFrame& df = *_df;

  const vector<double>& uut = df.getDataVector(index);
  const vector<double>& predicted = _interpolate(uut, index);

  double result = 0.0;
  for (size_t j = 0; j < predicted.size(); j++)
  {
    double diff = uut[_depColumns[j]] - predicted[j];
    result += diff * diff;
  }
  result = sqrt(result);

  return result;
}

const vector<double>& IdwInterpolator::interpolate(const vector<double>& point) const
{
  return _interpolate(point, -1);
}

const vector<double>& IdwInterpolator::_interpolate(const vector<double>& point, int ignoreId) const
{
  const DataFrame& df = *_df;

  _result.resize(_depColumns.size());
  for (size_t i = 0; i < _result.size(); i++)
  {
    _result[i] = 0.0;
  }

  vector<double> simplePoint(_indColumns.size());
  for (size_t i = 0; i < _indColumns.size(); i++)
  {
    simplePoint[i] = point[_indColumns[i]];
  }

  KnnIteratorNd it(_getIndex(), simplePoint);
  double wSum = 0.0;
  int samples = 0;
  int iterations = 0;
  while (it.next() && samples < 50 && iterations <= _maxAllowedPerLoopOptimizationIterations)
  {
    size_t i = it.getId();
    if ((int)i == ignoreId)
    {
      continue;
    }
    const vector<double>& record = df.getDataVector(i);

    // figure out the distance between point and this data vector
    double d = 0;
    for (size_t j = 0; j < _indColumns.size(); j++)
    {
      double v = point[_indColumns[j]] - record[_indColumns[j]];
      d += v * v;
    }
    d = sqrt(d);
    // if the distance is zero then the weight is infinite and we don't need to look any further.
    if (d == 0)
    {
      wSum = 1.0;

      // Set the contribution equal to this value.
      for (size_t j = 0; j < _result.size(); j++)
      {
        _result[j] += record[_depColumns[j]];
      }
      break;
    }

    // calculate the weight of this sample.
    double w = _calculateWeight(d);
    wSum += w;

    // calculate the contribution to the predicted value.
    for (size_t j = 0; j < _result.size(); j++)
    {
      _result[j] += (record[_depColumns[j]] * w);
    }

    iterations++;
  }
  if (iterations > _iterations)
  {
    _iterations = iterations;
  }

  for (size_t j = 0; j < _result.size(); j++)
  {
    _result[j] /= wSum;
  }

  return _result;
}

void IdwInterpolator::_readInterpolator(QIODevice& is)
{
  QDataStream ds(&is);

  quint32 version;
  ds >> version;

  if (version != 0x0)
  {
    throw Exception("Unexpected version.");
  }

  ds >> _p;
  ds >> _stopDelta;
}

QString IdwInterpolator::toString() const
{
  stringstream ss;
  ss << "IDW, p: " << _p;
  return QString::fromStdString(ss.str());
}

void IdwInterpolator::_writeInterpolator(QIODevice& os) const
{
  QDataStream ds(&os);

  // write a version.
  ds << (quint32)0x0;
  ds << _p;
  ds << _stopDelta;
}

}
