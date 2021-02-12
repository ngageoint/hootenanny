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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */

#include "PrincipalComponentsAnalysis.h"

// Standard Includes
#include <assert.h>
#include <iostream>

#include <tgs/StreamUtils.h>
#include <tgs/RandomForest/DataFrame.h>

#define WANT_STREAM
#define WANT_MATH

#include <tgs/FeatureReduction/Jacobi.h>

using namespace NEWMAT;

namespace Tgs
{
  void PrincipalComponentsAnalysis::compute(DataFrame& df)
  {
    if (df.getNumFactors() > 2)
    {
      // see PrincipalComponentsAnalysisTest
      cout << "You realize this hasn't been tested, right?" << endl;
    }
    Matrix dataMat(df.getNumFactors(), df.getNumDataVectors());
    Matrix deviates(df.getNumFactors(), df.getNumDataVectors());
    SymmetricMatrix covar(df.getNumFactors());
    DiagonalMatrix eigenValues(df.getNumFactors());
    Matrix eigenVectors;
    ColumnVector means(df.getNumFactors());
    means = 0.0;
    RowVector h(df.getNumDataVectors());
    h = 1.0;

    for (unsigned int j = 0; j < df.getNumFactors(); j++)
    {
      if (df.isNominal(j))
      {
        throw Tgs::Exception("Only numeric values are supported.");
      }
    }


    for (unsigned int i = 0; i < df.getNumDataVectors(); i++)
    {
      for (unsigned int j = 0; j < df.getNumFactors(); j++)
      {
        double v = df.getDataElement(i, j);
        if (df.isNull(v))
        {
          throw Tgs::Exception("Only non-null values are supported.");
        }
        dataMat.element(j, i) = v;
        means.element(j) += v / (double)df.getNumDataVectors();
      }
    }

    try
    {
      deviates = dataMat - (means * h);
      covar << (1.0/(float)df.getNumDataVectors()) * (deviates * deviates.t());
      Jacobi::jacobi(covar, eigenValues, eigenVectors);
    }
    catch (const std::exception&)
    {
      throw;
    }
    catch (...)
    {
      throw Tgs::Exception("Unknown error while calculating PCA");
    }

    _sortEigens(eigenVectors, eigenValues);

    _components.resize(df.getNumFactors());
    for (unsigned int v = 0; v < df.getNumFactors(); v++)
    {
      _components[v].resize(df.getNumFactors());
      for (unsigned int d = 0; d < df.getNumFactors(); d++)
      {
        _components[v][d] = eigenVectors.element(d, v);
      }
    }
  }

  const std::vector<double>& PrincipalComponentsAnalysis::reprojectFromPca(
    const std::vector<double>& point)
  {
    // only supports two dimensions
    assert(_components.size() == 2);
    _tmp.resize(2);
    double x = point[0] * _components[0][0] - point[1] * _components[0][1];
    double y = point[0] * _components[0][1] + point[1] * _components[0][0];
    _tmp[0] = x;
    _tmp[1] = y;
    return _tmp;
  }

  const std::vector<double>& PrincipalComponentsAnalysis::reprojectIntoPca(
    const std::vector<double>& point)
  {
    // only supports two dimensions
    assert(_components.size() == 2);
    _tmp.resize(2);
    double x =  point[0] * _components[0][0] + point[1] * _components[0][1];
    double y = -point[0] * _components[0][1] + point[1] * _components[0][0];
    _tmp[0] = x;
    _tmp[1] = y;
    return _tmp;
  }

  void PrincipalComponentsAnalysis::_sortEigens(Matrix& eigenVectors, 
    DiagonalMatrix& eigenValues)
  {
    // simple bubble sort, slow, but I don't expect very large matrices. Lots of room for 
    // improvement.
    bool change = true;
    while (change)
    {
      change = false;
      for (int c = 0; c < eigenVectors.Ncols() - 1; c++)
      {
        if (eigenValues.element(c) < eigenValues.element(c + 1))
        {
          std::swap(eigenValues.element(c), eigenValues.element(c + 1));
          for (int r = 0; r < eigenVectors.Nrows(); r++)
          {
            std::swap(eigenVectors.element(r, c), eigenVectors.element(r, c + 1));
          }
        }
      }
    }
  }
}
