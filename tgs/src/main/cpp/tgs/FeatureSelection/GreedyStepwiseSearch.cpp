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

#include "GreedyStepwiseSearch.h"

// Standard Includes
#include <float.h>
#include <stdio.h>

#include "../Progress.h"
#include "../RandomForest/DataFrame.h"
#include "FactorSubsetEvaluator.h"

using namespace std;

namespace Tgs
{
  GreedyStepwiseSearch::GreedyStepwiseSearch()
  {
    _direction = Backward;
  }

  std::vector<std::string> GreedyStepwiseSearch::findSubset(const DataFrame& df, 
    TgsProgress* p)
  {
    TgsProgress* dataFrameProgress = NULL;
    TgsProgress* evaluateProgress = NULL;
    if (p)
    {
      dataFrameProgress = p->createTgsChild("Data Frame Processing", .5);
      evaluateProgress = p->createTgsChild("Evaluate Subsets", .5);
    }
    std::vector<std::string> result;

    _fse->setDataFrame(df, dataFrameProgress);

    assert(_direction == Backward);

    _progress = evaluateProgress;
    _iteration = 0;
    _totalIterations = (df.getNumFactors() * (df.getNumFactors() + 1)) / 2;

    double bestScore = -1e300;
    vector<int> bestSolution;
    if (_direction == Backward)
    {
      vector<int> columns;

      for (unsigned int i = 0; i < df.getNumFactors(); i++)
      {
        columns.push_back(i);
      }

      double score = _fse->evaluateSubset(columns);
      bestScore = score;
      bestSolution = columns;
//      printf("score: %.3f size: %d\n", score, columns.size());
      do
      {
        score = _removeWorst(columns);

//        printf("score: %.3f size: %d\n", score, columns.size());
        for (unsigned int i = 0; i < columns.size(); i++)
        {
          df.getFactorLabelFromIndex(columns[i]);
        }
        if (score >= bestScore)
        {
          bestScore = score;
          bestSolution = columns;
        }
      } while (columns.size() > 1);
    }

    result.clear();
    for (unsigned int i = 0; i < bestSolution.size(); i++)
    {
      string s = df.getFactorLabelFromIndex(bestSolution[i]);
      result.push_back(s);
    }

    if (p)
    {
      p->setProgress(1.0);
    }

    return result;
  }

  double GreedyStepwiseSearch::_removeWorst(std::vector<int>& columns)
  {
    double bestScore = -1e300;
    unsigned int bestIndex = 0;
    for (unsigned int i = 0; i < columns.size(); i++)
    {
      std::vector<int> subset;
      for (unsigned int j = 0; j < columns.size(); j++)
      {
        if (j != i)
        {
          subset.push_back(columns[j]);
        }
      }
      double score = _fse->evaluateSubset(subset);
//       printf("%.3f\t", score);
      if (score >= bestScore)
      {
        //printf("\nbest %f %d\n", score, i);
        bestScore = score;
        bestIndex = i;
      }

      _iteration++;
      if (_progress)
      {
        _progress->setProgress((double)_iteration / (double)_totalIterations);
      }
    }
//     printf("\n");
//     printf("%f %d\n", bestScore, bestIndex);

    std::vector<int> subset;
    for (unsigned int j = 0; j < columns.size(); j++)
    {
      if (j != bestIndex)
      {
        subset.push_back(columns[j]);
      }
    }
    columns = subset;
    return bestScore;
  }
}
