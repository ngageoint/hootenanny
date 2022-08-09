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
 * @copyright Copyright (C) 2015, 2017, 2021 Maxar (http://www.maxar.com/)
 */

#include "FayyadMdlDiscretizer.h"

// Standard Includes
#include <algorithm>
#include <assert.h>
#include <iostream>
#include <math.h>
#include <set>
#include <stdio.h>
using namespace std;

#include "../HashMap.h"

namespace Tgs
{
  const double log2 = log(2.0);

  FayyadMdlDiscretizer::FayyadMdlDiscretizer()
    : _classes(NULL), _values(NULL)
  {
  }

  FayyadMdlDiscretizer::SplitPoints FayyadMdlDiscretizer::calculateSplitPoints(
    const std::vector<int>& classes, const vector<double>& values)
  {
    _values = &values;
    _classes = &classes;
    _entropyCacheDown.resize(classes.size());
    _entropyCacheUp.resize(classes.size());

    _result.clear();
    _calculateSplits(0, classes.size());
    sort(_result.begin(), _result.end());

    return _result;
  }

  double FayyadMdlDiscretizer::_calculateDelta(int start, int size, int split)
  {
    // we assume that _entropyCache* has been populated appropriately
    double totalEntropy = _entropyCacheUp[start + size - 1];
    double leftEntropy = _entropyCacheUp[split];
    double rightEntropy = 0.0;
    if (split + 1 < start + size)
    {
      rightEntropy = _entropyCacheDown[split + 1];
    }
    const vector<int>& classes = *_classes;

    set<int> leftClasses, rightClasses, totalClasses;
    for (int i = start; i < start + size; i++)
    {
      if (i <= split)
      {
        leftClasses.insert(classes[i]);
      }
      else
      {
        rightClasses.insert(classes[i]);
      }
      totalClasses.insert(classes[i]);
    }

    double k = totalClasses.size(), k1 = leftClasses.size(), k2 = rightClasses.size();
    double result = log(pow(3.0, k) - 2) / log2 -
      (k * totalEntropy - k1 * leftEntropy - k2 * rightEntropy);

    return result;
  }

  double FayyadMdlDiscretizer::_calculateEntropy(int start, int size)
  {
    assert(start + size <= (int)_classes->size());
    assert(start >= 0);
    const std::vector<int>& classes = *_classes;

    HashMap<int, int> frequency;

    HashMap<int, int>::const_iterator it;
    for (int i = start; i < start + size; i++)
    {
      it = frequency.find(classes[i]);
      if (it == frequency.end())
      {
        frequency[classes[i]] = 1;
      }
      else
      {
        int tmp = it->second;
        frequency[classes[i]] = tmp + 1;
      }
    }

    double entropy = 0.0;
    for (it = frequency.begin(); it != frequency.end(); ++it)
    {
      double proportion = (double)it->second / (double)size;
      entropy += proportion * log(proportion) / log2;
    }

    return -entropy;
  }

  void FayyadMdlDiscretizer::_calculateSplits(int start, int size)
  {
    if (size <= 1)
    {
      return;
    }

    double gain;
    int split = _findBestSplitPoint(start, size, gain);
    double delta = _calculateDelta(start, size, split);

    double stopValue = ( (log((double)size - 1.0) / log2) / (double)size ) +
      (delta / (double)size);

    if (gain <= stopValue)
    {
      return;
    }
    _result.push_back(split);
    int leftSize = split - start + 1;
    int rightSize = size - leftSize;
    _calculateSplits(start, leftSize);
    _calculateSplits(split + 1, rightSize);
  }

  int FayyadMdlDiscretizer::_findBestSplitPoint(int start, int size, double& informationGain)
  {
    assert(start + size <= (int)_classes->size());
    assert(start >= 0);
    const std::vector<double>& values = *_values;
    const std::vector<int>& classes = *_classes;

    HashMap<int, int> frequency;

    HashMap<int, int>::const_iterator it;
    int cnt = 0;
    for (int i = start; i < start + size; i++)
    {
      int seqClasses = 1;

      it = frequency.find(classes[i]);
      if (it == frequency.end())
      {
        frequency[classes[i]] = seqClasses;
      }
      else
      {
        int tmp = it->second;
        frequency[classes[i]] = tmp + seqClasses;
      }
      cnt++;

      double entropy = 0.0;
      for (it = frequency.begin(); it != frequency.end(); ++it)
      {
        double proportion = (double)it->second / (double)cnt;
        entropy += proportion * log(proportion) / log2;
      }
      _entropyCacheUp[i] = -entropy;
    }

    cnt = 0;
    frequency.clear();
    for (int i = start + size - 1; i >= start; i--)
    {
      int seqClasses = 1;

      it = frequency.find(classes[i]);
      if (it == frequency.end())
      {
        frequency[classes[i]] = seqClasses;
      }
      else
      {
        int tmp = it->second;
        frequency[classes[i]] = tmp + seqClasses;
      }
      cnt++;

      double entropy = 0.0;
      for (it = frequency.begin(); it != frequency.end(); ++it)
      {
        double proportion = (double)it->second / (double)cnt;
        entropy += proportion * log(proportion) / log2;
      }
      _entropyCacheDown[i] = -entropy;
    }

    double totalEntropy = _entropyCacheUp[start + size - 1];
    int bestSplitPoint = -1;
    double bestGain = -1.0;
    int bestSmallSide = -1;
    for (int i = start; i < start + size; i++)
    {
      // move on up to the next possible split point.
      while (i < start + size - 1 && values[i] == values[i + 1])
      {
        i++;
      }
      double leftEnt = _entropyCacheUp[i];
      double rightEnt = 0.0;
      if (i < start + size - 1)
      {
        rightEnt = _entropyCacheDown[i + 1];
      }

      double leftSize = i - start + 1;
      double rightSize = size - leftSize;
      double gain = totalEntropy -
        (rightEnt * rightSize / (double)size +
         leftEnt * leftSize / (double)size);

      int smallSide = (int)((leftSize < rightSize ? leftSize : rightSize) + .5);
      // find the split with the most information gain and the most even split
      if (gain > bestGain || (gain == bestGain && smallSide > bestSmallSide))
      {
        bestGain = gain;
        bestSplitPoint = i;
        bestSmallSide = smallSide;
      }
    }

    informationGain = bestGain;
    return bestSplitPoint;
  }
}
