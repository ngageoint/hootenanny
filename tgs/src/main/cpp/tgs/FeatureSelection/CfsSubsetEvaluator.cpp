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
 * @copyright Copyright (C) 2015, 2018, 2021 Maxar (http://www.maxar.com/)
 */

/***************************************************************************
* Copyright (c) 2005-2007 by SPADAC Inc. (formerly Spatial Data Analytics Corporation).  All rights reserved.
****************************************************************************/

#include "CfsSubsetEvaluator.h"

// Standard Includes
#include <assert.h>
#include <iostream>
#include <math.h>
#include <stdio.h>
using namespace std;

#include "../Progress.h"
#include "../StreamUtils.h"
#include "../RandomForest/DataFrame.h"
#include "DataFrameDiscretizer.h"
#include "SymmetricUncertaintyCalculator.h"

namespace Tgs
{

  void CfsSubsetEvaluator::_calculateClassCorrelations(TgsProgress* progress)
  {
    _classCorr.resize(_df.getNumFactors());
    SymmetricUncertaintyCalculator suc;

    for (unsigned int f = 0; f < _df.getNumFactors(); f++)
    {
      if (progress)
      {
        progress->setProgress((double)f / (double)_df.getNumFactors());
      }
      _classCorr[f] = suc.calculateUncertainty(_df, f, _df, -1);
    }
    if (progress)
    {
      progress->setProgress(1.0);
    }
  }

  void CfsSubsetEvaluator::_calculateCorrelationMatrix(TgsProgress* progress)
  {
    _corrMatrix.resize(_df.getNumFactors());
    for (unsigned int row = 0; row < _df.getNumFactors(); row++)
    {
      _corrMatrix[row].resize(_df.getNumFactors());
    }

    int calcs = (_df.getNumFactors() * (_df.getNumFactors() + 1)) / 2;
    int current = 0;

    SymmetricUncertaintyCalculator suc;
    for (unsigned int row = 0; row < _df.getNumFactors(); row++)
    {
      _corrMatrix[row].resize(_df.getNumFactors());
      for (unsigned int col = 0; col <= row; col++)
      {
        if (row == col)
        {
          _corrMatrix[row][col] = 1.0;
        }
        else
        {
          double corr = suc.calculateUncertainty(_df, row, _df, col);
          // it is symmetric.
          _corrMatrix[row][col] = corr;
          _corrMatrix[col][row] = corr;
        }
        current++;
        if (progress)
        {
          progress->setProgress((double)current / (double)calcs);
        }
      }
    }
    if (progress)
    {
      progress->setProgress(1.0);
    }
  }

  double CfsSubsetEvaluator::evaluateSubset(const std::vector<int>& columns, 
    TgsProgress*)
  {
    double classCorrSum = 0.0;
    for (unsigned int i = 0; i < columns.size(); i++)
    {
      classCorrSum += _classCorr[columns[i]];
    }
    double classCorrMean = classCorrSum / columns.size();

    double intraFactorCorrMean;
    if (columns.size() <= 1)
    {
      intraFactorCorrMean = 1.0;
    }
    else
    {
      double intraFactorCorrSum = 0.0;
      int count = 0;
      for (unsigned int i = 0; i < columns.size(); i++)
      {
        for (unsigned int j = i + 1; j < columns.size(); j++)
        {
          intraFactorCorrSum += _corrMatrix[columns[i]][columns[j]];
          count++;
        }
      }
      intraFactorCorrMean = intraFactorCorrSum / (double)count;
    }

    double k = columns.size();

    double result = k * classCorrMean / sqrt(k + k * (k - 1) * intraFactorCorrMean);

//     printf("  result: %f k: %f classCorrMean: %f intraFactorCorrMean: %f\n", 
//       result, k, classCorrMean, intraFactorCorrMean);
    return result;
  }

  void CfsSubsetEvaluator::setDataFrame(const DataFrame& dataFrame, TgsProgress* progress)
  {
    TgsProgress* discretizeProgress = NULL;
    TgsProgress* classCorrelationProgress = NULL;
    TgsProgress* correlationMatrixProgress = NULL;
    if (progress)
    {
      discretizeProgress = progress->createTgsChild("Discretizing Data Frame", .5);
      classCorrelationProgress = progress->createTgsChild("Calculating Class Correlation", .1);
      correlationMatrixProgress = progress->createTgsChild("Calculating Correlation Matrix", .4);
    }

    _df = dataFrame;
    DataFrameDiscretizer dfd;
    dfd.discretize(_df, discretizeProgress);
    _calculateClassCorrelations(classCorrelationProgress);
    _calculateCorrelationMatrix(correlationMatrixProgress);

//     cout << "class corr: " << _classCorr << endl;
//     cout << "corr matrix: " << endl;
//     for (unsigned int i = 0; i < _corrMatrix.size(); i++)
//     {
//       cout << _corrMatrix[i] << endl;
//     }
    _df.clear();
  }

}
