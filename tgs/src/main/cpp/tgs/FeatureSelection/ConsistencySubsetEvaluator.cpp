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
 * @copyright Copyright (C) 2015, 2017 Maxar (http://www.maxar.com/)
 */

#include "ConsistencySubsetEvaluator.h"

// Standard Includes
#include <assert.h>
#include <stdio.h>
using namespace std;

#include "../HashMap.h"
#include "../Progress.h"
#include "../StreamUtils.h"
#include "../TgsException.h"
#include "../RandomForest/DataFrame.h"
#include "DataFrameDiscretizer.h"

namespace Tgs
{
  class BinHashFunctor 
    : public HashCompare< std::vector<int> >
  {
  public:

    /// Unfortunately this makes this class not thread safe, but I don't see a work around.
    static std::vector<int> _columns;

    BinHashFunctor() {}

    // hash operator
    size_t operator() (const std::vector<int>& factorBins) const
    {
      size_t hash = 0;
      for (unsigned int i = 0; i < _columns.size(); i++)
      {
        if (factorBins[_columns[i]] == -1)
        {
          hash += i * 17;
        }
        else
        {
          hash += i * 7 * factorBins[_columns[i]];
        }
      }
      return hash;
    }

    // equals operator
    bool operator() (const std::vector<int>& k1, const std::vector<int>& k2) const
    {
      for (unsigned int i = 0; i < _columns.size(); i++)
      {
        if (k1[_columns[i]] != k2[_columns[i]])
        {
          return false;
        }
      }
      return true;
    }
  };

  std::vector<int> BinHashFunctor::_columns;

  ConsistencySubsetEvaluator::ConsistencySubsetEvaluator()
    : _enumCnt(0)
  {
  }

  int ConsistencySubsetEvaluator::_calculateBin(double v, int column)
  {
    const DataFrame& df = _dataFrame;

    int result;

    if (df.isNominal(column) == true)
    {
      if (DataFrame::isNull(v))
      {
        result = -1;
      }
      else
      {
        // the bin is simply the enumerated value.
        result = (int)(v + .5);
      }
    }
    else
    {
      throw Exception("Internal Error: Shouldn't be here.");
    }
    return result;
  }

  int ConsistencySubsetEvaluator::_calculateInconsistentCount(
    const InconsistentInstancesMap& classes) const
  { 
    int count = 0;
    int max = 0;
    for (InconsistentInstancesMap::const_iterator it = classes.begin(); it != classes.end();
      ++it)
    {
      count += *it;
      max = *it > max ? *it : max;
    }
    return count - max;
  }

  double ConsistencySubsetEvaluator::evaluateSubset(const std::vector<int>& columns, 
    TgsProgress*) 
  { 
    BinHashFunctor::_columns = columns;

    const DataFrame& df = _dataFrame;
    // hash<bin key, hash<class enumeration, instance count> >
    typedef HashMap< 
      std::vector<int>, // bin key
      InconsistentInstancesMap, // map of class enumerations to instance count
      BinHashFunctor, // fancy hash function and comparison that only looks at 'columns'
      BinHashFunctor>
      BinMap;
    BinMap binMap;
    
    for (unsigned int i = 0; i < _bins.size(); i++)
    {
      //BinMap::iterator it = binMap.find(_bins[i]);
      InconsistentInstancesMap& eim = binMap[_bins[i]];
      int classEnum = _enumMap[df.getTrainingLabel(i)];
      if (eim.size() == 0)
      {
        eim.resize(_enumMap.size(), 0);
      }
      eim[classEnum]++;
    }

    int inconsistencyCount = 0;
    for (BinMap::const_iterator it = binMap.begin(); it != binMap.end(); ++it)
    {
      inconsistencyCount += _calculateInconsistentCount(it->second);
    }

    double inconsistencyRate = (double)inconsistencyCount / (double)_bins.size();
    return -inconsistencyRate;
  }

  void ConsistencySubsetEvaluator::_putDataIntoBins()
  {
    const DataFrame& df = _dataFrame;
    _bins.resize(df.getNumDataVectors());
    for (unsigned int i = 0; i < df.getNumDataVectors(); i++)
    {
      _bins[i].resize(df.getNumFactors());
      for (unsigned int j = 0; j < df.getNumFactors(); j++)
      {
        _bins[i][j] = _calculateBin(df.getDataElement(i, j), j);
      }
    }
  }

  void ConsistencySubsetEvaluator::setDataFrame(const DataFrame& dataFrame, TgsProgress* progress) 
  {
    _dataFrame = dataFrame;
    DataFrameDiscretizer dfd;
    dfd.discretize(_dataFrame, progress);

    _enumCnt = 0;
    for (unsigned int i = 0; i < _dataFrame.getNumDataVectors(); i++)
    {
      string className = _dataFrame.getTrainingLabel(i);
      if (_enumMap.find(className) == _enumMap.end())
      {
        _enumMap[className] = _enumCnt++;
      }
    }

    _putDataIntoBins();
  }

}
