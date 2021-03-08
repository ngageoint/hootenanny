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
 * @copyright Copyright (C) 2015, 2017, 2019, 2021 Maxar (http://www.maxar.com/)
 */
#include "BaseInterpolator.h"

// Qt
#include <QByteArray>
#include <QDataStream>
#include <QString>

// Standard
#include <cassert>
#include <cmath>
#include <sstream>

// Tgs
#include <tgs/StreamUtils.h>
#include <tgs/RandomForest/DataFrame.h>
#include <tgs/RStarTree/HilbertRTree.h>
#include <tgs/RStarTree/MemoryPageStore.h>

using namespace std;

namespace Tgs
{

BaseInterpolator::BaseInterpolator() :
_maxAllowedPerLoopOptimizationIterations(INT_MAX),
_iterations(0)
{
}

void BaseInterpolator::_checkRebuild()
{
  if (_indColumns.size() > 0 && _depColumns.size() > 0 && _df.get() != 0)
  {
    _buildModel();
  }
}

double BaseInterpolator::estimateError()
{
  double se = 0.0;
  for (size_t i = 0; i < _df->getNumDataVectors(); i++)
  {
    double e = _estimateError(i);
    se += e * e;
  }
  return sqrt(se / (double)(_df->getNumDataVectors()));
}

HilbertRTree* BaseInterpolator::_getIndex() const
{
  if (_index.get() == 0)
  {
    const DataFrame& df = *_df;
    // 8 children was picked experimentally with two dimensions.
    std::shared_ptr<MemoryPageStore> mps(new MemoryPageStore(
      BoxInternalData::size(_indColumns.size()) * 8 + sizeof(int) * 4));
    _index.reset(new HilbertRTree(mps, _indColumns.size()));

    vector<Box> boxes(df.getNumDataVectors());
    vector<int> fids(df.getNumDataVectors());

    for (size_t i = 0; i < df.getNumDataVectors(); i++)
    {
      fids[i] = i;
      boxes[i] = Box(_indColumns.size());
      for (size_t j = 0; j < _indColumns.size(); j++)
      {
        double v = df.getDataVector(i)[_indColumns[j]];
        boxes[i].setBounds(j, v, v);
      }
    }

    _index->bulkInsert(boxes, fids);
  }

  return _index.get();
}

void BaseInterpolator::readInterpolator(QIODevice& is)
{
  QDataStream ds(&is);

  // read the version
  quint32 version;
  ds >> version;

  if (version != 0x0)
  {
    throw Exception("Unexpected version.");
  }

  quint64 s;
  ds >> s;
  _indColumns.resize(s);
  _indColumnsLabels.resize(s);
  for (size_t i = 0; i < _indColumns.size(); i++)
  {
    ds >> _indColumns[i];
    QString str;
    ds >> str;
    _indColumnsLabels[i] = str.toStdString();
  }

  ds >> s;
  _depColumns.resize(s);
  _depColumnsLabels.resize(s);
  for (size_t i = 0; i < _depColumns.size(); i++)
  {
    ds >> _depColumns[i];
    QString str;
    ds >> str;
    _depColumnsLabels[i] = str.toStdString();
  }

  QByteArray qb;
  ds >> qb;
  string str = QString::fromUtf8(qb.constData()).toStdString();
  stringstream ss(str);
  std::shared_ptr<DataFrame> df(new DataFrame());
  df->import(ss);
  _df = df;

  // read from child class.
  _readInterpolator(is);

  _buildModel();
}

void BaseInterpolator::setData(const std::shared_ptr<const DataFrame>& df)
{
  _df = df;
  _checkRebuild();
}

void BaseInterpolator::setDependentColumns(const vector<string>& labels)
{
  assert(_df != 0);
  _depColumnsLabels = labels;
  _depColumns.resize(labels.size());
  for (size_t i = 0; i < _depColumns.size(); i++)
  {
    _depColumns[i] = _df->getIndexFromFactorLabel(labels[i]);
  }
  _checkRebuild();
}

void BaseInterpolator::setIndependentColumns(const vector<string>& labels)
{
  assert(_df != 0);
  _indColumnsLabels = labels;
  _indColumns.resize(labels.size());
  for (size_t i = 0; i < _indColumns.size(); i++)
  {
    _indColumns[i] = _df->getIndexFromFactorLabel(labels[i]);
  }
  _checkRebuild();
}

void BaseInterpolator::writeInterpolator(QIODevice& os) const
{
  QDataStream ds(&os);

  // write the version
  ds << (quint32)0x0;

  ds << (quint64)_indColumns.size();
  for (size_t i = 0; i < _indColumns.size(); i++)
  {
    ds << _indColumns[i];
    ds << QString::fromStdString(_indColumnsLabels[i]);
  }

  ds << (quint64)_depColumns.size();
  for (size_t i = 0; i < _depColumns.size(); i++)
  {
    ds << _depColumns[i];
    ds << QString::fromStdString(_depColumnsLabels[i]);
  }

  stringstream ss;
  _df->exportData(ss);
  QByteArray arr = QString::fromStdString(ss.str()).toUtf8();
  ds << arr;

  // write from child class.
  _writeInterpolator(os);
}

}
