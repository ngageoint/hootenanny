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
 * @copyright Copyright (C) 2015, 2017, 2019, 2021 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "DelaunayInterpolator.h"

// Qt
#include <QDataStream>

// Tgs
#include <tgs/StreamUtils.h>
#include <tgs/TgsException.h>
#include <tgs/RStarTree/HilbertRTree.h>
#include <tgs/RStarTree/KnnIteratorNd.h>
#include <tgs/Statistics/Random.h>

using namespace std;

namespace Tgs
{

DelaunayInterpolator::DelaunayInterpolator()
{
  _kFold = 10;
}

double DelaunayInterpolator::_addToResult(const Point2d& p, double w) const
{
  std::map<Point2d, int>::const_iterator it = _pointToIndex.find(p);
  assert(it != _pointToIndex.end());
  if (it->second == -1)
  {
    return 0.0;
  }

  for (size_t i = 0; i < _depColumns.size(); i++)
  {
    const vector<double>& v = _df->getDataVector(it->second);
    _result[i] += v[_depColumns[i]] * w;
//    cout << "w: " << w << " _result[i]: " << _result[i] << endl;
//    cout << "v: " << v[_depColumns[i]] << " p: " << p << endl;
  }

  return w;
}

void DelaunayInterpolator::_buildModel()
{
  // only supports 2 dimensions
  assert(_indColumns.size() == 2);

  if (_dt.get() == 0)
  {
    _dt.reset(new DelaunayTriangulation());

    const DataFrame& df = *_df;

    _minX = numeric_limits<double>::max();
    _minY = _minX;
    _maxX = -_minX;
    _maxY = -_minX;
    for (size_t i = 0; i < df.getNumDataVectors(); i++)
    {
      const vector<double>& v = df.getDataVector(i);
      Point2d p(v[_indColumns[0]], v[_indColumns[1]]);
      _pointToIndex[p] = i;
      _minX = min(p.x, _minX);
      _minY = min(p.y, _minY);
      _maxX = max(p.x, _maxX);
      _maxY = max(p.y, _maxY);
    }

    double size = max(_maxX - _minX, _maxY - _minY);
    double midX = (_maxX + _minX) / 2.0;
    double midY = (_maxY + _minY) / 2.0;

    // define way out of bounds
    _minX -= size;
    _minY -= size;
    _maxX += size;
    _maxY += size;

    Point2d a(midX - size * 5, midY - size * 3);
    Point2d b(midX + size * 5, midY - size * 3);
    Point2d c(midX, midY + size * 5);
    _pointToIndex[a] = -1;
    _pointToIndex[b] = -1;
    _pointToIndex[c] = -1;
    _dt->insert(a.x, a.y);
    _dt->insert(b.x, b.y);
    _dt->insert(c.x, c.y);

    for (size_t i = 0; i < df.getNumDataVectors(); i++)
    {
      const vector<double>& v = df.getDataVector(i);
      Point2d p(v[_indColumns[0]], v[_indColumns[1]]);
      _dt->insert(p.x, p.y);
    }
  }
}

double DelaunayInterpolator::_calculateFoldError(int fold, const vector<size_t>& indexes) const
{
  std::shared_ptr<const DataFrame> originalDf = _df;
  std::shared_ptr<DataFrame> copiedDf(new DataFrame());

  copiedDf->setFactorLabels(_df->getFactorLabels());
  copiedDf->setFactorTypes(_df->getFactorTypes());

  for (size_t i = 0; i < indexes.size(); i++)
  {
    if ((int)i % _kFold != fold)
    {
      copiedDf->addDataVector("", originalDf->getDataVector(indexes[i]));
    }
  }

  // make a new interpolator that only uses a subset of the data.
  DelaunayInterpolator uut;
  uut.setData(copiedDf);
  uut.setDependentColumns(_depColumnsLabels);
  uut.setIndependentColumns(_indColumnsLabels);

  double result = 0.0;

  for (size_t i = 0; i < indexes.size(); i++)
  {
    if ((int)i % _kFold == fold)
    {
      const vector<double>& v = originalDf->getDataVector(indexes[i]);
      const vector<double>& r = uut.interpolate(v);
      double e = 0.0;

      for (size_t j = 0; j < r.size(); j++)
      {
        double diff = fabs(r[j] - v[_depColumns[j]]);
        e += diff * diff;
      }
      result += e;
    }
  }

  return result;
}

double DelaunayInterpolator::estimateError()
{
  vector<size_t> indexes(_df->getNumDataVectors());

  for (size_t i = 0; i < _df->getNumDataVectors(); i++)
  {
    indexes[i] = i;
  }

  Random::randomizeVector<size_t>(indexes);

  double se = 0.0;
  for (int fold = 0; fold < _kFold; fold++)
  {
    double e = _calculateFoldError(fold, indexes);
    se += e;
  }

  return sqrt(se / (double)(_df->getNumDataVectors()));
}

const vector<double>& DelaunayInterpolator::interpolate(const vector<double>& point) const
{
  Point2d p(point[_indColumns[0]], point[_indColumns[1]]);

  _result.resize(_depColumns.size());
  for (size_t i = 0; i < _result.size(); i++)
  {
    _result[i] = 0.0;
  }

  // if this point is out of bounds.
  if (p.x < _minX || p.y < _minY || p.x > _maxX || p.y > _maxY)
  {
//    cout << "Using nearest neighbor." << endl;

    vector<double> simplePoint(_indColumns.size());
    for (size_t i = 0; i < _indColumns.size(); i++)
    {
      simplePoint[i] = point[_indColumns[i]];
    }

    // find the nearest neighbor
    KnnIteratorNd it(_getIndex(), simplePoint);
    if (it.next())
    {
      size_t i = it.getId();
      const vector<double>& record = _df->getDataVector(i);

      for (size_t j = 0; j < _result.size(); j++)
      {
        _result[j] += record[_depColumns[j]];
      }
    }
    else
    {
      throw Exception("Couldn't find a nearest neighbor.");
    }
  }
  else
  {
//    cout << "Using triangulation." << endl;
    Face f = _dt->findContainingFace(p.x, p.y);

    // using barycentric interpolation
    // http://classes.soe.ucsc.edu/cmps160/Fall10/resources/barycentricInterpolation.pdf
    double a1 = fabs(TriArea2(p, f.getEdge(2).getOrigin(), f.getEdge(4).getOrigin()));
    double a2 = fabs(TriArea2(p, f.getEdge(0).getOrigin(), f.getEdge(4).getOrigin()));
    double a3 = fabs(TriArea2(p, f.getEdge(0).getOrigin(), f.getEdge(2).getOrigin()));
    double aSum = 0.0;

//    cout << "a1: " << a1 << " a / aSum: " << a1 / aSum << endl;
//    cout << "a2: " << a2 << " a / aSum: " << a2 / aSum << endl;
//    cout << "a3: " << a3 << " a / aSum: " << a3 / aSum << endl;
//    cout << "aSum: " << aSum << endl;

    aSum += _addToResult(f.getEdge(0).getOrigin(), a1);
    aSum += _addToResult(f.getEdge(2).getOrigin(), a2);
    aSum += _addToResult(f.getEdge(4).getOrigin(), a3);

    for (size_t i = 0; i < _result.size(); i++)
    {
      _result[i] /= aSum;
    }

  }

  return _result;
}

void DelaunayInterpolator::_readInterpolator(QIODevice& is)
{
  QDataStream ds(&is);

  quint32 version;
  ds >> version;

  if (version != 0x0)
  {
    throw Exception("Unexpected version.");
  }

  quint32 kf;
  ds >> kf;
  _kFold = kf;
}

QString DelaunayInterpolator::toString() const
{
  return "DelaunayInterpolator";
}

void DelaunayInterpolator::_writeInterpolator(QIODevice& os) const
{
  QDataStream ds(&os);

  // write a version.
  ds << (quint32)0x0;
  ds << (quint32)_kFold;
}

}
